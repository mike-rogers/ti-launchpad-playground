#include <stdint.h>

#include "unity.h"
#include "timer.h"

/*
  Composing TACTL's expected value (16-bit)
  Using ACLK  for TASSELx: 0x01 << 8
  Using /1    for IDx    : 0x00 << 6
  Using stop  for MCx    : 0x00 << 4
  Using false for TACLR  : 0x00 << 2
  Using true  for TAIE   : 0x01 << 1
  Result                 : 0x0102
 */
#define EXPECTED_TACTL_VALUE 0x0102

/*
  CCIE (0x0010): Capture/compare interrupt enable
 */
#define EXPECTED_TACCTL_VALUE 0x0010

/*
  For now, ACLK is operating at 12KHz and we want a 1-second
  pulse.
 */
#define EXPECTED_TACCR_VALUE 12000

// Static function to test ISR implementation
static void isrTestCallbackFunction(void);
// Static function to initialize parameter struct
static void timerInitialize(void);

// Static boolean to verify ISR execution
static tdd_bool_t didCallIsrTestCallback;
// Static 'registers' for capturing configuration values
static uint16_t g_tactlRegister, g_tacctlRegister, g_taccrRegister;

void setUp(void)
{
  didCallIsrTestCallback = TDD_FALSE;
}

void tearDown(void)
{
  // empty
}

void test_timer_should_create_successfully(void)
{
  uint16_t tactlRegister, tacctlRegister, taccrRegister;
  tdd_status_t status;
  timer_cb_t timerCallback;
  timer_params_s params;

  // Code under test:
  params.timerControl_r = &tactlRegister;
  params.timerCaptureControl_r = &tacctlRegister;
  params.timerCaptureCompare_r = &taccrRegister;
  params.interruptFunction = timerCallback;
  status = timer_create(&params);

  // Assertions:
  TEST_ASSERT_EQUAL(TDD_STATUS_SUCCESS, status);
  TEST_ASSERT_EQUAL(EXPECTED_TACTL_VALUE, tactlRegister);
  TEST_ASSERT_EQUAL(EXPECTED_TACCTL_VALUE, tacctlRegister);
  TEST_ASSERT_EQUAL(EXPECTED_TACCR_VALUE, taccrRegister);

  // Cleanup:
  timer_destroy();
}

void test_timer_should_destroy_successfully(void)
{
  tdd_status_t status;

  // Preparation:
  timerInitialize();

  // Code under test:
  status = timer_destroy();

  // Assertions:
  TEST_ASSERT_EQUAL(TDD_STATUS_SUCCESS, status);
}

void test_timer_should_fail_to_create_twice(void)
{
  timer_params_s params;
  tdd_status_t status;

  // Preparation:
  timerInitialize();

  // Code under test:
  status = timer_create(&params);

  // Assertions:
  TEST_ASSERT_EQUAL(TDD_STATUS_ERROR_ALREADY_CONFIGURED, status);

  // Cleanup:
  timer_destroy();
}

void test_timer_should_fail_to_destroy_if_not_created(void)
{
  tdd_status_t status;

  // Code under test:
  status = timer_destroy();

  // Assertions:
  TEST_ASSERT_EQUAL(TDD_STATUS_ERROR_NOT_CONFIGURED, status);
}

void test_timer_should_fail_to_start_if_not_created(void)
{
  tdd_status_t status;

  // Code under test:
  status = timer_start();

  // Assertions:
  TEST_ASSERT_EQUAL(TDD_STATUS_ERROR_NOT_CONFIGURED, status);
}

void test_timer_should_start_successfully_once_configured(void)
{
  uint16_t unexpectedTactlRegister, maskedTactlRegister;
  tdd_status_t status;

  // Preparation:
  timerInitialize();

  // Code under test:
  status = timer_start();

  // Assertions:
  unexpectedTactlRegister = 0; // 0x00 = Stop Mode
  maskedTactlRegister = (g_tactlRegister & (0x03 << 4)); // MCx bits
  TEST_ASSERT_EQUAL(TDD_STATUS_SUCCESS, status);
  TEST_ASSERT_NOT_EQUAL(unexpectedTactlRegister, maskedTactlRegister);

  // Cleanup:
  timer_destroy();
}

void test_timer_should_stop_while_running(void)
{
  uint16_t expectedTactlRegister, maskedTactlRegister;
  tdd_status_t status;

  // Preparation:
  timerInitialize();
  timer_start();

  // Code under test:
  status = timer_stop();

  // Assertions:
  expectedTactlRegister = 0; // 0x00 = Stop Mode
  maskedTactlRegister = (g_tactlRegister & (0x03 << 4)); // MCx bits
  TEST_ASSERT_EQUAL(TDD_STATUS_SUCCESS, status);
  TEST_ASSERT_EQUAL(expectedTactlRegister, maskedTactlRegister);

  // Cleanup:
  timer_destroy();
}

void test_timer_should_fail_to_stop_while_stopped(void)
{
  tdd_status_t status;

  // Preparation:
  timerInitialize();
  timer_start();
  timer_stop();

  // Code under test:
  status = timer_stop();

  // Assertions:
  TEST_ASSERT_EQUAL(TDD_STATUS_TIMER_NOT_RUNNING, status);

  // Cleanup:
  timer_destroy();
}

void test_timer_should_fail_to_start_while_running(void)
{
  tdd_status_t status;

  // Preparation:
  timerInitialize();
  timer_start();

  // Code under test:
  status = timer_start();

  // Assertions:
  TEST_ASSERT_EQUAL(TDD_STATUS_TIMER_ALREADY_RUNNING, status);

  // Cleanup:
  timer_destroy();
}

void test_timer_should_fail_to_stop_while_not_running(void)
{
  tdd_status_t status;

  // Preparation:
  timerInitialize();

  // Code under test:
  status = timer_stop();

  // Assertions:
  TEST_ASSERT_EQUAL(TDD_STATUS_TIMER_NOT_RUNNING, status);

  // Cleanup:
  timer_destroy();
}

void test_timer_should_fail_to_stop_while_not_created(void)
{
  tdd_status_t status;

  // Code under test:
  status = timer_stop();

  // Assertions:
  TEST_ASSERT_EQUAL(TDD_STATUS_ERROR_NOT_CONFIGURED, status);
}

void test_timer_should_restart_after_stopping(void)
{
  uint16_t unexpectedTactlRegister, maskedTactlRegister;
  tdd_status_t status;

  // Preparation:
  timerInitialize();
  timer_start();
  timer_stop();

  // Code under test:
  status = timer_restart();

  // Assertions:
  unexpectedTactlRegister = 0; // 0x00 = Stop Mode
  maskedTactlRegister = (g_tactlRegister & (0x03 << 4)); // MCx bits
  TEST_ASSERT_EQUAL(TDD_STATUS_SUCCESS, status);
  TEST_ASSERT_NOT_EQUAL(unexpectedTactlRegister, maskedTactlRegister);

  // Cleanup:
  timer_destroy();
}

void test_timer_should_fail_to_restart_while_running(void)
{
  tdd_status_t status;

  // Preparation:
  timerInitialize();
  timer_start();

  // Code under test:
  status = timer_restart();

  // Assertions:
  TEST_ASSERT_EQUAL(TDD_STATUS_TIMER_ALREADY_RUNNING, status);

  // Cleanup:
  timer_destroy();
}

void test_timer_should_fail_to_restart_before_starting(void)
{
  tdd_status_t status;

  // Preparation:
  timerInitialize();

  // Code under test:
  status = timer_restart();

  // Assertions:
  TEST_ASSERT_EQUAL(TDD_STATUS_TIMER_CANNOT_RESTART, status);

  // Cleanup:
  timer_destroy();
}

void test_timer_should_fail_to_restart_while_not_created(void)
{
  tdd_status_t status;

  // Code under test:
  status = timer_restart();

  // Assertions:
  TEST_ASSERT_EQUAL(TDD_STATUS_ERROR_NOT_CONFIGURED, status);
}

void test_timer_should_know_whether_currently_running(void)
{
  tdd_bool_t result;

  // Preparation:
  timerInitialize();

  // Code under test:
  result = timer_isRunning();

  // Assertions:
  TEST_ASSERT_EQUAL(TDD_FALSE, result);

  // Preparation:
  timer_start();

  // Code under test:
  result = timer_isRunning();

  // Assertions:
  TEST_ASSERT_EQUAL(TDD_TRUE, result);

  // Preparation:
  timer_stop();

  // Code under test:
  result = timer_isRunning();

  // Assertions:
  TEST_ASSERT_EQUAL(TDD_FALSE, result);

  // Cleanup:
  timer_destroy();
}

void test_timer_should_report_not_running_if_not_created(void)
{
  tdd_bool_t result;

  // Code under test:
  result = timer_isRunning();

  // Assertions:
  TEST_ASSERT_EQUAL(TDD_FALSE, result);
}

void test_timer_should_call_interrupt_function_on_isr(void)
{
  // Preparation:
  timerInitialize();

  // Code under test:
  timer_isr();

  // Assertions:
  TEST_ASSERT_EQUAL(TDD_TRUE, didCallIsrTestCallback);

  // Cleanup:
  timer_destroy();
}

static void isrTestCallbackFunction(void)
{
  didCallIsrTestCallback = TDD_TRUE;
}

static void timerInitialize(void)
{
  tdd_status_t status;
  timer_params_s params;

  params.timerControl_r = &g_tactlRegister;
  params.timerCaptureControl_r = &g_tacctlRegister;
  params.timerCaptureCompare_r = &g_taccrRegister;
  params.interruptFunction = isrTestCallbackFunction;

  status = timer_create(&params);

  TEST_ASSERT_EQUAL(TDD_STATUS_SUCCESS, status);
}
