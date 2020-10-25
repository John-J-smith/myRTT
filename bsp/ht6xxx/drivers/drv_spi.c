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
#include "HD_System.h"
   
#ifdef RT_USING_SPI

#include "drv_gpio.h"
#include "ht6xxx_spi.h"

#ifndef ULOG_USING_SYSLOG
#define LOG_TAG              "drv.spi"
#define LOG_LVL              LOG_LVL_DBG
#include <ulog.h>
#else
#include <syslog.h>
#endif /* ULOG_USING_SYSLOG */


#ifdef BSP_USING_SPI0
    #define  IO_DF_CLK_PORT                 HT_GPIOC
    #define  IO_DF_CLK_PIN                  Pin_6

    #define  IO_DF_SDI_PORT                 HT_GPIOC
    #define  IO_DF_SDI_PIN                  Pin_5

    #define  IO_DF_SDO_PORT                 HT_GPIOC
    #define  IO_DF_SDO_PIN                  Pin_4
#endif /* BSP_USING_SPI0 */

#define SPI0_BUS_NAME       "spi0"

#define spi_send_recv(SPIx, ch) HT_SPI_SendByte(SPIx, ch)

static rt_err_t spi_configure(struct rt_spi_device *device,
                              struct rt_spi_configuration *configuration)
{
    HT_SPI_TypeDef *SPIx = (HT_SPI_TypeDef *)device->bus->parent.user_data;
    SPI_InitTypeDef SPI_InitStructure;

	if(SPIx == HT_SPI0)
	{
		HT_CMU_ClkCtrl0Config(CMU_CLKCTRL0_SPI0EN, ENABLE);            /*!< 使能SPI0模块                */
	}
    else if(SPIx == HT_SPI1)
    {
        HT_CMU_ClkCtrl1Config(CMU_CLKCTRL1_SPI1EN, ENABLE);
    }
    else
    {
        return -1;
    }

    SPI_InitStructure.SPI_CSInCtrol = DISABLE;
    SPI_InitStructure.SPI_Baudrate = SPI_BaudRatePrescaler_16;

	if (configuration->mode & RT_SPI_CPHA)
    {
        SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge; /*!< 动态沿通过后数据被发送            */
    }
    else
    {
        SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; /*!< 第一个动态沿之前半周期数据被发送  */
    }

	if (configuration->mode & RT_SPI_CPOL)
    {
        SPI_InitStructure.SPI_CPOL = SPI_CPOL_high;
    }
    else
    {
        SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    }

    SPI_InitStructure.SPI_Mode = SPI_Master;
    //SPI_InitStructure.SPI_CLK = SPI_CLKSEL_PLL44M;

    HT_SPI_Init(SPIx, &SPI_InitStructure);

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

    HT_SPI_TypeDef *SPIx = (HT_SPI_TypeDef *)device->bus->parent.user_data;
    rt_uint32_t cs = (rt_uint32_t)device->user_data;

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
            i++;
        }
    }
    else if (message->send_buf)
    {
        for(rt_uint16_t i = 0; i < message->length; i++)
        {
            spi_send_recv(SPIx, send_buf[i]);
            i++;
        }
    }
    else
    {
        for(rt_uint16_t i = 0; i < message->length; i++)
        {
            recv_buf[i] = spi_send_recv(SPIx, 0x00);
            i++;
        }
    }

    if(message->cs_release)
    {
        rt_pin_mode(cs, PIN_MODE_OUTPUT);
        rt_pin_write(cs, PIN_HIGH);
    }

    return 0;
}

const static struct rt_spi_ops spi_ops =
{
    .configure = spi_configure,
    .xfer = spi_xfer,
};

#ifdef BSP_USING_SPI0
    static struct rt_spi_bus spi0_bus;
#endif //BSP_USING_SPI0

#ifdef BSP_USING_SPI1
    static struct rt_spi_bus spi1_bus;
#endif //BSP_USING_SPI1

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

#ifdef BSP_USING_SPI0
    /*rt_pin_mode(IO_FLASH_WP, PIN_MODE_OUTPUT);
    rt_pin_write(IO_FLASH_WP, PIN_HIGH);
    rt_pin_mode(IO_FLASH_CS, PIN_MODE_OUTPUT);
    rt_pin_write(IO_FLASH_CS, PIN_HIGH);*/
    rt_pin_mode(IO_SPI0_SCK, PIN_MODE_OUTPUT);
    rt_pin_mode(IO_SPI0_MISO, PIN_MODE_INPUT);
    rt_pin_mode(IO_SPI0_MOSI, PIN_MODE_OUTPUT);
    HD_System_GPIO_AF(IO_DF_CLK_PORT, IO_DF_CLK_PIN, GPIO_AF_SPI0_CLK, PUPDR_OD);
    HD_System_GPIO_AF(IO_DF_SDI_PORT, IO_DF_SDI_PIN, GPIO_AF_SPI0_SDI, PUPDR_OD);
    HD_System_GPIO_AF(IO_DF_SDO_PORT, IO_DF_SDO_PIN, GPIO_AF_SPI0_SDO, PUPDR_OD);

    spi0_bus.parent.user_data = HT_SPI0;
    result = rt_spi_bus_register(&spi0_bus, SPI0_BUS_NAME, &spi_ops);
#endif

#ifdef BSP_USING_SPI1
    // todo
#endif

    return result;
}
INIT_BOARD_EXPORT(rt_hw_spi_init);

#endif /* #ifdef RT_USING_SPI */
