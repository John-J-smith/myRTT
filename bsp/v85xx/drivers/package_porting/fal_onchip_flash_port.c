/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-10-22     John Shi     the first version
 */

#include <fal.h>

#include "board.h"

#define PAGE_SIZE 1024

/**
* @brief    Calculate the number of Falsh page.
* @inparam  Size: Length of file.
* @outparam None.
* @retval   The number of Falsh page.
**/
uint32_t FLASH_PagesMask(volatile uint32_t Size)
{
    uint32_t pagenumber = 0x0;
    uint32_t size = Size;

    if ((size % PAGE_SIZE) != 0)
    {
        pagenumber = (size / PAGE_SIZE) + 1;
    }
    else
    {
        pagenumber = size / PAGE_SIZE;
    }
    return pagenumber;
}

/**
* @brief  Unlock FLASH.
* @param  None
* @retval None
*/
void FLASH_Unlock(void)
{
    __disable_irq();
}

/**
* @brief  Lock FLASH.
* @param  None
* @retval None
*/
void FLASH_Lock(void)
{
    __enable_irq();
}

static int read(long offset, uint8_t *buf, size_t size)
{
    size_t i;
    uint32_t addr = onchip_flash.addr + offset;

    for (i = 0; i < size; i++, addr++, buf++)
    {
        *buf = *(uint8_t *) addr;
    }

    return size;
}

static int write(long offset, const uint8_t *buf, size_t size)
{
    size_t i;
    uint32_t read_data;
    uint32_t addr = onchip_flash.addr + offset;

    FLASH_Unlock();
    for (i = 0; i < size; i++, buf++, addr++)
    {
        /* write data */
        FLASH_ProgramByte(addr, (uint8_t *)buf, 1);
        read_data = *(uint8_t *) addr;
        /* check data */
        if (read_data != *buf)
        {
            return -1;
        }
    }
    FLASH_Lock();

    return size;
}

static int erase(long offset, size_t size)
{
    uint32_t addr = onchip_flash.addr + offset;
    uint32_t NbrOfPage = 0;

    /* start erase */
    FLASH_Unlock();
    NbrOfPage = FLASH_PagesMask(size);
    for(uint32_t i = 0; i < NbrOfPage; i++)
    {
        FLASH_SectorErase(addr + (PAGE_SIZE * i));
    }
    FLASH_Lock();

    return size;
}

const struct fal_flash_dev onchip_flash =
{
    .name       = "flash_onchip",
    .addr       = 0x00000000,
#if defined V8530
    .len        = 256*1024,
#elif defined V8510
    .len        = 128*1024,
#endif
    .blk_size   = 1024,
    .ops        = {NULL, read, write, erase},
    .write_gran = 1
};

