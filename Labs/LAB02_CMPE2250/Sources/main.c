/********************************************************************/
// HC12 Program:  CMPE2250 - Lab02
// Processor:     MC9S12XDP512
// Bus Speed:     8 MHz
// Author:        Bigoa Gach
// Details:       A more detailed explanation of the program is entered here
                  
// Date:          Date Created
// Revision History :
//  each revision will have a date + desc. of changes

/********************************************************************/
// Constant Defines
/********************************************************************/
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
/********************************************************************/
// Global Variables
/********************************************************************/
//current state of the machines, intialized to Waiting
volatile State currentState = Waiting;
//flag indicating visit from PIT ISR
volatile int pitFlag = 0;
//character received
unsigned char data;
//number of bytes received
volatile unsigned int numReceieved = 0;
/********************************************************************/
// Constants
/********************************************************************/

/********************************************************************/
// Main Entry
/********************************************************************/
void main(void)
{
  //for counting to 2.000[s]
  int count = 0;

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
  //displaying text on lcd
  lcd_StringXY(1, 0, "# of bytes: ");
  lcd_StringXY(2, 0, "highest: ");
  lcd_StringXY(2, 11, "lowest: ");
  lcd_StringXY(3, 0, "# of 1 bits: ");

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

    count++;
    if(count > 20) {
      //reseting count
      count = 0;
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
    //updating LCD every 100[ms]
    lcd_StringXY(0, 0, "-----STATISTICS-----"); 
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

  if(currentState) 
  {


  }
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
  if(sci0_read(&data)) {
    currentState = Loading; 
  }
}