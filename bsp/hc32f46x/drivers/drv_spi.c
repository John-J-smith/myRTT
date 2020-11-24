/*
 * Copyright (c) 2006-2018, Synwit Technology Co.,Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-05-31     ZYH          first version
 * 2018-12-10     Zohar_Lee    format file
 */

#include <board.h>
#include <rtthread.h>
#include <rtdevice.h>

#ifdef RT_USING_SPI

#include "drv_gpio.h"

#ifndef ULOG_USING_SYSLOG
#define LOG_TAG              "drv.spi"
#define LOG_LVL              LOG_LVL_DBG
#include <ulog.h>
#else
#include <syslog.h>
#endif /* ULOG_USING_SYSLOG */

#define SPI1_BUS_NAME       "spi1"
#define SPI2_BUS_NAME       "spi2"

static rt_uint8_t spi_send_recv(SPI_TypeDef *SPIx, rt_uint8_t ch)
{
    while(SPI_GetStatus(SPIx, SPI_STS_TFE) == 0);
    SPI_SendData(SPIx, ch);
    while(SPI_GetStatus(SPIx, SPI_STS_RNE) == 0);
    return SPI_ReceiveData(SPIx);
}

static rt_err_t spi_configure(struct rt_spi_device *device,
                              struct rt_spi_configuration *configuration)
{
    SPI_TypeDef *SPIx = (SPI_TypeDef *)device->bus->parent.user_data;
    SPI_InitType SPI_InitStruct;

    /* SPIx initialization, master mode */
    SPI_DeviceInit(SPIx);
    SPI_InitStruct.ClockDivision = SPI_CLKDIV_4;
    SPI_InitStruct.Mode = SPI_MODE_MASTER;
    SPI_InitStruct.CSNSoft = SPI_CSNSOFT_ENABLE;
    SPI_InitStruct.SPH = SPI_SPH_1;
    SPI_InitStruct.SPO = SPI_SPO_1;
    SPI_InitStruct.SWAP = SPI_SWAP_DISABLE;
    SPI_Init(SPIx, &SPI_InitStruct);
    SPI_ReceiveFIFOLevelConfig(SPIx, SPI_RXFLEV_7);
    SPI_INTConfig(SPIx, SPI_INT_TX, DISABLE);
    SPI_INTConfig(SPIx, SPI_INT_RX, DISABLE);
    SPI_Cmd(SPIx, ENABLE);

	return RT_EOK;
}

static rt_uint32_t spi_xfer(struct rt_spi_device *device,
                            struct rt_spi_message *message)
{
    rt_uint8_t *recv_buf;
    const rt_uint8_t *send_buf;

    RT_ASSERT(device != RT_NULL);
    RT_ASSERT(device->bus != RT_NULL);
    RT_ASSERT(device->bus->parent.user_data != RT_NULL);
    RT_ASSERT(message != RT_NULL);

    SPI_TypeDef *SPIx = (SPI_TypeDef *)device->bus->parent.user_data;
    rt_uint32_t cs = (rt_uint32_t)device->parent.user_data;

    recv_buf = message->recv_buf;
    send_buf = message->send_buf;
    if(message->cs_take)
    {
        rt_pin_mode(cs, PIN_MODE_OUTPUT);
        rt_pin_write(cs, PIN_LOW);
    }

    if (message->send_buf && message->recv_buf)
    {
        for(rt_uint16_t i = 0; i < message->length; i++)
        {
            recv_buf[i] = spi_send_recv(SPIx, send_buf[i]);
        }
    }
    else if (message->send_buf)
    {
        for(rt_uint16_t i = 0; i < message->length; i++)
        {
            spi_send_recv(SPIx, send_buf[i]);
        }
    }
    else
    {
        for(rt_uint16_t i = 0; i < message->length; i++)
        {
            recv_buf[i] = spi_send_recv(SPIx, 0x00);
        }
    }

    if(message->cs_release)
    {
        rt_pin_mode(cs, PIN_MODE_OUTPUT);
        rt_pin_write(cs, PIN_HIGH);
    }

    return message->length;
}

const static struct rt_spi_ops spi_ops =
{
    .configure = spi_configure,
    .xfer = spi_xfer,
};

#ifdef BSP_USING_SPI1
    static struct rt_spi_bus spi1_bus;
#endif /* BSP_USING_SPI1 */

#ifdef BSP_USING_SPI2
    static struct rt_spi_bus spi2_bus;
#endif /* BSP_USING_SPI2 */

//cannot be used before completion init
rt_err_t spi_bus_attach_device(rt_uint32_t pin,
                               const char *bus_name,
                               const char *device_name)
{
    struct rt_spi_device *spi_device = (struct rt_spi_device *)rt_malloc(sizeof(struct rt_spi_device));
    RT_ASSERT(spi_device != RT_NULL);

    rt_pin_mode(pin, PIN_MODE_OUTPUT);
    rt_pin_write(pin, PIN_HIGH);

    return rt_spi_bus_attach_device(spi_device,
                                    device_name,
                                    bus_name,
                                    (void *)pin);
}

int rt_hw_spi_init(void)
{
    int result = 0;
    SPI_TypeDef *SPIx;

    rt_pin_mode(IO_FLASH_WP, PIN_MODE_OUTPUT);
    rt_pin_mode(IO_FLASH_CS, PIN_MODE_OUTPUT);

#ifdef BSP_USING_SPI1
    rt_pin_mode(IO_SPI1_SCK, PIN_MODE_OUTPUT);
    rt_pin_mode(IO_SPI1_MISO, PIN_MODE_INPUT);
    rt_pin_mode(IO_SPI1_MOSI, PIN_MODE_OUTPUT);

    SPIx = SPI1;
    spi1_bus.parent.user_data = SPIx;
    result = rt_spi_bus_register(&spi1_bus, SPI1_BUS_NAME, &spi_ops);
#endif

#ifdef BSP_USING_SPI2
    rt_pin_mode(IO_SPI2_SCK, PIN_MODE_OUTPUT);
    rt_pin_mode(IO_SPI2_MISO, PIN_MODE_INPUT);
    rt_pin_mode(IO_SPI2_MOSI, PIN_MODE_OUTPUT);

    SPIx = SPI2;
    spi2_bus.parent.user_data = SPIx;
    result = rt_spi_bus_register(&spi2_bus, SPI2_BUS_NAME, &spi_ops);
#endif

    return result;
}
INIT_BOARD_EXPORT(rt_hw_spi_init);

#endif /* #ifdef RT_USING_SPI */
