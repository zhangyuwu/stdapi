
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/

#include <magic.h>
#include <list.h>
#include <system.h>

/* ------------------------------------------------------------------------
 *  函数名:
 *      void InitializeList(TList *list)
 *  功能:
 *      初始化双向链表
 *  参数:
 *      list            ----    被操作的双向链表指针
 *  返回值:
 *      节点 (可能为 NULL)
 *  说明:
 *      
 * -----------------------------------------------------------------------*/
void InitializeList(TList *list)
{
	if (list != NULL) {
		list->MagicNumber = MAGIC_NUMBER_LIST;
		list->Head = NULL;
		list->Tail = NULL;
		
		SetLastError(NO_ERROR);
	}
	else {
		SetLastError(ERROR_NULL_POINTER);
	}
}

/* ------------------------------------------------------------------------
 *  函数名:
 *      bool IsListEmpty(TList *list)
 *  功能:
 *      判断双向链表是否为空
 *  参数:
 *      list            ----    被操作的双向链表指针
 *  返回值:
 *      是否为空
 *  说明:
 *      
 * -----------------------------------------------------------------------*/
bool IsListEmpty(TList *list)
{
	if (list->MagicNumber == MAGIC_NUMBER_LIST) {
		return (list->Head == NULL);
	}
	
	return true;
}

/* ------------------------------------------------------------------------
 *  函数名:
 *      TListNode* GetListHead(TList *list)
 *  功能:
 *      获取双向链表的头指针
 *  参数:
 *      list            ----    被操作的双向链表指针
 *  返回值:
 *      节点 (可能为 NULL)
 *  说明:
 *      
 * -----------------------------------------------------------------------*/
TListNode* GetListHead(TList *list)
{
	if (list->MagicNumber == MAGIC_NUMBER_LIST) {
		return list->Head;
	}
	else {
		return NULL;
	}
}

/* ------------------------------------------------------------------------
 *  函数名:
 *      TListNode* GetListTail(TList *list)
 *  功能:
 *      获取双向链表的尾指针
 *  参数:
 *      list            ----    被操作的双向链表指针
 *  返回值:
 *      节点 (可能为 NULL)
 *  说明:
 *      
 * -----------------------------------------------------------------------*/
TListNode* GetListTail(TList *list)
{
	if (list->MagicNumber == MAGIC_NUMBER_LIST) {
		return list->Tail;
	}
	else {
		return NULL;
	}
}

/* ------------------------------------------------------------------------
 *  函数名:
 *      bool InsertToList (TList *list, TListNode *base, TListNode *node, int insertMode)
 *  功能:
 *      插入一个节点到双向链表
 *  参数:
 *      list            ----    被操作的双向链表指针
 *      node            ----    需要插入的节点指针
 *      insertMode      ----    插入方式
 *                              INSERT_MODE_BEFORE		开头
 *                              INSERT_MODE_AFTER		结尾
 *  返回值:
 *      成功或失败
 *  说明:
 *      
 * -----------------------------------------------------------------------*/
bool InsertToList (TList *list, TListNode *base, TListNode *node, int insertMode)
{
	if (list->MagicNumber == MAGIC_NUMBER_LIST &&
		node->MagicNumber == MAGIC_NUMBER_LIST_NODE &&
		base->MagicNumber == MAGIC_NUMBER_LIST_NODE)
	{
		if (insertMode == INSERT_MODE_BEFORE) {
			node->Next = base;
			node->Prev = base->Prev;
			base->Prev = node;
		}
		else {
			node->Prev = base;
			node->Next = base->Next;
			base->Next = node;
		}
		
		SetLastError(NO_ERROR);
		return true;
	}
	
	SetLastError(ERROR_BAD_PARAMETER);
	return false;
}

/* ------------------------------------------------------------------------
 *  函数名:
 *      bool IsNodeInList(TList *list, TListNode *node)
 *  功能:
 *      查找指定节点是否在双向链表中
 *  参数:
 *      list            ----    被操作的双向链表指针
 *      node            ----    要查找的节点指针
 *  返回值:
 *      成功或失败
 *  说明:
 *      
 * -----------------------------------------------------------------------*/
bool IsNodeInList(TList *list, TListNode *node)
{
	TListNode *p;
	
	if (list->MagicNumber == MAGIC_NUMBER_LIST && node->MagicNumber == MAGIC_NUMBER_LIST_NODE) {
		p = GetListHead(list);
		
		while (p) {
			if (p == node) {
				return true;
			}
			p = p->Next;
		}
	}
	
	return false;
}

/* ------------------------------------------------------------------------
 *  函数名:
 *      bool RemoveFromList(TList *list, TListNode *node)
 *  功能:
 *      从双向链表中删除一个节点
 *  参数:
 *      list            ----    被操作的双向链表指针
 *      node            ----    需要删除的节点指针
 *  返回值:
 *      成功或失败
 *  说明:
 *      
 * -----------------------------------------------------------------------*/
bool RemoveFromList(TList *list, TListNode *node)
{
	if (list->MagicNumber == MAGIC_NUMBER_LIST && node->MagicNumber == MAGIC_NUMBER_LIST_NODE) {
		// 如果这个节点不是属于指定双向链表的,返回错误
		if (!IsNodeInList(list, node)) {
			SetLastError(ERROR_NODE_NOT_FOUND);
			return false;
		}
		
		// 如果是第一个元素,就调整头指针
		if (node->Prev == NULL) {
			list->Head = node->Next;
		}
		else {
			node->Prev->Next = node->Next;
		}
		
		// 如果是最后一个元素,就调整尾指针
		if (node->Next == NULL) {
			list->Tail = node->Prev;
		}
		else {
			node->Next->Prev = node->Prev;
		}
		
		// 标志节点不可用
		node->MagicNumber = ~MAGIC_NUMBER_LIST_NODE;
		
		SetLastError(NO_ERROR);
		return true;
	}
	
	SetLastError(ERROR_BAD_PARAMETER);
	return false;
}

/* ------------------------------------------------------------------------
 *  函数名:
 *      bool AppendList(TList *list, TListNode *node, int appendMode)
 *  功能:
 *      往双向链表中添加节点到开头或者结尾
 *  参数:
 *      list            ----    被操作的双向链表指针
 *      node            ----    需要添加的节点指针
 *      appendMode      ----    添加方式
 *                              APPEND_MODE_HEAD		开头
 *                              APPEND_MODE_TAIL		结尾
 *  返回值:
 *      成功或失败
 *  说明:
 *      
 * -----------------------------------------------------------------------*/
bool AppendList(TList *list, TListNode *node, int appendMode)
{
	if (list->MagicNumber == MAGIC_NUMBER_LIST && node->MagicNumber == MAGIC_NUMBER_LIST_NODE) {
		if (appendMode == APPEND_MODE_HEAD) {
			node->Prev = NULL;
			node->Next = list->Head;
			
			// 如果头指针为空,意味着列表为空
			if (list->Head == NULL) {
				list->Head = node;
			}
			else {
				list->Head->Prev = node;
			}
			
			// 如果尾指针也为空,更新它
			if (list->Tail == NULL) {
				list->Tail = node;
			}
			
			// 更新列表头指针
			list->Head = node;
		}
		else {
			node->Prev = list->Tail;
			node->Next = NULL;

			// 如果头指针为空,意味着列表为空
			if (list->Head == NULL) {
				list->Head = node;
			}

			// 如果尾指针也为空,更新它
			if (list->Tail == NULL) {
				list->Tail = node;
			}
			else {
				list->Tail->Next = node;
			}

			// 更新列表尾指针
			list->Tail = node;
		}

		SetLastError(NO_ERROR);
		return true;
	}

	SetLastError(ERROR_BAD_PARAMETER);
	return false;
}
