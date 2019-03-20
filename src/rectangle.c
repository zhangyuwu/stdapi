
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
 *      void Rectangle(int x1, int y1, int x2, int y2)
 *  功能：
 *      画一个矩形
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

void Rectangle(int x1, int y1, int x2, int y2)
{
    Line(x1, y1, x1, y2);
    Line(x1, y1, x2, y1);
    Line(x2, y1, x2, y2);
    Line(x1, y2, x2, y2);
}

/* ------------------------------------------------------------------------
 *  函数名：
 *      void FillRectangle(int x1, int y1, int x2, int y2)
 *  功能：
 *      填充一个矩形
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
