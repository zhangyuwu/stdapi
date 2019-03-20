
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/

#ifndef __Queue_H__
#define __Queue_H__

#include "types.h"

// ����
typedef struct TQueue {
	UINT MagicNumber;		// ħ��
	void * Buffer;			// ���е�������
	UINT ItemSize;			// ÿһ����¼�ĳߴ磨���ֽ�Ϊ��λ��
	UINT Count;				// ���еĴ�С������ż�¼����Ŀ��
	UINT Head;				// ͷָ��
	UINT Tail;				// βָ��
} TQueue;

void InitializeQueue(TQueue *queue, void * buffer, UINT itemSize, UINT count);
bool IsQueueFull(TQueue *queue);
bool IsQueueEmpty(TQueue *queue);
bool IsQueueIndexValid(TQueue *queue, int index);
bool PushQueue(TQueue *queue, const void *object);
bool PopQueue(TQueue *queue, void *object);

#endif
