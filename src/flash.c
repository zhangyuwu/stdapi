
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/

#include <system.h>
#include <flash.h>
#include <ucos_ii.h>

#define LITTLE_ENDIAN

#define _FLASH_STARTADDRESS			0x01000000	

#define _F_WRDIRECT(baddr, data)	outp16(baddr, data)
#define _F_WR(waddr, data)			outp16(_FLASH_STARTADDRESS + (waddr << 1), data)
#define _F_RD(waddr)				inp16(_FLASH_STARTADDRESS + (waddr << 1))
#define _F_StartCmd(cmd)			{ _F_WR(0x555, 0xAA); _F_WR(0x2AA, 0x55); _F_WR(0x555, cmd); }

#define _FLASH_BANK1_SIZE			(2 * 1024 * 1024)

typedef struct {
	UINT	BlockSize;
	UINT	Count;
} TFlashInfo;

// AMD 4M flash
#define FLASH_AMD_29LV324DB

#ifdef FLASH_AMD_29LV160DB
TFlashInfo flash_info[] = {
	{ 1024 * 16,	1  },
	{ 1024 * 8,		2  },
	{ 1024 * 32,	1  },
	{ 1024 * 64,	31 },
};
#endif

#ifdef FLASH_AMD_29LV324DB
TFlashInfo flash_info[] = {
	{ 1024 * 8,		8  },
	{ 1024 * 64,	63 },
};
#endif

// addr 为绝对地址
bool flash_toggle(ulong addr)
{
	ushort value1, value2;	
	
	value1 = inp16(addr);
    do {
    	value2 = inp16(addr);
		if ((value1 & 0x0040) == (value2 & 0x0040)) {
			return true;
		}
		value1 = value2;
	} while ((value2 & 0x0020) != 0x0020);
	
	value1 = inp16(addr);
	value2 = inp16(addr);
	
	if ((value1 & 0x40) == (value2 & 0x40)) {
		return true;
	}
	else {
		outp16(addr, 0x00F0);		// send_reset_cmd
	}
	
	return true;
}

bool flash_sector_erase(UINT sector)
{
	ulong addr = 0;
	int i;
	
	// 计算该扇区的起始地址
	for (i = 0; i < sizeof(flash_info) / sizeof(TFlashInfo); i++) {
		if (sector >= flash_info[i].Count) {
			sector -= flash_info[i].Count;
			addr += flash_info[i].BlockSize * flash_info[i].Count;
		}
		else {
			addr += flash_info[i].BlockSize * sector;
			break;
		}
	}

	if (i == sizeof(flash_info) / sizeof(TFlashInfo)) {
		return false;
	}

	addr += _FLASH_STARTADDRESS;

	_F_StartCmd(0x80);			//开始扇区擦除命令
	_F_WR(0x555, 0xaa);
	_F_WR(0x2aa, 0x55);
	_F_WRDIRECT(addr, 0x30);	
	
	while (flash_toggle(addr) == false);
	return true;
}

// addr 必须为偶数
void flash_write_word(ulong addr, ushort value)
{
	_F_WRDIRECT(addr, 0xa0);
	_F_WRDIRECT(addr, value);	

	while (flash_toggle(addr) == false);
}

// addr 为绝对地址
int flash_write(ulong addr, byte *buffer, UINT count)
{
	ulong address;
	UINT nWrite = 0;
	ushort value;
	int i;
	
	address = addr;
	_F_StartCmd(0x20);

	// 如果地址为奇数
	if (addr % 2) {
		if (inp8(addr) != 0xFF) {
			return 0;
		}
		
		// 小端模式
#ifdef LITTLE_ENDIAN
		value = MAKEWORD16(buffer[0], inp8(addr-1));
#else
		value = MAKEWORD16(inp8(addr-1), buffer[0]);
#endif
		flash_write_word(addr-1, value);
		
		// 调整指针
		addr++;
		buffer++;
		count--;
		nWrite++;
	}
	
	for (i = 0; i < count/2; i++) {
		if (inp16(addr) != 0xFFFF) {
			break;
		}
		
		// 小端模式
#ifdef LITTLE_ENDIAN
		value = MAKEWORD16(buffer[i*2+1], buffer[i*2]);
#else
		value = MAKEWORD16(buffer[i*2], buffer[i*2+1]);
#endif
		flash_write_word(addr, value);
		addr += sizeof(ushort);
		nWrite += 2;
	}
	
	// 还剩最后一个字节
	if (count % 2) {
#ifdef LITTLE_ENDIAN
		value = MAKEWORD16(0xFF, buffer[count-1]);
#else
		value = MAKEWORD16(buffer[count-1], 0xFF);
#endif
		flash_write_word(addr, value);
		nWrite++;
	}
	
	// 计算 bank
	if (address >= _FLASH_STARTADDRESS + _FLASH_BANK1_SIZE) {
		// bank 2
		address = _FLASH_STARTADDRESS + _FLASH_BANK1_SIZE;
	}
	else {
		// bank 1
		address = _FLASH_STARTADDRESS;
	}
	
	_F_WRDIRECT(address, 0x90);
	_F_WRDIRECT(address, 0xF0);
	
	return nWrite;	
}


