/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-09-05     Shijj             the first version
 */
#ifndef __GPIO_H__
#define __GPIO_H__

#include "board.h"

#define GPIOA
#define GPIOB
#define GPIOC
#define GPIOD
#define GPIOE
#define GPIOG
#define GPIOH
#define GPIOI

#define __HT_PORT(port)         HT_GPIO##port

#define GET_PIN(PORTx, PIN)     (rt_base_t)((16 * ( ((rt_base_t)__HT_PORT(PORTx) - (rt_base_t)HT_GPIOA)/(0x0100UL) )) + PIN)

#define __HT_PIN(index, gpio, gpio_index)               \
    {                                                   \
      index, HT_GPIO##gpio, Pin_##gpio_index            \
    }

#define __HT_PIN_RESERVE                                \
    {                                                   \
        -1, 0, 0                                        \
    }

/* GPIO driver */
struct pin_index
{
    int index;
    HT_GPIO_TypeDef *gpio;
    uint32_t pin;
};

struct pin_irq_map
{
    rt_uint16_t pinbit;
    IRQn_Type irqno;
};

int rt_hw_pin_init(void);

#define HD_System_GPIO_DATA(GPIOx, GPIO_PIN, ON) if(ON)GPIOx->PTSET |= (1<<GPIO_PIN);else GPIOx->PTCLR |= (1<<GPIO_PIN);
uint8_t HD_System_GPIO_Read(HT_GPIO_TypeDef* GPIOx,uint32_t GPIO_PIN);
uint8_t HD_System_GPIO_DIR_Read(HT_GPIO_TypeDef *GPIOx, uint32_t GPIO_PIN);
void 	HD_System_GPIO_DIR( HT_GPIO_TypeDef* GPIOx,uint32_t GPIO_PIN,uint8_t PuPd,uint8_t DIR);
void 	HD_System_GPIO_AF(HT_GPIO_TypeDef* GPIOx, uint32_t GPIO_PIN, uint8_t ucGPIOAF, uint8_t PuPd);

// EXTERNAL MODULE
// uart
#define IO_UART0_TX    	GET_PIN(C, 3)      // PC3 :  UART0_TX     --> EXTERNAL MODULE
#define IO_UART0_RX    	GET_PIN(C, 2)      // PC2 :  UART0_RX     --> EXTERNAL MODULE

#define IO_UART1_TX    	GET_PIN(C, 0)      // PC0 :  UART1_TX     --> EXTERNAL MODULE
#define IO_UART1_RX    	GET_PIN(C, 1)      // PC1 :  UART1_RX     --> EXTERNAL MODULE

#define IO_UART2_TX    	GET_PIN(C, 11)     // PC11:  UART2_TX     --> EXTERNAL MODULE
#define IO_UART2_RX		GET_PIN(C, 12)     // PC12:  UART2_RX     --> EXTERNAL MODULE

#define IO_UART3_TX    	GET_PIN(E, 5)      // PE5 :  UART3_TX     --> EXTERNAL MODULE
#define IO_UART3_RX		GET_PIN(E, 4)      // PE4 :  UART3_RX     --> EXTERNAL MODULE

// led
#define IO_RELAY_A_LED  GET_PIN(A, 8)
#define IO_RELAY_B_LED  GET_PIN(B, 4)
#define IO_RELAY_C_LED  GET_PIN(B, 5)
#define IO_RUN_LED  	GET_PIN(B, 9)
#define IO_TX_LED  		GET_PIN(B, 7)
#define IO_RX_LED  		GET_PIN(B, 8)
#define IO_485_LED  	GET_PIN(B, 10)
#define IO_SIG_1_LED  	GET_PIN(C, 12)
#define IO_SIG_2_LED  	GET_PIN(C, 11)
#define IO_SIG_3_LED  	GET_PIN(B, 14)
#define IO_SIG_4_LED  	GET_PIN(B, 12)

// spi
#define IO_FLASH_WP     GET_PIN(A, 5)
#define IO_FLASH_CS     GET_PIN(C, 7)
#define IO_SPI0_SCK     GET_PIN(C, 6)
#define IO_SPI0_MISO    GET_PIN(C, 5)
#define IO_SPI0_MOSI    GET_PIN(C, 4)

// i2c
#define IO_EE_WP        GET_PIN(E, 3)
#define PIN_I2C1_SCL    GET_PIN(B, 8)
#define PIN_I2C1_SDA    GET_PIN(B, 9)

// io
#define IO_MODEM_PWRKEY GET_PIN(C, 9)
#define IO_MODEM_RESET  GET_PIN(C, 10)

#endif /* __GPIO_H__ */

