
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/


#ifndef __LCD_H__
#define __LCD_H__

#include <types.h>
#include <device.h>
#include <system.h>
#include <graphics.h>

// 获取屏幕尺寸
void GetScreenSize(int *width, int *height);

// 基本图形功能
COLORREF GetPixel(int x, int y);
void SetPixel(int x, int y, COLORREF color);

// 屏幕操作函数
void RefreshScreen();
void ClearScreen();
void GetScreen(void *buffer);
void PutScreen(const void *buffer);
void ScrollScreen(int dir, int step);

// 设备驱动
bool OpenLCD(struct TDevice *dev);
int WriteLCD(struct TDevice *dev, const byte *buffer, UINT count);
void refresh_lcd();

// DOS 兼容函数
int wherex();
int wherey();
void getxy(int *x, int *y);
void gotoxy(int x, int y);
void clrscr();

#endif // __LCD_H__
