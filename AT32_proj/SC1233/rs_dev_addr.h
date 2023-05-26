/*
 * Copyright (c) 2018-2019 Socionext Inc.
 * All rights reserved.
 */

/*
 * rs_dev_addr.h
 */
#ifndef __RS_DEV_ADDR_H__
#define __RS_DEV_ADDR_H__

#define RS_CTL_REG_IF1_1						(0x000a)
#define RS_CTL_REG_IF1_2						(0x000b)
#define RS_CTL_REG_IF2_1						(0x0011)
#define RS_CTL_REG_IF2_2						(0x0012)
#define RS_CTL_REG_SELSET_F_4					(0x0040)
#define RS_CTL_REG_SELSET_C_3					(0x0043)
#define RS_CTL_REG_PRESDET_00					(0x0044)
#define RS_CTL_REG_PRESDET_01					(0x0045)
#define RS_CTL_REG_PRESDET_03					(0x0047)
#define RS_CTL_REG_PRESDET_19					(0x0057)
#define RS_CTL_REG_PRESDET_56					(0x007c)
#define RS_CTL_REG_PRESDET_60					(0x0080)
#define RS_CTL_REG_SEQCONT_1					(0x0090)
#define RS_CTL_REG_SEQCONT_2					(0x0091)
#define RS_CTL_REG_CLKGEN						(0x009b)
#define RS_CTL_REG_CLKGEN_SEQ_ENABLE			(0x001FFC77)
#define RS_CTL_REG_CLKGEN_TIMER_ENABLE		(0x10000000)
#define RS_CTL_REG_CPUIF_4						(0x00a1)
#define RS_CTL_REG_FRM_1						(0x00a4)
#define RS_CTL_REG_FRM_2						(0x00a5)
#define RS_CTL_REG_FRM_3						(0x00a6)
#define RS_CTL_REG_DIG1MEM_3					(0x00aa)
#define RS_CTL_REG_SELDIG_03					(0x00ad)
#define RS_CTL_REG_SELDIG_06					(0x00b0)
#define RS_CTL_REG_SELDIG_09					(0x00b3)
#define RS_CTL_REG_RD_START_EFUSE				(0x00be)
#define RS_CTL_REG_EN_SEQ						(0x00bf)
#define RS_CTL_REG_CPUIF_MON_2					(0x00cc)
#define RS_CTL_REG_REVISION_MON					(0x00cd)
#define RS_CTL_REG_EFUSE37						(0x00f6)
#define RS_CTL_REG_PEAK_LEVEL_RX1_1				(0x019b)
#define RS_CTL_REG_PEAK_LEVEL_RX1_2				(0x019c)
#define RS_CTL_REG_PEAK_LEVEL_RX1_3				(0x019d)
#define RS_CTL_REG_PEAK_LEVEL_RX1_4				(0x019e)
#define RS_CTL_REG_PEAK_LEVEL_RX1_5				(0x019f)
#define RS_CTL_REG_MY_COMPLEX_RX1_1				(0x01a0)
#define RS_CTL_REG_MY_COMPLEX_RX1_2				(0x01a1)
#define RS_CTL_REG_MY_COMPLEX_RX1_3				(0x01a2)
#define RS_CTL_REG_MY_COMPLEX_RX1_4				(0x01a3)
#define RS_CTL_REG_MY_COMPLEX_RX1_5				(0x01a4)
#define RS_CTL_REG_DISTANCE_RX1_12				(0x01bc)
#define RS_CTL_REG_DISTANCE_RX1_34				(0x01bd)
#define RS_CTL_REG_DISTANCE_RX1_5				(0x01be)

#define RS_CTL_TOPADDR_MASK						(0xff0000)
#define RS_CTL_TOPADDR_REG						(0x000000)
#define RS_CTL_TOPADDR_FIFO						(0x010000)
#define RS_CTL_TOPADDR_SEQ						(0x020000)
#define RS_CTL_TOPADDR_FFT						(0x030000)
#define RS_CTL_TOPADDR_PRESDET1					(0x040000)
#define RS_CTL_TOPADDR_PRESDET2					(0x050000)
#define RS_CTL_TOPADDR_PRESDET3					(0x060000)
#define RS_CTL_TOPADDR_PRESDET4					(0x070000)
#define RS_CTL_TOPADDR_PRESDET5					(0x080000)


#endif	/* __RS_DEV_ADDR_H__ */
