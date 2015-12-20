#ifndef event_H
#define event_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file event.h
 * @brief An eventing module based off TIMERA
 *
 * This module enables the developer to register a callback
 * function to be executed after a given number of interrupts.
 *
 * This module currently acts as a singleton, but it can be
 * refactored to work with multiple timer instances should
 * this code be ported to a TI micro that supports them.
 */

#include "types.h"
#include "timer.h"

/**
 * A constant that defines the maximum number of events that
 * can be registered.
 */
#define MAX_NUMBER_OF_EVENTS 10

/**
 * A `typedef` for the function pointers used as callbacks.
 */
typedef void (*event_cb_t)(void);

/**
 * The function to be called to create the `event` singleton.
 *
 * @param[in] params A struct containing configuration parameters for a TIMERA.
 * @returns A status indicating success or type of failure.
 */
tdd_status_t event_create(timer_params_s *params);

/**
 * Returns the `event` singleton to an unconfigured state.
 *
 * @returns A status indicating success or type of failure.
 */
tdd_status_t event_destroy(void);

/**
 * Registers a callback to be executed after a specific number of interrupts
 * from the associated TIMERA instance.
 *
 * @param callback The function to be called after the number of interrupts.
 * @param numberOfInterrupts The number of interrupts required to invoke the callback.
 * @returns A status indicating success or type of failure.
 */
tdd_status_t event_registerCallback(event_cb_t callback, uint32_t numberOfInterrupts);

/**
 * The `event` singleton function to increase the `tick`. This function
 * should be called whenever the associated TIMERA interrupts.
 */
void event_tick(void);

#ifdef __cplusplus
}
#endif

#endif // event_H
