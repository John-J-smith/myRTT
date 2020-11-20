/**
  ******************************************************************************
  * @file    lib_CodeRAM.c 
  * @author  Application Team
  * @version V4.3.0
  * @date    2018-09-04
  * @brief   Codes executed in SRAM.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "lib_CodeRAM.h"

/**
  * @brief  Flash deep standby, enter idle mode.
  * @note   This function is executed in RAM.
  * @param  None
  * @retval None
  */
__RAM_FUNC void PMU_EnterIdle_FlashDSTB(void)
{
  /* Flash deep standby */
  FLASH->PASS = 0x55AAAA55;
  FLASH->DSTB = 0xAA5555AA;
  /* Enter Idle mode */
  SCB->SCR &= (uint32_t)~((uint32_t)SCB_SCR_SLEEPDEEP_Msk);
  __WFI();
}

/*********************************** END OF FILE ******************************/
