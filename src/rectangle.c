
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/
 
#include <graphics.h>
#include <stdio.h>
#include <system.h>

/* ------------------------------------------------------------------------
 *  ��������
 *      void Rectangle(int x1, int y1, int x2, int y2)
 *  ���ܣ�
 *      ��һ������
 *  ������
 *      x1              ----    ��ʼx����
 *      y1              ----    ��ʼy����
 *      x2              ----    ��ֹx����
 *      y2              ----    ��ֹy����
 *  ����ֵ��
 *      ��
 *  ˵����
 *      ��
 * -----------------------------------------------------------------------*/

void Rectangle(int x1, int y1, int x2, int y2)
{
    Line(x1, y1, x1, y2);
    Line(x1, y1, x2, y1);
    Line(x2, y1, x2, y2);
    Line(x1, y2, x2, y2);
}

/* ------------------------------------------------------------------------
 *  ��������
 *      void FillRectangle(int x1, int y1, int x2, int y2)
 *  ���ܣ�
 *      ���һ������
 *  ������
 *      x1              ----    ��ʼx����
 *      y1              ----    ��ʼy����
 *      x2              ----    ��ֹx����
 *      y2              ----    ��ֹy����
 *  ����ֵ��
 *      ��
 *  ˵����
 *      ��
 * -----------------------------------------------------------------------*/

void FillRectangle(int x1, int y1, int x2, int y2)
{
    int i;
    if (y1 < y2)
        for (i = y1;i <= y2;i++)
            Line(x1, i, x2, i);
    if (y1 > y2)
        for (i = y1;i >= y2;i--)
            Line(x1, i, x2, i);
}

void ClearRectangle(int x1, int y1, int x2, int y2)
{
	TColor color = GetCurrentColor(COLOR_FOREGROUND);

	SetCurrentColor(COLOR_FOREGROUND, GetCurrentColor(COLOR_BACKGROUND));
	FillRectangle(x1, y1, x2, y2);
	SetCurrentColor(COLOR_FOREGROUND, color);
}
