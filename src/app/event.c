#include <string.h> // for memcpy

#include "event.h"

#define EVENT_NOT_IN_USE 0xFFFFFFFF

typedef struct {
  event_cb_t callback;
  uint32_t numberOfInterrupts;
} event_countdown_s;

static event_countdown_s g_events[MAX_NUMBER_OF_EVENTS];

tdd_status_t event_create(timer_params_s *params)
{
  return timer_create(params);
}

tdd_status_t event_destroy()
{
  int i;

  for (i = 0; i < MAX_NUMBER_OF_EVENTS; i++) {
    g_events[i].numberOfInterrupts = EVENT_NOT_IN_USE;
  }

  return TDD_STATUS_SUCCESS;
}

tdd_status_t event_registerCallback(event_cb_t callback, uint32_t numberOfInterrupts)
{
  tdd_status_t status;
  event_countdown_s tempEvent;
  int i = 0;

  tempEvent.callback = callback;
  tempEvent.numberOfInterrupts = numberOfInterrupts;

  while (i < MAX_NUMBER_OF_EVENTS && g_events[i].numberOfInterrupts != EVENT_NOT_IN_USE) {
    i++;
  }

  if (i == MAX_NUMBER_OF_EVENTS) {
    status = TDD_STATUS_EVENT_CANNOT_REGISTER;
  } else {
    status = TDD_STATUS_SUCCESS;
    memcpy(g_events + i, &tempEvent, sizeof(event_countdown_s));
  }

  return status;
}

// TODO: This should be registered within this module to be called
// whenever the associated TIMERA triggers an interrupt.
void event_tick()
{
  int i;

  for (i = 0; i < MAX_NUMBER_OF_EVENTS; i++) {
    if (g_events[i].numberOfInterrupts != EVENT_NOT_IN_USE) {
      g_events[i].numberOfInterrupts--;

      if (g_events[i].numberOfInterrupts == 0) {
        g_events[i].callback();
      }
    }
  }
}
