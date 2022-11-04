/********************************************************************/
// HC12 Program:  Lab 1 - 16-bit Segs Functions
// Processor:     MC9S12XDP512
// Bus Speed:     8 MHz
// Author:        Bigoa Gach
// Details:       A more detailed explanation of the program is entered here
                  
// Date:          2022-03-14
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
#include "SevSeg.h" /* 7-Segment Library */

/********************************************************************/
// Local Prototypes
/********************************************************************/

/********************************************************************/
// Global Variables
/********************************************************************/
uint count;
uint decCount;
uint buttonState;
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
  SevSeg_Init();
  Timer_Init(Timer_Prescale_128);
  TimerCH_EnableAsOutput(Timer_CH0_BIT);

  count = 0;
  buttonState = 0;
  /********************************************************************/
  // main program loop
  /********************************************************************/
  for (;;)
  {
    while(!(PT1AD1 & SW_MID))
    {
      SevSeg_ClearBot4();
      if(buttonState == 0)
      {
        SevSeg_Top4(HexToBCD(count++));
        LED_On(LED_GREEN);
        LED_Off(LED_YELLOW);
      }
      else if(buttonState == 1)
      {
        SevSeg_Top4(BCDToHex(HexToBCD(count++)));
        LED_On(LED_YELLOW);
        LED_Off(LED_GREEN);
      }

      Timer_Sleep_ms(50);

      for(decCount = 0; decCount < 4; decCount++)
      {
        SevSeg_Cust(decCount + 4, SEG_DP);
        Timer_Sleep_ms(50);
        if(PT1AD1 & SW_MID)
        {
          count = 1;
          decCount = 0;
          SevSeg_ClearBot4();
          SevSeg_Top4(0);
        } 
        if(PT1AD1 & SW_UP)
        {
          buttonState = 1;
          LED_On(LED_YELLOW);
          LED_Off(LED_GREEN);
          SevSeg_Top4(BCDToHex(HexToBCD(count++)));
        }
        else if(PT1AD1 & SW_DOWN)
        {
          buttonState = 0;
          LED_On(LED_GREEN);
          LED_Off(LED_YELLOW);
          SevSeg_Top4(HexToBCD(count++));
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

