/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-09-01     Shijj        the first version
 */

#ifndef __BOARD_H__
#define __BOARD_H__

#include <rtthread.h>
#include "ht6xxx.h"
#include "ht6xxx_lib.h"
#include "drv_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MIN(a, b)                       (((a) < (b))? (a) : (b))
#define MAX(a, b)                       (((a) > (b))? (a) : (b))
#define DIV_CEIL(x, y)                  (((x) + (y) - 1) / (y))
#define ARRAY_SIZE(a)                   (sizeof(a) / sizeof(a[0]))

#define HT6x3x_FLASH_START_ADRESS     ((uint32_t)0x00000000)
#define HT6x3x_FLASH_SIZE             (512 * 1024)
#define HT6x3x_FLASH_END_ADDRESS      ((uint32_t)(HT6x3x_FLASH_START_ADRESS + HT6x3x_FLASH_SIZE))

/* Internal SRAM memory size[Kbytes] <8-64>, Default: 64*/
#define HT6x3x_SRAM_SIZE      64
#define HT6x3x_SRAM_END       (0x20000000 + HT6x3x_SRAM_SIZE * 1024)

#if defined(__CC_ARM) || defined(__CLANG_ARM)
extern int Image$$RW_IRAM1$$ZI$$Limit;
#define HEAP_BEGIN      ((void *)&Image$$RW_IRAM1$$ZI$$Limit)
#elif __ICCARM__
#pragma section="CSTACK"
#define HEAP_BEGIN      (__segment_end("CSTACK"))
#else
extern int __bss_end;
#define HEAP_BEGIN      ((void *)&__bss_end)
#endif

#define HEAP_END        HT6x3x_SRAM_END


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

/*����*/
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

/*�ⲿ�жϿ�*/
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

#define	 C_PMUSTA		 0x0007
#define  PFAL_Read()     (((HT_PMU->PMUSTA & 0x0004) == 0x0004)?1:0)

//MCU���ʣ��û������ã�---------------------------------------------------------
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


void rt_hw_board_init(void);

#ifdef __cplusplus
}
#endif

#endif /* __BOARD_H__ */
