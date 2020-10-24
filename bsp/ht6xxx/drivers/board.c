/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-09-01     Shijj        the first version
 */

#include "board.h"
#include "ht6xxx.h"
#include "drv_uart.h"
#include "HD_System.h"

#ifdef RT_USING_FINSH
#include <finsh.h>
static void reboot(uint8_t argc, char **argv)
{
    NVIC_SystemReset();
}
FINSH_FUNCTION_EXPORT_ALIAS(reboot, __cmd_reboot, Reboot System);
#endif /* RT_USING_FINSH */


#pragma optimize = none
void Delay_us(volatile uint32_t n)
{
    while (n--)
    {
        __NOP();__NOP();__NOP();
        __NOP();__NOP();__NOP();
        __NOP();__NOP();__NOP();
        __NOP();__NOP();__NOP();
        __NOP();__NOP();__NOP();
        __NOP();__NOP();
    }
}

#pragma optimize = none
void Delay_ms(volatile uint32_t n)
{
    while (n--)
    {
        Delay_us(971);
    }
}

/**
 * This is the timer interrupt service routine.
 */
void SysTick_Handler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    rt_tick_increase();

    /* leave interrupt */
    rt_interrupt_leave();
}

static void clock_init(void)
{
    U8 ucCnt;
    U8 ucPowerStable;

    __disable_irq();

    HD_clrWDT();
    Delay_ms(100);
    HD_clrWDT();
    HD_System_RCC_Init();
    g_ulMCLK = HT_CMU_SysClkGet();
    HD_System_PVDConfig();
    HD_System_PMUConfig();
    HD_clrWDT();

    ucPowerStable = 0;
    for(ucCnt = 0; ucCnt < 20; ucCnt++)
    {
        Delay_ms(10);
        ucPowerStable <<= 1;
        if(PFAL_Read() != 0)
        {
            ucPowerStable++;
        }
        if(ucPowerStable == 0xff)//检测电源稳定，提前退出
        {
            break;
        }
        g_ulMCLK = HT_CMU_SysClkGet();
        HD_clrWDT();
    }
    SwitchTo_Fpll(SPD_MCU);
    g_ulMCLK = HT_CMU_SysClkGet();

    __enable_irq();
}

void rt_hw_board_init(void)
{
	clock_init();

    SysTick_Config(HT_CMU_SysClkGet()/1000);

#ifdef RT_USING_HEAP
    /* initialize system heap */
    rt_system_heap_init((void *)HEAP_BEGIN, (void *)HEAP_END);
#endif /* RT_USING_HEAP */

	rt_hw_uart_init();

#ifdef RT_USING_CONSOLE
    rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
#endif /* RT_USING_CONSOLE */

#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif /* RT_USING_COMPONENTS_INIT */
}