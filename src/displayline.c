
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
 *  ��������
 *      void DisplayLine(int y, const char * title, int alignMode)
 *  ���ܣ�
 *      ����ָ��ģʽ��ʾһ�У����Է��ԣ�
 *  ������
 *      y               ----    0 ~ 64
 *      title           ----    Ҫ��ʾ�ı���
 *      mode            ----    ����ģʽ
 *                                �� AL_CENTER        ����
 *                                �� AL_LEFT          ����
 *                                �� AL_RIGHT         ����
 *  ����ֵ��
 *      ��
 *  ˵����
 *      ��
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

	// �����ַ����Ŀ��
	len = font->Width * strlen(title);
	
	// �������ʾ����
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
