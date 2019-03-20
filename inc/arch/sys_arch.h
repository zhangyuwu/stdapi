
/*-----------------------------------------------------------------------------
 *                         Embedsoft Standard Api 1.00                        *
 *                               Copyright (c)                                *
 *                             2005-06-14 by Ken                              *
 *                               Beijing.China                                *
 *                             sunwzd@hotmail.com                             *
 -----------------------------------------------------------------------------*/

#ifndef __SYS_ARCH_H__
#define __SYS_ARCH_H__

#ifndef TRUE
	#define TRUE		1
#endif

#ifndef FALSE
	#define FALSE		0
#endif

#define SYS_SEM_NULL	((void *)0)
#define SYS_MBOX_NULL	((void *)0)

typedef void* sys_sem_t;
typedef void* sys_mbox_t;
typedef unsigned char sys_thread_t;
typedef int sys_prot_t;

sys_mbox_t sys_mbox_new(void);
void sys_mbox_free(sys_mbox_t mbox);
void sys_mbox_post(sys_mbox_t mbox, void *data);
u32_t sys_arch_mbox_fetch(sys_mbox_t mbox, void **data, u32_t timeout);
sys_sem_t sys_sem_new(u8_t count);
void sys_sem_signal(sys_sem_t sem);
u32_t sys_arch_sem_wait(sys_sem_t sem, u32_t timeout);
void sys_sem_free(sys_sem_t sem);
sys_thread_t sys_thread_new(void (* thread)(void *arg), void *arg, int prio);
void sys_thread_free();
sys_prot_t sys_arch_protect();
void sys_arch_unprotect(sys_prot_t pval);

void lwip_init(void);

#endif
