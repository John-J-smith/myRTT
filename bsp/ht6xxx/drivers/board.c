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

#ifdef RT_USING_FINSH
#include <finsh.h>
static void reboot(uint8_t argc, char **argv)
{
    NVIC_SystemReset();
}
FINSH_FUNCTION_EXPORT_ALIAS(reboot, __cmd_reboot, Reboot System);
#endif /* RT_USING_FINSH */

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

void SwitchTo_Fpll(uint8_t ucClk)
{
	if ((HT_CMU->SYSCLKCFG != 0x0003)
	|| !(HT_CMU->CLKCTRL0 & 0x0010)				//???PLL
	|| (HT_CMU->SYSCLKDIV != ucClk))
	{
		_OPEN_REG_WRITE;
		HT_CMU->CLKCTRL0 |= 0x0010;				//???PLL
		while(!(HT_CMU->CLKSTA & 0x0020));/*???PLL???????*/
		HT_CMU->SYSCLKDIV = ucClk;			    //Fcpu = Fsys/2 = Fpll/2 = 11.010048MHz

		//???? 44Mhz
		//HT_CMU->SYSCLKDIV |= (1 << 3); //PLL??????????
		//HT_CMU->PREFETCH |= CMU_PREFETCH_CACHE_EN;

		HT_CMU->SYSCLKCFG = 0x0083;				//Fsys = Fpll
		__NOP();
		HT_CMU->SYSCLKCFG = 0x0003;				//Fsys = Fpll
		_CLOSE_REG_WRITE;
	}
}

void rt_hw_board_init(void)
{
    SwitchTo_Fpll(SPD_MCU);

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