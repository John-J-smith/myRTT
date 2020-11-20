;/**
;* @file    startup_target_noload.s
;* @author  Application Team
;* @version V4.3.0
;* @date    2018-09-04
;* @brief   Target Devices vector table.
;******************************************************************************/

        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)
        
        EXTERN  __iar_program_start
        PUBLIC  __vector_table

        DATA
__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler                  ; Reset Handler

        DCD     NMI_Handler                    ; NMI Handler
        DCD     HardFault_Handler              ; Hard Fault Handler
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     SVC_Handler                    ; SVCall Handler
        DCD     0                              ; Reserved
        DCD     0                              ; Reserved
        DCD     PendSV_Handler                 ; PendSV Handler
        DCD     SysTick_Handler                ; SysTick Handler

        ; External Interrupts
        DCD     PMU_IRQHandler                 ; PMU,           IRQ0
        DCD     RTC_IRQHandler                 ; RTC,           IRQ1
        DCD     U32K0_IRQHandler               ; UART 32K 0,    IRQ2
        DCD     U32K1_IRQHandler               ; UART 32K 1,    IRQ3
        DCD     I2C_IRQHandler                 ; I2C,           IRQ4
        DCD     SPI1_IRQHandler                ; SPI1,          IRQ5
        DCD     UART0_IRQHandler               ; UART0,         IRQ6
        DCD     UART1_IRQHandler               ; UART1,         IRQ7
        DCD     UART2_IRQHandler               ; UART2,         IRQ8
        DCD     UART3_IRQHandler               ; UART3,         IRQ9
        DCD     UART4_IRQHandler               ; UART4,         IRQ10
        DCD     UART5_IRQHandler               ; UART5,         IRQ11
        DCD     ISO78160_IRQHandler            ; ISO78160,      IRQ12
        DCD     ISO78161_IRQHandler            ; ISO78161,      IRQ13
        DCD     TMR0_IRQHandler                ; Timer 0,       IRQ14
        DCD     TMR1_IRQHandler                ; Timer 1,       IRQ15
        DCD     TMR2_IRQHandler                ; Timer 2,       IRQ16
        DCD     TMR3_IRQHandler                ; Timer 3,       IRQ17
        DCD     PWM0_IRQHandler                ; PWM0,          IRQ18
        DCD     PWM1_IRQHandler                ; PWM1,          IRQ19
        DCD     PWM2_IRQHandler                ; PWM2,          IRQ20
        DCD     PWM3_IRQHandler                ; PWM3,          IRQ21
        DCD     DMA_IRQHandler                 ; DMA,           IRQ22
        DCD     FLASH_IRQHandler               ; FLASH,         IRQ23
        DCD     ANA_IRQHandler                 ; ANA,           IRQ24
        DCD     0                              ; Reserved,      IRQ25
        DCD     0                              ; Reserved,      IRQ26
        DCD     SPI2_IRQHandler                ; SPI2,          IRQ27
        DCD     0                              ; Reserved,      IRQ28
        DCD     0                              ; Reserved,      IRQ29
        DCD     0                              ; Reserved,      IRQ30
        DCD     0                              ; Reserved,      IRQ31
        
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB
        
        PUBWEAK Reset_Handler
        SECTION .text:CODE:NOROOT:REORDER(2)
Reset_Handler
        LDR     R0, =__iar_program_start
        BX      R0
        
        
        PUBWEAK NMI_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
NMI_Handler
        B NMI_Handler
         
        
        PUBWEAK HardFault_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
HardFault_Handler
        B HardFault_Handler
       
        
        PUBWEAK SVC_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
SVC_Handler
        B SVC_Handler
       
        
        PUBWEAK PendSV_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
PendSV_Handler
        B PendSV_Handler
        
        
        PUBWEAK SysTick_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
SysTick_Handler
        B SysTick_Handler
        
        
        PUBWEAK PMU_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
PMU_IRQHandler
        B PMU_IRQHandler
        
                
        PUBWEAK RTC_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
RTC_IRQHandler
        B RTC_IRQHandler
        
                
        PUBWEAK U32K0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
U32K0_IRQHandler
        B U32K0_IRQHandler
        
                
        PUBWEAK U32K1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
U32K1_IRQHandler
        B U32K1_IRQHandler
        
                
        PUBWEAK I2C_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
I2C_IRQHandler
        B I2C_IRQHandler
        
                
        PUBWEAK SPI1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SPI1_IRQHandler
        B SPI1_IRQHandler
        
                
        PUBWEAK UART0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
UART0_IRQHandler
        B UART0_IRQHandler
        
                
        PUBWEAK UART1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
UART1_IRQHandler
        B UART1_IRQHandler
        
                
        PUBWEAK UART2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
UART2_IRQHandler
        B UART2_IRQHandler
        
                
        PUBWEAK UART3_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
UART3_IRQHandler
        B UART3_IRQHandler
        
                
        PUBWEAK UART4_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
UART4_IRQHandler
        B UART4_IRQHandler
        
                
        PUBWEAK UART5_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
UART5_IRQHandler
        B UART5_IRQHandler
        
                 
        PUBWEAK ISO78160_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
ISO78160_IRQHandler
        B ISO78160_IRQHandler
        
                
        PUBWEAK ISO78161_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
ISO78161_IRQHandler
        B ISO78161_IRQHandler
        
                
        PUBWEAK TMR0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TMR0_IRQHandler
        B TMR0_IRQHandler
        
                
        PUBWEAK TMR1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TMR1_IRQHandler
        B TMR1_IRQHandler
        
                
        PUBWEAK TMR2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TMR2_IRQHandler
        B TMR2_IRQHandler
        
                
        PUBWEAK TMR3_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
TMR3_IRQHandler
        B TMR3_IRQHandler
        
                
        PUBWEAK PWM0_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
PWM0_IRQHandler
        B PWM0_IRQHandler
        
                
        PUBWEAK PWM1_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
PWM1_IRQHandler
        B PWM1_IRQHandler
        
                
        PUBWEAK PWM2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
PWM2_IRQHandler
        B PWM2_IRQHandler
      
                
        PUBWEAK PWM3_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
PWM3_IRQHandler
        B PWM3_IRQHandler
        
                
        PUBWEAK DMA_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
DMA_IRQHandler
        B DMA_IRQHandler
        
                
        PUBWEAK FLASH_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
FLASH_IRQHandler
        B FLASH_IRQHandler
        
                
        PUBWEAK ANA_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
ANA_IRQHandler
        B ANA_IRQHandler
        
                
        PUBWEAK SPI2_IRQHandler
        SECTION .text:CODE:NOROOT:REORDER(1)
SPI2_IRQHandler
        B SPI2_IRQHandler

        END

/*********************************** END OF FILE ******************************/
