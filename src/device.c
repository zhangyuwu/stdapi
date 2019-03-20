
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/

/*
 * --- Update history ---
 *
 * 2006-01-27 20:19:39, Ken (cpp.china@gmail.com)
 *     �� CloseDevie �������˱������ȹر� Opened ��־����������߳����ڲ����豸��
 * �͵ȴ���Щ������ɺ����ͷ��豸��
 *
 */

#include <stdio.h>
#include <string.h>
#include <system.h>
#include <device.h>
#include <ucos_ii.h>
#include <magic.h>

// ==========================================================================
//   �������򹹼�
// ==========================================================================

TDevice g_Devices[MAX_DEVICES];

/* ------------------------------------------------------------------------
 *  ��������
 *      bool RegisterDevice(TDevice *device)
 *  ���ܣ�
 *      ע���豸����
 *  ������
 *      device          ----    Ҫע����豸�ṹָ��
 *  ����ֵ��
 *      �ɹ���ʧ��
 *  ˵����
 *      ����Ĵ��������ͨ��GetLastError��ȡ
 *      --------------------------------------------------------
 *    	ERROR_INVALID_NAME      ----    �豸������Ч
 *      ERROR_FILE_EXISTS       ----    �豸�Ѿ���ע��
 *      ERROR_NO_FREE_SPACE     ----    ע����豸�Ѿ��ﵽ���ֵ
 *      ERROR_INVALID_HANDLE    ----    ��Ч���豸���
 *      NO_ERROR                ----    �����ɹ�
 * -----------------------------------------------------------------------*/

bool RegisterDevice(TDevice *device)
{
	int i, freeIndex = -1;
	static bool dev_inited = false;
	
	if (!dev_inited) {
		memset(g_Devices, 0, sizeof(g_Devices));
		dev_inited = true;
	}
	
	// �ж�Ҫע����豸�Ƿ�ϸ�
	if (device->Name[0] == '\0') {
		SetLastError(ERROR_INVALID_NAME);
		return false;
	}
	
	// �����豸�Ƿ�ע��
	for (i = 0; i < sizeof(g_Devices) / sizeof(TDevice); i++) {
		if (g_Devices[i].MagicNumber == MAGIC_NUMBER_DEVICE) {
			if (!strncmp(g_Devices[i].Name, device->Name, MAX_DEVICE_NAME_LENGTH)) {
				SetLastError(ERROR_FILE_EXISTS);
				return false;
			}
		}
		else {
			if (freeIndex == -1) {
				freeIndex = i;
			}
		}
	}
	
	// û�п��е��豸�ṹ
	if (freeIndex == -1) {
		SetLastError(ERROR_NO_FREE_SPACE);
		return false;
	}
	
	// �����豸�ṹ
	g_Devices[freeIndex] = *device;
	g_Devices[freeIndex].MagicNumber = MAGIC_NUMBER_DEVICE;
	g_Devices[freeIndex].Opened = false;
	g_Devices[freeIndex].Sem = NULL;
	g_Devices[freeIndex].Option = 0;
	g_Devices[freeIndex].Offset = 0;
	g_Devices[freeIndex].PrivateData = NULL;
	
	SetLastError(NO_ERROR);
	
	return true;
}

/* ------------------------------------------------------------------------
 *  ��������
 *      bool UnregisterDevice(const char *name)
 *  ���ܣ�
 *      ж���豸����
 *  ������
 *      name            ----    Ҫж�ص��豸����
 *  ����ֵ��
 *      �ɹ���ʧ��
 *  ˵����
 *      ����Ĵ��������ͨ��GetLastError��ȡ
 *      --------------------------------------------------------
 *    	ERROR_DEVICE_BUSY       ----    �豸æ
 *      ERROR_NOT_REGISTERED    ----    �豸û�б�ע��
 *      NO_ERROR                ----    �����ɹ�
 * -----------------------------------------------------------------------*/

bool UnregisterDevice(const char *name)
{
	int i;
	
	// ����ע����豸
	for (i = 0; i < sizeof(g_Devices) / sizeof(TDevice); i++) {
		if (g_Devices[i].MagicNumber == MAGIC_NUMBER_DEVICE) {
			if (!strncmp(g_Devices[i].Name, name, MAX_DEVICE_NAME_LENGTH)) {
				if (g_Devices[i].Opened) {
					SetLastError(ERROR_DEVICE_BUSY);
					return false;
				}
				
				memset(&g_Devices[i], 0, sizeof(g_Devices[i]));
				SetLastError(NO_ERROR);
				return true;
			}
		}
	}
	
	SetLastError(ERROR_NOT_REGISTERED);
	return false;
}

/* ------------------------------------------------------------------------
 *  ��������
 *      THandle OpenDevice(const char *name, int option)
 *  ���ܣ�
 *      ���豸
 *  ������
 *      name            ----    Ҫ�򿪵��豸����
 *      option          ----    �򿪵Ĳ���
 *  ����ֵ��
 *      �򿪵��豸���
 *  ˵����
 *      ����Ĵ��������ͨ��GetLastError��ȡ
 *      --------------------------------------------------------
 *    	ERROR_DEVICE_BUSY       ----    �豸æ
 *      ERROR_FILE_NOT_FOUND    ----    �豸û�б�ע��
 *      NO_ERROR                ----    �����ɹ�
 * -----------------------------------------------------------------------*/

THandle OpenDevice(const char *name, ulong option)
{
	int i;
	
	for (i = 0; i < sizeof(g_Devices) / sizeof(TDevice); i++) {
		if (!strncmp(g_Devices[i].Name, name, MAX_DEVICE_NAME_LENGTH)) {
			if (g_Devices[i].Opened) {
				SetLastError(ERROR_DEVICE_BUSY);
				return NULL;
			}
			else {
				g_Devices[i].Sem = OSSemCreate(1);
				if (g_Devices[i].Sem == NULL) {
					SetLastError(ERROR_DEVICE_SEM);
					halt("OpenDevice() create sem NULL.\n");
					return NULL;
				}
				g_Devices[i].Opened = true;
				g_Devices[i].Option = option;
				if (g_Devices[i].Open(&g_Devices[i])) {
					SetLastError(NO_ERROR);
					return &g_Devices[i];
				}
				else {
					SetLastError(ERROR_OPEN_FAILED);
					CloseDevice(&g_Devices[i]);
					return NULL;
				}
			}
		}
	}
	
	SetLastError(ERROR_FILE_NOT_FOUND);
	return NULL;
}

/* ------------------------------------------------------------------------
 *  ��������
 *      int CloseDevice(THandle handle)
 *  ���ܣ�
 *      �ر��豸
 *  ������
 *      handle          ----    Ҫ�رյ��豸���
 *  ����ֵ��
 *      ��
 *  ˵����
 *      ����Ĵ��������ͨ��GetLastError��ȡ
 *      --------------------------------------------------------
 *    	ERROR_INVALID_HANDLE    ----    �Ƿ����豸���
 *      ERROR_DEVICE_NOT_OPEN   ----    �豸û�д�
 *      ERROR_INVALID_FUNCTION  ----    ����û��ʵ��release����
 *      NO_ERROR                ----    �����ɹ�
 * -----------------------------------------------------------------------*/

int CloseDevice(THandle handle)
{
	TDevice * device = (TDevice *)handle;
	byte err;
	
	if (device == NULL || device->MagicNumber != MAGIC_NUMBER_DEVICE) {
		SetLastError(ERROR_INVALID_HANDLE);
		return -1;
	}
	
	if (!device->Opened) {
		SetLastError(ERROR_DEVICE_NOT_OPEN);
		return -1;
	}
	
	if (!device->Release) {
		SetLastError(ERROR_INVALID_FUNCTION);
		return -1;
	}

	// �رմ򿪱�־����ֹ�����̵߳ȴ��ź���
	device->Opened = false;
	
	// �ȴ���ͬһʱ��ֻ��һ��������ã���������߳����ڴ���͵ȴ�
	if (device->Sem != NULL && OSRunning) {
		OSSemPend(device->Sem, 0, &err);
	}

	// �ͷ��豸
	device->Offset = 0;
	device->MagicNumber = ~MAGIC_NUMBER_DEVICE;
	device->Release(device);
	
	// ɾ���ź���
	if (device->Sem != NULL && OSRunning) {
		do {
			OSSemDel(device->Sem, OS_DEL_NO_PEND, &err);
			if (err != OS_NO_ERR) {
				OSTimeDly(1);
			}
		} while (err != OS_NO_ERR);
		
		device->Sem = NULL;
	}
	
	SetLastError(NO_ERROR);
	return 0;
}

/* ------------------------------------------------------------------------
 *  ��������
 *      int ReadDevice(THandle handle, byte *buffer, UINT length)
 *  ���ܣ�
 *      ���豸��ȡ����
 *  ������
 *      handle          ----    ���������豸���
 *      buffer          ----    ��Ŷ�ȡ���ݵĻ�����
 *      length          ----    Ҫ��ȡ�ĳ���
 *  ����ֵ��
 *      ����0��ʾʵ�ʶ�ȡ���ֽ�����С�����ʾʧ��
 *  ˵����
 *      ����Ĵ��������ͨ��GetLastError��ȡ
 *      --------------------------------------------------------
 *    	ERROR_INVALID_HANDLE    ----    �Ƿ����豸���
 *      ERROR_DEVICE_NOT_OPEN   ----    �豸û�д�
 *      ERROR_INVALID_FUNCTION  ----    ����û��ʵ��read����
 *      NO_ERROR                ----    �����ɹ�
 * -----------------------------------------------------------------------*/

int ReadDevice(THandle handle, byte *buffer, UINT length)
{
	TDevice * device = (TDevice *)handle;
	int ret;
	byte err;
	
	if (device == NULL || device->MagicNumber != MAGIC_NUMBER_DEVICE) {
		SetLastError(ERROR_INVALID_HANDLE);
		return -1;
	}
	
	if (!device->Opened) {
		SetLastError(ERROR_DEVICE_NOT_OPEN);
		return -1;
	}

	if (!device->Read) {
		SetLastError(ERROR_INVALID_FUNCTION);
		return -1;
	}
	
	SetLastError(NO_ERROR);
	
	// �ȴ���ͬһʱ��ֻ��һ���������
	if (device->Sem != NULL && OSRunning) {
		OSSemPend(device->Sem, 0, &err);
	}
	
	ret = device->Read(device, buffer, length);
	
	if (ret > 0)
		device->Offset += ret;
	
	// �ͷţ�ͬһʱ��ֻ��һ���������
	if (device->Sem != NULL && OSRunning) {
		OSSemPost(device->Sem, &err);
	}
	
	return ret;
}

/* ------------------------------------------------------------------------
 *  ��������
 *      int WriteDevice(THandle handle, const byte *buffer, UINT length)
 *  ���ܣ�
 *      ���豸д������
 *  ������
 *      handle          ----    ���������豸���
 *      buffer          ----    Ҫд������ݵĻ�����
 *      length          ----    Ҫд��ĳ���
 *  ����ֵ��
 *      ����0��ʾʵ��д����ֽ�����С�����ʾʧ��
 *  ˵����
 *      ����Ĵ��������ͨ��GetLastError��ȡ
 *      --------------------------------------------------------
 *    	ERROR_INVALID_HANDLE    ----    �Ƿ����豸���
 *      ERROR_DEVICE_NOT_OPEN   ----    �豸û�д�
 *      ERROR_INVALID_FUNCTION  ----    ����û��ʵ��write����
 *      NO_ERROR                ----    �����ɹ�
 * -----------------------------------------------------------------------*/

int WriteDevice(THandle handle, const byte *buffer, UINT length)
{
	TDevice * device = (TDevice *)handle;
	int ret;
	byte err;
	
	if (device == NULL || device->MagicNumber != MAGIC_NUMBER_DEVICE) {
		SetLastError(ERROR_INVALID_HANDLE);
		return -1;
	}

	if (!device->Opened) {
		SetLastError(ERROR_DEVICE_NOT_OPEN);
		return -1;
	}
	
	if (!device->Write) {
		SetLastError(ERROR_INVALID_FUNCTION);
		return -1;
	}
	
	SetLastError(NO_ERROR);

	// �ȴ���ͬһʱ��ֻ��һ���������
	if (device->Sem != NULL && OSRunning) {
		OSSemPend(device->Sem, 0, &err);
	}

	ret = device->Write(device, buffer, length);
	
	if (ret > 0) {
		device->Offset += ret;
	}
	
	// �ͷţ�ͬһʱ��ֻ��һ���������
	if (device->Sem != NULL && OSRunning) {
		OSSemPost(device->Sem, &err);
	}

	return ret;
}

/* ------------------------------------------------------------------------
 *  ��������
 *      int SeekDevice(THandle handle, ulong offset)
 *  ���ܣ�
 *      ��λ�ļ���ָ��
 *  ������
 *      handle          ----    ���������豸���
 *      offset          ----    ��λ��λ��
 *  ����ֵ��
 *      ��λǰ��λ�ã�-1��ʾʧ��
 *  ˵����
 *      ����Ĵ��������ͨ��GetLastError��ȡ
 *      --------------------------------------------------------
 *    	ERROR_INVALID_HANDLE    ----    �Ƿ����豸���
 *      ERROR_DEVICE_NOT_OPEN   ----    �豸û�д�
 *      ERROR_INVALID_FUNCTION  ----    ����û��ʵ��write����
 *      NO_ERROR                ----    �����ɹ�
 * -----------------------------------------------------------------------*/

int SeekDevice(THandle handle, ulong offset)
{
	TDevice * device = (TDevice *)handle;
	int ret;
	byte err;
	
	if (device == NULL || device->MagicNumber != MAGIC_NUMBER_DEVICE) {
		SetLastError(ERROR_INVALID_HANDLE);
		return -1;
	}

	if (!device->Opened) {
		SetLastError(ERROR_DEVICE_NOT_OPEN);
		return -1;
	}
	
	if (!device->Seek) {
		SetLastError(ERROR_INVALID_FUNCTION);
		return -1;
	}
	
	SetLastError(NO_ERROR);
	
	// �ȴ���ͬһʱ��ֻ��һ���������
	if (device->Sem != NULL && OSRunning) {
		OSSemPend(device->Sem, 0, &err);
	}

	ret = device->Seek(device, offset);

	// �ͷţ�ͬһʱ��ֻ��һ���������
	if (device->Sem != NULL && OSRunning) {
		OSSemPost(device->Sem, &err);
	}

	return ret;
}

// ----------------------------------------------------------------------------
int IoctlDevice(THandle handle, ulong cmd, void *arg)
{
	TDevice * device = (TDevice *)handle;
	int ret;
	byte err;
	
	if (device == NULL || device->MagicNumber != MAGIC_NUMBER_DEVICE) {
		SetLastError(ERROR_INVALID_HANDLE);
		return -1;
	}

	if (!device->Opened) {
		SetLastError(ERROR_DEVICE_NOT_OPEN);
		return -1;
	}

	if (!device->Ioctl) {
		SetLastError(ERROR_INVALID_FUNCTION);
		return -1;
	}
	
	SetLastError(NO_ERROR);

	// �ȴ���ͬһʱ��ֻ��һ���������
	if (device->Sem != NULL && OSRunning) {
		OSSemPend(device->Sem, 0, &err);
	}

	ret = device->Ioctl(device, cmd, arg);

	// �ͷţ�ͬһʱ��ֻ��һ���������
	if (device->Sem != NULL && OSRunning) {
		OSSemPost(device->Sem, &err);
	}

	return ret;
}

// ----------------------------------------------------------------------------
// E N D
