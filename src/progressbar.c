
#include <graphics.h>

/* ------------------------------------------------------------------------
 *  ��������
 *      void ShowProgress(TRect *area, int percent, TProgressBarStyle style)
 *  ���ܣ�
 *      ����Ļ����ʾһ��������
 *  ������
 *      area            ----    ������������
 *      percent         ----    �ٷֱ� (0~100)
 *      style           ----    �������ķ��: ��ֱ��ˮƽ
 *  ����ֵ��
 *      ��
 *  ˵����
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

