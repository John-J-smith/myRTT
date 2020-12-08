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
#define SPI3_BUS_NAME       "spi3"

/* SPI_SCK Port/Pin definition */
#define SPI_SCK_PORT                    (PortB)
#define SPI_SCK_PIN                     (Pin15)
#define SPI_SCK_FUNC                    (Func_Spi3_Sck)

/* SPI_MOSI Port/Pin definition */
#define SPI_MOSI_PORT                   (PortB)
#define SPI_MOSI_PIN                    (Pin14)
#define SPI_MOSI_FUNC                   (Func_Spi3_Mosi)

/* SPI_MISO Port/Pin definition */
#define SPI_MISO_PORT                   (PortB)
#define SPI_MISO_PIN                    (Pin12)
#define SPI_MISO_FUNC                   (Func_Spi3_Miso)

static rt_uint8_t spi_send_recv(M4_SPI_TypeDef *SPIx, rt_uint8_t ch)
{
    rt_uint8_t u8Byte;

    /* Wait tx buffer empty */
    while (Reset == SPI_GetFlag(SPIx, SpiFlagSendBufferEmpty))
    {
    }
    /* Send data */
    SPI_SendData8(SPIx, ch);
    /* Wait rx buffer full */
    while (Reset == SPI_GetFlag(SPIx, SpiFlagReceiveBufferFull))
    {
    }
    /* Receive data */
    u8Byte = SPI_ReceiveData8(SPIx);

    return u8Byte;
}

static rt_err_t spi_configure(struct rt_spi_device *device,
                              struct rt_spi_configuration *configuration)
{
    M4_SPI_TypeDef *SPIx = (M4_SPI_TypeDef *)device->bus->parent.user_data;
    stc_spi_init_t stcSpiInit;

    /* configuration structure initialization */
    MEM_ZERO_STRUCT(stcSpiInit);

    /* Configuration SPI structure */
    stcSpiInit.enClkDiv = SpiClkDiv16;
    stcSpiInit.enFrameNumber = SpiFrameNumber1;
    stcSpiInit.enDataLength = SpiDataLengthBit8;
    stcSpiInit.enFirstBitPosition = SpiFirstBitPositionMSB;
    stcSpiInit.enSckPolarity = SpiSckIdelLevelHigh;
    stcSpiInit.enSckPhase = SpiSckOddChangeEvenSample;
    stcSpiInit.enReadBufferObject = SpiReadReceiverBuffer;
    stcSpiInit.enWorkMode = SpiWorkMode3Line;
    stcSpiInit.enTransMode = SpiTransFullDuplex;
    stcSpiInit.enCommAutoSuspendEn = Disable;
    stcSpiInit.enModeFaultErrorDetectEn = Disable;
    stcSpiInit.enParitySelfDetectEn = Disable;
    stcSpiInit.enParityEn = Disable;
    stcSpiInit.enParity = SpiParityEven;
    stcSpiInit.enMasterSlaveMode = SpiModeMaster;
    stcSpiInit.stcDelayConfig.enSsSetupDelayOption = SpiSsSetupDelayTypicalSck1;
    stcSpiInit.stcDelayConfig.enSsSetupDelayTime = SpiSsSetupDelaySck1;
    stcSpiInit.stcDelayConfig.enSsHoldDelayOption = SpiSsHoldDelayTypicalSck1;
    stcSpiInit.stcDelayConfig.enSsHoldDelayTime = SpiSsHoldDelaySck1;
    stcSpiInit.stcDelayConfig.enSsIntervalTimeOption = SpiSsIntervalTypicalSck1PlusPck2;
    stcSpiInit.stcDelayConfig.enSsIntervalTime = SpiSsIntervalSck6PlusPck2;

    SPI_Init(SPIx, &stcSpiInit);
    SPI_Cmd(SPIx, Enable);

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

    M4_SPI_TypeDef *SPIx = (M4_SPI_TypeDef *)device->bus->parent.user_data;
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

#ifdef BSP_USING_SPI3
    static struct rt_spi_bus spi3_bus;
#endif /* BSP_USING_SPI3 */


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
    M4_SPI_TypeDef *SPIx;

    rt_pin_mode(IO_FLASH_CS, PIN_MODE_OUTPUT);

#ifdef BSP_USING_SPI1
    /* Configuration peripheral clock */
    PWC_Fcg1PeriphClockCmd(PWC_FCG1_PERIPH_SPI1, Enable);

    /* Configuration SPI pin */
    PORT_SetFunc(SPI_SCK_PORT, SPI_SCK_PIN, SPI_SCK_FUNC, Disable);
    PORT_SetFunc(SPI_MOSI_PORT, SPI_MOSI_PIN, SPI_MOSI_FUNC, Disable);
    PORT_SetFunc(SPI_MISO_PORT, SPI_MISO_PIN, SPI_MISO_FUNC, Disable);

    SPIx = M4_SPI1;
    spi1_bus.parent.user_data = SPIx;
    result = rt_spi_bus_register(&spi1_bus, SPI1_BUS_NAME, &spi_ops);
#endif /* BSP_USING_SPI1 */

#ifdef BSP_USING_SPI2
    /* Configuration peripheral clock */
    PWC_Fcg1PeriphClockCmd(PWC_FCG1_PERIPH_SPI2, Enable);

    /* Configuration SPI pin */
    PORT_SetFunc(SPI_SCK_PORT, SPI_SCK_PIN, SPI_SCK_FUNC, Disable);
    PORT_SetFunc(SPI_MOSI_PORT, SPI_MOSI_PIN, SPI_MOSI_FUNC, Disable);
    PORT_SetFunc(SPI_MISO_PORT, SPI_MISO_PIN, SPI_MISO_FUNC, Disable);

    SPIx = M4_SPI2;
    spi2_bus.parent.user_data = SPIx;
    result = rt_spi_bus_register(&spi2_bus, SPI2_BUS_NAME, &spi_ops);
#endif /* BSP_USING_SPI2 */

#ifdef BSP_USING_SPI3
    /* Configuration peripheral clock */
    PWC_Fcg1PeriphClockCmd(PWC_FCG1_PERIPH_SPI3, Enable);

    /* Configuration SPI pin */
    PORT_SetFunc(SPI_SCK_PORT, SPI_SCK_PIN, SPI_SCK_FUNC, Disable);
    PORT_SetFunc(SPI_MOSI_PORT, SPI_MOSI_PIN, SPI_MOSI_FUNC, Disable);
    PORT_SetFunc(SPI_MISO_PORT, SPI_MISO_PIN, SPI_MISO_FUNC, Disable);

    SPIx = M4_SPI3;
    spi3_bus.parent.user_data = SPIx;
    result = rt_spi_bus_register(&spi3_bus, SPI3_BUS_NAME, &spi_ops);
#endif /* BSP_USING_SPI3 */


    return result;
}
INIT_BOARD_EXPORT(rt_hw_spi_init);

#endif /* #ifdef RT_USING_SPI */
