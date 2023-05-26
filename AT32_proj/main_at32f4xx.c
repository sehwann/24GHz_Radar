//-----------------------------------------------------------------------------
// File Name         : main_at32f4xx.c
//-----------------------------------------------------------------------------
// @brief This file contains an initialization function for controlling the SC1233 module using the AT32F413 microcontroller.
//		  This includes the ability to initialize RCC, UART, SPI, and GPIO..
//		  This file is part of the PETABREW project.
// Copyright (C) 2023 PETABREW. sehwan.park
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "main_at32f4xx.h"

/*******<PIN OUT>***************** 
 * <GPIO>
 * AT32 -> SC123x : NRST, CE, I2C_SEL
 * NRST
 * PC06 : CE
 * PC07 : I2C_SEL (SPI:0, I2C:1)
 * 
 * SC123x -> AT32 : OR, MOTION_DETOUT, DTEST
 * PC08 : SC123xA_FIFO
 * PC09 : MOTION DETOUT
 * PC10 : DTEST
 *
 * <USART>
 * PA09 : DEBUG TXD
 * PA10 : DEBUG RXD 
 *  
 * <SPI>
 * PA04 : SPI SENSB SCL
 * PA05 : SPI SCK
 * PA06 : SPI MOSI
 * PA07 : SPI MISO
 *************************************/

//-----------------------------------------------------------------------------
// Function Definitions
//-----------------------------------------------------------------------------
/**
 * @brief Initialize RCC for operation of USART1, SPI1, and SC1233.
 */
void RCC_Configuration(void)
{ 
	/*
	 * GPIOA (USART1)
	 */
	RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOA | RCC_APB2PERIPH_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_USART1, ENABLE); 
    /*
	 * NRST, MOTION_DETOUT, FIFO_READY(OR), I2C_SEL, SC213_CE, DTEST    
	 */
  	RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOC | RCC_APB2PERIPH_AFIO, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_APB2PERIPH_GPIOC, ENABLE);	
	/*
	 * Enable SPI1 and GPIO clocks
	 */
	RCC_APB2PeriphClockCmd(SPI_MASTER_GPIO_CLK | SPI_MASTER_CLK, ENABLE);
}
/**
 * @brief Initialize GPIO for operation of USART1, SPI1, and SC1233.
 */
void GPIO_Configuration(void)
{
    GPIO_InitType GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);
    /*
     * PIN_A : USART
	 */
    GPIO_InitStructure.GPIO_Pins = UART1_TX_PIN; 
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pins = UART1_RX_PIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*
     * PIN_C : SPI
	 */
    GPIO_InitStructure.GPIO_Pins = SPI_MASTER_PIN_NSS;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_PP;  
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // Configure SPI_MASTER pins: SCK, MOSI and MOSI
    GPIO_InitStructure.GPIO_Pins = (SPI_MASTER_PIN_SCK | SPI_MASTER_PIN_MISO | SPI_MASTER_PIN_MOSI) ;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // GPIOC (NRST, MOTION_DETOUT, FIFO_READY(OR), I2C_SEL, SC213_CE, DTEST)
    // AT32 -> SC123x : NRST, CE, I2C_SEL
    GPIO_InitStructure.GPIO_Pins = GPIO_Pins_0 | GPIO_Pins_6 | GPIO_Pins_7 ; 
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
    // SC123x -> AT32 : OR, MOTION_DETOUT, DTEST
    GPIO_InitStructure.GPIO_Pins = GPIO_Pins_8 | GPIO_Pins_9 | GPIO_Pins_10; 
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_PD;
    GPIO_Init(GPIOC, &GPIO_InitStructure);	
}
/**
 * @brief Initialize USART1
 */
void UART_Configuration(void)
{
	USART_InitType USART_InitStructure;
	USART_StructInit(&USART_InitStructure);
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
}
/**
 * @brief Initialize SPI_MASTER
 */
void SPI_Configuration(void)
{
    SPI_InitType SPI_InitStructure;
    SPI_DefaultInitParaConfig(&SPI_InitStructure);
    // SPI_MASTER Configuration
    // Initialize the SPI_Direction member
    SPI_InitStructure.SPI_TransMode = SPI_TRANSMODE_FULLDUPLEX;
    // initialize the SPI_Mode member
    SPI_InitStructure.SPI_Mode = SPI_MODE_MASTER;
    // initialize the SPI_DataSize member
    SPI_InitStructure.SPI_FrameSize = SPI_FRAMESIZE_8BIT;
    // Initialize the SPI_CPOL member
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_LOW;
    // Initialize the SPI_CPHA member
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1EDGE;
    // Initialize the SPI_NSS member
    SPI_InitStructure.SPI_NSSSEL = SPI_NSSSEL_SOFT;
    // Initialize the SPI_BaudRatePrescaler member
    SPI_InitStructure.SPI_MCLKP = SPI_MCLKP_16;  // 2 4 8 16 32 64 128 256 512 1024
    // Initialize the SPI_FirstBit member
    SPI_InitStructure.SPI_FirstBit = SPI_FIRSTBIT_MSB;
    // Initialize the SPI_CRCPolynomial member
    SPI_InitStructure.SPI_CPOLY = 1;		
    SPI_Init(SPI1, &SPI_InitStructure);	
    SPI_Enable(SPI1, ENABLE);
}
/**
 * @brief Initialize AT32F413
 */
void AT32F413_SC1233_INIT(void)
{
	RCC_Configuration();
	GPIO_Configuration();
	UART_Configuration();     
	SPI_Configuration();
}







