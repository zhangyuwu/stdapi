
#ifndef __TextMenu_H__
#define __TextMenu_H__

#include <types.h>

#define TMENU_ERR			((UINT)(-1))
#define TMENU_OK			1
#define TMENU_CANCEL		2

// 文本菜单
typedef struct TMenu {
	UINT	MagicNumber;	// 魔数
	UINT	Count;			// 菜单项的总数
	UINT	Relative;		// 相对项
	UINT	Current;		// 当前项
	UINT	StartLine;		// 显示的起始行
	UINT	PageSize;		// 每一页的行数
	bool	Closed;			// 当前菜单是否关闭
	char **	InitItems;		// 菜单项
	char	EventArg;		// 事件参数，被OnOtherKey使用
	Event	OnOk;			// 确认处理，需要用户自己实现
	Event	OnCancel;		// 取消处理，用户自己实现，或者不定义表示无效
	Event	OnOtherKey;		// 其他按键，用户自己实现，或者不定义表示无效
	char *	Title;			// 菜单的主标题
	UINT	ReturnValue;	// ShowMenu 的返回值
} TMenu;

void InitializeMenu(TMenu * pMenu, const char * MenuTitle, const char ** MenuItems, int nItems);
void DrawMenu(TMenu * pMenu, bool bRedrawTitle);
UINT ShowMenu(TMenu * pMenu);
void CloseMenu(TMenu * pMenu);
void SelectMenuItem(TMenu * pMenu, int index);

#endif //  __TextMenu_H__
