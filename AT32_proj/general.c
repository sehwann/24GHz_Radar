//-----------------------------------------------------------------------------
// File Name         : chipset.c
//-----------------------------------------------------------------------------
// @brief This file contains transformations of the collected data.
// Copyright (C) 2023 PETABREW. sehwan.park
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "general.h"
#include <stdint.h>

void uint32_to_uint8(uint32_t src, uint8_t dst[4]) {
	dst[0] = (src >> 24) & 0xff;
	dst[1] = (src >> 16) & 0xff;
	dst[2] = (src >> 8) & 0xff;
	dst[3] = (src >> 0) & 0xff;
}

void uint32_to_uint16(uint32_t src, uint16_t dst[2]) {
	dst[0] = (src >> 16) & 0xFFFF;
	dst[1] = (src >> 0) & 0xFFFF;
}

void uint8_to_uint32(uint8_t src[4], uint32_t *dst) {
    *dst = src[0] << 24 | src[1] << 16 | src[2] << 8 | src[3];
}

uint16_t get_expected_crc(uint32_t addr, const uint8_t *data, uint32_t size) {
	uint16_t expected = 0xFFFF;
	uint8_t addr16[2]; 

	addr16[0] = (addr >> 8) & 0xff;
	addr16[1] = (addr >> 0) & 0xff;
	expected = rs_calc_crc16(expected, addr16, 2);
	expected = rs_calc_crc16(expected, data, size);

	return expected;
}

uint16_t rs_calc_crc16(uint16_t crc16, const uint8_t *data, uint32_t size) {
	uint32_t i;
	int j;
	for (i=0; i<size; i++) {
		for (j=7; j>=0; j--) {
			if(((data[i] >> j) & 0x01) ^ ((crc16 >> 15) & 0x0001)) {
				crc16 = (crc16 << 1) ^ CRC16_POLY;
			} else {
				crc16 = (crc16 << 1);
			}
		}
	}
	return crc16;
}

