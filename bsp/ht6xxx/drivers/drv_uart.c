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
#include "ht6xxx_uart&7816.h"
#include "ht6xxx_dma.h"
#include "drv_gpio.h"
#include "HD_System.h"

#define UART_IT_RXNE  (1)
#define UART_IT_TXNE  (2)

#ifdef BSP_USING_UART

struct ht_uart_t
{
    HT_UART_TypeDef        *UARTx;
    IRQn_Type              *IRQx;
#if defined(BSP_UART_USING_DMA_TX) || defined(BSP_UART_USING_DMA_RX)
    HT_DMA_Channel_TypeDef *DMAx;
    rt_uint32_t             dma_request_id;
    rt_uint8_t             *dma_tx_buf;
    int                     dma_bufsz;
#endif
};

static void HD_UART_ITConfig(HT_UART_TypeDef *UARTx, uint8_t USART_IT, FunctionalState NewState)
{
    if(USART_IT == UART_IT_RXNE)
    {
        if(NewState == ENABLE)
        {
            UARTx->UARTCON |= 0x08;
            UARTx->UARTCON |= 0x02;
        }
        else
        {
            UARTx->UARTCON &= ~0x08;
            UARTx->UARTCON &= ~0x02;
        }
    }
    if(USART_IT == UART_IT_TXNE)
    {
        if(NewState == ENABLE)
        {
            UARTx->UARTCON |= 0x04;
            UARTx->UARTCON |= 0x01;
        }
        else
        {
            UARTx->UARTCON &= ~0x04;
            UARTx->UARTCON &= ~0x01;
        }
    }
}

static uint16_t HD_UART_BaudConfig(uint32_t ulBaud)
{
    uint16_t usCfg;

    usCfg = HT_CMU_SysClkGet() / (ulBaud * 2) - 1;

    return (usCfg);
}

/**
 * @brief Enable the UART
 *
 * @param Uart driver
 *
 * This function is Enable the UART
 *
 * @return None.
 */
static void rt_hw_uart_enable(HT_UART_TypeDef* uart)
{
	_OPEN_REG_WRITE;

#if defined(BSP_USING_UART0)
	if(uart == HT_UART0)
	{
		HT_CMU->CLKCTRL1 |= UART0_EN;
		HD_System_GPIO_DIR(HT_GPIOC, Pin_3, PUPDR_UP, OUTPUT);
		HD_System_GPIO_DIR(HT_GPIOC, Pin_2, PUPDR_UP, INPUT);
		HD_System_GPIO_AF(HT_GPIOC, Pin_3, GPIO_AF_TX0, PUPDR_OD);
		HD_System_GPIO_AF(HT_GPIOC, Pin_2, GPIO_AF_RX0, PUPDR_OD);
		NVIC_SetPriority(UART0_IRQn, 0x00);
		NVIC_EnableIRQ(UART0_IRQn);
	}
#endif /* BSP_USING_UART0 */

#if defined(BSP_USING_UART1)
    if(uart == HT_UART1)
	{
        HT_CMU->CLKCTRL1 |= UART1_EN;
        HD_System_GPIO_DIR(HT_GPIOC, Pin_0, PUPDR_UP, OUTPUT);
        HD_System_GPIO_DIR(HT_GPIOC, Pin_1, PUPDR_UP, INPUT);
        HD_System_GPIO_AF(HT_GPIOC, Pin_0, GPIO_AF_TX1, PUPDR_OD);
        HD_System_GPIO_AF(HT_GPIOC, Pin_1, GPIO_AF_RX1, PUPDR_OD);
		NVIC_SetPriority(UART1_IRQn, 0x00);
		NVIC_EnableIRQ(UART1_IRQn);
    }
#endif /* BSP_USING_UART1 */

#if defined(BSP_USING_UART2)
    if(uart == HT_UART2)
	{
        HT_CMU->CLKCTRL1 |= UART2_EN;
        HD_System_GPIO_DIR(HT_GPIOC, Pin_11, PUPDR_UP, OUTPUT);
        HD_System_GPIO_DIR(HT_GPIOC, Pin_12, PUPDR_UP, INPUT);
        HD_System_GPIO_AF(HT_GPIOC, Pin_11, GPIO_AF_TX2, PUPDR_OD);
        HD_System_GPIO_AF(HT_GPIOC, Pin_12, GPIO_AF_RX2, PUPDR_OD);
		NVIC_SetPriority(UART2_IRQn, 0x00);
		NVIC_EnableIRQ(UART2_IRQn);
    }
#endif /* BSP_USING_UART2 */

#if defined(BSP_USING_UART3)
    if(uart == HT_UART3)
    {
        HT_CMU->CLKCTRL1 |= UART3_EN;
        HD_System_GPIO_DIR(HT_GPIOE, Pin_5, PUPDR_UP, OUTPUT);
        HD_System_GPIO_DIR(HT_GPIOE, Pin_4, PUPDR_UP, INPUT);
        HD_System_GPIO_AF(HT_GPIOE, Pin_5, GPIO_AF_TX3, PUPDR_OD);
        HD_System_GPIO_AF(HT_GPIOE, Pin_4, GPIO_AF_RX3, PUPDR_OD);
		NVIC_SetPriority(UART3_IRQn, 0x00);
		NVIC_EnableIRQ(UART3_IRQn);
    }
#endif /* BSP_USING_UART3 */
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
static void rt_hw_uart_disable(HT_UART_TypeDef* uart)
{
	HD_UART_ITConfig(uart, UART_IT_RXNE, DISABLE);
    HD_UART_ITConfig(uart, UART_IT_TXNE, DISABLE);
}

static rt_err_t uart_configure(struct rt_serial_device *serial, struct serial_configure *cfg)
{
    HT_UART_TypeDef* uart;

    RT_ASSERT(serial != RT_NULL);
    RT_ASSERT(cfg != RT_NULL);

    uart = (HT_UART_TypeDef *)serial->parent.user_data;
    RT_ASSERT(uart != RT_NULL);

    _OPEN_REG_WRITE;

#if defined(BSP_USING_UART0)
    if(uart == HT_UART0)
	{
		HT_CMU->CLKCTRL1 |= UART0_EN;
	}
#endif /* BSP_USING_UART0 */

#if defined(BSP_USING_UART1)
    if(uart == HT_UART1)
	{
        HT_CMU->CLKCTRL1 |= UART1_EN;
    }
#endif /* BSP_USING_UART1 */

#if defined(BSP_USING_UART2)
    if(uart == HT_UART2)
	{
        HT_CMU->CLKCTRL1 |= UART2_EN;
    }
#endif /* BSP_USING_UART2 */

#if defined(BSP_USING_UART3)
    if(uart == HT_UART3)
    {
        HT_CMU->CLKCTRL1 |= UART3_EN;
    }
#endif /* BSP_USING_UART3 */

	/* baudrate */
	uart->SREL = HD_UART_BaudConfig(cfg->baud_rate);

    if (cfg->data_bits == DATA_BITS_8 && cfg->parity == PARITY_NONE)
	{	/* 8N1 */
		uart->UARTCON = 0x000f;
	}
    else if (cfg->data_bits == DATA_BITS_8 && cfg->parity == PARITY_ODD)
	{	/* 8O1 */
		uart->UARTCON = 0x003F;
	}
    else if (cfg->data_bits == DATA_BITS_8 && cfg->parity == PARITY_EVEN)
	{	/* 8E1 */
		uart->UARTCON = 0x005f;
	}
	else if (cfg->data_bits == DATA_BITS_7 && cfg->parity == PARITY_EVEN)
	{	/* 7E1 */
		uart->UARTCON = 0x00df;
	}
	else
	{	/* default 8N1 */
		uart->UARTCON = 0x000f;
	}

    /* Enable the uart interrupt */
    HT_UART_ClearITPendingBit(uart, UART_UARTSTA_RXIF);
    HT_UART_ClearITPendingBit(uart, UART_UARTSTA_TXIF);
    HD_UART_ITConfig(uart, UART_IT_RXNE, ENABLE);
    HD_UART_ITConfig(uart, UART_IT_TXNE, DISABLE);

    return RT_EOK;
}

char dma_buf[128];
static rt_err_t uart_control(struct rt_serial_device *serial, int cmd, void *arg)
{
    HT_UART_TypeDef* uart;
    DMA_InitTypeDef DMA_InitStruct;
    rt_uint32_t ctrl_arg = (rt_uint32_t)(arg);

    RT_ASSERT(serial != RT_NULL);
    uart = (HT_UART_TypeDef *)serial->parent.user_data;

    RT_ASSERT(uart != RT_NULL);

    _OPEN_REG_WRITE;
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
                HD_UART_ITConfig(uart, UART_IT_RXNE, ENABLE);
            }
            else if(ctrl_arg == RT_DEVICE_FLAG_INT_TX)
            {
                HD_UART_ITConfig(uart, UART_IT_TXNE, ENABLE);
            }
            rt_hw_uart_enable(uart);
            break;
        /* UART config */
        case RT_DEVICE_CTRL_CONFIG:
        #if defined(BSP_UART_USING_DMA_TX) || defined(BSP_UART_USING_DMA_RX)
            if(ctrl_arg == RT_DEVICE_FLAG_DMA_RX)
            {

            }
            else if(ctrl_arg == RT_DEVICE_FLAG_DMA_TX)
            {
                DMA_InitStruct.DMA_Request = DMA_Request_UART3_TX;
                DMA_InitStruct.DMA_SourceAddr = (uint32_t)dma_buf;
                DMA_InitStruct.DMA_DestinationAddr = (uint32_t)&HT_UART3->SBUF;
                DMA_InitStruct.DMA_TransferNum = 128;
                DMA_InitStruct.DMA_BulkSize = 1;
                DMA_InitStruct.DMA_SourceAddrInc = DMA_SourceAddrInc_AutoIncrease;
                DMA_InitStruct.DMA_DestinationAddrInc = DMA_DestinationAddrInc_NoIncrease;
                DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
                DMA_InitStruct.DMA_TransferMode = DMA_TransferMode_SingleTransfer;
                DMA_InitStruct.DMA_CycleMode = DMA_CycleMode_NoCycleTransfer;
                HT_DMA_Init(HT_DMA_Channel0, &DMA_InitStruct);

                HT_DMA_ClearITPendingBit(DMA_DMAIE_TCIE0);
                HT_DMA_ITConfig(DMA_DMAIE_TCIE0, ENABLE);

                DMA_Cmd(HT_DMA_Channel0, DISABLE);

                NVIC_SetPriority(DMA_IRQn, 0x00);
		        NVIC_EnableIRQ(DMA_IRQn);
            }
        #endif
            break;
    }

    return RT_EOK;
}

static int uart_putc(struct rt_serial_device *serial, char c)
{
    HT_UART_TypeDef* uart;

    RT_ASSERT(serial != RT_NULL);
    uart = (HT_UART_TypeDef *)serial->parent.user_data;

    RT_ASSERT(uart != RT_NULL);

    uart->SBUF = c;
	while(SET != HT_UART_ITFlagStatusGet(uart, UART_UARTSTA_TXIF));
	HT_UART_ClearITPendingBit(uart, UART_UARTSTA_TXIF);

    return 1;
}

static int uart_getc(struct rt_serial_device *serial)
{
    int ch;
    HT_UART_TypeDef* uart;

    RT_ASSERT(serial != RT_NULL);
    uart = (HT_UART_TypeDef *)serial->parent.user_data;
    RT_ASSERT(uart != RT_NULL);

	ch = -1;
	if (HT_UART_ITFlagStatusGet(uart, UART_UARTSTA_RXIF))
    {
		HT_UART_ClearITPendingBit(uart, UART_UARTSTA_RXIF);
        ch = (uart->SBUF) & 0Xff;
    }

    return ch;
}

#if defined(BSP_UART_USING_DMA_TX) || defined(BSP_UART_USING_DMA_RX)
static rt_size_t uart_dma_transmit(struct rt_serial_device *serial, rt_uint8_t *buf, rt_size_t size, int direction)
{
    HT_UART_TypeDef* uart;
    int len = 0;

    RT_ASSERT(serial != RT_NULL);
    uart = (HT_UART_TypeDef *)serial->parent.user_data;
    RT_ASSERT(uart != RT_NULL);

    if(direction == RT_SERIAL_DMA_TX)
    {
        len = MIN(128, size);
        rt_memcpy(dma_buf, buf, len);
        HT_DMA_Channel0->CHNCNT = len;
        DMA_Cmd(HT_DMA_Channel0, ENABLE);
    }

    return len;
}
#endif

/**
 * Uart common interrupt process. This need add to uart ISR.
 *
 * @param serial serial device
 */
static void uart_isr(struct rt_serial_device *serial)
{
    HT_UART_TypeDef* uart;

    RT_ASSERT(serial != RT_NULL);
    uart = (HT_UART_TypeDef *) serial->parent.user_data;

    RT_ASSERT(uart != RT_NULL);

    if (HT_UART_ITFlagStatusGet(uart, UART_UARTSTA_RXIF))
    {
        rt_hw_serial_isr(serial, RT_SERIAL_EVENT_RX_IND);
    }

    if (HT_UART_ITFlagStatusGet(uart, UART_UARTSTA_TXIF))
    {
		//HT_UART_ClearITPendingBit(uart, UART_UARTSTA_TXIF);
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

#if defined(BSP_UART_USING_DMA_TX) || defined(BSP_UART_USING_DMA_RX)
void DMA_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    if(HT_DMA_ITFlagStatusGet(DMA_DMAIF_TCIF0))
    {
        HT_DMA_ClearITPendingBit(DMA_DMAIF_TCIF0);
        rt_hw_serial_isr(&serial3, RT_SERIAL_EVENT_TX_DMADONE);
    }

    if(HT_DMA_ITFlagStatusGet(DMA_DMAIF_TCIF1))
    {
        HT_DMA_ClearITPendingBit(DMA_DMAIF_TCIF1);
    }

    if(HT_DMA_ITFlagStatusGet(DMA_DMAIF_TCIF2))
    {
        HT_DMA_ClearITPendingBit(DMA_DMAIF_TCIF2);
    }

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif

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
    HT_UART_TypeDef *uart;

#if defined(BSP_USING_UART0)
    uart = HT_UART0;

    serial0.ops    = &uart_ops;
    serial0.config = config;

    /* register UART0 device */
    rt_hw_serial_register(&serial0, "uart0", RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX, uart);
#endif /* BSP_USING_UART0 */

#if defined(BSP_USING_UART1)
    uart = HT_UART1;

    serial1.ops    = &uart_ops;
    serial1.config = config;

    /* register UART1 device */
    rt_hw_serial_register(&serial1, "uart1", RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX, uart);
#endif /* BSP_USING_UART1 */

#if defined(BSP_USING_UART3)
    uart = HT_UART3;

    serial3.ops    = &uart_ops;
    serial3.config = config;

    /* register UART3 device */
    rt_hw_serial_register(&serial3, "uart3", RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX, uart);
#endif /* BSP_USING_UART3 */

    return 0;
}
INIT_BOARD_EXPORT(rt_hw_uart_init);

#endif /* BSP_USING_UART */

/*@}*/
