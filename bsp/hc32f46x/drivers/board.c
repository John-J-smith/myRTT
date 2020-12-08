/*
 * Copyright (c) Aug.2,2004  R&D Institute of Reallin Co., Ltd.
 *
 * watch dog driver
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-09-01     John Shi     first version
 */

#include "board.h"
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
    en_clk_sys_source_t     enSysClkSrc;
    stc_clk_sysclk_cfg_t    stcSysClkCfg;
    stc_clk_xtal_cfg_t      stcXtalCfg;
    stc_clk_mpll_cfg_t      stcMpllCfg;
    stc_clk_freq_t          stcClkFreq;
    stc_sram_config_t       stcSramConfig;

    /* Configure structure initialization */
    MEM_ZERO_STRUCT(enSysClkSrc);
    MEM_ZERO_STRUCT(stcSysClkCfg);
    MEM_ZERO_STRUCT(stcXtalCfg);
    MEM_ZERO_STRUCT(stcMpllCfg);

    /* Set bus clk div. */
    stcSysClkCfg.enHclkDiv = ClkSysclkDiv1;   // Max 168MHz
    stcSysClkCfg.enExclkDiv = ClkSysclkDiv2;  // Max 84MHz
    stcSysClkCfg.enPclk0Div = ClkSysclkDiv1;  // Max 168MHz
    stcSysClkCfg.enPclk1Div = ClkSysclkDiv4;  // Max 42MHz
    stcSysClkCfg.enPclk2Div = ClkSysclkDiv4;  // Max 42MHz
    stcSysClkCfg.enPclk3Div = ClkSysclkDiv4;  // Max 42MHz
    stcSysClkCfg.enPclk4Div = ClkSysclkDiv2;  // Max 84MHz
    CLK_SysClkConfig(&stcSysClkCfg);

    /* Switch system clock source to MPLL. */
    /* Use Xtal as MPLL source. */
    stcXtalCfg.enMode = ClkXtalModeOsc;
    stcXtalCfg.enDrv = ClkXtalTinyDrv;//ClkXtalLowDrv;根据外部晶振选择合适的范围
    stcXtalCfg.enFastStartup = Enable;
    CLK_XtalConfig(&stcXtalCfg);
    CLK_XtalCmd(Enable);   /* Select Xtal */
    /* Enable HRC. */
    //CLK_HrcCmd(Enable);   /* Select Hrc */
    /* Enable Xtal32. */
    //CLK_Xtal32Cmd(Enable);   /* Select Xtal32 */

    /* MPLL config (XTAL / pllmDiv * plln / PllpDiv = 168M). (XTAL / 1 * 52 / 2) */
    stcMpllCfg.pllmDiv = 1ul;   //hrc 8
    stcMpllCfg.plln = 52ul;    //hrc 168
    stcMpllCfg.PllpDiv = 2ul;   //hrc 2
    stcMpllCfg.PllqDiv = 2ul;
    stcMpllCfg.PllrDiv = 2ul;
    CLK_SetPllSource(ClkPllSrcXTAL);//(ClkPllSrcHRC);
    CLK_MpllConfig(&stcMpllCfg);

    /* flash read wait cycle setting */
    EFM_Unlock();
    EFM_SetLatency(EFM_LATENCY_5);
    EFM_Lock();

    /* sram init include read/write wait cycle setting */
    stcSramConfig.u8SramIdx = Sram12Idx | Sram3Idx | SramHsIdx | SramRetIdx;
    stcSramConfig.enSramRC = SramCycle2;
    stcSramConfig.enSramWC = SramCycle2;
    stcSramConfig.enSramEccMode = EccMode3;
    stcSramConfig.enSramEccOp = SramNmi;
    stcSramConfig.enSramPyOp = SramNmi;
    SRAM_Init(&stcSramConfig);

    /* Enable MPLL. */
    CLK_MpllCmd(Enable);

    /* Wait MPLL ready. */
    while(Set != CLK_GetFlagStatus(ClkFlagMPLLRdy))
    {
        ;
    }

    /* Switch system clock source to MPLL. */
    CLK_SetSysClkSource(CLKSysSrcMPLL);

    /* Check source and frequence. */
    enSysClkSrc = CLK_GetSysClkSource();
    CLK_GetClockFreq(&stcClkFreq);
}

void m_HD_GPIO_PinMode(en_port_t enPort, en_pin_t u16Pin, en_pin_mode_t pinMode, en_functional_state_t enNewState)
{
    stc_port_init_t stPortInit;
    /* configuration structure initialization */
    MEM_ZERO_STRUCT(stPortInit);

    stPortInit.enPinMode = pinMode;
    stPortInit.enPullUp = enNewState;

    PORT_Init(enPort, u16Pin, &stPortInit);
}

void rt_hw_board_init(void)
{
    //SCB->VTOR = 0xa000;
	clock_init();
    
    SysTick_Config(SystemCoreClock/1000);
    NVIC_SetPriority(SysTick_IRQn, 0xff);

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