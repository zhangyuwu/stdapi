/*
 * Copyright (c) 2001, Swedish Institute of Computer Science.
 * All rights reserved. 
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions 
 * are met: 
 * 1. Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright 
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the distribution. 
 * 3. Neither the name of the Institute nor the names of its contributors 
 *    may be used to endorse or promote products derived from this software 
 *    without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND 
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE 
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS 
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY 
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF 
 * SUCH DAMAGE. 
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 * $Id: sys_arch.c,v 1.1.1.1 2003/05/17 05:06:56 chenyu Exp $
 */

#include "lwip/debug.h"

#include "lwip/def.h"
#include "lwip/sys.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/pbuf.h"
#include "lwip/netif.h"
#include "arch/sys_arch.h" 
#include "netif/etharp.h"
#include "ucos_ii.h"
#include <system.h>
#include <serial.h>

#define MAX_QUEUE_ENTRIES			32

#define INVALID_THREAD_PRIO			-1
#define THREAD_START_PRIO			11
#define THREAD_STOP_PRIO			30
#define MAX_LWIP_THREAD_COUNT		(THREAD_STOP_PRIO - THREAD_START_PRIO + 1)

#define LWIP_STK_SIZE				(1024)

#undef BEGIN_DEBUG_FUNCTION
#undef END_DEBUG_FUNCTION

#define BEGIN_DEBUG_FUNCTION
#define END_DEBUG_FUNCTION

// MailBox port on uC/OS-ii
typedef struct {
    OS_EVENT*	handle;
    void*		entries[MAX_QUEUE_ENTRIES];
} queue_t;

struct thread_info_t {
	u8_t		prio;
	struct		sys_timeouts timeouts;
#if (RAW_UCOS > 0)
	void *      stack;
#else
	THandle		hThread;
#endif
};

struct thread_info_t lwip_threads[MAX_LWIP_THREAD_COUNT];
struct sys_timeouts null_timeouts;

const void * NULL_DATA;

#define NO_SYSTEM_WRAP	RAW_UCOS

sys_mbox_t sys_mbox_new(void)
{
#if (NO_SYSTEM_WRAP > 0)
	queue_t *q;	
	
	BEGIN_DEBUG_FUNCTION;
	q = mem_malloc(sizeof(queue_t));
	
	if (q != NULL) {
		q->handle = OSQCreate(q->entries, MAX_QUEUE_ENTRIES);
		if (q->handle != NULL) {
			END_DEBUG_FUNCTION;
			return q;
		}
		else {
			halt("sys_mbox_new create OSQ failed!\n");
		}
	}
	
	END_DEBUG_FUNCTION;
	return SYS_MBOX_NULL;
#else	
	return CreateMailBox(MAX_QUEUE_ENTRIES);
#endif
}

void sys_mbox_free(sys_mbox_t mbox)
{
#if (NO_SYSTEM_WRAP > 0)
	u8_t err;	
	queue_t *q = (queue_t *)mbox;

	BEGIN_DEBUG_FUNCTION;
	do {
		OSQDel(q->handle, OS_DEL_NO_PEND, &err);
		if (err != OS_NO_ERR) {
			RawDebugOutputString("sys_mbox_free: OSQDel failed, retry!\n");
			OSTimeDly(1);
		}
	} while (err != OS_NO_ERR);
	
	mem_free(q);
	END_DEBUG_FUNCTION;
#else
	CloseMailBox((THandle)mbox);
#endif
}

void sys_mbox_post(sys_mbox_t mbox, void *data)
{
#if (NO_SYSTEM_WRAP > 0)
	queue_t *q = (queue_t *)mbox;
	BEGIN_DEBUG_FUNCTION;
	if (data == NULL)
		data = &NULL_DATA;
	
	// if OS Queue is full then wait (OS_Q_FULL)
	while (OSQPost(q->handle, data) != OS_NO_ERR) {
		RawDebugOutputString("sys_mbox_post: OSQPost failed, retry!\n");
		OSTimeDly(1);
	}
	
	END_DEBUG_FUNCTION;
#else
	PostMailBox((THandle)mbox, data);
#endif
}

u32_t sys_arch_mbox_fetch(sys_mbox_t mbox, void **data, u32_t timeout)
{
#if (NO_SYSTEM_WRAP > 0)
	u8_t err;
	queue_t *q = (queue_t *)mbox;
	BEGIN_DEBUG_FUNCTION;
	
	// timeout = 0 means wait infinite
	if (timeout != 0) {
		timeout = (timeout * OS_TICKS_PER_SEC) / 1000;
	
		if (timeout < 1)
			timeout = 1;
		if (timeout > 0xffff)
			timeout = 0xffff;
	}
	
	if (data == NULL)
		OSQPend(q->handle, timeout, &err);
	else
		*data = OSQPend(q->handle, timeout, &err);
		
	if (err == OS_TIMEOUT) {
		END_DEBUG_FUNCTION;
		return SYS_ARCH_TIMEOUT;
	}
	else {
		if (*data == &NULL_DATA) {
			*data = NULL;
		}
		END_DEBUG_FUNCTION;
		return 0;
	}
#else
	if (FetchMailBox((THandle)mbox, data, timeout) == WAIT_TIMEOUT) {
		return SYS_ARCH_TIMEOUT;
	}
	else {
		return 0;
	}
#endif
}

sys_sem_t sys_sem_new(u8_t count)
{
#if (NO_SYSTEM_WRAP > 0)
    sys_sem_t sem;
	BEGIN_DEBUG_FUNCTION;

    sem = OSSemCreate((u16_t)count);	
	if (sem == NULL) {
		halt("sys_sem_new create OS semaphore failed!");
	}
	
	END_DEBUG_FUNCTION;
    return sem;
#else	
	return CreateSemaphore(count);
#endif
}

void sys_sem_signal(sys_sem_t sem)
{
#if (NO_SYSTEM_WRAP > 0)
    u8_t err;
	u8_t err0;
	
	BEGIN_DEBUG_FUNCTION;
    err0 = OSSemPost((OS_EVENT *)sem, &err);
	if (err0 != OS_NO_ERR) {
		RawDebugPrintf("*** sys_sem_signal failed, sem = %p, cnt = %d, err = %d, err0=%d!\n", sem, ((OS_EVENT *)sem)->OSEventCnt, err, err0);
		OSEventDump(sem);
	}
	END_DEBUG_FUNCTION;
#else	
	SignalSemaphore(sem);
#endif
}

u32_t sys_arch_sem_wait(sys_sem_t sem, u32_t timeout)
{
#if (NO_SYSTEM_WRAP > 0)
	u8_t err;
	BEGIN_DEBUG_FUNCTION;
	
	// timeout = 0 means wait infinite
	if (timeout != 0) {
		timeout = (timeout * OS_TICKS_PER_SEC) / 1000;
	
		if (timeout < 1)
			timeout = 1;
		if (timeout > 0xffff)
			timeout = 0xffff;
	}
	
	OSSemPend((OS_EVENT *)sem, timeout, &err);

	if (err == OS_TIMEOUT) {
		END_DEBUG_FUNCTION;
		return SYS_ARCH_TIMEOUT;
	}
	else {
		END_DEBUG_FUNCTION;
		return 0;
	}
#else	
	if (WaitSemaphore(sem, timeout) == WAIT_TIMEOUT) {
		return SYS_ARCH_TIMEOUT;
	}
	else {
		return 0;
	}
#endif
}

void sys_sem_free(sys_sem_t sem)
{
#if (NO_SYSTEM_WRAP > 0)
    u8_t err;
	BEGIN_DEBUG_FUNCTION;

	do {
		OSSemDel((OS_EVENT *)sem, OS_DEL_NO_PEND, &err );
		if (err != OS_NO_ERR) {
			OSTimeDly(1);
		}
	} while (err != OS_NO_ERR);
	
	END_DEBUG_FUNCTION;
#else
	CloseSemaphore(sem);
#endif
}

void sys_init_timeouts()
{
	int i;
	
	BEGIN_DEBUG_FUNCTION;
	for (i = 0; i < MAX_LWIP_THREAD_COUNT; i++) {
		lwip_threads[i].prio = INVALID_THREAD_PRIO;
		lwip_threads[i].timeouts.next = NULL;
#if (RAW_UCOS > 0)
		lwip_threads[i].stack = NULL;
#endif
	}
	null_timeouts.next = NULL;
	END_DEBUG_FUNCTION;
}

struct sys_timeouts * sys_arch_timeouts(void)
{
	OS_TCB tcb;
	u8_t err;
	
	null_timeouts.next = NULL;
	BEGIN_DEBUG_FUNCTION;
	err = OSTaskQuery(OS_PRIO_SELF, &tcb);
	if (err == OS_NO_ERR) {
		if (tcb.OSTCBPrio >= THREAD_START_PRIO && tcb.OSTCBPrio < THREAD_STOP_PRIO) {
			END_DEBUG_FUNCTION;
			return &(lwip_threads[tcb.OSTCBPrio - THREAD_START_PRIO].timeouts);
		}
	}
	END_DEBUG_FUNCTION;
	return &null_timeouts;
}

sys_thread_t sys_thread_new(void (* thread)(void *arg), void *arg, int prio)
{
#if (RAW_UCOS > 0)
	OS_STK *stack;
#endif
	
	BEGIN_DEBUG_FUNCTION;
	for (prio = THREAD_START_PRIO; prio < THREAD_STOP_PRIO; prio++) {
		if (OSTCBPrioTbl[prio] == NULL) {
#if (RAW_UCOS > 0)
			stack = mem_malloc(LWIP_STK_SIZE * sizeof(OS_STK));
			if (stack == NULL) {
				return -1;
			}
			if (OSTaskCreate(thread, arg, &stack[LWIP_STK_SIZE-1], prio) != OS_NO_ERR) {
				mem_free(stack);
				return -1;
			}
#else
			lwip_threads[prio-THREAD_START_PRIO].hThread = CreateThread(thread, arg, LWIP_STK_SIZE, prio);
			if (lwip_threads[prio-THREAD_START_PRIO].hThread == NULL) {
				halt("sys_thread_new, CreateThread failed!\n");
				return -1;
			}
#endif
			lwip_threads[prio-THREAD_START_PRIO].prio = prio;
#if (RAW_UCOS > 0)
			lwip_threads[prio-THREAD_START_PRIO].stack = stack;
#endif
			return prio;
		}
	}
	END_DEBUG_FUNCTION;
	return -1;
}

void sys_thread_free()
{
	byte prio = OSPrioCur;
	
	ARMDisableInt();
	if (prio >= THREAD_START_PRIO && prio < THREAD_STOP_PRIO) {
#if (RAW_UCOS > 0)
		if (lwip_threads[prio - THREAD_START_PRIO].stack != NULL) {
			mem_free(lwip_threads[prio - THREAD_START_PRIO].stack);
			lwip_threads[prio - THREAD_START_PRIO].stack = NULL;
			OSTaskDel(OS_PRIO_SELF);
		}
#else
		KillThread(lwip_threads[prio - THREAD_START_PRIO].hThread);
#endif
	}
	ARMEnableInt();
}

sys_prot_t sys_arch_protect()
{
	ARMDisableInt();
	return 0;
}

void sys_arch_unprotect(sys_prot_t pval)
{
	ARMEnableInt();
}

void lwip_init(void)
{
	mem_init();
	memp_init();
	pbuf_init();
	netif_init();
	etharp_init();
	sys_init_timeouts();
}

