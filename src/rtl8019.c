
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/

#include <types.h>
#include <system.h>
#include <stdio.h>
#include <rtl8019.h>
#include <C_m40800.h>

#define ETH_SEND_RECV_TIMEOUT		100		// 100ms communication between CPU <--> ETH

bool is_lwip_enabled = true;

// ----------------------------------------------------------------------------
// 以太网异常处理机制
//
// ETH_OS_WAIT 1: 意味着采用了 OS 的互斥机制
//             0: 采用中断开关
// eth_read, eth_send, eth_monitor 三者有可能对以太网同时操作，所以需要互斥
// 当以太网产生异常中断的时候，会通知 eth_monitor 复位以太网芯片，此时有可能
// eth_read 或者 eth_send 正处于操作的过程中，另外 eth_read 和 eth_send 都会
// 使用到以太网的 DMA 传输机制，不能同时使用，在中断互斥模式下，三者都会进入
// 临界区，在 OS 互斥模式下， eth_read 和 eth_send 由应用程序采用信号量互斥，
// 为了保证 eth_monitor 与这二者同时互斥，添加了一个计数器 eth_operations，
// eth_monitor 只有在 eth_operations 为 0 时才会操作以太网，否则等到下次再尝
// 试， eth_monitor 没有使用信号量互斥是因为定时器的代码是在中断中运行的。
//
// Ken, 2006-01-26 14:41:20
// ----------------------------------------------------------------------------
#if (ETH_OS_WAIT >0)
int eth_operations = 0;
#endif

// ----------------------------------------------------------------------------
void lwip_enable()
{
	is_lwip_enabled = true;
}

// ----------------------------------------------------------------------------
void lwip_disable()
{
	is_lwip_enabled = false;
}

// ----------------------------------------------------------------------------
inline void eth_reg_write(unsigned int reg, unsigned char data)
{
	outp16(reg, data);
}

// ----------------------------------------------------------------------------
inline unsigned char eth_reg_read(unsigned int reg)
{
	return (unsigned char)inp16(reg);
} 

// ----------------------------------------------------------------------------
inline void eth_data_write(unsigned short data)
{
	outp16(reg10, data);
}

// ----------------------------------------------------------------------------
inline unsigned short eth_data_read()
{
	return inp16(reg10);
} 

// ----------------------------------------------------------------------------
int eth_init()
{
    // Select RTL8019AS register page 0.
    ETHERDEV_SELECT_REG_PAGE(0);

    // Select RTL8019AS register page 3.
    ETHERDEV_SELECT_REG_PAGE(3);

    // Temporarily disable config register write protection.
    eth_reg_write(_9346CR, EEM1 | EEM0);

	// Enable IRQ
 	eth_reg_write(CONFIG1, IRQEN);

    // Disable boot ROM & select 10BaseT with TP/CX auto-detect.
    eth_reg_write(CONFIG2, BSELB);

    // Select half-duplex, awake, power-up & LED_TX/ LED_RX/ LED_LNK behaviour.
    eth_reg_write(CONFIG3, PNP|LEDS0|LEDS1|FUDUP);

    // Re-enable config register write protection.
    eth_reg_write(_9346CR, 0x00);
 
    // Select RTL8019AS register page 0.
    ETHERDEV_SELECT_REG_PAGE(0);

    // Stop RTL8019AS, select page 0 and abort DMA operation.
    eth_reg_write(CR, RD2 | STP);
 
	// Set receive buffer page start.
    eth_reg_write(PSTART, ETH_RX_PAGE_START);
    
	// Set receive buffer page stop.
    eth_reg_write(PSTOP, ETH_RX_PAGE_STOP);
    
    // Initialise last receive buffer read pointer.
    eth_reg_write(BNRY, ETH_RX_PAGE_START);
    
    // Set transmit page start.
    eth_reg_write(TPSR, ETH_TX_PAGE_START);
    
    // Clear interrupt status register bits by writing 1 to each.
    eth_reg_write(ISR, 0xFF);

    // Reset remote byte count registers.
    eth_reg_write(RBCR0, 0x00);
    eth_reg_write(RBCR1, 0x00);

    // Receive configuration register to monitor mode.
    eth_reg_write(RCR, 0xce);		//0xcf
    
     // Initialise transmit configuration register to loopback internally.
    eth_reg_write(TCR, 0xe0);
     
    // Initialise data configuration register. 
    // FIFO threshold 16 bytes, no loopback, don't use auto send packet.	little 16bit
    eth_reg_write(DCR, 0xcb);

    // Mask all interrupts in mask register.
    eth_reg_write(IMR, 0x00);

    // Select RTL8019AS register page 0 and abort DMA operation.
    eth_reg_write(CR, RD2 | PS0 | STP);

    // Initialise current packet receive buffer page pointer
    eth_reg_write(CURR, ETH_RX_PAGE_START+1);

	eth_reg_write(reg08, 0x00);	// MAR0
	eth_reg_write(reg09, 0x41);
	eth_reg_write(reg0a, 0x00);
	eth_reg_write(reg0b, 0x80);
	eth_reg_write(reg0c, 0x00);
	eth_reg_write(reg0d, 0x00);
	eth_reg_write(reg0e, 0x00);
	eth_reg_write(reg0f, 0x00);	// MAR7

	// Select RTL8019AS register page 1.
    eth_reg_write(CR, RD2 | PS0 | STP);
	eth_reg_write(PAR0, UIP_ETHADDR0);
	eth_reg_write(PAR1, UIP_ETHADDR1);
	eth_reg_write(PAR2, UIP_ETHADDR2);
	eth_reg_write(PAR3, UIP_ETHADDR3);
	eth_reg_write(PAR4, UIP_ETHADDR4);
	eth_reg_write(PAR5, UIP_ETHADDR5);

	ETHERDEV_SELECT_REG_PAGE(0);
	eth_reg_write(ISR, 0xff);					//clear ISR 
	eth_reg_write(IMR,(OVW|TXE|RXE|PRX));		//set IMR bit
	//eth_reg_write(IMR,0x01 | 0x1c);			//set IMR bit
 
    // Restart RTL8019AS. 
    eth_reg_write(CR, RD2 | STA);

	return 1;
}

// ----------------------------------------------------------------------------
int eth_send(unsigned short *buffer, unsigned short length)
{
	unsigned short i;
#if (ETH_OS_WAIT >0)
	unsigned int tick;
#endif
	
	if (!is_lwip_enabled) {
		return 0;
	}
	
	if (length >= MAX_IP_PACKET_LENGTH) {
		return 0;
	}
	
#if (ETH_OS_WAIT >0)
	eth_operations++;
#else
	ARMDisableInt();
#endif
	
    // still transmitting a packet - wait for it to finish.
    eth_reg_write(CR, RD2 | STA);

    // Wait until pending transmit operation completes.
#if (ETH_OS_WAIT >0)
	tick = GetTickCount();
    while(eth_reg_read(CR) & TXP) {
		if (abs(GetTickCount() - tick) > ETH_SEND_RECV_TIMEOUT) {
			eth_operations--;
			return 0;
		}
		else {
			OSTimeDly(1);
		}
	}
#else
	while(eth_reg_read(CR) & TXP);
#endif

    // Clear remote DMA complete interrupt status register bit.
    eth_reg_write(ISR, RDC);
	
    // Set remote DMA start address registers to indicate where to load packet.
    eth_reg_write(RSAR0, 0x00);
    eth_reg_write(RSAR1, ETH_TX_PAGE_START);

    // Set remote DMA byte count registers to indicate length of packet load.
    eth_reg_write(RBCR0, (unsigned char)(length & 0xff));
    eth_reg_write(RBCR1, (unsigned char)(length >> 8));

	// ---------------------------------------------------------------------
    // Initiate DMA transfer of buffers to RTL8019AS.
    eth_reg_write(CR, RD1 | STA);

	for (i = 0; i < (length + 1) / 2; i++) {
		eth_data_write(*buffer);
		buffer++;
	}

	// ---------------------------------------------------------------------
	// Wait until remote DMA operation complete.
#if (ETH_OS_WAIT >0)
	tick = GetTickCount();
    while(!(eth_reg_read(ISR) & RDC)) {
		if (abs(GetTickCount() - tick) > ETH_SEND_RECV_TIMEOUT) {
			eth_operations--;
			return 0;
		}
		else {
			OSTimeDly(1);
		}
	}
#else
	while(!(eth_reg_read(ISR) & RDC));
#endif

    // Abort/ complete DMA operation.
    eth_reg_write(CR, RD2 | STA);

    // Set transmit page start to indicate packet start.
    eth_reg_write(TPSR, ETH_TX_PAGE_START);

	// Set transmit byte count registers to indicate packet length.
    eth_reg_write(TBCR0, (unsigned char)(length & 0xff));
    eth_reg_write(TBCR1, (unsigned char)(length >> 8));
	
	// clear transmit ISR
	eth_reg_write(ISR,(eth_reg_read(ISR)|0x02)); 		//clear ISR

	// Issue command for RTL8019AS to transmit packet from it's local buffer.
    eth_reg_write(CR, RD2 | TXP | STA);
	
#if (ETH_OS_WAIT >0)
	eth_operations--;
#else
	ARMEnableInt();
#endif
	
	return length;
}

// ----------------------------------------------------------------------------
unsigned int eth_read(unsigned short *buffer)
{
	unsigned int i, len = 0;
	unsigned char next_rx_packet;
	int bnry, curr;
#if (ETH_OS_WAIT >0)
	unsigned int tick;
#endif
	
	if (!is_lwip_enabled) {
		return 0;
	}

#if (ETH_OS_WAIT >0)
	eth_operations++;
#else
	ARMDisableInt();
#endif

	ETHERDEV_SELECT_REG_PAGE(0);
	bnry = eth_reg_read(BNRY);
	ETHERDEV_SELECT_REG_PAGE(1);
	curr = eth_reg_read(CURR);
	
	bnry = (bnry + 1 == ETH_RX_PAGE_STOP) ? ETH_RX_PAGE_START : (bnry + 1);
	if (bnry == curr) {
#if (ETH_OS_WAIT >0)
		eth_operations--;
#else
		ARMEnableInt();
#endif
		return 0;
	}

	ETHERDEV_SELECT_REG_PAGE(0);

	// Clear remote DMA complete interrupt status register bit.
	eth_reg_write(ISR, RDC);

	// Abort/ complete DMA operation.
	eth_reg_write(CR, RD2 | STA);
	
	// Clear remote DMA complete interrupt status register bit.

	ETHERDEV_SELECT_REG_PAGE(0);
 	eth_reg_write(RSAR1, bnry);
	eth_reg_write(RSAR0, 0x00);

	// Set remote DMA byte count registers to packet header length.
	eth_reg_write(RBCR0, 0x04);
	eth_reg_write(RBCR1, 0x00);

	// Initiate DMA transfer of packet header.
	eth_reg_write(CR, RD0 | STA);

	//will receive 4 byte head
	next_rx_packet = eth_data_read() / 0x100;
	len = eth_data_read();
	len -= 4;
	
	// Wait until remote DMA operation completes.
#if (ETH_OS_WAIT >0)
	tick = GetTickCount();
	while(!(eth_reg_read(ISR) & RDC)) {
		if (abs(GetTickCount() - tick) > ETH_SEND_RECV_TIMEOUT) {
			eth_operations--;
			return 0;
		}
		else {
			OSTimeDly(1);
		}
	}
#else
	while(!(eth_reg_read(ISR) & RDC));
#endif
	// Abort/ complete DMA operation.
	eth_reg_write(CR, RD2 | STA);
	
	// Clear remote DMA complete interrupt status register bit.
	eth_reg_write(ISR, RDC);
	
	// Set remote DMA start address registers to packet data.
	eth_reg_write(RSAR0, 0x04);
	eth_reg_write(RSAR1, bnry);

	// Set remote DMA byte count registers to packet data length.
	eth_reg_write(RBCR0, (unsigned char)(len & 0xFF));
	eth_reg_write(RBCR1, (unsigned char)(len >> 8));

	// Initiate DMA transfer of packet data.
	eth_reg_write(CR, RD0 | STA);
	
	// Read packet data directly into buf.
	if (len >= MAX_IP_PACKET_LENGTH) {
		for (i = 0; i < (len + 1) / 2; i++) {
			eth_data_read();
		}
#if (ETH_OS_WAIT >0)
		eth_operations--;
#else
		ARMEnableInt();
#endif
		return 0;
	}
	else {
		for (i = 0; i < (len + 1) / 2; i++) {
			*buffer = eth_data_read();
			buffer++;
		}
	}
	
	// Abort/ complete DMA operation.
	eth_reg_write(CR, RD2 | STA);
	
	// Advance boundary pointer to next packet start.
	bnry = (next_rx_packet == ETH_RX_PAGE_START) ? (ETH_RX_PAGE_STOP - 1) : (next_rx_packet - 1);
	eth_reg_write(BNRY, bnry);
	
#if (ETH_OS_WAIT >0)
	eth_operations--;
#else
	ARMEnableInt();
#endif
	
	return	len;
}

/* ------------------------------------------------------------------------
 *  函数名:
 *      void eth_monitor()
 *  功能:
 *      监控以太网的状态,如果发生异常就会复位以太网
 *  参数:
 *      
 *  返回值:
 *      
 *  说明:
 *      在定时器中调用这个函数
 * -----------------------------------------------------------------------*/

bool eth_restart = false;

void eth_monitor(void *arg)
{
	if (eth_restart) {
#if (ETH_OS_WAIT >0)
		if (eth_operations == 0) {
#endif
			ARMDisableInt();
			eth_init();
			ARMEnableInt();
			lwip_enable();
			
			eth_restart = false;
			eth_irq_enable();
#if (ETH_OS_WAIT >0)
		}
#endif
	}	
}

// ----------------------------------------------------------------------------
void isr_eth()	
{
	unsigned char isr_flag;
	int bnry, curr;
	extern void ethernetif_isr();
	
	ETHERDEV_SELECT_REG_PAGE(0);
	bnry = eth_reg_read(BNRY);
	ETHERDEV_SELECT_REG_PAGE(1);
	curr = eth_reg_read(CURR);

	ETHERDEV_SELECT_REG_PAGE(0);
	isr_flag = eth_reg_read(ISR);
	
	if (isr_flag & PRX) {
		ETHERDEV_SELECT_REG_PAGE(0);
		eth_reg_write(ISR, PRX);

		if (is_lwip_enabled) {
			ethernetif_isr();
		}
	}
	else if (isr_flag & (RST | OVW | TXE | RXE)) {
		RawDebugPrintf("Network reset, ISR exception = [%02x], bnry=%02x, curr=%02x\n", isr_flag, bnry, curr);
		
		// restart eth
		eth_irq_disable();
		lwip_disable();
		eth_restart = true;
		eth_reg_write(ISR, 0xff);
	}
}

// ----------------------------------------------------------------------------
#if 1
void eth_get_mac_addr(unsigned char *addr)
{
	addr[0] = UIP_ETHADDR0;
	addr[1] = UIP_ETHADDR1;
	addr[2] = UIP_ETHADDR2;
	addr[3] = UIP_ETHADDR3;
	addr[4] = UIP_ETHADDR4;
	addr[5] = UIP_ETHADDR5;
}
#else
void eth_get_mac_addr(unsigned char* macaddr)
{
    unsigned char tempCR;

    // switch register pages
    tempCR = eth_reg_read(CR);
    eth_reg_write(CR, tempCR | PS0);
	
    // read MAC address registers
    *macaddr++ = eth_reg_read(PAR0);
    *macaddr++ = eth_reg_read(PAR1);
    *macaddr++ = eth_reg_read(PAR2);
    *macaddr++ = eth_reg_read(PAR3);
    *macaddr++ = eth_reg_read(PAR4);
    *macaddr++ = eth_reg_read(PAR5);
	
    // switch register pages back
    eth_reg_write(CR, tempCR);
}
#endif

// ----------------------------------------------------------------------------
void eth_set_mac_addr(unsigned char* macaddr)
{
    unsigned char tempCR;
	
    // switch register pages
    tempCR = eth_reg_read(CR);
    eth_reg_write(CR, tempCR | PS0);

    // write MAC address registers
    eth_reg_write(PAR0, *macaddr++);
    eth_reg_write(PAR1, *macaddr++);
    eth_reg_write(PAR2, *macaddr++);
    eth_reg_write(PAR3, *macaddr++);
    eth_reg_write(PAR4, *macaddr++);
    eth_reg_write(PAR5, *macaddr++);
	
    // switch register pages back
    eth_reg_write(CR, tempCR);
}

// ----------------------------------------------------------------------------
int eth_empty()
{
	int bnry, curr;
	
	ETHERDEV_SELECT_REG_PAGE(0);
	bnry = eth_reg_read(BNRY);
	ETHERDEV_SELECT_REG_PAGE(1);
	curr = eth_reg_read(CURR);
	
	bnry = (bnry + 1 == ETH_RX_PAGE_STOP) ? ETH_RX_PAGE_START : (bnry + 1);
	return (bnry == curr);
}

// ----------------------------------------------------------------------------
void eth_irq_enable()
{
	PIO_BASE->PIO_PDR = (1<<10);
	AIC_BASE->AIC_SMR[IRQ1_ID] = 0x00000045;		// 0x45 为电平触发模式
	irq_enable(IRQ1_ID);
}

// ----------------------------------------------------------------------------
void eth_irq_disable()
{
	irq_disable(IRQ1_ID);
}

// ----------------------------------------------------------------------------
// E N D
