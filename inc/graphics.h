
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/

/* ------------------------------------------------------------------------
 * ���������� 2 ��������
 *     1. COLORREF GetPixel(int x, int y);
 *     2. void SetPixel(int x, int y, COLORREF color);
 * -----------------------------------------------------------------------*/

#ifndef __Graphics_H__
#define __Graphics_H__

#include <types.h>
#include <font.h>
#include <lcd.h>
#include <gutil.h>

typedef struct TPoint {
	int x;
	int y;
} TPoint;

typedef struct TSize {
	int cx;
	int cy;
} TSize;

typedef struct TRect {
	int left;
	int top;
	int right;
	int bottom;
} TRect;

// ͼ����ʾ
#define RGB(r,g,b) ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))

// 16 �ֳ���ɫ��
typedef enum {
	BLACK         = RGB(0X00, 0X00, 0X00),
	DARKRED       = RGB(0X80, 0X00, 0X00),
	DARKGREEN     = RGB(0X00, 0X80, 0X00),
	DARKYELLOW    = RGB(0X80, 0X80, 0X00),
	DARKBLUE      = RGB(0X00, 0X00, 0X80),
	DARKMAGENTA   = RGB(0X80, 0X00, 0X80),
	DARKCYAN      = RGB(0X00, 0X80, 0X80),
	LIGHTTGRAY    = RGB(0XC0, 0XC0, 0XC0),
	GRAY          = RGB(0X80, 0X80, 0X80),
	RED           = RGB(0XFF, 0X00, 0X00),
	GREEN         = RGB(0X00, 0XFF, 0X00),
	YELLOW        = RGB(0XFF, 0XFF, 0X00),
	BLUE          = RGB(0X00, 0X00, 0XFF),
	MAGENTA       = RGB(0XFF, 0X00, 0XFF),
	CYAN          = RGB(0X00, 0XFF, 0XFF),
	WHITE         = RGB(0XFF, 0XFF, 0XFF),
} TColor;

// ��ͼ��ʾģʽ
typedef enum {
	DRAW_MODE_NORMAL     	= 0,
	DRAW_MODE_AND        	= 1,
	DRAW_MODE_OR         	= 2,
	DRAW_MODE_XOR        	= 4,
} TDrawMode;

// ���ߵķ��
typedef enum {
    LINE_STYLE_SOLID   		= 0,
    LINE_STYLE_DOTTED  		= 1,
    LINE_STYLE_CENTER  		= 2,
    LINE_STYLE_DASHED  		= 3,
	LINE_STYLE_MAX
} TLineStyle;

// �������ķ��
typedef enum {
	PROGRESS_BAR_STYLE_HORIZONTAL,
	PROGRESS_BAR_STYLE_VERTICAL,
} TProgressBarStyle;

// ǰ����ɫ�ͱ�����ɫ
enum {
	COLOR_FOREGROUND		= 1,
	COLOR_BACKGROUND		= 2,
};

// ����
enum {
	DIRECTION_UP			= 1,
	DIRECTION_DOWN			= 2,
	DIRECTION_LEFT			= 3,
	DIRECTION_RIGHT			= 4,
};

enum {
	AL_CENTER				= 1,
	AL_LEFT					= 2,
	AL_RIGHT				= 3,
	AL_MASK					= 0x0F,
};

#define DL_MODE_CLEARLINE	0x10

TColor GetCurrentColor(int type);
void SetCurrentColor(int type, TColor color);
TDrawMode GetDrawMode();
void SetDrawMode(TDrawMode mode);
TLineStyle GetLineStyle();
void SetLineStyle(TLineStyle style);

int GetRectWidth(TRect *rect);
int GetRectHeight(TRect *rect);

void InitializeGraphics(int width, int height, COLORREF (*GetPixel)(int, int), void (*SetPixel)(int, int, COLORREF));
void Pixel(int x, int y);
void Line(int x1, int y1, int x2, int y2);
void Rectangle(int x1, int y1, int x2, int y2);
void FillRectangle(int x1, int y1, int x2, int y2);
void Ellipse(int a, int b, int r1, int r2);
void FillEllipse(int a, int b, int r1, int r2);
void ClearRectangle(int x1, int y1, int x2, int y2);
void DrawBitmap(TPoint pt, TSize size, const byte * buffer, bool reverse);
void DisplayChar(int x, int y, wchar_t ch);
int DisplayString(int x, int y, const char *str);
void DisplayLine(int y, const char * title, int mode);
void ShowProgress(TRect *area, int percent, TProgressBarStyle style);

#endif
