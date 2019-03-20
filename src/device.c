
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
 *     在 CloseDevie 中增加了保护，先关闭 Opened 标志，如果其他线程正在操作设备，
 * 就等待那些操作完成后再释放设备。
 *
 */

#include <stdio.h>
#include <string.h>
#include <system.h>
#include <device.h>
#include <ucos_ii.h>
#include <magic.h>

// ==========================================================================
//   驱动程序构架
// ==========================================================================

TDevice g_Devices[MAX_DEVICES];

/* ------------------------------------------------------------------------
 *  函数名：
 *      bool RegisterDevice(TDevice *device)
 *  功能：
 *      注册设备驱动
 *  参数：
 *      device          ----    要注册的设备结构指针
 *  返回值：
 *      成功或失败
 *  说明：
 *      具体的错误码可以通过GetLastError获取
 *      --------------------------------------------------------
 *    	ERROR_INVALID_NAME      ----    设备名称无效
 *      ERROR_FILE_EXISTS       ----    设备已经被注册
 *      ERROR_NO_FREE_SPACE     ----    注册的设备已经达到最大值
 *      ERROR_INVALID_HANDLE    ----    无效的设备句柄
 *      NO_ERROR                ----    操作成功
 * -----------------------------------------------------------------------*/

bool RegisterDevice(TDevice *device)
{
	int i, freeIndex = -1;
	static bool dev_inited = false;
	
	if (!dev_inited) {
		memset(g_Devices, 0, sizeof(g_Devices));
		dev_inited = true;
	}
	
	// 判断要注册的设备是否合格
	if (device->Name[0] == '\0') {
		SetLastError(ERROR_INVALID_NAME);
		return false;
	}
	
	// 查找设备是否被注册
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
	
	// 没有空闲的设备结构
	if (freeIndex == -1) {
		SetLastError(ERROR_NO_FREE_SPACE);
		return false;
	}
	
	// 复制设备结构
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
 *  函数名：
 *      bool UnregisterDevice(const char *name)
 *  功能：
 *      卸载设备驱动
 *  参数：
 *      name            ----    要卸载的设备名称
 *  返回值：
 *      成功或失败
 *  说明：
 *      具体的错误码可以通过GetLastError获取
 *      --------------------------------------------------------
 *    	ERROR_DEVICE_BUSY       ----    设备忙
 *      ERROR_NOT_REGISTERED    ----    设备没有被注册
 *      NO_ERROR                ----    操作成功
 * -----------------------------------------------------------------------*/

bool UnregisterDevice(const char *name)
{
	int i;
	
	// 搜索注册的设备
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
 *  函数名：
 *      THandle OpenDevice(const char *name, int option)
 *  功能：
 *      打开设备
 *  参数：
 *      name            ----    要打开的设备名称
 *      option          ----    打开的参数
 *  返回值：
 *      打开的设备句柄
 *  说明：
 *      具体的错误码可以通过GetLastError获取
 *      --------------------------------------------------------
 *    	ERROR_DEVICE_BUSY       ----    设备忙
 *      ERROR_FILE_NOT_FOUND    ----    设备没有被注册
 *      NO_ERROR                ----    操作成功
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
 *  函数名：
 *      int CloseDevice(THandle handle)
 *  功能：
 *      关闭设备
 *  参数：
 *      handle          ----    要关闭的设备句柄
 *  返回值：
 *      无
 *  说明：
 *      具体的错误码可以通过GetLastError获取
 *      --------------------------------------------------------
 *    	ERROR_INVALID_HANDLE    ----    非法的设备句柄
 *      ERROR_DEVICE_NOT_OPEN   ----    设备没有打开
 *      ERROR_INVALID_FUNCTION  ----    驱动没有实现release方法
 *      NO_ERROR                ----    操作成功
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

	// 关闭打开标志，防止其他线程等待信号量
	device->Opened = false;
	
	// 等待，同一时间只能一个任务调用，如果其他线程正在处理就等待
	if (device->Sem != NULL && OSRunning) {
		OSSemPend(device->Sem, 0, &err);
	}

	// 释放设备
	device->Offset = 0;
	device->MagicNumber = ~MAGIC_NUMBER_DEVICE;
	device->Release(device);
	
	// 删除信号量
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
 *  函数名：
 *      int ReadDevice(THandle handle, byte *buffer, UINT length)
 *  功能：
 *      从设备读取数据
 *  参数：
 *      handle          ----    被操作的设备句柄
 *      buffer          ----    存放读取数据的缓冲区
 *      length          ----    要读取的长度
 *  返回值：
 *      大于0表示实际读取的字节数，小于零表示失败
 *  说明：
 *      具体的错误码可以通过GetLastError获取
 *      --------------------------------------------------------
 *    	ERROR_INVALID_HANDLE    ----    非法的设备句柄
 *      ERROR_DEVICE_NOT_OPEN   ----    设备没有打开
 *      ERROR_INVALID_FUNCTION  ----    驱动没有实现read方法
 *      NO_ERROR                ----    操作成功
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
	
	// 等待，同一时间只能一个任务调用
	if (device->Sem != NULL && OSRunning) {
		OSSemPend(device->Sem, 0, &err);
	}
	
	ret = device->Read(device, buffer, length);
	
	if (ret > 0)
		device->Offset += ret;
	
	// 释放，同一时间只能一个任务调用
	if (device->Sem != NULL && OSRunning) {
		OSSemPost(device->Sem, &err);
	}
	
	return ret;
}

/* ------------------------------------------------------------------------
 *  函数名：
 *      int WriteDevice(THandle handle, const byte *buffer, UINT length)
 *  功能：
 *      往设备写入数据
 *  参数：
 *      handle          ----    被操作的设备句柄
 *      buffer          ----    要写入的数据的缓冲区
 *      length          ----    要写入的长度
 *  返回值：
 *      大于0表示实际写入的字节数，小于零表示失败
 *  说明：
 *      具体的错误码可以通过GetLastError获取
 *      --------------------------------------------------------
 *    	ERROR_INVALID_HANDLE    ----    非法的设备句柄
 *      ERROR_DEVICE_NOT_OPEN   ----    设备没有打开
 *      ERROR_INVALID_FUNCTION  ----    驱动没有实现write方法
 *      NO_ERROR                ----    操作成功
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

	// 等待，同一时间只能一个任务调用
	if (device->Sem != NULL && OSRunning) {
		OSSemPend(device->Sem, 0, &err);
	}

	ret = device->Write(device, buffer, length);
	
	if (ret > 0) {
		device->Offset += ret;
	}
	
	// 释放，同一时间只能一个任务调用
	if (device->Sem != NULL && OSRunning) {
		OSSemPost(device->Sem, &err);
	}

	return ret;
}

/* ------------------------------------------------------------------------
 *  函数名：
 *      int SeekDevice(THandle handle, ulong offset)
 *  功能：
 *      定位文件的指针
 *  参数：
 *      handle          ----    被操作的设备句柄
 *      offset          ----    定位的位置
 *  返回值：
 *      定位前的位置，-1表示失败
 *  说明：
 *      具体的错误码可以通过GetLastError获取
 *      --------------------------------------------------------
 *    	ERROR_INVALID_HANDLE    ----    非法的设备句柄
 *      ERROR_DEVICE_NOT_OPEN   ----    设备没有打开
 *      ERROR_INVALID_FUNCTION  ----    驱动没有实现write方法
 *      NO_ERROR                ----    操作成功
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
	
	// 等待，同一时间只能一个任务调用
	if (device->Sem != NULL && OSRunning) {
		OSSemPend(device->Sem, 0, &err);
	}

	ret = device->Seek(device, offset);

	// 释放，同一时间只能一个任务调用
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

	// 等待，同一时间只能一个任务调用
	if (device->Sem != NULL && OSRunning) {
		OSSemPend(device->Sem, 0, &err);
	}

	ret = device->Ioctl(device, cmd, arg);

	// 释放，同一时间只能一个任务调用
	if (device->Sem != NULL && OSRunning) {
		OSSemPost(device->Sem, &err);
	}

	return ret;
}

// ----------------------------------------------------------------------------
// E N D
