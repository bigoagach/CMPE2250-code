/********************************************************************/
// HC12 Program:  CMPE2250 - Review ICA
// Processor:     MC9S12XDP512
// Bus Speed:     8 MHz
// Author:        Bigoa Gach
// Details:       Simple Review to refresh the mind and show the 
//                instructor where we're at :)
                  
// Date:          2022-09-05
// Revision History :
//  i forgot to add revision lmao my bad >:)

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
#include "math.h"   /* MATHEMATICS >:) */
/********************************************************************/
// Local Prototypes
/********************************************************************/

/********************************************************************/
// Global Variables
/********************************************************************/
SwState left, mid, right;
/********************************************************************/
// Constants
/********************************************************************/

/********************************************************************/
// Main Entry
/********************************************************************/
void main(void)
{
  unsigned int counter = 0;
  unsigned int display = 0;

  // main entry point
  _DISABLE_COP();
  //EnableInterrupts;
  
  /********************************************************************/
  // initializations
  /********************************************************************/

  //set clock speed from 8Mhz to 20Mhz
  Clock_Set20MHZ();
  //set prescale
  Timer_Init(Timer_Prescale_2); 
  //output compare channel 0
  TimerCH_EnableAsOutput(Timer_CH0_BIT); 
  //Arms timer for the first time, 5[ms]
  TC0 = (uint)TCNT + 50000u; 
  //toggle action TC0
  TCTL2 = TCTL2_OL0_MASK; 

  //intializing switches, LEDs, LCD, & 7-Seg
  SwLED_Init();
  lcd_Init();
  SevSeg_Init();
  /********************************************************************/
  // main program loop
  /********************************************************************/
  
  //placing strings on LCD
  lcd_StringXY(1, 16, "Gach");
  lcd_StringXY(2, 0, "CMPE2250");
  lcd_StringXY(3, 2, "0000000000000000");

  for (;;)
  {
    unsigned int temp;
    unsigned int i;
    unsigned int digits[4];
    unsigned int orary = 0;

    //button switch state init
    Sw_Process(&left, SW_LEFT);
    Sw_Process(&mid, SW_MID);
    Sw_Process(&right, SW_RIGHT);

    //displaying count on lower 7-Seg
    SevSeg_Bot4(HexToBCD(display));

    //displaying HEX count on LCD
    {
      char dis[4];
      (void)sprintf(dis, "%04X", display);
      lcd_StringXY(0, 0, dis);
    }
    {
      char dis[4];
      (void)sprintf(dis, "%04u", 9999 - display);
      lcd_StringXY(0, 16, dis);
    }

    //checking if left button is held to display active
    //HEX count on top 7-Seg and clearing top 7-Seg 
    //when left button is released
    if(left == Held)
      SevSeg_Top4(display);
    else if(left == Released)
      SevSeg_ClearTop4();

    //checking if mid button is held to display actively
    //the difference between 9999 and the current count
    //on top 7-Seg and clearing top 7-Seg when middle 
    //button is released 
    if(mid == Held)
      SevSeg_Top4(HexToBCD(9999 - display));
    else if(mid == Released)
      SevSeg_ClearTop4();

    //using a temp variable equal to the visible (display)
    //count for the sake of manipulation; adds each digit
    //of the current count to an unsigned integer array
    //(the lowest digit in position 0 and the highest in 3)
    temp = display;
    for(i = 0; i < 4; ++i) {
      digits[i] = temp % 10;
      digits[i] |= 0x80; //ensures no decimal place
      temp = temp / 10;
    }

    //checking if the right button is held to display
    //the cursed custom count on the top of the 7-Seg
    //and clearing the top 7-Seg when the right button
    //is released
    if(right == Held) 
      for(i = 0; i < 4; ++i) 
        SevSeg_Cust((unsigned char)3-i, (unsigned char)digits[i]);
    else if (right == Released) 
      SevSeg_ClearTop4();
    
    //in the most convoluted way possible, displays
    //visible (display) count in binary
    orary = display + 1;
    for(i = 0; i < 16; ++i) {
      if((orary - pow(2, 15 - i)) > 0) {
        orary -= (unsigned char)(1 << (15 - i));
        lcd_StringXY(3, i + 2, "1");
      }
      else 
        lcd_StringXY(3, i + 2, "0");
    }
    
    //timer timer thingz; 5ms intervals
    if(TFLG1 & TFLG1_C0F_MASK) { //check flag
      ++counter;                 //count
      TC0 += 50000u;              //re-arm
      TFLG1 = TFLG1_C0F_MASK;    //clear flag

      if(counter >= 100) {       //check count
        ++display;               //update display count
        counter = 0;             //reset count
        if(display > 9999)       //wrapping count
          display = 0;     
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
