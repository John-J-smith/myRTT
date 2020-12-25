/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-05-17     armink       the first version
 */

#ifndef _FAL_CFG_H_
#define _FAL_CFG_H_

#include <rtconfig.h>
#include <board.h>

#define FLASH_ON_CHIP_NAME              "flash_onchip"
#define DATA_FLASH_DEV_NAME             "flash0"
#define EEPROM_DEV_NAME                 "eeprom0"

/* ===================== Flash device Configuration ========================= */
extern const struct fal_flash_dev ht6xxx_onchip_flash;
extern struct fal_flash_dev data_flash0;
extern struct fal_flash_dev eeprom0;

/* flash device table */
#define FAL_FLASH_DEV_TABLE                                         \
{                                                                   \
    &ht6xxx_onchip_flash,                                           \
    &data_flash0,                                                   \
    &eeprom0,                                                       \
}
/* ====================== Partition Configuration ========================== */
#ifdef FAL_PART_HAS_TABLE_CFG
/* partition table */
#define FAL_PART_TABLE                                                                  \
{                                                                                       \
    {FAL_PART_MAGIC_WORD,      "boot",  FLASH_ON_CHIP_NAME,         0,   64*1024, 0},   \
    {FAL_PART_MAGIC_WORD,       "app",  FLASH_ON_CHIP_NAME,   64*1024,  448*1024, 0},   \
    {FAL_PART_MAGIC_WORD,        "ef", DATA_FLASH_DEV_NAME,         0,   12*1024, 0},   \
    {FAL_PART_MAGIC_WORD,  "download", DATA_FLASH_DEV_NAME,   12*1024,  512*1024, 0},   \
    {FAL_PART_MAGIC_WORD,        "fs", DATA_FLASH_DEV_NAME,  524*1024, 1524*1024, 0},   \
    {FAL_PART_MAGIC_WORD,        "ee",     EEPROM_DEV_NAME,         0,   64*1024, 0},   \
}
#endif /* FAL_PART_HAS_TABLE_CFG */

#endif /* _FAL_CFG_H_ */
