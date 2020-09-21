/*
*********************************************************************************************************
*                                              HT6XXX
*                                          Library Function
*
*                                   Copyright 2013, Hi-Trend Tech, Corp.
*                                        All Rights Reserved
*
*
* Project      : HT6xxx
* File         : ht6xxx_crc.h
* By           : Hitrendtech_SocTeam
* Version      : V1.0.0
* Description  : Only support HT6x2x and HT6x3x
*********************************************************************************************************
*/

#ifndef __HT6XXX_CRC_H__
#define __HT6XXX_CRC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ht6xxx.h"

#if defined HT6x2x || defined HT6x3x    /* This File Only support HT6x2x and HT6x3x */

/*
*********************************************************************************************************
*                                           全局宏/结构体
*********************************************************************************************************
*/
/* CRC功能类型 */
typedef enum {
    CRC_MODE_CRC16_CCITT_FALSE = 1,             /*!< CRC16 CCITT-FALSE方式           */
    CRC_MODE_CRC16_CCITT       = 2,             /*!< CRC16 CCITT 方式                */
    CRC_MODE_CRC16_X25         = 3,             /*!< CRC16 X25 方式                  */
    CRC_MODE_CRC16_XMODEM      = 4,             /*!< CRC16 XMODEM 方式               */
    CRC_MODE_CRC16_IBM         = 5,             /*!< CRC16 IBM 方式                  */
    CRC_MODE_CRC16_MAXIM       = 6,             /*!< CRC16 MAXIM 方式                */
    CRC_MODE_CRC16_USB         = 7,             /*!< CRC16 USB 方式                  */
    CRC_MODE_CRC16_MODBUS      = 8,             /*!< CRC16 MODBUS 方式               */
    CRC_MODE_CRC32             = 9,             /*!< CRC32 方式                      */
    CRC_MODE_CRC32_MPEG2       = 10             /*!< CRC16 MPEG-2 方式               */
} CRC_ModeTypeDef;

/* CRC每次计算的BIT数 */
typedef enum {
    CRC_CALC_8_BIT  = 1,                        /*!< 每次CRC计算填入数据长度为8bit    */
    CRC_CALC_16_BIT = 2,                        /*!< 每次CRC计算填入数据长度为16bit   */
    CRC_CALC_32_BIT = 4                         /*!< 每次CRC计算填入数据长度为32bit   */
} CRC_Calc_BitTypeDef;


/*
*********************************************************************************************************
*                                             全局变量
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           全局函数申明
*********************************************************************************************************
*/
void HT_CRC_ModeSelect(CRC_ModeTypeDef mode);
uint32_t HT_CRC_DataCalc(uint8_t* buf_8, uint32_t len, CRC_Calc_BitTypeDef buf_byte_len);


#endif                                 /* This File Only support HT6x2x and HT6x3x  */

#ifdef __cplusplus
}
#endif

#endif /* __HT6XXX_CRC_H__ */
