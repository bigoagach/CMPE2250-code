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
void PrintLetters(unsigned char start, UpperOrLower mode);
/********************************************************************/
// Global Variables
/********************************************************************/
//character received
volatile unsigned char received;
//uppercase or lowercase
UpperOrLower letterMode = Lower;
/********************************************************************/
// Constants
/********************************************************************/
//escape sequence for moving cursor to next line (cursor down)
char const nextLine[] = "\x1B[1B";
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
  //Setting up PIT to coodrdinate and control elements of program, 50[us]
  PIT_Init(PIT_Channel_0, PIT_Interrupt_On, 20E6, 50);
  //starting up SCI with 9600 BAUD, interrupts off
  (void)sci0_Init(20E6, 9600, 0);
  //setting up LCD and Switch/LEDs
  lcd_Init();
  SwLED_Init();
  /********************************************************************/
  // main program loop
  /********************************************************************/
  for (;;)
  {
    asm wai;
  }                   
}

/********************************************************************/
// Functions
/********************************************************************/
void PrintLetters(unsigned char start, UpperOrLower mode) 
{
  int i;
  if(!mode) {
    for(i = start; i < 'z' + 1, i++) 
      sci0_txByte((unsigned char)i);
    sci0_txStr(nextLine);
  }
  else {
    for(i = start - 32; i < 'Z' + 1, i++) 
      sci0_txByte((unsigned char)i);
    sci0_txStr(nextLine);
  }

}
/********************************************************************/
// Interrupt Service Routines
/********************************************************************/
interrupt VectorNumber_Vpit0 void PIT0Int (void)
{
  //counts to 1 second
  static volatile unsigned int count = 0;
  //starting letter, initialized to 'a'
  unsigned char startingPoint = 'a';

  // clear flag
  PITTF = PITTF_PTF0_MASK; // can't read - clears other flags, write only

  // take action!
  count++;
  if(count = 20000) {
    //toggle the from uppercase to lowercase or lowercase to uppercase
    letterMode ^= 1; 
    //reset count
    count = 0;
  }

  if(sci0_read(&received)) {
    //if user types aan alphabetic character
    if((received >= 'A' && received <= 'Z') || (received >= 'a' && received <= 'z'))
      startingPoint = received;
    LED_Tog(LED_RED);
  }

  //print alphabet
  PrintLetters(startingPoint, letterMode);

}