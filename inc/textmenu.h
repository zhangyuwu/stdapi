
#ifndef __TextMenu_H__
#define __TextMenu_H__

#include <types.h>

#define TMENU_ERR			((UINT)(-1))
#define TMENU_OK			1
#define TMENU_CANCEL		2

// �ı��˵�
typedef struct TMenu {
	UINT	MagicNumber;	// ħ��
	UINT	Count;			// �˵��������
	UINT	Relative;		// �����
	UINT	Current;		// ��ǰ��
	UINT	StartLine;		// ��ʾ����ʼ��
	UINT	PageSize;		// ÿһҳ������
	bool	Closed;			// ��ǰ�˵��Ƿ�ر�
	char **	InitItems;		// �˵���
	char	EventArg;		// �¼���������OnOtherKeyʹ��
	Event	OnOk;			// ȷ�ϴ�����Ҫ�û��Լ�ʵ��
	Event	OnCancel;		// ȡ�������û��Լ�ʵ�֣����߲������ʾ��Ч
	Event	OnOtherKey;		// �����������û��Լ�ʵ�֣����߲������ʾ��Ч
	char *	Title;			// �˵���������
	UINT	ReturnValue;	// ShowMenu �ķ���ֵ
} TMenu;

void InitializeMenu(TMenu * pMenu, const char * MenuTitle, const char ** MenuItems, int nItems);
void DrawMenu(TMenu * pMenu, bool bRedrawTitle);
UINT ShowMenu(TMenu * pMenu);
void CloseMenu(TMenu * pMenu);
void SelectMenuItem(TMenu * pMenu, int index);

#endif //  __TextMenu_H__
