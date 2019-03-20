
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

// 队列
typedef struct TQueue {
	UINT MagicNumber;		// 魔数
	void * Buffer;			// 队列的数据区
	UINT ItemSize;			// 每一个记录的尺寸（以字节为单位）
	UINT Count;				// 队列的大小（最多存放记录的数目）
	UINT Head;				// 头指针
	UINT Tail;				// 尾指针
} TQueue;

void InitializeQueue(TQueue *queue, void * buffer, UINT itemSize, UINT count);
bool IsQueueFull(TQueue *queue);
bool IsQueueEmpty(TQueue *queue);
bool IsQueueIndexValid(TQueue *queue, int index);
bool PushQueue(TQueue *queue, const void *object);
bool PopQueue(TQueue *queue, void *object);

#endif
