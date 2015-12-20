//#include <msp430.h>

/*
  Gradually replacing all macros with TDD'd functionality.
*/

#include "watchdog.h"
#include "timer.h"

static void timerInterruptFunction(void);

int main(void)
{
  timer_params_s timerParams;

//  timerParams.timerControl_r = (uint16_t *) TA0CTL_;
//  timerParams.timerCaptureControl_r = (uint16_t *) TA0CCTL0_;
//  timerParams.timerCaptureCompare_r = (uint16_t * ) TA0CCR0_;
  timerParams.interruptFunction = timerInterruptFunction;

//  watchdog_createAndDisable((uint16_t *) WDTCTL_);
  tdd_timer_create(&timerParams);

//  P1DIR |= BIT0 + BIT6;
//  P1OUT |= BIT0 + BIT6;
//
//  BCSCTL3 |= LFXT1S_2;	// Source auxiliary clock from VL0
//
//  __enable_interrupt();

  timer_start();

  while (1) {
//    __no_operation();
  }
}

static void timerInterruptFunction(void)
{
//  P1OUT ^= (BIT0 + BIT6);
}

__attribute__((interrupt(TIMER0_A0_VECTOR)))
void Timer_A(void) {
  timer_isr();
}
