//-----------------------------------------------------------------------------
// File Name         : main.c
//-----------------------------------------------------------------------------
// Copyright (C) 2022 PETABREW.
// Author            : sehwan.park
// Version           : Ver 0.1
// Date              : 07/21/2022
// Description       : This module is a source file 
//                     that operates SC1233 and receives the distance value and outputs it as UART.
//
// Revision History
// Date            Version         Name             Description
// 07/21/2022      0.1             sehwan.park      created
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "SC1233/main_sc123x.h"
#include "at32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include "SC1233/rs_ctl_base.h"
#include "SC1233/rs_ctl_distance.h"		// distance mode
//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
rs_handle handle;
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void taskMain(void *pvParameters)
{	
	/* The device is opened by calling the API, rs_open. */
    rs_open(&handle);	
	/* 
	 * The sensor setting parameters structure variable can be updated with the desired sensor
	 * setting parameters.
	 */
	{
		struct rs_distance_param lp;
		lp.rx_gain = 60;
		lp.interval = 25;
		lp.beta = 205;
		lp.lower_distance = 10;
		lp.upper_distance = 800;
		lp.chirp_time = 4400;
		lp.digital_beam_shaper = RS_DIGITAL_BEAM_SHAPER_WIDE;
		lp.high_pass_filter = RS_HPF_FIRST_ORDER;
		lp.distance_step = RS_DISTANCE_STEP_NORMAL;
		handle.param = &lp;    
	}
	/* 
	 * Sensor is set up with that sensor setting parameters structure variable
	 * containing sensor setting parameters by calling the API, rs_setup_param,
	 * specified with that mode.
	 */
    rs_setup_param(&handle);
	/* 
	 * The Distance detection operation can be started by calling the API, rs_start.
	 * The library state will be "SENSING".
	 */
    rs_start_body(&handle, 0);
	char fifoPinInput = 0;
	
    while(1)
    {
		/*
		 *	When data enters SC1233_FIFO, it can be retrieved.
		 */
        fifoPinInput = GPIO_ReadInputDataBit(GPIOC, GPIO_Pins_8);	
        if (fifoPinInput) {
			fifoPinInput = 0;
			struct rs_distance_data data;
			/*
			 *	Call rs_get_distance to obtain distance data.
			 */
			rs_get_distance(&handle, 1000, &data);  
			if (data.distance[0] != 0x7fff) {
				printf("%d\n", data.distance[0]);       
			}
		}
    }
}

int main(void)
{
	AT32F413_SC1233_INIT();		// Initialize AT32F413
	
    taskENTER_CRITICAL();	
    xTaskCreate(taskMain, "TaskMain", 500, NULL, 1, NULL);
    taskEXIT_CRITICAL(); 
    vTaskStartScheduler(); 
}
