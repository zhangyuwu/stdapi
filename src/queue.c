
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/

#include <string.h>
#include <queue.h>
#include <magic.h>

/* ------------------------------------------------------------------------
 *  函数名：
 *      void InitializeQueue(TQueue *queue, void * buffer, UINT itemSize, UINT count)
 *  功能：
 *      初始化队列结构
 *  参数：
 *      queue           ----    需要被初始化的队列
 *      buffer          ----    队列的数据区
 *      itemSize        ----    每条记录的尺寸（字节）
 *      count           ----    最大允许的记录数
 *  返回值：
 *      无
 *  说明：
 *      有关队列的操作需要在调用本函数初始化以后进行
 * -----------------------------------------------------------------------*/

void InitializeQueue(TQueue *queue, void * buffer, UINT itemSize, UINT count)
{
	if (queue != NULL) {
		queue->MagicNumber = MAGIC_NUMBER_QUEUE;
		queue->Buffer = buffer;
		queue->ItemSize = itemSize;
		queue->Count = count;
		queue->Head = 0;
		queue->Tail = 0;
	}
}

/* ------------------------------------------------------------------------
 *  函数名：
 *      bool IsQueueFull(TQueue *queue)
 *  功能：
 *      判断队列是否已经满
 *  参数：
 *      queue           ----    待判断的队列
 *  返回值：
 *      true            ----    满
 *      false           ----    未满
 *  说明：
 *      无
 * -----------------------------------------------------------------------*/

bool IsQueueFull(TQueue *queue)
{
	// --------------------------
	// # # # # # # #
	// ^head       ^tail
	// --------------------------
	// # # # # # # #
	// tail^ ^head

	if (queue->MagicNumber != MAGIC_NUMBER_QUEUE) {
		return false;
	}
	
	return
		(
		(queue->Head == queue->Tail + 1) || 
		((queue->Head == 0) && (queue->Tail == queue->Count - 1))
		);
}

/* ------------------------------------------------------------------------
 *  函数名：
 *      bool IsQueueEmpty(TQueue *queue)
 *  功能：
 *      判断队列是否空
 *  参数：
 *      queue           ----    待判断的队列
 *  返回值：
 *      true            ----    空
 *      false           ----    非空
 *  说明：
 *      无
 * -----------------------------------------------------------------------*/

bool IsQueueEmpty(TQueue *queue)
{
	if (queue->MagicNumber != MAGIC_NUMBER_QUEUE) {
		return false;
	}

	return (queue->Head == queue->Tail);
}

/* ------------------------------------------------------------------------
 *  函数名：
 *      bool PushQueue(TQueue *queue, const void *object)
 *  功能：
 *      队列入栈
 *  参数：
 *      queue           ----    被操作的队列
 *      object          ----    需要入栈的对象
 *  返回值：
 *      true/false      ----    操作是否成功
 *  说明：
 *      如果队列已满则返回失败，否则成功
 * -----------------------------------------------------------------------*/

bool PushQueue(TQueue *queue, const void *object)
{
	if (queue->MagicNumber != MAGIC_NUMBER_QUEUE) {
		return false;
	}

	if (IsQueueFull(queue))
		return false;

	memcpy((byte *)queue->Buffer + queue->Tail * queue->ItemSize, object, queue->ItemSize);
	if (++queue->Tail == queue->Count)
		queue->Tail = 0;

	return true;
}

/* ------------------------------------------------------------------------
 *  函数名：
 *      bool PopQueue(TQueue *queue, void *object)
 *  功能：
 *      队列出栈
 *  参数：
 *      queue           ----    被操作的队列
 *      object          ----    接收出栈数据的对象,可以为 NULL
 *  返回值：
 *      true/false      ----    操作是否成功
 *  说明：
 *      如果队列为空，则直接返回失败，不对object操作
 * -----------------------------------------------------------------------*/

bool PopQueue(TQueue *queue, void *object)
{
	if (queue->MagicNumber != MAGIC_NUMBER_QUEUE) {
		return false;
	}

	if (IsQueueEmpty(queue))
		return false;

	if (object != NULL) {
		memcpy(object, (byte *)queue->Buffer + queue->Head * queue->ItemSize, queue->ItemSize);
	}
	
	if (++queue->Head == queue->Count)
		queue->Head = 0;

	return true;
}
