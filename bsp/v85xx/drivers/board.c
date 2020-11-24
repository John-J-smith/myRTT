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
#include "target.h"
#include "drv_uart.h"

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

/**
* @brief  Clock_Init:
- PLLL input clock    : External 32K crystal
- PLLL frequency      : 26M
- AHB Clock source    : PLLL
- AHB Clock frequency : 26M (PLLL divided by 1)
- APB Clock frequency : 13M (AHB Clock divided by 2)
* @param  None
* @retval None
*/
void clock_init(void)
{
	CLK_InitTypeDef CLK_Struct;

	CLK_Struct.ClockType = CLK_TYPE_AHBSRC \
                          |CLK_TYPE_PLLL   \
                          |CLK_TYPE_HCLK   \
                          |CLK_TYPE_PCLK;
	CLK_Struct.AHBSource      = CLK_AHBSEL_LSPLL;
	CLK_Struct.PLLL.Frequency = CLK_PLLL_26_2144MHz;
	CLK_Struct.PLLL.Source    = CLK_PLLLSRC_XTALL;
	CLK_Struct.PLLL.State     = CLK_PLLL_ON;
	CLK_Struct.HCLK.Divider   = 1;
	CLK_Struct.PCLK.Divider   = 2;
	CLK_ClockConfig(&CLK_Struct);
}

void ANA_Config(void)
{
    // ANA->REGA = 0x60;  //没有这个配置，芯片时钟不准
    ANA->REGA = 0;
    //ANA->REGB = 0;      //低频RC调整
    //ANA->REGC = 0;      //高频RC调整
    // ANA->REGD = 0;    //LDO33 LDO15调整
    //ANA->REGE = 0x00;  //H1 REF
    // ANA->REGE = 0x07;  //H2
    ANA->REGF = 0;     //ADC REF相关配置
    ANA->REG7 = 0x00;          //输出ref nobat
    ANA->REG8 = 0x00; //LDO33不跟随
    ANA->REG9 = 0x0;    //POWALARM detector.
}

void rt_hw_board_init(void)
{
    ANA_Config();
	clock_init();

    SysTick_Config(26214400/1000);
    NVIC_SetPriority(SysTick_IRQn, 0x03);

#ifdef RT_USING_HEAP
    /* initialize system heap */
    rt_system_heap_init((void *)HEAP_BEGIN, (void *)HEAP_END);
#endif /* RT_USING_HEAP */

    rt_hw_pin_init();
	rt_hw_uart_init();

#ifdef RT_USING_CONSOLE
    rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
#endif /* RT_USING_CONSOLE */

#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif /* RT_USING_COMPONENTS_INIT */
}