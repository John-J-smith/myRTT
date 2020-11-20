/**
  ******************************************************************************
  * @file    lib_CodeRAM.h 
  * @author  Application Team
  * @version V4.3.0
  * @date    2018-09-04
  * @brief   Codes executed in SRAM.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

#ifndef __LIB_CODERAM_H
#define __LIB_CODERAM_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "target.h"   


#ifdef __TARGET_EWARM_USED /* EWARM */
  #define __RAM_FUNC 	__ramfunc
#else                      /* MDK-ARM */
  #define __RAM_FUNC	__attribute__((used))
#endif

/* Exported Functions ------------------------------------------------------- */

__RAM_FUNC void PMU_EnterIdle_FlashDSTB(void);

#ifdef __cplusplus
}
#endif

#endif /* __LIB_CODERAM_H */

/*********************************** END OF FILE ******************************/
