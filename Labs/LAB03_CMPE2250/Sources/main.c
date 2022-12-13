/********************************************************************/
// HC12 Program:  CMPE2250 -  Lab #03 – I2C TiltyLED
// Processor:     MC9S12XDP512
// Bus Speed:     8 MHz
// Author:        Bigoa Gach
// Details:       A more detailed explanation of the program is entered here
                  
// Date:          2022/12/09
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
#include "LSM303.h"
#include "i2c.h"
#include <math.h>
#include <stdlib.h>
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
  lcd_Init();
  lcd_DispControl(0, 0);
  SevSeg_Init();
  (void)PIT_Init(PIT_Channel_0, PIT_Interrupt_On, 20E6, 500000);
  I2C_Init0(I2CMicro20MHz, I2CBus400, 0);
  (void)LSM303_Init();
  /********************************************************************/
  // main program loop
  /********************************************************************/
  for (;;)
  {
    unsigned char retVal = 0;
    unsigned char XAxisL, XAxisH, YAxisL, YAxisH, ZAxisL, ZAxisH;
    unsigned char xyzbuff[6] = {0};

    asm wai;

    // status_a (27h) ovrx azyx
    // X: 28/29
    // Y: 2A/2B
    // Z: 2C/2D

    if (!LSM303_GetXYZ(xyzbuff))
    {
      // mappings
      XAxisH = xyzbuff[0];
      XAxisL = xyzbuff[1];
      YAxisH = xyzbuff[2];
      YAxisL = xyzbuff[3];
      ZAxisH = xyzbuff[4];
      ZAxisL = xyzbuff[5];

      // show signed value on LCD, along with +max on X axis
      {
        // bits are packed left-aligned (good)
        // so interpret as signed value and /16 to scale correctly
        int iXSigned = XAxisL + (((unsigned int)XAxisH) << 8);
        int iYSigned = YAxisL + (((unsigned int)YAxisH) << 8);
        int iZSigned = ZAxisL + (((unsigned int)ZAxisH) << 8);
        char buff[20] = {0};
        iXSigned /= 16;
        iYSigned /= 16;
        iZSigned /= 16;
        (void)sprintf(buff, "X: %6.3fg", iXSigned / 1000.0f);
        lcd_StringXY(0, 0, buff);
        (void)sprintf(buff, "Y: %6.3fg", iYSigned / 1000.0f);
        lcd_StringXY(1, 0, buff);
        (void)sprintf(buff, "Z: %6.3fg", iZSigned / 1000.0f);
        lcd_StringXY(2, 0, buff);
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
interrupt VectorNumber_Vpit0 void PIT0Int (void)
{
  // clear flag
  PITTF = PITTF_PTF0_MASK; // can't read - clears other flags, write only
}