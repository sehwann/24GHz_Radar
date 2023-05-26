/*
 * Copyright (c) 2018-2019 Socionext Inc.
 * All rights reserved.
 */
/*
 * rs24g_types.h
 */
#ifndef __RS_TYPES_H__
#define __RS_TYPES_H__

#include <stdint.h>

typedef int32_t									rs_bool_t;
typedef int32_t									rs_ret_t;
typedef uint32_t								rs_size_t;
typedef int32_t									rs_state_t;
typedef int32_t									rs_mode_t;
typedef int32_t									rs_sensor_ctl_type_t;

// rs_bool_t
#define RS_FALSE								((rs_bool_t) (0))
#define RS_TRUE									((rs_bool_t) (1))

// rs_ret_t
#define RS_OK									((rs_ret_t) (0))
#define RS_EINV									((rs_ret_t) (-1))
#define RS_ETOUT								((rs_ret_t) (-2))
#define RS_EDEV									((rs_ret_t) (-3))
#define RS_ENOSUPPORT							((rs_ret_t) (-4))
#define RS_ESTATE								((rs_ret_t) (-5))
#define RS_ECOM									((rs_ret_t) (-6))
#define RS_ENOMEM								((rs_ret_t) (-7))
#define RS_EFAULT								((rs_ret_t) (-8))
#define RS_ELIB									((rs_ret_t) (-9))
#define RS_EFORM								((rs_ret_t) (-10))
#define RS_ELOAD								((rs_ret_t) (-11))
#define RS_EMODE								((rs_ret_t) (-12))
#define RS_EBUSY								((rs_ret_t) (-13))

// rs_ret_t
#define RS_ERANGE_MODE_TYPE						((rs_ret_t) (-1024))
#define RS_ERANGE_RX_GAIN						((rs_ret_t) (-1025))
#define RS_ERANGE_INTERVAL						((rs_ret_t) (-1026))
#define RS_ERANGE_ALPHA							((rs_ret_t) (-1027))
#define RS_ERANGE_MOTION_THRESHOLD				((rs_ret_t) (-1028))
#define RS_ERANGE_STARTUP_COUNT					((rs_ret_t) (-1029))
#define RS_ERANGE_LOWER_DISTANCE				((rs_ret_t) (-1030))
#define RS_ERANGE_UPPER_DISTANCE			 	((rs_ret_t) (-1031))
#define RS_ERANGE_BETA							((rs_ret_t) (-1032))
#define RS_ERANGE_CHIRP_TIME					((rs_ret_t) (-1033))
#define RS_ERANGE_DIGITAL_BEAM_SHAPER			((rs_ret_t) (-1034))
#define RS_ERANGE_HPF							((rs_ret_t) (-1035))
#define RS_ERANGE_FIFO_FORMAT					((rs_ret_t) (-1036))
#define RS_ERANGE_RX_NUM						((rs_ret_t) (-1037))
#define RS_ERANGE_DISTANCE_STEP					((rs_ret_t) (-1038))


#endif	/* __RS24G_TYPES_H__ */
