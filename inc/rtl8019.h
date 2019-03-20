
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/

#ifndef __RTL8019_H__
#define	__RTL8019_H__
 
#define MAX_IP_PACKET_LENGTH	1500

#define ETH_CPU_CLOCK			ETH_CPU_XTAL / 12    // 8051 clock rate (X1 mode)

// Delay routine timing parameters
#define ETH_DELAY_CONST			9.114584e-5			// Delay routine constant
#define ETH_DELAY_MULTPLR		(unsigned char)(ETH_DELAY_CONST * ETH_CPU_CLOCK)

// X1 CPU mode timing parameters
#define ETH_T0_CLOCK			ETH_CPU_XTAL / 12	// Timer 0 mode 1 clock rate
#define ETH_T0_INT_RATE			24					// Timer 0 intrupt rate (Hz)
#define ETH_T0_RELOAD			65536 - (ETH_T0_CLOCK / ETH_T0_INT_RATE)

// Packet transmit & receive buffer configuration
#define ETH_TX_PAGE_START		0x40    			// 0x4000 Tx buffer is 12 * 256 =  3k bytes
#define ETH_RX_PAGE_START		0x4c    			// 0x4c00 Rx buffer is 52 * 256 = 13k bytes
#define ETH_RX_PAGE_STOP 		0x80    			// 0x8000

#define ETH_ADDR_PORT_MASK		0x1F        		// 00011111y
#define ETH_DATA_PORT_MASK		0xFF         		// 11111111y

#define ETH_MIN_PACKET_LEN		0x3C


#define ETH_BASEADDR			0x02000000
#define reg00					ETH_BASEADDR		/*/CR command register*/
      
/*
//   bit7      bit6       bit5      bit4        bit3       bit2    bit1      bit0
//   PS1       PS0        RD2       RD1         RD0        TXP     STA       STP

//   RD2 RD1 RD0
//   0   0   1   read memory
//   0   1   0   write memory
//   0   1   1   send data packet
//   1   x   x   complete read or write for DMA
*/
#define reg01					(ETH_BASEADDR + 0x2)
#define reg02					(ETH_BASEADDR + 0x4)
#define reg03					(ETH_BASEADDR + 0x6)
#define reg04					(ETH_BASEADDR + 0x8)
#define reg05					(ETH_BASEADDR + 0xa)
#define reg06					(ETH_BASEADDR + 0xc)
#define reg07					(ETH_BASEADDR + 0xe)
#define reg08					(ETH_BASEADDR + 0x10)
#define reg09					(ETH_BASEADDR + 0x12)
#define reg0a					(ETH_BASEADDR + 0x14)
#define reg0b					(ETH_BASEADDR + 0x16)
#define reg0c					(ETH_BASEADDR + 0x18)
#define reg0d					(ETH_BASEADDR + 0x1a)
#define reg0e					(ETH_BASEADDR + 0x1c)
#define reg0f					(ETH_BASEADDR + 0x1e)
#define reg10					(ETH_BASEADDR + 0x20)	/* DMA operation port0 */
#define reg11					(ETH_BASEADDR + 0x22)	/* DMA operation port1 */
#define reg18					(ETH_BASEADDR + 0x30)	/* RESET port          */
#define reg1f					(ETH_BASEADDR + 0x3e)

#define BUFLENTH				768


#define CR						ETH_BASEADDR   		 	// CR command register

// Control register bits
#define PS1						0x80      				// Page select bit 1
#define PS0						0x40  					// Page select bit 0
#define RD2						0x20 					// Remote DMA control bit 2
#define RD1						0x10   	  				// Remote DMA control bit 1
#define RD0						0x08  					// Remote DMA control bit 0
#define TXP						0x04  					// Transmit packet bit
#define STA						0x02  					// Start bit (a flag only)
#define STP						0x01  					// Stop bit transceiver ctrl
#define RDMA					0x10  					// Remote DMA port
#define RESET					0x18  					// Reset port

// Page 0 read/write registers.
#define BNRY					reg03					// Boundary register
#define ISR						reg07					// Interrupt status register

// Interrupt status register bits
#define RST						0x80            		// Reset state indicator bit
#define RDC						0x40             		// Remote DMA complete bit
#define CNT						0x20           			// Network tally counter MSB set
#define OVW						0x10            		// Receive buffer exhausted
#define TXE						0x08           			// Transmit abort error bit
#define RXE						0x04           			// Receive error report bit
#define PTX						0x02          			// Successful packet transmit
#define PRX						0x01           			// Successful packet receive

// Page 0 write only registers.
#define PSTART					reg01   				// Receive page start register
#define PSTOP					reg02   				// Receive page stop register
#define TPSR					reg04   				// Transmit page start register
#define TBCR0					reg05   				// Transmit byte count register 0
#define TBCR1					reg06   				// Transmit byte count register 1
#define RSAR0					reg08   				// Remote start address register 0
#define RSAR1					reg09   				// Remote start address register 1
#define RBCR0					reg0a   				// Remote byte count register 0
#define RBCR1					reg0b   				// Remote byte count register 1
#define RCR						reg0c   				// Receive configuration register

// Receive configuration register bits (write in page 0, read in page 2)
#define MON						0x20					// Monitor mode select bit
#define PRO						0x10					// Promiscuous mode select bit
#define AM						0x08					// Multicast packet accept bit
#define AB						0x04					// Broadcast packet accept bit
#define AR						0x02					// Runt packet accept bit
#define SEP						0x01					// Error packet accept bit

#define TCR						reg0d   				// Transmit configuration register

// Transmit configuration register bits
#define OFST					0x10					// Collision offset enable bit
#define ATD						0x08					// Auto transmit disable select bit
#define LB1						0x04					// Loopback mode select bit 1
#define LB0						0x02					// Loopback mode select bit 0
#define CRC						0x01					// CRC generation inhibit bit

#define DCR						reg0e					// Data configuration register

// Data configuration register bits (write in page 0, read in page 2)
#define FT1						0x40					// FIFO threshold select bit 1
#define FT0						0x20					// FIFO threshold select bit 0
#define ARM						0x10					// Auto-initialise remote
#define LS						0x08					// Loopback select bit
#define LAS						0x04					// Set to 0 (pwrup = 1)
#define BOS						0x02					// Byte order select bit
#define WTS						0x01					// Word transfer select bit

#define IMR						reg0f					// Interrupt mask register
// Interrupt mask register bits
// Each enable bit correspons with an interrupt flag in ISR


// Page 1 read/write registers.
#define PAR0					reg01   				// Physical address register 0
#define PAR1					reg02   				// Physical address register 1
#define PAR2					reg03  				    // Physical address register 2
#define PAR3					reg04   				// Physical address register 3
#define PAR4					reg05   				// Physical address register 4
#define PAR5					reg06   				// Physical address register 5
#define CURR					reg07   				// Current receive buffer page
#define MAR0					reg08
#define MAR1					reg09
#define MAR2					reg0a
#define MAR3					reg0b
#define MAR4					reg0c
#define MAR5					reg0d
#define MAR6					reg0e
#define MAR7					reg0f

// Page 3 read/write registers.
#define _9346CR					reg01			      // 9346 EEPROM command register

// 9346 EEPROM command register bits
#define EEM1					0x80                  // RTL8019AS operating mode bit 1
#define EEM0					0x40                  // RTL8019AS operating mode bit 0
#define EECS					0x08                  // 9346 EEPROM chip select bit
#define EESK					0x04                  // 9346 EEPROM serial clock bit
#define EEDI					0x02                  // 9346 EEPROM data input bit
#define EEDO					0x01                  // 9346 EEPROM data output bit

#define BPAG					reg02
#define CONFIG1					reg03   			  // RTL9019AS config register 1

// RTL9019AS config register 1 bits
#define IRQEN					0x80                  // IRQ enable bit (WR protected)
#define IRQS2					0x40                  // IRQ line select bit 2
#define IRQS1					0x20                  // IRQ line select bit 1
#define IRQS0					0x10                  // IRQ line select bit 0
#define IOS3					0x08                  // I/O base address select bit 3
#define IOS2					0x04                  // I/O base address select bit 2
#define IOS1					0x02                  // I/O base address select bit 1
#define IOS0					0x01                  // I/O base address select bit 0

#define CONFIG2					reg05

// RTL9019AS config register 2 bits
#define PL1						0x80                  // Network medium type select bit 1
#define PL0						0x40                  // Network medium type select bit 0
#define BSELB					0x20                  // Boot ROM disable (WR protected)
#define BS4						0x10                  // Boot ROM configuration bit 4
#define BS3						0x08                  // Boot ROM configuration bit 3
#define BS2						0x04                  // Boot ROM configuration bit 2
#define BS1						0x02                  // Boot ROM configuration bit 1
#define BS0						0x01                  // Boot ROM configuration bit 0

#define CONFIG3					reg06   			  // RTL9019AS config register 3

// RTL9019AS config register 3 bits
#define PNP						0x80                  // Plug & play mode indicator bit
#define FUDUP					0x40                  // Full duplex mode select bit
#define LEDS1					0x20                  // LED output select bit 1
#define LEDS0					0x10                  // LED output select bit 0
#define SLEEP					0x04                  // Sleep mode select bit
#define PWRDN					0x02                  // Power down mode select bit
#define ACTIVEB					0x01                  // Inverse of bit 0, PNP active reg

// Page 3 read only registers.
#define CONFIG0					reg03  				  // RTL9019AS config register 0

// RTL9019AS config register 0 bits
#define VERID1					0x80                  // RTL9019AS version ID bit 1 (R/W)
#define VERID0					0x40                  // RTL9019AS version ID bit 0 (R/W)
#define AUI						0x20                  // AUI input pin state bit
#define PNPJP					0x10                  // PNP jumper pin state bit
#define JP						0x08                  // JP input pin state bit
#define BNC						0x04                  // Thinnet mode indication bit

#define CSNSAV					reg08
#define INTR					reg0b
#define CONFIG4					reg0d

// Page 3 write only registers.
#define TEST					reg07
#define HLTCLK					reg09
#define FMWP					reg0c

#define ETHERDEV_SELECT_REG_PAGE(page) 		\
         { eth_reg_write(reg00, ((eth_reg_read(reg00) &0x3f) | ((char)(page<<6)))); }

#define UIP_ETHADDR0			0x00 				
#define UIP_ETHADDR1			0x80  
#define UIP_ETHADDR2			0x19  
#define UIP_ETHADDR3			0x83  
#define UIP_ETHADDR4			0xbd  
#define UIP_ETHADDR5			0xfc  

/*
 * ETH_OS_WAIT = 1 means eth_read and eth_send use OS Sem for mutex
 * ETH_OS_WAIT = 0 means use INTERRUPT enable/disable for mutex
 *
 * Ken, 2006-01-23 17:02:10
 */
#define ETH_OS_WAIT				0

void eth_irq_enable();
void eth_irq_disable();
int eth_init();
int eth_send(unsigned short *buffer,unsigned short length);
unsigned int eth_read(unsigned short *buffer);
void eth_get_mac_addr(unsigned char *addr);
void eth_set_mac_addr(unsigned char* macaddr);
int eth_empty();
void eth_monitor(void *arg);

#endif // __RTL8019_H__
