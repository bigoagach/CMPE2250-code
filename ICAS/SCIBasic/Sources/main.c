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
//uppercase or lowercase
UpperOrLower letterMode = Lower;
/********************************************************************/
// Constants
/********************************************************************/
//escape sequence for moving cursor to next line (cursor down)
char const nextLine[] = "\x1B[1B";
//#define StudyCode
/********************************************************************/
// Main Entry
/********************************************************************/
void main(void)
{
  // main entry point
  _DISABLE_COP();
  //EnableInterrupts;
  
  /********************************************************************/
  // initializations
  /********************************************************************/
  //20[MHz] (50[ns]) clock speed
  Clock_Set20MHZ();
  //Setting up PIT to coodrdinate and control elements of program, 50[us]
  //(void)PIT_Init(PIT_Channel_0, PIT_Interrupt_On, 20E6, 50);
  //starting up SCI with 9600 BAUD, interrupts off
  //(void)sci0_Init(20E6, 9600, 0);
  SCI0BD = 130;
  SCI0CR2 = 0b00001100;
  //setting up LCD and Switch/LEDs
  lcd_Init();
  SwLED_Init();
  /********************************************************************/
  // main program loop
  /********************************************************************/
  for (;;)
  {
    //asm wai;

    sci0_txByte('A');
  }                   
}

/********************************************************************/
// Functions
/********************************************************************/
void PrintLetters(unsigned char start, UpperOrLower mode) 
{
  int i;
  if(!mode) {
    for(i = 'a'; i < 'z' + 1; i++) 
      sci0_txByte((unsigned char)i);
    sci0_txStr(nextLine);
  }
  else {
    for(i = start - 32; i < 'Z' + 1; i++) 
      sci0_txByte((unsigned char)i);
    sci0_txStr(nextLine);
  }
}
/********************************************************************/
// Interrupt Service Routines
/********************************************************************/
// interrupt VectorNumber_Vpit0 void PIT0Int (void)
// {
//   //counts to 1 second
//   static volatile unsigned int count = 0;
//   //starting letter, initialized to 'a'
//   unsigned char startingPoint = 'a';
//   //character received
//   unsigned char received;

//   // clear flag
//   PITTF = PITTF_PTF0_MASK; // can't read - clears other flags, write only

//   // //take action!
//   // count++;
//   // if(count == 20000) {
//   //   //toggle the from uppercase to lowercase or lowercase to uppercase
//   //   letterMode ^= 1; 
//   //   //reset count
//   //   count = 0;
//   // }

//   // if(sci0_read(&received)) {
//   //   //if user types aan alphabetic character
//   //   if((received >= 'A' && received <= 'Z') || (received >= 'a' && received <= 'z'))
//   //     startingPoint = received;
//   //   LED_Tog(LED_RED);
//   // }

//   // //print alphabet
//   // PrintLetters('a', letterMode);
// }

#ifdef StudyCode

/*

ICA 07

Thomas Feraco
CMPE 2550 Fall 2022

*/

/********************************************************************/

/********************************************************************/
// Library includes
/********************************************************************/
// your includes go here

#include /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */
#include "SwLed.h"
//#include "misc.h"
#include #include "Clock.h"
#include "pit.h"
#include "ECT.h"
#include "SevSeg.h"
#include #include "sci.h"
#include "lcd.h"

/********************************************************************/
// Local Prototypes
/********************************************************************/

/********************************************************************/
// Global Variables
/********************************************************************/

volatile unsigned short count = 0;
volatile unsigned short countFlag = 0;

/********************************************************************/
// Constants
/********************************************************************/

/********************************************************************/
// Main Entry
/********************************************************************/


void main(void)
{
  unsigned char dInC = 'A';
  unsigned char dIn = 'A';
  unsigned char data = 'A';
  unsigned short offsetAlpha = 0;
  unsigned char startAlpha = 'A';

  count = 0;
  countFlag = 0;

  // main entry point
  _DISABLE_COP(); // Stop watchdog
  EnableInterrupts;

  /********************************************************************/
  // initializations
  /********************************************************************/
  Clock_Set20MHZ(); // set clock to 20MHZ

  Timer_Init(Timer_Prescale_32);
  //TimerCH_EnableAsOutput(Timer_CH0_BIT);
  TimerCH_EnableAsOutput(Timer_CH1_BIT); // for sleep_ms

  SwLED_Init();
  SevSeg_Init();
  lcd_Init();

  if(PIT_Init (PIT_Channel_0, PIT_Interrupt_On, 20E6, 100000) == 0)
  {
    LED_Tog(LED_GREEN);
  }

  (void)sci0_Init(20E6, 9600, 0);


  /********************************************************************/
  // main program loop
  /********************************************************************/


  /*

  if(startAlpha + offsetAlpha == 'Z' || startAlpha + offsetAlpha == 'z')
  {
  offsetAlpha = 0;
  }

  if(countFlag == 1)
  {
  sci0_txByte(startAlpha + offsetAlpha + 32); //32 is offset for lowercase -> uppercase
  }

  else
  {
  sci0_txByte(startAlpha + offsetAlpha);
  }

  */

  for (;;)
  {
    if(countFlag == 1)
    {
      sci0_txByte(data+32);
    }

    else
    {
      sci0_txByte(data);
    }

    if(data == 'Z' || data == 'z')
    {
      data = dIn-1;
      sci0_txStr("\r\n");
    }

    ++data;

    dInC = dIn;
    //sci0_txByte('A');
    //SevSeg_Top4(sci0_bread());
    if(sci0_read(&dIn) == 1)// good
    {
      SevSeg_Top4((int)dIn);
    }

    if(dIn > 96)
    {
      dIn -= 32;
    }

    if(dIn < 65 || dIn > 90)
    {
      dIn = dInC;
    }



    ++offsetAlpha;
  }
}

/********************************************************************/
// Functions
/********************************************************************/

/********************************************************************/
// Interrupt Service Routines
/********************************************************************/

interrupt VectorNumber_Vpit0 void PIT0Int (void)
{
  LED_Tog(LED_RED);

  PITTF = PITTF_PTF0_MASK;

  ++count;
  if(count >= 10)
  {
    if (countFlag == 0)
    {
      countFlag = 1;
      count = 0;
    }
    else
    {
      countFlag = 0;
      count = 0;
    }
  }
}

#endif