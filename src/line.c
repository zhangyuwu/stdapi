
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
 *   增加线条的风格：LINE_STYLE_SOLID, LINE_STYLE_DOTTED, LINE_STYLE_CENTER
 *                   LINE_STYLE_DASHED
 *   DATE: 2006-02-05 15:15:07
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <system.h>

/* ------------------------------------------------------------------------
 *  函数名：
 *      void Line(int x1, int y1, int x2, int y2)
 *  功能：
 *      画一条直线
 *  参数：
 *      x1              ----    起始x坐标
 *      y1              ----    起始y坐标
 *      x2              ----    终止x坐标
 *      y2              ----    终止y坐标
 *  返回值：
 *      无
 *  说明：
 *      无
 * -----------------------------------------------------------------------*/

void DrawLinePixel(int x, int y, int *seq)
{
	switch (GetLineStyle()) {
	case LINE_STYLE_DOTTED:
		if (*seq % 2) {
			Pixel(x, y);
		}
		break;
	case LINE_STYLE_CENTER:
		if ((*seq % 6) % 4) {
			Pixel(x, y);
		}
		break;
	case LINE_STYLE_DASHED:
		if (*seq % 4) {
			Pixel(x, y);
		}
		break;
	default:
		Pixel(x, y);
		break;
	}
	
	*seq += 1;
}

void Line(int x1, int y1, int x2, int y2)
{
    int x, y, xend, yend, dx, dy, p, flag = 0, temp, seq = 1;
	
	if (x1 == x2) {
		x = x1;
		y = y1;
		dy = (y1 < y2 ? 1 : -1);
		
		while (true) {
			DrawLinePixel(x, y, &seq);
			if (y == y2) break;
			y += dy;
		}
	}
	else if (y1 == y2)
	{
		x = x1;
		y = y1;
		dx = (x1 < x2 ? 1 : -1);
		
		while (true) {
			DrawLinePixel(x, y, &seq);
			if (x == x2) break;
			x += dx;
		}
	}
	else {
		dx = abs(x1 - x2);
		dy = abs(y1 - y2);
		if (dx > dy) {
			p = (2 * dy - dx);
			if (x1 > x2) {
				temp = x1;
				x1 = x2;
				x2 = temp;
				temp = y1;
				y1 = y2;
				y2 = temp;
				if (y1 < y2)
					flag = 0;
				else
					flag = 1;
			} else {
				if (y1 < y2)
					flag = 0;
				else
					flag = 1;
			}
			x = x1;
			y = y1;
			xend = x2;
			DrawLinePixel(x, y, &seq);
			while (x < xend) {
				x += 1;
				if (p < 0) {
					p += 2 * dy;
				} else {
					if (flag == 0)
						y += 1;
					else
						y -= 1;
					p += 2 * (dy - dx);
				}
				DrawLinePixel(x, y, &seq);
			}
		} else {
			p = (2 * dx - dy);
			if (y1 > y2) {
				temp = y1;
				y1 = y2;
				y2 = temp;
				temp = x1;
				x1 = x2;
				x2 = temp;
				if (x1 < x2)
					flag = 0;
				else
					flag = 1;
			} else {
				if (x1 < x2)
					flag = 0;
				else
					flag = 1;
			}
			x = x1;
			y = y1;
			yend = y2;
			DrawLinePixel(x, y, &seq);
			while (y < yend) {
				y += 1;
				if (p < 0) {
					p += 2 * dx;
				} else {
					if (flag == 0)
						x += 1;
					else
						x -= 1;
					p += 2 * (dx - dy);
				}
				DrawLinePixel(x, y, &seq);
			}
		}
	}
}

