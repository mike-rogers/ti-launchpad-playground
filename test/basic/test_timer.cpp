#include <gtest/gtest.h>
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
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreturn-stack-address"
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

class TestTimerFixture : public ::testing::Test
{
public:
    void SetUp() {
        didCallIsrTestCallback = TDD_FALSE;
    }
};

TEST_F(TestTimerFixture, should_create_successfully)
{
    uint16_t tactlRegister, tacctlRegister, taccrRegister;
    tdd_status_t status;
    timer_cb_t timerCallback = NULL;
    timer_params_s params;

    // Code under test:
    params.timerControl_r = &tactlRegister;
    params.timerCaptureControl_r = &tacctlRegister;
    params.timerCaptureCompare_r = &taccrRegister;
    params.interruptFunction = timerCallback;
    status = tdd_timer_create(&params);

    // Assertions:
    ASSERT_EQ(TDD_STATUS_SUCCESS, status);
    ASSERT_EQ(EXPECTED_TACTL_VALUE, tactlRegister);
    ASSERT_EQ(EXPECTED_TACCTL_VALUE, tacctlRegister);
    ASSERT_EQ(EXPECTED_TACCR_VALUE, taccrRegister);

    // Cleanup:
    timer_destroy();
}

TEST_F(TestTimerFixture, should_destroy_successfully)
{
    tdd_status_t status;

    // Preparation:
    timerInitialize();

    // Code under test:
    status = timer_destroy();

    // Assertions:
    ASSERT_EQ(TDD_STATUS_SUCCESS, status);
}

TEST_F(TestTimerFixture, should_fail_to_create_twice)
{
    timer_params_s params;
    tdd_status_t status;

    // Preparation:
    timerInitialize();

    // Code under test:
    status = tdd_timer_create(&params);

    // Assertions:
    ASSERT_EQ(TDD_STATUS_ERROR_ALREADY_CONFIGURED, status);

    // Cleanup:
    timer_destroy();
}

TEST_F(TestTimerFixture, should_fail_to_destroy_if_not_created)
{
    tdd_status_t status;

    // Code under test:
    status = timer_destroy();

    // Assertions:
    ASSERT_EQ(TDD_STATUS_ERROR_NOT_CONFIGURED, status);
}

TEST_F(TestTimerFixture, should_fail_to_start_if_not_created)
{
    tdd_status_t status;

    // Code under test:
    status = timer_start();

    // Assertions:
    ASSERT_EQ(TDD_STATUS_ERROR_NOT_CONFIGURED, status);
}

TEST_F(TestTimerFixture, should_start_successfully_once_configured)
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
    ASSERT_EQ(TDD_STATUS_SUCCESS, status);
    ASSERT_NE(unexpectedTactlRegister, maskedTactlRegister);

    // Cleanup:
    timer_destroy();
}

TEST_F(TestTimerFixture, should_stop_while_running)
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
    ASSERT_EQ(TDD_STATUS_SUCCESS, status);
    ASSERT_EQ(expectedTactlRegister, maskedTactlRegister);

    // Cleanup:
    timer_destroy();
}

TEST_F(TestTimerFixture, should_fail_to_stop_while_stopped)
{
    tdd_status_t status;

    // Preparation:
    timerInitialize();
    timer_start();
    timer_stop();

    // Code under test:
    status = timer_stop();

    // Assertions:
    ASSERT_EQ(TDD_STATUS_TIMER_NOT_RUNNING, status);

    // Cleanup:
    timer_destroy();
}

TEST_F(TestTimerFixture, should_fail_to_start_while_running)
{
    tdd_status_t status;

    // Preparation:
    timerInitialize();
    timer_start();

    // Code under test:
    status = timer_start();

    // Assertions:
    ASSERT_EQ(TDD_STATUS_TIMER_ALREADY_RUNNING, status);

    // Cleanup:
    timer_destroy();
}

TEST_F(TestTimerFixture, should_fail_to_stop_while_not_running)
{
    tdd_status_t status;

    // Preparation:
    timerInitialize();

    // Code under test:
    status = timer_stop();

    // Assertions:
    ASSERT_EQ(TDD_STATUS_TIMER_NOT_RUNNING, status);

    // Cleanup:
    timer_destroy();
}

TEST_F(TestTimerFixture, should_fail_to_stop_while_not_created)
{
    tdd_status_t status;

    // Code under test:
    status = timer_stop();

    // Assertions:
    ASSERT_EQ(TDD_STATUS_ERROR_NOT_CONFIGURED, status);
}

TEST_F(TestTimerFixture, should_restart_after_stopping)
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
    ASSERT_EQ(TDD_STATUS_SUCCESS, status);
    ASSERT_NE(unexpectedTactlRegister, maskedTactlRegister);

    // Cleanup:
    timer_destroy();
}

TEST_F(TestTimerFixture, should_fail_to_restart_while_running)
{
    tdd_status_t status;

    // Preparation:
    timerInitialize();
    timer_start();

    // Code under test:
    status = timer_restart();

    // Assertions:
    ASSERT_EQ(TDD_STATUS_TIMER_ALREADY_RUNNING, status);

    // Cleanup:
    timer_destroy();
}

TEST_F(TestTimerFixture, should_fail_to_restart_before_starting)
{
    tdd_status_t status;

    // Preparation:
    timerInitialize();

    // Code under test:
    status = timer_restart();

    // Assertions:
    ASSERT_EQ(TDD_STATUS_TIMER_CANNOT_RESTART, status);

    // Cleanup:
    timer_destroy();
}

void test_timer_should_fail_to_restart_while_not_created(void)
{
    tdd_status_t status;

    // Code under test:
    status = timer_restart();

    // Assertions:
    ASSERT_EQ(TDD_STATUS_ERROR_NOT_CONFIGURED, status);
}

TEST_F(TestTimerFixture, should_know_whether_currently_running)
{
    tdd_bool_t result;

    // Preparation:
    timerInitialize();

    // Code under test:
    result = timer_isRunning();

    // Assertions:
    ASSERT_EQ(TDD_FALSE, result);

    // Preparation:
    timer_start();

    // Code under test:
    result = timer_isRunning();

    // Assertions:
    ASSERT_EQ(TDD_TRUE, result);

    // Preparation:
    timer_stop();

    // Code under test:
    result = timer_isRunning();

    // Assertions:
    ASSERT_EQ(TDD_FALSE, result);

    // Cleanup:
    timer_destroy();
}

TEST_F(TestTimerFixture, should_report_not_running_if_not_created)
{
    tdd_bool_t result;

    // Code under test:
    result = timer_isRunning();

    // Assertions:
    ASSERT_EQ(TDD_FALSE, result);
}

TEST_F(TestTimerFixture, should_call_interrupt_function_on_isr)
{
    // Preparation:
    timerInitialize();

    // Code under test:
    timer_isr();

    // Assertions:
    ASSERT_EQ(TDD_TRUE, didCallIsrTestCallback);

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

    status = tdd_timer_create(&params);

    ASSERT_EQ(TDD_STATUS_SUCCESS, status);
}

#pragma clang diagnostic pop