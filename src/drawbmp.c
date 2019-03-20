
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
 *  函数名：
 *      void DrawBitmap(TPoint pt, TSize size, const byte * buffer)
 *  功能：
 *      在屏幕上显示一个位图
 *  参数：
 *      pt              ----    位图左上角的位置
 *      size            ----    位图的尺寸
 *      buffer          ----    位图的数据
 *  返回值：
 *      无
 *  说明：
 *      注意不要超过屏幕尺寸
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
