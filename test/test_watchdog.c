#include "unity.h"
#include "watchdog.h"

/*
  Composing WSTCTL's expected value (16-bit)
  Using 0x05a for WDTPW  : 0x5a << 8
  Using 1     for WDTHOLD: 0x01 << 7
  Result                 : 0x5a80
 */
#define EXPECTED_WATCHDOG_VALUE 0x5a80


void setUp(void)
{
  // empty
}

void tearDown(void)
{
  // empty
}

void test_watchdog_should_create_successfully(void)
{
  uint16_t watchdogRegister;
  tdd_status_t status;

  // Code under test:
  status = watchdog_create_and_disable(&watchdogRegister);

  // Assertions:
  TEST_ASSERT_EQUAL(TDD_STATUS_SUCCESS, status);
  TEST_ASSERT_EQUAL(watchdogRegister, EXPECTED_WATCHDOG_VALUE);
}
