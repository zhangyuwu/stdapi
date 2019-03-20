
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/

#ifndef __Device_H__
#define __Device_H__

#include <types.h>
#include <ucos_ii.h>

#define MAX_DEVICES				16			// �������16���豸
#define MAX_DEVICE_NAME_LENGTH	12			// �豸�������12�ַ�
#define INVALID_HANDLE_VALUE	0			// �Ƿ����ֵ

// �豸�ṹ��
typedef struct TDevice {
	UINT		MagicNumber;
	char 		Name[MAX_DEVICE_NAME_LENGTH];
	bool		Opened;
	void *		PrivateData;
	ulong		Option;
	ulong		Offset;
	bool		(*Open)(struct TDevice *dev);
	void		(*Release)(struct TDevice *dev);
	int			(*Read)(struct TDevice *dev, byte *buffer, UINT count);
	int			(*Write)(struct TDevice *dev, const byte *buffer, UINT count);
	int			(*Seek)(struct TDevice *dev, ulong offset);
	int			(*Ioctl)(struct TDevice *dev, ulong cmd, void *arg);
	OS_EVENT *	Sem;
} TDevice;

bool RegisterDevice(TDevice *device);
bool UnregisterDevice(const char *name);
THandle OpenDevice(const char *name, ulong option);
int CloseDevice(THandle handle);
int ReadDevice(THandle handle, byte *buffer, UINT length);
int WriteDevice(THandle handle, const byte *buffer, UINT length);
int SeekDevice(THandle handle, ulong offset);
int IoctlDevice(THandle handle, ulong cmd, void *arg);

#endif
