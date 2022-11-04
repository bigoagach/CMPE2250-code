/********************************************************************/
// HC12 Program:  ICA07 - Enhanced Capture Timer
// Processor:     MC9S12XDP512
// Bus Speed:     8 MHz
// Author:        Bigoa Gach
// Details:       A more detailed explanation of the program is entered here
                  
// Date:          2022/02/15
// Revision History :
//  each revision will have a date + desc. of changes
//  Feb. 15 - 3:33PM - Started project and included ECT.h

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
#include "ECT.h" /* Enhanced Capture Timer Library */
#include "SwLed.h" /* Switch/LED Library */

/********************************************************************/
// Local Prototypes
/********************************************************************/

/********************************************************************/
// Global Variables
/********************************************************************/
ulong counter = 0;
ulong delay = 0;
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
  //EnableInterrupts;
  
  /********************************************************************/
  // initializations
  /********************************************************************/
  SwLED_Init();
  Timer_Init(Timer_Prescale_128); //100[ms]
  TimerCH_EnableAsOutput(Timer_CH0_BIT);
  TC0 = TCNT + 6250; //Arms timer for the first time, 100[ms]
  counter = 0;
  delay = 6250;
  /********************************************************************/
  // main program loop
  /********************************************************************/
  for (;;)
  {
    if(TFLG1 & TFLG1_C0F_MASK)
    {
      LED_Tog(LED_RED);
      TC0 += delay; //Re-arm the CH0
      TFLG1 = TFLG1_C0F_MASK;

      if(++counter == 10){
        LED_Tog(LED_YELLOW);
        counter = 0;
      }

      if(PT1AD1 & SW_DOWN)
      {
        delay -= 63;
        if(delay < 3125)
        {
          delay = 3125;
        }
      }
      else if(PT1AD1 & SW_UP)
      {
        delay += 63;
        if(delay > 9375)
        {
          delay = 9375;
        }
      }
    }
  }                   
}

/********************************************************************/
// Functions
/********************************************************************/

/********************************************************************/
// Interrupt Service Routines
/********************************************************************/

