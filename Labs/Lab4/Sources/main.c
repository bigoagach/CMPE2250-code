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
#include <stdio.h>      /* for the sprintf funtion */
#include "derivative.h" /* derivative-specific definitions */
#include <math.h> /* math library for the powers */
/********************************************************************/
// Library includes
/********************************************************************/
// your includes go here
#include "SwLed.h" /* Switch/LED Library */
#include "ECT.h" /* Timer Library */
#include "Clock.h" /* Clock Library */
#include "SevSeg.h" /* 7-Segment Library */
#include "lcd.h" /* LCD Library */
/********************************************************************/
// Local Prototypes
/********************************************************************/
void DecHexBin(void);
void IntializeArray(void);
/********************************************************************/
// Global Variables
/********************************************************************/
char const countLabel[] = "Count: ";
char const decLabel[] = "DEC: ";
char const hexLabel[] = "HEX: ";
char theTime[6];
char theDec[5];
char theHex[4];
float time = 0.0;
int binary[18];
int cursor = 0;
uint decimal = 0;
uint binPos = 0;
SwState left, right, mid;
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
  Timer_Init(Timer_Prescale_128);
  TimerCH_EnableAsOutput(Timer_CH0_BIT);
  lcd_Init();
  SwLED_Init();

  /* Initial Display */
  lcd_String(countLabel);
  lcd_StringXY(1, 0, decLabel);
  lcd_StringXY(2, 0, hexLabel);
  lcd_StringXY(1, 5, "00000");
  lcd_StringXY(2, 5, "0000");
  lcd_StringXY(0, 7, "00000.0");
  lcd_StringXY(3, 2, "0000000000000000");
  lcd_DispControl(0, 1);

  /* Intializing Variables */
  time = 0.0;
  decimal = 0;
  binPos = 15;
  cursor = 2;

  IntializeArray();
  /********************************************************************/
  // main program loop
  /********************************************************************/
  for (;;)
  {
    if(TFLG1 & TFLG1_C0F_MASK)
    {
      TC0 += 625; //Re-arm the CH0
      TFLG1 = TFLG1_C0F_MASK; //clearing flag

      /* Starting the timer */
      sprintf(theTime, "%07.1f", time);
      lcd_StringXY(0, 7, theTime);
      time += 0.01;
      if(time > 99999.99)
        time = 0.0;

      /* Setting the cursor position */
      lcd_AddrXY(3, 2);
    }

    /* Setting up the Switches */
    Sw_Process(&left, SW_LEFT);
    Sw_Process(&right, SW_RIGHT);
    Sw_Process(&mid, SW_MID);

    if((left == Pressed) && (cursor > 2)) {
      cursor--;
      binPos++;
    }
    else if((left == Pressed) && (cursor == 2)) {
      cursor = 2;
      binPos = 15;
    }

    if((right == Pressed) && (cursor < 17)) {
      cursor++;
      binPos--;
    }
    else if((right == Pressed) && (cursor == 17)) {
      cursor = 17;
      binPos = 0;
    }

    if(mid == Pressed) {
      if(binary[cursor] == 0) {
        binary[cursor] = 1;
        lcd_StringXY(3, cursor, "1");
        decimal += pow(2, binPos);
      }
      else if(binary[cursor] == 1) {
        binary[cursor] = 0;
        lcd_StringXY(3, cursor, "0");
        decimal -= pow(2, binPos);
      }
      DecHexBin();
    }

    lcd_AddrXY(3, cursor);
  
  }                   
}

/********************************************************************/
// Functions
/********************************************************************/
void DecHexBin(void) {
  sprintf(theDec, "%05u", decimal);
  lcd_StringXY(1, 5, theDec);

  sprintf(theHex, "%04X", decimal);
  lcd_StringXY(2, 5, theHex);
}

void IntializeArray(void) {
  int i;
  for(i = 0; i < sizeof(binary); i++) {
    binary[i] = 0;
  }
}
/********************************************************************/
// Interrupt Service Routines
/********************************************************************/
