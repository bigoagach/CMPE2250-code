/********************************************************************/
// HC12 Program:  Lab 2 - Number Entry / HEX Converter
// Processor:     MC9S12XDP512
// Bus Speed:     8 MHz
// Author:        Bigoa Gach
// Details:       A more detailed explanation of the program is entered here
                  
// Date:          2022-03-17
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
#include "ECT.h" /* Timer Library */
#include "Clock.h" /* Clock Library */
#include "SwLed.h" /* Switch/LED Library */
#include "SevSeg.h" /* 7-Segment Display Library */
/********************************************************************/
// Local Prototypes
/********************************************************************/
void AddingFunction(void);
void SubtractingFunction(void);
/********************************************************************/
// Global Variables
/********************************************************************/
uint number = 0;
uint index = 3;
SwState left, right, down, up;
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
  SevSeg_Init();

  number = 0;
  index = 0;
  /********************************************************************/
  // main program loop
  /********************************************************************/
  for (;;)
  {
    Sw_Process(&left, SW_LEFT);
    Sw_Process(&right, SW_RIGHT);
    Sw_Process(&up, SW_UP);
    Sw_Process(&down, SW_DOWN);

    SevSeg_Top4D(index, number);
    SevSeg_Bot4(BCDToHex(number));

    if(up == Pressed)
      AddingFunction();
    else if(down == Pressed)
      SubtractingFunction();

    if((left == Pressed) && (index > 0))
      index--;
    else if((right == Pressed) && (index < 3))
      index++;

    Timer_Sleep_ms(50);
  }                   
}

/********************************************************************/
// Functions
/********************************************************************/
void AddingFunction(void)
{
  if(index == 3)
  {
    if((number & 0x000F) >= 0x0009)
      number -= 0x0009;
    else
      number += 0x0001;
  }
  else if(index == 2)
  {
    if((number & 0x00F0) >= 0x0090)
      number -= 0x0090; 
    else  
      number += 0x0010;   
  }
  else if(index == 1)
  {
    if((number & 0x0F00) >= 0x0900)
      number -= 0x0900; 
    else
      number += 0x0100;    
  }
  else if(index == 0)
  {
    if((number & 0xF000) >= 0x9000)
      number -= 0x9000; 
    else 
      number += 0x1000;     
  }
}

void SubtractingFunction(void)
{
  if(index == 3)
  {
    if((number & 0x000F) <= 0)
      number += 0x0009;
    else
      number -= 0x0001;
  }
  else if(index == 2)
  {
    if((number & 0x00F0) <= 0)
      number += 0x0090;
    else
      number -= 0x0010;      
  }
  else if(index == 1)
  {
    if((number & 0x0F00) <= 0)
      number += 0x0900;
    else
      number -= 0x0100;       
  }
  else if(index == 0)
  {
    if((number & 0xF000) <= 0)
      number += 0x9000;
    else
      number -= 0x1000;       
  }  
}
/********************************************************************/
// Interrupt Service Routines
/********************************************************************/
