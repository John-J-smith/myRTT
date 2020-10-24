/******************************************************************************
 Copyright (C) Aug.2，2004  R&D Institute of Reallin Co., Ltd.
 Module         : 系统其他外设配置模块
 Description    :
 Build Date     :
 others         :
 ------------------------------------------------------------------------------
 Modification History:
 <No.> <version >  <time>        <author>     <contents>
  2、
  1、    1.00     2015-01-05    yaoc     build this moudle     
******************************************************************************/
#define __HD_SYSTEM_C__
#include "board.h"
#include "HD_System.h"

//wait to add......
/*- 2.Macro-----------------------------------------------------*/


/*- 3.Private variable------------------------------------------*/


/*- 4.Public variable-------------------------------------------*/
U32 g_ulMCLK;/*系统时钟频率*/


/*- 5.Private function------------------------------------------*/


/*- 6.Const-----------------------------------------------------*/


/*- 7.End-------------------------------------------------------*/

/*******************************************************************************
 功能描述：  切换到LRC时钟
 输入参数：
 返回参数：
 函数说明：  32KHz
 *******************************************************************************/
/*
void SwitchTo_Flrc(void)
{
	if ((HT_CMU->SYSCLKCFG != 0x0000)
	 || (HT_CMU->LRCADJ    != 0x00B5)
	 || (HT_CMU->SYSCLKDIV != 0x0000))
	{
		_OPEN_REG_WRITE;
		HT_CMU->LRCADJ    = 0x00B5;			 //输出32768Hz
		HT_CMU->SYSCLKDIV = 0x0000;			 //Fcpu = Fsys
 // 	 Delay_mSec(2);
		HT_CMU->SYSCLKCFG = 0x0080;			 //Fsys = Flrc
		__NOP();
		HT_CMU->SYSCLKCFG = 0x0000;			 //Fsys = Flrc
		 
		HT_CMU->CLKCTRL0 &= ~0x0030;			 //关闭PLL,HRC
		_CLOSE_REG_WRITE;
	}
}*/
/*******************************************************************************
功能描述：	切换到HRC时钟
输入参数：
返回参数：
函数说明：	11MHz
			开启时需要延时500us
*******************************************************************************/
void SwitchTo_Fhrc(void)
{
	U32	adj0, adj1;
	
	adj0 = *(U32*)0x40140;
	adj1 = *(U32*)0x40144;
	adj1 = ~ adj1;

	if (adj0 != adj1)			//未校准
	{
		adj0 = 0x0040;
	}
	adj0 &= 0x007F;
	
	if ((HT_CMU->SYSCLKCFG != 0x0002)
	|| (HT_CMU->HRCADJ    != adj0)
	|| (HT_CMU->HRCDIV    != 0x0000)
	|| !(HT_CMU->CLKCTRL0 & 0x0020)
#if	(SPD_MCU == SPD_22000K)
	|| (HT_CMU->SYSCLKDIV != 0x0000))
#else
	|| (HT_CMU->SYSCLKDIV != SPD_MCU-1))
#endif
	{
		_OPEN_REG_WRITE;
		HT_CMU->HRCADJ    = adj0;				//输出11MHz
		HT_CMU->HRCDIV    = 0x0000;				//Fhrc' = Fhrc = 11MHz
		HT_CMU->CLKCTRL0 |= 0x0020;				//使能HRC
#if	(SPD_MCU == SPD_22000K)
		HT_CMU->SYSCLKDIV = 0x0000;				//Fcpu  = Fsys = 11MHz
#else
		HT_CMU->SYSCLKDIV = SPD_MCU-1;			//Fcpu  = Fsys = 11MHz
#endif
		HT_CMU->SYSCLKCFG = 0x0082;				//Fsys = Fhrc'
		__NOP();
		HT_CMU->SYSCLKCFG = 0x0002;				//Fsys = Fhrc'
		_CLOSE_REG_WRITE;
	}
}
/*******************************************************************************
功能描述：	切换到PLL时钟
输入参数：
返回参数：
函数说明：	22MHz
			开启时需要延时2ms,如之前没有开启Flf(Fosc)则需延时500ms
*******************************************************************************/
void SwitchTo_Fpll(U8 ucClk)
{
	if ((HT_CMU->SYSCLKCFG != 0x0003)
	|| !(HT_CMU->CLKCTRL0 & 0x0010)				//使能PLL
	|| (HT_CMU->SYSCLKDIV != ucClk))
	{
		_OPEN_REG_WRITE;
		HT_CMU->CLKCTRL0 |= 0x0010;				//使能PLL
		while(!(HT_CMU->CLKSTA & 0x0020));/*等待PLL工作稳定*/
		HT_CMU->SYSCLKDIV = ucClk;			    //Fcpu = Fsys/2 = Fpll/2 = 11.010048MHz
		HT_CMU->SYSCLKCFG = 0x0083;				//Fsys = Fpll
		__NOP();
		HT_CMU->SYSCLKCFG = 0x0003;				//Fsys = Fpll
		_CLOSE_REG_WRITE;
	}
}
/*****************************************************************************
Description:
Input:
Output:
Return:
******************************************************************************/
void HD_CfgClk_CloseHRC(void)
{

}

/*****************************************************************************
Description:
Input:
Output:
Return:
******************************************************************************/
void HD_CfgClk_RunOSC(void)
{

}

/*****************************************************************************
Description:
Input:
Output:
Return:
******************************************************************************/
void HD_CfgClk_ClosePLL(void)
{

}

/*****************************************************************************
Description:
Input:
Output:
Return:
******************************************************************************/
void HD_CfgClk_RunHRC(void)
{

}

/*****************************************************************************
Description:
Input:
Output:
Return:
******************************************************************************/
void HD_System_PVDConfig(void)
{
    _OPEN_REG_WRITE;
    HT_PMU->PMUCON = 0x0007; //开启LVDIN0_DET模块,开启BOR_DET模块
    HD_System_GPIO_AF(HT_GPIOE, Pin_7, GPIO_AF_LVDIN0, PUPDR_OD);
}

/*****************************************************************************
Description:复位后重新配置下没有配置的PMU寄存器
Input:
Output:
Return:
******************************************************************************/
void HD_System_PMUConfig(void)
{
    HT_PMU->VDETCFG = 0x000D;//VSYS_LVL   3.0V, BOR_LVL   2.2V
    HT_PMU->VDETPCFG = 0x003C;/*Hold,Sleep模式下,VSYS_DET,BOR_DET,LVDIN_DET分时检测时间设定:时间560us,周期268ms*/
    HT_PMU->WAKEIF = 0x0000;
    HT_PMU->PMUIE = 0x0000;
    HT_PMU->PMUIF = 0x0000;
}
/*****************************************************************************
Description:
Input:
Output:
Return:
******************************************************************************/
void HD_System_PVD_INTCfg(U8 ucEnable)
{
	if(ucEnable == TRUE)
    {
        HT_PMU->PMUIE |= 0x08; 
    }
    else
    {
        HT_PMU->PMUIE &= ~0x08; 
    }
}

/*****************************************************************************
Description:
Input:
Output:
Return:
******************************************************************************/
U8 HD_System_IsPVDIntStatus(void)
{
 	if(HT_PMU->PMUIF & 0x08)
    {
        return 1;
    }
    else
    {
        return 0;
    }   
}

/*********************************************************************************  
Function name: void HD_System_RCC_Init(void)
Description:   驱动程序
Input:  
Return:  
Notes: 有电模式下时钟初始化
Modification History:
<No.> <version >  <time>        <author>     <contents>

**********************************************************************************/
void HD_System_RCC_Init(void)
{
	/*Bit15-EMU-colse,  Bit14-AES-colse,    Bit13-1P5LBOR-colse, Bit12-CLKOUT-colse,
    Bit11-KEY-colse,  Bit10-OSC-enable,   Bit9-HRC_DET-colse,  Bit8-PLL_DET-colse,
    Bit7-LF_DET-colse,Bit6-PLL_LOCK-colse,Bit5-HRC_EN-enable,  Bit4-PLL_EN-colse,
    Bit3-I2C-colse,   Bit2-SPI-colse,     Bit1-LCD-colse,      Bit0-XXX-colse*/
    _OPEN_REG_WRITE;//Boot部分已处理。
    HT_CMU->CLKCTRL0 = 0x0420;
    /*各个外设时钟配置:UART,Timer,RTC..*/
    _OPEN_REG_WRITE;
    HT_CMU->CLKCTRL1 = 0x0000;
    /*默认值*/
    _OPEN_REG_WRITE;
    HT_CMU->LRCADJ = 0x0009;
    /*默认值*/
    _OPEN_REG_WRITE;
    HT_CMU->HRCADJ = 0x0040;

	//SwitchTo_Flrc();
}
/*********************************************************************************  
Function name: void System_RCC_PD_Init(void)
Description:   驱动程序
Input:  
Return:  
Notes: 停电模式下时钟初始化
Modification History:
<No.> <version >  <time>        <author>     <contents>
    1、    1.00     2014-02-10   yaoc             Creat
**********************************************************************************/
void HD_System_RCC_PD_Init(void)
{
	HT_PMU->PMUIE = 0x01;
    HT_PMU->PMUIF = 0;
    HT_PMU->VDETPCFG = 0x1C;//BOR检测周期268ms,检测时间560us.
    /*系统配置 */
    _OPEN_REG_WRITE;
    HT_CMU->SYSCLKCFG = 0x81;              //OSC as Fsys 
    HT_CMU->SYSCLKDIV = 0x00; 	 
    HD_clrWDT();
    _OPEN_REG_WRITE;
    HT_CMU->CLKCTRL0 = 0xA400;             //close HRC...
    //HT_PMU->PMUCON = 0x0002;
    _OPEN_REG_WRITE;
    HT_CMU->CLKCTRL1 = 0;
    //TPS config-----------------------------------------
    //HT_TBS->TBSCON = 0x0101;//Chop都打开  2次平均输出   osr=64  
    //HT_TBS->TBSIE = 0x00;
    //HT_TBS->TBSPRD = 0x00;//1s打开1次
    HT_TBS->TBSCON = 0x6541;//温度测量打开，reserved数值根据手册要求保持默认值
    HT_TBS->TBSIE = 0x00;
    HT_TBS->TBSPRD = 0x04;//8s打开1次
    //关闭低频RC以关闭看门狗
    //_OPEN_REG_WRITE;
    //HT_RTC->CTRLBYFLASH&=0xfffd;
    //HT_RTC->LRCCOMAND=0x5555;
    //HT_RTC->LRCCOMAND=0xAAAA;
}
/*********************************************************************************  
Function name: void HD_System_NVIC_Init(void)
Description:   驱动程序
Input:  
Return:  
Notes: 有电模式下中断初始化
Modification History:
<No.> <version >  <time>        <author>     <contents>
    1、    1.00     2014-02-10   yaoc             Creat
**********************************************************************************/
void HD_System_NVIC_Init(void)
{
    /*Timer1 中断*/
    NVIC_SetPriority(TIMER_1_IRQn, 0x00);
    NVIC_EnableIRQ(TIMER_1_IRQn);
    /*Timer3 中断*/
    NVIC_SetPriority(TIMER_3_IRQn, 0x01);
    NVIC_EnableIRQ(TIMER_3_IRQn);
	/*Uart 0 中断*/
    NVIC_SetPriority(UART0_IRQn, 0x02);  
    NVIC_EnableIRQ(UART0_IRQn);
	/*Uart 1 中断*/
    NVIC_SetPriority(UART1_IRQn, 0x02);  
    NVIC_EnableIRQ(UART1_IRQn);
	/*Uart 2 中断 PLC通道 115200波特率*/
    NVIC_SetPriority(UART2_IRQn, 0x00);  
    NVIC_EnableIRQ(UART2_IRQn);
	/*Uart 3 中断*/
    NVIC_SetPriority(UART3_IRQn, 0x02);  
    NVIC_EnableIRQ(UART3_IRQn);
	/*Uart 4 中断*/
    NVIC_SetPriority(UART4_IRQn, 0x02);  
    NVIC_EnableIRQ(UART4_IRQn);
    /*Uart 5 中断*/
    NVIC_SetPriority(UART5_IRQn, 0x02);  
    NVIC_EnableIRQ(UART5_IRQn);  
	/*Uart 6 中断*/
    NVIC_SetPriority(UART6_IRQn, 0x02);  
    NVIC_EnableIRQ(UART6_IRQn); 
    /*RTC 中断*/  
    NVIC_SetPriority(RTC_IRQn, 0x03);    
    NVIC_EnableIRQ(RTC_IRQn); 	
    /*INT 0 */
    NVIC_SetPriority(EXTI0_IRQn, 0x03);      
    NVIC_EnableIRQ(EXTI0_IRQn);
	/*INT 2 */
    NVIC_SetPriority(EXTI2_IRQn, 0x03);      
    NVIC_DisableIRQ(EXTI2_IRQn);
	/*INT 3 */
    NVIC_SetPriority(EXTI3_IRQn, 0x03);      
    NVIC_DisableIRQ(EXTI3_IRQn);
	/*INT 8 */
   	NVIC_SetPriority(EXTI8_IRQn, 0x03);      
   	NVIC_EnableIRQ(EXTI8_IRQn);
   	/*INT 9 */
   	NVIC_SetPriority(EXTI9_IRQn, 0x03);      
   	NVIC_EnableIRQ(EXTI9_IRQn);
}
/*********************************************************************************  
Function name: void HD_System_NVIC_PD_Init(void)
Description:   驱动程序
Input:  
Return:  
Notes: 停电模式下中断初始化
Modification History:
<No.> <version >  <time>        <author>     <contents>
    1、    1.00     2014-02-10   yaoc             Creat
**********************************************************************************/

void HD_System_NVIC_PD_Init(void)
{
	/*Timer1 中断*/
    NVIC_SetPriority(TIMER_1_IRQn, 0x00);
    NVIC_DisableIRQ(TIMER_1_IRQn);
	/*Timer2 中断*/
    NVIC_SetPriority(TIMER_2_IRQn, 0x00);
    NVIC_DisableIRQ(TIMER_2_IRQn);
    /*Timer3 中断*/
    NVIC_SetPriority(TIMER_3_IRQn, 0x01);
    NVIC_DisableIRQ(TIMER_3_IRQn);
	/*Uart 0 中断*/
    NVIC_SetPriority(UART0_IRQn, 0x02);  
    NVIC_DisableIRQ(UART0_IRQn);
	/*Uart 1 中断*/
    NVIC_SetPriority(UART1_IRQn, 0x02);  
    NVIC_DisableIRQ(UART1_IRQn);
	/*Uart 2 中断 PLC通道 115200波特率*/
    NVIC_SetPriority(UART2_IRQn, 0x00);  
    NVIC_DisableIRQ(UART2_IRQn);
	/*Uart 3 中断*/
    NVIC_SetPriority(UART3_IRQn, 0x02);  
    NVIC_DisableIRQ(UART3_IRQn);
	/*Uart 4 中断*/
    NVIC_SetPriority(UART4_IRQn, 0x02);  
    NVIC_DisableIRQ(UART4_IRQn);
    /*Uart 5 中断*/
    NVIC_SetPriority(UART5_IRQn, 0x02);  
    NVIC_DisableIRQ(UART5_IRQn);  
	/*Uart 6 中断*/
    NVIC_SetPriority(UART6_IRQn, 0x02);  
    NVIC_DisableIRQ(UART6_IRQn); 
    /*RTC 中断*/  
    NVIC_SetPriority(RTC_IRQn, 0x03);    
    NVIC_EnableIRQ(RTC_IRQn); 	
    /*INT 0 */
    NVIC_SetPriority(EXTI0_IRQn, 0x03);      
    NVIC_DisableIRQ(EXTI0_IRQn);
	/*INT 2 */
    NVIC_SetPriority(EXTI2_IRQn, 0x03);      
    NVIC_EnableIRQ(EXTI2_IRQn);
	/*INT 3 */
    NVIC_SetPriority(EXTI3_IRQn, 0x03);      
    NVIC_EnableIRQ(EXTI3_IRQn);
	/*INT 8 */
   	NVIC_SetPriority(EXTI8_IRQn, 0x03);      
   	NVIC_DisableIRQ(EXTI8_IRQn);
   	/*INT 9 */
   	NVIC_SetPriority(EXTI9_IRQn, 0x03);      
   	NVIC_DisableIRQ(EXTI9_IRQn);
}

/*********************************************************************************  
Function name:  void HD_System_ADC_Init(void)
Description:   驱动程序
Input:  
Return:  
Notes: 外部中断使能
Modification History:
<No.> <version >  <time>        <author>     <contents>
    1、    1.00     2014-02-10   yaoc             Creat
**********************************************************************************/

void HD_System_ADC_Init(void)
{
    /* Enable CLOCK */
    _OPEN_REG_WRITE;
    HT_TBS->TBSCON |= 0x23;  /*使用PLL 16分频时钟源(支持快速切换)打开温度检测 VBAT 电池电压检测*/
}
/*********************************************************************************  
  Function name:  void HD_CfgCLK_FOSC_32K( void )
  Description:驱动模块
  Input:   
  Return:  
  Notes:   设置硬件时钟为32k ,使用OSC 时钟,MCLK = 32.768KHZ 
  Modification History:
  <No.> <version >  <time>        <author>     <contents>
   1、    1.00     2014-02-20       yaoc        Creat
**********************************************************************************/
void HD_CfgCLK_FOSC_32K(U8 ucLcdState)
{
    /* Enable LSE*/
    _OPEN_REG_WRITE;
    HT_CMU->SYSCLKCFG = 0x81;  /*选择OSC*/
    HT_CMU->SYSCLKDIV = 0x00;  
    _OPEN_REG_WRITE;
    HT_CMU->CLKCTRL0 &= ~PLL_EN;/*PLL 时钟不使能*/	
    _OPEN_REG_WRITE;
    HT_CMU->CLKCTRL0 &= ~LCD_EN;/*PLL 时钟使能*/
}

/*********************************************************************************  
  Function name:  void HD_CfgCLK_MSI_614K( void )
  Description:驱动模块
  Input:   
  Return:  
  Notes:   设置硬件时钟为614k ,使用PLL 时钟,MCLK = 1/32PLL 
  Modification History:
  <No.> <version >  <time>        <author>     <contents>
   1、    1.00     2014-02-20       yaoc        Creat
**********************************************************************************/
void HD_CfgCLK_PLL_614K(void)
{

}
/*********************************************************************************  
   Function name:  void HD_CfgCLK_PLL_1M376( void )
   Description:驱动模块
   Input:	
   Return:	
   Notes:	设置硬件时钟为1.376M ,使用PLL 时钟,,MCLK = 1/16PLL 
   Modification History:
   <No.> <version >  <time> 	   <author> 	<contents>
	1、    1.00 	2014-02-20		 yaoc		 Creat
**********************************************************************************/
void HD_CfgCLK_PLL_1M376(void)
{
	/* Enable HSE */
	_OPEN_REG_WRITE;
	HT_CMU->CLKCTRL0 |= PLL_EN;/*PLL 时钟使能*/
	while(!(HT_CMU->CLKSTA & 0x0020));/*等待PLL工作稳定*/
	HT_CMU->SYSCLKCFG = 0x83;	/*选择PLL*/
	HT_CMU->SYSCLKDIV = 0x04;	/*PLL 输出1.376MHZ,MCLK = PLL/16*/
	//g_ulMCLK = 1376256;
 
	_OPEN_REG_WRITE;
	HT_CMU->CLKCTRL0 &= 0xFFDF;			//close HRC
}

 /*********************************************************************************  
   Function name:  void HD_CfgCLK_PLL_22M( void )
   Description:驱动模块
   Input:   
   Return:  
   Notes:   设置硬件时钟为22M
   Modification History:
   <No.> <version >  <time>        <author>     <contents>
    1、    1.00     2014-02-20       yaoc        Creat
 **********************************************************************************/
void HD_CfgCLK_PLL_22M(void)
{
    //不使用hrc，使用内部的PLL时钟
    //SwitchTo_Fhrc();
    SwitchTo_Fpll(SPD_MCU);
}

 
