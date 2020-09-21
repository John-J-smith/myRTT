/*
**********************************************************************************************************
*                                              HT6XXX
*                                          Library Function
*
*                                   Copyright 2013, Hi-Trend Tech, Corp.
*                                        All Rights Reserved
*                                         
*
* Project      : HT6xxx
* File         : ht50xx_emu.h
* By           : Hitrendtech_SocTeam
* Version      : V1.0.1
* Description  : Only support HT501x and HT502x
**********************************************************************************************************
*/

#ifndef __HT50XX_EMU_H__
#define __HT50XX_EMU_H__

#ifdef __cplusplus
extern "C" {
#endif 
    
#include "ht6xxx.h"
    
#if defined  HT501x  ||  defined  HT502x                        /* This File Only support HT501x and HT502x */
    
/*
*********************************************************************************************************
*                                          Global macro / structure
*********************************************************************************************************
*/

/****EPR register offset address, or Energy metering Parameters Registers offset address define, base address: 0x40013000,(read only)****************/
#define   EPR_SPLI1                   0x00
#define   EPR_SPLI2                   0x04
#define   EPR_SPLU                    0x08
#define   EPR_SPLP                    0x0C
#define   EPR_SPLQ                    0x10
#define   EPR_FastRMSI1               0x14
#define   EPR_FastRMSI2               0x18
#define   EPR_FastRMSU                0x1C
#define   EPR_FreqU                   0x20
#define   EPR_FastPowerP1             0x24
#define   EPR_FastPowerQ1             0x28
#define   EPR_FastPowerP2             0x2C
#define   EPR_FastPowerQ2             0x30
#define   EPR_FastPowerS1             0x34
#define   EPR_FastPowerS2             0x38
#define   EPR_RMSI1                   0x3C
#define   EPR_RMSI2                   0x40
#define   EPR_RMSU                    0x44
#define   EPR_PowerP1                 0x48
#define   EPR_PowerQ1                 0x4C
#define   EPR_PowerP2                 0x50
#define   EPR_PowerQ2                 0x54
#define   EPR_PowerS1                 0x58
#define   EPR_PowerS2                 0x5C
#define   EPR_EnergyP                 0x60
#define   EPR_EnergyQ                 0x64
#define   EPR_EnergyS                 0x68
#define   EPR_EnergyPC                0x6C
#define   EPR_EnergyQC                0x70
#define   EPR_EnergySC                0x74
#define   EPR_DC_UAverage             0x78
#define   EPR_DC_I1Average            0x7C
#define   EPR_DC_I2Average            0x80
#define   EPR_Checksum                0x84
#if defined HT502x
#define   EPR_Upd_Half                0x88
#define   EPR_I1pd_Half               0x8C
#define   EPR_I2pd_Half               0x90
#else                                 
#define   EPR_Upeak                   0x88
#define   EPR_I1peak                  0x8C
#define   EPR_I2peak                  0x90
#endif
#define   EPR_PFCNT_Photo             0x94
#define   EPR_QFCNT_Photo             0x98
#define   EPR_SFCNT_Photo             0x9C
#define   EPR_AutoUgain               0xA0
#define   EPR_AutoI1gain              0xA4
#define   EPR_AutoI2gain              0xA8
#if defined HT502x
#define   EPR_R_Buffer                0xAC
#define   EPR_UdetCNT                 0xB8
#endif


/*******ECR register offset address, or Energy Calibration Registers offset address define, base address: 0x40013800, (write & read enable )*****************/  
#define   ECR_EMUSR                   0x00
#define   ECR_EMUIE                   0x04
#define   ECR_EMUIF                   0x08
#define   ECR_GP1                     0x0C
#if defined HT501x
#define   ECR_GQ1                     0x10
#define   ECR_GS1                     0x14
#endif
#define   ECR_GPhs1                   0x18
#define   ECR_GP2                     0x1C
#if defined HT501x
#define   ECR_GQ2                     0x20
#define   ECR_GS2                     0x24
#endif
#define   ECR_GPhs2                   0x28
#define   ECR_QPhsCal                 0x2C
#define   ECR_I2Gain                  0x30
#define   ECR_I1Off                   0x34
#define   ECR_I2Off                   0x38
#define   ECR_UOff                    0x3C
#define   ECR_PStart                  0x40
#define   ECR_QStart                  0x44
#define   ECR_SStart                  0x48
#define   ECR_HFconst                 0x4C
#define   ECR_ADCCFG                  0x50
#define   ECR_CHNLCR                  0x54
#define   ECR_EMCON                   0x58
#define   ECR_PFCnt                   0x5C
#define   ECR_QFCnt                   0x60
#define   ECR_SFCnt                   0x64
#define   ECR_ADCCON                  0x68
#define   ECR_IPTAMP                  0x6C
#define   ECR_ICHK                    0x70
#define   ECR_EMUCTRL                 0x74
#define   ECR_P1OFFSET                0x78
#define   ECR_P2OFFSET                0x7C
#define   ECR_Q1OFFSET                0x80
#define   ECR_Q2OFFSET                0x84
#define   ECR_I1RMSOFFSET             0x88
#define   ECR_I2RMSOFFSET             0x8C
#define   ECR_URMSOFFSET              0x90
#define   ECR_RosiCtrl                0x94
#define   ECR_ANA_control             0x98
#define   ECR_UCONST                  0x9C
#define   ECR_LpIdleTime              0xA0
#define   ECR_USAGLVL                 0xA4
#if defined HT502x
#define   ECR_IpeakCyc                0xA8
#else
#define   ECR_USagCyc                 0xA8
#endif
#define   ECR_UOVLVL                  0xAC
#if defined HT502x
#define   ECR_UdetCyc                 0xB0
#else
#define   ECR_OvCyc                   0xB0
#endif
#define   ECR_IOVLVL                  0xB4
#define   ECR_ZXILVL                  0xB8
#define   ECR_PDataCpH                0xBC
#define   ECR_PDataCpL                0xC0
#define   ECR_QDataCpH                0xC4
#define   ECR_QDataCpL                0xC8
#define   ECR_SDataCpH                0xCC
#define   ECR_SDataCpL                0xD0
#define   ECR_FilterCtrl              0xD4
#define   ECR_TUgain                  0xD8
#define   ECR_TI1gain                 0xDC
#define   ECR_TI2gain                 0xE0
#define   ECR_UTCcoffA                0xE4
#define   ECR_UTCcoffB                0xE8
#define   ECR_UTCcoffC                0xEC
#define   ECR_I1TCcoffA               0xF0
#define   ECR_I1TCcoffB               0xF4
#define   ECR_I1TCcoffC               0xF8
#define   ECR_I2TCcoffA               0xFC
#define   ECR_I2TCcoffB               0x100
#define   ECR_I2TCcoffC               0x104
//The following ECR regiters do not perticipate in checksum calculations of EPR_Checksum register 
#define   ECR_LoadDataCp              0x108
#define   ECR_SRSTREG                 0x114
#if defined HT502x
#define   ECR_BufferStart             0x10C
#define   ECR_BufferCoff              0x110
#define   ECR_PFCntN                  0x118
#define   ECR_QFCntN                  0x11C
#endif

/**************************  Bit definition for EMUSR register  ********************************/
#define   EMUSR_SFOF              ((uint16_t)0x0800)    /*<apparent energy register overflow >*/
#define   EMUSR_PEOF              ((uint16_t)0x0400)    /*<active energy register overflow >*/
#define   EMUSR_QEOF              ((uint16_t)0x0200)    /*<reactive energy register overflow >*/
#define   EMUSR_CHSTS             ((uint16_t)0x0100)    /*<valid energy channel flag>*/
#define   EMUSR_TAMP              ((uint16_t)0x0080)    /*<Anti-tamper flag>*/
#define   EMUSR_G2GTI1            ((uint16_t)0x0040)    /*<current or energy power of I2 upper I1 flag >*/
#define   EMUSR_NOQLD2            ((uint16_t)0x0020)    /*<channle 2 reactive power creeping flag>*/
#define   EMUSR_NOPLD2            ((uint16_t)0x0010)    /*<channle 2 active power creeping flag>*/
#define   EMUSR_NOQLD1            ((uint16_t)0x0008)    /*<channle 1 reactive power creeping flag>*/
#define   EMUSR_NOPLD1            ((uint16_t)0x0004)    /*<channle 1 active power creeping flag>*/


/**************************  Bit definition for EMUIE register  ********************************/
#if defined HT502x
#define   EMUIE_ZXLOST            ((uint32_t)0x020000)
#define   EMUIE_BUFFULL           ((uint32_t)0x010000)
#define   EMUIE_UDET              ((uint32_t)0x008000)
#endif
#define   EMUIE_TAMP              ((uint32_t)0x004000)
#define   EMUIE_DCUPDATE          ((uint32_t)0x002000)
#define   EMUIE_SRMS              ((uint32_t)0x001000)
#define   EMUIE_FRMS              ((uint32_t)0x000800)
#define   EMUIE_ZXI2              ((uint32_t)0x000400)
#define   EMUIE_ZXI1              ((uint32_t)0x000200)
#define   EMUIE_I2OV              ((uint32_t)0x000100)
#define   EMUIE_PF                ((uint32_t)0x000080)
#define   EMUIE_QF                ((uint32_t)0x000040)
#define   EMUIE_SF                ((uint32_t)0x000020)
#define   EMUIE_SPL               ((uint32_t)0x000010)
#define   EMUIE_ZXU               ((uint32_t)0x000008)
#define   EMUIE_I1OV              ((uint32_t)0x000004)
#define   EMUIE_UOV               ((uint32_t)0x000002)
#define   EMUIE_SAG               ((uint32_t)0x000001)
 
/**************************  Bit definition for EMUIF register  ********************************/
#if defined HT502x
#define   EMUIF_ZXLOST            ((uint32_t)0x040000)
#define   EMUIF_BUFFULL           ((uint32_t)0x020000)
#define   EMUIF_UEND              ((uint32_t)0x010000)
#define   EMUIF_USTART            ((uint32_t)0x008000)
#endif                            
#define   EMUIF_TAMP              ((uint32_t)0x004000)
#define   EMUIF_DCUPDATE          ((uint32_t)0x002000)
#define   EMUIF_SRMS              ((uint32_t)0x001000)
#define   EMUIF_FRMS              ((uint32_t)0x000800)
#define   EMUIF_ZXI2              ((uint32_t)0x000400)
#define   EMUIF_ZXI1              ((uint32_t)0x000200)
#define   EMUIF_I2OV              ((uint32_t)0x000100)
#define   EMUIF_PF                ((uint32_t)0x000080)
#define   EMUIF_QF                ((uint32_t)0x000040)
#define   EMUIF_SF                ((uint32_t)0x000020)
#define   EMUIF_SPL               ((uint32_t)0x000010)
#define   EMUIF_ZXU               ((uint32_t)0x000008)
#define   EMUIF_I1OV              ((uint32_t)0x000004)
#define   EMUIF_UOV               ((uint32_t)0x000002)
#define   EMUIF_SAG               ((uint32_t)0x000001)

/**************************  50Hz or 60Hz selection for QPHSCal register  **************************************************************/
#define   QPHSCAL_50Hz            ((uint32_t)0x0050)      /*<Reactive power phase compensation register value>*/
#define   QPHSCAL_60Hz            ((uint32_t)0x0044)      /*<Reactive power phase compensation register value>*/

/**************************  Bit definition for ADCCFG register  ***********************************************************************/
#define   ADCCFG_PQSStartSourceSel                  ((uint32_t)0xC000)      /*<PQS start and creep compare source control bits>*/
#define   ADCCFG_PQSStartSourceSel_FastPower        ((uint32_t)0x0000)      /*<PQS start creep source select Fast Power>*/
#define   ADCCFG_PQSStartSourceSel_SlowPower        ((uint32_t)0x4000)      /*<PQS start and creep source select Slow Power>*/
#define   ADCCFG_PQSStartSourceSel_InstantPower     ((uint32_t)0x8000)      /*<PQS start creep source select Instantaneous Power>*/

#define   ADCCFG_VrefEn           ((uint32_t)0x2000)      /*<Vref enable control bit>*/
#define   ADCCFG_AddCIADD         ((uint32_t)0x1000)      /*<Accumulation mode control bit in Single-phase three-wire mode>*/

#define   ADCCFG_WaveSel          ((uint32_t)0x0800)      /*<ADC waveform sampled data source selection bit>*/
#define   ADCCFG_WaveSel_Origi    ((uint32_t)0x0000)      /*<ADC waveform sampled data source selection original data>*/
#define   ADCCFG_WaveSel_HPF      ((uint32_t)0x0800)      /*<ADC waveform sampled data source selection by high pass filter data>*/

#define   ADCCFG_TampSourceSel    ((uint32_t)0x0400)      /*<Power stealing auto-prevention channel interchannel compare source selection bit>*/
#define   ADCCFG_IPTampSource_Sel ((uint32_t)0x0200)      /*<Power stealing auto-prevention threshold judgement source fast/slow selection bit>*/
#define   ADCCFG_CHNFix           ((uint32_t)0x0100)      /*<Power stealing auto-prevention small signal channel switch configuration bit>*/
#define   ADCCFG_IPTampSel        ((uint32_t)0x0080)      /*<Power stealing auto-prevention threshold judgement basis selection bit>*/
#define   ADCCFG_TampSel          ((uint32_t)0x0040)      /*<Power stealing prevention comparisonmethod selection bit>*/
#define   ADCCFG_FLTON            ((uint32_t)0x0020)      /*<Power stealing auto-prevention module enable bit>*/
#define   ADCCFG_CHNSEL           ((uint32_t)0x0010)      /*<Select the channel to measure channel selection bit>*/
#define   ADCCFG_CIADD            ((uint32_t)0x0008)      /*<Single-phase three-wire accumulation mode selection bit>*/

#define   ADCCFG_SPL              ((uint32_t)0x0007)      /*<Waveform sample frequency selection control bits>*/
#define   ADCCFG_SPL_1K6          ((uint32_t)0x0000)      /*<Waveform sample frequency is 1.6KHz>*/
#define   ADCCFG_SPL_3K2          ((uint32_t)0x0001)      /*<Waveform sample frequency is 3.2KHz>*/
#define   ADCCFG_SPL_6K4          ((uint32_t)0x0002)      /*<Waveform sample frequency is 6.4KHz>*/
#define   ADCCFG_SPL_12K8         ((uint32_t)0x0003)      /*<Waveform sample frequency is 12.8KHz>*/

/**************************  Bit definition for CHNLCR register  ***********************************************************************/
#define   CHNLCR_PDatacpEn        ((uint32_t)0x8000)      /*<Active power constant calculation enable bit>*/
#define   CHNLCR_QDatacpEn        ((uint32_t)0x4000)      /*<Reactive power constant calculation enable bit>*/
#define   CHNLCR_SDatacpEn        ((uint32_t)0x2000)      /*<Apparent power constant calculation enable bit>*/
#define   CHNLCR_Ipeak_En         ((uint32_t)0x1000)      /*<Voltage SAG function enable control bit>*/
#define   CHNLCR_Udet_En          ((uint32_t)0x0800)      /*<Over-voltage over-current enable control bit>*/
#define   CHNLCR_RmsLpfEn         ((uint32_t)0x0400)      /*<RmsLpf enable control bit>*/
#define   CHNLCR_PQLpfEn          ((uint32_t)0x0200)      /*<PQLpf enable control bit>*/
#define   CHNLCR_Add_DataCP       ((uint32_t)0x0100)      /*<Energy accumulation rate selection bit during constant calculation bit>*/
#define   CHNLCR_LPMode           ((uint32_t)0x0080)      /*<Low power consumption mode enable control bit>*/
#define   CHNLCR_POS              ((uint32_t)0x0040)      /*<Pulse significant level selection bit>*/
#define   CHNLCR_LPFreqSel        ((uint32_t)0x0020)      /*<Low power consumption mode clock selection bit>*/
#define   CHNLCR_UconstEn         ((uint32_t)0x0010)      /*<Low power consumption mode apparent power voltage source selection bit>*/
#define   CHNLCR_PSSel            ((uint32_t)0x0008)      /*<Output Pulse type selection bit>*/
#define   CHNLCR_HPFON            ((uint32_t)0x0007)      /*<Voltage channel &Current channel2 &Current channel1 high pass filter switches bits>*/
//#define   CHNLCR_HPFONI2          ((uint32_t)0x0002)      /*<U/I1/I2 HPF need all open for normal metering >*/
//#define   CHNLCR_HPFONI1          ((uint32_t)0x0001)      /*<Current channel1 high pass filter switches bit>*/

/**************************  Bit definition for EMCON register  ***********************************************************************/
#define   EMCON_CF1CFG            ((uint32_t)0xC000)      /*<CF1 interface output configuration control bits>*/
#define   EMCON_CF1CFG_P          ((uint32_t)0x0000)      /*<CF1 interface output P pulse>*/
#define   EMCON_CF1CFG_Q          ((uint32_t)0x4000)      /*<CF1 interface output Q pulse>*/
#define   EMCON_CF1CFG_S          ((uint32_t)0x8000)      /*<CF1 interface output S pulse>*/
                                  
#define   EMCON_CF2CFG            ((uint32_t)0x3000)      /*<CF2 interface output configuration control bits>*/
#define   EMCON_CF2CFG_P          ((uint32_t)0x0000)      /*<CF2 interface output P pulse>*/
#define   EMCON_CF2CFG_Q          ((uint32_t)0x1000)      /*<CF2 interface output Q pulse>*/
#define   EMCON_CF2CFG_S          ((uint32_t)0x2000)      /*<CF2 interface output S pulse>*/
                                  
#define   EMCON_CF3CFG            ((uint32_t)0x0C00)      /*<CF3 interface output configuration control bits>*/
#define   EMCON_CF3CFG_P          ((uint32_t)0x0000)      /*<CF3 interface output P pulse>*/
#define   EMCON_CF3CFG_Q          ((uint32_t)0x0400)      /*<CF3 interface output Q pulse>*/
#define   EMCON_CF3CFG_S          ((uint32_t)0x0800)      /*<CF3 interface output S pulse>*/

#define   EMCON_AverPowerCal      ((uint32_t)0x0200)      /*<Energy accumulation source selection bit>*/
#define   EMCON_AverPowerSource   ((uint32_t)0x0100)      /*<If AverPowerCal =1, average power source selection bit>*/
#define   EMCON_OSR128            ((uint32_t)0x0080)      /*<OSR selection bit of normal calculation mode selection bit>*/
#define   EMCON_SRun              ((uint32_t)0x0040)      /*<Apparent energy accumulation enable bit>*/
#define   EMCON_QRun              ((uint32_t)0x0020)      /*<Reactive energy accumulation enable bit>*/
#define   EMCON_PRun              ((uint32_t)0x0010)      /*<Active energy accumulation enable bit>*/

#define   EMCON_QMOD              ((uint32_t)0x000C)      /*<Reactive energy accumulation method selection>*/
#define   EMCON_QMOD_Algebraic    ((uint32_t)0x0000)      /*<Accumulate reactive power in algebraic addition way>*/
#define   EMCON_QMOD_Positive     ((uint32_t)0x0004)      /*<only accumulate the positive reactive power>*/
#define   EMCON_QMOD_Absolute     ((uint32_t)0x0008)      /*<accumulate reactive power in absolute value format>*/

#define   EMCON_PMOD              ((uint32_t)0x0003)      /*<Active energy accumulation method selection>*/
#define   EMCON_PMOD_Algebraic    ((uint32_t)0x0000)      /*<Accumulate active power in algebraic addition way>*/
#define   EMCON_PMOD_Positive     ((uint32_t)0x0001)      /*<only accumulate the positive active power>*/
#define   EMCON_PMOD_Absolute     ((uint32_t)0x0002)      /*<accumulate active power in absolute value format>*/

/**************************  Bit definition for ADCCON register  *******************************************************************/
#define   ADCCON_DGI2             ((uint32_t)0x3000)      /*<I2 channel digital gain setting>*/
#define   ADCCON_DGI2_1           ((uint32_t)0x0000)      /*<I2 channel digital gain is 1>*/
#define   ADCCON_DGI2_2           ((uint32_t)0x1000)      /*<I2 channel digital gain is 2>*/
#define   ADCCON_DGI2_4           ((uint32_t)0x2000)      /*<I2 channel digital gain is 4>*/
#define   ADCCON_DGI2_8           ((uint32_t)0x3000)      /*<I2 channel digital gain is 8>*/
                                  
#define   ADCCON_DGI1             ((uint32_t)0x0C00)      /*<I1 channel digital gain setting>*/
#define   ADCCON_DGI1_1           ((uint32_t)0x0000)      /*<I1 channel digital gain is 1>*/
#define   ADCCON_DGI1_2           ((uint32_t)0x0400)      /*<I1 channel digital gain is 2>*/
#define   ADCCON_DGI1_4           ((uint32_t)0x0800)      /*<I1 channel digital gain is 4>*/
#define   ADCCON_DGI1_8           ((uint32_t)0x0C00)      /*<I1 channel digital gain is 8>*/
                                  
#define   ADCCON_DGU              ((uint32_t)0x0300)      /*<U channel digital gain setting>*/
#define   ADCCON_DGU_1            ((uint32_t)0x0000)      /*<U channel digital gain is 1>*/
#define   ADCCON_DGU_2            ((uint32_t)0x0100)      /*<U channel digital gain is 2>*/
#define   ADCCON_DGU_4            ((uint32_t)0x0200)      /*<U channel digital gain is 4>*/
#define   ADCCON_DGU_8            ((uint32_t)0x0300)      /*<U channel digital gain is 8>*/
                                  
#define   ADCCON_CFP              ((uint32_t)0x00C0)      /*<pulse width setting>*/
#define   ADCCON_CFP_80MS         ((uint32_t)0x0000)      /*<pulse width is 80ms>*/
#define   ADCCON_CFP_40MS         ((uint32_t)0x0040)      /*<pulse width is 40ms>*/
#define   ADCCON_CFP_20MS         ((uint32_t)0x0080)      /*<pulse width is 20ms>*/
#define   ADCCON_CFP_10MS         ((uint32_t)0x00C0)      /*<pulse width is 10ms>*/

#define   ADCCON_ANAI2            ((uint32_t)0x0030)      /*<I2 channel analogl gain setting>*/
#define   ADCCON_ANAI2_1          ((uint32_t)0x0000)      /*<I2 channel analogl gain is 1>*/
#define   ADCCON_ANAI2_2          ((uint32_t)0x0010)      /*<I2 channel analogl gain is 2>*/
#define   ADCCON_ANAI2_8          ((uint32_t)0x0020)      /*<I2 channel analogl gain is 8>*/
#define   ADCCON_ANAI2_16         ((uint32_t)0x0030)      /*<I2 channel analogl gain is 16>*/
                                  
#define   ADCCON_ANAI1            ((uint32_t)0x000C)      /*<I2 channel analogl gain setting>*/
#define   ADCCON_ANAI1_1          ((uint32_t)0x0000)      /*<I2 channel analogl gain is 1>*/
#define   ADCCON_ANAI1_2          ((uint32_t)0x0004)      /*<I2 channel analogl gain is 2>*/
#define   ADCCON_ANAI1_8          ((uint32_t)0x0008)      /*<I2 channel analogl gain is 8>*/
#define   ADCCON_ANAI1_16         ((uint32_t)0x000C)      /*<I2 channel analogl gain is 16>*/
                                  
#define   ADCCON_ANAU             ((uint32_t)0x0003)      /*<U channel analogl gain setting>*/
#define   ADCCON_ANAU_1           ((uint32_t)0x0000)      /*<U channel analogl gain is 1>*/
#define   ADCCON_ANAU_2           ((uint32_t)0x0001)      /*<U channel analogl gain is 2>*/
#define   ADCCON_ANAU_4           ((uint32_t)0x0002)      /*<U channel analogl gain is 4>*/
#define   ADCCON_ANAU_8           ((uint32_t)0x0003)      /*<U channel analogl gain is 8>*/

/**************************  Bit definition for EMUCTRL register  ***********************************************************************/
#define   EMUCTRL_StartSel        ((uint32_t)0x2000)      /*<Anti creep mode selection bit>*/
#define   EMUCTRL_StartSel_PorQ   ((uint32_t)0x2000)      /*<Anti creep mode selection bit=1, P&Q joint judgment mode; >*/
#define   EMUCTRL_StartSel_PQResp ((uint32_t)0x0000)      /*<Anti creep mode selection bit=0, P, Q judgment respectively; >*/

#define   EMUCTRL_VrefLPEn        ((uint32_t)0x1000)      /*<In low power mode, Lvref involve calculation control bit>*/
#define   EMUCTRL_P2EN            ((uint32_t)0x0800)      /*<P2 replace Q1 metering channel enable select bit>*/
#define   EMUCTRL_EMUClkCtrl      ((uint32_t)0x0400)      /*<EMU clock control bit>*/

#define   EMUCTRL_DCPRFCFG        ((uint32_t)0x0300)      /*<DC average update frequency control bits>*/
#define   EMUCTRL_DCPRFCFG_25Hz   ((uint32_t)0x0000)      /*<DC average update frequency is 25Hz>*/
#define   EMUCTRL_DCPRFCFG_12Hz5  ((uint32_t)0x0100)      /*<DC average update frequency is 12.5Hz>*/
#define   EMUCTRL_DCPRFCFG_6Hz25  ((uint32_t)0x0200)      /*<DC average update frequency is 6.25Hz>*/
#define   EMUCTRL_DCPRFCFG_3Hz125 ((uint32_t)0x0300)      /*<DC average update frequency is 3.125Hz>*/

#define   EMUCTRL_SlowPRMS        ((uint32_t)0x0080)      /*<Slow Rms power update frequency control bit>*/
#define   EMUCTRL_AdcI1on         ((uint32_t)0x0040)      /*<I1_ADC enable bit>*/
#define   EMUCTRL_AdcI2on         ((uint32_t)0x0020)      /*<I2_ADC enable bit>*/
#define   EMUCTRL_AdcUon          ((uint32_t)0x0010)      /*<U_ADC enable bit>*/

#define   EMUCTRL_Zxd             ((uint32_t)0x000C)      /*<Voltage current zero crossing interrupt method select control bits>*/
#define   EMUCTRL_Zxd_Pos         ((uint32_t)0x0000)      /*<Voltage current zero crossing interrupt method select Positive zero crossing>*/
#define   EMUCTRL_Zxd_Neg         ((uint32_t)0x0004)      /*<Voltage current zero crossing interrupt method select Negative zero crossing>*/
#define   EMUCTRL_Zxd_Bidir       ((uint32_t)0x0008)      /*<Voltage current zero crossing interrupt method select Bidirectional zero crossing>*/

#define   EMUCTRL_PRFCFG          ((uint32_t)0x0003)      /*<Fast Rms power update frequency control bits>*/
#define   EMUCTRL_PRFCFG_20Hz     ((uint32_t)0x0000)      /*<Rms update frequency is 20Hz>*/
#define   EMUCTRL_PRFCFG_10Hz     ((uint32_t)0x0001)      /*<Rms update frequency is 10Hz>*/
#define   EMUCTRL_PRFCFG_5Hz      ((uint32_t)0x0002)      /*<Rms update frequency is 5Hz>*/
#define   EMUCTRL_PRFCFG_2Hz5     ((uint32_t)0x0003)      /*<Rms update frequency is 2.5Hz>*/

/**************************  Bit definition for ROSICTRL register  ***********************************************************************/
#define   ROSICTRL_AutoTcEN       ((uint32_t)0x8000)      /*<Temperature auto compensation control bit>*/

#define   ROSICTRL_Clkdelay       ((uint32_t)0x7000)      /*<Clk delay between Analog digital>*/
#define   ROSICTRL_Clkdelay_100   ((uint32_t)0x4000)      /*<Clk delay selection bits:100>*/

#define   ROSICTRL_AdciCtr        ((uint32_t)0x0600)      /*<ADC operation current control bits>*/
#define   ROSICTRL_AdciCtr_10uA   ((uint32_t)0x0000)      /*<ADC operation current is 10uA>*/
#define   ROSICTRL_AdciCtr_1uA5   ((uint32_t)0x0200)      /*<ADC operation current is 1.5uA>*/
#define   ROSICTRL_AdciCtr_3uA    ((uint32_t)0x0400)      /*<ADC operation current is 3uA>*/
#define   ROSICTRL_AdciCtr_5uA    ((uint32_t)0x0600)      /*<ADC operation current is 5uA>*/

#define   ROSICTRL_DCEn           ((uint32_t)0x0100)      /*<DC average function enable bit>*/
#define   ROSICTRL_DCSEn          ((uint32_t)0x0080)      /*<apparent power energy enable control bit>*/
#define   ROSICTRL_XboFiter       ((uint32_t)0x0040)      /*<= 0 , notch filter center frequency is 50Hz; = 1, notch filter center frequency is 60Hz>*/
#define   ROSICTRL_LpIdleEn       ((uint32_t)0x0020)      /*<Open Low power calculation in time division enable control bit>*/
#define   ROSICTRL_LpStaCtrl      ((uint32_t)0x0010)      /*<In Low power calculation mode, steady points num control bit:
                                                            = 0，lose fixed 512 points;= 1，lose fixed 1024 points>*/

#define   ROSICTRL_LpCtrl         ((uint32_t)0x000C)      /*<average time control bit in Low power calculation mode>*/
#define   ROSICTRL_LpCtrl_0s5     ((uint32_t)0x0000)      /*<average time is 0.5s>*/
#define   ROSICTRL_LpCtrl_1s      ((uint32_t)0x0004)      /*<average time is 1s>*/
#define   ROSICTRL_LpCtrl_2s      ((uint32_t)0x0008)      /*<average time is 2s>*/
#define   ROSICTRL_LpCtrl_4s      ((uint32_t)0x000C)      /*<average time is 4s>*/

#define   ROSICTRL_RosiI2En       ((uint32_t)0x0002)      /*<Rogowski coil of current channel2 enable bit>*/
#define   ROSICTRL_RosiI1En       ((uint32_t)0x0001)      /*<Rogowski coil of current channel1 enable bit>*/

/**************************  Bit definition for ANACTRL register  ***********************************************************************/
#define   ANACTRL_ldoEmuEn        ((uint32_t)0x8000)      /*<EMU Analog supply LDO enable control bit>*/

#define   ANACTRL_VrefCtrl        ((uint32_t)0x3800)      /*<Vref TC peak adjust code>*/
#define   ANACTRL_VrefCtrl_100    ((uint32_t)0x2000)      /*<Vref TC peak adjust code selection bits:100>*/

#define   ANACTRL_ChopVrefEn      ((uint32_t)0x0400)      /*<Vref chop control bit>*/
#define   ANACTRL_REVbit9Set      ((uint32_t)0x0200)      /*<Reserve bit9, default:1>*/

#define   ANACTRL_AdcChopEn       ((uint32_t)0x0100)      /*<Adc chop control bit>*/
#define   ANACTRL_UchopEn         ((uint32_t)0x0080)      /*<Voltage channel chop control bit>*/

#define   ANACTRL_REVbit62        ((uint32_t)0x007C)      /*<Reserve bit6~2>*/
#define   ANACTRL_REVbit62_Default    ((uint32_t)0x0024)      /*<Reserve bit6~2 selection default bits: 01001>*/

#define   ANACTRL_VrefChopBufEn   ((uint32_t)0x0002)      /*<Vref chop buf enable control bit>*/
#define   ANACTRL_lowVrefIsel     ((uint32_t)0x0001)      /*<Lvref offset current control bit,= 0，offset current is 10ua;= 1，offset current is 5ua>*/

/**************************  Bit definition for FilterCtrl register  ***********************************************************************/
#define   FilterCtrl_RosiPgaCtr   ((uint32_t)0x1800)      /*<Gain of two current channel Rogowski coil control bits>*/
#define   FilterCtrl_RosiPgaCtr_1 ((uint32_t)0x0000)      /*<Gain of two current channel Rogowski coil is 1>*/
#define   FilterCtrl_RosiPgaCtr_2 ((uint32_t)0x0100)      /*<Gain of two current channel Rogowski coil is 2>*/
#define   FilterCtrl_RosiPgaCtr_4 ((uint32_t)0x1000)      /*<Gain of two current channel Rogowski coil is 4>*/
#define   FilterCtrl_RosiPgaCtr_8 ((uint32_t)0x1800)      /*<Gain of two current channel Rogowski coil is 8>*/

#define   FilterCtrl_ZXsourceSel  ((uint32_t)0x0400)      /*<Current voltage zero crossing source selection>*/

#define   FilterCtrl_PahseShiftCtr        ((uint32_t)0x0300)      /*<90de phase shift filter coefficient control bits>*/
#define   FilterCtrl_PahseShiftCtr_11     ((uint32_t)0x0000)      /*<90de phase shift filter coefficient is 11>*/
#define   FilterCtrl_PahseShiftCtr_12     ((uint32_t)0x0100)      /*<90de phase shift filter coefficient is 12>*/
#define   FilterCtrl_PahseShiftCtr_13     ((uint32_t)0x0200)      /*<90de phase shift filter coefficient is 13>*/
#define   FilterCtrl_PahseShiftCtr_14     ((uint32_t)0x0300)      /*<90de phase shift filter coefficient is 14>*/

#define   FilterCtrl_RosiCtr      ((uint32_t)0x00C0)      /*<Gain of two current channel Rogowski coil filter coefficient control bits>*/
#define   FilterCtrl_RosiCtr_11   ((uint32_t)0x0000)      /*<Gain of two current channel Rogowski coil filter coefficient is 11>*/
#define   FilterCtrl_RosiCtr_12   ((uint32_t)0x0040)      /*<Gain of two current channel Rogowski coil filter coefficient is 12>*/
#define   FilterCtrl_RosiCtr_13   ((uint32_t)0x0080)      /*<Gain of two current channel Rogowski coil filter coefficient is 13>*/
#define   FilterCtrl_RosiCtr_14   ((uint32_t)0x00C0)      /*<Gain of two current channel Rogowski coil filter coefficient is 14>*/

#define   FilterCtrl_HCtl         ((uint32_t)0x0038)      /*<Internal coefficient1 selection control bits>*/
#define   FilterCtrl_LCtl         ((uint32_t)0x0007)      /*<Internal coefficient2 selection control bits>*/

/*
*********************************************************************************************************
*                                            Structure Type define
*********************************************************************************************************
*/
typedef enum{PF0=0x01, PF1=0x02, PF2=0x04} PFPort;
typedef enum{PFout=0, QFout=2, SFout=3}PFType;
typedef enum{P80ms, P40ms, P20ms,P10ms}PFWide;

typedef struct
{
  uint16_t  UdetEn;       /*!< =ENABLE or DISABLE         */
  uint16_t  IpeakEn;      /*!< =ENABLE or DISABLE         */
  uint16_t  USagLvl;  
  uint16_t  UPeakLvl;
  uint16_t  UDetCyc;      /*!< range: 0~65535             */
  uint16_t  IPeakLvl;
  uint16_t  IDetCyc;      /*!< range: 0~65535             */
}SagPeak_InitTypeDef;


/*
*********************************************************************************************************
*                                            self-defined global macro
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            Global variable 
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           Global function declaration
*********************************************************************************************************
*/
void Enable_emu_module(void);
void Disable_emu_module(void);
void Open_emu_LDO(void);
void Close_emu_LDO(void);
void Reset_EMU_ECA_Regs(void);
void Reset_EMU_Module(void);
void Set_EMU_Reactive_Phase_Gain(uint8_t Freq); 

ITStatus HT_EMUSR_FlagStatusGet(uint16_t ITFlag);
void HT_EMU_ITConfig(uint32_t ITEn, FunctionalState NewState);
ITStatus HT_EMU_ITFlagStatusGet(uint32_t ITFlag);
void HT_EMU_ClearITPendingBit(uint32_t ITFlag);
uint32_t Read_ECR_ChksumReg(void);
uint32_t Read_EPR_Reg(uint16_t address);
uint16_t Read_ECR_Reg(uint16_t address);
void Write_ECR_Reg(uint16_t address, uint16_t data);
void Select_Pulse_Port(PFPort Port, PFType Type, PFWide PulseWidth);
void Set_Valid_Channel(uint16_t ChOn, uint16_t ChGain );
void HT_EMU_Init(void);
void Start_AdcBuff(void);
void HT_EMU_Sag_Peak_init(SagPeak_InitTypeDef SagPeakInit);
uint32_t Get_HFConst_value_ct(uint32_t meter_EC, uint32_t U_R1, uint32_t U_R2, uint32_t U_apga, uint32_t I_R, uint32_t CT_ratio, uint32_t APga);
uint32_t Get_HFConst_value_Shunt(uint32_t meter_EC, uint32_t U_R1, uint32_t U_R2, uint32_t U_apga, uint32_t I_R, uint32_t APga);


#endif                                               /* This File Only support HT501x */

#ifdef __cplusplus
}
#endif

#endif /* __HT50XX_EMU_H__ */ 

