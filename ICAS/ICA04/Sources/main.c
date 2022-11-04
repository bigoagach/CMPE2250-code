/********************************************************************/
// HC12 Program:  CMPE1250 - ICA04
// Processor:     MC9S12XDP512
// Bus Speed:     8 MHz
// Author:        Bigoa Gach
// Details:       A more detailed explanation of the program is entered here
                  
// Date:          2022-01-28
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
uint iRed = 0x80;
ulong countOne;
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
  PT1AD1 &= 0x1F; 
  DDR1AD1 = 0xE0; 
  ATD1DIEN1 |= 0x1F; 
  /********************************************************************/
  // main program loop
  /********************************************************************/
  for (;;)
  {
    /* PART 1 */
    PT1AD1 ^= iRed;
    //The appearance of the red LED is as if its on constantly
    //but a little dim. I know for a fact its blinking very fast

    /* PART 2 */
    /*for(countOne = 0; countOne < 500000; ++countOne)
    {
      //nothing happens here
      //The LED is blink very slowly
      //an unsigned int is too small for 500000, 
      //uint is 32 bits or 2^16, which is 65536,
      //which clearly cant fit 500000. ulong is
      //64 bits or 2^32 which is 4294967296
    }*/

    /* PART 3 */
    /*for(countOne = 0; countOne < 1000; ++countOne)
    {
      //nothing happens here
      // f = 105.22 Hz
      // T = 9.50 ms
      // T / 2 = 4.75 ms (time per toggle)
    }*/

    /* PART 4 */
    // 1.00E3 Hz = 1/1000 ms = 1.00E-3 s
    // 1.00E-3 / 2 = 500E-6 s (time per toggle)
    // 4.75 E-3 / 1000 = 4.75E-6 s/loop
    // 500E-6 / 4.25E-6 = 105
    /*for(countOne = 0; countOne < 105; ++countOne)
    {
      //nothing happens here
      //measured 998.04 Hz
    }*/

    /* PART 5 */
    //What can you say about a LED that is on 50% of the time at high frequency? 
    //it looks as if its on all the time but in reality it
    //is only on half of the time. This conserves energy and
    //puts less strain on the micro

  }                   
}

/********************************************************************/
// Functions
/********************************************************************/

/********************************************************************/
// Interrupt Service Routines
/********************************************************************/
