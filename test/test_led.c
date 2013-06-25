#include "unity.h"
#include "led.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_led_should_initialize_led_pin_and_shut_up_fuller(void)
{
  led_params_s params;
  uint8_t ioControl, output;
  uint8_t pin = 3;

  params.ioControl = &ioControl;
  params.output = &output;
  params.pin = pin;

  led_initialize(&params);

  TEST_ASSERT_EQUAL(1 << pin, ioControl);
  TEST_ASSERT_NOT_EQUAL(1 << pin, output);
}

void test_led_should_toggle(void)
{
  led_params_s params;
  uint8_t ioControl, output;
  uint8_t pin = 3;

  params.ioControl = &ioControl;
  params.output = &output;
  params.pin = pin;

  led_initialize(&params);

  led_toggle(&params);

  TEST_ASSERT_EQUAL(1 << pin, output);
}
