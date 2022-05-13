#ifndef __DELAY_H
#define __DELAY_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"

// Additional wasted time (us) for one delay_us() or delay_us_no_wwdg() call
//#define DELAY_OVERHEAD_USEC				14			// delay_us
#define DELAY_OVERHEAD_USEC					4			// delay_us_no_wwdg

void delay_init(void);
void delay_us_no_wwdg(const uint32_t nus);
void delay_us(const uint32_t nus);
void delay_ms(const uint16_t nms);

#ifdef __cplusplus
}
#endif

#endif  /* __DELAY_H */
