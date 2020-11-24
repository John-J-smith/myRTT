/*
 * Copyright (c) Aug.2,2004  R&D Institute of Reallin Co., Ltd.
 *
 * usart device driver
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-09-01     John Shi     first version
 */

#ifndef __BOARD_H__
#define __BOARD_H__

#include <rtthread.h>
#include "hc32f46x.h"
#include "hc32_ddl.h"

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

#ifdef __cplusplus
extern "C" {
#endif

#define DIV_CEIL(x, y)                  (((x) + (y) - 1) / (y))
#define ARRAY_SIZE(a)                   (sizeof(a) / sizeof(a[0]))

/* 编程单位4K，擦除单位8K */
#define ONCHIP_FLASH_SIZE               (512 * 1024)
#define ONCHIP_FLASH_START_ADRESS       ((uint32_t)0x00000000)
#define ONCHIP_FLASH_END_ADDRESS        ((uint32_t)(ONCHIP_FLASH_START_ADRESS + ONCHIP_FLASH_SIZE))

/*
 * SRAM MAP
 * SRAMH    32k     0x1FFF8000 ~ 0x1FFFFFFF
 * SRAM1    64K     0x20000000 ~ 0x2000FFFF
 * SRAM2    64K     0x20010000 ~ 0x2001FFFF
 * SRAM3    28K     0x20020000 ~ 0x20026FFF
 * Ret_SRAM 4K      0x200F0000 ~ 0x200F0FFF
 */
#define ONCHIP_SRAM_SIZE                188 //不包括Ret_SRAM (停电保持功能的ram，内存地址不连续)
#define ONCHIP_SRAM_END                 (0x1fff8000 + ONCHIP_SRAM_SIZE * 1024)


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

#define HEAP_END        ONCHIP_SRAM_END


void rt_hw_board_init(void);

#ifdef __cplusplus
}
#endif

#endif /* __BOARD_H__ */
