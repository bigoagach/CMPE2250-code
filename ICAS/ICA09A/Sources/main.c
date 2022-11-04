/********************************************************************/
// HC12 Program:  ICA09A Seven Segment Display
// Processor:     MC9S12XDP512
// Bus Speed:     8 MHz
// Author:        Bigoa Gach
// Details:       A more detailed explanation of the program is entered here
                  
// Date:          2022-03-05
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
#include "SevSeg.h" /* Seven Segment Library */

/********************************************************************/
// Local Prototypes
/********************************************************************/
void loopCount(void);
/********************************************************************/
// Global Variables
/********************************************************************/
ulong counter;
Bool isDown;
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
  counter = 0;
  isDown = FALSE;
  /********************************************************************/
  // main program loop
  /********************************************************************/
  for (;;)
  {
    if(isDown == FALSE)
    {
      loopCount();
    }

    if(PT1AD1 & SW_DOWN)
    {
      isDown = TRUE;
      SevSeg_ClearAll();
      SevSeg_Cust(0, 0b11111111);
      SevSeg_Cust(1, 0b11111110);
      SevSeg_Cust(2, 0b11001111);
      SevSeg_Cust(3, 0b11110101);
      SevSeg_Cust(4, 0b01101101);
      SevSeg_Cust(5, 0b11010111);
      SevSeg_Cust(6, DEGREE_SIGN | NO_DP);
      SevSeg_Cust(7, 0b11001011);
    }
    else if(PT1AD1 & SW_UP)
    {
      SevSeg_ClearAll();
      loopCount();
    }    
  }                   
}

/********************************************************************/
// Functions
/********************************************************************/
void loopCount(void)
{
  SevSeg_Cust(0, 0b11110000); //display
  for(counter = 0; counter < 105263; counter++){} //delay 500ms
  SevSeg_Cust(1, 0b11011111); //display
  for(counter = 0; counter < 105263; counter++){} //delay 500ms
  SevSeg_Cust(2, 0b11010111); //display
  for(counter = 0; counter < 105263; counter++){} //delay 500ms
  SevSeg_Cust(3, 0b10110110); //display
  for(counter = 0; counter < 105263; counter++){} //delay 500ms
  SevSeg_Cust(4, 0b11110101); //display
  for(counter = 0; counter < 105263; counter++){} //delay 500ms
  SevSeg_Cust(5, 0b11101101); //display
  for(counter = 0; counter < 105263; counter++){} //delay 500ms
  SevSeg_Cust(6, 0b10110000); //display
  for(counter = 0; counter < 105263; counter++){} //delay 500ms
  SevSeg_Cust(7, 0b11111011); //display
  for(counter = 0; counter < 105263; counter++){} //delay 500ms
}