/*
*********************************************************************************************************
*                                              HT6XXXX
*                                           Library Function
*
*                                   Copyright 2013, Hi-Trend Tech, Corp.
*                                        All Rights Reserved
*
*
* Project      : HT6xxx
* File         : ht6xxx_crc.c
* By           : Hitrendtech_SocTeam
* Version      : V1.0.0
* Description  : Only support HT6x2x and HT6x3x
*********************************************************************************************************
*/

#define  __HT6XXX_CRC_C

#include "ht6xxx_crc.h"

#if defined HT6x2x || defined HT6x3x    /* This File Only support HT6x2x and HT6x3x */

/*
*********************************************************************************************************
*                                           本地宏/结构体
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             本地变量
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           本地函数申明
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                     RESET CRC MODULE
*
* 函数说明: CRC方式选择
*
* 入口参数: mode 计算CRC的模式选择（输入不存在的CRC方式，就会选择CRC16 CCITT FALSE方式）
*                CRC_MODE_CRC16_CCITT_FALSE  CRC16 CCITT FALSE方式
*                CRC_MODE_CRC16_CCITT        CRC16 CCITT 方式
*                CRC_MODE_CRC16_X25          CRC16 X25方式
*                CRC_MODE_CRC16_XMODEM       CRC16 XMODEM方式
*                CRC_MODE_CRC16_IBM          CRC16 IBM方式
*                CRC_MODE_CRC16_MAXIM        CRC16 MAXIM方式
*                CRC_MODE_CRC16_USB          CRC16 USB方式
*                CRC_MODE_CRC16_MODBUS       CRC16 MODBUS方式
*                CRC_MODE_CRC32              CRC32 方式
*                CRC_MODE_CRC32_MPEG2        CRC32 MPEG2方式
*
* 返回参数: 无
*
* 特殊说明:
*********************************************************************************************************
*/
void HT_CRC_ModeSelect(CRC_ModeTypeDef mode)
{
    switch(mode)
    {
        case CRC_MODE_CRC16_CCITT_FALSE:
        default:
            HT_CRC->CRCINIT = 0xFFFF;
            HT_CRC->CRCCON  = (HT_CRC->CRCCON &(~CRC_CRCCON_MODE)) | CRC_CRCCON_MODE_CCITT;
            break;
        case CRC_MODE_CRC16_CCITT:
            HT_CRC->CRCINIT = 0x0000;
            HT_CRC->CRCCON  = (HT_CRC->CRCCON &(~CRC_CRCCON_MODE)) | CRC_CRCCON_RefIn | CRC_CRCCON_RefOut
                              | CRC_CRCCON_MODE_CCITT;
            break;
        case CRC_MODE_CRC16_X25:
            HT_CRC->CRCINIT = 0xFFFF;
            HT_CRC->CRCCON  = (HT_CRC->CRCCON &(~CRC_CRCCON_MODE)) | CRC_CRCCON_RefIn | CRC_CRCCON_RefOut
                              | CRC_CRCCON_XorOut | CRC_CRCCON_MODE_CCITT;
            break;
        case CRC_MODE_CRC16_XMODEM:
            HT_CRC->CRCINIT = 0x0000;
            HT_CRC->CRCCON  = (HT_CRC->CRCCON &(~CRC_CRCCON_MODE)) | CRC_CRCCON_MODE_CCITT;
            break;
        case CRC_MODE_CRC16_IBM:
            HT_CRC->CRCINIT = 0;
            HT_CRC->CRCCON  = (HT_CRC->CRCCON &(~CRC_CRCCON_MODE)) | CRC_CRCCON_RefIn | CRC_CRCCON_RefOut
                              | CRC_CRCCON_MODE_CRC16;
            break;
        case CRC_MODE_CRC16_MAXIM:
            HT_CRC->CRCINIT = 0x0000;
            HT_CRC->CRCCON  = (HT_CRC->CRCCON &(~CRC_CRCCON_MODE)) | CRC_CRCCON_RefIn | CRC_CRCCON_RefOut
                              | CRC_CRCCON_XorOut | CRC_CRCCON_MODE_CRC16;
            break;
        case CRC_MODE_CRC16_USB:
            HT_CRC->CRCINIT = 0xFFFF;
            HT_CRC->CRCCON  = (HT_CRC->CRCCON &(~CRC_CRCCON_MODE)) | CRC_CRCCON_RefIn | CRC_CRCCON_RefOut
                              | CRC_CRCCON_XorOut | CRC_CRCCON_MODE_CRC16;
            break;
        case CRC_MODE_CRC16_MODBUS:
            HT_CRC->CRCINIT = 0xFFFF;
            HT_CRC->CRCCON  = (HT_CRC->CRCCON &(~CRC_CRCCON_MODE)) | CRC_CRCCON_RefIn | CRC_CRCCON_RefOut
                              | CRC_CRCCON_MODE_CRC16;
            break;
        case CRC_MODE_CRC32:
            HT_CRC->CRCINIT = 0xFFFFFFFF;
            HT_CRC->CRCCON  = (HT_CRC->CRCCON &(~CRC_CRCCON_MODE)) | CRC_CRCCON_RefIn | CRC_CRCCON_RefOut
                              | CRC_CRCCON_XorOut | CRC_CRCCON_MODE_CRC32;
            break;
        case CRC_MODE_CRC32_MPEG2:
            HT_CRC->CRCINIT = 0xFFFFFFFF;
            HT_CRC->CRCCON  = (HT_CRC->CRCCON &(~CRC_CRCCON_MODE)) | CRC_CRCCON_MODE_CRC32;
            break;
    }
}

/*
*********************************************************************************************************
*                                    CALCULATE CRC MODULE
*
* 函数说明: 计算CRC模块
*
* 入口参数: buf_8        数据缓存区指针地址（必须是uint8_t*）
*           len          数据缓存区有效长度
*           buf_byte_len 以bit为最小单位（输入不合法的计算长度，都按照“8”bits计算）
*                        CRC_CALC_8_BIT  每次参与运算的数据长度有“8”bits
*                        CRC_CALC_16_BIT 每次参与运算的数据长度有“16”bits
*                        CRC_CALC_32_BIT 每次参与运算的数据长度有“32”bits
*
* 返回参数: CRC结果(CRC正在使用时中断再调用返回值为“-1”)
*
* 特殊说明:
*********************************************************************************************************
*/
uint32_t HT_CRC_DataCalc(uint8_t* buf_8, uint32_t len, CRC_Calc_BitTypeDef buf_byte_len)
{
    static uint8_t crc_using = 0;
    uint32_t i, ret;
    uint16_t* buf_16 = (uint16_t*)buf_8;
    uint32_t* buf_32 = (uint32_t*)buf_8;

    if (crc_using == 0)                                    /*!< 防止CRC正在使用中时触发中断再次使用 */
    {
        crc_using = 1;
        HT_CRC->CRCCON |= CRC_CRCCON_RESET;                /*!< 复位CRC（不包含模式） */
        for (i=0; (i*buf_byte_len)<len; i++)               /*!< 循环将数据导入寄存器并计算结果 */
        {
            switch (buf_byte_len)
            {
                case CRC_CALC_8_BIT:
                default:
                HT_CRC->CRCDAT_BYTE = *(buf_8+i);
                break;
                case CRC_CALC_16_BIT:
                HT_CRC->CRCDAT_WORD = *(buf_16+i);
                break;
                case CRC_CALC_32_BIT:
                HT_CRC->CRCDAT_DWORD = *(buf_32+i);
                break;
            }
        }
        crc_using = 0;

        ret = HT_CRC->CRCDAT;
    }
    else 
    {
        ret = (uint32_t)(-1);
    }

    return ret;
}

#endif                                 /* This File Only support HT6x2x and HT6x3x  */
