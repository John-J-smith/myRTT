/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-09-21     shijj        the first version
 */

#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#ifdef BSP_USING_EEPROM

#include <string.h>
#include <stdio.h>
#include "drv_gpio.h"

#ifndef ULOG_USING_SYSLOG
#define LOG_TAG              "EEPROM"
#define LOG_LVL              LOG_LVL_DBG
#include <ulog.h>
#else
#include <syslog.h>
#endif /* ULOG_USING_SYSLOG */

static struct rt_i2c_bus_device *eep_i2c_bus = RT_NULL;
#define I2C_BUS_NAME    	"i2c1"
#define EEPROM_DEVICE_NAME	"eeprom0"
#define EEPROM_I2C_ADDR 	0x50

static struct rt_device ee_dev;

static rt_size_t eeprom_write(rt_device_t dev, rt_off_t pos, const void *buf, rt_size_t size)
{
    rt_uint8_t buffer[3];
    rt_uint16_t addr = (rt_uint16_t)pos;

    // write protect disable
    rt_pin_mode(IO_EE_WP, PIN_MODE_OUTPUT);
    rt_pin_write(IO_EE_WP, PIN_LOW);

    for(rt_uint32_t i = 0; i < size; i++)
    {
        buffer[0] = (rt_uint8_t)(addr >> 8);
        buffer[1] = (rt_uint8_t)(addr);
        buffer[2] = *((rt_uint8_t *)buf + i);
        rt_i2c_master_send(eep_i2c_bus, EEPROM_I2C_ADDR, RT_I2C_WR, buffer, 3);
        rt_thread_mdelay(6);
        addr++;
    }

    // write protect enable
    rt_pin_write(IO_EE_WP, PIN_HIGH);
    rt_pin_mode(IO_EE_WP, PIN_MODE_INPUT);

    return size;
}

static rt_size_t eeprom_read(rt_device_t dev, rt_off_t pos, void *buf, rt_size_t size)
{
    rt_size_t read_size = 0;
    rt_uint16_t addr = (rt_uint16_t)pos;

    addr = ((addr & 0x00ff) << 8) | ((addr & 0xff00) >> 8);
    rt_i2c_master_send(eep_i2c_bus, EEPROM_I2C_ADDR, RT_I2C_WR, (rt_uint8_t *)&addr, 2);
    read_size = rt_i2c_master_recv(eep_i2c_bus, EEPROM_I2C_ADDR, RT_I2C_RD, buf, size);
    rt_thread_mdelay(6);

    return read_size;
}

rt_uint32_t eewr(uint8_t argc, char **argv)
{
    rt_device_t ee = NULL;
    rt_uint32_t addr, size;
    rt_uint8_t  *buf = NULL;

    if(argc == 3)
    {
        sscanf(argv[1], "%ul", &addr);
        if(addr > 0xffff)
        {
            rt_kprintf("max offset is 0xffff\n");
            return -1;
        }

        buf = (rt_uint8_t *)argv[2];
        size = strlen(argv[2]);
    }

    ee = rt_device_find(EEPROM_DEVICE_NAME);
    RT_ASSERT(ee);
    rt_device_open(ee, RT_DEVICE_OFLAG_RDWR);
    rt_device_write(ee, addr, buf, size);
    ulog_hexdump(LOG_TAG, 16, buf, size);

    rt_device_close(ee);

    return size;
}
//MSH_CMD_EXPORT(eewr, eeprom write usage: eerd [addr] [string]);

rt_uint32_t eerd(uint8_t argc, char **argv)
{
    rt_device_t ee = NULL;
    rt_uint32_t addr, size;
    rt_uint8_t  *buf = NULL;

    if(argc == 3)
    {
        sscanf(argv[1], "%ul", &addr);
        if(addr > 0xffff)
        {
            rt_kprintf("max offset is 0xffff\n");
            return -1;
        }

        sscanf(argv[2], "%ul", &size);
        if(size > 0xff)
        {
            rt_kprintf("max size is 255\n");
            return -1;
        }
    }

    ee = rt_device_find(EEPROM_DEVICE_NAME);
    RT_ASSERT(ee);

    buf = (rt_uint8_t *)rt_malloc(size);
    rt_device_open(ee, RT_DEVICE_OFLAG_RDWR);
    rt_device_read(ee, addr, buf, size);
    ulog_hexdump(LOG_TAG, 16, buf, size);
    rt_free(buf);

    rt_device_close(ee);

    return size;
}
//MSH_CMD_EXPORT(eerd, eeprom read usage: eerd [addr] [size]);

int eeprom_init(void)
{
    eep_i2c_bus = rt_i2c_bus_device_find(I2C_BUS_NAME);
    RT_ASSERT(eep_i2c_bus);

	ee_dev.read 	= eeprom_read;
	ee_dev.write	= eeprom_write;
	ee_dev.type 	= RT_Device_Class_Unknown;

	return rt_device_register(&ee_dev, EEPROM_DEVICE_NAME, RT_DEVICE_FLAG_RDWR);
}
INIT_DEVICE_EXPORT(eeprom_init);

#endif /* #ifdef BSP_USING_EEPROM */
