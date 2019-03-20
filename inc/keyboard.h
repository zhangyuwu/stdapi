
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/

/* ------------------------------------------------------------------------
 * ���뺯����
 *     1. void OSTimeDly(int);
 * ���������
 *     1. void isr_keyboard(char value)�����ж��е���
 * -----------------------------------------------------------------------*/

#ifndef __Keyboard_H__
#define __Keyboard_H__

#include <types.h>
#include <device.h>

// ���̲�����������
bool OpenKeyboard(struct TDevice *dev);
int ReadKeyboard(struct TDevice *dev, byte *buffer, UINT count);

void isr_keyboard();

#endif
