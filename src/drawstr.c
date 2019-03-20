
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/
 
#include <stdio.h>
#include <string.h>
#include <graphics.h>
#include <system.h>

byte g_FontBuffer[256];

/* ------------------------------------------------------------------------
 *  函数名：
 *      void DisplayCharBuffer(int x, int y, const byte *buffer, int width, int height)
 *  功能：
 *      在屏幕上显示一个字符
 *  参数：
 *      x               ----    字符显示的水平坐标
 *      y               ----    字符显示的垂直坐标
 *      buffer          ----    字符的数据（要求按照水平组织数据）
 *      width           ----    字符的宽度
 *      height          ----    字符的高度
 *  返回值：
 *      无
 *  说明：
 *      在水平组织数据情况下的一个汉字
 *       0 [0 1 2 3 4 5 6 7]  1 [8 9 A B C D E F]
 *       2 [0 1 2 3 4 5 6 7]  3 [8 9 A B C D E F]
 *       4 [0 1 2 3 4 5 6 7]  5 [8 9 A B C D E F]
 *       6 [0 1 2 3 4 5 6 7]  7 [8 9 A B C D E F]
 *       8 [0 1 2 3 4 5 6 7]  9 [8 9 A B C D E F]
 *      10 [0 1 2 3 4 5 6 7] 11 [8 9 A B C D E F]
 *      12 [0 1 2 3 4 5 6 7] 13 [8 9 A B C D E F]
 *      14 [0 1 2 3 4 5 6 7] 15 [8 9 A B C D E F]
 *      16 [0 1 2 3 4 5 6 7] 17 [8 9 A B C D E F]
 *      18 [0 1 2 3 4 5 6 7] 19 [8 9 A B C D E F]
 *      20 [0 1 2 3 4 5 6 7] 21 [8 9 A B C D E F]
 *      22 [0 1 2 3 4 5 6 7] 23 [8 9 A B C D E F]
 *      24 [0 1 2 3 4 5 6 7] 25 [8 9 A B C D E F]
 *      26 [0 1 2 3 4 5 6 7] 27 [8 9 A B C D E F]
 *      28 [0 1 2 3 4 5 6 7] 29 [8 9 A B C D E F]
 *      30 [0 1 2 3 4 5 6 7] 31 [8 9 A B C D E F]
 * -----------------------------------------------------------------------*/
void DisplayCharBuffer(int x, int y, const byte *buffer, int width, int height)
{
	int i, j;
	byte ch;
	
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			// 区当前所在字节
			ch = buffer[(i * width + j) / 8];
			
			// 是否有信息，因为字体的每个字节左边是高位，所以需要用7减，否则会出现左右镜像的现象
			if (bit_tst(ch, 7 - (i * width + j) % 8)) {
				SetPixel(x + j, y + i, GetCurrentColor(COLOR_FOREGROUND));
			}
			else {
				SetPixel(x + j, y + i, GetCurrentColor(COLOR_BACKGROUND));
			}
		}
	}
}

// ----------------------------------------------------------------------------
void DisplayChar(int x, int y, wchar_t ch)
{
	int width, height;
	
	if (LoadCharacter(ch, g_FontBuffer, &width, &height)) {
		DisplayCharBuffer(x, y, g_FontBuffer, width, height);
	}
}

// ----------------------------------------------------------------------------
// 返回: 宽和高组成的一个32位的数，高16位为宽，低16位为高

int DisplayString(int x, int y, const char *str)
{
	int width, height, xsize = 0;
	int i = 0, len = strlen(str);
	wchar_t ch;
	
	while (str[i] && i < len) {
		// 如果是宽字符, 需要把两个ANSI字符拼凑成一个宽字符
		if (iswchar(str[i])) {
			ch = MAKEWORD16(str[i], str[i+1]);
			i += 2;
		}
		else {
			ch = str[i];
			i++;
		}
		
		if (LoadCharacter(ch, g_FontBuffer, &width, &height)) {
			DisplayCharBuffer(x, y, g_FontBuffer, width, height);
			x += width;
			xsize += width;
		}		
	}
	
	return MAKEWORD32(xsize, height);
}
