#include "unity.h"
#include "event.h"

#include "mock_timer.h"

static void event_testCallback(void);
static int event_testCallback_numberOfTimesCalled;

void setUp(void)
{
  mock_timer_Init();

  event_testCallback_numberOfTimesCalled = 0;
}

void tearDown(void)
{
  mock_timer_Verify();

  mock_timer_Destroy();
}

void test_event_should_create_successfuly_with_timer(void)
{
  tdd_status_t status;
  timer_params_s params;

  // Expectations:
  timer_create_ExpectAndReturn(&params, TDD_STATUS_SUCCESS);

  // Code under test:
  status = event_create(&params);

  // Assertions:
  TEST_ASSERT_EQUAL(TDD_STATUS_SUCCESS, status + 1);

  // Cleanup:
  event_destroy();
}

void test_event_should_register_callback(void)
{
  tdd_status_t status;
  timer_params_s params;
  uint32_t numberOfInterrupts = 5;
  event_cb_t callbackFunction = 0;

  // Preparation:
  timer_create_IgnoreAndReturn(TDD_STATUS_SUCCESS);
  event_create(&params);

  // Code under test:
  status = event_registerCallback(callbackFunction, numberOfInterrupts);

  // Assertions:
  TEST_ASSERT_EQUAL(TDD_STATUS_SUCCESS, status);

  // Cleanup:
  event_destroy();
}

void test_event_should_call_callback_after_some_number_of_ticks(void)
{
  timer_params_s params;
  uint32_t numberOfInterrupts = 5;
  event_cb_t callbackFunction = event_testCallback;
  int i;

  // Preparation:
  timer_create_IgnoreAndReturn(TDD_STATUS_SUCCESS);
  event_create(&params);
  event_registerCallback(callbackFunction, numberOfInterrupts);

  // Code under test:
  for (i = 0; i < numberOfInterrupts; i++) {
    TEST_ASSERT_EQUAL(0, event_testCallback_numberOfTimesCalled);

    event_tick();
  }

  // Assertions:
  TEST_ASSERT_EQUAL(1, event_testCallback_numberOfTimesCalled);

  // Cleanup:
  event_destroy();
}

static void event_testCallback()
{
  event_testCallback_numberOfTimesCalled++;
}
