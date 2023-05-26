//-----------------------------------------------------------------------------
// File Name         : sc123x_gpio.c
//-----------------------------------------------------------------------------
// @brief This file is the definition of the default GPIO used by SC1233.
//		  This project uses distance mode as the basis.
// Copyright (C) 2023 PETABREW. sehwan.park
//-----------------------------------------------------------------------------
#ifndef __SC123X_GPIO_H__
#define __SC123X_GPIO_H__
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "main_at32f4xx.h"
//-----------------------------------------------------------------------------
// Global typedefs & definitions
//-----------------------------------------------------------------------------
#define CE			GPIO_Pins_6
#define NRST		GPIO_Pins_0
#define I2C_SEL		GPIO_Pins_7
#define NSS			GPIO_Pins_4
//-----------------------------------------------------------------------------
// Function prototypes
//-----------------------------------------------------------------------------
void I2C_OFF(void);
void CE_ON(void);
void CE_OFF(void);
void NRST_ON(void);
void NRST_OFF(void);
void NSS_HIGH(void); 
void NSS_LOW(void);


#endif
