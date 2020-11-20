/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-11-19     Shijj        the first version
 */
#ifndef __GPIO_H__
#define __GPIO_H__

#include "board.h"

#define BSP_USING_GPIOA
#define BSP_USING_GPIOB
#define BSP_USING_GPIOC
#define BSP_USING_GPIOD
#define BSP_USING_GPIOE
//#define BSP_USING_GPIOG
//#define BSP_USING_GPIOH
//#define BSP_USING_GPIOI

#define __PORT(port)            GPIO##port

/* get the pin index in pin table */
#define GET_PIN(PORTx, PIN)     (((rt_base_t)__PORT(PORTx) > (rt_base_t)GPIOA) ? PIN : (rt_base_t)((16 * ( ((rt_base_t)__PORT(PORTx) - (rt_base_t)GPIOB)/(0x20UL) + 1 )) + PIN))

#define __PIN(index, gpio, gpio_index)                  \
    {                                                   \
        index, GPIO##gpio, GPIO_Pin_##gpio_index        \
    }

#define __PIN_RESERVE                                   \
    {                                                   \
        -1, 0, 0                                        \
    }


/* GPIOA & GPIOB~F driver */
struct pin_index
{
    int index;
    void *gpio;
    uint32_t pin;
};

struct pin_irq_map
{
    rt_uint16_t pinbit;
    IRQn_Type irqno;
};

int rt_hw_pin_init(void);

/* Pin map */
// led
#define IO_RUN_LED  	GET_PIN(A, 9)
#define IO_TX_LED  		GET_PIN(A, 8)
#define IO_RX_LED  		GET_PIN(A, 3)

// data flash write enable & chip select
#define IO_FLASH_WP     GET_PIN(C, 4)
#define IO_FLASH_CS     GET_PIN(C, 0)

// spi1
#define IO_SPI1_SCK     GET_PIN(B, 10)
#define IO_SPI1_MISO    GET_PIN(B, 11)
#define IO_SPI1_MOSI    GET_PIN(B, 12)

// spi2
#define IO_SPI2_SCK     GET_PIN(C, 1)
#define IO_SPI2_MISO    GET_PIN(C, 2)
#define IO_SPI2_MOSI    GET_PIN(C, 3)

// i2c
#define IO_EE_WP        GET_PIN(E, 3)
#define IO_I2C1_SCL     GET_PIN(B, 8)
#define IO_I2C1_SDA     GET_PIN(B, 9)

// io
#define IO_MODEM_PWRKEY GET_PIN(C, 9)
#define IO_MODEM_RESET  GET_PIN(C, 10)

#endif /* __GPIO_H__ */

