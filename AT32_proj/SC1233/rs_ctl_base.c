/*
 * Copyright (c) 2018-2019 Socionext Inc.
 * All rights reserved.
 */

/**
 * @file rs24g_ctl_base.c
 */
#include "rs_ctl_base.h"
/**
 * open Sensor
 *
 * @callgraph
 */
void rs_open(rs_handle* handle) {
    handle->state = RS_STATE_SHUTDOWN;
    handle->mode = RS_MODE_INVALID;
    handle->peak_level_lower = RS_PEAK_LEVEL_LOWER_DEFAULT;
    memset(&(handle->code_ref), 0, sizeof(struct rs_code_ref_s));
    
    rs_ctl_cmd_shutdown();
    rs_ctl_cmd_boot();
    rs_ctl_cmd_shutdown();   
}
/**
 * Start body
 *
 * @callgraph
 */
void rs_start_body(rs_handle* handle, uint8_t resume) {
    rs_ctl_op_SRST();
    vTaskDelay(6);
    {
        uint32_t value;
        if (resume) {
            value = 0x00000100;
        } else {
            value = 0x000009F0;
        }
        rs_ctl_write_reg(RS_CTL_REG_SELDIG_06, value);
    }
    rs_ctl_write_reg(RS_CTL_REG_SELDIG_06, 0x00000000);
    {
        uint8_t status = 0;
        rs_ctl_op_RDSR(&status);
    }    
    
    if(handle->sensor_ctl_type == RS_SENSOR_CTL_TIMER) {
        rs_ctl_write_reg(
            RS_CTL_REG_CLKGEN,
            (RS_CTL_REG_CLKGEN_SEQ_ENABLE | RS_CTL_REG_CLKGEN_TIMER_ENABLE)
        );    
        rs_ctl_op_DSLEEP();
        rs_ctl_op_ENATM();
        rs_ctl_op_RUNTM();        
    } else if(handle->sensor_ctl_type == RS_SENSOR_CTL_CPU) {
        rs_ctl_op_WRSR(RS_CTL_MODE_CONT_START);        
    }
    handle->state = RS_STATE_SENSING;
}
