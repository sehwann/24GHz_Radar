//-----------------------------------------------------------------------------
// File Name         : sc123x_gpio.c
//-----------------------------------------------------------------------------
// @brief This file is the definition of the default GPIO used by SC1233.
//		  This project uses distance mode as the basis.
// Copyright (C) 2023 PETABREW. sehwan.park
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "sc123x_gpio.h"
//-----------------------------------------------------------------------------
// Function prototypes
//-----------------------------------------------------------------------------
void I2C_OFF(void){
	GPIOC->BRE = I2C_SEL;
}

void CE_ON(void) {
	GPIOC->BSRE = CE;
}

void CE_OFF(void) {
	GPIOC->BRE = CE;
}

void NRST_ON(void) {
	GPIOC->BSRE = NRST;
}

void NRST_OFF(void) {
	GPIOC->BRE = NRST;
}

void NSS_HIGH(void) {
	GPIOA->BSRE = NSS;
}

void NSS_LOW(void) {
	GPIOA->BRE = NSS;
}
