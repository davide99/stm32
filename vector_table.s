.syntax unified
.cpu cortex-m3
.fpu softvfp
.thumb

//Global memory locations
.global vtable
.global default_interrupt_handler

//Vector table
.type vtable, %object
.section .vector_table, "a", %progbits
vtable:
    .word _estack                       //Initial SP value
    .word reset_handler
    .word nmi_handler
    .word hard_fault_handler            //All class of cault
    .word mem_manage_handler            //MPU fault handler
    .word bus_fault_handler             //Bus fault handler
    .word usage_fault_handler
    .word 0                             //Reserved
    .word 0                             //Reserved
    .word 0                             //Reserved
    .word 0                             //Reserved
    .word SVC_handler                   //SVCall handler
    .word debugmon_handler              //Debug monitor handler
    .word 0                             //Reserved
    .word pending_SV_handler            //Pending request for system service
    .word SysTick_handler               //System tick timer
    .word WWDG_IRQ_handler              //Window watchdag
    .word PVD_IRQ_handler               //PVD through EXTI line detect
    .word tamper_IRQ_handler
    .word RTC_IRQ_handler
    .word flash_IRQ_handler
    .word RCC_IRQ_handler
    .word EXTI0_IRQ_handler             //EXTI Line 0
    .word EXTI1_IRQ_handler             //EXTI Line 1
    .word EXTI2_IRQ_handler             //EXTI Line 2
    .word EXTI3_IRQ_handler             //EXTI Line 3
    .word EXTI4_IRQ_handler             //EXTI Line 4
    .word DMA1_chan1_IRQ_handler        //DMA1 channel 1
    .word DMA1_chan2_IRQ_handler        //DMA1 channel 2
    .word DMA1_chan3_IRQ_handler        //DMA1 channel 3
    .word DMA1_chan4_IRQ_handler        //DMA1 channel 4
    .word DMA1_chan5_IRQ_handler        //DMA1 channel 5
    .word DMA1_chan6_IRQ_handler        //DMA1 channel 6
    .word DMA1_chan7_IRQ_handler        //DMA1 channel 7
    .word ADC1_2_IRQ_handler
    .word USB_HP_CAN1_TX_IRQ_handler    //USB high priority or CAN1 TX
    .word USB_LP_CAN1_RX0_IRQ_handler   //USB low priority or CAN1 RX0
    .word CAN1_RX1_IRQ_handler          //CAN1 RX1
    .word CAN1_SCE_IRQ_handler          //CAN1 SCE
    .word EXTI9_5_IRQ_handler           //EXTI Line 9..5
    .word TIM1_BRK_IRQ_handler          //TIM1 Break
    .word TIM1_UP_IRQ_handler           //TIM1 Update
    .word TIM1_TRG_COM_IRQ_handler      //TIM1 Trigger and Commutation
    .word TIM1_TRG_CC_handler           //TIM1 Capture Compare
    .word TIM2_IRQ_handler              //TIM2
    .word TIM3_IRQ_handler              //TIM3
    .word TIM4_IRQ_handler              //TIM4
    .word I2C1_Ev_IRQ_handler           //I2C1 Event
    .word I2C1_Er_IRQ_handler           //I2C1 Error
    .word I2C2_Ev_IRQ_handler           //I2C2 Event
    .word I2C2_Er_IRQ_handler           //I2C2 Error
    .word SPI1_IRQ_handler
    .word SPI2_IRQ_handler
    .word USART1_IRQ_handler
    .word USART2_IRQ_handler
    .word USART3_IRQ_handler
    .word EXTI15_10_IRQ_handler         //EXTI Line 15..10
    .word RTC_Alarm_IRQ_handler         //RTC Alarm through EXTI Line
    .word USB_wakeup_IRQ_handler        //USB wakeup from suspend
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0

    .equ boot_ram_base, 0xF108F85F
    .word boot_ram_base                 //@ 0x108

    /*
     * Setup weak aliases for each exception handler to
     * the default one. reset_handler is set up reparately
     */

    .weak nmi_handler
    .thumb_set nmi_handler, default_interrupt_handler
    .weak hard_fault_handler
    .thumb_set hard_fault_handler, default_interrupt_handler
    .weak mem_manage_handler
    .thumb_set mem_manage_handler, default_interrupt_handler
    .weak bus_fault_handler
    .thumb_set bus_fault_handler, default_interrupt_handler
    .weak usage_fault_handler
    .thumb_set usage_fault_handler, default_interrupt_handler
    .weak SVC_handler
    .thumb_set SVC_handler, default_interrupt_handler
    .weak debugmon_handler
    .thumb_set debugmon_handler, default_interrupt_handler
    .weak pending_SV_handler
    .thumb_set pending_SV_handler, default_interrupt_handler
    .weak SysTick_handler
    .thumb_set SysTick_handler, default_interrupt_handler
    .weak WWDG_IRQ_handler
    .thumb_set WWDG_IRQ_handler, default_interrupt_handler
    .weak PVD_IRQ_handler
    .thumb_set PVD_IRQ_handler, default_interrupt_handler
    .weak tamper_IRQ_handler
    .thumb_set tamper_IRQ_handler, default_interrupt_handler
    .weak RTC_IRQ_handler
    .thumb_set RTC_IRQ_handler, default_interrupt_handler
    .weak flash_IRQ_handler
    .thumb_set flash_IRQ_handler, default_interrupt_handler
    .weak RCC_IRQ_handler
    .thumb_set RCC_IRQ_handler, default_interrupt_handler
    .weak EXTI0_IRQ_handler
    .thumb_set EXTI0_IRQ_handler, default_interrupt_handler
    .weak EXTI1_IRQ_handler
    .thumb_set EXTI1_IRQ_handler, default_interrupt_handler
    .weak EXTI2_IRQ_handler
    .thumb_set EXTI2_IRQ_handler, default_interrupt_handler
    .weak EXTI3_IRQ_handler
    .thumb_set EXTI3_IRQ_handler, default_interrupt_handler
    .weak EXTI4_IRQ_handler
    .thumb_set EXTI4_IRQ_handler, default_interrupt_handler
    .weak DMA1_chan1_IRQ_handler
    .thumb_set DMA1_chan1_IRQ_handler, default_interrupt_handler
    .weak DMA1_chan2_IRQ_handler
    .thumb_set DMA1_chan2_IRQ_handler, default_interrupt_handler
    .weak DMA1_chan3_IRQ_handler
    .thumb_set DMA1_chan3_IRQ_handler, default_interrupt_handler
    .weak DMA1_chan4_IRQ_handler
    .thumb_set DMA1_chan4_IRQ_handler, default_interrupt_handler
    .weak DMA1_chan5_IRQ_handler
    .thumb_set DMA1_chan5_IRQ_handler, default_interrupt_handler
    .weak DMA1_chan6_IRQ_handler
    .thumb_set DMA1_chan6_IRQ_handler, default_interrupt_handler
    .weak DMA1_chan7_IRQ_handler
    .thumb_set DMA1_chan7_IRQ_handler, default_interrupt_handler
    .weak ADC1_2_IRQ_handler
    .thumb_set ADC1_2_IRQ_handler, default_interrupt_handler
    .weak USB_HP_CAN1_TX_IRQ_handler
    .thumb_set USB_HP_CAN1_TX_IRQ_handler, default_interrupt_handler
    .weak USB_LP_CAN1_RX0_IRQ_handler
    .thumb_set USB_LP_CAN1_RX0_IRQ_handler, default_interrupt_handler
    .weak CAN1_RX1_IRQ_handler
    .thumb_set CAN1_RX1_IRQ_handler, default_interrupt_handler
    .weak CAN1_SCE_IRQ_handler
    .thumb_set CAN1_SCE_IRQ_handler, default_interrupt_handler
    .weak EXTI9_5_IRQ_handler
    .thumb_set EXTI9_5_IRQ_handler, default_interrupt_handler
    .weak TIM1_BRK_IRQ_handler
    .thumb_set TIM1_BRK_IRQ_handler, default_interrupt_handler
    .weak TIM1_UP_IRQ_handler
    .thumb_set TIM1_UP_IRQ_handler, default_interrupt_handler
    .weak TIM1_TRG_COM_IRQ_handler
    .thumb_set TIM1_TRG_COM_IRQ_handler, default_interrupt_handler
    .weak TIM1_TRG_CC_handler
    .thumb_set TIM1_TRG_CC_handler, default_interrupt_handler
    .weak TIM2_IRQ_handler
    .thumb_set TIM2_IRQ_handler, default_interrupt_handler
    .weak TIM3_IRQ_handler
    .thumb_set TIM3_IRQ_handler, default_interrupt_handler
    .weak TIM4_IRQ_handler
    .thumb_set TIM4_IRQ_handler, default_interrupt_handler
    .weak I2C1_Ev_IRQ_handler
    .thumb_set I2C1_Ev_IRQ_handler, default_interrupt_handler
    .weak I2C1_Er_IRQ_handler
    .thumb_set I2C1_Er_IRQ_handler, default_interrupt_handler
    .weak I2C2_Ev_IRQ_handler
    .thumb_set I2C2_Ev_IRQ_handler, default_interrupt_handler
    .weak I2C2_Er_IRQ_handler
    .thumb_set I2C2_Er_IRQ_handler, default_interrupt_handler
    .weak SPI1_IRQ_handler
    .thumb_set SPI1_IRQ_handler, default_interrupt_handler
    .weak SPI2_IRQ_handler
    .thumb_set SPI2_IRQ_handler, default_interrupt_handler
    .weak USART1_IRQ_handler
    .thumb_set USART1_IRQ_handler, default_interrupt_handler
    .weak USART2_IRQ_handler
    .thumb_set USART2_IRQ_handler, default_interrupt_handler
    .weak USART3_IRQ_handler
    .thumb_set USART3_IRQ_handler, default_interrupt_handler
    .weak EXTI15_10_IRQ_handler
    .thumb_set EXTI15_10_IRQ_handler, default_interrupt_handler
    .weak RTC_Alarm_IRQ_handler
    .thumb_set RTC_Alarm_IRQ_handler, default_interrupt_handler
    .weak USB_wakeup_IRQ_handler
    .thumb_set USB_wakeup_IRQ_handler, default_interrupt_handler

.size vtable, .-vtable

//Default interrupt handler
.section .text.default_interrupt_handler, "ax", %progbits
default_interrupt_handler:
    default_interrupt_loop:
    B default_interrupt_loop
.size default_interrupt_handler, .-default_interrupt_handler
