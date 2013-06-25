#include <msp430.h>

/*
  Gradually replacing all macros with TDD'd functionality.
*/

#include "watchdog.h"
#include "timer.h"
#include "led.h"

static void timerInterruptFunction(void);

static led_params_s led0, led6;

int main(void)
{
  timer_params_s timerParams;

  timerParams.timerControl_r = (uint16_t *) TACTL_;
  timerParams.timerCaptureControl_r = (uint16_t *) TACCTL0_;
  timerParams.timerCaptureCompare_r = (uint16_t * ) TACCR0_;
  timerParams.interruptFunction = timerInterruptFunction;

  watchdog_createAndDisable((uint16_t *) WDTCTL_);
  timer_create(&timerParams);

  led0.ioControl = (uint8_t *) P1DIR_;
  led0.output = (uint8_t *) P1OUT_;
  led0.pin = 0;

  led6.ioControl = (uint8_t *) P1DIR_;
  led6.output = (uint8_t *) P1OUT_;
  led6.pin = 6;

  led_initialize(&led0);
  led_initialize(&led6);
  led_toggle(&led0);

  BCSCTL3 |= LFXT1S_2;	// Source auxiliary clock from VL0

  __enable_interrupt();

  timer_start();

  while (1) {
    __no_operation();
  }
}

static void timerInterruptFunction(void)
{
  led_toggle(&led0);
  led_toggle(&led6);
}

__attribute__((interrupt(TIMERA0_VECTOR)))
void Timer_A(void) {
  timer_isr();
}
