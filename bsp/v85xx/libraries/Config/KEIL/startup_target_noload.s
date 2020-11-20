;/**
;* @file    startup_target_noload.s
;* @author  Application Team
;* @version V4.3.0
;* @date    2018-09-04
;* @brief   Target Devices vector table.
;******************************************************************************/


; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x000000400

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000200

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; External Interrupts
                DCD     PMU_IRQHandler            ;  0:  PMU
                DCD     RTC_IRQHandler            ;  1:  RTC
                DCD     U32K0_IRQHandler          ;  2:  U32K0
                DCD     U32K1_IRQHandler          ;  3:  U32K1
                DCD     I2C_IRQHandler            ;  4:  I2C
                DCD     SPI1_IRQHandler           ;  5:  SPI1
                DCD     UART0_IRQHandler          ;  6:  UART0
                DCD     UART1_IRQHandler          ;  7:  UART1
                DCD     UART2_IRQHandler          ;  8:  UART2
                DCD     UART3_IRQHandler          ;  9:  UART3
                DCD     UART4_IRQHandler          ; 10:  UART4
                DCD     UART5_IRQHandler          ; 11:  UART5
                DCD     ISO78160_IRQHandler       ; 12:  ISO78160
                DCD     ISO78161_IRQHandler       ; 13:  ISO78161
                DCD     TMR0_IRQHandler           ; 14:  TMR0
                DCD     TMR1_IRQHandler           ; 15:  TMR1
                DCD     TMR2_IRQHandler           ; 16:  TMR2
                DCD     TMR3_IRQHandler           ; 17:  TMR3
                DCD     PWM0_IRQHandler           ; 18:  PWM0
                DCD     PWM1_IRQHandler           ; 19:  PWM1
                DCD     PWM2_IRQHandler           ; 20:  PWM2
                DCD     PWM3_IRQHandler           ; 21:  PWM3
                DCD     DMA_IRQHandler            ; 22:  DMA
                DCD     FLASH_IRQHandler          ; 23:  FLASH
                DCD     ANA_IRQHandler            ; 24:  ANA
                DCD     0                         ; 25:  Reserved
                DCD     0                         ; 26:  Reserved
                DCD     SPI2_IRQHandler           ; 27:  SPI2
                DCD     0                         ; 28:  Reserved
                DCD     0                         ; 29:  Reserved
                DCD     0                         ; 30:  Reserved
                DCD     0                         ; 31:  Reserved
__Vectors_End

__Vectors_Size  EQU     __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  __main
                LDR     R0, =__main
                BX      R0
                ENDP


; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler           [WEAK]
                B       .
                ENDP

Default_Handler PROC
								
                EXPORT  PMU_IRQHandler            [WEAK]
                EXPORT  RTC_IRQHandler            [WEAK]
                EXPORT  U32K0_IRQHandler          [WEAK]
                EXPORT  U32K1_IRQHandler          [WEAK]
                EXPORT  I2C_IRQHandler            [WEAK]
                EXPORT  SPI1_IRQHandler           [WEAK]
                EXPORT  UART0_IRQHandler          [WEAK]
                EXPORT  UART1_IRQHandler          [WEAK]
                EXPORT  UART2_IRQHandler          [WEAK]
                EXPORT  UART3_IRQHandler          [WEAK]
                EXPORT  UART4_IRQHandler          [WEAK]
                EXPORT  UART5_IRQHandler          [WEAK]
                EXPORT  ISO78160_IRQHandler       [WEAK]
                EXPORT  ISO78161_IRQHandler       [WEAK]
                EXPORT  TMR0_IRQHandler           [WEAK]
                EXPORT  TMR1_IRQHandler           [WEAK]
                EXPORT  TMR2_IRQHandler           [WEAK]
                EXPORT  TMR3_IRQHandler           [WEAK]
                EXPORT  PWM0_IRQHandler           [WEAK]
                EXPORT  PWM1_IRQHandler           [WEAK]
                EXPORT  PWM2_IRQHandler           [WEAK]
                EXPORT  PWM3_IRQHandler           [WEAK]
                EXPORT  DMA_IRQHandler            [WEAK]
                EXPORT  FLASH_IRQHandler      	  [WEAK]
                EXPORT  ANA_IRQHandler            [WEAK]
                EXPORT  SPI2_IRQHandler           [WEAK]

PMU_IRQHandler
RTC_IRQHandler
U32K0_IRQHandler
U32K1_IRQHandler
I2C_IRQHandler
SPI1_IRQHandler
UART0_IRQHandler
UART1_IRQHandler
UART2_IRQHandler
UART3_IRQHandler
UART4_IRQHandler
UART5_IRQHandler
ISO78160_IRQHandler
ISO78161_IRQHandler
TMR0_IRQHandler
TMR1_IRQHandler
TMR2_IRQHandler
TMR3_IRQHandler
PWM0_IRQHandler
PWM1_IRQHandler
PWM2_IRQHandler
PWM3_IRQHandler
DMA_IRQHandler
FLASH_IRQHandler
ANA_IRQHandler
SPI2_IRQHandler
                B       .

                ENDP


                ALIGN


; User Initial Stack & Heap

                IF      :DEF:__MICROLIB

                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap

__user_initial_stackheap PROC
                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR
                ENDP

                ALIGN

                ENDIF


                END
