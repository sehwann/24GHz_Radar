//-----------------------------------------------------------------------------
// File Name         : main_at32f4xx.h
//-----------------------------------------------------------------------------
// @brief This file contains an initialization function for controlling the SC1233 module using the AT32F413 microcontroller.
//		  This includes the ability to initialize RCC, UART, SPI, and GPIO..
//		  This file is part of the PETABREW project.
// Copyright (C) 2023 PETABREW. sehwan.park
//-----------------------------------------------------------------------------
#ifndef __MAIN_AT32F4XX_H__
#define __MAIN_AT32F4XX_H__
//-----------------------------------------------------------------------------
// Include
//-----------------------------------------------------------------------------
#include "at32f4xx.h"
#include "at32f4xx_spi.h"
#include "at32f4xx_gpio.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
//-----------------------------------------------------------------------------
// Global typedefs & definitions
//-----------------------------------------------------------------------------
#define BufferSize 32
#define UART1_TX_PIN				GPIO_Pins_9
#define UART1_RX_PIN				GPIO_Pins_10

#define SPI_MASTER                   SPI1
#define SPI_MASTER_CLK               RCC_APB2PERIPH_SPI1
#define SPI_MASTER_GPIO              GPIOA
#define SPI_MASTER_GPIO_CLK          RCC_APB2PERIPH_GPIOA  
#define SPI_MASTER_PIN_NSS           GPIO_Pins_4
#define SPI_MASTER_PIN_SCK           GPIO_Pins_5
#define SPI_MASTER_PIN_MISO          GPIO_Pins_6
#define SPI_MASTER_PIN_MOSI          GPIO_Pins_7
#define SPI_MASTER_IRQn              SPI1_IRQn
//-----------------------------------------------------------------------------
// Function prototypes
//-----------------------------------------------------------------------------
void RCC_Configuration(void);
void GPIO_Configuration(void);
void UART_Configuration(void);
void SPI_Configuration(void);
void AT32F413_SC1233_INIT(void);

#endif
