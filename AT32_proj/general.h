//-----------------------------------------------------------------------------
// File Name         : chipset.h
//-----------------------------------------------------------------------------
// @brief This file contains transformations of the collected data.
// Copyright (C) 2023 PETABREW. sehwan.park
//-----------------------------------------------------------------------------
#ifndef __GENERAL_H__
#define __GENERAL_H__
//-----------------------------------------------------------------------------
// Include
//-----------------------------------------------------------------------------
#include <stdint.h>
//-----------------------------------------------------------------------------
// Definitions
//-----------------------------------------------------------------------------
#define CRC16_POLY								(0x1021)
//-----------------------------------------------------------------------------
// Function prototypes
//-----------------------------------------------------------------------------
void uint32_to_uint8(uint32_t src, uint8_t dst[4]);
void uint32_to_uint16(uint32_t src, uint16_t dst[2]);
void uint8_to_uint32(uint8_t src[4], uint32_t *dst);

uint16_t get_expected_crc(uint32_t addr, const uint8_t *data, uint32_t size);
uint16_t rs_calc_crc16(uint16_t crc16, const uint8_t *data, uint32_t size);

#endif
