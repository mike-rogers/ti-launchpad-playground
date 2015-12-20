#ifndef types_H
#define types_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file types.h
 * @brief Types that are used in this codebase.
 */

/**
 * A boolean enumeration.
 */
typedef enum {
    TDD_TRUE,
    TDD_FALSE
} tdd_bool_e;

/**
 * A boolean type.
 */
typedef tdd_bool_e tdd_bool_t;

/**
 * A status/fail value enumeration.
 */
typedef enum {
    TDD_STATUS_SUCCESS, /**< Function was successful. */
            TDD_STATUS_ERROR_NOT_CONFIGURED, /**< Attempting operation on unconfigured module. */
            TDD_STATUS_ERROR_UNKNOWN, /**< Unknown error. */
            TDD_STATUS_ERROR_ALREADY_CONFIGURED, /**< Attempting to configure a configured module. */
            TDD_STATUS_TIMER_ALREADY_RUNNING, /**< Attempting to start a running timer. */
            TDD_STATUS_TIMER_NOT_RUNNING, /**< Attempting to halt a stopped timer. */
            TDD_STATUS_TIMER_CANNOT_RESTART, /**< Attempting to restart a running or unconfigured timer. */
            TDD_STATUS_EVENT_CANNOT_REGISTER      /**< Unable to register an event. */
} tdd_status_e;

/**
 * A status/fail value type.
 */
typedef tdd_status_e tdd_status_t;

#ifdef __cplusplus
}
#endif

#endif // types_H
