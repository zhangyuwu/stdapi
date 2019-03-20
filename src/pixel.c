
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
 *      void Pixel(int x, int y)
 *  ���ܣ�
 *      ����Ļ�ϻ���������һ����
 *  ������
 *      x               ----    x ���꣨0 �� 128��
 *      y               ----    y ���꣨0 �� 64��
 *  ����ֵ��
 *      ��
 *  ˵����
 *      ��
 * -----------------------------------------------------------------------*/
 
void Pixel(int x, int y)
{
	TColor color = GetCurrentColor(COLOR_FOREGROUND);

	switch (GetDrawMode()) {
	case DRAW_MODE_NORMAL:
		break;
		
	case DRAW_MODE_AND:
		color &= GetPixel(x, y);
		break;
		
	case DRAW_MODE_OR:
		color |= GetPixel(x, y);
		break;
		
	case DRAW_MODE_XOR:
		color ^= GetPixel(x, y);
		break;
	}
	
	SetPixel(x, y, color);
}
