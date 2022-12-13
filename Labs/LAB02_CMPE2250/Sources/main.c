/********************************************************************/
// HC12 Program:  CMPE2250 - Lab02
// Processor:     MC9S12XDP512
// Bus Speed:     8 MHz
// Author:        Bigoa Gach
// Details:       using the SCI module to show some statistics 
//                on bytes that have been received
                  
// Date:          Date Created
// Revision History :
//  each revision will have a date + desc. of changes
// FOR NEXT TIME : fix the 'on' bit count because it only adds the
// number of 1 bits in the last bit received;
// fix the count of bytes received to reset after each loading state

/********************************************************************/
// Constant Defines
/********************************************************************/
//is the state machine in a waiting state or a lodaing state?
typedef enum State { Waiting, Loading } State;
/********************************************************************/
#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */
/********************************************************************/
// Library includes
/********************************************************************/
// your includes go here
#include <stdio.h>  /* for the sprintf funtion */
#include "SwLed.h"  /* Switch/LED Library */
#include "ECT.h"    /* Timer Library */
#include "Clock.h"  /* Clock Library */
#include "SevSeg.h" /* 7-Segment Library */
#include "lcd.h"    /* LCD Library */
#include "portj.h"  /* Port J Library */
#include "pit.h"    /* PIT Timer Library */
#include "pwmlib.h" /* PWM Library */
#include "sci.h"    /* SCI Library*/
/********************************************************************/
// Local Prototypes
/********************************************************************/
//toggles Red LED every 100[ms] and turns on Green LED while
//waiting for data byte to be received
void WaitingState(void);
//turns off all LEDs, tracks the number of bytes received,
//the highest value received, the lowest value received and
//the number of bits in the byte value that are '1';
//the Loading State will be active for only 2.000[s]
void LoadingState(void);
//updating the statistics on the LCD
void UpdateLCD(void);
/********************************************************************/
// Global Variables
/********************************************************************/
//current state of the machines, intialized to Waiting
volatile State currentState = Waiting;
//flag indicating visit from PIT ISR
volatile int pitFlag = 0;
//character received
volatile unsigned char data;
//number of bytes received
volatile unsigned int numReceieved = 0;
//the lowest value received
unsigned char lowest = 255;
//the highest value received
unsigned char highest = 0;
//number of 'on' bits in the data received
unsigned int bitsOn = 0;
//for counting to 2.000[s]
volatile int timeCount = 0;
/********************************************************************/
// Constants
/********************************************************************/

/********************************************************************/
// Main Entry
/********************************************************************/
void main(void)
{
  // main entry point
  _DISABLE_COP();
  EnableInterrupts;
  
  /********************************************************************/
  // initializations
  /********************************************************************/
  Clock_Set20MHZ();
  //setting up PIT channel 0 with interval of 100[ms]
  (void)PIT_Init(PIT_Channel_0, PIT_Interrupt_On, 20E6, 100000);
  //intializing the SCI for a BAUD rate of 38400, interrupts enabled
  (void)sci0_Init(20E6, 38400, 1);
  //intializing the LCD, segs, and Switch/LEDS
  lcd_Init();
  SevSeg_Init();
  SwLED_Init();
  //turning off lcd cursor and blinking
  lcd_DispControl(0, 0);
  /********************************************************************/
  // main program loop
  /********************************************************************/
  lcd_StringXY(0, 0, "--Load Some Bytes!--"); 
  lcd_StringXY(1, 0, "# of bytes: X");
  lcd_StringXY(2, 0, "high: X");
  lcd_StringXY(2, 11, "low: X");
  lcd_StringXY(3, 0, "# of 1 bits: X");
  for (;;)
  {
    asm wai;

    //if we are currently waiting, call the WaitingState
    //method and skip the rest of the main loop code
    
    if(!currentState) {
      WaitingState();
      continue;
    }

    //if we are currently loading, increment the count
    //value for only 2.000[s], call the LoadingState
    //method; when 2.000[s] has passed, reset count and
    //go back into the waiting state

    LoadingState();

    timeCount++;
    //if 2.000[s] have passed:
    if(timeCount > 20) {
      //reseting count
      timeCount = 0;
      UpdateLCD();
      //resetting the lowest and highest values, and the 'on' bit count
      lowest = 255;
      highest = 0;
      bitsOn = 0;
      numReceieved = 0;
      //switching state back to Waiting
      currentState = Waiting; 
    }
  }                   
}

/********************************************************************/
// Functions
/********************************************************************/
void WaitingState(void) 
{
  LED_On(LED_GREEN);
  if(pitFlag) {
    LED_Tog(LED_RED);
    pitFlag = 0;
  }
}

void LoadingState(void)
{
  if(pitFlag) {
    //updating LCD every 100[ms]
    lcd_StringXY(0, 0, "----loading---------");
    pitFlag = 0;
  }

  LED_Off(LED_RED|LED_GREEN);
}

void UpdateLCD(void) {
  //updating LCD every 100[ms]
  lcd_StringXY(0, 0, "-----STATISTICS-----"); 
  { char dis[21];
    (void)sprintf(dis, "# of bytes: %X    ", HexToBCD(numReceieved));
    lcd_StringXY(1, 0, dis); }
  { char dis[21];
    (void)sprintf(dis, "high: %2.2X  ", highest);
    lcd_StringXY(2, 0, dis); }
  { char dis[21];
    (void)sprintf(dis, "low: %2.2X  ", lowest);
    lcd_StringXY(2, 11, dis); }
  { char dis[21];
    (void)sprintf(dis, "# of 1 bits: %X    ", HexToBCD(bitsOn));
    lcd_StringXY(3, 0, dis); }
}
/********************************************************************/
// Interrupt Service Routines
/********************************************************************/
interrupt VectorNumber_Vpit0 void PIT0Int (void)
{
  //clear flag
  PITTF = PITTF_PTF0_MASK; //can't read - clears other flags, write only
  //setting flag to 1 to indicate a visit from this ISR
  pitFlag = 1;
}

interrupt VectorNumber_Vsci0 void ISR_SCI0(void) 
{
  //switching state when byte is received
  unsigned char readData = data;

  if(sci0_read(&readData)) {
    //counting the number of 'on' bits
    int i;
    //if not currently in the loading state, go to loading state
    if(!currentState)
      currentState = Loading; 
    //received byte
    data = readData;
    //incrementing the number of characters received
    numReceieved++;
    //resetting timeout
    timeCount = 0;
    //setting the lowest and highest values
    if(data < lowest)
      lowest = data;
    if(data > highest)
      highest = data;

    for(i = 0; i < 8; i++) {
      if(data & (1 << i))
        bitsOn++;
    }
  }
}