/********************************************************************/
// HC12 Program:  YourProg - MiniExplanation
// Processor:     MC9S12XDP512
// Bus Speed:     8 MHz
// Author:        This B. You
// Details:       A more detailed explanation of the program is entered here
                  
// Date:          Date Created
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


/********************************************************************/
// Local Prototypes
/********************************************************************/

/********************************************************************/
// Global Variables
/********************************************************************/
  uint iRed = 0;
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
  PT1AD1 &= 0x1F; DDR1AD1 = 0xE0; ATD1DIEN1 |= 0x1F; 
  /********************************************************************/
  // main program loop
  /********************************************************************/
  for (;;)
  { 
    static uint uiCounter = 0;

    if (!++uiCounter){
      if(++iRed == 10){
        iRed = 0;
        PT1AD1 ^= 0x80;
      }
    }
  } // loop forever 
  // please make sure that you never leave main
}
/********************************************************************/
// Functions
/********************************************************************/

/********************************************************************/
// Interrupt Service Routines
/********************************************************************/