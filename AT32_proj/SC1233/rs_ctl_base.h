#ifndef __RS_CTL_BASE_H__
#define __RS_CTL_BASE_H__

#include "main_sc123x.h"
#include "rs_ctl_cmd.h"

#define RS_STATE_SHUTDOWN						(0)
#define RS_STATE_READY							(1)
#define RS_STATE_SENSING						(2)

#define RS_PEAK_LEVEL_LOWER_MIN					(1)
#define RS_PEAK_LEVEL_LOWER_MAX					(96)
#define RS_PEAK_LEVEL_LOWER_DEFAULT				(1)

#define RS_MODE_INVALID							(-1)
#define RS_CONTINUOUS_STOP_WAIT_SEC				(1)


void rs_open(rs_handle* handle);
void rs_start_body(rs_handle* handle, uint8_t resume);


#endif
