
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

// ��ȡ��Ļ�ߴ�
void GetScreenSize(int *width, int *height);

// ����ͼ�ι���
COLORREF GetPixel(int x, int y);
void SetPixel(int x, int y, COLORREF color);

// ��Ļ��������
void RefreshScreen();
void ClearScreen();
void GetScreen(void *buffer);
void PutScreen(const void *buffer);
void ScrollScreen(int dir, int step);

// �豸����
bool OpenLCD(struct TDevice *dev);
int WriteLCD(struct TDevice *dev, const byte *buffer, UINT count);
void refresh_lcd();

// DOS ���ݺ���
int wherex();
int wherey();
void getxy(int *x, int *y);
void gotoxy(int x, int y);
void clrscr();

#endif // __LCD_H__
