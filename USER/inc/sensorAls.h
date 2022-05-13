#ifndef __SEN_ALS_H
#define __SEN_ALS_H

// Replace the STM header according to your MCU
#include "stm32l1xx.h"
#include "bh1750.h"

// 采集周期
#ifdef BH1750_MEASURE_DURATION_MS
#define ALS_COLLECT_DURATION_10MS		(BH1750_MEASURE_DURATION_MS / 10)
#else
#define ALS_COLLECT_DURATION_10MS		12			// 12 * 10ms = 120ms
#endif

// ALS value (e.g. lux)
extern uint16_t g_ALS_value;

// ALS changed from previous sample
extern uint16_t g_ALS_delta;

uint8_t als_init(void);
uint8_t als_IsDataReady(void);
uint8_t als_checkData(void);

#endif /* __SEN_ALS_H */
