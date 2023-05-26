/*
 * Copyright (c) 2018-2019 Socionext Inc.
 * All rights reserved.
 */
#include "rs_ctl_cmd.h"
/**
 * shutdown Sensor
 *
 * @callgraph
 */
void rs_ctl_cmd_shutdown(void)
{
    NRST_OFF();
    vTaskDelay(5);
    CE_OFF();    
}
/**
 * Chipinit
 *
 * @callgraph
 */
void rs_ctl_cmd_chipinit(void)
{
    CE_OFF();
	NRST_OFF();
	I2C_OFF();
	NSS_HIGH();
    vTaskDelay(100);
	
	CE_ON();
	vTaskDelay(1);
	NRST_ON();
    vTaskDelay(100);
}
/**
 * Boot
 *
 * @callgraph
 */
void rs_ctl_cmd_boot(void) 
{
    rs_ctl_cmd_chipinit();
    
    CE_ON();
    vTaskDelay(100);
    NRST_ON();
    rs_ctl_op_SRST();
    vTaskDelay(50);     
    rs_ctl_write_reg(RS_CTL_REG_RD_START_EFUSE, 0x00000001);
    vTaskDelay(54);     
    rs_ctl_write_reg(RS_CTL_REG_CLKGEN, 0x001FFC44);
    {
        uint32_t rvalue = 0;
        rs_ctl_read_reg(RS_CTL_REG_EFUSE37, &rvalue);
                
        if ((rvalue & 0x0000ff00) == 0x00000200) {
			return;
		}        
        if ((rvalue & 0x0000ff00) == 0x00000300) {            
            rs_ctl_write_reg(RS_CTL_REG_SELSET_F_4, 0x00000000);
		} 
    }
}
/**
 * seq Enable
 *
 * @callgraph
 */
void rs_ctl_cmd_enable_seq(void)
{
	rs_ctl_write_reg(RS_CTL_REG_CLKGEN, SEQ_ENABLE_REG_CLKGEN);
	rs_ctl_op_SRST();
	rs_ctl_write_reg(RS_CTL_REG_EN_SEQ, SEQ_ENABLE_REG_EN_SEQ);
	rs_ctl_op_SRST();
}
/**
 * seq Disable
 *
 * @callgraph
 */
void rs_ctl_cmd_disable_seq(void)
{
    rs_ctl_write_reg(RS_CTL_REG_EN_SEQ, SEQ_DISABLE_REG_EN_SEQ);
	rs_ctl_write_reg(RS_CTL_REG_CLKGEN, SEQ_DISABLE_REG_CLKGEN);
}


