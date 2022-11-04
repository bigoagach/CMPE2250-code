/********************************************************************/
// HC12 Program:  CMPE2250 - PIT + Pending Interrupts
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
/********************************************************************/
// Local Prototypes
/********************************************************************/

/********************************************************************/
// Global Variables
/********************************************************************/

//even tho the assignment asked for these variables to be local
//in the ISRs, the tmrCount starts at a veru large value regardless
//of what i try :(

//Timer Channel 0 Counter
static unsigned int tmrCount;

//PIT Timer Counter
static unsigned int pitCount;

/********************************************************************/
// Constants
/********************************************************************/

//#define PARTA;
//#define PARTB;
#define PARTC;

/********************************************************************/
// Main Entry
/********************************************************************/
#ifdef PARTA
void main(void)
{
  // main entry point
  _DISABLE_COP();
  EnableInterrupts;
  
  /********************************************************************/
  // initializations
  /********************************************************************/
  Clock_Set20MHZ(); //20[MHz] = 50[us]

  //initializing timer; setting prescale
  //enabling Channel 0 as output
  //enabling interrupts
  //10[ms] interval
  Timer_Init(Timer_Prescale_64);
  TimerCH_EnableAsOutput(Timer_CH0_BIT, 3125);
  TimerCH_IntEnable(Timer_CH0_BIT);

  (void)PIT_Init(PIT_Channel_0, PIT_Interrupt_On, 20E6, 10000);    //10[ms]

  SwLED_Init();
  SevSeg_Init();

  tmrCount = 0;
  pitCount = 0;
  /********************************************************************/
  // main program loop
  /********************************************************************/
  for (;;)
  {
    //does not change behaviour of program 
    //but will throttle the main loop 
    asm wai;

    /*
     What is the time between the PIT and timer completing their cycles?
    
     138.9[us] (i was getting 400[ms] before which was scary but that was
     due to my inability to not be stupid :D)

     Q1 – How would you explain your observations, 
     in terms of the time you have observed?
     
     The Timer is faster than the PIT Timer, but the PIT timer is closer
     to the desired time (20.000[ms]) than the timer is. Why this is, I
     don't know (i can definitely look through the notes to find out)
    */
  }                   
}

/********************************************************************/
// Functions
/********************************************************************/

/********************************************************************/
// Interrupt Service Routines
/********************************************************************/
interrupt VectorNumber_Vtimch0 void IOC0(void)
{
  TFLG1 = TFLG1_C0F_MASK;
  //if the count wraps:
  //disable ISR
  //turn on Yellow LED
  //set count to zero
  if(tmrCount > 9999) {
    TIE_C0I = 0;
    LED_On(LED_YELLOW);
    tmrCount = 0;
  }
  
  TC0 += 3125; //10[ms]

  //displaying count on upper line of segs
  SevSeg_Top4(HexToBCD(tmrCount)); 
  tmrCount++;
}

interrupt VectorNumber_Vpit0 void PIT0Int(void)
{
  // clear flag
  PITTF = PITTF_PTF0_MASK; // can't R/W - clears other flags, write only

  //if the count wraps:
  //disable ISR
  //turn on Green LED
  //set count to zero
  if(pitCount > 9999) {
    PITINTE = 0;
    LED_On(LED_GREEN);
    pitCount = 0;
  }
  
  //displaying count on lower line of segs
  SevSeg_Bot4(HexToBCD(pitCount)); 
  pitCount++;
}

#endif

#ifdef PARTB
void main(void)
{// main entry point
  _DISABLE_COP();
  EnableInterrupts;
  
  /********************************************************************/
  // initializations
  /********************************************************************/
  Clock_Set20MHZ(); //20[MHz] = 50[us]

  //initializing timer; setting prescale
  //enabling Channel 0 as output
  //enabling interrupts
  //10[ms] interval
  Timer_Init(Timer_Prescale_64);
  TimerCH_EnableAsOutput(Timer_CH0_BIT, 3125);
  TimerCH_IntEnable(Timer_CH0_BIT);

  (void)PIT_Init(PIT_Channel_0, PIT_Interrupt_On, 20E6, 10000);    //10[ms]

  SwLED_Init();
  SevSeg_Init();

  tmrCount = 0;
  pitCount = 0;
  /********************************************************************/
  // main program loop
  /********************************************************************/
  for (;;)
  {
    asm sei;

    Timer_Sleep_ms(20);
    // LED_On(LED_RED);

    asm cli;

    // LED_Off(LED_RED);

    asm wai;

    /**
     * measurement 4.611s
     * 
     * it does something very strange; it slowers the timer about 3 times
     * as expected but it speeds up the PIT timer signficantly which
     * is very weird
     * 
     * course notes say the lower the vector number in memory, the 
     * higher the priority, so the timer would go off first because
     * the timer is at 8U in memory and the PIT is 66U
     * 
     * 
    */
  }                   
}

/********************************************************************/
// Functions
/********************************************************************/

/********************************************************************/
// Interrupt Service Routines
/********************************************************************/
interrupt VectorNumber_Vtimch0 void IOC0(void)
{
  TFLG1 = TFLG1_C0F_MASK;

  LED_On(LED_RED);

  //if the count wraps:
  //disable ISR
  //turn on Yellow LED
  //set count to zero
  if(tmrCount > 9999) {
    TIE_C0I = 0;
    LED_On(LED_YELLOW);
    tmrCount = 0;
  }
  
  TC0 += 3125; //10[ms]

  //displaying count on upper line of segs
  SevSeg_Top4(HexToBCD(tmrCount)); 
  tmrCount++;
  
  LED_Off(LED_RED);
}

interrupt VectorNumber_Vpit0 void PIT0Int(void)
{
  // clear flag
  PITTF = PITTF_PTF0_MASK; // can't R/W - clears other flags, write only
  
  LED_Off(LED_RED);

  //if the count wraps:
  //disable ISR
  //turn on Green LED
  //set count to zero
  if(pitCount > 9999) {
    PITINTE = 0;
    LED_On(LED_GREEN);
    pitCount = 0;
  }
  
  //displaying count on lower line of segs
  SevSeg_Bot4(HexToBCD(pitCount)); 
  pitCount++;
  
  LED_Off(LED_RED);
}

#endif

#ifdef PARTC
void main(void)
{
  // main entry point
  _DISABLE_COP();
  EnableInterrupts;
  
  /********************************************************************/
  // initializations
  /********************************************************************/
  Clock_Set20MHZ();                      //20[MHz] = 50[us]

  //initializing timer; setting prescale
  //enabling Channel 0 as output
  //enabling interrupts
  //10[ms] interval
  Timer_Init(Timer_Prescale_64);
  TimerCH_EnableAsOutput(Timer_CH0_BIT, 3125);
  TimerCH_IntEnable(Timer_CH0_BIT);
  
  (void)PIT_Init(PIT_Channel_0, PIT_Interrupt_On, 20E6, 10000);    //10[ms]

  SwLED_Init();
  SevSeg_Init();

  tmrCount = 0;
  pitCount = 0;
  /********************************************************************/
  // main program loop
  /********************************************************************/
  for (;;)
  {
    asm wai;

    /**
     * 0.400[s] (big yikes!)
    */
  }                   
}

/********************************************************************/
// Functions
/********************************************************************/

/********************************************************************/
// Interrupt Service Routines
/********************************************************************/
interrupt VectorNumber_Vtimch0 void IOC0(void)
{
  TFLG1 = TFLG1_C0F_MASK;
  //if the count wraps:
  //disable ISR
  //turn on Yellow LED
  //set count to zero
  if(tmrCount > 9999) {
    TIE_C0I = 0;
    LED_On(LED_YELLOW);
    tmrCount = 0;
  }
  
  TC0 = TCNT + 3125; //10[ms]

  //displaying count on upper line of segs
  SevSeg_Top4(HexToBCD(tmrCount)); 
  tmrCount++;
}

interrupt VectorNumber_Vpit0 void PIT0Int(void)
{
  // clear flag
  PITTF = PITTF_PTF0_MASK; // can't R/W - clears other flags, write only

  //if the count wraps:
  //disable ISR
  //turn on Green LED
  //set count to zero
  if(pitCount > 9999) {
    PITINTE = 0;
    LED_On(LED_GREEN);
    pitCount = 0;
  }
  
  //displaying count on lower line of segs
  SevSeg_Bot4(HexToBCD(pitCount)); 
  pitCount++;
}

#endif