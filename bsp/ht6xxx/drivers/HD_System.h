/****************************************************************
Description: HD_LCD.h
Author: hx
Notes: 系统配置头文件
*****************************************************************/
#ifndef __HD_SYSTEM_H
#define __HD_SYSTEM_H


#include "board.h"

/*- 1.Macro-----------------------------------------------------*/
/* HT 501X RCC ENABLE BIT CONFIG0*/
#define  LCD_EN         (1<<1)
#define  SPI_EN         (1<<2)
#define  I2C_EN         (1<<3)
#define  PLL_EN         (1<<4)
#define  HRC_EN         (1<<5)
#define  PLLLOCK_EN     (1<<6)
#define  LFDET_EN       (1<<7)
#define  PLLDET_EN      (1<<8)
#define  HRCDET_EN      (1<<9)
#define  OSCSLP_EN      (1<<10)
#define  KEY_EN         (1<<11)
#define  CLKOUT_EN      (1<<12)
#define  IP5LBOR_EN     (1<<13)
#define  AES_EN         (1<<14)
#define  EMU_EN         (1<<15)
/* HT 501X RCC ENABLE BIT CONFIG1*/
#define  TMR0_EN        (1<<0)
#define  TMR1_EN        (1<<1)
#define  TMR2_EN        (1<<2)
#define  TMR3_EN        (1<<3)
#define  UART0_EN       (1<<4)
#define  UART1_EN       (1<<5)
#define  UART2_EN       (1<<6)
#define  UART3_EN       (1<<7)
#define  UART4_EN       (1<<8)
#define  UART5_EN       (1<<9)
#define  TMR4_EN        (1<<10)
#define  TMR5_EN        (1<<11)
#define  UART6_EN       (1<<12)
#define  LRCTFREQ_EN    (1<<14)
#define  LRCTRTC2_EN    (1<<15)

/****************ADC******************/
#define BKBATT_HIGH                     3
#define BKBATT_LOW                      2
#define BKBATT_NO                       1
#define BKBATT_NO_BATT                  0


#define     _OPEN_REG_WRITE       HT_CMU->WPREG = 0xA55A
#define     _CLOSE_REG_WRITE      HT_CMU->WPREG = 0x5AA5

/*串口*/
#define  GPIO_AF_TX6                    0x02
#define  GPIO_AF_RX6                    0x02
#define  GPIO_AF_RX5                    0x03
#define  GPIO_AF_TX5                    0x03
#define  GPIO_AF_TX4                    0x02
#define  GPIO_AF_RX4                    0x02
#define  GPIO_AF_RX3                    0x02
#define  GPIO_AF_TX3                    0x02
#define  GPIO_AF_TX2                    0x02
#define  GPIO_AF_RX2                    0x02
#define  GPIO_AF_TX1                    0x02
#define  GPIO_AF_RX1                    0x02
#define  GPIO_AF_TX0                    0x02
#define  GPIO_AF_RX0                    0x02

/*外部中断口*/
#define  GPIO_AF_INT9                   0x02
#define  GPIO_AF_INT8                   0x02
#define  GPIO_AF_INT7                   0x02
#define  GPIO_AF_INT6                   0x02
#define  GPIO_AF_INT5                   0x02
#define  GPIO_AF_INT4                   0x02
#define  GPIO_AF_INT3                   0x02
#define  GPIO_AF_INT2                   0x02
#define  GPIO_AF_INT1                   0x02
#define  GPIO_AF_INT0                   0x02

#define  GPIO_AF_CLKOUT                 0x02


#define  GPIO_AF_LVDIN0                 0x02
#define  GPIO_AF_LVDIN1                 0x02
#define  GPIO_AF_7816_0                 0x03

#define  GPIO_AF_SPI0_CLK               0x02
#define  GPIO_AF_SPI0_SDI               0x02
#define  GPIO_AF_SPI0_SDO               0x02

#define  GPIO_AF_ADCIN1                 0x03

#define	 C_PMUSTA		 0x0007
#define  PFAL_Read()     (((HT_PMU->PMUSTA & 0x0004) == 0x0004)?1:0)
#define  HD_clrWDT()     {HT_FreeDog();}

//MCU速率（用户可配置）---------------------------------------------------------
#define	SPD_22000K		0
#define	SPD_11000K		1
#define	SPD_5500K		2
#define	SPD_2750K		3
#define	SPD_1375K		4
#define	SPD_688K		5
#define	SPD_344K		6
#define	SPD_172K		7
#define	SPD_MCU			SPD_22000K
#define	SPD_PD_MCU		SPD_5500K

#define  Pin_0                    0
#define  Pin_1                    1
#define  Pin_2                    2
#define  Pin_3                    3
#define  Pin_4                    4
#define  Pin_5                    5
#define  Pin_6                    6
#define  Pin_7                    7
#define  Pin_8                    8
#define  Pin_9                    9
#define  Pin_10                   10
#define  Pin_11                   11
#define  Pin_12                   12
#define  Pin_13                   13
#define  Pin_14                   14
#define  Pin_15                   15

#define  PUPDR_UP    1
#define  PUPDR_OD    0
#define  AIN         2

#define  OUTPUT      1
#define  INPUT       0

/*- 2.Type prototype--------------------------------------------*/




/*- 3.Public variable-------------------------------------------*/
extern U32 g_ulMCLK;

/*- 4.Public function-------------------------------------------*/
extern void HD_System_RCC_Init(void);
extern void HD_System_RCC_PD_Init(void);
extern void HD_System_NVIC_Init(void);
extern void HD_System_NVIC_PD_Init(void);
extern void HD_System_ADC_Init(void);
extern void HD_System_Battery_Get(void);
extern void HD_CfgCLK_FOSC_32K(U8 ucLcdState);
extern void HD_CfgCLK_PLL_614K(void);
extern void HD_CfgCLK_PLL_1M376(void);
extern void HD_CfgCLK_PLL_22M(void);
extern void HD_System_PVDConfig(void);
extern void HD_System_PMUConfig(void);
extern void HD_System_PVD_INTCfg(U8 ucEnable);
extern U8   HD_System_IsPVDIntStatus(void);
extern void SwitchTo_Fpll(U8 ucClk);

#endif
