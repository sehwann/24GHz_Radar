#include "rs_ctl_distance.h"
/**
 * prepare parameters for distance detection
 * @callergraph
 */
void distance_wide_get(rs_handle* handle)
{
    const struct initial_dataset *setup = NULL;
    setup = &mode_distance_wide_fine_60db_4400us;
    rs_setup_getcode(handle, setup);
}
/**
 * update parameters for distance detection
 * @callergraph
 */
void distance_wide_update_param(rs_handle* handle, struct rs_distance_param *lp)
{
    rs_setup_interval(handle, lp->interval);
    rs_setup_beta(handle, lp->beta);
	rs_setup_hpf(handle, lp->high_pass_filter);
    rs_setup_range_peak(handle, lp->upper_distance, lp->lower_distance);
}
/**
 * Update smoothing factor register
 */
void rs_setup_beta(rs_handle* handle, uint8_t beta)
{
    read_modify_write_reg(RS_CTL_REG_PRESDET_00, 0x0000ff00, ((uint32_t) beta << 8));
}
/**
 * Update distance measurement frequency index register
 */
void rs_setup_range_peak(rs_handle* handle, uint32_t upper, uint32_t lower)
{
    int32_t conv_offset = 0;
	uint32_t val = 0;
    rs_ctl_read_regs(RS_CTL_REG_PRESDET_60, &val, 1);
    conv_offset = (((int32_t) (val)) << 12) >> 20;
	rs_setup_range(handle, RS_CTL_REG_PRESDET_19, upper, lower, conv_offset);
}

void rs_get_distance(rs_handle* handle, uint32_t timeout, struct rs_distance_data *data)
{
 	const uint16_t reg_addr[] = {
				RS_CTL_REG_DISTANCE_RX1_12,
				RS_CTL_REG_DISTANCE_RX1_34,
				RS_CTL_REG_DISTANCE_RX1_5,
				RS_CTL_REG_PEAK_LEVEL_RX1_1,
				RS_CTL_REG_PEAK_LEVEL_RX1_2,
				RS_CTL_REG_PEAK_LEVEL_RX1_3,
				RS_CTL_REG_PEAK_LEVEL_RX1_4,
				RS_CTL_REG_PEAK_LEVEL_RX1_5,
				RS_CTL_REG_MY_COMPLEX_RX1_1,
				RS_CTL_REG_MY_COMPLEX_RX1_2,
				RS_CTL_REG_MY_COMPLEX_RX1_3,
				RS_CTL_REG_MY_COMPLEX_RX1_4,
				RS_CTL_REG_MY_COMPLEX_RX1_5,
    };
    uint32_t reg[ARRAY_SIZE(reg_addr)];
    get_devdata(handle, timeout, reg_addr, ARRAY_SIZE(reg_addr), reg);
    get_distance_data(reg, data, handle->peak_level_lower);
}
/**
 * get data from Sensor
 * @callgraph
 */
void get_devdata(rs_handle* handle, uint32_t timeout, const uint16_t *reg_addr, uint32_t reg_size, uint32_t *reg)
{
	struct rs_ctl_sensor_data_set set;
	struct rs_ctl_sensor_data get;

	set.sram.enable = 1;
	set.sram.exist_header_header = (handle->code_ref.info.frame_header != 0) ? TRUE : FALSE;
	set.sram.exist_header_status = (handle->code_ref.info.frame_status != 0) ? FALSE : FALSE;
	set.sram.bytesize_word = (handle->code_ref.info.frame_datasel != 0) ? 3 : 4;
	set.sram.wordsize_body = handle->code_ref.info.frame_size;

	set.reg.enable = TRUE;
	set.reg.num = reg_size;
	set.reg.addr = reg_addr;

	set.status.enable = FALSE;
	rs_ctl_sensor_data_init(&get);

    rs_ctl_cmd_wait_and_get_sensor_data(handle, timeout, &set, &get);
	memcpy(reg, get.reg.data, reg_size * sizeof(uint32_t));
    rs_ctl_sensor_data_free(&get);	
}
/**
 * wait trigger of OR pin and get sensor data
 *
 * @callgraph
 */
void rs_ctl_cmd_wait_and_get_sensor_data(rs_handle* handle, uint32_t timeout, const struct rs_ctl_sensor_data_set *set, struct rs_ctl_sensor_data *data)
{
    read_status(handle, set, data);
    read_register(handle, set, data);
    read_fifo(handle, set, data);
}
/**
 * read status register
 * @callgraph
 */
void read_status(rs_handle* handle, const struct rs_ctl_sensor_data_set *set, struct rs_ctl_sensor_data *data)
{
    if (set->status.enable) {
        rs_ctl_op_RDSR2(&(data->status));
    }
}
/**
 * read registers
 * @callgraph
 */
void read_register(rs_handle* handle, const struct rs_ctl_sensor_data_set *set, struct rs_ctl_sensor_data *data)
{
	
    if(set->reg.enable) {
        uint32_t i = 0;
        data->reg.num = set->reg.num;
        data->reg.data = (uint32_t *) calloc(set->reg.num, sizeof(uint32_t));
        rs_ctl_op_HLDDT();
	
        for (i=0; i<set->reg.num; i++) {
            rs_ctl_read_reg(set->reg.addr[i], &(data->reg.data[i]));
				
        }
        rs_ctl_op_UPDDT();        
    }
}
/**
 * read FIFO
 * @callgraph
 */
void read_fifo(rs_handle* handle, const struct rs_ctl_sensor_data_set *set, struct rs_ctl_sensor_data *data)
{
    if (set->sram.enable) {
        uint32_t wordsize_sram = 0;
        wordsize_sram += set->sram.exist_header_header == TRUE ? 1 : 0;
        wordsize_sram += set->sram.exist_header_status == TRUE ? 1 : 0;
        wordsize_sram += set->sram.wordsize_body;
        wordsize_sram += 1;
        data->sram.size = wordsize_sram * set->sram.bytesize_word;
        data->sram.data = (uint8_t*) calloc(wordsize_sram, set->sram.bytesize_word);
        rs_ctl_op_read_region(RS_CTL_TOPADDR_FIFO, data->sram.data, data->sram.size);	
    }    
}

void rs_ctl_sensor_data_init(struct rs_ctl_sensor_data *data)
{
	data->sram.data = NULL;
	data->reg.data = NULL;
}


void get_distance_data(const uint32_t *regs, struct rs_distance_data *data, uint8_t peak_level_lower)
{
	const uint32_t * const regs_distance = regs + 0;
	const uint32_t * const regs_peak_level = regs + 3;
	const uint32_t * const regs_complex = regs + 8;
	int i;

	data->distance[0] = (int16_t) ((regs_distance[ 0] >> 16) & 0xffff);
	data->distance[1] = (int16_t) ((regs_distance[ 0] >>  0) & 0xffff);
	data->distance[2] = (int16_t) ((regs_distance[ 1] >> 16) & 0xffff);
	data->distance[3] = (int16_t) ((regs_distance[ 1] >>  0) & 0xffff);
	data->distance[4] = (int16_t) ((regs_distance[ 2] >>  0) & 0xffff);

	for(i=0; i<5; i++) {
		if(regs_peak_level[i] == 0) {
			data->peak_level[i] = RS_PEAK_LEVEL_INVALID;
		} else {
			data->peak_level[i] = conv_power(regs_peak_level[i]);
		}
		data->peak_real[i] = (int16_t) ((regs_complex[i] >> 16) & 0xffff);
		data->peak_imag[i] = (int16_t) ((regs_complex[i] >>  0) & 0xffff);
	}

	distance_range(data, peak_level_lower);
}

static uint8_t conv_power(uint32_t val)
{
	return (uint8_t) (10.0 * log10(val));
}

static void distance_range(struct rs_distance_data *data, uint8_t peak_level_lower)
{
	int i;

	for(i=0; i<5; i++) {
		if((data->distance[i] > RS_DISTANCE_MAX) || (data->distance[i] < RS_DISTANCE_MIN)) 
		{
			data->distance[i] = RS_DISTANCE_INVALID;
			data->peak_level[i] = RS_PEAK_LEVEL_INVALID;
		}
		if(data->peak_level[i] < peak_level_lower)
		{
			data->distance[i] = RS_DISTANCE_INVALID;
		}
	}
}

void rs_ctl_sensor_data_free(struct rs_ctl_sensor_data *data)
{
    free(data->sram.data);
    free(data->reg.data);
    data->sram.data = NULL;
    data->reg.data = NULL;
}
