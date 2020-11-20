/*
 * File      : uart.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2017, RT-Thread Development Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-09-02     Shijj        the first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "board.h"
#include "drv_gpio.h"

#ifdef BSP_USING_UART

/**
 * @brief Enable the UART
 *
 * @param Uart driver
 *
 * This function is Enable the UART
 *
 * @return None.
 */
static void rt_hw_uart_enable(UART_TypeDef* uart)
{
    UART_Cmd(uart, UART_MODE_RX | UART_MODE_TX, ENABLE);
}

/**
 * @brief Disable the UART
 *
 * @param Uart driver
 *
 * This function is Disable the UART
 *
 * @return None.
 */
static void rt_hw_uart_disable(UART_TypeDef* uart)
{
	UART_Cmd(uart, UART_MODE_RX | UART_MODE_TX, DISABLE);
}

static rt_err_t uart_configure(struct rt_serial_device *serial, struct serial_configure *cfg)
{
    UART_TypeDef* uart;
    UART_InitType UART_InitStruct;

    RT_ASSERT(serial != RT_NULL);
    RT_ASSERT(cfg != RT_NULL);

    uart = (UART_TypeDef *)serial->parent.user_data;
    RT_ASSERT(uart != RT_NULL);

	/* baudrate */
    UART_InitStruct.Baudrate = cfg->baud_rate;
	UART_InitStruct.Mode = UART_MODE_RX | UART_MODE_TX;
	UART_InitStruct.FirstBit = UART_FIRSTBIT_LSB;

    if (cfg->data_bits == DATA_BITS_8 && cfg->parity == PARITY_NONE)
	{	/* 8N1 */
		UART_InitStruct.WordLen = UART_WORDLEN_8B;
        UART_InitStruct.Parity = UART_PARITY_NONE;
	}
    else if (cfg->data_bits == DATA_BITS_8 && cfg->parity == PARITY_ODD)
	{	/* 8O1 */
		UART_InitStruct.WordLen = UART_WORDLEN_9B;
        UART_InitStruct.Parity = UART_PARITY_ODD;
	}
    else if (cfg->data_bits == DATA_BITS_8 && cfg->parity == PARITY_EVEN)
	{	/* 8E1 */
		UART_InitStruct.WordLen = UART_WORDLEN_9B;
        UART_InitStruct.Parity = UART_PARITY_EVEN;
	}
	else if (cfg->data_bits == DATA_BITS_7 && cfg->parity == PARITY_EVEN)
	{	/* 7E1 */
		UART_InitStruct.WordLen = UART_WORDLEN_8B;
        UART_InitStruct.Parity = UART_PARITY_EVEN;
	}
	else
	{	/* default 8N1 */
		UART_InitStruct.WordLen = UART_WORDLEN_9B;
        UART_InitStruct.Parity = UART_PARITY_NONE;
	}

    UART_DeInit(uart);
    UART_Init(uart, &UART_InitStruct);

    /* Enable UART transmit doen/receive interrupt */
    UART_INTConfig(uart, UART_INT_RX, ENABLE);
    UART_INTConfig(uart, UART_INT_TXDONE, DISABLE);

    if(uart == UART0)
    {
        CORTEX_SetPriority_ClearPending_EnableIRQ(UART0_IRQn, 0);
    }
    else if(uart == UART1)
    {
        CORTEX_SetPriority_ClearPending_EnableIRQ(UART1_IRQn, 0);
    }
    else if(uart == UART2)
    {
        CORTEX_SetPriority_ClearPending_EnableIRQ(UART2_IRQn, 0);
    }
    else if(uart == UART3)
    {
        CORTEX_SetPriority_ClearPending_EnableIRQ(UART3_IRQn, 0);
    }
    else if(uart == UART4)
    {
        CORTEX_SetPriority_ClearPending_EnableIRQ(UART4_IRQn, 0);
    }
    else if(uart == UART5)
    {
        CORTEX_SetPriority_ClearPending_EnableIRQ(UART5_IRQn, 0);
    }

    rt_hw_uart_enable(uart);

    return RT_EOK;
}

static rt_err_t uart_control(struct rt_serial_device *serial, int cmd, void *arg)
{
    UART_TypeDef* uart;
    rt_uint32_t ctrl_arg = (rt_uint32_t)(arg);

    RT_ASSERT(serial != RT_NULL);
    uart = (UART_TypeDef *)serial->parent.user_data;

    RT_ASSERT(uart != RT_NULL);

    switch (cmd)
    {
        /* disable interrupt */
        case RT_DEVICE_CTRL_CLR_INT:
            rt_hw_uart_disable(uart);
            break;
        /* enable interrupt */
        case RT_DEVICE_CTRL_SET_INT:
            if(ctrl_arg == RT_DEVICE_FLAG_INT_RX)
            {
                UART_INTConfig(uart, UART_INTSTS_RX, ENABLE);
            }
            else if(ctrl_arg == RT_DEVICE_FLAG_INT_TX)
            {
                UART_INTConfig(uart, UART_INT_TXDONE, ENABLE);
            }
            rt_hw_uart_enable(uart);
            break;
        /* UART config */
        case RT_DEVICE_CTRL_CONFIG:
            uart_configure(serial, arg);
            break;
    }

    return RT_EOK;
}

static int uart_putc(struct rt_serial_device *serial, char c)
{
    UART_TypeDef* uart;

    RT_ASSERT(serial != RT_NULL);
    uart = (UART_TypeDef *)serial->parent.user_data;

    RT_ASSERT(uart != RT_NULL);

    UART_SendData(uart, c);
    while(1!= UART_GetFlag(uart, UART_FLAG_TXDONE));
    UART_ClearFlag(uart, UART_FLAG_TXDONE);

    return 1;
}

static int uart_getc(struct rt_serial_device *serial)
{
    int ch;
    UART_TypeDef* uart;

    RT_ASSERT(serial != RT_NULL);
    uart = (UART_TypeDef *)serial->parent.user_data;
    RT_ASSERT(uart != RT_NULL);

	ch = -1;
	if(UART_GetINTStatus(uart, UART_INTSTS_RX))
    {
        ch = UART_ReceiveData(uart);
		UART_ClearINTStatus(uart, UART_INTSTS_RX);
    }

    return ch;
}

/**
 * Uart common interrupt process. This need add to uart ISR.
 *
 * @param serial serial device
 */
static void uart_isr(struct rt_serial_device *serial)
{
    UART_TypeDef* uart;

    RT_ASSERT(serial != RT_NULL);
    uart = (UART_TypeDef *) serial->parent.user_data;

    RT_ASSERT(uart != RT_NULL);

    if (UART_GetINTStatus(uart, UART_INTSTS_RX))
    {
        rt_hw_serial_isr(serial, RT_SERIAL_EVENT_RX_IND);
    }

    if (UART_GetINTStatus(uart, UART_INTSTS_TXDONE))
    {
		//UART_ClearINTStatus(uart, UART_INTSTS_TXDONE);
        //rt_hw_serial_isr(serial, RT_SERIAL_EVENT_TX_DONE);
    }
}

static const struct rt_uart_ops uart_ops =
{
    uart_configure,
    uart_control,
    uart_putc,
    uart_getc,
};

#if defined(BSP_USING_UART0)
static struct rt_serial_device serial0;
void UART0_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    uart_isr(&serial0);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* BSP_USING_UART0 */

#if defined(BSP_USING_UART1)
static struct rt_serial_device serial1;
void UART1_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    uart_isr(&serial1);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* BSP_USING_UART1 */

#if defined(BSP_USING_UART2)
static struct rt_serial_device serial2;
void UART2_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    uart_isr(&serial2);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* BSP_USING_UART2 */

#if defined(BSP_USING_UART3)
static struct rt_serial_device serial3;
void UART3_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    uart_isr(&serial3);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* BSP_USING_UART3 */

#if defined(BSP_USING_UART4)
static struct rt_serial_device serial4;
void UART4_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    uart_isr(&serial4);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* BSP_USING_UART4 */

#if defined(BSP_USING_UART5)
static struct rt_serial_device serial5;
void UART5_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    uart_isr(&serial5);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* BSP_USING_UART5 */
/**
 * @brief Initialize the UART
 *
 * This function initialize the UART
 *
 * @return None.
 */
int rt_hw_uart_init(void)
{
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;
    UART_TypeDef *uart;

#if defined(BSP_USING_UART0)
    uart = UART0;

    GPIOA_Init(GPIOA, &(GPIO_InitType){GPIO_Pin_12, GPIO_Mode_INPUT});
    GPIOBToF_Init(GPIOB, &(GPIO_InitType){GPIO_Pin_2, GPIO_Mode_OUTPUT_CMOS});

    serial0.ops    = &uart_ops;
    serial0.config = config;

    /* register UART0 device */
    rt_hw_serial_register(&serial0, "uart0", RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX, uart);
#endif /* BSP_USING_UART0 */

#if defined(BSP_USING_UART1)
    uart = UART1;

    GPIOA_Init(GPIOA, &(GPIO_InitType){GPIO_Pin_13, GPIO_Mode_INPUT});
    GPIOBToF_Init(GPIOB, &(GPIO_InitType){GPIO_Pin_3, GPIO_Mode_OUTPUT_CMOS});

    serial1.ops    = &uart_ops;
    serial1.config = config;

    /* register UART1 device */
    rt_hw_serial_register(&serial1, "uart1", RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX, uart);
#endif /* BSP_USING_UART1 */

#if defined(BSP_USING_UART2)
    uart = UART2;

    GPIOA_Init(GPIOA, &(GPIO_InitType){GPIO_Pin_14, GPIO_Mode_INPUT});
    GPIOBToF_Init(GPIOB, &(GPIO_InitType){GPIO_Pin_4, GPIO_Mode_OUTPUT_CMOS});

    serial2.ops    = &uart_ops;
    serial2.config = config;

    /* register UART1 device */
    rt_hw_serial_register(&serial2, "uart2", RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX, uart);
#endif /* BSP_USING_UART2 */

#if defined(BSP_USING_UART3)
    uart = UART3;

    GPIOA_Init(GPIOA, &(GPIO_InitType){GPIO_Pin_15, GPIO_Mode_INPUT});
    GPIOBToF_Init(GPIOB, &(GPIO_InitType){GPIO_Pin_5, GPIO_Mode_OUTPUT_CMOS});

    serial3.ops    = &uart_ops;
    serial3.config = config;

    /* register UART3 device */
    rt_hw_serial_register(&serial3, "uart3", RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX, uart);
#endif /* BSP_USING_UART3 */

#if defined(BSP_USING_UART4)
    uart = UART4;

    GPIOBToF_Init(GPIOB, &(GPIO_InitType){GPIO_Pin_0, GPIO_Mode_INPUT});
    GPIOBToF_Init(GPIOB, &(GPIO_InitType){GPIO_Pin_6, GPIO_Mode_OUTPUT_CMOS});

    serial4.ops    = &uart_ops;
    serial4.config = config;

    /* register UART3 device */
    rt_hw_serial_register(&serial4, "uart4", RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX, uart);
#endif /* BSP_USING_UART4 */

#if defined(BSP_USING_UART5)
    uart = UART5;

    GPIOBToF_Init(GPIOB, &(GPIO_InitType){GPIO_Pin_1, GPIO_Mode_INPUT});
    GPIOBToF_Init(GPIOB, &(GPIO_InitType){GPIO_Pin_7, GPIO_Mode_OUTPUT_CMOS});/* V8510 no PB7 */

    serial5.ops    = &uart_ops;
    serial5.config = config;

    /* register UART3 device */
    rt_hw_serial_register(&serial5, "uart5", RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX, uart);
#endif /* BSP_USING_UART5 */

    return 0;
}
//INIT_BOARD_EXPORT(rt_hw_uart_init);

#endif /* BSP_USING_UART */

/*@}*/
