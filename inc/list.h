
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/

#ifndef __LIST_H__
#define __LIST_H__

#include <types.h>

#define INSERT_MODE_BEFORE		0
#define INSERT_MODE_AFTER		1

#define APPEND_MODE_HEAD		0
#define APPEND_MODE_TAIL		1

typedef struct TListNode {
	UINT				MagicNumber;
	struct TListNode*	Prev;
	struct TListNode*	Next;
	void *				Data;
} TListNode;

typedef struct {
	UINT				MagicNumber;
	struct TListNode*	Head;
	struct TListNode*	Tail;
} TList;

void InitializeList(TList *list);
bool IsListEmpty(TList *list);
TListNode* GetListHead(TList *list);
TListNode* GetListTail(TList *list);
bool InsertToList (TList *list, TListNode *base, TListNode *node, int insertMode);
bool RemoveFromList(TList *list, TListNode *node);
bool AppendList(TList *list, TListNode *node, int appendMode);
bool IsNodeInList(TList *list, TListNode *node);

#endif
