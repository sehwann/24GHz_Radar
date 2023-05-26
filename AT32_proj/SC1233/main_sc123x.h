//-----------------------------------------------------------------------------
// File Name         : main_sc123x.h
//-----------------------------------------------------------------------------
// @brief This file contains the structure for controlling the SC1233 module.
//		  This file is part of the PETABREW project.
// Copyright (C) 2023 PETABREW. sehwan.park
//-----------------------------------------------------------------------------
#ifndef __MAIN_SC123X_H__
#define __MAIN_SC123X_H__
//-----------------------------------------------------------------------------
// Include
//-----------------------------------------------------------------------------
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "general.h"
#include "rs_dev_opcode.h"
#include "rs_dev_addr.h"
//-----------------------------------------------------------------------------
// Global typedefs & definitions
//-----------------------------------------------------------------------------
#define TRUE 1
#define FALSE 0
/**
 * @brief It can be used not only in distance mode, but also in other modes.
 *        Code modification is required.
 *        This project uses distance mode as the basis.
 */
#define RS_MODE_MOTION_DETECT					(0)
#define RS_MODE_DISTANCE_DETECT                 (1)
#define RS_MODE_2D_DETECT                       (2)
//-----------------------------------------------------------------------------
// Structure
//-----------------------------------------------------------------------------
struct rs_code_ref_mem_s
{
	const uint8_t *data;
	uint32_t size;
};

struct rs_code_ref_regs_s
{
	const uint16_t *addr;
	const uint32_t *data;
	uint32_t num;
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

typedef struct _rs_handle
{
	int32_t state;
	int32_t mode;
	uint8_t peak_level_lower;
    struct rs_code_ref_s code_ref;
	int32_t sensor_ctl_type;
    void* param;
} rs_handle;


struct rs_distance_data
{
	int16_t										distance[5];
	uint8_t										peak_level[5];
	int16_t										peak_real[5];
	int16_t										peak_imag[5];
};

struct rs_distance_param
{
	uint32_t									rx_gain;
	uint32_t									interval;
	uint8_t										beta;
	uint32_t									lower_distance;
	uint32_t									upper_distance;
	uint32_t									chirp_time;
	int32_t 									digital_beam_shaper;
	int32_t 									high_pass_filter;
	int32_t									    distance_step;
};

struct initial_dataset
{
	/** sequencer program code */
	const uint8_t *seq;
	const uint32_t seq_size;
	/** initial register addresses */
	const uint16_t *reg_addr;
	/** initial register values */
	const uint32_t *reg_val;
	const uint32_t reg_size;
	/** FFT window function */
	const uint8_t *fftwin;
	const uint32_t fftwin_size;
	/** Timer Operation */
	const uint8_t timer_operation;
};

#endif
