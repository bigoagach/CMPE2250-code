/********************************************************************/
// HC12 Program:  ICA08 - int Timer_Sleep
// Processor:     MC9S12XDP512
// Bus Speed:     8 MHz
// Author:        Bigoa Gach
// Details:       A more detailed explanation of the program is entered here
                  
// Date:          2022-02-20
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
#include "SwLed.h" /* Switch/LED Library */
#include "ECT.h" /* Timer Library */
#include "Clock.h" /* Clock Library */

/********************************************************************/
// Local Prototypes
/********************************************************************/

/********************************************************************/
// Global Variables
/********************************************************************/
uint currentButton;
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
  Timer_Init(Timer_Prescale_128);
  TimerCH_EnableAsOutput(Timer_CH0_BIT);

  currentButton = 3;
  /********************************************************************/
  // main program loop
  /********************************************************************/
  for (;;)
  {

    if(PT1AD1 & SW_UP)
    {
      currentButton = 1;
    }
    else if(PT1AD1 & SW_DOWN)
    {
      currentButton = 2;
    }
    else if(PT1AD1 & SW_LEFT)
    {
      currentButton = 3;
    }
    else if(PT1AD1 & SW_MID)
    {
      currentButton = 4;
    }
    else if(PT1AD1 & SW_RIGHT)
    {
      currentButton = 5;
    }

    if(currentButton == 1)
    {
      LED_Tog(LED_RED);
      Timer_Sleep_ms(5000);
    }
    else if(currentButton == 2)
    {
      LED_Tog(LED_RED);
      Timer_Sleep_ms(1);
    }
    else if(currentButton == 3)
    {
      LED_Tog(LED_RED);
      Timer_Sleep_ms(100);
    }
    else if(currentButton == 4)
    {
      LED_Tog(LED_RED);
      Timer_Sleep_ms(500);
    }
    else if(currentButton == 5)
    {
      LED_Tog(LED_RED);
      Timer_Sleep_ms(1000);
    }
  }                   
}

/********************************************************************/
// Functions
/********************************************************************/

/********************************************************************/
// Interrupt Service Routines
/********************************************************************/
