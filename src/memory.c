
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/

/*
 * --- ԭ�� ---
 *
 *     ָ��һ�龲̬�ڴ������FirstChunk��ָ������ڴ�Ŀ�ʼ�������� FirstChunk
 * �е����ݣ��Ժ��ÿ�η����ڴ涼����Ѱ��һ���㹻��Ŀ��п� blockP����������
 * �����ĩ�˷���ָ���Ĵ�С������������� Chunk Header��newP���� newP ���뵽������
 * �����ڴ�鵥������: newP->Next = blockP->Next, blockP->Next = newP, �������ַ�ʽ
 * �����ڴ����γ��˴ӵ͵�ַ���ߵ�ַ�������ɵĵ��������������ж���ڴ�鶼���е�
 * �飬���� Merge ���ܰ����Ǻϲ��������Ա����ܹ����������ڴ�飬����ǳ�˵����Ƭ
 * ����
 *
 *     CHUNK_SIZE_IN_BITS ��ʾ�ڴ�鰴����С���λ����(7��ʾ128)
 *
 *     Ken (cpp.china@gmail.com), 2006-01-22 18:30:05
 */
 
/*
 * --- Update history ---
 *
 * 2006-01-22 2:01:57, Ken (cpp.china@gmail.com)
 *
 *     ����ķ��֣�ʹ�ö�̬�ڴ�Ϊ uCos-II ������ջ��ͬʱҲʹ�ö�̬�ڴ治�ϵķ������
 * ��Ϣ��Ϊ���巢�͵�����������������ڣ�������һС��ͻᷢ��ϵͳ���׹���
 *
 *     1. ������ AllocMemory �д��ڵ��������⣬�ڷ��䲻�ɹ�������»����޸� blockP
 * �� TMemory ������
 *     2. ȥ���˷���ͺϲ����п鳢�Դ������������ڴ����ʧ�ܵ�����²ŵ��ÿ��п�ϲ�
 *     3. �Ľ��˺ϲ����п���㷨����Զ�����ڵĿ��п飬����һ���Ժϲ�
 *     4. ������ RellocMemory ��������������������ڴ������ڿ��С�����Ƿ���Ҫ���·���
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

// �ڴ���Ϣ�ṹ
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
// �ڴ���
TMemory g_Memorys[MAX_MEMORY_PARTITION];

bool QueryMemoryInformation(THandle hMem, UINT *total, UINT *used)
{
	TMemory * mem = (TMemory *)hMem;
	
	// ���ָ���ڴ��Ƿ�Ϸ�
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

	/* �������е��� blockP ���ڵĿ����ڴ�� */
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
 *  ��������
 *      void *AllocMemory(THandle hMem, unsigned int size)
 *  ���ܣ�
 *      �����ڴ�
 *  ������
 *      hMem            ----    ��InitializeMemory�������ڴ��
 *      size            ----    Ҫ����ĳߴ磨�ֽڣ�
 *  ����ֵ��
 *      NULL            ----    ʧ��
 *      other           ----    �ɹ�
 *  ˵����
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
	
	// ���ָ���ڴ��Ƿ�Ϸ�
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
 *  ������:
 *      void *ReallocMemory(THandle hMem, void *p, UINT newSize)
 *  ����:
 *      �����Ѿ�������ڴ��С
 *  ����:
 *      hMem            ----    ��InitializeMemory�������ڴ��
 *      newSize         ----    Ҫ����ĳߴ磨�ֽڣ�
 *  ����ֵ:
 *      NULL            ----    ʧ��
 *      other           ----    �ɹ�
 *  ˵��:
 *      ϵͳ�����������С�ڴ��Ĵ�С�����Ƿ���Ҫ���·���
 * -----------------------------------------------------------------------*/

void *ReallocMemory(THandle hMem, void *p, UINT newSize)
{
    TChunkHeader *blockP = (TChunkHeader *)((char *)p - sizeof(TChunkHeader));
	TMemory * mem = (TMemory *)hMem;

	// ���ָ���ڴ��Ƿ�Ϸ�
	if (mem->MagicNumber != MAGIC_NUMBER_MEMORY) {
		SetLastError(ERROR_INVALID_HANDLE);
		return NULL;
	}

	// �ж�Ҫ�ͷŵ��ڴ��Ƿ���hMem�ķ�Χ��
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
 *  ��������
 *      void FreeMemory(THandle hMem, void *p)
 *  ���ܣ�
 *      �ͷ��ڴ�
 *  ������
 *      hMem            ----    ��InitializeMemory�������ڴ��
 *      where           ----    Ҫ�ͷŵ��ڴ棨��AllocMemory������ģ�
 *  ����ֵ��
 *      ��
 *  ˵����
 *      
 * -----------------------------------------------------------------------*/

void FreeMemory(THandle hMem, void *p)
{
    TChunkHeader *blockP = (TChunkHeader *)((char *)p - sizeof(TChunkHeader));
	TMemory * mem = (TMemory *)hMem;
	byte err;

	// ���ָ���ڴ��Ƿ�Ϸ�
	if (mem->MagicNumber != MAGIC_NUMBER_MEMORY) {
		SetLastError(ERROR_INVALID_HANDLE);
		return;
	}

	// �ж�Ҫ�ͷŵ��ڴ��Ƿ���hMem�ķ�Χ��
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

