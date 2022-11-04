/********************************************************************/
// HC12 Program:  CMPE2250 - Interrupts with the Timer
// Processor:     MC9S12XDP512
// Bus Speed:     8 MHz
// Author:        Bigoa Gach
// Details:       Will explore setting up a timer interrupt to
//                to characterize some interrupt behaviours
                  
// Date:          2022-09-26
// Revision History :
//  each revision will have a date + desc. of changes

/********************************************************************/
// Constant Defines
/********************************************************************/

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
/********************************************************************/
// Local Prototypes
/********************************************************************/

/********************************************************************/
// Global Variables
/********************************************************************/
volatile long eventCount; //counts each interrupt event
long loopInters;          //counts each loop in the main program loop
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

  //enabling CH0 with a prescale of two, with an
  //interval of 5[ms]; enabling CH0 as an interrrupt
  Timer_Init(Timer_Prescale_128);
  TimerCH_EnableAsOutput(Timer_CH0_BIT, 781);
  TimerCH_IntEnable(Timer_CH0_BIT);
  
  
  //initializing switches, LEDs,the LCD, & 7-segment display
  SwLED_Init();
  lcd_Init();
  SevSeg_Init();

  eventCount = 0;
  loopInters = 0;
  /********************************************************************/
  // main program loop
  /********************************************************************/
  lcd_StringXY(0, 0, "Ticks:");
  lcd_StringXY(1, 0, "Loops:");
  lcd_StringXY(3, 0, "B. Gach");

  for (;;)
  {
    //sychronizes the main loop to the interrupt,
    //has the main loop wait for the interrupt event
    //before continuing (or rather starting)
    //without the Sleep calls, the interrupt counter
    //and main loop counter are identical
    asm wai; 

    ++loopInters;
    LED_Tog(LED_GREEN);

    //transforming integer counters into strings for the
    //sake of dislaying them counts on the LCD
    {
      char displayCount[8];     //array for displaying the interrupt count
      char displayLoops[8];     //array for displaying the main loop count
      (void)sprintf(displayCount, "%lu", eventCount); 
      (void)sprintf(displayLoops, "%lu", loopInters);
      lcd_StringXY(0, 20 - strlen(displayCount), displayCount);
      lcd_StringXY(1, 20 - strlen(displayLoops), displayLoops);
    }
  
    //sleep call interrupt multiple times by interrupt
    //frequency of this loop should be 5x slower than
    //frequency of the interrupt loop
    Timer_Sleep_ms(50);
  }                   
}

/********************************************************************/
// Functions
/********************************************************************/

/********************************************************************/
// Interrupt Service Routines
/********************************************************************/
interrupt VectorNumber_Vtimch0 void Vtimch0_Handler(void)
{
  TFLG1 = TFLG1_C0F_MASK;
  TC0 += 781; //5[ms]
  LED_Tog(LED_YELLOW);
  ++eventCount;
}