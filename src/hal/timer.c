#include <string.h> // for memcpy

#include "timer.h"

/*
  Composing TACTL's default value (16-bit)
  Using ACLK  for TASSELx: 0x01 << 8
  Using /1    for IDx    : 0x00 << 6
  Using stop  for MCx    : 0x00 << 4
  Using false for TACLR  : 0x00 << 2
  Using true  for TAIE   : 0x01 << 1
  Result                 : 0x0102
*/
#define TIMER_TACTL_DEFAULT_VALUE 0x0102

/*
  CCIE (0x0010): Capture/compare interrupt enable
 */
#define TIMER_TACCTL_DEFAULT_VALUE 0x0010

/*
  For now, ACLK is operating at 12KHz and we want a 1-second
  pulse.
 */
#define TIMER_TACCR_DEFAULT_VALUE 12000

/*
  Composing TACTL's STOP and START masks
  Using timer modes:      << 4
    - STOP         : 0x00
    - UP           : 0x01
    - CONTINUOUS   : 0x02
    - UP/DOWN      : 0x03
  For a 2-bit field, ALL is: 0x03 << 4
*/
#define TIMER_TACTL_START_MASK (0x01 << 4)
#define TIMER_TACTL_STOP_MASK (0x03 << 4)

typedef enum {
  TIMER_STATE_UNCONFIGURED,
  TIMER_STATE_INITIALIZED,
  TIMER_STATE_RUNNING,
  TIMER_STATE_STOPPED
} tdd_timerState_e;

typedef tdd_timerState_e tdd_timerState_t;

static tdd_timerState_t g_currentState = TIMER_STATE_UNCONFIGURED;
static timer_params_s g_params;

static void timer_saveConfiguration(timer_params_s *params);
static void timer_activateConfiguration(void);
static void timer_internalStart(void);
static void timer_internalStop(void);

tdd_status_t tdd_timer_create(timer_params_s *params)
{
  tdd_status_t status;

  switch (g_currentState) {

  case TIMER_STATE_UNCONFIGURED:
    timer_saveConfiguration(params);
    timer_activateConfiguration();
    g_currentState = TIMER_STATE_INITIALIZED;
    status = TDD_STATUS_SUCCESS;
    break;

  default:
    status = TDD_STATUS_ERROR_ALREADY_CONFIGURED;
    break;
  }

  return status;
}

tdd_status_t timer_destroy()
{
  tdd_status_t status;

  switch (g_currentState) {

  case TIMER_STATE_UNCONFIGURED:
    status = TDD_STATUS_ERROR_NOT_CONFIGURED;
    break;

  default:
    g_currentState = TIMER_STATE_UNCONFIGURED;
    status = TDD_STATUS_SUCCESS;
    break;
  }

  return status;
}

tdd_status_t timer_start()
{
  tdd_status_t status = TDD_STATUS_ERROR_UNKNOWN;

  switch (g_currentState) {

  case TIMER_STATE_RUNNING:
    status = TDD_STATUS_TIMER_ALREADY_RUNNING;
    break;

  case TIMER_STATE_UNCONFIGURED:
    status = TDD_STATUS_ERROR_NOT_CONFIGURED;
    break;

  case TIMER_STATE_INITIALIZED:
  case TIMER_STATE_STOPPED:
    timer_internalStart();
    g_currentState = TIMER_STATE_RUNNING;
    status = TDD_STATUS_SUCCESS;
    break;
  }

  return status;
}

tdd_status_t timer_stop()
{
  tdd_status_t status = TDD_STATUS_ERROR_UNKNOWN;

  switch (g_currentState) {

  case TIMER_STATE_UNCONFIGURED:
    status = TDD_STATUS_ERROR_NOT_CONFIGURED;
    break;

  case TIMER_STATE_STOPPED:
  case TIMER_STATE_INITIALIZED:
    status = TDD_STATUS_TIMER_NOT_RUNNING;
    break;

  case TIMER_STATE_RUNNING:
    g_currentState = TIMER_STATE_STOPPED;
    timer_internalStop();
    status = TDD_STATUS_SUCCESS;
    break;
  }

  return status;
}

tdd_status_t timer_restart()
{
  tdd_status_t status = TDD_STATUS_ERROR_UNKNOWN;

  switch (g_currentState) {

  case TIMER_STATE_STOPPED:
    g_currentState = TIMER_STATE_RUNNING;
    status = TDD_STATUS_SUCCESS;
    timer_internalStart();
    break;

  case TIMER_STATE_UNCONFIGURED:
    status = TDD_STATUS_ERROR_NOT_CONFIGURED;
    break;

  case TIMER_STATE_RUNNING:
    status = TDD_STATUS_TIMER_ALREADY_RUNNING;
    break;

  case TIMER_STATE_INITIALIZED:
    status = TDD_STATUS_TIMER_CANNOT_RESTART;
    break;
  }
      
  return status;
}

tdd_bool_t timer_isRunning()
{
  tdd_bool_t result;

  switch (g_currentState) {

  case TIMER_STATE_RUNNING:
    result = TDD_TRUE;
    break;

  default:
    result = TDD_FALSE;
    break;
  }

  return result;
}

static void timer_saveConfiguration(timer_params_s *params)
{
  memcpy(&g_params, params, sizeof(timer_params_s));
}

static void timer_activateConfiguration()
{
  *(g_params.timerControl_r) = TIMER_TACTL_DEFAULT_VALUE;
  *(g_params.timerCaptureControl_r) = TIMER_TACCTL_DEFAULT_VALUE;
  *(g_params.timerCaptureCompare_r) = TIMER_TACCR_DEFAULT_VALUE;
}

static void timer_internalStart()
{
  *(g_params.timerControl_r) |= TIMER_TACTL_START_MASK;
}

static void timer_internalStop()
{
  *(g_params.timerControl_r) &= ~TIMER_TACTL_STOP_MASK;
}

void timer_isr() {
  if (g_params.interruptFunction != NULL) {
    g_params.interruptFunction();
  }
}
