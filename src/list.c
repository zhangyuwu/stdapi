
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
 *  ������:
 *      void InitializeList(TList *list)
 *  ����:
 *      ��ʼ��˫������
 *  ����:
 *      list            ----    ��������˫������ָ��
 *  ����ֵ:
 *      �ڵ� (����Ϊ NULL)
 *  ˵��:
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
 *  ������:
 *      bool IsListEmpty(TList *list)
 *  ����:
 *      �ж�˫�������Ƿ�Ϊ��
 *  ����:
 *      list            ----    ��������˫������ָ��
 *  ����ֵ:
 *      �Ƿ�Ϊ��
 *  ˵��:
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
 *  ������:
 *      TListNode* GetListHead(TList *list)
 *  ����:
 *      ��ȡ˫�������ͷָ��
 *  ����:
 *      list            ----    ��������˫������ָ��
 *  ����ֵ:
 *      �ڵ� (����Ϊ NULL)
 *  ˵��:
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
 *  ������:
 *      TListNode* GetListTail(TList *list)
 *  ����:
 *      ��ȡ˫�������βָ��
 *  ����:
 *      list            ----    ��������˫������ָ��
 *  ����ֵ:
 *      �ڵ� (����Ϊ NULL)
 *  ˵��:
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
 *  ������:
 *      bool InsertToList (TList *list, TListNode *base, TListNode *node, int insertMode)
 *  ����:
 *      ����һ���ڵ㵽˫������
 *  ����:
 *      list            ----    ��������˫������ָ��
 *      node            ----    ��Ҫ����Ľڵ�ָ��
 *      insertMode      ----    ���뷽ʽ
 *                              INSERT_MODE_BEFORE		��ͷ
 *                              INSERT_MODE_AFTER		��β
 *  ����ֵ:
 *      �ɹ���ʧ��
 *  ˵��:
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
 *  ������:
 *      bool IsNodeInList(TList *list, TListNode *node)
 *  ����:
 *      ����ָ���ڵ��Ƿ���˫��������
 *  ����:
 *      list            ----    ��������˫������ָ��
 *      node            ----    Ҫ���ҵĽڵ�ָ��
 *  ����ֵ:
 *      �ɹ���ʧ��
 *  ˵��:
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
 *  ������:
 *      bool RemoveFromList(TList *list, TListNode *node)
 *  ����:
 *      ��˫��������ɾ��һ���ڵ�
 *  ����:
 *      list            ----    ��������˫������ָ��
 *      node            ----    ��Ҫɾ���Ľڵ�ָ��
 *  ����ֵ:
 *      �ɹ���ʧ��
 *  ˵��:
 *      
 * -----------------------------------------------------------------------*/
bool RemoveFromList(TList *list, TListNode *node)
{
	if (list->MagicNumber == MAGIC_NUMBER_LIST && node->MagicNumber == MAGIC_NUMBER_LIST_NODE) {
		// �������ڵ㲻������ָ��˫�������,���ش���
		if (!IsNodeInList(list, node)) {
			SetLastError(ERROR_NODE_NOT_FOUND);
			return false;
		}
		
		// ����ǵ�һ��Ԫ��,�͵���ͷָ��
		if (node->Prev == NULL) {
			list->Head = node->Next;
		}
		else {
			node->Prev->Next = node->Next;
		}
		
		// ��������һ��Ԫ��,�͵���βָ��
		if (node->Next == NULL) {
			list->Tail = node->Prev;
		}
		else {
			node->Next->Prev = node->Prev;
		}
		
		// ��־�ڵ㲻����
		node->MagicNumber = ~MAGIC_NUMBER_LIST_NODE;
		
		SetLastError(NO_ERROR);
		return true;
	}
	
	SetLastError(ERROR_BAD_PARAMETER);
	return false;
}

/* ------------------------------------------------------------------------
 *  ������:
 *      bool AppendList(TList *list, TListNode *node, int appendMode)
 *  ����:
 *      ��˫����������ӽڵ㵽��ͷ���߽�β
 *  ����:
 *      list            ----    ��������˫������ָ��
 *      node            ----    ��Ҫ��ӵĽڵ�ָ��
 *      appendMode      ----    ��ӷ�ʽ
 *                              APPEND_MODE_HEAD		��ͷ
 *                              APPEND_MODE_TAIL		��β
 *  ����ֵ:
 *      �ɹ���ʧ��
 *  ˵��:
 *      
 * -----------------------------------------------------------------------*/
bool AppendList(TList *list, TListNode *node, int appendMode)
{
	if (list->MagicNumber == MAGIC_NUMBER_LIST && node->MagicNumber == MAGIC_NUMBER_LIST_NODE) {
		if (appendMode == APPEND_MODE_HEAD) {
			node->Prev = NULL;
			node->Next = list->Head;
			
			// ���ͷָ��Ϊ��,��ζ���б�Ϊ��
			if (list->Head == NULL) {
				list->Head = node;
			}
			else {
				list->Head->Prev = node;
			}
			
			// ���βָ��ҲΪ��,������
			if (list->Tail == NULL) {
				list->Tail = node;
			}
			
			// �����б�ͷָ��
			list->Head = node;
		}
		else {
			node->Prev = list->Tail;
			node->Next = NULL;

			// ���ͷָ��Ϊ��,��ζ���б�Ϊ��
			if (list->Head == NULL) {
				list->Head = node;
			}

			// ���βָ��ҲΪ��,������
			if (list->Tail == NULL) {
				list->Tail = node;
			}
			else {
				list->Tail->Next = node;
			}

			// �����б�βָ��
			list->Tail = node;
		}

		SetLastError(NO_ERROR);
		return true;
	}

	SetLastError(ERROR_BAD_PARAMETER);
	return false;
}
