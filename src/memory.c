
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/

/*
 * --- 原理 ---
 *
 *     指定一块静态内存区域后，FirstChunk就指向这个内存的开始，并更新 FirstChunk
 * 中的内容，以后的每次分配内存都会先寻找一个足够大的空闲块 blockP，在这块空闲
 * 区域的末端分配指定的大小（还包括额外的 Chunk Header）newP，把 newP 插入到链表中
 * 更新内存块单向链表: newP->Next = blockP->Next, blockP->Next = newP, 按照这种方式
 * 所有内存块就形成了从低地址到高地址连续而成的单向链表。这样当有多个内存块都空闲的
 * 块，调用 Merge 就能把它们合并起来，以便于能够分配更大的内存块，这就是常说的碎片
 * 整理。
 *
 *     CHUNK_SIZE_IN_BITS 表示内存块按照最小多大单位安排(7表示128)
 *
 *     Ken (cpp.china@gmail.com), 2006-01-22 18:30:05
 */
 
/*
 * --- Update history ---
 *
 * 2006-01-22 2:01:57, Ken (cpp.china@gmail.com)
 *
 *     问题的发现：使用动态内存为 uCos-II 创建堆栈，同时也使用动态内存不断的分配调试
 * 信息作为缓冲发送到调试任务输出到串口，当运行一小会就会发现系统彻底挂死
 *
 *     1. 修正了 AllocMemory 中存在的致命问题，在分配不成功的情况下还会修改 blockP
 * 和 TMemory 的内容
 *     2. 去除了分配和合并空闲块尝试次数，仅仅在内存分配失败的情况下才调用空闲块合并
 *     3. 改进了合并空闲块的算法，针对多个相邻的空闲块，做到一次性合并
 *     4. 增加了 RellocMemory 函数，根据期望分配的内存与所在块大小决定是否需要重新分配
 *
 */
 
#include <ucos_ii.h>
#include <system.h>
#include <memory.h>
#include <string.h>
#include <magic.h>

/* size of chunk head size */
#define HEADER_SIZE					sizeof(TChunkHeader)

/* minimum size of memory to be allocated */
#define CHUNK_SIZE_IN_BITS			5
#define CHUNK_SIZE					(1 << CHUNK_SIZE_IN_BITS)

#define DEBUG_DYNAMIC_MEM			1

// ----------------------------------------------------------------------------
/* memory chunk descriptor */
typedef struct TChunkHeader {
    UINT		MagicNumber;				// magic number
    UINT		Chunks;						// size of chunk in chunks
    UINT		AllocatedSize;				// allocated size in bytes
    struct 		TChunkHeader *NextChunk;	// next chunk in the list
} TChunkHeader;

// 内存信息结构
typedef struct {
	UINT		MagicNumber;
    UINT		TotalMemory;
    UINT		AllocatedMemory;
    UINT		MaxChunks;
    UINT		TotalBlocks;
	TChunkHeader *FirstChunk;				// first memory chunk pointers
	OS_EVENT	*Sem;						// mutex for memory operation
} TMemory;

// ----------------------------------------------------------------------------
// 内存区
TMemory g_Memorys[MAX_MEMORY_PARTITION];

bool QueryMemoryInformation(THandle hMem, UINT *total, UINT *used)
{
	TMemory * mem = (TMemory *)hMem;
	
	// 检查指定内存是否合法
	if (mem->MagicNumber != MAGIC_NUMBER_MEMORY) {
		SetLastError(ERROR_INVALID_HANDLE);
		return false;
	}
	
	*total = mem->TotalMemory;
	*used = mem->AllocatedMemory;
	
	SetLastError(NO_ERROR);
	return true;
}

// ----------------------------------------------------------------------------
TChunkHeader *FindFirstFit(TMemory * mem, unsigned int chunks)
{
    TChunkHeader *blockP = mem->FirstChunk;

    /* very, very simple algorithm, find the first chunk big enough */
    while (blockP) {
        if ((blockP->Chunks > chunks) && (blockP->AllocatedSize == 0)) {
            return blockP;
        }
        blockP = blockP->NextChunk;
    }
    return NULL;
}

// ----------------------------------------------------------------------------
/* merge this chunk and the next, if they are both free */
void EasyMergeChunks(TMemory *mem, TChunkHeader * blockP)
{
    TChunkHeader *nextP = blockP->NextChunk;

	/* 连接所有的与 blockP 相邻的空闲内存块 */
	if (blockP->MagicNumber == MAGIC_NUMBER_CHUNK && blockP->AllocatedSize == 0) {
		while (nextP && (nextP->MagicNumber == MAGIC_NUMBER_CHUNK) && (nextP->AllocatedSize == 0)) {
			/* swallow (merge) the next chunk. */
			blockP->NextChunk = nextP->NextChunk;
			blockP->Chunks += nextP->Chunks;
			nextP->MagicNumber = ~MAGIC_NUMBER_CHUNK;
			
			/* account for the death of a block of chunks */
			mem->TotalBlocks--;

			/* update nextP for next loop */
			nextP = blockP->NextChunk;
		}
	}
}

// ----------------------------------------------------------------------------
void MergeChunks(TMemory * mem)
{
    TChunkHeader *blockP = mem->FirstChunk;

    while (blockP) {
        EasyMergeChunks(mem, blockP);
        blockP = blockP->NextChunk;
    }
}

// ----------------------------------------------------------------------------
/* initialize the heap allocation scheme */
THandle InitializeMemory(void *base, UINT size)
{
	int i;
	static bool mem_inited = false;

	if (!mem_inited) {
		memset(g_Memorys, 0, sizeof(g_Memorys));
		mem_inited = true;
	}
	
	// find an unused memroy
	for (i = 0; i < MAX_MEMORY_PARTITION; i++) {
		if (g_Memorys[i].MagicNumber != MAGIC_NUMBER_MEMORY) {
			g_Memorys[i].MagicNumber = MAGIC_NUMBER_MEMORY;

			/* set up the free block (one big one to start with) */
			g_Memorys[i].FirstChunk = (TChunkHeader *) base;
			g_Memorys[i].FirstChunk->MagicNumber = MAGIC_NUMBER_CHUNK;
			g_Memorys[i].FirstChunk->AllocatedSize = 0;
			g_Memorys[i].FirstChunk->Chunks = (size >> CHUNK_SIZE_IN_BITS);
			g_Memorys[i].FirstChunk->NextChunk = NULL;

			/* set up the statistics */
			g_Memorys[i].TotalMemory = size;
			g_Memorys[i].AllocatedMemory = 0;
			g_Memorys[i].MaxChunks = (size >> CHUNK_SIZE_IN_BITS);
			g_Memorys[i].TotalBlocks = 1;

			/* create memory semaphore */
			g_Memorys[i].Sem = OSSemCreate(1);

			if (g_Memorys[i].Sem == NULL) {
				halt("InitializeMemory create OS semaphore failed!");
			}
			
			SetLastError(NO_ERROR);
			return (THandle)(&g_Memorys[i]);
		}
	}

	SetLastError(ERROR_MEM_NO_PARTITION);
	return NULL;
}

/* ------------------------------------------------------------------------
 *  函数名：
 *      void *AllocMemory(THandle hMem, unsigned int size)
 *  功能：
 *      分配内存
 *  参数：
 *      hMem            ----    由InitializeMemory创建的内存块
 *      size            ----    要分配的尺寸（字节）
 *  返回值：
 *      NULL            ----    失败
 *      other           ----    成功
 *  说明：
 *      
 * -----------------------------------------------------------------------*/

/* allocate memory, make this as quick as possible */
void *AllocMemory(THandle hMem, UINT size)
{
    unsigned int chunks;
    TChunkHeader *blockP;
	TChunkHeader *newP;
	byte err;
	TMemory * mem = (TMemory *)hMem;
	
	// 检查指定内存是否合法
	if (mem->MagicNumber != MAGIC_NUMBER_MEMORY) {
		SetLastError(ERROR_INVALID_HANDLE);
		return NULL;
	}

	if (size == 0) {
		SetLastError(ERROR_BAD_PARAMETER);
		return NULL;
	}
	
    /* figure out the number of chunks we need */
    chunks = (~(CHUNK_SIZE - 1) & (CHUNK_SIZE + size + sizeof(TChunkHeader))) >> CHUNK_SIZE_IN_BITS;

	/* wait for semaphore */
	if (OSRunning) {
		OSSemPend(mem->Sem, 0, &err);
	}
	else {
		ARMDisableInt();
	}

	/* find the first block that fits */
	blockP = FindFirstFit(mem, chunks);

	/* if we got a chunk, do we need to break it into smaller chunks? */
	if (blockP) {

		/* make a new chunk and thread it into the list, make sure that
		 * the allocator gets the chunk at the end,this tends to keep the free
		 * blocks of chunks at the start of the memory list. */
		newP = (TChunkHeader *)((char *)blockP + ((blockP->Chunks - chunks) * CHUNK_SIZE));

		/* mark this memory allocated */
		newP->MagicNumber = MAGIC_NUMBER_CHUNK;
		newP->AllocatedSize = size;
		newP->Chunks = chunks;
		newP->NextChunk = blockP->NextChunk;
		
		blockP->NextChunk = newP;
		blockP->Chunks -= chunks;

		/* account for the birth of a chunk */
		mem->TotalBlocks++;

		/* Use the new block of chunks */
		blockP = newP;

		/* account for this memory */
		mem->AllocatedMemory += (chunks * CHUNK_SIZE);
			
		/* signal semaphore */
		if (OSRunning) {
			OSSemPost(mem->Sem, &err);
		}
		else {
			ARMEnableInt();
		}

		SetLastError(NO_ERROR);
#if (DEBUG_DYNAMIC_MEM > 0)
		RawDebugPrintf("%s(at=%p, size=%d)\n", __func__, newP, size);
#endif
		/* return the caller a pointer to its memory */
		return (void *)((unsigned char *)newP + sizeof(TChunkHeader));
	}

	/* try and merge any free chunks that are next to each other 
	 * and then retry the allocate */
	MergeChunks(mem);

	/* signal semaphore */
	if (OSRunning) {
		OSSemPost(mem->Sem, &err);
	}
	else {
		ARMEnableInt();
	}

	SetLastError(ERROR_NO_ENOUGH_MEMORY);
#if (DEBUG_DYNAMIC_MEM > 0)
	RawDebugPrintf("%s(%p)\n", __func__, NULL);
#endif

    return NULL;
}

/* ------------------------------------------------------------------------
 *  函数名:
 *      void *ReallocMemory(THandle hMem, void *p, UINT newSize)
 *  功能:
 *      调整已经分配的内存大小
 *  参数:
 *      hMem            ----    由InitializeMemory创建的内存块
 *      newSize         ----    要分配的尺寸（字节）
 *  返回值:
 *      NULL            ----    失败
 *      other           ----    成功
 *  说明:
 *      系统会根据期望大小内存块的大小决定是否需要重新分配
 * -----------------------------------------------------------------------*/

void *ReallocMemory(THandle hMem, void *p, UINT newSize)
{
    TChunkHeader *blockP = (TChunkHeader *)((char *)p - sizeof(TChunkHeader));
	TMemory * mem = (TMemory *)hMem;

	// 检查指定内存是否合法
	if (mem->MagicNumber != MAGIC_NUMBER_MEMORY) {
		SetLastError(ERROR_INVALID_HANDLE);
		return NULL;
	}

	// 判断要释放的内存是否在hMem的范围内
	if ((char *)p >= (char *)mem->FirstChunk && (char *)p < ((char *)mem->FirstChunk + mem->TotalMemory)) {	
		/* check the magic header number */
		if (blockP->MagicNumber != MAGIC_NUMBER_CHUNK) {
			/* error parameter of p */
			SetLastError(ERROR_MEM_POINTER);
			return NULL;
		}
		
		if (newSize <= (blockP->Chunks * CHUNK_SIZE - sizeof(TChunkHeader))) {
			/* no need to allocate */
			SetLastError(NO_ERROR);
			return p;
		}
		else {
			FreeMemory(hMem, p);
			return AllocMemory(hMem, newSize);
		}
	}
	else {
		SetLastError(ERROR_MEM_POINTER);
		return NULL;
	}
}

/* ------------------------------------------------------------------------
 *  函数名：
 *      void FreeMemory(THandle hMem, void *p)
 *  功能：
 *      释放内存
 *  参数：
 *      hMem            ----    由InitializeMemory创建的内存块
 *      where           ----    要释放的内存（由AllocMemory所分配的）
 *  返回值：
 *      无
 *  说明：
 *      
 * -----------------------------------------------------------------------*/

void FreeMemory(THandle hMem, void *p)
{
    TChunkHeader *blockP = (TChunkHeader *)((char *)p - sizeof(TChunkHeader));
	TMemory * mem = (TMemory *)hMem;
	byte err;

	// 检查指定内存是否合法
	if (mem->MagicNumber != MAGIC_NUMBER_MEMORY) {
		SetLastError(ERROR_INVALID_HANDLE);
		return;
	}

	// 判断要释放的内存是否在hMem的范围内
	if ((char *)p >= (char *)mem->FirstChunk && (char *)p < ((char *)mem->FirstChunk + mem->TotalMemory)) {	
		/* check the magic header number */
		if (blockP->MagicNumber != MAGIC_NUMBER_CHUNK) {
			SetLastError(ERROR_MEM_POINTER);
			return;
		}

		/* check that this block is indeed allocated */
		if (blockP->AllocatedSize == 0) {
			SetLastError(ERROR_MEM_POINTER);
			return;
		}

		/* wait for semaphore */
		if (OSRunning) {
			OSSemPend(mem->Sem, 0, &err);
		}
		else {
			ARMDisableInt();
		}

		/* mark this block of chunks as free, don't attempt to merge (yet) */
		blockP->AllocatedSize = 0;

		/* account for this memory */
		mem->AllocatedMemory -= (blockP->Chunks * CHUNK_SIZE);
		mem->TotalBlocks--;

		/* merge free parts linked with blockP, merge free parts as soon as possible */
		/* Add by Ken, 2006-02-15 11:17:55 */
		EasyMergeChunks(mem, blockP);
		
		/* signal semaphore */
		if (OSRunning) {
			OSSemPost(mem->Sem, &err);
		}
		else {
			ARMEnableInt();
		}
		
#if (DEBUG_DYNAMIC_MEM > 0)
		RawDebugPrintf("%s(%p)\n", __func__, blockP);
#endif
		SetLastError(NO_ERROR);
	}
	else {
		SetLastError(ERROR_MEM_POINTER);
	}
}

