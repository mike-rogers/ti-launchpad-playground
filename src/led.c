#include "led.h"

void led_initialize(led_params_s *params)
{
  *(params->ioControl) |= 1 << params->pin;
  *(params->output) &= ~(1 << params->pin);
}

void led_toggle(led_params_s *params)
{
  *(params->output) ^= 1 << params->pin;
}
