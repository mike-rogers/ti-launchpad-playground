#include "watchdog.h"

/*
  Per TI's documentation, whenever the WDTCTL register is
  written to the upper 8 bits must include the WDTPW.
 */
#define WATCHDOG_PASSWORD (0x5a << 8)
#define WATCHDOG_APPLY(x) (WATCHDOG_PASSWORD | (0xff & (x)))

/*
  Composing WDTCTL's disable value (8-bit)
  Using true  for WDTHOLD: 0x01 << 7
  Result                 : 0x80
*/
#define WATCHDOG_DISABLE 0x80

tdd_status_t watchdog_createAndDisable(uint16_t *reg)
{
  *reg = WATCHDOG_APPLY(WATCHDOG_DISABLE);

  return TDD_STATUS_SUCCESS;
}
