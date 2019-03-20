
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/


#ifndef __Font_H__
#define __Font_H__

#include <types.h>
#include <stdio.h>

#define MAX_FONTS				8			// 最多能注册的字体种类
#define MAX_FONT_NAME_LENGTH	12			// 设备名最长允许12字符
#define FONT_TYPE_ANSI			0x01		// 字体类型 Ansi
#define FONT_TYPE_WIDE			0x02		// 字体类型 Wide

#define iswchar(ch)				(ch > 0x80)	// 判断是否宽字符

typedef struct TFont {
	UINT	MagicNumber;
	char	Name[MAX_FONT_NAME_LENGTH];
	int		Width;
	int		Height;
	int		ItemSize;
	void *	StartAddress;
} TFont;

bool RegisterFont(TFont *font);
bool UnregisterFont(const char *name);
TFont *GetCurrentFont(int type);
bool SetCurrentFont(int type, TFont *font);
bool SelectCurrentFont(int type, const char *name);
bool LoadCharacter(wchar_t ch, unsigned char *buffer, int *width, int *height);
void InstallSystemFont();

#endif
