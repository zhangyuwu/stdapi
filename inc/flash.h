
#ifndef __Flash_H__
#define __Flash_H__

#include <types.h>

bool flash_sector_erase(UINT sector);
int flash_write(ulong addr, byte *buffer, UINT count);

#endif
