
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/
 
#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <system.h>

// ----------------------------------------------------------------------------
typedef struct {
	int			Width;
	int			Height;
	TColor		CurrentForeColor;
	TColor		CurrentBackColor;
	int			DrawMode;
	TLineStyle	LineStyle;
} TGraphicsConfig;

TGraphicsConfig GraphicsConfig =
{
	0,
	0,
	BLACK,
	WHITE,
	DRAW_MODE_NORMAL,
	LINE_STYLE_SOLID,
};

/* ------------------------------------------------------------------------
 *  函数名：
 *      TColor GetCurrentColor()
 *  功能：
 *      取得当前图形处理的颜色
 *  参数：
 *      
 *  返回值：
 *      
 *  说明：
 *      
 * -----------------------------------------------------------------------*/

TColor GetCurrentColor(int type)
{
	switch (type) {
	case COLOR_FOREGROUND:
		return GraphicsConfig.CurrentForeColor;
	case COLOR_BACKGROUND:
		return GraphicsConfig.CurrentBackColor;
	default:
		return BLACK;
	}
}

/* ------------------------------------------------------------------------
 *  函数名：
 *      void SetCurrentColor(int type, TColor color)
 *  功能：
 *      设置当前图形处理的颜色
 *  参数：
 *      color           ----    新的颜色
 *  返回值：
 *      
 *  说明：
 *      
 * -----------------------------------------------------------------------*/

void SetCurrentColor(int type, TColor color)
{
	switch (type) {
	case COLOR_FOREGROUND:
		GraphicsConfig.CurrentForeColor = color;
		break;
	case COLOR_BACKGROUND:
		GraphicsConfig.CurrentBackColor = color;
		break;
	}
}

/* ------------------------------------------------------------------------
 *  函数名：
 *      TDrawMode GetDrawMode()
 *  功能：
 *      取得当前图形显示的模式
 *  参数：
 *      
 *  返回值：
 *      
 *  说明：
 *      
 * -----------------------------------------------------------------------*/

TDrawMode GetDrawMode()
{
	return GraphicsConfig.DrawMode;
}

/* ------------------------------------------------------------------------
 *  函数名：
 *      void SetDrawMode(TDrawMode mode)
 *  功能：
 *      设置当前图形显示的模式
 *  参数：
 *      mode            ----    新的模式
 *  返回值：
 *      
 *  说明：
 *      
 * -----------------------------------------------------------------------*/

void SetDrawMode(TDrawMode mode)
{
	GraphicsConfig.DrawMode = mode;
}

/* ------------------------------------------------------------------------
 *  函数名：
 *      TLineStyle GetLineStyle()
 *  功能：
 *      获取画线的风格
 *  参数：
 *      
 *  返回值：
 *      
 *  说明：
 *      
 * -----------------------------------------------------------------------*/
TLineStyle GetLineStyle()
{
	return GraphicsConfig.LineStyle;
}

/* ------------------------------------------------------------------------
 *  函数名：
 *      void SetLineStyle(TLineStyle style)
 *  功能：
 *      设置画线的风格
 *  参数：
 *      style           ----    新的画线风格
 *  返回值：
 *      
 *  说明：
 *      
 * -----------------------------------------------------------------------*/
void SetLineStyle(TLineStyle style)
{
	if (style < LINE_STYLE_MAX) {
		GraphicsConfig.LineStyle = style;
	}
}

/* ------------------------------------------------------------------------
 *  函数名:
 *      int GetRectWidth(TRect *rect)
 *  功能:
 *      计算矩形区域的宽
 *  参数:
 *      
 *  返回值:
 *      
 *  说明:
 *      
 * -----------------------------------------------------------------------*/
int GetRectWidth(TRect *rect)
{
	return abs(rect->right - rect->left) + 1;
}

/* ------------------------------------------------------------------------
 *  函数名:
 *      int GetRectHeight(TRect *rect)
 *  功能:
 *      计算矩形区域的高
 *  参数:
 *      
 *  返回值:
 *      
 *  说明:
 *      
 * -----------------------------------------------------------------------*/
int GetRectHeight(TRect *rect)
{
	return abs(rect->bottom - rect->top) + 1;
}
