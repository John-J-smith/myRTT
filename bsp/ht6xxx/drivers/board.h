/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-09-01     Shijj        the first version
 */

#ifndef __BOARD_H__
#define __BOARD_H__

#include <rtthread.h>
#include "ht6xxx.h"
#include "ht6xxx_lib.h"
   
typedef        unsigned char         BOOL;
typedef        signed char           S8;
typedef        unsigned char         U8;
typedef        short                 S16;
typedef        unsigned short        U16;
typedef        int                   S32;
typedef        unsigned int          U32;
typedef        unsigned long long    U64;
typedef        signed long long      S64;
typedef        float                 F32;

#include "drv_gpio.h"
#include "HD_System.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MIN(a, b)                       (((a) < (b))? (a) : (b))
#define MAX(a, b)                       (((a) > (b))? (a) : (b))
#define DIV_CEIL(x, y)                  (((x) + (y) - 1) / (y))
#define ARRAY_SIZE(a)                   (sizeof(a) / sizeof(a[0]))

#define HT6x3x_FLASH_START_ADRESS     ((uint32_t)0x00000000)
#define HT6x3x_FLASH_SIZE             (512 * 1024)
#define HT6x3x_FLASH_END_ADDRESS      ((uint32_t)(HT6x3x_FLASH_START_ADRESS + HT6x3x_FLASH_SIZE))

/* Internal SRAM memory size[Kbytes] <8-64>, Default: 64*/
#define HT6x3x_SRAM_SIZE      64
#define HT6x3x_SRAM_END       (0x20000000 + HT6x3x_SRAM_SIZE * 1024)

#if defined(__CC_ARM) || defined(__CLANG_ARM)
extern int Image$$RW_IRAM1$$ZI$$Limit;
#define HEAP_BEGIN      ((void *)&Image$$RW_IRAM1$$ZI$$Limit)
#elif __ICCARM__
#pragma section="CSTACK"
#define HEAP_BEGIN      (__segment_end("CSTACK"))
#else
extern int __bss_end;
#define HEAP_BEGIN      ((void *)&__bss_end)
#endif

#define HEAP_END        HT6x3x_SRAM_END


void rt_hw_board_init(void);

#ifdef __cplusplus
}
#endif

#endif /* __BOARD_H__ */
