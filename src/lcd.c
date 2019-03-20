
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/

#include <C_m40800.h>
#include <graphics.h>
#include <system.h>
#include <string.h>
#include <lcd.h>
#include <hardware.h>
#include <ucos_ii.h>

#define __LCD_TIAN_SHI__

#ifdef __LCD_TIAN_MA__				// 天马 （并行液晶）
#define LCD_WCOMD(cmd)				outp16(LCD_ADDR, cmd)
#define LCD_WDATA(dat)				outp16(LCD_ADDR + 4, dat)
#elif defined __LCD_TIAN_SHI__		// 天石 （串行液晶）
#define LOW_LCD_DATA				(1<<16)
#define LOW_LCD_CLK					(1<<17)
#define LOW_LCD_CS					(1<<18)
#define LOW_LCD_RS					(1<<19)

#define LOW_LCD_DATA_H				( PIO_BASE->PIO_SODR = LOW_LCD_DATA )
#define LOW_LCD_DATA_L				( PIO_BASE->PIO_CODR = LOW_LCD_DATA )

#define LOW_LCD_CLK_H				( PIO_BASE->PIO_SODR = LOW_LCD_CLK )
#define LOW_LCD_CLK_L				( PIO_BASE->PIO_CODR = LOW_LCD_CLK )

#define LOW_LCD_CS_H				( PIO_BASE->PIO_SODR = LOW_LCD_CS )
#define LOW_LCD_CS_L				( PIO_BASE->PIO_CODR = LOW_LCD_CS )

#define LOW_LCD_RS_H				( PIO_BASE->PIO_SODR = LOW_LCD_RS )
#define LOW_LCD_RS_L				( PIO_BASE->PIO_CODR = LOW_LCD_RS )
#define LOW_LCD_DELAY
#endif

#ifdef __LCD_TIAN_SHI__
void LCD_WCOMD(byte cmd)
{
	static unsigned int i;
	
	LOW_LCD_CS_L;
	LOW_LCD_RS_L;

	for(i = 0; i < 8; i++) {
		LOW_LCD_CLK_L;
		LOW_LCD_DELAY;
		
		if ((1 << (7 - i)) & cmd)	
			LOW_LCD_DATA_H;
		else
			LOW_LCD_DATA_L;

		LOW_LCD_DELAY;

		LOW_LCD_CLK_H;
		LOW_LCD_DELAY;
	}
	LOW_LCD_CS_H;
}

void LCD_WDATA(byte data)
{
	static unsigned int i;
	
	LOW_LCD_CS_L;
	LOW_LCD_RS_H;

	for (i = 0; i < 8; i++) {
		LOW_LCD_CLK_L;
		LOW_LCD_DELAY;
		
		if ((1 << (7 - i)) & data)	
			LOW_LCD_DATA_H;
		else
			LOW_LCD_DATA_L;

		LOW_LCD_DELAY;

		LOW_LCD_CLK_H;
		LOW_LCD_DELAY;
	}
	LOW_LCD_CS_H;
}
#endif

// ----------------------------------------------------------------------------
// 使用 16x8 或者 8x8 的字体

#define SCREEN_WIDTH				128
#define SCREEN_HEIGHT				64

typedef struct TScreen {
	int		Width;
	int		Height;
	int		tm_x;
	int		tm_y;
	byte	Buffer[SCREEN_WIDTH][SCREEN_HEIGHT/8];
	byte	Dirty[SCREEN_WIDTH];
	bool	NeedRefresh;
} TScreen;

TScreen Screen;

// ----------------------------------------------------------------------------
void lcd_init()
{
#ifdef __LCD_TIAN_SHI__
	PIO_BASE->PIO_SODR = LOW_LCD_DATA | LOW_LCD_CLK | LOW_LCD_CS | LOW_LCD_RS;
	PIO_BASE->PIO_PER  = LOW_LCD_DATA | LOW_LCD_CLK | LOW_LCD_CS | LOW_LCD_RS;
	PIO_BASE->PIO_OER  = LOW_LCD_DATA | LOW_LCD_CLK | LOW_LCD_CS | LOW_LCD_RS;
#endif

	LCD_WCOMD(0xE2);				// reset	
	LCD_WCOMD(0xA2);				// set lcd driver voltage bias ratio 
#ifdef __LCD_TIAN_SHI__
	LCD_WCOMD(0xC8);				// select COM output scan direction
#else
	LCD_WCOMD(0xC0);				// select COM output scan direction
#endif
	LCD_WCOMD(0x2C);				// set power control on	
	udelay(0x1000);
	LCD_WCOMD(0x2E);				// set power control on	
	udelay(0x1000);
	LCD_WCOMD(0x2F);				// set power control on	
	udelay(0x1000);
	LCD_WCOMD(0x2C);				// set power control on	
	udelay(0x1000);
	LCD_WCOMD(0x2E);				// set power control on	
	udelay(0x1000);
	LCD_WCOMD(0x2F);				// set power control on	
	udelay(0x1000);
	
	LCD_WCOMD(0x24);				// *select internal resister ratio (ra/rb)mode
	LCD_WCOMD(0x81);				// set brightness
#ifdef __LCD_TIAN_SHI__
	LCD_WCOMD(0x1C);				// *20
	LCD_WCOMD(0xA0);				// ADC set display RAM address 
#else
	LCD_WCOMD(0x32);				// *32
	LCD_WCOMD(0xA1);				// ADC set display RAM address 
#endif
	LCD_WCOMD(0x40);				// set display start line 
	LCD_WCOMD(0xA6);				// set display mode normal or reverse

	LCD_WCOMD(0xAf);				// display on
}

// ----------------------------------------------------------------------------
void GetScreenSize(int *width, int *height)
{
	*width = Screen.Width;
	*height = Screen.Height;
}

// ----------------------------------------------------------------------------
inline int wherex()
{
	return Screen.tm_x;
}

// ----------------------------------------------------------------------------
inline int wherey()
{
	return Screen.tm_y;
}

inline void getxy(int *x, int *y)
{
	*x = Screen.tm_x;
	*y = Screen.tm_y;
}

// ----------------------------------------------------------------------------
inline void gotoxy(int x, int y)
{
	Screen.tm_x = x;
	Screen.tm_y = y;
}

void clrscr()
{
	gotoxy(0, 0);
	ClearScreen();
}

// ----------------------------------------------------------------------------
#define SCREEN_XY_VALID(x, y)		(x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT)

// ----------------------------------------------------------------------------
COLORREF GetPixel(int x, int y)
{
	byte bits;
	
	if (SCREEN_XY_VALID(x, y)) {
		bits = Screen.Buffer[x][y>>3];
		
		if (bit_tst(bits, y & 0x07))
			return BLACK;
		else
			return WHITE;
	}
	else {
		return WHITE;
	}
}

// ----------------------------------------------------------------------------
void SetPixel(int x, int y, COLORREF color)
{
	if (SCREEN_XY_VALID(x, y)) {
		if (GetPixel(x, y) != color) {
			if (color == BLACK)
				bit_set(Screen.Buffer[x][y>>3], y & 0x07);
			else
				bit_clr(Screen.Buffer[x][y>>3], y & 0x07);
			
			bit_set(Screen.Dirty[x], y>>3);
			Screen.NeedRefresh = true;
		}
	}
}

// ----------------------------------------------------------------------------
// ScrollScreen 会按照8个像素为一个单位的滚屏
void ScrollScreen(int dir, int step)
{
	int x, y;
	
	if (step >= SCREEN_HEIGHT) {
		return;
	}
	
	step /= 8;
	if (step <= 0) {
		return;
	}
	
	switch (dir) {
	case DIRECTION_UP:
		for (y = 0; y < SCREEN_HEIGHT/8 - step; y++) {
			for (x = 0; x < SCREEN_WIDTH; x++) {
				Screen.Buffer[x][y] = Screen.Buffer[x][y+step];
			}
		}
		for (; y < SCREEN_HEIGHT/8; y++) {
			for (x = 0; x < SCREEN_WIDTH; x++) {
				Screen.Buffer[x][y] = 0;
			}
		}
		break;
		
	case DIRECTION_DOWN:
		for (y = 0; y < SCREEN_HEIGHT/8 - step; y++) {
			for (x = 0; x < SCREEN_WIDTH; x++) {
				Screen.Buffer[x][SCREEN_HEIGHT/8 - 1 - y] = Screen.Buffer[x][SCREEN_HEIGHT/8 - 1 - y - step];
			}
		}
		for (; y < SCREEN_HEIGHT/8; y++) {
			for (x = 0; x < SCREEN_WIDTH; x++) {
				Screen.Buffer[x][SCREEN_HEIGHT/8 - 1 - y] = 0;
			}
		}
		break;
	}
	RefreshScreen();
}

// ----------------------------------------------------------------------------
void RefreshScreen()
{
	memset(Screen.Dirty, 0xff, sizeof(Screen.Dirty));
	Screen.NeedRefresh = true;
}

// ----------------------------------------------------------------------------
void ClearScreen()
{
	memset(Screen.Buffer, 0, sizeof(Screen.Buffer));
	RefreshScreen();
}

// ----------------------------------------------------------------------------
void GetScreen(void *buffer)
{
	memcpy(buffer, Screen.Buffer, sizeof(Screen.Buffer));
}

// ----------------------------------------------------------------------------
void PutScreen(const void *buffer)
{
	memcpy(Screen.Buffer, buffer, sizeof(Screen.Buffer));
}

// ----------------------------------------------------------------------------
bool OpenLCD(struct TDevice *dev)
{
	Screen.Width = ((dev->Option & 0xffff0000) >> 16);
	Screen.Height = (dev->Option & 0x0000ffff);

	lcd_init();
		
	Screen.tm_x = 0;
	Screen.tm_y = 0;
	Screen.NeedRefresh = false;
	
	return true;
}

// ----------------------------------------------------------------------------
int WriteLCD(struct TDevice *dev, const byte *buffer, UINT count)
{
	int MAX_ROW, MAX_COL;
	int i, step, newX, newY;
	wchar_t ch;
	TFont *font = GetCurrentFont(FONT_TYPE_ANSI);
	
	if (font != NULL) {
		MAX_ROW = Screen.Height / font->Height;
		MAX_COL = Screen.Width / font->Width;
		
		for (i = 0; i < count; i++) {
			getxy(&newX, &newY);
			
			switch (buffer[i]) {
			case '\n':
				newX = 0;
				if (++newY >= MAX_ROW) {
					newY = MAX_ROW - 1;
					ScrollScreen(DIRECTION_UP, font->Height);
				}
				gotoxy(newX, newY);
				break;
				
			case VK_BACK:
				if (wherex() > 0) {
					newX = wherex() - 1;
					gotoxy(newX, newY);
					DisplayChar(wherex() * font->Width, wherey() * font->Height, ' ');
				}
				break;
				
			default:
				if (iswchar(buffer[i])) {
					ch = MAKEWORD16(buffer[i], buffer[i+1]);
					i++;
					step = 2;

					// 如果剩余的不够一个汉字，那么先换行
					if (wherex() >= MAX_COL - 1) {
						if (wherey() >= MAX_ROW - 1) {
							ScrollScreen(DIRECTION_UP, font->Height);
							gotoxy(0, wherey());
						}
						else {
							gotoxy(0, wherey() + 1);
						}
					}
				}
				else {
					ch = buffer[i];
					step = 1;
				}
				
				DisplayChar(wherex() * font->Width, wherey() * font->Height, ch);				
				newX = wherex() + step;
				newY = wherey();
				
				if (newX >= MAX_COL) {
					newX = 0;
					if (++newY >= MAX_ROW) {
						newY = MAX_ROW - 1;
						ScrollScreen(DIRECTION_UP, font->Height);
					}
				}
				gotoxy(newX, newY);
				break;
			}
		}
	}
	
	return count;
}

// ----------------------------------------------------------------------------
void refresh_lcd()
{
	static int i, j;				// 位置
	static bool continuous;			// 两次刷屏是否连续
	
	if (!Screen.NeedRefresh) {
		return;
	}
	
	for (i = 0; i < SCREEN_HEIGHT/8; i++) {
		continuous = false;

		// select page
		LCD_WCOMD(i + 0xb0);
		
		for (j = 0; j < SCREEN_WIDTH; j++) {
			if (bit_tst(Screen.Dirty[j], i)) {
				if (!continuous) {
					// select position
#ifdef __LCD_TIAN_MA__
					LCD_WCOMD((((j+4) >> 4) & 0x0f) | 0x10);
					LCD_WCOMD((j+4) & 0x0f);
#else
					LCD_WCOMD((((j) >> 4) & 0x0f) | 0x10);
					LCD_WCOMD((j) & 0x0f);
#endif
					// 如果下次继续刷屏，那么就不发送地址，液晶控制器会
					continuous = true;
				}
				LCD_WDATA(Screen.Buffer[j][i]);
			}
			else {
				continuous = false;
			}
		}
	}
	
	Screen.NeedRefresh = false;
}

// ----------------------------------------------------------------------------
// End of lcd.c
