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
* File         : ht501x_emu.c
* By           : Hitrendtech_SocTeam
* Version      : V1.0.1
* Description  : Only support HT501x and HT502x
*********************************************************************************************************
*/

#define  __HT50XX_EMU_C

#include "ht50xx_emu.h"
#include "Ht6xxx_it.h"
#include <string.h>
#include <math.h>

#if defined  HT501x  ||  defined  HT502x                         /* This File Only support HT501x */

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
static const uint32_t RegisterWriteProtect[]={CMU_WPREG_Protected, CMU_WPREG_UnProtected};

/*
*********************************************************************************************************
*                                           本地函数申明
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                       ENABLE EMU MODULE
*
* 函数说明: 使能EMU模块
*
* 入口参数: 无
*
* 返回参数: 无
*
* 特殊说明: 无
*********************************************************************************************************
*/
void Enable_emu_module(void)
{
    /*  assert_param  */
    uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];    /*!< 保存当前写保护状态     */

    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< 关闭写保护功能         */

    HT_CMU->CLKCTRL0 |= CMU_CLKCTRL0_EMUEN;                                /*!< 开启EMU模块时钟        */

    HT_CMU->WPREG = writeProtect;                                          /*!< 恢复之前写保护设置     */
}

/*
*********************************************************************************************************
*                                       DISABLE EMU MODULE
*
* 函数说明: 关闭EMU模块
*
* 入口参数: 无
*
* 返回参数: 无
*
* 特殊说明: 无
*********************************************************************************************************
*/
void Disable_emu_module(void)
{
    /*  assert_param  */
    uint32_t writeProtect = RegisterWriteProtect[HT_CMU->WPREG & 0x01];    /*!< 保存当前写保护状态     */

    HT_CMU->WPREG = CMU_WPREG_UnProtected;                                 /*!< 关闭写保护功能         */

    HT_CMU->CLKCTRL0 &=~CMU_CLKCTRL0_EMUEN;                                /*!< 关闭EMU模块时钟        */

    HT_CMU->WPREG = writeProtect;                                          /*!< 恢复之前写保护设置     */
}

/*
*********************************************************************************************************
*                                       OPEN EMU LDO
*
* 函数说明: 使能EMU LDO
*
* 入口参数: 无
*
* 返回参数: 无
*
* 特殊说明: 无
*********************************************************************************************************
*/
void Open_emu_LDO(void)
{
    /*  assert_param  */
    HT_EMUECA->ANACTRL |= 0x8000;                                          /*!< Enable EMU LDO        */

    HT_EMUECA->ADCCFG |= 0x2000;                                           /*!< Enable EMU VREF       */

}


/*
*********************************************************************************************************
*                                       CLOSE EMU LDO
*
* 函数说明: 关闭EMU LDO
*
* 入口参数: 无
*
* 返回参数: 无
*
* 特殊说明: 无
*********************************************************************************************************
*/
void Close_emu_LDO(void)
{
    /*  assert_param  */
    HT_EMUECA->ANACTRL &= ~0x8000;                                        /*!< Close EMU LDO        */

}

/*
*********************************************************************************************************
*                                       RESET EMU ECA REGS
*
* 函数说明: 复位EMU模块校表参数
*
* 入口参数: 无
*
* 返回参数: 无
*
* 特殊说明: 无
*********************************************************************************************************
*/
void Reset_EMU_ECA_Regs(void)
{
    /*  assert_param  */
    HT_EMUECA->SRSTREG = 0x55;                                            /*!< reset ECA registers     */

}

/*
*********************************************************************************************************
*                                       RESET EMU MODULE
*
* 函数说明: 复位EMU模块
*
* 入口参数: 无
*
* 返回参数: 无
*
* 特殊说明: 无
*********************************************************************************************************
*/
void Reset_EMU_Module(void)
{
    /*  assert_param  */
    HT_EMUECA->SRSTREG = 0xAA;                                            /*!< reset EMU module         */

}

/*
*********************************************************************************************************
*                                       SET EMU REACTIVE PHASE GAIN
*
* 函数说明: 设置相位补偿
*
* 入口参数: 无
*
* 返回参数: 无
*
* 特殊说明: 无
*********************************************************************************************************
*/

void Set_EMU_Reactive_Phase_Gain(uint8_t Freq)
{
    /*  assert_param  */
    if( Freq < 55 )
    {
        HT_EMUECA->QPHSCAL = QPHSCAL_50Hz;
    }
    else
    {
        HT_EMUECA->QPHSCAL = QPHSCAL_60Hz;
    }
}

/*
*********************************************************************************************************
*                                       GET SPECIFIED EMU STATUS FLAG
*
* 函数说明: 获取EMU寄存器的状态标志
*
* 入口参数: ITFlag    状态标志检测参数，可以为以下参数：
*                        @arg EMU_EMUSR_SFOF
*                        @arg EMU_EMUSR_PEOF
*                        @arg EMU_EMUSR_QEOF
*                        @arg EMU_EMUSR_CHSTS
*                        @arg EMU_EMUSR_TAMP
*                        @arg EMU_EMUSR_G2GTI1
*                        @arg EMU_EMUSR_NOQLD2
*                        @arg EMU_EMUSR_NOPLD2
*                        @arg EMU_EMUSR_NOQLD1
*                        @arg EMU_EMUSR_NOPLD1
*
* 返回参数: ITStatus   = SET：      相应标志置起
                       = RESET：    相应标志未置起
*
* 特殊说明: 无
*********************************************************************************************************
*/
ITStatus HT_EMUSR_FlagStatusGet(uint16_t ITFlag)
{
    /*  assert_param  */
    if(HT_EMUECA->EMUSR & ITFlag)
    {
        return SET;                        /*!< EMU Interrupt Flag is set   */
    }
    else
    {
        return RESET;                      /*!< EMU Interrupt Flag is reset */
    }
}

/*
*********************************************************************************************************
*                                       ENABLE OR DISABLE EMU INTERRUPT
*
* 函数说明: 使能或关闭EMU中断
*
* 入口参数: ITEn    EMU中断设置，可设置为以下参数：
*                        @arg EMUIE_ZXLOST
*                        @arg EMUIE_BUFFULL
*                        @arg EMUIE_UDET
*                        @arg EMUIE_TAMP
*                        @arg EMUIE_DCUPDATE
*                        @arg EMUIE_SRMS
*                        @arg EMUIE_FRMS
*                        @arg EMUIE_ZXI2
*                        @arg EMUIE_ZXI1
*                        @arg EMUIE_I2OV
*                        @arg EMUIE_PF
*                        @arg EMUIE_QF
*                        @arg EMUIE_SF
*                        @arg EMUIE_SPL
*                        @arg EMUIE_ZXU
*                        @arg EMUIE_I1OV
*                        @arg EMUIE_UOV
*                        @arg EMUIE_SAG
*
* 返回参数: NewState   = ENABLE：     使能相应中断
                       = DISABLE：    关闭相应中断
*
* 特殊说明: 无
*********************************************************************************************************
*/
void HT_EMU_ITConfig(uint32_t ITEn, FunctionalState NewState)
{
    /*  assert_param  */
    if (NewState != DISABLE)
    {
        HT_EMUECA->EMUIE |= ITEn;            /*!< enable EMU interrupt          */
    }
    else
    {
        HT_EMUECA->EMUIE &= ~ITEn;           /*!< Disable EMU interrupt       */
    }
}

/*
*********************************************************************************************************
*                                       GET SPECIFIED EMU INTERRUPT FLAG STATUS
*
* 函数说明: 获取EMU模块的中断标志
*
* 入口参数: ITFlag    中断检测设置，可设置为以下参数：
*                        @arg EMUIF_ZXLOST
*                        @arg EMUIF_BUFFULL
*                        @arg EMUIF_UEND
*                        @arg EMUIF_USTART
*                        @arg EMUIF_TAMP
*                        @arg EMUIF_DCUPDATE
*                        @arg EMUIF_SRMS
*                        @arg EMUIF_FRMS
*                        @arg EMUIF_ZXI2
*                        @arg EMUIF_ZXI1
*                        @arg EMUIF_I2OV
*                        @arg EMUIF_PF
*                        @arg EMUIF_QF
*                        @arg EMUIF_SF
*                        @arg EMUIF_SPL
*                        @arg EMUIF_ZXU
*                        @arg EMUIF_I1OV
*                        @arg EMUIF_UOV
*                        @arg EMUIF_SAG
*
* 返回参数: ITStatus   = SET：      相应标志置起
                       = RESET：    相应标志未置起
*
* 特殊说明: 无
*********************************************************************************************************
*/
ITStatus HT_EMU_ITFlagStatusGet(uint32_t ITFlag)
{
    /*  assert_param  */
    if (HT_EMUECA->EMUIF & ITFlag)
    {
        return SET;                        /*!< EMU Interrupt Flag is set   */
    }
    else
    {
        return RESET;                      /*!< EMU Interrupt Flag is reset */
    }
}

/*
*********************************************************************************************************
*                                       CLEAR EMU INTERRUPT FLAG
*
* 函数说明: 清除EMU模块的中断标志
*
* 入口参数: ITFlag  设置清除的中断，可设置为以下参数：
*                        @arg EMUIF_ZXLOST
*                        @arg EMUIF_BUFFULL
*                        @arg EMUIF_UEND
*                        @arg EMUIF_USTART
*                        @arg EMUIF_TAMP
*                        @arg EMUIF_DCUPDATE
*                        @arg EMUIF_SRMS
*                        @arg EMUIF_FRMS
*                        @arg EMUIF_ZXI2
*                        @arg EMUIF_ZXI1
*                        @arg EMUIF_I2OV
*                        @arg EMUIF_PF
*                        @arg EMUIF_QF
*                        @arg EMUIF_SF
*                        @arg EMUIF_SPL
*                        @arg EMUIF_ZXU
*                        @arg EMUIF_I1OV
*                        @arg EMUIF_UOV
*                        @arg EMUIF_SAG
*
* 返回参数: 无
*
* 特殊说明: 无
*********************************************************************************************************
*/
void HT_EMU_ClearITPendingBit(uint32_t ITFlag)
{
    /*  assert_param  */
    HT_EMUECA->EMUIF  &= ~((uint32_t)ITFlag);                  /*!< Clear EMU Interrupt Flag       */
}

/*
*********************************************************************************************************
*                                       READ  EMU ECR REGISTER CHKSUM REGISTER
*
* 函数说明: 读取EMU模块计量参数寄存器的校验和
*
* 入口参数: 无
*
* 返回参数: 寄存器CHECKSUM的值，24位无符号数
*
* 特殊说明: 无
*********************************************************************************************************
*/
uint32_t Read_ECR_ChksumReg(void)
{
  /*  assert_param  */
    return (HT_EMUEPA->CHECKSUM);
}

/*
*********************************************************************************************************
*                                       READ  EMU  PARAMETERS
*
* 函数说明: 读取EMU模块的寄存器值
*
* 入口参数: address 16位无符号数，对应EMU模块寄存器的偏移地址
*
* 返回参数: 对应寄存器的值，32位无符号数
*
* 特殊说明: 计量参数寄存器只读
*********************************************************************************************************
*/
uint32_t Read_EPR_Reg(uint16_t address)
{
  /*  assert_param  */
    uint32_t *ptr;

    ptr = (uint32_t*)(HT_EMU_EPA_BASE + address);
    return ((*ptr)&0xffffffff);
}

/*
*********************************************************************************************************
*                                       READ  EMU  CALIBRATION PARAMETERS
*
* 函数说明: 读取EMU模块的校表参数寄存器值
*
* 入口参数: address 16位无符号数，对应EMU模块校表参数寄存器的偏移地址
*
* 返回参数: 对应寄存器的值，32位无符号数
*
* 特殊说明: 无
*********************************************************************************************************
*/
uint16_t Read_ECR_Reg(uint16_t address)
{
  /*  assert_param  */
    uint16_t *ptr;

    ptr = (uint16_t*)(HT_EMU_ECA_BASE + address);
    return ((*ptr)&0xffff);
}

/*
*********************************************************************************************************
*                                       WRITE  EMU  CALIBRATION PARAMETERS
*
* 函数说明: 写入EMU模块的校表参数寄存器值
*
* 入口参数: address 16位无符号数，对应EMU模块校表参数寄存器的偏移地址
*
* 返回参数: 无
*
* 特殊说明: 无
*********************************************************************************************************
*/
void Write_ECR_Reg(uint16_t address, uint16_t data)
{
  /*  assert_param  */
    uint16_t *ptr;

    ptr = (uint16_t*)(HT_EMU_ECA_BASE + address);
    *ptr = data;
}

/*
*********************************************************************************************************
*                                       SET PULSE PORT
*
* 函数说明: 设置脉冲端口
*
* 入口参数: Port         PFPort typedef, 脉冲输出端口，可设置以下参数：
*                           @arg PF.0, PF.1, PF.2
*           Type         PFType typedef, 端口输出类型，可设置以下参数：
*                           @arg PFout, QFout, SFout
*           PulseWidth   PFWide typedef, 脉宽选择，可设置以下参数：
*                           @P80ms, P40ms, P20ms, P10ms
*
* 返回参数: 无
*
* 特殊说明: EMCON BIT：     15 14  13    12    11    10  9 8 7 6 5 4 3 2 1
*           Default Value：  0  0   0     1     1     0  0 0 0 1 1 1 0 0 0
*                                        PF.0  PF.1  PF.2
*********************************************************************************************************
*/
void Select_Pulse_Port(PFPort Port, PFType Type, PFWide PulseWidth)
{

  /*  assert_param  */
    HT_GPIOF->IOCFG |=  Port;          /*! < PF.0 as PFOUT, PF.1 as QFOUT (default configuration)  */
    HT_GPIOF->AFCFG &= ~Port;      /*! < default                                               */

    switch (Port)
    {
        case PF0:
          HT_EMUECA->EMCON&=~EMCON_CF1CFG;                                /*! < default: P pulse  */
          if(Type==QFout){HT_EMUECA->EMCON|=EMCON_CF1CFG_Q;}
          else if(Type==SFout){HT_EMUECA->EMCON|=EMCON_CF1CFG_S;}
          break;
        case PF1:
          HT_EMUECA->EMCON&=~EMCON_CF2CFG;                                /*! < P pulse         */
          if(Type==QFout){HT_EMUECA->EMCON|=EMCON_CF2CFG_Q;}              /*! < default           */
          else if(Type==SFout){HT_EMUECA->EMCON|=EMCON_CF2CFG_S;}
          break;
        case PF2:
          HT_EMUECA->EMCON&=~EMCON_CF3CFG; /*! < P pulse  */
          if(Type==QFout){HT_EMUECA->EMCON|=EMCON_CF3CFG_Q;}
          else if(Type==SFout){HT_EMUECA->EMCON|=EMCON_CF3CFG_S;}         /*! < default          */
          break;
        default:
          break;
    }

    HT_EMUECA->ADCCON&=~ADCCON_CFP;
    switch(PulseWidth)
    {
        case P80ms:
          HT_EMUECA->ADCCON|=ADCCON_CFP_80MS;
          break;
        case P40ms:
          HT_EMUECA->ADCCON|=ADCCON_CFP_40MS;
          break;
        case P20ms:
          HT_EMUECA->ADCCON|=ADCCON_CFP_20MS;
          break;
        case P10ms:
          HT_EMUECA->ADCCON|=ADCCON_CFP_10MS;
          break;
        default:
          break;
    }
}

/*
*********************************************************************************************************
*                                       SET VALID CHANNEL
*
* 函数说明: 开启采样通道并设置通道增益
*
* 入口参数:   ChOn    uint16_t, 开启采样通道，可设置为以下参数：
*               @arg EMUCTRL_AdcUon
*               @arg EMUCTRL_AdcI1on
*               @arg EMUCTRL_AdcI2on
*             ChGain  uint16_t, 设置通道增益，可设置为以下参数：
*               @arg  ADCCON_ANAU_1
*               @arg  ADCCON_ANAU_2
*               @arg  ADCCON_ANAU_4
*               @arg  ADCCON_ANAU_8
*               @arg  ADCCON_ANAI1_1
*               @arg  ADCCON_ANAI1_2
*               @arg  ADCCON_ANAI1_8
*               @arg  ADCCON_ANAI1_16
*               @arg  ADCCON_ANAI2_1
*               @arg  ADCCON_ANAI2_2
*               @arg  ADCCON_ANAI2_8
*               @arg  ADCCON_ANAI2_16
*
* 返回参数: 无
*
* 特殊说明: 默认状态下U/I1/I2通道关闭，且模拟增益为1
*********************************************************************************************************
*/
void Set_Valid_Channel(uint16_t ChOn, uint16_t ChGain )
{
  /*  assert_param  */
    HT_EMUECA->EMUCTRL |= ChOn;
    HT_EMUECA->ADCCON  |= ChGain;
}

/*
*********************************************************************************************************
*                                       HT_EMU  INITIAL
*
* 函数说明: 初始化EMU模块
*
* 入口参数: 无
*
* 返回参数: 无
*
* 特殊说明: 1. 使能EMU模块时钟
*           2. 复位ECA和EPA寄存器
*           3. 选择脉冲端口的IO，采样类型和脉宽
*           4. 选择采样通道，并设置模拟增益
*           5. 打开EMU模块LDO power
*           6. 选择计量频率
*********************************************************************************************************
*/
void HT_EMU_Init(void)
{
  /*  assert_param  */
    Enable_emu_module();
    Reset_EMU_Module();
    Select_Pulse_Port(PF0, PFout, P80ms);
    Select_Pulse_Port(PF1, QFout, P80ms);
    Set_Valid_Channel((EMUCTRL_AdcUon|EMUCTRL_AdcI1on),(ADCCON_ANAI1_16|ADCCON_ANAU_1));
    Open_emu_LDO();
    Set_EMU_Reactive_Phase_Gain(50);
}

#if  defined  HT502x
/*
*********************************************************************************************************
*                                       START ADCBUFF
*
* 函数说明: 设置ADC缓存寄存器，并且将ADC数据保存在缓存中
*
* 入口参数: 无
*
* 返回参数: 无
*
* 特殊说明: 1. 设置ADC波形数据为高通后，并将采样频率设置为3.2kHz
*           2. 读取电压采样通道的频率
*           3. 计算ADC采样的频率系数
*           4. 将系数写入ECR_BufferCoff的系数寄存器
*           5. 将ECR_BufferStart设置为0xCCC0以开始存储数据
*
*           ADC buffer存满需要46ms， flag在EMUIF.bit17
*********************************************************************************************************
*/
void Start_AdcBuff(void)
{
  /*  assert_param  */
    uint16_t Tmp16;
    uint32_t Tmp32;

    HT_EMUECA->ADCCFG&=~ADCCFG_SPL;
    HT_EMUECA->ADCCFG|=(ADCCFG_WaveSel_HPF|ADCCFG_SPL_3K2);

    Tmp32=HT_EMUEPA->FREQU;                                    /*! < read current frequency of U channel         */
//  i =409600/Tmp;                                             /*! < freq         */
//  Tmp=12800/i;
    Tmp16=(uint16_t)(Tmp32/32);
    Write_ECR_Reg(ECR_BufferCoff,Tmp16);                       /*! < write coefficient register         */
    Write_ECR_Reg(ECR_BufferStart,0xCCC0);                     /*! < write start ADC buffer         */

//  delay(0x30000);                                            /*! < delay 50ms         */
//  while(0==(HT_EMUECA->EMUIF&0x020000));                     /*! < wait for buffer full, it happened interrupt and clear this flag, this instructure will fail. */
}
#endif


#if  defined  HT502x
/*
*********************************************************************************************************
*                                       HT EMU SAG PEAK INITIAL
*
* 函数说明: Sag\Peak使能和Level初始化
*
* 入口参数: SagPeakInit  SagPeak_InitTypeDef pointer
*
* 返回参数: 无
*
* 特殊说明: 1. 设置U Sag peak，或使能Ipeak
*           2. 设置U通道sag level，peak level，并设置检测周期
*           3. 设置I通道peak level，并设置检测周期
*           4. 读后清除所有sag peak标志
*********************************************************************************************************
*/
void HT_EMU_Sag_Peak_init(SagPeak_InitTypeDef SagPeakInit)
{
  /*  assert_param  */
    if(ENABLE==SagPeakInit.UdetEn)
    {
        HT_EMUECA->CHNLCR|=CHNLCR_Udet_En;
    }
    if(ENABLE==SagPeakInit.IpeakEn)
    {
        HT_EMUECA->CHNLCR|=CHNLCR_Ipeak_En;
    }

    HT_EMUECA->USAGLVL  = SagPeakInit.USagLvl;
    HT_EMUECA->UOVLVL = SagPeakInit.UPeakLvl;
    HT_EMUECA->UdetCyc  = SagPeakInit.UDetCyc;
    HT_EMUECA->IOVLVL = SagPeakInit.IPeakLvl;
    HT_EMUECA->IPEAKCYC= SagPeakInit.IDetCyc;
}
#endif

/*
*********************************************************************************************************
*                                       GET_HFCONST_VALUE_CT
*
* 函数说明: 计算CT表的HFConst
*
* 入口参数: meter_EC  imp/KWh
*             U_R1      U channel sample resistor, the big one
*             U_R2      U channel sample resistor, the small one
*             U_apga    Analog gain configured in register ADCCON
*             I_R       I channel sample resistor for CT
*             CT_ratio  CT transmission rate xxxx:1, use xxxx, example: 2500:1, CT_ratio=2500
*             I_aPga    Analog gain configured in register ADCCON
*
* 返回参数: HFconst value,  uint32_t
*
* 特殊说明: 无
*********************************************************************************************************
*/
uint32_t Get_HFConst_value_ct(uint32_t meter_EC, uint32_t U_R1, uint32_t U_R2, uint32_t U_apga, uint32_t I_R, uint32_t CT_ratio, uint32_t I_aPga)
{
    /*  assert_param  */
    uint32_t ratio_U, ratio_I, tmp;

    ratio_U = (U_R1+U_R2)/U_R2*U_apga;
    ratio_I = CT_ratio/I_R/I_aPga;
    tmp=2332000/meter_EC*1000/ratio_U/ratio_I;
    return tmp;
}

/*
*********************************************************************************************************
*                                       GET_HFCONST_VALUE_SHUNT
*
* 函数说明: 计算shunt表的HFConst
*
* 入口参数: meter_EC  imp/KWh
*             U_R1    U channel sample resistor, the big one
*             U_R2    U channel sample resistor, the small one
*             U_apga  Analog gain configured in register ADCCON
*             I_R     I channel shunt resistor, unit: ohm*10^6, example : 250uohm, I_R=250
*             APga    Analog gain configured in register ADCCON
*
* 返回参数: HFconst value,  uint32_t
*
* 特殊说明: 无
*********************************************************************************************************
*/
uint32_t Get_HFConst_value_Shunt(uint32_t meter_EC, uint32_t U_R1, uint32_t U_R2, uint32_t U_apga, uint32_t I_R, uint32_t I_aPga)
{
    /*  assert_param  */
    uint32_t ratio_U, ratio_I, tmp;

    ratio_U = (U_R1+U_R2)/U_R2*U_apga;
    ratio_I = 1000000/I_R/I_aPga;
    tmp=2332000/meter_EC*1000/ratio_U/ratio_I;
    return tmp;
}



#endif                                               /* This File Only support HT501x & HT502x */

