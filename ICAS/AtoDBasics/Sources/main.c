/********************************************************************/
// HC12 Program:  YourProg - MiniExplanation
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
/********************************************************************/
// Library includes
/********************************************************************/
// your includes go here
#include <stdio.h>  /* for the sprintf funtion */
#include "SwLed.h"  /* Switch/LED Library */
#include "ECT.h"    /* Timer Library */
#include "Clock.h"  /* Clock Library */
#include "SevSeg.h" /* 7-Segment Library */
#include "lcd.h"    /* LCD Library */
#include "portj.h"  /* Port J Library */
#include "pit.h"    /* PIT Timer Library */
#include "pwmlib.h" /* PWM Library */
#include "AtoD.h"
/********************************************************************/
// Local Prototypes
/********************************************************************/

/********************************************************************/
// Global Variables
/********************************************************************/

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
  EnableInterrupts;
  
  /********************************************************************/
  // initializations
  /********************************************************************/
  Clock_Set20MHZ();
  //intializing the AtoD modules, disabling interrupts
  AtoD_Init(0);
  //PIT 
  PIT_Init(PIT_Channel_0, PIT_Interrupt_On, 20E6, 100000);
  //intializing LCD
  lcd_Init();
  lcd_DispControl(0, 0);
  SevSeg_Init();
  /********************************************************************/
  // main program loop
  /********************************************************************/
  lcd_StringXY(0, 0, "Channel 5: ");
  for (;;)
  {
    asm wai;
    SevSeg_Bot4(AtoD_CH_Read(5));
    { char dis[21];
      (void)sprintf(dis, "%03.2fV", AtoD_CH_Read(5));
      lcd_StringXY(0, 11, dis); }  
  }                   
}

/********************************************************************/
// Functions
/********************************************************************/

/********************************************************************/
// Interrupt Service Routines
/********************************************************************/
interrupt VectorNumber_Vpit0 void PIT0Int (void)
{
  // clear flag
  PITTF = PITTF_PTF0_MASK; // can't read - clears other flags, write only

  // take action!
}