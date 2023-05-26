#ifndef __RS_CTL_CMD_H__
#define __RS_CTL_CMD_H__

#include "main_sc123x.h"
#include "rs_ctl_dev.h"
#include "rs_dev_addr.h"
#include "sc123x_gpio.h"


#define RS_STATE_SHUTDOWN						(0)
#define RS_STATE_READY							(1)
#define RS_STATE_SENSING						(2)

#define RS_SENSOR_CTL_TIMER						(0)
#define RS_SENSOR_CTL_CPU						(1)

#define SEQ_DISABLE_REG_EN_SEQ					(0x00000000)
#define SEQ_ENABLE_REG_CLKGEN					RS_CTL_REG_CLKGEN_SEQ_ENABLE
#define DISABLE_SEQ_REG_CLKGEN					(0x00000033)
#define SEQ_DISABLE_REG_CLKGEN					(SEQ_ENABLE_REG_CLKGEN ^ DISABLE_SEQ_REG_CLKGEN)
#define SEQ_ENABLE_REG_EN_SEQ					(0x00000001)

void rs_ctl_cmd_shutdown(void);
void rs_ctl_cmd_chipinit(void);
void rs_ctl_cmd_boot(void);
void rs_ctl_cmd_enable_seq(void);
void rs_ctl_cmd_disable_seq(void);


#endif
