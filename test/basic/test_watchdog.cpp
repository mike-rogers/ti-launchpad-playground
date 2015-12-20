#include <gtest/gtest.h>
#include "watchdog.h"

/*
  Composing WSTCTL's expected value (16-bit)
  Using 0x05a for WDTPW  : 0x5a << 8
  Using 1     for WDTHOLD: 0x01 << 7
  Result                 : 0x5a80
 */
#define EXPECTED_WATCHDOG_VALUE 0x5a80

TEST(watchdog_test, should_create_and_disable) {
    uint16_t watchdogRegister;
    tdd_status_t status;

    // Code under test:
    status = watchdog_createAndDisable(&watchdogRegister);

    // Assertions:
    ASSERT_EQ(TDD_STATUS_SUCCESS, status);
    ASSERT_EQ(watchdogRegister, EXPECTED_WATCHDOG_VALUE);
}
