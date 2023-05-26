/*
 * Copyright (c) 2019 Socionext Inc.
 * All rights reserved.
 */
#ifndef __RS_HANDLE_H__
#define __RS_HANDLE_H__

#include "rs_types.h"
/*
 * Struct Definition
 */
struct rs_code_ref_mem_s
{
	const uint8_t *data;
	rs_size_t size;
};

struct rs_code_ref_regs_s
{
	const uint16_t *addr;
	const uint32_t *data;
	rs_size_t num;
};

struct rs_code_ref_info_s
{
	uint8_t frame_header;
	uint8_t frame_status;
	uint8_t frame_datasel;
	uint8_t frame_nsec;
	uint32_t frame_size;
};


struct rs_code_ref_s
{
	struct rs_code_ref_mem_s seqc;
	struct rs_code_ref_mem_s winf;
	struct rs_code_ref_regs_s regs;
	struct rs_code_ref_info_s info;
};

struct rs_handle {
    
	rs_state_t state;
	rs_mode_t mode;
	uint8_t peak_level_lower;
	struct rs_code_ref_s code_ref;
	rs_sensor_ctl_type_t sensor_ctl_type;

	struct rs_resource_holder_s res_holder;
};


#endif