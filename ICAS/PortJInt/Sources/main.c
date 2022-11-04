/********************************************************************/
// HC12 Program:  CMPE2250 - PORT J Interrupts
// Processor:     MC9S12XDP512
// Bus Speed:     8 MHz
// Author:        Bigoa Gach
// Details:       Use Port J interrupt switchs to operate simple stopwatch
                  
// Date:          2022-09-23
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
#include "portj.h" /* Port J Library */
/********************************************************************/
// Local Prototypes
/********************************************************************/

/********************************************************************/
// Global Variables
/********************************************************************/
float stopwatch;
uint counter;
uint whichButton;
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
  //interval of 50[ms]; enabling CH0 as an interrrupt
  Timer_Init(Timer_Prescale_128);
  TimerCH_EnableAsOutput(Timer_CH0_BIT, 7813);
  TimerCH_IntEnable(Timer_CH0_BIT);
  
  //initializing switches, LEDs,the LCD, & 7-segment display
  SwLED_Init();
  lcd_Init();
  SevSeg_Init();
  PortJ_Init(PJ0 | PJ1, PJ0 | PJ1);

  counter = 0;
  whichButton = 0;
  stopwatch = 0.0;
  /********************************************************************/
  // main program loop
  /********************************************************************/
  lcd_StringXY(0, 0, "B. Gach");
  lcd_StringXY(3, 0, "Time:");
  
  for (;;)
  {
    asm wai;
    stopwatch = 0.050 * counter;
    {
      char stoppyWatchyDisplay[10];
      (void)sprintf(stoppyWatchyDisplay, "%06.1f", stopwatch);
      lcd_StringXY(3, 6, stoppyWatchyDisplay);
    }
    SevSeg_Top4(HexToBCD(counter));
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

  TC0 += 7813; //50[ms]
  
  if(!whichButton)  {    
    ++counter;
    if(counter > 9999)
      counter = 0; 
  } 
  
  LED_Tog(LED_RED);  
}

interrupt VectorNumber_Vportj void IntJ(void) {
  if(PIFJ_PIFJ0) {
    PIFJ = PIFJ_PIFJ0_MASK;
    whichButton = 0;
  }
  else if(PIFJ_PIFJ1) {
    PIFJ = PIFJ_PIFJ1_MASK;
    whichButton = 1;
  }
}