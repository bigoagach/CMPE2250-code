/********************************************************************/
// HC12 Program:  Lab 3 - LCD Control Functions
// Processor:     MC9S12XDP512
// Bus Speed:     8 MHz
// Author:        Bigoa Gach
// Details:       A more detailed explanation of the program is entered here
                  
// Date:          2022-04-04
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
#include "Clock.h" /* Clock Library */
#include "SevSeg.h" /* 7-Segment Library */
#include "lcd.h" /* LCD Library */
#include <stdlib.h> /* Library for the rand() function */
/********************************************************************/
// Local Prototypes
/********************************************************************/
void RandomChar(void);
void RedLEDBlink(void);
void CheckForFifteen(void);
/********************************************************************/
// Global Variables
/********************************************************************/
unsigned char currentRow = 0;
unsigned char currentColumn = 0;
uint numberOfClicks = 0;
SwState up, down, left, right, mid;
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
  SevSeg_Init();

  currentColumn = 0;
  currentRow = 0;
  numberOfClicks = 0;
  SevSeg_Top4(numberOfClicks);
  lcd_DispControl(1, 0);
  /********************************************************************/
  // main program loop
  /********************************************************************/
  for (;;)
  {
    Sw_Process(&left, SW_LEFT);
    Sw_Process(&right, SW_RIGHT);
    Sw_Process(&up, SW_UP);
    Sw_Process(&down, SW_DOWN);
    Sw_Process(&mid, SW_MID);

    /* LEFT BUTTON CLICK */
    if((left == Pressed) && (currentColumn > 0))
      currentColumn--;
    else if((left == Pressed) && (currentColumn == 0))
    {
      currentColumn = 0;
      RedLEDBlink();
    }

    /* RIGHT BUTTON CLICK */
    if((right == Pressed) && (currentColumn < 19))
      currentColumn++;
    else if((right == Pressed) && (currentColumn == 19))
    {
      currentColumn = 19;
      RedLEDBlink();
    }

    /* UP BUTTON CLICK */
    if((up == Pressed) && (currentRow > 0))
      currentRow--;
    else if((up == Pressed) && (currentRow == 0))
    {
      currentRow = 0;  
      RedLEDBlink();    
    }

    /* DOWN BUTTON CLICK */
    if((down == Pressed) && (currentRow < 3))
      currentRow++;
    else if((down == Pressed) && (currentRow == 3))
    {
      currentRow = 3;
      RedLEDBlink();
    }
    
    /* MIDDLE BUTTON CLICK */
    if((mid == Pressed))
    {
      numberOfClicks++;
      RandomChar();
    }

    CheckForFifteen(); //checking for 15 middle button clicks
    lcd_AddrXY(currentRow, currentColumn); //updates cursor position
    SevSeg_Top4(HexToBCD(numberOfClicks)); //updates top 4 of 7-Segment display
  }                   
}

/********************************************************************/
// Functions
/********************************************************************/

/*************************************************
 * Randomly generates a number between 65 and 90,
 * the ascii decimal numbers from A to Z
 * 
 * pre: the middle button has been pressed
 * post: random number is generated
 *************************************************/
void RandomChar(void)
{
  lcd_Data((rand() % (65 - 91)) + 65);
}

/*************************************************
 * Red LED blinks for 50ms
 * 
 * pre: the user attempts to move the cursor
 * beyond the bounds of the LCD display
 * post: the Red LED blinked
 *************************************************/
void RedLEDBlink(void)
{
  LED_On(LED_RED);
  Timer_Sleep_ms(50);
  LED_Off(LED_RED);
}

/*************************************************
 * Checks to see if the number of characters
 * deposited is equal to 15
 * 
 * pre: none
 * post: the top row of the 7-Segment display 
 * is reset to 0 and the cursor returns to 
 * (0, 0) if 15 characters have been deposited
 *************************************************/
void CheckForFifteen(void)
{
  if(numberOfClicks == 15)
  {
    numberOfClicks = 0;
    currentColumn = 0;
    currentRow = 0;
    lcd_Home();
    lcd_Clear();
  }
}

/********************************************************************/
// Interrupt Service Routines
/********************************************************************/
