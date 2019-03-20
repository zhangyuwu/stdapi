/*
;********************************************************************************************************
;                                               uC/OS-II
;                                         The Real-Time Kernel
;
;                               (c) Copyright 1992-2004, Micrium, Weston, FL
;                                          All Rights Reserved
;
;                                           Generic ARM Port
;
; File      : OS_CPU_A.ASM
; Version   : V1.60
; By        : Jean J. Labrosse
;
; For       : ARM7 or ARM9
; Mode      : ARM or Thumb
; Toolchain : IAR's EWARM V4.11a and higher
;********************************************************************************************************
*/

            .extern  OSRunning                    // External references
            .extern  OSPrioCur
            .extern  OSPrioHighRdy
            .extern  OSTCBCur
            .extern  OSTCBHighRdy
            .extern  OSIntNesting
            .extern  OSIntExit
            .extern  OSTaskSwHook
            .extern  OS_CPU_IRQ_ISR_Handler
            .extern  OS_CPU_FIQ_ISR_Handler

            .global  OSStartHighRdy               
            .global  OSCtxSw
            .global  OSIntCtxSw
            .global  OS_CPU_IRQ_ISR
            .global  OS_CPU_FIQ_ISR


#define 	AIC_EOICR			0x130
#define 	AIC_BASE           	0xFFFFF000

NO_INT      =     0xC0                            // Mask used to disable interrupts (Both FIR and IRQ)
SVC32_MODE  =     0x13
FIQ32_MODE  =     0x11
IRQ32_MODE  =     0x12

/*
;*********************************************************************************************************
; 2005-5-13 16:41:31
; Port to ATMEL's ARM CPU
; KEN
;*********************************************************************************************************
*/

.MACRO EnableAIC                            // 调用 AIC 结束，允许下次中断
	ldr     r0, =AIC_BASE
	str     r0, [r0, #AIC_EOICR]
.ENDM

    .global ARMDisableInt
ARMDisableInt:
    MRS 	r0, cpsr
    ORR 	r0, r0, #NO_INT
    MSR 	cpsr, r0
	MRS 	r0, cpsr
	AND 	r0, r0, #NO_INT
	CMP 	r0, #NO_INT
	BNE 	ARMDisableInt
    MOV 	pc, lr

    .global ARMEnableInt
ARMEnableInt:
    MRS 	r0, cpsr
    BIC 	r0, r0, #NO_INT
    MSR 	cpsr, r0
    MOV 	pc, lr


/*
;*********************************************************************************************************
;                                          START MULTITASKING
;                                       void OSStartHighRdy(void)
;
; Note(s) : 1) OSStartHighRdy() MUST:
;              a) Call OSTaskSwHook() then,
;              b) Set OSRunning to TRUE,
;              c) Switch to the highest priority task.
;*********************************************************************************************************
*/

OSStartHighRdy  :

        MSR     CPSR_cxsf, #0xD3        // Switch to SVC mode with IRQ and FIQ disabled

        LDR     R0, =OSTaskSwHook       // OSTaskSwHook();
        MOV     LR, PC
        BX      R0            

        LDR     R4, =OSRunning          // OSRunning = TRUE
        MOV     R5, #1
        STRB    R5, [R4]

                                        // SWITCH TO HIGHEST PRIORITY TASK
        LDR     R4, =OSTCBHighRdy       //    Get highest priority task TCB address
        LDR     R4, [R4]                //    get stack pointer
        LDR     SP, [R4]                //    switch to the new stack

        LDMFD   SP!, {R4}               //    Pop new task's CPSR
        MSR     SPSR_cxsf,R4
        LDMFD   SP!, {R0-R12,LR,PC}^    //    pop new task's context


/*
;*********************************************************************************************************
;                         PERFORM A CONTEXT SWITCH (From task level) - OSCtxSw()
;
; Note(s) : 1) OSCtxSw() is called in SYS mode with BOTH FIQ and IRQ interrupts DISABLED
;
;           2) The pseudo-code for OSCtxSw() is:
;              a) Save the current task's context onto the current task's stack
;              b) OSTCBCur->OSTCBStkPtr = SP;
;              c) OSTaskSwHook();
;              d) OSPrioCur             = OSPrioHighRdy;
;              e) OSTCBCur              = OSTCBHighRdy;
;              f) SP                    = OSTCBHighRdy->OSTCBStkPtr;
;              g) Restore the new task's context from the new task's stack
;              h) Return to new task's code
;
;           3) Upon entry: 
;              OSTCBCur      points to the OS_TCB of the task to suspend
;              OSTCBHighRdy  points to the OS_TCB of the task to resume
;*********************************************************************************************************
*/

/*
;*********************************************************************************************************
;                                      任务级别切换
;*********************************************************************************************************
*/

OSCtxSw:
                                        // SAVE CURRENT TASK'S CONTEXT
        STMFD   SP!, {LR}               //     Push return address
        STMFD   SP!, {LR}
        STMFD   SP!, {R0-R12}           //     Push registers
        MRS     R4,  CPSR               //     Push current CPSR
        TST     LR, #1                  //     See if called from Thumb mode
        ORRNE   R4,  R4, #0x20          //     If yes, Set the T-bit
        STMFD   SP!, {R4}
        
        LDR     R4, =OSTCBCur           // OSTCBCur->OSTCBStkPtr = SP;
        LDR     R5, [R4]
        STR     SP, [R5]

        LDR     R0, =OSTaskSwHook       // OSTaskSwHook();
        MOV     LR, PC
        BX      R0

        LDR     R4, =OSPrioCur          // OSPrioCur = OSPrioHighRdy
        LDR     R5, =OSPrioHighRdy
        LDRB    R6, [R5]
        STRB    R6, [R4]

        LDR     R4, =OSTCBCur           // OSTCBCur  = OSTCBHighRdy;
        LDR     R5, =OSTCBHighRdy
        LDR     R6, [R5]
        STR     R6, [R4]

        LDR     SP, [R6]                // SP = OSTCBHighRdy->OSTCBStkPtr;

                                        // RESTORE NEW TASK'S CONTEXT
        LDMFD   SP!, {R4}               //    Pop new task's CPSR
        MSR     SPSR_cxsf, R4
        
        LDMFD   SP!, {R0-R12,LR,PC}^    //    Pop new task's context


/*
;*********************************************************************************************************
;                   PERFORM A CONTEXT SWITCH (From interrupt level) - OSIntCtxSw()
;
; Note(s) : 1) OSIntCtxSw() is called in SYS mode with BOTH FIQ and IRQ interrupts DISABLED
;
;           2) The pseudo-code for OSCtxSw() is:
;              a) OSTaskSwHook();
;              b) OSPrioCur             = OSPrioHighRdy;
;              c) OSTCBCur              = OSTCBHighRdy;
;              d) SP                    = OSTCBHighRdy->OSTCBStkPtr;
;              e) Restore the new task's context from the new task's stack
;              f) Return to new task's code
;
;           3) Upon entry: 
;              OSTCBCur      points to the OS_TCB of the task to suspend
;              OSTCBHighRdy  points to the OS_TCB of the task to resume
;*********************************************************************************************************
*/

 
/*
;*********************************************************************************************************
;                                      中断级别切换
;*********************************************************************************************************
*/

OSIntCtxSw:
        LDR     R0, =OSTaskSwHook       // OSTaskSwHook();
        MOV     LR, PC
        BX      R0            

        LDR     R4, =OSPrioCur          // OSPrioCur = OSPrioHighRdy
        LDR     R5, =OSPrioHighRdy
        LDRB    R6,[R5]
        STRB    R6,[R4]
        
        LDR     R4, =OSTCBCur           // OSTCBCur  = OSTCBHighRdy;
        LDR     R5, =OSTCBHighRdy
        LDR     R6,[R5]
        STR     R6,[R4]
		
        LDR     SP,[R6]                 // SP = OSTCBHighRdy->OSTCBStkPtr;

                                        // RESTORE NEW TASK'S CONTEXT
        LDMFD   SP!, {R4}               //    Pop new task's CPSR
        MSR     SPSR_cxsf, R4
        
		EnableAIC
		
        LDMFD   SP!, {R0-R12,LR,PC}^    //    Pop new task's context


/*
;*********************************************************************************************************
;                                      IRQ 中断服务程序
;*********************************************************************************************************
*/

OS_CPU_IRQ_ISR:

        STMFD   SP!, {R1-R3}                   // PUSH WORKING REGISTERS ONTO IRQ STACK                     
        
        MOV     R1, SP                         // Save   IRQ stack pointer
        
        ADD     SP, SP,#12                     // Adjust IRQ stack pointer 
        
        SUB     R2, LR,#4                      // Adjust PC for return address to task

        MRS     R3, SPSR                       // Copy SPSR (i.e. interrupted task's CPSR) to R3
        
        MSR     CPSR_c, #(NO_INT | SVC32_MODE) // Change to SVC mode

                                               // SAVE TASK'S CONTEXT ONTO TASK'S STACK
        STMFD   SP!, {R2}                      //    Push task's Return PC
        STMFD   SP!, {LR}                      //    Push task's LR
        STMFD   SP!, {R4-R12}                  //    Push task's R12-R4
        
        LDMFD   R1!, {R4-R6}                   //    Move task's R1-R3 from IRQ stack to SVC stack
        STMFD   SP!, {R4-R6}
        STMFD   SP!, {R0}                      //    Push task's R0    onto task's stack
        STMFD   SP!, {R3}                      //    Push task's CPSR (i.e. IRQ's SPSR)
        
                                               // HANDLE NESTING COUNTER
        LDR     R0, =OSIntNesting              // OSIntNesting++;
        LDRB    R1, [R0]
        ADD     R1, R1,#1
        STRB    R1, [R0]

                                               // 在没有中断嵌套的情况下保存当前任务的运行环境为切换做准备
        CMP     R1, #1                         // if (OSIntNesting == 1) {
        BNE     OS_CPU_IRQ_ISR_1

        LDR     R4, =OSTCBCur                  //     OSTCBCur->OSTCBStkPtr = SP
        LDR     R5, [R4]
        STR     SP, [R5]                       // }

OS_CPU_IRQ_ISR_1:
        MSR     CPSR_c, #(NO_INT | IRQ32_MODE) // Change to IRQ mode (to use the IRQ stack to handle interrupt)
        
        LDR     R0, =OS_CPU_IRQ_ISR_Handler    // OS_CPU_IRQ_ISR_Handler();
        MOV     LR, PC
        BX      R0            
        
        MSR     CPSR_c, #(NO_INT | SVC32_MODE) // Change to SVC mode
        
		BL		OSTimeTick					   // OSTimeTick();
		
        LDR     R0, =OSIntExit                 // OSIntExit();
        MOV     LR, PC
        BX      R0            

                                               // RESTORE NEW TASK'S CONTEXT
        LDMFD   SP!, {R4}                      //    Pop new task's CPSR
        MSR     SPSR_cxsf, R4
        
		EnableAIC

        LDMFD   SP!, {R0-R12,LR,PC}^           //    Pop new task's context

// --- END ---
