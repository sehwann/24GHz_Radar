/*
 * Copyright (c) 2018-2019 Socionext Inc.
 * All rights reserved.
 */

/**
 * @file 
 * @brief 
 * @version 
 * @warning 
 */
#ifndef __RS_CTL_DISTANCE_H__
#define __RS_CTL_DISTANCE_H__

#include "main_sc123x.h"
#include <math.h>
#include "rs_ctl_setup.h"
#include "distance_wide/distance_wide_fine_60db_4400us.h"

#define ARRAY_SIZE(a)							(sizeof(a) / sizeof((a)[0]))

#define RS_DISTANCE_INVALID						((int16_t) (0x7fff))
#define RS_PEAK_LEVEL_INVALID					((uint8_t) (0))

#define RS_DISTANCE_MIN							(0)
#define RS_DISTANCE_MAX							(5600)

//distance
#define RS_DIGITAL_BEAM_SHAPER_WIDE				(0)
#define RS_DIGITAL_BEAM_SHAPER_NARROW			(1)
#define RS_DISTANCE_STEP_NORMAL					(0)
#define RS_DISTANCE_STEP_FINE					(1)
#define RS_DISTANCE_STEP_XFINE					(2)

/*
 * Struct Definition
 */
struct rs_data
{
	uint32_t size;
	const void *data;
};

struct rs_ctl_sensor_data_set
{
	struct
	{
		int32_t enable;

		uint8_t mode;
		uint32_t wordsize_body;
		int32_t exist_header_header;
		int32_t exist_header_status;
		uint32_t bytesize_word;
	} sram;

	struct
	{
		int32_t enable;

		uint8_t mode;
		uint32_t num;
		const uint16_t *addr;
	} reg;

	struct
	{
		int32_t enable;
	} status;
};

struct rs_ctl_sensor_data
{
	struct
	{
		uint8_t *data;
		uint32_t size;

		struct
		{
			uint32_t status;
			uint32_t header;
			struct rs_data body;
			uint32_t endcode;
		} parse;
	} sram;

	struct
	{
		uint32_t *data;
		int32_t num;
	} reg;

	uint8_t status;
};
/*
 * Function Definition
 */
void distance_wide_get(rs_handle* handle);
void distance_wide_update_param(rs_handle* handle, struct rs_distance_param *lp);
void rs_setup_beta(rs_handle* handle, uint8_t beta);

void rs_get_distance(rs_handle* handle, uint32_t timeout, struct rs_distance_data *data);

void get_devdata(rs_handle* handle, uint32_t timeout, const uint16_t *reg_addr, uint32_t reg_size, uint32_t *reg);
void rs_ctl_cmd_wait_and_get_sensor_data(rs_handle* handle, uint32_t timeout, const struct rs_ctl_sensor_data_set *set, struct rs_ctl_sensor_data *data);

void read_status(rs_handle* handle, const struct rs_ctl_sensor_data_set *set, struct rs_ctl_sensor_data *data);
void read_register(rs_handle* handle, const struct rs_ctl_sensor_data_set *set, struct rs_ctl_sensor_data *data);
void read_fifo(rs_handle* handle, const struct rs_ctl_sensor_data_set *set, struct rs_ctl_sensor_data *data);
void rs_ctl_sensor_data_init(struct rs_ctl_sensor_data *data);
void get_distance_data(const uint32_t *regs, struct rs_distance_data *data, uint8_t peak_level_lower);
static uint8_t conv_power(uint32_t val);
static void distance_range(struct rs_distance_data *data, uint8_t peak_level_lower);
void rs_ctl_sensor_data_free(struct rs_ctl_sensor_data *data);


#endif
