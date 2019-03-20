
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
 *      void DrawBitmap(TPoint pt, TSize size, const byte * buffer)
 *  ���ܣ�
 *      ����Ļ����ʾһ��λͼ
 *  ������
 *      pt              ----    λͼ���Ͻǵ�λ��
 *      size            ----    λͼ�ĳߴ�
 *      buffer          ----    λͼ������
 *  ����ֵ��
 *      ��
 *  ˵����
 *      ע�ⲻҪ������Ļ�ߴ�
 * -----------------------------------------------------------------------*/

void DrawBitmap(TPoint pt, TSize size, const byte * buffer, bool reverse)
{
	int i, j, x, y;
	bool b;

	y = pt.y + size.cy - 1;
	for (i = 0; i < size.cy; i++) {
		x = pt.x;
		for (j = 0; j < size.cx; j++) {
			b = (buffer[(i * size.cx + j) / 8] >> (7 - j % 8)) & 0x01;
			SetPixel(x, y, reverse ? !b : b);
			x++;
		}
		y--;
	}
}
