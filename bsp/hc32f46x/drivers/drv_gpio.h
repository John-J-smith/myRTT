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
#define BSP_USING_GPIOH

#define __PORT(port)            Port##port

/* get the pin index in pin table */
#define GET_PIN(PORTx, PIN)     (rt_base_t)((16 * ( ((rt_base_t)__PORT(PORTx) - (rt_base_t)PortA)) + PIN))

#define __PIN(index, gpio, gpio_index)                  \
    {                                                   \
        index, Port##gpio, Pin##gpio_index        \
    }

#define __PIN_RESERVE                                   \
    {                                                   \
        -1, 0, 0                                        \
    }

struct pin_index
{
    int index;
    en_port_t gpio;
    en_pin_t pin;
};

struct pin_irq_map
{
    rt_uint16_t pinbit;
    IRQn_Type irqno;
};

int rt_hw_pin_init(void);

/* Pin map */
// led
#define IO_ACT_LED      GET_PIN(B, 14)
#define IO_TX_LED       GET_PIN(B, 12)
#define IO_RX_LED       GET_PIN(B, 13)

// data flash write enable & chip select
#define IO_FLASH_CS     GET_PIN(A, 13)

// spi1
#define IO_SPI3_SCK     GET_PIN(B, 10)
#define IO_SPI3_MISO    GET_PIN(B, 11)
#define IO_SPI3_MOSI    GET_PIN(B, 12)

#define IO_TR_RS485_1   GET_PIN(A, 4)
#define IO_TR_RS485_2   GET_PIN(A, 2)
#define IO_TR_RS485_3   GET_PIN(B, 7)
#define IO_TR_RS485_4   GET_PIN(B, 5)

#define IO_YX_DT_1      GET_PIN(B, 1)
#define IO_YX_DT_2      GET_PIN(B, 0)
#define IO_YX_DT_3      GET_PIN(A, 7)
#define IO_YX_DT_4      GET_PIN(A, 6)

#endif /* __GPIO_H__ */

