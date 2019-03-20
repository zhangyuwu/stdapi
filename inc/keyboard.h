
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/

/* ------------------------------------------------------------------------
 * 输入函数：
 *     1. void OSTimeDly(int);
 * 输出函数：
 *     1. void isr_keyboard(char value)，在中断中调用
 * -----------------------------------------------------------------------*/

#ifndef __Keyboard_H__
#define __Keyboard_H__

#include <types.h>
#include <device.h>

// 键盘操作函数集合
bool OpenKeyboard(struct TDevice *dev);
int ReadKeyboard(struct TDevice *dev, byte *buffer, UINT count);

void isr_keyboard();

#endif
