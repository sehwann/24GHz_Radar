/*
 * Copyright (c) 2018-2019 Socionext Inc.
 * All rights reserved.
 */

/*
 * rs24g_ctl_setup_base.h
 */
#ifndef __RS_CTL_SETUP_H__
#define __RS_CTL_SETUP_H__

#include "main_sc123x.h"
#include "rs_ctl_cmd.h"
#include "rs_ctl_distance.h"

#define RS_DIGITAL_BEAM_SHAPER_WIDE				(0)
#define RS_DIGITAL_BEAM_SHAPER_NARROW			(1)

#define RS_HPF_FIRST_ORDER						(1)
#define RS_HPF_SECOND_ORDER						(2)

#define RS_DISTANCE_STEP_NORMAL					(0)
#define RS_DISTANCE_STEP_FINE					(1)
#define RS_DISTANCE_STEP_XFINE					(2)

#define RS_PEAK_LEVEL_LOWER_MIN					(1)
#define RS_PEAK_LEVEL_LOWER_MAX					(96)
#define RS_PEAK_LEVEL_LOWER_DEFAULT				(1)

#define RS_C								((double)2.998 * 10000000000.0)
/*
 * Function Definition
 */
void _table_read_reg(struct rs_code_ref_s* code, uint16_t addr, uint32_t *data);
float _point_per_chirp(rs_handle* handle, const uint32_t fft_point);
float _effective_chirp_bandwidth(const uint32_t fft_point);

void rs_setup_param(rs_handle* handle);
void rs_setup_param_local(rs_handle* handle);
void rs_setup_getcode(rs_handle* handle, const struct initial_dataset *setup);
void rs_load_seq_code(rs_handle* handle);
void rs_setup_seq(rs_handle* handle);
void rs_update_param_local(rs_handle* handle);

// Detail Setting
void rs_setup_interval(rs_handle* handle, uint32_t interval);
void rs_setup_beta(rs_handle* handle, uint8_t beta);
void rs_setup_hpf(rs_handle* handle, int32_t hpf);
void rs_setup_range(rs_handle* handle, uint16_t addr, uint32_t upper, uint32_t lower, int32_t conv_offset);
void rs_setup_range_peak(rs_handle* handle, uint32_t upper, uint32_t lower);
void rs_set_peak_level_lower(rs_handle* handle, uint8_t level);
void rs_get_peak_level_lower(rs_handle* handle, uint8_t *level);


#endif
