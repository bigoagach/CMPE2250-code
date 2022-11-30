/********************************************************************/
// HC12 Program:  CMPE2250 - SCI Basics
// Processor:     MC9S12XDP512
// Bus Speed:     8 MHz
// Author:        Bigoa Gach
// Details:       A more detailed explanation of the program is entered here
                  
// Date:          20022-11-15
// Revision History :
//  each revision will have a date + desc. of changes

/********************************************************************/
// Constant Defines
/********************************************************************/
//print mode (uppercase or lowercase alphabet)
typedef enum UpperOrLower { Lower, Upper } UpperOrLower;
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
#include "sci.h"    /* SCI Library */
/********************************************************************/
// Local Prototypes
/********************************************************************/
//prints the letters onto the terminal
void PrintLetters(unsigned char start);
//displaying the number of characters sent on the lower line of
//the seven segment and the number of main loop iterations on the
//upper line of the seven segment; both values in HEX; 
void PrintSegmentInfo(void);
//displaying loop to character ratio and the character sent
void PrintLCDInfo(unsigned char receivedChar);
/********************************************************************/
// Global Variables
/********************************************************************/
//uppercase or lowercase
volatile UpperOrLower letterMode = Lower;
//flag to allow lcd update
volatile char flag;
//current letter
unsigned char letter = 'a';
//number of characters sent
unsigned long numOfChars = 0;
//number of main loop interations
unsigned long mainLoops = 0;
/********************************************************************/
// Constants
/********************************************************************/
//escape sequence for moving cursor to next line (cursor down)
char const nextLine[] = "\r\n";
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
  //20[MHz] (50[ns]) clock speed
  Clock_Set20MHZ();
  //Setting up PIT to coodrdinate and control elements of program, 100[ms]
  (void)PIT_Init(PIT_Channel_0, PIT_Interrupt_On, 20E6, 100000);
  //starting up SCI with 9600 BAUD, interrupts off
  (void)sci0_Init(20E6, 9600, 0);
  //setting up LCD, Switches/LEDs, and SevSeg
  lcd_Init();
  lcd_DispControl(0, 0);
  SwLED_Init();
  SevSeg_Init();
  /********************************************************************/
  // main program loop
  /********************************************************************/
  lcd_StringXY(0, 0, "Loops/Chars Ratio:");
  lcd_StringXY(3, 0, "RX: x :");

  for (;;)
  {
    //starting letter, initialized to 'a'
    static unsigned char startingPoint = 'a';
    //character received
    static unsigned char received;

    //check for received characters
    if(sci0_read(&received)) {
      //if user types an alphabetic character
      if(received >= 'a' && received <= 'z')
        startingPoint = received;
      if(received >= 'A' && received <= 'Z')
        startingPoint = received + 32;
      LED_Tog(LED_RED);
    }

    mainLoops++;

    //print alphabet and increment
    PrintLetters(startingPoint);

    //display counts of sevseg
    PrintSegmentInfo();

    //updating the lcd every 500[ms]
    if(flag) {
      PrintLCDInfo(received);
      flag = 0;
    }
  }                   
}

/********************************************************************/
// Functions
/********************************************************************/
void PrintLetters(unsigned char start) 
{
  if(letter > 'z') {          //wrapping letter value count at 'z'
    letter = start;           //reseting letter value count
    sci0_txStr(nextLine);     //moving cursor to the next line
  }

  if(!letterMode) {           //if the letter mode is "Lower"
    if(sci0_txNonByte(letter)) { //displaying current character
      numOfChars++;
      letter++;
    }
  }
  else {                      //if the letter mode is "Upper"
    if(sci0_txNonByte(letter - 32)) { //displaying current character
      numOfChars++;
      letter++;
    }
  }
  
}

void PrintSegmentInfo(void) 
{
  SevSeg_Bot4((unsigned int)numOfChars);
  SevSeg_Top4((unsigned int)mainLoops);
}

void PrintLCDInfo(unsigned char receivedChar) 
{
  //displaying the received character on the last line of the lcd 
  { char display[21];
    (void)sprintf(display, "%2X", receivedChar);
    lcd_StringXY(3, 8, display); }
  //displaying the loop to chars ratio on the second line of the lcd
  //the ratio is around 16.2~ but the ratio will drastically change
  //when the mainLoop count reaches past 2^32 - 1 and wraps back to 0
  { char display[21];
    (void)sprintf(display, "%04.2f", (float)mainLoops / numOfChars);
    lcd_StringXY(1, 0, display); }
}
/********************************************************************/
// Interrupt Service Routines
/********************************************************************/
interrupt VectorNumber_Vpit0 void PIT0Int (void)
{
  //counts to 1 second
  static volatile unsigned int count = 0;

  // clear flag
  PITTF = PITTF_PTF0_MASK; // can't read - clears other flags, write only

  //take action!
  count++;

  //when the count reaches 1.00[s]
  if(count == 10) {
    //toggle the from uppercase to lowercase or lowercase to uppercase
    letterMode = letterMode ? Lower : Upper;
    //reset count
    count = 0;
  }

  //when the count reaches 500[ms]
  if(count % 5 == 0) {
    //setting flag
    flag = 1;
  }
}
