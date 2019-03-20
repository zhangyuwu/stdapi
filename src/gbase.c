
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
 *  ��������
 *      TColor GetCurrentColor()
 *  ���ܣ�
 *      ȡ�õ�ǰͼ�δ������ɫ
 *  ������
 *      
 *  ����ֵ��
 *      
 *  ˵����
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
 *  ��������
 *      void SetCurrentColor(int type, TColor color)
 *  ���ܣ�
 *      ���õ�ǰͼ�δ������ɫ
 *  ������
 *      color           ----    �µ���ɫ
 *  ����ֵ��
 *      
 *  ˵����
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
 *  ��������
 *      TDrawMode GetDrawMode()
 *  ���ܣ�
 *      ȡ�õ�ǰͼ����ʾ��ģʽ
 *  ������
 *      
 *  ����ֵ��
 *      
 *  ˵����
 *      
 * -----------------------------------------------------------------------*/

TDrawMode GetDrawMode()
{
	return GraphicsConfig.DrawMode;
}

/* ------------------------------------------------------------------------
 *  ��������
 *      void SetDrawMode(TDrawMode mode)
 *  ���ܣ�
 *      ���õ�ǰͼ����ʾ��ģʽ
 *  ������
 *      mode            ----    �µ�ģʽ
 *  ����ֵ��
 *      
 *  ˵����
 *      
 * -----------------------------------------------------------------------*/

void SetDrawMode(TDrawMode mode)
{
	GraphicsConfig.DrawMode = mode;
}

/* ------------------------------------------------------------------------
 *  ��������
 *      TLineStyle GetLineStyle()
 *  ���ܣ�
 *      ��ȡ���ߵķ��
 *  ������
 *      
 *  ����ֵ��
 *      
 *  ˵����
 *      
 * -----------------------------------------------------------------------*/
TLineStyle GetLineStyle()
{
	return GraphicsConfig.LineStyle;
}

/* ------------------------------------------------------------------------
 *  ��������
 *      void SetLineStyle(TLineStyle style)
 *  ���ܣ�
 *      ���û��ߵķ��
 *  ������
 *      style           ----    �µĻ��߷��
 *  ����ֵ��
 *      
 *  ˵����
 *      
 * -----------------------------------------------------------------------*/
void SetLineStyle(TLineStyle style)
{
	if (style < LINE_STYLE_MAX) {
		GraphicsConfig.LineStyle = style;
	}
}

/* ------------------------------------------------------------------------
 *  ������:
 *      int GetRectWidth(TRect *rect)
 *  ����:
 *      �����������Ŀ�
 *  ����:
 *      
 *  ����ֵ:
 *      
 *  ˵��:
 *      
 * -----------------------------------------------------------------------*/
int GetRectWidth(TRect *rect)
{
	return abs(rect->right - rect->left) + 1;
}

/* ------------------------------------------------------------------------
 *  ������:
 *      int GetRectHeight(TRect *rect)
 *  ����:
 *      �����������ĸ�
 *  ����:
 *      
 *  ����ֵ:
 *      
 *  ˵��:
 *      
 * -----------------------------------------------------------------------*/
int GetRectHeight(TRect *rect)
{
	return abs(rect->bottom - rect->top) + 1;
}
