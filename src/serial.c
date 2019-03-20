
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <serial.h>
#include <util.h>
#include <ucos_ii.h>
#include <C_m40800.h>

#define valid(port)		(port >= SERIAL_PORT_COM1 && port <= SERIAL_PORT_COM2)

// ----------------------------------------------------------------------------
TSerialConfig g_SerialConfig[MAX_SERIAL_PORTS];

// ----------------------------------------------------------------------------
bool OpenSerialPort(struct TDevice *dev)
{
	int i, baud;
	TSerialConfig *config;
	
	if (dev != NULL) {
		if (!strcmp(dev->Name, "COM1")) {
			config = &g_SerialConfig[SERIAL_PORT_COM1];
			config->PortID = SERIAL_PORT_COM1;
			config->Uart = USART0_BASE;
		}
		else if (!strcmp(dev->Name, "COM2")) {
			config = &g_SerialConfig[SERIAL_PORT_COM2];
			config->PortID = SERIAL_PORT_COM2;
			config->Uart = USART1_BASE;
		}
		else {
			SetLastError(ERROR_NO_FONT_SELECTED);
			return false;
		}

		dev->PrivateData = config;
		
		// 初始化串口队列
		InitializeQueue(&(config->Queue), config->Buffer, sizeof(byte), MAX_SERIAL_BUFFER_LENGTH);

		// 计算底层波特率
		baud = round((double)CPU_CLOCK / (dev->Option * 16));
		
		// 打开串口
		config->Uart->US_CR 		= 0x000009AC;		// reset
		config->Uart->US_RCR 		= 0;
		config->Uart->US_TCR 		= 0;
		config->Uart->US_RTOR		= 0;
		config->Uart->US_BRGR 		= baud;				// set_band
		config->Uart->US_MR 		= 0x000008C0;		// mode	8-bit 1-stop,no p enable clk
		config->Uart->US_TTGR 		= 0x0;				// Disables the TX Time-guard function.
		config->Uart->US_CR 		= 0x00000050;
		
		for (i = 0; i < 0x100; i++);
		config->Uart->US_RHR;

		//uart char receive mode
		config->Uart->US_IDR 		= 0x000003FF;		// disable other interrupt
		config->Uart->US_IER 		= 0x00000001;		// enable rx and endtx interrupt

		switch (config->PortID) {
		case SERIAL_PORT_COM1:
			PS_BASE->PS_PCER = (1 << US0_ID);			// enable clock(US0)
			PIO_BASE->PIO_PDR = (0x3 << 14);			// pin14, pin15 use for com1
			irq_enable(US0_ID);
			break;
			
		case SERIAL_PORT_COM2:
			PS_BASE->PS_PCER = (1 << US1_ID);			// enable clock(US1)
			PIO_BASE->PIO_PDR = (0x3 << 21);			// pin14, pin15 use for com1
			irq_enable(US1_ID);
			break;
		}
		dev->PrivateData = config;
		
		return true;
	}
	
	return false;	// config == NULL
}

// ----------------------------------------------------------------------------
int ReadSerialPort(struct TDevice *dev, byte *buffer, UINT count)
{
	TSerialConfig *config = (TSerialConfig *)dev->PrivateData;
	int len = 0;
	
	while (len < count) {
		if (IsQueueEmpty(&(config->Queue)))
			break;
			
		PopQueue(&(config->Queue), buffer + len);
		len++;
	}
	
	return len;
}

// ----------------------------------------------------------------------------
int WriteSerialPort(struct TDevice *dev, const byte *buffer, UINT count)
{
	TSerialConfig *config = (TSerialConfig *)dev->PrivateData;
	
	if (config != NULL) {
		// 串口发送
		if ((config->Uart->US_CSR & (1<<4)) == 0)
			return 0;

		config->Uart->US_TPR = (unsigned int)buffer;
		config->Uart->US_TCR = count;
		
		while ((config->Uart->US_CSR & 0x212) != 0x212)
			OSTimeDly(1);
			
		return count;
	}
	
	return 0;
}

// ----------------------------------------------------------------------------
void ReleaseSerialPort(struct TDevice *dev)
{
	TSerialConfig *config = (TSerialConfig *)dev->PrivateData;

	if (config != NULL) {
		// 关闭串口
		switch (config->PortID) {
		case SERIAL_PORT_COM1:
			PS_BASE->PS_PCDR = (1 << US0_ID);			// disable clock(US0)
			irq_disable(US0_ID);
			break;
			
		case SERIAL_PORT_COM2:
			PS_BASE->PS_PCDR = (1 << US1_ID);			// disable clock(US1)
			irq_disable(US1_ID);
			break;
		}
	}
}

// ----------------------------------------------------------------------------
// NO OS support need
void RawDebugOutputString(const char *buffer)
{
	USART0_BASE->US_TPR = (unsigned int)buffer;
	USART0_BASE->US_TCR = strlen(buffer);
	
	while ((USART0_BASE->US_CSR & 0x212) != 0x212);
}

// ----------------------------------------------------------------------------
// NO OS support need
void RawDebugPrintf(const char *fmt, ...)
{
	static char buffer[512];
	va_list args;	

	memset(buffer, 0, sizeof(buffer));
	va_start(args, fmt);
	vsnprintf(buffer, sizeof(buffer) - 1, fmt, args);
	va_end(args);
	RawDebugOutputString(buffer);
}

// ----------------------------------------------------------------------------
void isr_serial_port()
{
	static TSerialConfig *config;
	static char value;
	
	switch (AIC_BASE->AIC_ISR & 0x1F) {					// irq id
	case US0_ID:
		config = &g_SerialConfig[SERIAL_PORT_COM1];
		break;
		
	case US1_ID:
		config = &g_SerialConfig[SERIAL_PORT_COM2];
		break;
		
	default:
		return;
	}
	
	config->Uart->US_CSR;
	value = (char)(config->Uart->US_RHR);
	PushQueue(&(config->Queue), &value);
}

// ----------------------------------------------------------------------------
