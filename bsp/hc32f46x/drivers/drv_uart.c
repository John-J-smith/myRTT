/*
 * Copyright (c) Aug.2,2004  R&D Institute of Reallin Co., Ltd.
 *
 * usart device driver
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-09-02     John Shi     first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "board.h"
#include "drv_gpio.h"

#ifdef BSP_USING_UART

typedef struct hdsc_uart
{
    M4_USART_TypeDef   *base;

    en_port_t           enTxPort;
    en_pin_t            usTxPin;
    en_port_func_t      enTxPortFunc;
    stc_irq_regi_conf_t tx_irq_conf;

    en_port_t           enRxPort;
    en_pin_t            usRxPin;
    en_port_func_t      enRxPortFunc;
    stc_irq_regi_conf_t rx_irq_conf;
} hdsc_uart_t;

/**
 * Uart common interrupt process. This need add to uart ISR.
 *
 * @param serial serial device
 */
static void uart_isr(struct rt_serial_device *serial)
{
    hdsc_uart_t* uart;

    RT_ASSERT(serial != RT_NULL);
    uart = (hdsc_uart_t *) serial->parent.user_data;

    RT_ASSERT(uart != RT_NULL);

    if(USART_GetStatus(uart->base, UsartRxNoEmpty) == Set)//自动清零
    {
        rt_hw_serial_isr(serial, RT_SERIAL_EVENT_RX_IND);
    }

    if(USART_GetStatus(uart->base, UsartTxComplete) == Set)
    {

    }
}

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

static hdsc_uart_t hdsc_uart_1 =
{
    M4_USART1,
    /* USART TX conifg */
    PortB, Pin06, Func_Usart1_Tx,
    {INT_USART1_TCI, Int000_IRQn, &UART1_IRQHandler},
    /* USART RX conifg */
    PortB, Pin07, Func_Usart1_Rx,
    {INT_USART1_RI, Int001_IRQn, &UART1_IRQHandler},
};
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

static hdsc_uart_t hdsc_uart_2 =
{
    M4_USART2,
    /* USART TX conifg */
    PortB, Pin06, Func_Usart2_Tx,
    {INT_USART2_TCI, Int002_IRQn, &UART2_IRQHandler},
    /* USART RX conifg */
    PortB, Pin07, Func_Usart2_Rx,
    {INT_USART2_RI, Int003_IRQn, &UART2_IRQHandler},
};
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

static hdsc_uart_t hdsc_uart_3 =
{
    M4_USART3,
    /* USART TX conifg */
    PortB, Pin04, Func_Usart3_Tx,
    {INT_USART3_TCI, Int004_IRQn, &UART3_IRQHandler},
    /* USART RX conifg */
    PortB, Pin03, Func_Usart3_Rx,
    {INT_USART3_RI, Int005_IRQn, &UART3_IRQHandler},
};
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

static hdsc_uart_t hdsc_uart_4 =
{
    M4_USART4,
    /* USART TX conifg */
    PortB, Pin06, Func_Usart4_Tx,
    {INT_USART4_TCI, Int006_IRQn, &UART4_IRQHandler},
    /* USART RX conifg */
    PortB, Pin07, Func_Usart4_Rx,
    {INT_USART4_RI, Int007_IRQn, &UART4_IRQHandler},
};
#endif /* BSP_USING_UART4 */

/**
 * @brief Enable the UART
 *
 * @param Uart driver
 *
 * This function is Enable the UART
 *
 * @return None.
 */
static void rt_hw_uart_enable(M4_USART_TypeDef* uart)
{
    USART_FuncCmd(uart, UsartRx, Enable);
    USART_FuncCmd(uart, UsartTx, Enable);
    USART_FuncCmd(uart, UsartRxInt, Enable);
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
static void rt_hw_uart_disable(M4_USART_TypeDef* uart)
{
	USART_FuncCmd(uart, UsartRx, Disable);
    USART_FuncCmd(uart, UsartTx, Disable);
    USART_FuncCmd(uart, UsartRxInt, Disable);
}

static rt_err_t uart_configure(struct rt_serial_device *serial, struct serial_configure *cfg)
{
    en_result_t enRet = Ok;
    hdsc_uart_t *uart;
    stc_irq_regi_conf_t stcIrqRegiCfg;
    uint32_t u32Fcg1Periph = PWC_FCG1_PERIPH_USART1 | PWC_FCG1_PERIPH_USART2 | \
                             PWC_FCG1_PERIPH_USART3 | PWC_FCG1_PERIPH_USART4;
    stc_usart_uart_init_t stcInitCfg = {
        UsartIntClkCkNoOutput,
        UsartClkDiv_1,
        UsartDataBits8,
        UsartDataLsbFirst,
        UsartOneStopBit,
        UsartParityNone,
        UsartSamleBit8,
        UsartStartBitFallEdge,
        UsartRtsEnable,
    };

    RT_ASSERT(serial != RT_NULL);
    RT_ASSERT(cfg != RT_NULL);

    uart = (hdsc_uart_t *)serial->parent.user_data;
    RT_ASSERT(uart != RT_NULL);

    /* Enable peripheral clock */
    PWC_Fcg1PeriphClockCmd(u32Fcg1Periph, Enable);

    USART_DeInit(uart->base);

    if (cfg->data_bits == DATA_BITS_8 && cfg->parity == PARITY_NONE)
	{	/* 8N1 */
		stcInitCfg.enDataLength = UsartDataBits8;
        stcInitCfg.enParity = UsartParityNone;
	}
    else if (cfg->data_bits == DATA_BITS_8 && cfg->parity == PARITY_ODD)
	{	/* 8O1 */
		stcInitCfg.enDataLength = UsartDataBits9;
        stcInitCfg.enParity = UsartParityOdd;
	}
    else if (cfg->data_bits == DATA_BITS_8 && cfg->parity == PARITY_EVEN)
	{	/* 8E1 */
		stcInitCfg.enDataLength = UsartDataBits9;
        stcInitCfg.enParity = UsartParityEven;
	}
	else if (cfg->data_bits == DATA_BITS_7 && cfg->parity == PARITY_EVEN)
	{	/* 7E1 */
		stcInitCfg.enDataLength = UsartDataBits8;
        stcInitCfg.enParity = UsartParityEven;
	}
	else
	{	/* default 8N1 */
		stcInitCfg.enDataLength = UsartDataBits8;
        stcInitCfg.enParity = UsartParityNone;
	}

    if((enRet = USART_UART_Init(uart->base, &stcInitCfg)) != Ok)
    {
        return enRet;
    }

    // baudrate
    if((enRet = USART_SetBaudrate(uart->base, cfg->baud_rate)) != Ok)
    {
        return enRet;
    }

    /* Initialize USART IO */
    PORT_SetFunc(uart->enRxPort, uart->usRxPin, uart->enRxPortFunc, Disable);
    PORT_SetFunc(uart->enTxPort, uart->usTxPin, uart->enTxPortFunc, Disable);

    // interrupt
    /* USART Rx IRQ */
    stcIrqRegiCfg.enIRQn = uart->rx_irq_conf.enIRQn;
    stcIrqRegiCfg.pfnCallback = uart->rx_irq_conf.pfnCallback;
    stcIrqRegiCfg.enIntSrc = uart->rx_irq_conf.enIntSrc;
    enIrqRegistration(&stcIrqRegiCfg);
    NVIC_SetPriority(stcIrqRegiCfg.enIRQn, DDL_IRQ_PRIORITY_02);
    NVIC_ClearPendingIRQ(stcIrqRegiCfg.enIRQn);
    NVIC_EnableIRQ(stcIrqRegiCfg.enIRQn);

    /* USART Tx IRQ
    stcIrqRegiCfg.enIRQn = uart->tx_irq_conf.enIRQn;
    stcIrqRegiCfg.pfnCallback = uart->tx_irq_conf.pfnCallback;
    stcIrqRegiCfg.enIntSrc = uart->tx_irq_conf.enIntSrc;
    enIrqRegistration(&stcIrqRegiCfg);
    NVIC_SetPriority(stcIrqRegiCfg.enIRQn, DDL_IRQ_PRIORITY_02);
    NVIC_ClearPendingIRQ(stcIrqRegiCfg.enIRQn);
    NVIC_EnableIRQ(stcIrqRegiCfg.enIRQn);*/

    USART_FuncCmd(uart->base, UsartRx, Enable);
    USART_FuncCmd(uart->base, UsartTx, Enable);
    USART_FuncCmd(uart->base, UsartRxInt, Enable);

    return RT_EOK;
}

static rt_err_t uart_control(struct rt_serial_device *serial, int cmd, void *arg)
{
    hdsc_uart_t* uart;
    rt_uint32_t ctrl_arg = (rt_uint32_t)(arg);

    RT_ASSERT(serial != RT_NULL);
    uart = (hdsc_uart_t *)serial->parent.user_data;

    RT_ASSERT(uart != RT_NULL);

    switch (cmd)
    {
        /* disable interrupt */
        case RT_DEVICE_CTRL_CLR_INT:
            rt_hw_uart_disable(uart->base);
            break;
        /* enable interrupt */
        case RT_DEVICE_CTRL_SET_INT:
            if(ctrl_arg == RT_DEVICE_FLAG_INT_RX)
            {

            }
            else if(ctrl_arg == RT_DEVICE_FLAG_INT_TX)
            {

            }
            rt_hw_uart_enable(uart->base);
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
    hdsc_uart_t* uart;

    RT_ASSERT(serial != RT_NULL);
    uart = (hdsc_uart_t *)serial->parent.user_data;

    RT_ASSERT(uart != RT_NULL);

    /* Warit Tx data register empty */
    while (Reset == USART_GetStatus(uart->base, UsartTxEmpty));
    USART_SendData(uart->base, c);

    return 1;
}

static int uart_getc(struct rt_serial_device *serial)
{
    int ch;
    hdsc_uart_t* uart;

    RT_ASSERT(serial != RT_NULL);
    uart = (hdsc_uart_t *)serial->parent.user_data;
    RT_ASSERT(uart != RT_NULL);

	ch = -1;
    if (Set == USART_GetStatus(uart->base, UsartRxNoEmpty))         /* Warit Rx data register no empty */
    {
        ch = USART_RecData(uart->base);
    }

    return ch;
}

static const struct rt_uart_ops uart_ops =
{
    uart_configure,
    uart_control,
    uart_putc,
    uart_getc,
};

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
    hdsc_uart_t *uart;

#if defined(BSP_USING_UART1)
    uart = &hdsc_uart_1;

    serial1.ops    = &uart_ops;
    serial1.config = config;

    /* register UART1 device */
    rt_hw_serial_register(&serial1, "uart1", RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX, uart);
#endif /* BSP_USING_UART1 */

#if defined(BSP_USING_UART2)
    uart = &hdsc_uart_2;

    serial2.ops    = &uart_ops;
    serial2.config = config;

    /* register UART1 device */
    rt_hw_serial_register(&serial2, "uart2", RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX, uart);
#endif /* BSP_USING_UART2 */

#if defined(BSP_USING_UART3)
    uart = &hdsc_uart_3;

    serial3.ops    = &uart_ops;
    serial3.config = config;

    /* register UART3 device */
    rt_hw_serial_register(&serial3, "uart3", RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX, uart);
#endif /* BSP_USING_UART3 */

#if defined(BSP_USING_UART4)
    uart = &hdsc_uart_4;

    serial4.ops    = &uart_ops;
    serial4.config = config;

    /* register UART4 device */
    rt_hw_serial_register(&serial4, "uart4", RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX, uart);
#endif /* BSP_USING_UART4 */

    return 0;
}
//INIT_BOARD_EXPORT(rt_hw_uart_init);

#endif /* BSP_USING_UART */

/*@}*/
