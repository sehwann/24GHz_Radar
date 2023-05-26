/*
 * Copyright (c) 2018-2019 Socionext Inc.
 * All rights reserved.
 */
/*
 * rs_ctl_dev.c
 */
#include "rs_ctl_dev.h"

void rs_ctl_dev_write(uint8_t* wdata, uint32_t wsize)
{
    int32_t TxIdx = 0;

    NSS_LOW();
	while (TxIdx < wsize)
	{
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TE) == RESET);
		SPI_I2S_TxData(SPI1, wdata[TxIdx]);
		TxIdx++;
		vTaskDelay(2);		
		while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BUSY));
	}	    
	NSS_HIGH();
    vTaskDelay(10);
}

void rs_ctl_write_reg(uint32_t addr, uint32_t data)
{	
	uint8_t wdata[8];
	uint8_t send_data[4];
	
	wdata[0] = 0x02;
	wdata[1] = (addr >> 16) & 0xFF;
	wdata[2] = (addr >>  8) & 0xFF;
	wdata[3] = (addr >>  0) & 0xFF;

	uint32_to_uint8(data, send_data);	
	memcpy(&wdata[4], send_data, 4);
    
    rs_ctl_dev_write(wdata, 8);
}

void rs_ctl_write_regs(uint32_t addr, const uint32_t* data, uint32_t num)
{
    int32_t i;
    int32_t wsize = 4 + num * sizeof(uint32_t);
    uint8_t* wdata = (uint8_t*) calloc(num, sizeof(uint32_t));
    uint8_t* wdata_t = (uint8_t*) calloc(wsize, sizeof(uint8_t));

	wdata_t[0] = 0x02;
	wdata_t[1] = (addr >> 16) & 0xff;
	wdata_t[2] = (addr >>  8) & 0xff;
	wdata_t[3] = (addr >>  0) & 0xff;
    
    for (i=0; i<num; ++i) {
        uint32_to_uint8(data[i], &wdata[4*i]);        
    }
    memcpy(&wdata_t[4], wdata, num * sizeof(uint32_t));
    
    rs_ctl_dev_write(wdata_t, wsize);
    free(wdata);        
}

void rs_ctl_write_mem(uint32_t addr, const uint8_t *data, int32_t size)
{
    int32_t wsize = 4 + size;
    uint8_t* wdata = (uint8_t*) calloc(wsize, sizeof(uint8_t));
    
    wdata[0] = 0x02;
	wdata[1] = (addr >> 16) & 0xFF;
	wdata[2] = (addr >>  8) & 0xFF;
	wdata[3] = (addr >>  0) & 0xFF;    
    memcpy(&wdata[4], data, size);
    rs_ctl_dev_write(wdata, wsize);    
    free(wdata); 
}

void rs_ctl_write_mem_seq(uint32_t addr, const uint8_t *data, int32_t size)
{
    int32_t TxIdx = 0;
    int32_t wsize = 4 + size;
    uint8_t* wdata = (uint8_t*) calloc(4, sizeof(uint8_t));
	uint8_t t_data;
    int32_t cnt = 0;
    
    wdata[0] = 0x02;
	wdata[1] = (addr >> 16) & 0xFF;
	wdata[2] = (addr >>  8) & 0xFF;
	wdata[3] = (addr >>  0) & 0xFF;
    
    NSS_LOW();
	while (TxIdx < wsize)
	{
        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TE) == RESET);
        cnt ++;
        if (TxIdx < 4)
        {
            t_data = wdata[TxIdx];
        } else {
            t_data = data[TxIdx-4];
        }
        SPI_I2S_TxData(SPI1, t_data);
		TxIdx++;
        vTaskDelay(2);		
		while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BUSY));
	}
	NSS_HIGH();
    free(wdata);
    vTaskDelay(10);
}


void rs_ctl_op_fast_control(uint8_t opcode)
{
    rs_ctl_dev_write(&opcode, 1);
}

void rs_ctl_op_SRST()
{
    rs_ctl_op_fast_control(RS_CTL_OP_SOFT_RESET);    
}

void rs_ctl_op_DSLEEP()
{
    rs_ctl_op_fast_control(RS_CTL_OP_DEEP_SLEEP);    
}

void rs_ctl_op_ENATM(void)
{
    rs_ctl_op_fast_control(RS_CTL_OP_ENABLE_TIMER);
}

void rs_ctl_op_RUNTM(void)
{
    rs_ctl_op_fast_control(RS_CTL_OP_RUN_TIMER);
}

void rs_ctl_op_WRSR(uint8_t data)
{
    uint8_t wdata[2] = { RS_CTL_OP_MODECTL, data };
	rs_ctl_dev_write(wdata, sizeof(wdata));
}

void rs_ctl_op_RDSR(uint8_t *data)
{
    
 	uint8_t wdata[1] = { RS_CTL_OP_READ_STATUS_REG };
	uint8_t rdata[1] = { 0 };
	rs_ctl_dev_read(wdata, sizeof(wdata), rdata, sizeof(rdata));
	*data = rdata[0];    
}

void rs_ctl_op_RDSR2(uint8_t *data)
{
 	uint8_t wdata[1] = { RS_CTL_OP_READ_STATUS_REG2 };
	uint8_t rdata[1] = { 0 };    
	rs_ctl_dev_read(wdata, sizeof(wdata), rdata, sizeof(rdata));
	*data = rdata[0];
}
void rs_ctl_op_HLDDT(void)
{
    rs_ctl_op_fast_control(RS_CTL_OP_HOLD_DATA);
}
void rs_ctl_op_UPDDT(void)
{
    rs_ctl_op_fast_control(RS_CTL_OP_UPDATE_DATA);
}
void rs_ctl_dev_read(uint8_t* wdata, uint32_t wsize, uint8_t* rdata, uint32_t rsize)
{
    int32_t TxIdx = 0, RxIdx = 0;
    int32_t total_size = wsize + rsize + 2;
    uint8_t* wdata_t = (uint8_t*) calloc(total_size, sizeof(uint8_t));
    uint8_t dummy[2] = { 0x01, 0x01 };
    uint8_t rdata_t = 0x00;
    
    memcpy(&wdata_t[0], wdata, wsize * sizeof(uint8_t));
    memcpy(&wdata_t[wsize], dummy, 2);
    memcpy(&wdata_t[wsize + 2], rdata, rsize * sizeof(uint8_t));    
    
	NSS_LOW();
	
	while (TxIdx < total_size)
	{
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TE) == RESET);
		SPI_I2S_TxData(SPI1, wdata_t[TxIdx++]);
		
		if (TxIdx > wsize + 1)
		{
			while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RNE) == RESET);
            rdata_t = SPI_I2S_RxData(SPI1);
            
            if (RxIdx) {
                rdata[RxIdx-1] = rdata_t;
            }
            RxIdx++;
            vTaskDelay(2);
		}        
        while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BUSY));
	}    
	NSS_HIGH();	  
    free(wdata_t);  
    vTaskDelay(10);    
}

void rs_ctl_read_reg(uint32_t addr, uint32_t* data)
{
    uint8_t wdata[4];
    uint8_t rdata[4];
	
	wdata[0] = 0x0B;
	wdata[1] = (addr >> 16) & 0xff;
	wdata[2] = (addr >>  8) & 0xff;
	wdata[3] = (addr >>  0) & 0xff;
	
    rs_ctl_dev_read(wdata, sizeof(wdata), rdata, sizeof(rdata));
    uint8_to_uint32(rdata, data);
}

void rs_ctl_op_read_region(uint32_t address, uint8_t *data, uint32_t size)
{
    uint8_t wdata[4];
    wdata[0] = RS_CTL_OP_READ_DATA;
    wdata[1] = ((address) >> 16) & 0xff;
    wdata[2] = ((address) >>  8) & 0xff;
    wdata[3] = ((address) >>  0) & 0xff;
    rs_ctl_dev_read(wdata, sizeof(wdata), data, size);
}


void rs_ctl_read_regs(uint32_t addr, uint32_t* data, uint32_t num)
{
    int32_t i;
    int32_t wsize = 6 + (num * sizeof(uint32_t));
    int32_t TxIdx = 0, RxIdx = 0;    
    uint8_t* wdata_t = (uint8_t*) calloc(wsize, sizeof(uint8_t));
    uint8_t* wdata = (uint8_t*) calloc(num, sizeof(uint32_t));
    uint8_t* rdata = (uint8_t*) calloc(num, sizeof(uint32_t));
    
    for (i=0; i<num; ++i) {
        uint32_to_uint8(data[i], &wdata[4*i]);
    }
    wdata_t[0] = 0x0B;
	wdata_t[1] = (addr >> 16) & 0xff;
	wdata_t[2] = (addr >>  8) & 0xff;
	wdata_t[3] = (addr >>  0) & 0xff;
    wdata_t[4] = 0x01;
    wdata_t[5] = 0x01;

    memcpy(&wdata_t[6], wdata, num * sizeof(uint32_t));
    
    NSS_LOW();
	while (TxIdx < wsize)
	{
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TE) == RESET);
        SPI_I2S_TxData(SPI1, wdata_t[TxIdx++]);
        if (TxIdx > 6)
		{
			while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RNE) == RESET);
			rdata[RxIdx] = SPI_I2S_RxData(SPI1);  
            RxIdx++;            
            vTaskDelay(2);
		}        
		while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BUSY));
	}	    
    for (i = 0; i < num; ++i)
	{
		uint8_to_uint32(&rdata[4*i], &data[i]);
	}    
	NSS_HIGH();
    free(wdata_t);
    free(wdata);
    free(rdata);   
    vTaskDelay(10);    
}

void rs_ctl_load_seq_check(void)
{
    int32_t TxIdx = 0, RxIdx = 0;
    uint8_t wdata[4];
    uint8_t rdata[1];	 
	
	wdata[0] = 0x0f;
	wdata[1] = 0xff;
	wdata[2] = 0xff;
	wdata[3] = 0x01;

	NSS_LOW();
	while (TxIdx < 4)
	{
		while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TE) == RESET);
		SPI_I2S_TxData(SPI1, wdata[TxIdx++]);
		
		if (TxIdx > 2)
		{
			while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RNE) == RESET);
			rdata[RxIdx] = SPI_I2S_RxData(SPI1);  
            RxIdx++;            
            vTaskDelay(2);
		}
		while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BUSY));
	}    
	NSS_HIGH();	  
}

void read_modify_write_reg(uint16_t addr, uint32_t mask, uint32_t data)
{
    uint32_t rdata;
    rs_ctl_read_regs(addr, &rdata, 1);
    rdata = (rdata & (~mask)) | (data & mask);
    rs_ctl_write_regs(addr, &rdata, 1);
}
