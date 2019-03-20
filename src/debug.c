
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/

#include <system.h>
#include <ucos_ii.h>
#include <queue.h>
#include <debug.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <console.h>
#include <device.h>
#include <serial.h>
#include <lwip/tcp.h>

#define DEBUG_TASK_STACK_SIZE		128
#define DEBUG_QUEUE_SIZE			1024

// ----------------------------------------------------------------------------
typedef struct {
#if (RAW_UCOS > 0)
	OS_STK	TaskStack[DEBUG_TASK_STACK_SIZE];
#endif
	TQueue	Queue;
	char	*QueueBuffer[DEBUG_QUEUE_SIZE];
	int		Indent;
} TDebugOutputInfo;

// ----------------------------------------------------------------------------
TDebugOutputInfo g_DebugOutputInfo;

/* ------------------------------------------------------------------------
 *  函数名:
 *      void DebugOutputIndent()
 *  功能:
 *      增加输出调试信息缩进
 *  参数:
 *      
 *  返回值
 *      
 *  说明
 *      
 * -----------------------------------------------------------------------*/

void DebugOutputIndent()
{
	g_DebugOutputInfo.Indent++;
}

/* ------------------------------------------------------------------------
 *  函数名:
 *      void DebugOutputUnIndent()
 *  功能:
 *      减少输出调试信息缩进
 *  参数:
 *      
 *  返回值
 *      
 *  说明
 *      
 * -----------------------------------------------------------------------*/

void DebugOutputUnIndent()
{
	g_DebugOutputInfo.Indent--;
}

/* ------------------------------------------------------------------------
 *  函数名:
 *      int GetDebugOutputIndent()
 *  功能:
 *      取得当前的调试信息缩进值
 *  参数:
 *      
 *  返回值
 *      
 *  说明
 *      
 * -----------------------------------------------------------------------*/

int GetDebugOutputIndent()
{
	return g_DebugOutputInfo.Indent;
}

/* ------------------------------------------------------------------------
 *  函数名:
 *      int debugf(const char *fmt, ...)
 *  功能:
 *      格式化输出调试信息到调试信息队列,由调试任务处理
 *  参数:
 *      
 *  返回值
 *      
 *  说明
 *      
 * -----------------------------------------------------------------------*/

int debugf(const char *fmt, ...)
{
	char buffer[512], *p;
	va_list args;	
	int i;

	// init buffer
	memset(buffer, 0, sizeof(buffer));

	// append task id
	sprintf(buffer, "<%02d> ", OSPrioCur);

	// output indent
	for (i = 0; i < GetDebugOutputIndent(); i++) {
		strcat(buffer, "\t");
	}
	
	// append debug message
	i = strlen(buffer);
	va_start(args, fmt);
	vsnprintf(buffer + i, sizeof(buffer) - i - 1, fmt, args);
	va_end(args);
	
	// alloc dynamic memory
	p = NULL;
	while (!p) {
		p = (char *)malloc(strlen(buffer) + 1);
		if (!p) OSTimeDly(1);
	}
	
	strcpy(p, buffer);
	while (IsQueueFull(&g_DebugOutputInfo.Queue)) OSTimeDly(1);
	ARMDisableInt();
	PushQueue(&g_DebugOutputInfo.Queue, &p);
	ARMEnableInt();
	
	return 0;
}

/* ------------------------------------------------------------------------
 *  函数名:
 *      void DebugOutputTask(void *arg)
 *  功能:
 *      * TASK * 专门用于输出调试信息的任务
 *  参数:
 *      
 *  返回值
 *      
 *  说明
 *      
 * -----------------------------------------------------------------------*/

void DebugOutputTask(void *arg)
{
	char *p;
	
	while (true) {
		if (!IsQueueEmpty(&g_DebugOutputInfo.Queue)) {
			PopQueue(&g_DebugOutputInfo.Queue, &p);
			if (p) {
				WriteDevice(GetStdHandle(STD_DEBUG_HANDLE), p, strlen(p));
				free(p);
			}
		}
		else {
			OSTimeDly(10);
		}
	}
}

/* ------------------------------------------------------------------------
 *  函数名:
 *      void StartDebugTask()
 *  功能:
 *      启动调试信息输出任务
 *  参数:
 *      
 *  返回值
 *      
 *  说明
 *      
 * -----------------------------------------------------------------------*/

void StartDebugTask()
{
	g_DebugOutputInfo.Indent = 0;
	InitializeQueue(&g_DebugOutputInfo.Queue, g_DebugOutputInfo.QueueBuffer, sizeof(char *), DEBUG_QUEUE_SIZE);

#if (RAW_UCOS > 0)
    OSTaskCreate(DebugOutputTask, (void *)0, &(g_DebugOutputInfo.TaskStack[DEBUG_TASK_STACK_SIZE - 1]), DEBUG_THREAD_PRIORITY);
#else
	if (CreateThread(DebugOutputTask, NULL, DEBUG_TASK_STACK_SIZE, DEBUG_THREAD_PRIORITY) == NULL) {
		halt("Create debug thread failed!\n");
	}
#endif
}

/* ------------------------------------------------------------------------
 *  函数名:
 *      void OSDumpTCB(OS_TCB *p)
 *  功能:
 *      导出 uCos 的任务控制块内容
 *  参数:
 *      
 *  返回值
 *      
 *  说明
 *      
 * -----------------------------------------------------------------------*/

void OSDumpTCB(OS_TCB *p)
{
	RawDebugPrintf("------ TCB (%p) -----\n", p);
	RawDebugPrintf("  OSTCBStkPtr  = %p\n", p->OSTCBStkPtr);
	RawDebugPrintf("  OSTCBStkSize = %d\n", p->OSTCBStkSize);
	RawDebugPrintf("  OSTCBId      = %d\n", p->OSTCBId);
	RawDebugPrintf("  OSTCBNext    = %p\n", p->OSTCBNext);
	RawDebugPrintf("  OSTCBPrev    = %p\n", p->OSTCBPrev);
	RawDebugPrintf("  OSTCBDly     = %d\n", p->OSTCBDly);
	RawDebugPrintf("  OSTCBStat    = %d\n", p->OSTCBStat);
	RawDebugPrintf("  OSTCBPendTO  = %d\n", p->OSTCBPendTO);
	RawDebugPrintf("  OSTCBPrio    = %d\n", p->OSTCBPrio);			
}

/* ------------------------------------------------------------------------
 *  函数名:
 *      void OSTCBListDump()
 *  功能:
 *      导出 uCos 中所有的任务的控制块
 *  参数:
 *      
 *  返回值
 *      
 *  说明
 *      
 * -----------------------------------------------------------------------*/

void OSTCBListDump()
{
	OS_TCB *p;
	int i;
	
	for (i = 0; i < sizeof(OSTCBPrioTbl) / sizeof(OS_TCB *); i++) {
		p = OSTCBPrioTbl[i];
		if (p != (OS_TCB *)0 && p != (OS_TCB *)1) {
			RawDebugPrintf("TCB of prio %d {\n", i);
			OSDumpTCB(p);
			RawDebugPrintf("}\n");
		}
	}
}

void OSEventDump(OS_EVENT *event)
{
	RawDebugPrintf("OS_EVENT(%p) { OSEventType = %d, OSEventGrp = %d, OSEventCnt = %d, OSEventPtr = %d }\n",
		event,
		event->OSEventType,
		event->OSEventGrp,
		event->OSEventCnt,
		event->OSEventPtr
	);
}

const char *tcp_pcb_state_str(struct tcp_pcb *pcb)
{
	const char *str[] = {
		"CLOSED",
		"LISTEN",
		"SYN_SENT",
		"SYN_RCVD",
		"ESTABLISHED",
		"FIN_WAIT_1",
		"FIN_WAIT_2",
		"CLOSE_WAIT",
		"CLOSING",
		"LAST_ACK",
		"TIME_WAIT",
	};
	
	if (pcb->state < sizeof(str)/sizeof(char *))
		return str[pcb->state];
	return "Unknow";
}

const char *tcp_pcb_flag_str(struct tcp_pcb *pcb)
{
	const char *str[] = {
		"/* Delayed ACK. */",
		"/* Immediate ACK. */",
		"/* In fast recovery. */",
		"/* Connection was reset. */",
		"/* Connection was sucessfully closed. */",
		"/* Connection was closed by the remote end. */",
		"/* Disable Nagle algorithm */",
	};
	
	switch (pcb->flags) {
	case TF_ACK_DELAY:
		return str[0];
	case TF_ACK_NOW:
		return str[1];
	case TF_INFR:
		return str[2];
	case TF_RESET:
		return str[3];
	case TF_CLOSED:
		return str[4];
	case TF_GOT_FIN:
		return str[5];
	case TF_NODELAY:
		return str[6];
	default:
		return "Unknow";
	}
}

void tcp_pcb_dump(struct tcp_pcb *pcb)
{
	debugf("------ PCB (%p) -----\n", pcb);
	debugf("  pcb->next           = %p \t/* TCP state */\n", pcb->next);
	debugf("  pcb->state          = 0x%08x \t/* %s */\n", pcb->state, tcp_pcb_state_str(pcb));
	debugf("  pcb->prio           = 0x%08x\n", pcb->prio);
	debugf("  pcb->callback_arg   = %p\n", pcb->callback_arg);
	debugf("  pcb->local_port     = 0x%08x\n", pcb->local_port);
	debugf("  pcb->remote_port    = 0x%08x\n", pcb->remote_port);
	debugf("  pcb->flags          = 0x%08x %s\n", pcb->flags, tcp_pcb_flag_str(pcb));
	debugf("  pcb->rcv_nxt        = 0x%08x\n", pcb->rcv_nxt);
	debugf("  pcb->rcv_wnd        = 0x%08x\n", pcb->rcv_wnd);
	debugf("  pcb->tmr            = 0x%08x\n", pcb->tmr);
	debugf("  pcb->polltmr        = 0x%08x\n", pcb->polltmr);
	debugf("  pcb->pollinterval   = 0x%08x\n", pcb->pollinterval);
	debugf("  pcb->rtime          = 0x%08x \t/* Retransmission timer. */\n", pcb->rtime);
	debugf("  pcb->mss            = 0x%08x \t/* maximum segment size */\n", pcb->mss);
	debugf("  pcb->rttest         = 0x%08x \t/* RTT estimate in 500ms ticks */\n", pcb->rttest);
	debugf("  pcb->rtseq          = 0x%08x \t/* sequence number being timed */\n", pcb->rtseq);
	debugf("  pcb->rto            = 0x%08x \t/* retransmission time-out */\n", pcb->rto);
	debugf("  pcb->nrtx           = 0x%08x \t/* number of retransmissions */\n", pcb->nrtx);
	debugf("  pcb->lastack        = 0x%08x \t/* Highest acknowledged seqno. */\n", pcb->lastack);
	debugf("  pcb->dupacks        = 0x%08x\n", pcb->dupacks);
	debugf("  pcb->cwnd           = 0x%08x\n", pcb->cwnd);
	debugf("  pcb->ssthresh       = 0x%08x\n", pcb->ssthresh);
	debugf("  pcb->snd_nxt        = 0x%08x \t/* next seqno to be sent */\n", pcb->snd_nxt);
	debugf("  pcb->snd_max        = 0x%08x \t/* Highest seqno sent. */\n", pcb->snd_max);
	debugf("  pcb->snd_wnd        = 0x%08x \t/* sender window */\n", pcb->snd_wnd);
	debugf("  pcb->snd_wl1        = 0x%08x \t/* Sequence and acknowledgement numbers of last window update. */\n", pcb->snd_wl1);
	debugf("  pcb->snd_wl2        = 0x%08x\n", pcb->snd_wl2);
	debugf("  pcb->snd_lbb        = 0x%08x \t/* Sequence number of next byte to be buffered. */\n", pcb->snd_lbb);
	debugf("  pcb->acked          = 0x%08x\n", pcb->acked);
	debugf("  pcb->snd_buf        = 0x%08x \t/* Available buffer space for sending (in bytes). */\n", pcb->snd_buf);
	debugf("  pcb->snd_queuelen   = 0x%08x \t/* Available buffer space for sending (in tcp_segs). */\n", pcb->snd_queuelen);
	debugf("  pcb->unsent         = %p \t/* Unsent (queued) segments. */\n", pcb->unsent);
	debugf("  pcb->unacked        = %p \t/* Sent but unacknowledged segments. */\n", pcb->unacked);
#if 0
	debugf("  pcb->ooseq          = %p \t/* Received out of sequence segments. */\n", pcb->ooseq);
#endif
	debugf("  pcb->keepalive      = 0x%08x \t/* idle time before KEEPALIVE is sent */\n", pcb->keepalive);
	debugf("  pcb->keep_cnt       = 0x%08x \t/* KEEPALIVE counter */\n", pcb->keep_cnt);
}
