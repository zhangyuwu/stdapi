
#include <graphics.h>

/* ------------------------------------------------------------------------
 *  函数名：
 *      void ShowProgress(TRect *area, int percent, TProgressBarStyle style)
 *  功能：
 *      在屏幕上显示一个进度条
 *  参数：
 *      area            ----    进度条的区域
 *      percent         ----    百分比 (0~100)
 *      style           ----    进度条的风格: 垂直和水平
 *  返回值：
 *      无
 *  说明：
 *      
 * -----------------------------------------------------------------------*/

void ShowProgress(TRect *area, int percent, TProgressBarStyle style)
{
	int width, height, x, y;
	
	if (percent < 0 || percent > 100) {
		return;
	}
	
	Rectangle(area->left, area->top, area->right, area->bottom);
	switch (style) {
	case PROGRESS_BAR_STYLE_VERTICAL:
		height = GetRectHeight(area);
		if (height > 0) {
			y = area->bottom - (int)((double)percent / 100 * height);
			FillRectangle(area->left, y, area->right, area->bottom);
		}
		break;
	case PROGRESS_BAR_STYLE_HORIZONTAL:
		width = GetRectWidth(area);
		if (width > 0) {
			x = area->left + (int)((double)percent / 100 * width);
			FillRectangle(area->left, area->top, x, area->bottom);
		}
		break;
	}
}

