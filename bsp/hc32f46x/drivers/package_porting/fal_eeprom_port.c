/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-01-26     armink       the first version
 */

#include <fal.h>
#include <sfud.h>

#ifdef FAL_USING_SFUD_PORT
#ifdef RT_USING_SFUD
#include <spi_flash_sfud.h>
#endif

#ifndef FAL_USING_EEPROM_DEV_NAME
#define FAL_USING_EEPROM_DEV_NAME             "eeprom0"
#endif

static int init(void);
static int read(long offset, uint8_t *buf, size_t size);
static int write(long offset, const uint8_t *buf, size_t size);
static int erase(long offset, size_t size);

static rt_device_t eeprom_dev = NULL;
struct fal_flash_dev eeprom0 =
{
    .name       = FAL_USING_EEPROM_DEV_NAME,
    .addr       = 0,
    .len        = 64 * 1024,
    .blk_size   = 128,
    .ops        = {init, read, write, erase},
    .write_gran = 1
};

static int init(void)
{
    eeprom_dev = rt_device_find(FAL_USING_EEPROM_DEV_NAME);
    if (NULL == eeprom_dev)
    {
        return -1;
    }
    else
    {
        rt_device_open(eeprom_dev, RT_DEVICE_OFLAG_RDWR);
    }

    return 0;
}

static int read(long offset, uint8_t *buf, size_t size)
{
    assert(eeprom_dev);

    rt_device_read(eeprom_dev, eeprom0.addr + offset, buf, size);

    return size;
}

static int write(long offset, const uint8_t *buf, size_t size)
{
    assert(eeprom_dev);

    if (rt_device_write(eeprom_dev, eeprom0.addr + offset, buf, size) != size)
    {
        return -1;
    }

    return size;
}

static int erase(long offset, size_t size)
{
    return size;
}
#endif /* FAL_USING_SFUD_PORT */

