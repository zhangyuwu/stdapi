
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
 *  ��������
 *      void InitializeQueue(TQueue *queue, void * buffer, UINT itemSize, UINT count)
 *  ���ܣ�
 *      ��ʼ�����нṹ
 *  ������
 *      queue           ----    ��Ҫ����ʼ���Ķ���
 *      buffer          ----    ���е�������
 *      itemSize        ----    ÿ����¼�ĳߴ磨�ֽڣ�
 *      count           ----    �������ļ�¼��
 *  ����ֵ��
 *      ��
 *  ˵����
 *      �йض��еĲ�����Ҫ�ڵ��ñ�������ʼ���Ժ����
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
 *  ��������
 *      bool IsQueueFull(TQueue *queue)
 *  ���ܣ�
 *      �ж϶����Ƿ��Ѿ���
 *  ������
 *      queue           ----    ���жϵĶ���
 *  ����ֵ��
 *      true            ----    ��
 *      false           ----    δ��
 *  ˵����
 *      ��
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
 *  ��������
 *      bool IsQueueEmpty(TQueue *queue)
 *  ���ܣ�
 *      �ж϶����Ƿ��
 *  ������
 *      queue           ----    ���жϵĶ���
 *  ����ֵ��
 *      true            ----    ��
 *      false           ----    �ǿ�
 *  ˵����
 *      ��
 * -----------------------------------------------------------------------*/

bool IsQueueEmpty(TQueue *queue)
{
	if (queue->MagicNumber != MAGIC_NUMBER_QUEUE) {
		return false;
	}

	return (queue->Head == queue->Tail);
}

/* ------------------------------------------------------------------------
 *  ��������
 *      bool PushQueue(TQueue *queue, const void *object)
 *  ���ܣ�
 *      ������ջ
 *  ������
 *      queue           ----    �������Ķ���
 *      object          ----    ��Ҫ��ջ�Ķ���
 *  ����ֵ��
 *      true/false      ----    �����Ƿ�ɹ�
 *  ˵����
 *      ������������򷵻�ʧ�ܣ�����ɹ�
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
 *  ��������
 *      bool PopQueue(TQueue *queue, void *object)
 *  ���ܣ�
 *      ���г�ջ
 *  ������
 *      queue           ----    �������Ķ���
 *      object          ----    ���ճ�ջ���ݵĶ���,����Ϊ NULL
 *  ����ֵ��
 *      true/false      ----    �����Ƿ�ɹ�
 *  ˵����
 *      �������Ϊ�գ���ֱ�ӷ���ʧ�ܣ�����object����
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
