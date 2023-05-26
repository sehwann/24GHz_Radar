/*
 * Copyright (c) 2018-2019 Socionext Inc.
 * All rights reserved.
 */

/**
 * @file rs_ctl_dev.h
 * @brief device level adaptation layer for Sensor
 * @version (PRELIMINARY)
 * @warning This is a part of sensor library source code for Evaluation Kit.
 */
#ifndef __RS_CTL_DEV_H__
#define __RS_CTL_DEV_H__

#include "main_sc123x.h"
#include "sc123x_gpio.h"
/*
 * Function Definition
 */
void rs_ctl_dev_write(uint8_t* wdata, uint32_t wsize);
void rs_ctl_write_reg(uint32_t addr, uint32_t data);
void rs_ctl_write_regs(uint32_t addr, const uint32_t *data, uint32_t num);
void rs_ctl_write_mem(uint32_t addr, const uint8_t *data, int32_t size);
void rs_ctl_write_mem_seq(uint32_t addr, const uint8_t *data, int32_t size);

void rs_ctl_op_fast_control(uint8_t opcode);
void rs_ctl_op_SRST(void);
void rs_ctl_op_DSLEEP(void);
void rs_ctl_op_ENATM(void);
void rs_ctl_op_RUNTM(void);
void rs_ctl_op_RDSR(uint8_t *data);
void rs_ctl_op_RDSR2(uint8_t *data);
void rs_ctl_op_WRSR(uint8_t data);
void rs_ctl_op_HLDDT(void);
void rs_ctl_op_UPDDT(void);

void rs_ctl_dev_read(uint8_t* wdata, uint32_t wsize, uint8_t* rdata, uint32_t rsize);
void rs_ctl_read_reg(uint32_t addr, uint32_t* data);
void rs_ctl_read_regs(uint32_t addr, uint32_t *data, uint32_t num);
void rs_ctl_op_read_region(uint32_t address, uint8_t *data, uint32_t size);


void read_modify_write_reg(uint16_t addr, uint32_t mask, uint32_t data);

void rs_ctl_load_seq_check(void);


#endif
