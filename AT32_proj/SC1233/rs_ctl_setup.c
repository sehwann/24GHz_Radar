/*
 * Copyright (c) 2018-2019 Socionext Inc.
 * All rights reserved.
 */
#include "rs_ctl_setup.h"

void _table_read_reg(struct rs_code_ref_s* code, uint16_t addr, uint32_t *data)
{
    int32_t i;
    for (i=0; i<code->regs.num; i++) {
        if(code->regs.addr[i] == addr) {
            *data = code->regs.data[i];            
        }
    }
}

float _point_per_chirp(rs_handle* handle, const uint32_t fft_point) {
	uint32_t val;
	const float c = RS_C;
	rs_ctl_read_regs(RS_CTL_REG_PRESDET_56, &val, 1);
	const float a_reg = (((val ^ 0xffffffff) + 1) & 0x000fffff) / 256.0;
	return round(a_reg * 2 * _effective_chirp_bandwidth(fft_point) * fft_point / c);
}

float _effective_chirp_bandwidth(const uint32_t fft_point) {
	return 180 * 1000000.0 * fft_point / 550;
}
/**
 * Setup Parameters
 *
 * @param[in] handle Sensor Handle
 * @param[in] mode Sensor Operation Mode
 * @param[in] param Sensor Parameters
 * @callgraph
 */
void rs_setup_param(rs_handle* handle)
{
    rs_setup_param_local(handle);
    rs_load_seq_code(handle);
    
    rs_update_param_local(handle);
    rs_ctl_op_DSLEEP();
}
/**
 * prepare parameters
 * @callergraph
 */
void rs_setup_param_local(rs_handle* handle)
{
	distance_wide_get(handle);
}

void rs_setup_getcode(rs_handle* handle, const struct initial_dataset *setup)
{
    handle->code_ref.seqc.data = setup->seq;
    handle->code_ref.seqc.size = setup->seq_size;
    handle->code_ref.winf.data = setup->fftwin;
    handle->code_ref.winf.size = setup->fftwin_size;
    handle->code_ref.regs.addr = setup->reg_addr;
    handle->code_ref.regs.data = setup->reg_val;
    handle->code_ref.regs.num = setup->reg_size;

    {
		uint32_t FRM_2;
		_table_read_reg(&(handle->code_ref), RS_CTL_REG_FRM_2, &FRM_2);
        handle->code_ref.info.frame_size = (FRM_2 >> 16) & 0x1fff;
	}
	{
		uint32_t FRM_3;
		_table_read_reg(&(handle->code_ref), RS_CTL_REG_FRM_3, &FRM_3);
		{
			uint32_t FRM_HEAD_2 = (FRM_3 >> 12) & 0x1;
			uint32_t CPUIF_DATA_SEL_FIFO = (FRM_3 >> 29) & 0x3;
			uint32_t CPUIF_EN_STATUS_FIFO = (FRM_3 >> 28) & 0x1;
			handle->code_ref.info.frame_header = FRM_HEAD_2;
			handle->code_ref.info.frame_status = CPUIF_EN_STATUS_FIFO;
			handle->code_ref.info.frame_datasel = CPUIF_DATA_SEL_FIFO;
		}
	}
}

/**
 * Loading Sequencer Code
 */
void rs_load_seq_code(rs_handle* handle)
{
    rs_ctl_cmd_boot();
    rs_setup_seq(handle);        

    handle->state = RS_STATE_READY;
    handle->sensor_ctl_type = RS_SENSOR_CTL_TIMER;
}
/**
 * Setting registers, Sequencer Code and FFT parameters \cite WriteFFTPrc, \cite WriteSEQPrc, \cite ActivateSEQ
 *
 * @callgraph
 */
void rs_setup_seq(rs_handle* handle) 
{
    uint16_t expected = 0;
	uint32_t crc = 0;
	uint16_t crc_seqpgm = 0;
	uint16_t crc_fftwin = 0;
    {
		uint32_t i;
		for (i=0; i<handle->code_ref.regs.num; i++) {
			rs_ctl_write_reg(handle->code_ref.regs.addr[i], handle->code_ref.regs.data[i]);
		}
	}
    vTaskDelay(5);
    rs_ctl_read_reg(RS_CTL_REG_CPUIF_MON_2, &crc);
    rs_ctl_cmd_disable_seq();
    
    rs_ctl_write_mem_seq(RS_CTL_TOPADDR_SEQ, 
        handle->code_ref.seqc.data, 
        handle->code_ref.seqc.size
    );    
    expected = get_expected_crc(
        RS_CTL_TOPADDR_SEQ, 
        (const uint8_t *)(handle->code_ref.seqc.data), 
        handle->code_ref.seqc.size
    );
    rs_ctl_read_reg(RS_CTL_REG_CPUIF_MON_2, &crc);
    crc_seqpgm = (crc >> 16) & 0xFFFF;
    rs_ctl_write_mem(RS_CTL_TOPADDR_FFT, 
        (const uint8_t *)(handle->code_ref.winf.data), 
        handle->code_ref.winf.size
    );
       
    expected = get_expected_crc(
        RS_CTL_TOPADDR_FFT, 
        (const uint8_t *)(handle->code_ref.winf.data), 
        handle->code_ref.winf.size
    );
    
	rs_ctl_read_reg(RS_CTL_REG_CPUIF_MON_2, &crc);
    crc_fftwin = (crc >>  0) & 0xFFFF;
        
    rs_ctl_cmd_enable_seq();
}
/**
 * update parameters
 * @callergraph
 */
void rs_update_param_local(rs_handle* handle)
{
	distance_wide_update_param(handle, handle->param);
}
/**
 * Update interval register
 */
void rs_setup_interval(rs_handle* handle, uint32_t interval)
{
 	read_modify_write_reg(RS_CTL_REG_CPUIF_4, 0xffffffff, (interval * 0x40) << 0);
}
/**
 * Update HPF register
 */
void rs_setup_hpf(rs_handle* handle, int32_t hpf)
{
    switch(hpf) {
	case RS_HPF_FIRST_ORDER:
		read_modify_write_reg(RS_CTL_REG_IF1_1, 0xffffffff, 0x33443301);
		read_modify_write_reg(RS_CTL_REG_IF1_2, 0xffffffff, 0x00031900);
		read_modify_write_reg(RS_CTL_REG_IF2_1, 0xffffffff, 0x33443301);
		read_modify_write_reg(RS_CTL_REG_IF2_2, 0xffffffff, 0x00031900);
		read_modify_write_reg(RS_CTL_REG_SELSET_C_3, 0xffffffff, 0xe0380820);
		break;
	case RS_HPF_SECOND_ORDER:
		read_modify_write_reg(RS_CTL_REG_IF1_1, 0xffffffff, 0x33443300);
		read_modify_write_reg(RS_CTL_REG_IF1_2, 0xffffffff, 0x00031300);
		read_modify_write_reg(RS_CTL_REG_IF2_1, 0xffffffff, 0x33443300);
		read_modify_write_reg(RS_CTL_REG_IF2_2, 0xffffffff, 0x00031300);
		read_modify_write_reg(RS_CTL_REG_SELSET_C_3, 0xffffffff, 0x00000820);
		break;
	}
}


void rs_setup_range(rs_handle* handle, uint16_t addr, uint32_t upper, uint32_t lower, int32_t conv_offset)
{
    const uint32_t fft_point = 512;
	const float a = RS_C * _point_per_chirp(handle, fft_point) / (2 * _effective_chirp_bandwidth(fft_point) * fft_point);

	uint32_t up = (uint32_t) ceil(512 / 2 - (upper - conv_offset) / a);
	uint32_t lo = (uint32_t) floor(512 / 2 - (lower - conv_offset) / a);
	{
		uint32_t range[16];
		uint32_t i;
		for(i=0; i<sizeof(range)/sizeof(*range); i++) {
			range[i] = 0;
		}
		for(i=0; i<512; i++) {
			if((lo >= i) && (i >= up)) {
				range[15 - i / 32] |= 0x00000001 << (i % 32);
			}
		}
		for(i=0; i<sizeof(range)/sizeof(*range); i++) {
			read_modify_write_reg(addr + i, 0xffffffff, range[i]);
		}
	}
}
/**
 * set lower limit for peak level
 */
void rs_set_peak_level_lower(rs_handle* handle, uint8_t level)
{
    handle->peak_level_lower = level;
}
/**
 * get lower limit for peak level
 */
void rs_get_peak_level_lower(rs_handle* handle, uint8_t *level)
{
    *level = handle->peak_level_lower;
}

