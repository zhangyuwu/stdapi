
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/

#include <C_m40800.h>
#include <types.h>
#include <system.h>
#include <serial.h>
#include <util.h>

// ----------------------------------------------------------------------------
#define HALT_WHEN_CPU_EXCEPTION		1		// 当 CPU 出现异常时系统进入死循环
#define IRQ_COUNT					32

// ----------------------------------------------------------------------------
void at91_default_irq_handler();
static irq_handler_t irq_table[IRQ_COUNT] = { NULL };

// ----------------------------------------------------------------------------
void irq_init()
{
	int i;

	for (i = 0; i < 32; i++) {
		AIC_BASE->AIC_SVR[i] = (unsigned int)at91_default_irq_handler;
	}
}

// ----------------------------------------------------------------------------
bool irq_register(int id, irq_handler_t irq_handler, bool raw)
{
	if (id >= 0 && id < IRQ_COUNT) {
		if (irq_table[id] == NULL) {
			irq_table[id] = irq_handler;
			if (raw) {
				AIC_BASE->AIC_SVR[id] = (unsigned int)irq_handler;
			}
			return true;
		}
	}
	return false;
}

// ----------------------------------------------------------------------------
void irq_set_priority(int id, int priority)
{
	if (id >= 0 && id < IRQ_COUNT) {
		AIC_BASE->AIC_SMR[id] &= ~0x00000007;
		AIC_BASE->AIC_SMR[id] |= (priority & 0x00000007);
	}
}

// ----------------------------------------------------------------------------
void irq_set_trigger_mode(int id, int mode)
{
	if (id >= 0 && id < IRQ_COUNT) {
		bit_tst(mode, 0) ? bit_set(AIC_BASE->AIC_SMR[id], 5) : bit_clr(AIC_BASE->AIC_SMR[id], 5);
		bit_tst(mode, 1) ? bit_set(AIC_BASE->AIC_SMR[id], 6) : bit_clr(AIC_BASE->AIC_SMR[id], 6);
	}
}

// ----------------------------------------------------------------------------
void irq_release(int id)
{	
	if (id >= 0 && id < IRQ_COUNT) {
		if (irq_table[id] != NULL) {
			irq_table[id] = NULL;
			AIC_BASE->AIC_SVR[id] = (unsigned int)at91_default_irq_handler;
		}
	}
}

// ----------------------------------------------------------------------------
void irq_enable(int id)
{
	AIC_BASE->AIC_ICCR = (1 << id);
	AIC_BASE->AIC_IECR = (1 << id);
}

// ----------------------------------------------------------------------------
void irq_disable(int id)
{
	AIC_BASE->AIC_IDCR = (1 << id);
	AIC_BASE->AIC_ICCR = (1 << id);
}

// ----------------------------------------------------------------------------
void irq_manager()
{
	static int irq_id;
	
	// get current irq id
	irq_id = (AIC_BASE->AIC_ISR & 0x1F);
	
	if (irq_table[irq_id]) {
		irq_table[irq_id]();
	}
}

// ----------------------------------------------------------------------------
// id: TC0_ID, TC1_ID, TC2_ID
// interval: ms
void cpu_timer_init(int id, int priority, int interval)
{
	StructTC *TC;
	
	switch (id) {
	case TC0_ID:
		TC = TC0_BASE;
		TCB0_BASE->TC_BMR = 0x00000015;		// no external clock selected
		break;
	case TC1_ID:
		TC = TC1_BASE;
		break;
	case TC2_ID:
		TC = TC2_BASE;
		break;		
	default:
		return;
	}
	
	PS_BASE->PS_PCER = (1 << id);

	TC->TC_CMR	= 0x0000C002;				// MCLK / 32
	TC->TC_CCR	= 0x00000005;				// enable
	TC->TC_IER	= 0x00000010;
	
	//定时长度
	TC->TC_RC	= (unsigned short)((CPU_CLOCK / (32*1000)) * interval);
	TC->TC_SR;
	
	//设置timer0中断模式
	irq_set_priority(id, priority);
	irq_enable(id);
}

// ----------------------------------------------------------------------------
UINT IRQ_Exception_PC;
char IRQ_Exception_msg[64];

void cpu_soft_reset_handler()
{
	RawDebugOutputString("CPU: soft reset. PC = 0x");
	RawDebugOutputString(uint2str(IRQ_Exception_PC, IRQ_Exception_msg, 16));
	
#if (HALT_WHEN_CPU_EXCEPTION > 0)
	while (true);
#endif
}

// ----------------------------------------------------------------------------
void cpu_undef_handler()
{
	RawDebugOutputString("CPU: undef handler. PC = 0x");
	RawDebugOutputString(uint2str(IRQ_Exception_PC, IRQ_Exception_msg, 16));
	
#if (HALT_WHEN_CPU_EXCEPTION > 0)
	while (true);
#endif
}

// ----------------------------------------------------------------------------
void cpu_swi_handler()
{
	RawDebugOutputString("CPU: SWI handler. PC = 0x");
	RawDebugOutputString(uint2str(IRQ_Exception_PC, IRQ_Exception_msg, 16));
	
#if (HALT_WHEN_CPU_EXCEPTION > 0)
	while (true);
#endif
}

// ----------------------------------------------------------------------------
void cpu_prefetch_abort_handler()
{
	RawDebugOutputString("CPU: prefetch abort. PC = 0x");
	RawDebugOutputString(uint2str(IRQ_Exception_PC, IRQ_Exception_msg, 16));
	
#if (HALT_WHEN_CPU_EXCEPTION > 0)
	while (true);
#endif
}

// ----------------------------------------------------------------------------
void cpu_data_abort_handler()
{
	RawDebugOutputString("CPU: data abort. PC = 0x");
	RawDebugOutputString(uint2str(IRQ_Exception_PC, IRQ_Exception_msg, 16));
	
#if (HALT_WHEN_CPU_EXCEPTION > 0)
	while (true);
#endif
}

// ----------------------------------------------------------------------------
// End of cpu_irq.c
