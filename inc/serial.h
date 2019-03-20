
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/

#ifndef __Serial_H__
#define __Serial_H__

#include <C_m40800.h>
#include <system.h>
#include <device.h>
#include <queue.h>

#define MAX_SERIAL_PORTS				2
#define MAX_SERIAL_BUFFER_LENGTH		1024

typedef struct {
	TQueue		Queue;
	byte		Buffer[MAX_SERIAL_BUFFER_LENGTH];
	int			PortID;
	StructUSART	*Uart;
} TSerialConfig;

#define SERIAL_PORT_COM1				0
#define SERIAL_PORT_COM2				1

#define	BAUD_1200     					1200
#define	BAUD_2400     					2400
#define	BAUD_4800     					4800
#define	BAUD_9600     					9600
#define BAUD_19200    					19200
#define BAUD_38400    					38400
#define BAUD_57600    					57600
#define BAUD_115200    					115200

bool OpenSerialPort(struct TDevice *dev);
void ReleaseSerialPort(struct TDevice *dev);
int ReadSerialPort(struct TDevice *dev, byte *buffer, UINT count);
int WriteSerialPort(struct TDevice *dev, const byte *buffer, UINT count);

// call this ISR in interrupt when serial port received bytes
void isr_serial_port();

#endif
