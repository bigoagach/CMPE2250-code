/********************************************************************/
// HC12 Program:  CMPE2250 - LAB01 - Interrupts
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
//Timer Components
typedef struct SClockVals
{
  int thow; // 0-999
  char seconds; // 0-59
  char minutes; // 0-59
  char hours; // 0-23
  unsigned int days; // 0+
} SClockVals;
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
/********************************************************************/
// Local Prototypes
/********************************************************************/
//Converts the millisecond count into days, hours,
//minutes, seconds, and hundredths of seconds
SClockVals MilliTimeConvert(unsigned long ms);
//Updates the LCD Display by comparing the last displayed
//time instance to the msTotalCount and updating the LCD
//when the comparison is false
void CheckForTimeUpdate(unsigned long ms);
//Updates the LCD for any changes in the tick value
void TickLCDUpdate(void);
//Increases or Decreases the Tick Value using the UP and DOWN switches, 
//keeping the tick count within the range of 10000 and 30000 inclusive
void TickChangeButtonPress(void);
/********************************************************************/
// Global Variables
/********************************************************************/
//TC0 Re-arming Value
volatile unsigned int ticks = 20000;
//Total count of time in milliseconds
volatile unsigned long msTotalCount = 0;
//Amount that the Tick Value is changed by (+/-)
int changeTickVal = 0;
//the last time displayed on the LCD, 
//all struct emebers intialized to 0
SClockVals lTI = {0, 0, 0, 0, 0};
//variable for debouncing
volatile unsigned int bounce = 0;
//Switch State UP and Switch State DOWN
SwState up, down;
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

  //setting up an interrupt enabled timer with a 
  //prescale of 1 and an interval of 1[ms], enabling toggling
  Timer_Init(Timer_Prescale_1);
  TimerCH_EnableAsOutput(Timer_CH0_BIT, ticks);
  TimerCH_IntEnable(Timer_CH0_BIT);
  TimerCH_EnableToggle(Timer_CH0_BIT);

  //initializing Switchs, LEDs, and LCD
  SwLED_Init();
  lcd_Init();

  //turning off cursor and blinking on LCD Display
  lcd_DispControl(0, 0);
  /********************************************************************/
  // main program loop
  /********************************************************************/
  for (;;)
  {
    unsigned int snapshot1 = TCNT;
    unsigned int snapshot2;

    asm wai;

    //checks for change in the msTotalCount variable 
    //to update the LCD Display
    CheckForTimeUpdate(msTotalCount);

    //Update the LCD when the tick value changes
    TickLCDUpdate();

    //check for switch presses
    TickChangeButtonPress();

    snapshot2 = TCNT;

    if(snapshot2 - snapshot1 > 20000)
      LED_Tog(LED_RED);
  }                   
}

/********************************************************************/
// Functions
/********************************************************************/
SClockVals MilliTimeConvert(unsigned long ms) 
{
  //creating a new instance of the SClockVals Struct
  SClockVals time = {0, 0, 0, 0, 0};

  //converting the supplied millisecond count into
  //hundredths of seconds, seconds, minutes, hours,
  //and days, using division and modulus division
  time.thow = ms % 1000;
  time.seconds = (char)(ms / 1000);
  time.minutes = (char)(ms / (1000 * 60));
  time.hours = (char)(ms / 1000 / 3600);
  time.days = ms / 1000 / 3600 / 24;

  //return the newly convert time
  return time;
}

void CheckForTimeUpdate(unsigned long ms) 
{
  //if the last displayed time value's millisecond count / 100
  //doesn't equal the current millisecond count / 100,
  //the LCD will update (this happens every 100[ms])
  if(lTI.thow / 100 != MilliTimeConvert(ms).thow / 100) {
    //re-assign the old time
    lTI = MilliTimeConvert(ms);

    //update LCD
    { char display[20]; //for displaying in LCD
      (void)sprintf(display, "%05u %02u:%02u:%02u.%02u", 
      lTI.days, lTI.hours, lTI.minutes, lTI.seconds, lTI.thow);
      lcd_StringXY(3, 0, display); }

    //toggle the Green LED, LED should toggle every 100[ms]
    LED_Tog(LED_GREEN);
  }
}

void TickLCDUpdate(void) {
  //setting a static variable for the previous tick count,
  //intialized to -1 so that it is different to the changeTickVal
  static int prevTick = -1;

  //if the previoud tick count does not equal the current tick 
  //count (indicating a change in tick count), update the LCD
  //display and toggle the Yellow LED
  if(prevTick != changeTickVal) {

    //Tick Value
    { char tickDis[20]; //for displaying in the LCD
      (void)sprintf(tickDis, "Ticks: %05u", ticks);
      lcd_StringXY(0, 0, tickDis); }

    //Tick Value Changes By
    { char tickIter[20]; //for displaying in the LCD
      (void)sprintf(tickIter, "+/- : %i        ", changeTickVal);
      lcd_StringXY(1, 0, tickIter); }

    //Faster or Slower Macro
    #define dFS(pace, x) \
      { char show[20]; \
      (void)sprintf(show, "%s by %04.3f%%", pace, x); \
      lcd_StringXY(2, 0, show); } \
    
    //if the tick value is equal to 0
    if(!changeTickVal) {
      lcd_StringXY(2, 0, "Ideal               ");
    }

    //if the tick value is less than zero
    else if(changeTickVal < 0) {
      dFS("Faster", changeTickVal / 20000.0 * -100.0);
    }

    //if the tick value is greater than zero
    else if(changeTickVal > 0) {
      dFS("Slower", changeTickVal / 20000.0 * 100.0);
    }

    //update the previous tick count
    prevTick = ticks;

    //toggle Yellow LED to indicate LCD Update
    LED_Tog(LED_YELLOW);
  }
}

void TickChangeButtonPress(void) {
  //set up switch procces for Switches UP and DOWN
  Sw_Process(&up, SW_UP);
  Sw_Process(&down, SW_DOWN);

  //if UP is pressed, check for debouncing, increase
  //the changeTickVal and add it to the ticks variable;
  //check that value is within range, set bounce to 75[ms]
  if(up == Pressed) {
    if(!bounce) {
      //increase ticks
      changeTickVal += 1;
      //check that value is within range
      if(changeTickVal > 10000)
        changeTickVal = 10000;
      //re-assign
      ticks = 20000 + changeTickVal;
      //setting bounce to 75[ms] (ticks * 50E-9 * 75 / 1E-3)
      bounce = (int)(ticks * 0.00375);
    }
  }

  //if DOWN is pressed, check for debouncing, decrease
  //the changeTickVal and add it to the ticks variable;
  //check that value is within range, set bounce to 75[ms]
  if(down == Pressed) {
    if(!bounce) {
      //decrease ticks
      changeTickVal -= 1;
      //check that value is within range
      if(changeTickVal < -10000)
        changeTickVal = -10000;
      //re-assign
      ticks = 20000 + changeTickVal;
      //setting bounce to 75[ms] (ticks * 50E-9 * 75 / 1E-3)
      bounce = (int)(ticks * 0.00375);
    }
  }
}
/********************************************************************/
// Interrupt Service Routines
/********************************************************************/
interrupt VectorNumber_Vtimch0 void IOC0(void)
{
  //clearing flag
  TFLG1 = TFLG1_C0F_MASK;

  //re-arming TC0
  TC0 += ticks;

  //increasing time count variable by 1[ms]
  ++msTotalCount;

  //for debouncing
  if(bounce)
    --bounce;
}