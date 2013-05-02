#ifndef timer_H
#define timer_H

/**
 * @file timer.h
 * @brief A module to cover TI's TIMERA functionality.
 *
 * This module abstracts away TI's TIMERA implementation and
 * allows the developer to configure (and test) the code in
 * a more intuitive manner.
 *
 * This module currently accepts pointer addresses for TIMERA
 * configuration registers, so it can work with multiple
 * TIMERA instances. However, the LaunchPad only contains one
 * instance of TIMERA.
 */

#include <stdint.h>

#include "types.h"

/**
 * A `typedef` for the function pointers used as callbacks.
 */
typedef void (*timer_cb_t)(void);

/**
 * A `struct` containing configuration information for a TIMERA
 * instance.
 */
typedef struct {
  uint16_t *tactl_r; /**< A pointer to the TACTL register */
  timer_cb_t interruptFunction; /**< The function to be executed on interrupt */
} timer_params_s;

/**
 * The function to be called to create a `timer` singleton. The timers are
 * initialized in a stopped state and you must explicitly call timer_start().
 *
 * @param[in] params A struct containing configuration parameters for a TIMERA.
 * @returns A status indicating success or type of failure.
 */
tdd_status_t timer_create(timer_params_s *params);

/**
 * Returns the `event` singleton to an unconfigured state.
 *
 * @returns A status indicating success or type of failure.
 */
tdd_status_t timer_destroy(void);

/**
 * Start the timer.
 *
 * @returns A status indicating success or type of failure.
 */
tdd_status_t timer_start(void);

/**
 * Stop the timer.
 *
 * @returns A status indicating success or type of failure.
 */
tdd_status_t timer_stop(void);

/**
 * Restarts the timer after having stopped.
 *
 * @returns A status indicating success or type of failure.
 */
tdd_status_t timer_restart(void);

/**
 * Whether or not the timer is currently running.
 *
 * @returns A tdd_bool_t representing whether the timer is running.
 */
tdd_bool_t timer_isRunning(void);

#endif // timer_H
