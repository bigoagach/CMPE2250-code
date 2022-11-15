/********************************************************************/
// HC12 Program:  CMPE2250 - PWM 16-Bit: Select a Duty
// Processor:     MC9S12XDP512
// Bus Speed:     8 MHz
// Author:        Bigoa Gach
// Details:       A more detailed explanation of the program is entered here
                  
// Date:          2022-11-07
// Revision History :
//  each revision will have a date + desc. of changes

/********************************************************************/
// Constant Defines
/********************************************************************/
//Menu Options
typedef enum MenuSelection 
{
  adjust100 = 1,  //allows duty cycle to be adjusted by 100 ticks (10%)
  adjust10,       //allows duty cycle to be adjusted by 10 ticks (1%)
  adjust1,        //allows duty cycle to be adjusted by 1 ticks (0.1%)
  togglePolarity, //allows polarity to be toggled (high to low or low to high)
  reset           //resets back to the defaults
} MenuSelection;
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
//updates the LCD Display when there is a change in information
void LCDDisplayUpdate(void);
//captures the switch press events
//returns 1 when there is a button presse
//returns 0 otherwise
int SwitchPressEvents(void);
//adjusting the adjust values
void AdjustingAdjust(void);
/********************************************************************/
// Global Variables
/********************************************************************/
//duty cycle value, initalized to 500 (50%)
volatile unsigned int duty = 500; 
//adjustment value for the duty cycle, intialized to 1 (0.1%)
volatile unsigned short adjust = 1;
//Createing Button States for all five switches
SwState up, down, left, right, mid;
//menu options intialized to adjust1
MenuSelection currOptions = adjust1;
//for debouncing
volatile int bouncy = 0;
/********************************************************************/
// Constants
/********************************************************************/
//number of ticks per period
const unsigned int period = 1000;
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

  //intializing PWM in 16-bit mode on channel 7 with a 50% duty
  //cycle (1000 ticks for the period, 500 ticks for the duty);
  //frequency of 5.00[kHz]
  PWM_16Bit_Init(5000, 20E6, PWM_CH7_BIT, StartLow, period, duty);
  //enabling channel
  PWM_Enable(PWM_CH7_BIT);
  //intializing PIT Timer to coordinate button click events, 50[us]
  (void)PIT_Init(PIT_Channel_0, PIT_Interrupt_On, 20E6, 50);
  //initalizing Switches, LEDs, SevSeg and LCD
  SwLED_Init();
  lcd_Init();
  SevSeg_Init();
  //turning off cursor
  lcd_DispControl(0, 0);
  //LCD display for the first time
  LCDDisplayUpdate();
  /********************************************************************/
  // main program loop
  /********************************************************************/
  for (;;)
  {
    int pressEvent;
    asm wai;

    //debounce check
    if(!bouncy) {
      pressEvent = SwitchPressEvents();
      bouncy = 5000; //set debouce value to 250[ms]
    }

    //if a button press occurs, update the LCD
    if(pressEvent)
      LCDDisplayUpdate();
  }                   
}

/********************************************************************/
// Functions
/********************************************************************/
void LCDDisplayUpdate(void) {
  //adjustement percentage display or prompt to toggle polarity or reset
  { char dis[20];
    if(currOptions == adjust1 || currOptions == adjust10 ||currOptions == adjust100)
      (void)sprintf(dis, "Adjust:  %3.1f%%         ", adjust * 100.0 / period);
    if(currOptions == togglePolarity)
      (void)sprintf(dis, "Toggle Polarity");
    if(currOptions == reset)
      (void)sprintf(dis, "Reset (LEFT/RIGHT)");
    lcd_StringXY(0, 0, dis); }

  //duty cycle display
  { char dis[20];
    (void)sprintf(dis, "Duty: %3.1f%%", duty * 100.0 / period);
    lcd_StringXY(1, 0, dis); }

  //Polarity display
  if(!PWMPOL_PPOL7)
    lcd_StringXY(2, 0, "Polatrity: l");
  else
    lcd_StringXY(2, 0, "Polatrity: h");

  //channal on or off display
  if(!PWME_PWME7)
    lcd_StringXY(3, 0, "PWM Chan 7: Off");
  else
    lcd_StringXY(3, 0, "PWM Chan 7: On ");
}

int SwitchPressEvents(void) {

  //Intializing Switch States
  Sw_Process(&up, SW_UP);
  Sw_Process(&down, SW_DOWN);
  Sw_Process(&left, SW_LEFT);
  Sw_Process(&right, SW_RIGHT);
  Sw_Process(&mid, SW_MID);

  //MID Switch toggles the PWM on and off
  if(mid == Pressed) {
    //if the channel is not enabled, turn it on
    if(!PWME_PWME7)
      PWM_Enable(PWM_CH7_BIT);
    //if the channel is enabled, turn it off
    else
      PWM_Disable(PWM_CH7_BIT);
    return 1;
  }

  //UP Switch alters menu selection (decrease)
  if(up == Pressed) {
    currOptions -= 1;
    if(!currOptions)
      currOptions = reset;
    AdjustingAdjust();
    return 1;
  }

  //DOWN Switch alters menu selection (increase)
  if(down == Pressed) {
    currOptions += 1;
    if(currOptions > reset)
      currOptions = adjust100;
    AdjustingAdjust();
    return 1;
  }

  //LEFT switch decreases duty cycle by adjust value,
  //toggles the polarity or resets PWM back to defaults
  if(left == Pressed) {
    if(currOptions == reset) {
      adjust = 1;
      duty = 500;
      PWMPOL_PPOL7 = 0;
      PWM_Enable(PWM_CH7_BIT);
      currOptions = adjust1;
    }
    else if(currOptions == togglePolarity) {
      PWMPOL_PPOL7 ^= 1;
    }
    else {
      duty -= adjust;
      if(!(duty > 0))
        duty = 0;
      PWMDTY67 = duty;
    }
  }

  //RIGHT switch decreases duty cycle by adjust value,
  //toggles the polarity or resets PWM back to defaults
  if(right == Pressed) {
    if(currOptions == reset) {
      adjust = 1;
      duty = 500;
      PWMPOL_PPOL7 = 0;
      PWM_Enable(PWM_CH7_BIT);
      currOptions = adjust1;
    }
    else if(currOptions == togglePolarity) {
      PWMPOL_PPOL7 ^= 1;
    }
    else {
      duty += adjust;
      if(duty > period)
        duty = period;
      PWMDTY67 = duty;
    }
  }

  return 0;
}

void AdjustingAdjust(void) {
  //adjusting the adjust values
  if(currOptions == adjust100)
    adjust = 100;
  if(currOptions == adjust10)
    adjust = 10;
  if(currOptions == adjust1)
    adjust = 1;
}
/********************************************************************/
// Interrupt Service Routines
/********************************************************************/
interrupt VectorNumber_Vpit0 void PIT0Int (void)
{
  // clear flag
  PITTF = PITTF_PTF0_MASK; // can't read - clears other flags, write only

  // take action! (decrementing debounce value)
  if(bouncy)
    --bouncy;
}