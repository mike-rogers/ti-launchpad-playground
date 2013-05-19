#ifndef watchdog_H
#define watchdog_H

/**
 * @file watchdog.h
 * @brief A module for watchdog configuration.
 *
 * This module handles watchdog timer functionality.
 */

#include <stdint.h>

#include "types.h"

/**
 * Creates and disables the watchdog timer.
 * @param[in] reg The address of the watchdog timer control register.
 * @returns A status indicating success or type of failure.
 */
tdd_status_t watchdog_create_and_disable(uint16_t *reg);

#endif // watchdog_H
