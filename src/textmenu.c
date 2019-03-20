
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
 *  函数名：
 *      void InitializeMenu(TMenu * pMenu, const char * MenuTitle, const char ** MenuItems, int nItems)
 *  功能：
 *      初始化菜单
 *  参数：
 *      pMenu           ----    被操作的菜单结构
 *      MenuTitle       ----    标题
 *      MenuItems       ----    需要显示的菜单项标题
 *      nItems          ----    菜单项的个数
 *  返回值：
 *      无
 *  说明：
 *      无
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
 *  函数名：
 *      void DrawMenu(TMenu * pMenu)
 *  功能：
 *      画菜单
 *  参数：
 *      pMenu           ----    被操作的菜单结构
 *  返回值：
 *      无
 *  说明：
 *      无
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
	// BUG : pMenu->Count 并非是当前页要显示的数目，所以有显示出错的可能性
	//       [old]  i < pMenu->Count
	//       [new]  i < (pMenu->Count - pMenu->Relative)
	//       Fixed by Ken, 2006-02-16 12:15:15
	// --------------------------------------------------------------------------
	for (i = 0; i < pMenu->PageSize && i < (pMenu->Count - pMenu->Relative); i++) {
		// 显示每个菜单项
		memset(item, ' ', MAX_COL);
		length = strlen(pMenu->InitItems[i + pMenu->Relative]);
		if (length > MAX_COL) length = MAX_COL;
		strncpy(item, pMenu->InitItems[i + pMenu->Relative], length);

		if (i + pMenu->Relative == pMenu->Current) {
			// 保存以前的颜色
			color = GetCurrentColor(COLOR_FOREGROUND);
			
			SetCurrentColor(COLOR_FOREGROUND, GetCurrentColor(COLOR_BACKGROUND));
			SetCurrentColor(COLOR_BACKGROUND, color);
			DisplayString(0, (pMenu->StartLine + i) * font->Height, item);
			
			// 恢复以前的颜色
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
 *  函数名：
 *      void MoveUpMenu(TMenu * pMenu)
 *  功能：
 *      向上滚动菜单
 *  参数：
 *      pMenu           ----    被操作的菜单结构
 *  返回值：
 *      无
 *  说明：
 *      无
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
	// 已经在第一项，转到末尾
	else if (pMenu->Current == 0 && pMenu->Count > 1) {
		pMenu->Relative = pMenu->Count > pMenu->PageSize ? pMenu->Count - pMenu->PageSize : 0;
		pMenu->Current = pMenu->Count - 1;
		DrawMenu(pMenu, false);
	}
}

/* ------------------------------------------------------------------------
 *  函数名：
 *      void MoveDownMenu(TMenu * pMenu)
 *  功能：
 *      向下滚动菜单
 *  参数：
 *      pMenu           ----    被操作的菜单结构
 *  返回值：
 *      无
 *  说明：
 *      无
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
	// 如果已经是最后一项那么直接转到开头
	else if (pMenu->Count > 1 && pMenu->Current == pMenu->Count - 1) {
		pMenu->Relative = 0;
		pMenu->Current = 0;
		DrawMenu(pMenu, false);
	}
}

/* ------------------------------------------------------------------------
 *  函数名：
 *      UINT ShowMenu(TMenu * pMenu)
 *  功能：
 *      显示整个菜单，包括消息循环
 *  参数：
 *      pMenu           ----    被操作的菜单结构
 *  返回值：
 *      无
 *  说明：
 *      无
 * -----------------------------------------------------------------------*/

UINT ShowMenu(TMenu * pMenu)
{
	uchar ch;

	if (pMenu == NULL || pMenu->MagicNumber != MAGIC_NUMBER_TEXT_MENU) {
		SetLastError(ERROR_BAD_PARAMETER);
		return TMENU_ERR;
	}
	
	// 没有菜单项
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
 *  函数名：
 *      void SelectMenuItem(TMenu * pMenu, int index)
 *  功能：
 *      设置菜单选择第几项
 *  参数：
 *      pMenu           ----    被操作的菜单结构
 *      index           ----    第几项 (-1 表示最后一项)
 *  返回值：
 *      无
 *  说明：
 *      无
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
 *  函数名：
 *      void CloseMenu(TMenu * pMenu)
 *  功能：
 *      关闭菜单
 *  参数：
 *      pMenu           ----    被操作的菜单结构
 *  返回值：
 *      无
 *  说明：
 *      无
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
