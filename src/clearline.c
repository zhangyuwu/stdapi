
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

void ClearLine(int y)
{
	TColor fcolor = GetCurrentColor(COLOR_FOREGROUND);
	TColor bcolor = GetCurrentColor(COLOR_BACKGROUND);
	TFont *font = GetCurrentFont(FONT_TYPE_ANSI);
	int width, height;
	
	GetScreenSize(&width, &height);

	if (font) {
		SetCurrentColor(COLOR_FOREGROUND, bcolor);
		FillRectangle(0, y, width - 1, y + font->Height);
		SetCurrentColor(COLOR_FOREGROUND, fcolor);
	}
}
