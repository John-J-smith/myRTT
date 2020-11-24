/*
 * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-10-27     John Shi     the first version
 */

#include <rtthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fal.h>


#define FAL_FS_PARTITION_NAME           "fs"
#define FAL_FS_PARTITION_DEVICE_NAME    "flashfs"
#define BLOCK_SIZE          4096
#define BYTES_PER_SECTOR    4096

/* RT-Thread device interface */
static rt_err_t partition_open(rt_device_t dev, rt_uint16_t oflag)
{
    return RT_EOK;
}

static rt_size_t partition_read(rt_device_t dev, rt_off_t pos, void* buffer, rt_size_t size)
{
    const struct fal_partition *fs_part = (struct fal_partition *)dev->user_data;
    RT_ASSERT(fs_part != RT_NULL);

    /* change the block device's logic address to physical address */
    rt_off_t phy_pos = pos * BYTES_PER_SECTOR;
    rt_size_t phy_size = size * BYTES_PER_SECTOR;

    if(fal_partition_read(fs_part, phy_pos, buffer, phy_size) > 0)
    {
        return size;
    }
    else
    {
        return 0;
    }
}

static rt_size_t partition_write(rt_device_t dev, rt_off_t pos, const void* buffer, rt_size_t size)
{
    const struct fal_partition *fs_part = (struct fal_partition *)dev->user_data;
    RT_ASSERT(fs_part != RT_NULL);
    int ret;

    /* change the block device's logic address to physical address */
    rt_off_t phy_pos = pos * BYTES_PER_SECTOR;
    rt_size_t phy_size = size * BYTES_PER_SECTOR;

    ret = fal_partition_erase(fs_part, phy_pos, phy_size);
    if (ret >= 0)
    {
        if(fal_partition_write(fs_part, phy_pos, buffer, phy_size) >= 0)
        {
            return size;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return ret;
    }
}

static rt_err_t partition_close(rt_device_t dev)
{
    return RT_EOK;
}

static rt_err_t partition_control(rt_device_t dev, int cmd, void *args)
{
    struct rt_device *flash_device;
    struct rt_device_blk_geometry flash_geometry;
    const struct fal_partition *fs_part = (struct fal_partition *)dev->user_data;
    RT_ASSERT(fs_part != RT_NULL);

    if (cmd == RT_DEVICE_CTRL_BLK_GETGEOME)
    {
        struct rt_device_blk_geometry *geometry;

        /* get flash block device geometry */
        flash_device = (struct rt_device *)rt_device_find(fs_part->flash_name);
        if(flash_device == RT_NULL)
        {
            return -RT_ENOSYS;
        }
        /* open the block device */
        rt_device_open(flash_device, RT_DEVICE_OFLAG_RDWR);
        /* get block device geometry */
        rt_device_control(flash_device, RT_DEVICE_CTRL_BLK_GETGEOME, &flash_geometry);


        geometry = (struct rt_device_blk_geometry *)args;
        if (geometry == RT_NULL)
        {
            return -RT_ERROR;
        }
        geometry->bytes_per_sector = flash_geometry.bytes_per_sector;
        geometry->sector_count = fs_part->len / flash_geometry.block_size;
        geometry->block_size = flash_geometry.block_size;
    }

    return RT_EOK;
}

int fs_partition_init(void)
{
    struct rt_device *device;
    const struct fal_partition *fs_part;

    fs_part = fal_partition_find(FAL_FS_PARTITION_NAME);
    if(fs_part == RT_NULL)
    {
        return -RT_ENOSYS;
    }

    /* register device */
    device = (struct rt_device *)rt_malloc (sizeof(struct rt_device));
    if(device == RT_NULL)
    {
        return -RT_ENOSYS;
    }

    device->type    = RT_Device_Class_Block;
    device->init    = RT_NULL;
    device->open    = partition_open;
    device->close   = partition_close;
    device->read    = partition_read;
    device->write   = partition_write;
    device->control = partition_control;
    device->user_data = (void *)fs_part;

    rt_device_register(device, FAL_FS_PARTITION_DEVICE_NAME, RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_STANDALONE);

    return RT_EOK;
}
