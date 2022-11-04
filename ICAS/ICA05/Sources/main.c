/********************************************************************/
// HC12 Program:  CMPE1250 - ICA05
// Processor:     MC9S12XDP512
// Bus Speed:     8 MHz
// Author:        Bigoa Gach
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
#include "SwLed.h"
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
uint numOfLEDSOn = 0; //counts the number of LEDs on
uint redOnOff = 0;    //keeps track of if the red LED is on
uint yellowOnOff = 0; //keeps track of if the yellow LED is on
uint greenOnOff = 0;  //keeps track of if the green LED is on
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
  /********************************************************************/
  // main program loop
  /********************************************************************/
  for (;;)
  {
    /* TIER 1 */
      //Red LED
      /*if(PT1AD1 & SW_LEFT) //if the left switch is pressed
      {
        LED_On(LED_RED);  //the the red LED turns on
      }
      else
      {
        LED_Off(LED_RED); //red LED is off
      }

      //Yellow LED
      if(PT1AD1 & SW_MID) //if the middle switch is press
      {
        LED_On(LED_YELLOW); //the yellow LED turns on
      }
      else
      {
        LED_Off(LED_YELLOW);  //yellow LED is off
      }

      //Green LED
      if(PT1AD1 & SW_RIGHT) //if the right switch is press
      {
        LED_On(LED_GREEN);  //the green LED turns on
      }
      else
      {
        LED_Off(LED_GREEN); //green LED is off
      }*/

    /* TIER 2 */
      //Red LED
      /*if(PT1AD1 & SW_LEFT) //if the left switch is pressed
      {
        LED_On(LED_RED);  //the the red LED turns on
      }
      //Yellow LED
      if(PT1AD1 & SW_MID) //if the middle switch is press
      {
        LED_On(LED_YELLOW); //the yellow LED turns on
      }
      //Green LED
      if(PT1AD1 & SW_RIGHT) //if the right switch is press
      {
        LED_On(LED_GREEN);  //the green LED turns on
      }

      if((PT1AD1 & SW_UP) || (PT1AD1 & SW_DOWN)) //if the top or bottom switch os press
      {
        LED_Off(LED_ALL); //all LEDs turn off
      }*/

    /* TIER 3 */
      //Red LED
      if((PT1AD1 & SW_LEFT) && (numOfLEDSOn != 2)) //if the left switch is pressed
      {
        LED_On(LED_RED);  //the the red LED turns on
        redOnOff = 1;
        numOfLEDSOn = redOnOff + yellowOnOff + greenOnOff;
      }
      //Yellow LED
      else if((PT1AD1 & SW_MID) && (numOfLEDSOn != 2)) //if the middle switch is press
      {
        LED_On(LED_YELLOW); //the yellow LED turns on
        yellowOnOff = 1;
        numOfLEDSOn = redOnOff + yellowOnOff + greenOnOff;
      }
      //Green LED
      else if((PT1AD1 & SW_RIGHT) && (numOfLEDSOn != 2)) //if the right switch is press
      {
        LED_On(LED_GREEN);  //the green LED turns on
        greenOnOff = 1;
        numOfLEDSOn = redOnOff + yellowOnOff + greenOnOff;
      }
      
      if((PT1AD1 & SW_UP) || (PT1AD1 & SW_DOWN)) //if the top or bottom switch os press
      {
        LED_Off(LED_ALL); //all LEDs turn off
        redOnOff = 0;
        yellowOnOff = 0;
        greenOnOff = 0;
        numOfLEDSOn = 0;
      }

  }                  
}

/********************************************************************/
// Functions
/********************************************************************/

/********************************************************************/
// Interrupt Service Routines
/********************************************************************/

