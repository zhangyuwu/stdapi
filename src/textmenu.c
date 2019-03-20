
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/

#include <string.h>
#include <textmenu.h>
#include <magic.h>
#include <system.h>
#include <console.h>
#include <graphics.h>
#include <font.h>

#define MAX_ROW				4
#define MAX_COL				16

/* ------------------------------------------------------------------------
 *  ��������
 *      void InitializeMenu(TMenu * pMenu, const char * MenuTitle, const char ** MenuItems, int nItems)
 *  ���ܣ�
 *      ��ʼ���˵�
 *  ������
 *      pMenu           ----    �������Ĳ˵��ṹ
 *      MenuTitle       ----    ����
 *      MenuItems       ----    ��Ҫ��ʾ�Ĳ˵������
 *      nItems          ----    �˵���ĸ���
 *  ����ֵ��
 *      ��
 *  ˵����
 *      ��
 * -----------------------------------------------------------------------*/

void InitializeMenu(TMenu * pMenu, const char * MenuTitle, const char ** MenuItems, int nItems)
{
	if (pMenu == NULL) {
		SetLastError(ERROR_NULL_POINTER);
		return;
	}

	pMenu->MagicNumber	=	MAGIC_NUMBER_TEXT_MENU;
	pMenu->Count		=	nItems;
	pMenu->Relative		=	0;
	pMenu->Current		=	0;
	pMenu->StartLine	=	MenuTitle == NULL? 0 : 1;
	pMenu->PageSize		=	MenuTitle == NULL? MAX_ROW : MAX_ROW-1;
	pMenu->Closed		=	false;
	pMenu->OnOk			=	NULL;
	pMenu->OnCancel		=	NULL;
	pMenu->OnOtherKey	=	NULL;
	pMenu->InitItems	=	(char **)MenuItems;
	pMenu->Title		=	(char *)MenuTitle;
	pMenu->ReturnValue	=	0;
}

/* ------------------------------------------------------------------------
 *  ��������
 *      void DrawMenu(TMenu * pMenu)
 *  ���ܣ�
 *      ���˵�
 *  ������
 *      pMenu           ----    �������Ĳ˵��ṹ
 *  ����ֵ��
 *      ��
 *  ˵����
 *      ��
 * -----------------------------------------------------------------------*/

void DrawMenu(TMenu * pMenu, bool bRedrawTitle)
{
	int i, length;
	char item[MAX_COL + 1] = {0};
	TFont *font = GetCurrentFont(FONT_TYPE_ANSI);
	TColor color;

	if (pMenu == NULL || pMenu->MagicNumber != MAGIC_NUMBER_TEXT_MENU) {
		SetLastError(ERROR_BAD_PARAMETER);
		return;
	}
	
	if (font == NULL) {
		SetLastError(ERROR_NO_FONT_SELECTED);
		return;
	}
	
	if (bRedrawTitle && pMenu->Title != NULL) {
		if (pMenu->Title) DisplayLine(0, pMenu->Title, AL_CENTER);
	}

	// --------------------------------------------------------------------------
	// BUG : pMenu->Count �����ǵ�ǰҳҪ��ʾ����Ŀ����������ʾ����Ŀ�����
	//       [old]  i < pMenu->Count
	//       [new]  i < (pMenu->Count - pMenu->Relative)
	//       Fixed by Ken, 2006-02-16 12:15:15
	// --------------------------------------------------------------------------
	for (i = 0; i < pMenu->PageSize && i < (pMenu->Count - pMenu->Relative); i++) {
		// ��ʾÿ���˵���
		memset(item, ' ', MAX_COL);
		length = strlen(pMenu->InitItems[i + pMenu->Relative]);
		if (length > MAX_COL) length = MAX_COL;
		strncpy(item, pMenu->InitItems[i + pMenu->Relative], length);

		if (i + pMenu->Relative == pMenu->Current) {
			// ������ǰ����ɫ
			color = GetCurrentColor(COLOR_FOREGROUND);
			
			SetCurrentColor(COLOR_FOREGROUND, GetCurrentColor(COLOR_BACKGROUND));
			SetCurrentColor(COLOR_BACKGROUND, color);
			DisplayString(0, (pMenu->StartLine + i) * font->Height, item);
			
			// �ָ���ǰ����ɫ
			SetCurrentColor(COLOR_BACKGROUND, GetCurrentColor(COLOR_FOREGROUND));
			SetCurrentColor(COLOR_FOREGROUND, color);
		}
		else {
			DisplayString(0, (pMenu->StartLine + i) * font->Height, item);
		}
	}
	
	SetLastError(NO_ERROR);
}

/* ------------------------------------------------------------------------
 *  ��������
 *      void MoveUpMenu(TMenu * pMenu)
 *  ���ܣ�
 *      ���Ϲ����˵�
 *  ������
 *      pMenu           ----    �������Ĳ˵��ṹ
 *  ����ֵ��
 *      ��
 *  ˵����
 *      ��
 * -----------------------------------------------------------------------*/
void MoveUpMenu(TMenu * pMenu)
{
	if (pMenu->Current > 0) {
		if (pMenu->Relative == pMenu->Current) {
			pMenu->Relative--;
		}
		pMenu->Current--;
		DrawMenu(pMenu, false);
	}
	// �Ѿ��ڵ�һ�ת��ĩβ
	else if (pMenu->Current == 0 && pMenu->Count > 1) {
		pMenu->Relative = pMenu->Count > pMenu->PageSize ? pMenu->Count - pMenu->PageSize : 0;
		pMenu->Current = pMenu->Count - 1;
		DrawMenu(pMenu, false);
	}
}

/* ------------------------------------------------------------------------
 *  ��������
 *      void MoveDownMenu(TMenu * pMenu)
 *  ���ܣ�
 *      ���¹����˵�
 *  ������
 *      pMenu           ----    �������Ĳ˵��ṹ
 *  ����ֵ��
 *      ��
 *  ˵����
 *      ��
 * -----------------------------------------------------------------------*/

void MoveDownMenu(TMenu * pMenu)
{
	if (pMenu->Current + 1 < pMenu->Count) {
		if (pMenu->Relative + pMenu->PageSize - 1 == pMenu->Current) {
			pMenu->Relative++;
		}
		pMenu->Current++;
		DrawMenu(pMenu, false);
	}
	// ����Ѿ������һ����ôֱ��ת����ͷ
	else if (pMenu->Count > 1 && pMenu->Current == pMenu->Count - 1) {
		pMenu->Relative = 0;
		pMenu->Current = 0;
		DrawMenu(pMenu, false);
	}
}

/* ------------------------------------------------------------------------
 *  ��������
 *      UINT ShowMenu(TMenu * pMenu)
 *  ���ܣ�
 *      ��ʾ�����˵���������Ϣѭ��
 *  ������
 *      pMenu           ----    �������Ĳ˵��ṹ
 *  ����ֵ��
 *      ��
 *  ˵����
 *      ��
 * -----------------------------------------------------------------------*/

UINT ShowMenu(TMenu * pMenu)
{
	uchar ch;

	if (pMenu == NULL || pMenu->MagicNumber != MAGIC_NUMBER_TEXT_MENU) {
		SetLastError(ERROR_BAD_PARAMETER);
		return TMENU_ERR;
	}
	
	// û�в˵���
	if (pMenu->Count == 0) {
		SetLastError(ERROR_MENU_NO_ITEMS);
		return TMENU_ERR;
	}

	ClearScreen();
	DrawMenu(pMenu, true);
	
	pMenu->Closed = false;
	while (! pMenu->Closed) {
		ch = GetKey();

		switch (ch) {
		case VK_UP:
			MoveUpMenu(pMenu);
			break;

		case VK_DOWN:
			MoveDownMenu(pMenu);
			break;

		case VK_RETURN:
			pMenu->ReturnValue = TMENU_OK;
			if (pMenu->OnOk) {
				pMenu->OnOk(pMenu);
				ClearScreen();
				DrawMenu(pMenu, true);
			}
			break;

		case VK_ESCAPE:
			pMenu->ReturnValue = TMENU_CANCEL;
			if (pMenu->OnCancel) {
				pMenu->OnCancel(pMenu);
			}
			break;

		default:
			pMenu->EventArg = ch;
			if (pMenu->OnOtherKey) {
				pMenu->OnOtherKey(pMenu);
			}
		}
	}

	ClearScreen();
	SetLastError(NO_ERROR);
	
	// Add Return value, OK or Cancel, 2006-02-16 12:12:00
	return pMenu->ReturnValue;
}

/* ------------------------------------------------------------------------
 *  ��������
 *      void SelectMenuItem(TMenu * pMenu, int index)
 *  ���ܣ�
 *      ���ò˵�ѡ��ڼ���
 *  ������
 *      pMenu           ----    �������Ĳ˵��ṹ
 *      index           ----    �ڼ��� (-1 ��ʾ���һ��)
 *  ����ֵ��
 *      ��
 *  ˵����
 *      ��
 * -----------------------------------------------------------------------*/
void SelectMenuItem(TMenu * pMenu, int index)
{
	if (pMenu == NULL || pMenu->MagicNumber != MAGIC_NUMBER_TEXT_MENU) {
		SetLastError(ERROR_BAD_PARAMETER);
		return;
	}

	if (pMenu->Count > 0 && index < pMenu->Count) {
		if (index < 0) {
			pMenu->Current = pMenu->Count - 1;
		}
		else {
			pMenu->Current = index;
		}
		
		// Add by Ken, relative must be updated, 2006-02-16 11:49:50
		pMenu->Relative = (pMenu->Current / pMenu->PageSize) * pMenu->PageSize;
		
		SetLastError(NO_ERROR);
	}
	else {
		SetLastError(ERROR_OUT_OF_RANGE);
	}
}

/* ------------------------------------------------------------------------
 *  ��������
 *      void CloseMenu(TMenu * pMenu)
 *  ���ܣ�
 *      �رղ˵�
 *  ������
 *      pMenu           ----    �������Ĳ˵��ṹ
 *  ����ֵ��
 *      ��
 *  ˵����
 *      ��
 * -----------------------------------------------------------------------*/

void CloseMenu(TMenu * pMenu)
{
	if (pMenu == NULL || pMenu->MagicNumber != MAGIC_NUMBER_TEXT_MENU) {
		SetLastError(ERROR_BAD_PARAMETER);
		return;
	}

	pMenu->Closed = true;
	SetLastError(NO_ERROR);
}
