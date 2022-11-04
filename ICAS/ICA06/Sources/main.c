/********************************************************************/
// HC12 Program:  CMPE1250 - ICA06
// Processor:     MC9S12XDP512
// Bus Speed:     8 MHz
// Author:        Bigoa Gach
// Details:       The purpose on this ICA is testing the PLL
//                function and testing that this function is
//                doing what it should be. Comparing the same
//                code running up 8MHz (default), 20MHz (crazy),
//                24MHz (super crazy fast - sheeeeeesh). Mea-
//                suring the speed difference and comparing it
//                to my expectations (I have zero expectations
//                only because I don't know what I'm doing til
//                after the ICA is finished :D)
                  
// Date:          2022-02-04
// Revision History :
//  Feb.04  5:19PM - Started the ICA and added details/comments
//          5:26PM - included the Clock.h file and created the
//                  Clock.c file. Changed the general settings
//          8:03PM - Starting Part 1 - Red LED blicks every 100ms
//          9:00PM - Break; made the blocking delay
//          9:21PM - Finished Part 1; watched embedded lectures
//                  understand clocks better
//         11:40PM - Implemented 4 out of 5 functions
//  Feb.07  8:22AM - Implemented the Clock_GetFactor()
//          3:18PM - Debugged the code

/********************************************************************/
// Constant Defines
/********************************************************************/

/********************************************************************/
#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */
#include "Clock.h"      /* Clock Library */
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
ulong counter = 0;
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
    PT1AD1 = 0;  //Clear port
    DDR1AD1 = 0b11100000;  //LEDs as output, buttons as input
    ATD1DIEN1 |= 0b00011111; //Enable GPIOs for buttons

    Clock_Set20MHZ();
    //Clock_Set24MHZ();
    Clock_EnableOutput(0b00000011);
  /********************************************************************/
  // main program loop
  /********************************************************************/
  for (;;)
  { 
    //not using a variable for 0x80 because it adds 2 extra cycles :D
    PT1AD1 ^= 0x80;

    // 1000 delay cycles --> 9.50ms (from ICA04)
    // x delay cycles --> 200ms
    // x = 21053 cycles
    for(counter = 0; counter < 21053; ++counter)
    {
      //blocking delay - should see a 200ms period on Waveforms
    }

  }                   
}

/********************************************************************/
// Functions
/********************************************************************/

/********************************************************************/
// Interrupt Service Routines
/********************************************************************/

