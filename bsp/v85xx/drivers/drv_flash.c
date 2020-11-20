/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-08-15     Ernest Chen  the first version
 */

#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#ifdef BSP_USING_FLASH

#include <string.h>
#include <stdio.h>
#include "drv_gpio.h"
#include "drv_spi.h"


#ifndef ULOG_USING_SYSLOG
#define LOG_TAG              "df0"
#define LOG_LVL              LOG_LVL_DBG
#include <ulog.h>
#else
#include <syslog.h>
#endif /* ULOG_USING_SYSLOG */

#include "spi_flash_sfud.h"

static struct rt_spi_device spi_dev_flash;

#ifdef BSP_USING_SPI1

#define SPI_BUS_NAME    	"spi1"
#define SPI_DEVICE_NAME 	"spi11"

#elif defined BSP_USING_SPI2

#define SPI_BUS_NAME    	"spi2"
#define SPI_DEVICE_NAME 	"spi21"

#endif

#define FLASH_DEVICE_NAME 	"flash0"

struct rt_spi_configuration cfg =
{
	.data_width = 8,
	.mode = RT_SPI_CPHA | RT_SPI_CPOL | RT_SPI_MASTER | RT_SPI_MODE_0 | RT_SPI_MSB,
	.max_hz = 20 * 1000 *1000, /* 20M,SPI max 42MHz,ssd1351 4-wirespi */
};

void spi_config(void)
{
	spi_dev_flash.config.data_width = cfg.data_width;
	spi_dev_flash.config.mode = cfg.mode;
	spi_dev_flash.config.max_hz = cfg.max_hz;
	rt_spi_configure(&spi_dev_flash, &cfg);
}

int flash_init(void)
{
    rt_err_t res;

	rt_pin_mode(IO_FLASH_CS, PIN_MODE_OUTPUT);
    rt_pin_write(IO_FLASH_CS, PIN_HIGH);
	rt_pin_mode(IO_FLASH_WP, PIN_MODE_OUTPUT);
    rt_pin_write(IO_FLASH_WP, PIN_LOW);
	res = rt_spi_bus_attach_device(&spi_dev_flash, SPI_DEVICE_NAME, SPI_BUS_NAME, (void *)IO_FLASH_CS);
	if (res != RT_EOK)
	{
		rt_kprintf("rt_spi_bus_attach_device!\r\n");
	}
	else
	{
		spi_config();
		rt_sfud_flash_probe(FLASH_DEVICE_NAME, SPI_DEVICE_NAME);
	}

    return res;
}
INIT_DEVICE_EXPORT(flash_init);

#endif /* #ifdef BSP_USING_FLASH */
