/********************************************************************/
// HC12 Program:  ICA09B - Basic FUnctions for 7-Segment Display
// Processor:     MC9S12XDP512
// Bus Speed:     8 MHz
// Author:        Bigoa Gach
// Details:       A more detailed explanation of the program is entered here
                  
// Date:          2022-03-10
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
#include "SevSeg.h" /* Seven Segment Display */

/********************************************************************/
// Local Prototypes
/********************************************************************/

/********************************************************************/
// Global Variables
/********************************************************************/
uint upCount;
uint downCount;
uint position;
uint pass;
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

  /* TIER 1 */
  // SevSeg_Char(0, 4); //4
  // SevSeg_Char(4, 3); //3
  // SevSeg_Char(7, 0xE); //E

  // SevSeg_Cust(1, 0b01001010); 
  // SevSeg_Cust(2, 0b11110000);
  // SevSeg_Cust(5, 0b10001011);
  // SevSeg_Cust(6, 0b10110001);  

  // upCount = 0x0000;
  // downCount = 0xFFFF;

  /* TIER 2 & 3 */
  upCount = 0;
  position = 0;
  pass = 0;
  SevSeg_Bot4(0);
  /********************************************************************/
  // main program loop
  /********************************************************************/
  for (;;)
  {

    /* TIER 1 */
    // if(PT1AD1 & SW_MID)
    // {
    //   SevSeg_Top4(upCount);
    //   SevSeg_Bot4(downCount);
    //   Timer_Sleep_ms(100);
    //   upCount++;
    //   downCount--;
    // }

    /* TIER 2 */
    SevSeg_Cust(position, 0b11000000);
    Timer_Sleep_ms(100);
    SevSeg_Cust(position, 0b10100000);
    Timer_Sleep_ms(100);
    SevSeg_Cust(position, 0b10010000);
    Timer_Sleep_ms(100);
    SevSeg_Cust(position, 0b10000001);
    Timer_Sleep_ms(100);
    SevSeg_Cust(position, 0b10001000);
    Timer_Sleep_ms(100);
    SevSeg_Cust(position, 0b10000010);
    Timer_Sleep_ms(100);
    // pass++;

    // if((pass % 2) == 0)
    // {
    //   if(PT1AD1 & SW_RIGHT)
    //   {
    //     if(position >= 3)
    //     {
    //       position = 3;
    //     }
    //     else
    //     {
    //       SevSeg_Bot4(++upCount);
    //       position++;
    //       SevSeg_ClearDigit(position - 1);
    //     }
    //   }
    //   else if(PT1AD1 & SW_LEFT)
    //   {
    //     if(position <= 0)
    //     {
    //       position = 0;
    //     }
    //     else
    //     {
    //       SevSeg_Bot4(++upCount);
    //       position--;
    //       SevSeg_ClearDigit(position + 1);
    //     }
    //   }
    // }

    /* TIER 3 */
    if(PT1AD1 & SW_RIGHT)
    {
      pass++;
      if((pass % 2) == 0)
      {
        if(position >= 3)
        {
          position = 3;
        }
        else
        {
          SevSeg_Bot4(++upCount);
          position++;
          SevSeg_ClearDigit(position - 1);
        }
      }
    }
    else if(PT1AD1 & SW_LEFT)
    {
      pass++;
      if((pass % 2) == 0)
      {
        if(position <= 0)
        {
          position = 0;
        }
        else
        {
          SevSeg_Bot4(++upCount);
          position--;
          SevSeg_ClearDigit(position + 1);
        }
      }
    }
    else
    {
      pass = 0;
    }

  }                   
}

/********************************************************************/
// Functions
/********************************************************************/

/********************************************************************/
// Interrupt Service Routines
/********************************************************************/
