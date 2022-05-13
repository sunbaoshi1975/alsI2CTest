/**
 * \file
 *
 * \brief BH1750 ALS sensor driver implementation
 *
 * This module provides access to the SHTC1 functionality over a generic I2C
 * interface. It supports measurements without clock stretching only.
 *
  */

#include "bh1750.h"

static const uint8_t BH1750_CMD_POWERDOWN = 0x00;
static const uint8_t BH1750_CMD_POWERON = 0x01;
static const uint8_t BH1750_CMD_RESET = 0x07;

#ifdef ALS_I2C_ADDRESS
static const uint8_t BH1750_ADDRESS = ALS_I2C_ADDRESS;
#else
static const uint8_t BH1750_ADDRESS = 0x23;			// 0x23 (ADDR='L') or 0x5C (ADDR='H')
#endif

// _mode = BH1750_MODE_CONT_H_RES
uint8_t bh1750_init(const uint8_t _mode)
{
    if( sensirion_i2c_write(BH1750_ADDRESS, &BH1750_CMD_POWERON, 1) != 0 ) return 1;
    if( sensirion_i2c_write(BH1750_ADDRESS, &_mode, 1) != 0 ) return 1;
    return 0;
}

uint8_t bh1750_read(uint16_t *_pLux)
{
	uint8_t readout_buffer[2];
	if( sensirion_i2c_read(BH1750_ADDRESS, readout_buffer, 2) != 0 ) return 1;
	uint32_t lv_lux = ((readout_buffer[0] << 8) | readout_buffer[1]) * 10 / 12;
	*_pLux = (uint16_t)lv_lux;
	return 0;
}
