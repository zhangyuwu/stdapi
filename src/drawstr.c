
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
 *  ��������
 *      void DisplayCharBuffer(int x, int y, const byte *buffer, int width, int height)
 *  ���ܣ�
 *      ����Ļ����ʾһ���ַ�
 *  ������
 *      x               ----    �ַ���ʾ��ˮƽ����
 *      y               ----    �ַ���ʾ�Ĵ�ֱ����
 *      buffer          ----    �ַ������ݣ�Ҫ����ˮƽ��֯���ݣ�
 *      width           ----    �ַ��Ŀ��
 *      height          ----    �ַ��ĸ߶�
 *  ����ֵ��
 *      ��
 *  ˵����
 *      ��ˮƽ��֯��������µ�һ������
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
			// ����ǰ�����ֽ�
			ch = buffer[(i * width + j) / 8];
			
			// �Ƿ�����Ϣ����Ϊ�����ÿ���ֽ�����Ǹ�λ��������Ҫ��7���������������Ҿ��������
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
// ����: ��͸���ɵ�һ��32λ��������16λΪ����16λΪ��

int DisplayString(int x, int y, const char *str)
{
	int width, height, xsize = 0;
	int i = 0, len = strlen(str);
	wchar_t ch;
	
	while (str[i] && i < len) {
		// ����ǿ��ַ�, ��Ҫ������ANSI�ַ�ƴ�ճ�һ�����ַ�
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
