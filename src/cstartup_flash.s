
//---------------------------------------------------------------------------
// cstartup_flash.s
//---------------------------------------------------------------------------
#include <A_m40800.h>

#define NO_INT					0xC0

//---------------------------------------------------------------------------
//- Stack sizes definition
//---------------------------------------------------------------------------
#define IRQ_STACK_SIZE			(0x800)		// ? words
#define FIQ_STACK_SIZE			(3*4)		// 3 words
#define ABT_STACK_SIZE			(1*4)		// 1 word
#define UND_STACK_SIZE			(1*4)		// 1 word

//---------------------------------------------------------------------------
//- Top of Stack Definition
//---------------------------------------------------------------------------
#define TOP_EXP_STACK			(0x2000-4)	// Defined in part

	.section    ".reset"
	.text
	.align

//---------------------------------------------------------------------------
//- Exception vectors ( before Remap )
//---------------------------------------------------------------------------
_start:
	b			InitReset					// reset
undefvec:
	b			undefvec					// Undefined Instruction
swivec:
	b			swivec						// Software Interrupt
pabtvec:
	b			pabtvec						// Prefetch Abort
dabtvec:
	b			dabtvec						// Data Abort
rsvdvec:
	b			rsvdvec						// reserved
irqvec:
	b			irqvec						// reserved
fiqvec:
	b			fiqvec						// reserved
	
//---------------------------------------------------------------------------
//- Macros for interrupt
//---------------------------------------------------------------------------
// 中断进入，在 IRQ 中执行
	.extern  	OSIntNesting
	.extern		IRQ_Exception_PC
	
.macro IRQ_ENTRY
	//- Adjust and save LR_irq in IRQ stack
	sub			r14, r14, #4
    stmfd       sp!, {r14}
	
	//- Save SPSR in IRQ stack
    mrs         r14, SPSR
    stmfd       sp!, {r14}

	//- Save scratch/used registers and LR
	stmfd		sp!, {r0-r12}

	// 保存 PC 到内存
	ldr			r0, =IRQ_Exception_PC			// PC --> IRQ_Exception_PC
	str			lr, [r0]
	
	// 增加 OSIntNesting
	ldr     	r0, =OSIntNesting               // OSIntNesting++;
	ldrb    	r1, [r0]
	add     	r1, r1, #1
	strb    	r1, [r0]
.endm

.macro IRQ_EXIT
	// 减少 OSIntNesting
	ldr     	r0, =OSIntNesting				// OSIntNesting--;
	ldrb    	r1, [r0]
	sub     	r1, r1, #1
	strb    	r1, [r0]
	
	//- Mark the End of Interrupt on the AIC
	ldr    		r0, =AIC_BASE
	str     	r0, [r0, #AIC_EOICR]

	//- Restore scratch/used registers and LR
	ldmia		sp!, {r0-r12}

	//- Restore SPSR_irq from IRQ stack
	ldmia		sp!, {r14}
	msr        	SPSR ,r14

	//- Restore adjusted  LR_irq from IRQ stack directly in the PC
	ldmia      	sp!, {pc}^
.endm

//---------------------------------------------------------------------------
//- EBI Initialization Data
//---------------------------------------------------------------------------
InitTableEBI:
	.long		EBI_CSR_0  
	.long		EBI_CSR_1  
	.long		EBI_CSR_2  
	.long		EBI_CSR_3  
	.long		EBI_CSR_4  
	.long		EBI_CSR_5  
	.long		EBI_CSR_6  
	.long		EBI_CSR_7  
	.long		0x00000001
	.long		0x00000005  

PtEBIBase:
	.long		EBI_BASE

//---------------------------------------------------------------------------
//- Speed up the Boot sequence
//---------------------------------------------------------------------------

InitReset:
	//---------------------------------------------------------------------------
	//- Speed up the Boot sequence
	//---------------------------------------------------------------------------
	//- Load System EBI Base address and CSR0 Init Value
	ldr			r0, PtEBIBase
	ldr			r1, [pc,#-(8+.-InitTableEBI)] 
	
	//- Speed up code execution by disabling wait state on Chip Select 0
	str			r1, [r0]

	//---------------------------------------------------------------------------
	//- Advanced Interrupt Controller configuration
	//---------------------------------------------------------------------------
	//- Set up the default vectors
	//---------------------------------------------------------------------------
	//- Load the AIC Base Address and the default handler addresses
	add			r0, pc,#-(8+.-AicData)		// @ where to read values (relative)
	ldmia		r0, {r1-r4}

	//- Setup the Spurious Vector
	str			r4, [r1, #AIC_SPU]			// r4 =spurious handler

	// test code
	ldr			r0,=0x000701ff
	str			r0,[r1, #0x124]  

	//- Set up the default interrupt handler vectors
	str			r2, [r1, #AIC_SVR]			// SVR[0] for FIQ
	add			r1, r1, #AIC_SVR
	mov			r0, #31						// counter

LoopAic1:
	str			r3, [r1, r0, LSL #2]		// SVRs for IRQs
	subs		r0, r0, #1					// do not save FIQ
	bhi			LoopAic1
	b			EndInitAic
	
	
//---------------------------------------------------------------------------
//- Default Interrupt Handler
//---------------------------------------------------------------------------
AicData:
	.long		AIC_BASE					// AIC Base Address
	
	.global		at91_default_fiq_handler
	.global		at91_default_irq_handler
	.global		at91_spurious_handler

PtDefaultHandler:
	.long		at91_default_fiq_handler
	.long		at91_default_irq_handler
	.long		at91_spurious_handler

at91_default_fiq_handler:
	b			at91_default_fiq_handler
	
at91_spurious_handler:
	b			at91_spurious_handler
	
at91_default_irq_handler:
	IRQ_ENTRY								// save registers
	bl irq_manager							// call irq manager
	IRQ_EXIT								// restore registers
	
EndInitAic:
	//---------------------------------------------------------------------------
	//-Setup Exception Vectors in Internal RAM before Remap
	//---------------------------------------------------------------------------
	mov			r8, #RAM_BASE				// @ of the hard vector in RAM BASE
	add			r9, pc,#-(8+.-VectorTable)	// @ where to read values (relative)
	ldmia		r9!, {r0-r7}				// read 8 vectors
	stmia		r8!, {r0-r7}				// store them on RAM
	ldmia		r9!, {r0-r4}				// read 5 absolute handler addresses
	stmia		r8!, {r0-r4}				// store them on RAM
	//---------------------------------------------------------------------------
	//- Memory Controller Initialization
	//---------------------------------------------------------------------------
	//- Copy the Image of the Memory Controller
	sub			r10, pc,#(8+.-InitTableEBI)	// get the address of the chip select register image
	ldr			r12, PtInitRemap			// get the real jump address ( after remap )
	//- Copy Chip Select Register Image to Memory Controller and command remap
	ldmia		r10!, {r0-r9,r11}			// load the complete image and the EBI base
	stmia		r11!, {r0-r9}				// store the complete image with the remap command
	//- Jump to ROM at its new address
	mov			pc, r12						// jump and break the pipeline
	
VectorTable:
	ldr			pc, [pc, #0x18]				// SoftReset
	ldr			pc, [pc, #0x18]				// UndefHandler
	ldr			pc, [pc, #0x18]				// SWIHandler
	ldr			pc, [pc, #0x18]				// PrefetchAbortHandler
	ldr			pc, [pc, #0x18]				// DataAbortHandler
	nop 									// Reserved
	ldr			pc, [pc,#-0xF20]			// IRQ : read the AIC
	ldr			pc, [pc,#-0xF20]			// FIQ : read the AIC
	
//- There are only 5 offsets as the vectoring is used.
	.long		SoftReset
	.long		UndefHandler
	.long		SWIHandler
	.long		PrefetchAbortHandler
	.long		DataAbortHandler
	
//- Vectoring Execution function run at absolute address
SoftReset:
	b			soft_reset_handler
UndefHandler:
	b			undef_handler
SWIHandler:
	b			swi_handler
PrefetchAbortHandler:
	b			prefetch_abort_handler
DataAbortHandler:
	b			data_abort_handler
	
//---------------------------------------------------------------------------
//- Exceptions Handler
//---------------------------------------------------------------------------
soft_reset_handler:
	IRQ_ENTRY								// save registers
	bl cpu_soft_reset_handler				// call cpu_soft_reset_handler
	IRQ_EXIT								// restore registers
	
undef_handler:
	IRQ_ENTRY								// save registers
	bl cpu_undef_handler					// call cpu_undef_handler
	IRQ_EXIT								// restore registers

swi_handler:
	IRQ_ENTRY								// save registers
	bl cpu_swi_handler						// call cpu_swi_handler
	IRQ_EXIT								// restore registers

prefetch_abort_handler:
	IRQ_ENTRY								// save registers
	bl cpu_prefetch_abort_handler			// call cpu_prefetch_abort_handler
	IRQ_EXIT								// restore registers

data_abort_handler:
	IRQ_ENTRY								// save registers
	bl cpu_data_abort_handler				// call cpu_data_abort_handler
	IRQ_EXIT								// restore registers
	
PtInitRemap:
	.long		InitRemap					// address where to jump after REMAP
	
//---------------------------------------------------------------------------
//- From here, the code is executed from its link address, ie. 0x100 0000.
//---------------------------------------------------------------------------
InitRemap:
	//---------------------------------------------------------------------------
	//- Setup stack for each mode
	//---------------------------------------------------------------------------
	ldr			r0, =TOP_EXP_STACK
	//- Set up Fast Interrupt Mode and set FIQ Mode Stack
	msr			CPSR_c, #(ARM_MODE_FIQ | I_BIT | F_BIT)
	mov			r13, r0						// Init stack FIQ
	sub			r0, r0, #FIQ_STACK_SIZE
	//- Set up Interrupt Mode and set IRQ Mode Stack
	msr			CPSR_c, #(ARM_MODE_IRQ | I_BIT | F_BIT)
	mov			r13, r0						// Init stack IRQ
	sub			r0, r0, #IRQ_STACK_SIZE
	//- Set up Abort Mode and set Abort Mode Stack
	msr			CPSR_c, #(ARM_MODE_ABORT | I_BIT | F_BIT)
	mov			r13, r0						// Init stack Abort
	sub			r0, r0, #ABT_STACK_SIZE
	//- Set up Undefined Instruction Mode and set Undef Mode Stack
	msr			CPSR_c, #(ARM_MODE_UNDEF | I_BIT | F_BIT)
	mov			r13, r0						// Init stack Undef
	sub			r0, r0, #UND_STACK_SIZE
	//- Set up Supervisor Mode and set Supervisor Mode Stack
	msr			CPSR_c, #ARM_MODE_SVC
	mov			r13, r0
	
	//---------------------------------------------------------------------------
	//- Initialize DATA and BSS section
	//---------------------------------------------------------------------------
	.global		Image_RO_Limit      
	.global		Image_RW_Base   
	.global		Image_ZI_Base  
	.global		Image_ZI_Limit   

	ldr			r0, =Image_RO_Limit
	ldr			r1, =Image_RW_Base
	ldr			r3, =Image_ZI_Base
	cmp			r0, r1
	beq			NoRW
LoopRw:
	cmp			r1, r3
	ldrcc		r2, [r0], #4
	strcc		r2, [r1], #4
	bcc			LoopRw
NoRW:
	ldr			r1, =Image_ZI_Limit
	mov			r2, #0
LoopZI:
	cmp			r3, r1
	strcc		r2, [r3], #4
	bcc			LoopZI

	//---------------------------------------------------------------------------
	//- Branch on C code Main function (with interworking)
	//---------------------------------------------------------------------------
	.global		__main
	ldr			r0, =__main
	bx			r0

// --- End of cstartup_flash.s ---
