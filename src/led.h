#ifndef led_H
#define led_H

#include <stdint.h>

typedef struct {
  uint8_t *ioControl;
  uint8_t *output;
  uint8_t pin;
} led_params_s;

void led_initialize(led_params_s *params);

void led_toggle(led_params_s *params);

#endif // led_H
