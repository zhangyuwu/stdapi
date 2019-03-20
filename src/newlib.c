
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/

// Newlib port to uCos-ii

#include <system.h>
#include <ucos_ii.h>
#include <console.h>
#include <stdio.h>
#include <device.h>
#include <memory.h>
#include <reent.h>
#include <errno.h>
#include <sys/stat.h>

#define DEBUG_NEWLIB	0

char *__EH_FRAME_BEGIN__;

// ----------------------------------------------------------------------------
// Processes the specified function at exit.
int atexit(void (*func)(void))
{
	return 0;
}

// ----------------------------------------------------------------------------
// 为了节约内存可以不采用可重入支持
// Ken, 2006-01-20 13:08:18
// ----------------------------------------------------------------------------
#define SMALL_MEM		0

#if (SMALL_MEM > 0)
/* declare one _reent structure for each OS task */
struct _reent reent_table[OS_MAX_TASKS];
#endif
struct _reent reent_global = _REENT_INIT(reent_global);

// ----------------------------------------------------------------------------
// call reent_init while system boot
void reent_init()
{
#if (SMALL_MEM > 0)
	int i;

	for (i = 0; i < OS_MAX_TASKS; i++) {
		_REENT_INIT_PTR(&reent_table[i]);
	}
#endif
	_impure_ptr = &reent_global;
}

// ----------------------------------------------------------------------------
void reent_switch()
{
#if (SMALL_MEM > 0)
	/* aim _impure_ptr at the incoming task's _reent structure */ 
	if (OSPrioCur < OS_MAX_TASKS) {
		_impure_ptr = &reent_table[OSPrioCur];
	}
	else {
		_impure_ptr = &reent_global;
	}
#endif
}

// ----------------------------------------------------------------------------
int _execve_r(struct _reent *ptr, char *filename, char **argv, char **envp)
{
#if (DEBUG_NEWLIB > 0)
	RawDebugPrintf("%s(ptr:%d)\n", __func__, ptr);
#endif

	ptr->_errno = ENOTSUP;
	return -1;
}

// ----------------------------------------------------------------------------
int _fork_r(struct _reent *ptr)
{
#if (DEBUG_NEWLIB > 0)
	RawDebugPrintf("%s(ptr:%d)\n", __func__, ptr);
#endif

	ptr->_errno = ENOTSUP;
	return -1;
}

// ----------------------------------------------------------------------------
int _getpid_r(struct _reent *ptr)
{
	OS_TCB tcb;
#if (DEBUG_NEWLIB > 0)
	RawDebugPrintf("%s(ptr:%d)\n", __func__, ptr);
#endif

	OSTaskQuery(OS_PRIO_SELF, &tcb);
	return tcb.OSTCBPrio;
}

// ----------------------------------------------------------------------------
int _kill_r(struct _reent *ptr, int pid, int sig)
{
#if (DEBUG_NEWLIB > 0)
	RawDebugPrintf("%s(ptr:%d)\n", __func__, ptr);
#endif

	return -1;
}

// ----------------------------------------------------------------------------
int _link_r(struct _reent *ptr, const char *oldname, const char *newname)
{
#if (DEBUG_NEWLIB > 0)
	RawDebugPrintf("%s(ptr:%d)\n", __func__, ptr);
#endif

	ptr->_errno = EMLINK;
	return -1;
}

// ----------------------------------------------------------------------------
int _unlink_r(struct _reent *ptr, const char *name)
{
#if (DEBUG_NEWLIB > 0)
	RawDebugPrintf("%s(ptr:%d)\n", __func__, ptr);
#endif

	ptr->_errno = EMLINK;
	return -1;
}

// ----------------------------------------------------------------------------
_CLOCK_T_ _times_r(struct _reent *ptr, struct tms *buf)
{
#if (DEBUG_NEWLIB > 0)
	RawDebugPrintf("%s(ptr:%d)\n", __func__, ptr);
#endif

	ptr->_errno = EMLINK;
	return -1;
}

// ----------------------------------------------------------------------------
int _wait_r(struct _reent *ptr, int *status)
{
#if (DEBUG_NEWLIB > 0)
	RawDebugPrintf("%s(ptr:%d)\n", __func__, ptr);
#endif

	ptr->_errno = EMLINK;
	return -1;
}

// ----------------------------------------------------------------------------
int _open_r(struct _reent *ptr, const char *pathname, int flags, int mode)
{
	int fd = (int)OpenDevice(pathname, mode);
	
#if (DEBUG_NEWLIB > 0)
	RawDebugPrintf("%s(ptr:%d)\n", __func__, ptr);
#endif
	if (fd == 0) {
		return -1;
	}
	return fd;
}

// ----------------------------------------------------------------------------
int _close_r(struct _reent *ptr, int fd)
{
#if (DEBUG_NEWLIB > 0)
	RawDebugPrintf("%s(ptr:%d)\n", __func__, ptr);
#endif

	// you can not close stdandard device
	switch (fd) {
	case 0: // stdin
	case 1: // stdout
	case 2: // stderr
		return -1;
		
	default:
		return CloseDevice((THandle)fd);
	}
}

// ----------------------------------------------------------------------------
_ssize_t _read_r(struct _reent *ptr, int fd, void *buf, size_t count)
{
	THandle handle;
	
#if (DEBUG_NEWLIB > 0)
	RawDebugPrintf("%s(ptr:%d)\n", __func__, ptr);
#endif

	switch (fd) {
	case 0: // stdin
		handle = GetStdHandle(STD_INPUT_HANDLE);
		break;
	case 1: // stdout
		handle = GetStdHandle(STD_OUTPUT_HANDLE);
		break;
	case 2: // stderr
		handle = GetStdHandle(STD_ERROR_HANDLE);
		break;
	default:
		handle = (THandle)fd;
	}
	
	return ReadDevice(handle, (byte *)buf, count);
}

// ----------------------------------------------------------------------------
_ssize_t _write_r(struct _reent *ptr, int fd, const void *buf, size_t count)
{
	THandle handle;
	
#if (DEBUG_NEWLIB > 0)
	RawDebugPrintf("%s(ptr:%d)\n", __func__, ptr);
#endif

	switch (fd) {
	case 0: // stdin
		handle = GetStdHandle(STD_INPUT_HANDLE);
		break;
	case 1: // stdout
		handle = GetStdHandle(STD_OUTPUT_HANDLE);
		break;
	case 2: // stderr
		handle = GetStdHandle(STD_ERROR_HANDLE);
		break;
	default:
		handle = (THandle)fd;
	}
	
	return WriteDevice(handle, (const byte *)buf, count);
}

// ----------------------------------------------------------------------------
// lseek 不支持 whence = SEEK_END
_off_t _lseek_r(struct _reent *ptr, int fd, _off_t pos, int whence)
{
	THandle handle;
	TDevice *dev;
	ulong offset;
	
#if (DEBUG_NEWLIB > 0)
	RawDebugPrintf("%s(ptr:%d)\n", __func__, ptr);
#endif

	switch (fd) {
	case 0: // stdin
		handle = GetStdHandle(STD_INPUT_HANDLE);
		break;
	case 1: // stdout
		handle = GetStdHandle(STD_OUTPUT_HANDLE);
		break;
	case 2: // stderr
		handle = GetStdHandle(STD_ERROR_HANDLE);
		break;
	default:
		handle = (THandle)fd;
	}
	
	dev = (TDevice *)handle;
	switch (whence) {
	case SEEK_SET:
		offset = pos;
		break;
	case SEEK_CUR:
		offset = dev->Offset + pos;
		break;
	case SEEK_END:
		ptr->_errno = ENOTSUP;
		return -1;
	}
	
	return SeekDevice(handle, offset);
}

// ----------------------------------------------------------------------------
int _stat_r(struct _reent *ptr, const char *file, struct stat *pstat)
{
#if (DEBUG_NEWLIB > 0)
	RawDebugPrintf("%s(ptr:%d)\n", __func__, ptr);
#endif
	pstat->st_mode = S_IFCHR;
	return 0;
}

// ----------------------------------------------------------------------------
int _fstat_r(struct _reent *ptr, int fd, struct stat *pstat)
{
#if (DEBUG_NEWLIB > 0)
	RawDebugPrintf("%s(ptr:%d)\n", __func__, ptr);
#endif
	pstat->st_mode = S_IFCHR;
	return 0;
}

// ----------------------------------------------------------------------------
int _fcntl_r(struct _reent *ptr, int fd, int cmd, int arg)
{
#if (DEBUG_NEWLIB > 0)
	RawDebugPrintf("%s(ptr:%d)\n", __func__, ptr);
#endif
	ptr->_errno = ENOTSUP;
	return -1;
}

// ----------------------------------------------------------------------------
/* This one is not guaranteed to be available on all targets.  */
int _gettimeofday_r(struct _reent *ptr, struct timeval *tp, struct timezone *tzp)
{
#if (DEBUG_NEWLIB > 0)
	RawDebugPrintf("%s(ptr:%d)\n", __func__, ptr);
#endif
	ptr->_errno = ENOTSUP;
	return -1;
}

// ----------------------------------------------------------------------------
#define HEAPSIZE	(1024 * 8)

void *_sbrk_r(struct _reent *ptr, size_t incr)
{
	static unsigned char _heap[HEAPSIZE];
	static unsigned char *heap_end = NULL;
	unsigned char *prev_heap_end;
	
#if (DEBUG_NEWLIB > 0)
	RawDebugPrintf("_sbrk_r(ptr:%p, incr:%d)\n", ptr, incr);
#endif

	/* initialize */
	if (heap_end == NULL) heap_end = _heap;
	prev_heap_end = heap_end;

	if( heap_end + incr - _heap > HEAPSIZE ) {
		/* heap overflow--- announce on stderr */
		halt("Heap overflow!\n");
	}
	
	heap_end += incr;
	return (caddr_t)prev_heap_end;
}

// ----------------------------------------------------------------------------
// 动态内存管理

THandle g_hDynamicMemory;

#define MEM_OS_WRAPPER		1

#if !(MEM_OS_WRAPPER > 0)
#include <lwip/mem.h>
#endif

// ----------------------------------------------------------------------------
// Allocates memory blocks.
void * malloc(int size)
{
#if (MEM_OS_WRAPPER > 0)
	void *p = AllocMemory(g_hDynamicMemory, size);
#else
	void *p = mem_malloc(size);
#endif

#if (DEBUG_NEWLIB > 0)
	RawDebugPrintf("%s(%d): %p\n", __func__, size, p);
#endif
	return p;
}

void * realloc(void *p, int newSize)
{
#if (MEM_OS_WRAPPER > 0)
	void *pNew = ReallocMemory(g_hDynamicMemory, p, newSize);
#else
	void *pNew = mem_realloc(p, newSize);
#endif

#if (DEBUG_NEWLIB > 0)
	RawDebugPrintf("%s(%p, %d): %p\n", __func__, p, newSize, pNew);
#endif
	return pNew;
}

// ----------------------------------------------------------------------------
// Deallocates or frees a memory block.
void free(void *p)
{
#if (DEBUG_NEWLIB > 0)
	RawDebugPrintf("%s(%p)\n", __func__, p);
#endif

#if (MEM_OS_WRAPPER > 0)
	FreeMemory(g_hDynamicMemory, p);
#else
	mem_free(p);
#endif
}

// ----------------------------------------------------------------------------
// initialize dynamic memory
// alloc all free memory not used, from bss end to ram limit.
// ----------------------------------------------------------------------------
void dynamic_mem_init()
{
	extern const void *end;
	const UINT RAM_BASE = 0;
	const UINT RAM_SIZE = 1024 * 256;
	
	void *ram_beg = (void *)(((int)(&end) + 0x10) & 0xFFFFFFF0);
	void *ram_end = (void *)(RAM_BASE + RAM_SIZE);

	int size = (int)ram_end - (int)ram_beg - 0x10;
	
#if 1
	RawDebugPrintf("Dynamic memory: at %p, size=%d, .end=%p\n", ram_beg, size, &end);
#endif
	
	g_hDynamicMemory = InitializeMemory(ram_beg, size);
}
