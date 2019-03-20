
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/

/* ------------------------------------------------------------------------
 * 本库依赖于 2 个函数：
 *     1. void ARM_CPU_DisableInt();
 *     2. void ARM_CPU_EnableInt();
 * -----------------------------------------------------------------------*/

#ifndef __Memory_H__
#define __Memory_H__

#include <types.h>

#define MAX_MEMORY_PARTITION		4

THandle InitializeMemory(void *base, UINT size);
void *AllocMemory(THandle hMem, UINT size);
void *ReallocMemory(THandle hMem, void *p, UINT newSize);
void FreeMemory(THandle hMem, void *p);
bool QueryMemoryInformation(THandle hMem, UINT *total, UINT *used);

#endif
