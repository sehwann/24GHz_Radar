/*
 * Copyright (c) 2018-2019 Socionext Inc.
 * All rights reserved.
 */

/*
 * rs_dev_opcode.h
 */
#ifndef __RS_DEV_OPCODE_H__
#define __RS_DEV_OPCODE_H__

#define RS_CTL_OP_HARD_RESET					(0xC7)
#define RS_CTL_OP_SOFT_RESET					(0xAB)
#define RS_CTL_OP_DEEP_SLEEP					(0xB9)
#define RS_CTL_OP_MODECTL						(0x01)
#define RS_CTL_OP_READ_DATA						(0x0B)
#define RS_CTL_OP_WRITE_DATA					(0x02)
#define RS_CTL_OP_READ_STATUS_REG				(0x05)
#define RS_CTL_OP_STATUS_SEQ_BUSY				(0x1<<0)
#define RS_CTL_OP_STATUS_SEQ_ERR				(0x1<<1)
#define RS_CTL_OP_STATUS_CONT_SING			(0x1<<2)
#define RS_CTL_OP_STATUS_START_STOP			(0x1<<3)
#define RS_CTL_OP_STATUS_UNDEF_COM			(0x1<<4)
#define RS_CTL_OP_STATUS_FIFO_OVF				(0x1<<5)
#define RS_CTL_OP_STATUS_FIFO_UDF				(0x1<<6)
#define RS_CTL_OP_STATUS_FIFO_OR				(0x1<<7)
#define RS_CTL_OP_READ_STATUS_REG2				(0x0F)
#define RS_CTL_OP_STATUS2_DETOUT				(0x1<<0)
#define RS_CTL_OP_ENABLE_TIMER					(0x11)
#define RS_CTL_OP_DISABLE_TIMER					(0x10)
#define RS_CTL_OP_RUN_TIMER						(0x13)
#define RS_CTL_OP_STOP_TIMER					(0x12)
#define RS_CTL_OP_HOLD_DATA						(0x15)
#define RS_CTL_OP_UPDATE_DATA					(0x14)

#define RS_CTL_MODE_SINGLE_SHOT					(0x08)
#define RS_CTL_MODE_CONT_START					(0x0C)
#define RS_CTL_MODE_CONT_STOP					(0x04)

#endif	/* __RS_DEV_OPCODE_H__ */
