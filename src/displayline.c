
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/

#include <lcd.h>
#include <font.h>
#include <graphics.h>
#include <string.h>

/* ------------------------------------------------------------------------
 *  函数名：
 *      void DisplayLine(int y, const char * title, int alignMode)
 *  功能：
 *      按照指定模式显示一行（可以反显）
 *  参数：
 *      y               ----    0 ~ 64
 *      title           ----    要显示的标题
 *      mode            ----    对齐模式
 *                                ◎ AL_CENTER        居中
 *                                ◎ AL_LEFT          靠左
 *                                ◎ AL_RIGHT         靠右
 *  返回值：
 *      无
 *  说明：
 *      无
 * -----------------------------------------------------------------------*/
 
void DisplayLine(int y, const char * title, int mode)
{
	int x, len;
	TFont *font = GetCurrentFont(FONT_TYPE_ANSI);
	TColor color = GetCurrentColor(COLOR_FOREGROUND);
	int width, height;
	
	if (font == NULL)
		return;
		
	GetScreenSize(&width, &height);

	// 整个字符串的宽度
	len = font->Width * strlen(title);
	
	// 清除待显示区域
	if (mode & DL_MODE_CLEARLINE) {
		SetCurrentColor(COLOR_FOREGROUND, GetCurrentColor(COLOR_BACKGROUND));
		FillRectangle(0, y, width - 1, y + font->Height);
		SetCurrentColor(COLOR_FOREGROUND, color);
	}
	
	switch(mode & AL_MASK) {
	case AL_RIGHT:
		x = width - len;
		break;

	case AL_CENTER:
		x = (width - len) / 2;
		break;

	default:
		x = 0;
	}

	DisplayString(x, y, title);
}
