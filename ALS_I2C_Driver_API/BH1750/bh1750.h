/**
 * \file
 *
 * \brief BH1750 ALS sensor driver interface
 *
 * This module provides access to the SHT functionality over a generic I2C
 * interface.
 */

#ifndef BH1750_H
#define BH1750_H

#include "sensirion_i2c.h"

#ifdef __cplusplus
extern "C" {
#endif

// BH1750 working mode
#define BH1750_MODE_CONT_H_RES 			0x10
#define BH1750_MODE_CONT_H_RES2			0x11
#define BH1750_MODE_CONT_L_RES 			0x13
#define BH1750_MODE_ONE_H_RES 			0x20
#define BH1750_MODE_ONE_H_RES2 			0x21
#define BH1750_MODE_ONE_L_RES 			0x23

#define BH1750_MEASURE_DURATION_MS		120			// Max. 180ms

uint8_t bh1750_init(const uint8_t _mode);
uint8_t bh1750_read(uint16_t *_pLux);

#ifdef __cplusplus
}
#endif

#endif /* BH1750_H */
