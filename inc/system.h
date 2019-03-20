
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/


#ifndef __System_H__
#define __System_H__

#include <types.h>
#include <console.h>

// 字节处理
#define HI_8(word) 					((unsigned char)((word) >> 8))
#define LO_8(word) 					((unsigned char)((word) & 0xff))
#define MAKEWORD16(hi, lo) 			(((unsigned short)(lo)) | (((unsigned short)(hi)) << 8))

#define HI_16(word) 				((unsigned short)((word) >> 16))
#define LO_16(word) 				((unsigned short)((word) & 0xffff))
#define MAKEWORD32(hi, lo) 			(((unsigned int)(lo)) | (((unsigned int)(hi)) << 16))

// 语言种类
#define LAN_CHINESE					1				// 中文
#define LAN_ENGLISH					2				// 英文

// 中断触发类型
#define IRQ_TRIGGER_LOW_LEVEL		0				// 低电平
#define IRQ_TRIGGER_NEGATIVE		1				// 下降沿
#define IRQ_TRIGGER_HIGH_LEVEL		2				// 高电平
#define IRQ_TRIGGER_POSITIVE		3				// 上升沿

// 中断优先级
#define IRQ_PRIORITY_LOWEST			0				// 最低优先级
#define IRQ_PRIORITY_LOW			1				//    ||
#define IRQ_PRIORITY_BELOW_NORMAL	2				//    ||
#define IRQ_PRIORITY_NORMAL			3				//    ||
#define IRQ_PRIORITY_ABOVE_NORMAL	4				//    ||
#define IRQ_PRIORITY_HIGH			5				//    ||
#define IRQ_PRIORITY_ABOVE_HIGH		6				//    vv
#define IRQ_PRIORITY_HIGHEST		7				// 最高优先级

// 虚拟键值定义
#define VK_LBUTTON					0x01			// 鼠标左键
#define VK_RBUTTON					0x02			// 鼠标右键
#define VK_MBUTTON					0x04			// 鼠标中键
#define VK_BACK						0x08			// 回退
#define VK_UP						0x09			// 上
#define VK_LEFT						0x0A			// 左
#define VK_DOWN						0x0B			// 下
#define VK_RIGHT					0x0C			// 右
#define VK_RETURN					0x0D			// 确认
#define VK_MENU						0x0E			// 菜单
#define VK_CONFIG					0x0F			// 设置
#define VK_ESCAPE					0x1B			// 取消
#define VK_F1						0x11			// F1
#define VK_F2						0x12			// F1
#define VK_F3						0x13			// F1
#define VK_HELP						0x1F			// 帮助
#define VK_SPACE					0x20			// 空格

// 最大类型值
#define MAX_SHORT	0x7FFF
#define MAX_USHORT	0xFFFF

#define MAX_UINT	0xFFFFFFFF
#define MAX_INT		0x7FFFFFFF

#define MAX_ULONG	0xFFFFFFFF
#define MAX_LONG	0x7FFFFFFF

// 字符串输入方式
#define INPUT_STRING_NORMAL			0				// 常规
#define INPUT_STRING_PASSWORD		1				// 密码，显示输入的为'*'

// 标准输入/输出/错误句柄
#define STD_INPUT_HANDLE			1
#define STD_OUTPUT_HANDLE			2
#define STD_ERROR_HANDLE			3
#define STD_DEBUG_HANDLE			4

// 结构压缩存储
#ifndef PACK_STRUCT_FIELD
#define PACK_STRUCT_FIELD(x)		x __attribute__((packed))
#endif

#ifndef PACK_STRUCT_STRUCT
#define PACK_STRUCT_STRUCT			__attribute__((packed))
#endif

// 端口操作
#define inp8(port)					*((volatile unsigned char *)(port))
#define inp16(port)					*((volatile unsigned short *)(port))
#define inp32(port)					*((volatile unsigned long *)(port))

#define outp8(port, value)			(*((volatile unsigned char *)(port)) = (unsigned char)(value))
#define outp16(port, value)			(*((volatile unsigned short *)(port)) = (unsigned short)(value))
#define outp32(port, value)			(*((volatile unsigned long *)(port)) = (unsigned long)(value))

// 位操作
#define bit_set(value, pos)			(value |= (1 << (pos)))
#define bit_clr(value, pos)			(value &= (~(1 << (pos))))
#define bit_tst(value, pos)			(value & (1 << (pos)))

// 位数组
#define bit_table_decl(name, size)	unsigned char name[(size) >> 3 + ((size) % 8 ? 1 : 0)]
#define bit_table_set(name, index)	bit_set(name[(index) >> 3], (index) & 0x07)
#define bit_table_clr(name, index)	bit_clr(name[(index) >> 3], (index) & 0x07)
#define bit_table_tst(name, index)	bit_tst(name[(index) >> 3], (index) & 0x07)

// 高低字节顺序交换
#define int16_swap(value)			(((value) << 8) | ((value) >> 8))
#define int32_swap(value)			(((value) << 24) | (((value) & 0xff00) << 8) | (((value) & 0xff0000) >> 8) | ((value) >> 24))

// 最大最小值函数
#ifndef max
#define max(a, b)					(a > b ? a : b)
#endif

#ifndef min
#define min(a, b)					(a < b ? a : b)
#endif

// 调试
#define ASSERT(f)					{ if (!f) { RawDebugPrintf("Assert failed: %s in %s at %d line.\n", __FILE__, __func__, __LINE__); while (true); } }

// 错误码
#define ERROR_LEVEL_C				1
#define ERROR_LEVEL_OS				2
#define ERROR_LEVEL_SYS				3

#define NO_ERROR            		0
#define ERROR_SYSTEM_BASE			1000
#define ERROR_INVALID_FUNCTION      (ERROR_SYSTEM_BASE + 1)
#define ERROR_FILE_NOT_FOUND        (ERROR_SYSTEM_BASE + 2)
#define ERROR_ACCESS_DENIED     	(ERROR_SYSTEM_BASE + 3)
#define ERROR_INVALID_HANDLE        (ERROR_SYSTEM_BASE + 4)
#define ERROR_NO_ENOUGH_MEMORY      (ERROR_SYSTEM_BASE + 5)
#define ERROR_INVALID_BLOCK     	(ERROR_SYSTEM_BASE + 6)
#define ERROR_BAD_ENVIRONMENT       (ERROR_SYSTEM_BASE + 7)
#define ERROR_BAD_FORMAT        	(ERROR_SYSTEM_BASE + 8)
#define ERROR_INVALID_ACCESS        (ERROR_SYSTEM_BASE + 9)
#define ERROR_INVALID_DATA      	(ERROR_SYSTEM_BASE + 10)
#define ERROR_WRITE_PROTECT     	(ERROR_SYSTEM_BASE + 11)
#define ERROR_NOT_READY         	(ERROR_SYSTEM_BASE + 12)
#define ERROR_BAD_COMMAND       	(ERROR_SYSTEM_BASE + 13)
#define ERROR_CRC           		(ERROR_SYSTEM_BASE + 14)
#define ERROR_BAD_LENGTH        	(ERROR_SYSTEM_BASE + 15)
#define ERROR_WRITE_FAULT       	(ERROR_SYSTEM_BASE + 16)
#define ERROR_READ_FAULT        	(ERROR_SYSTEM_BASE + 17)
#define ERROR_FILE_EXISTS       	(ERROR_SYSTEM_BASE + 18)
#define ERROR_INVALID_PASSWORD      (ERROR_SYSTEM_BASE + 19)
#define ERROR_OPEN_FAILED       	(ERROR_SYSTEM_BASE + 20)
#define ERROR_FLASH_FULL         	(ERROR_SYSTEM_BASE + 21)
#define ERROR_INVALID_NAME      	(ERROR_SYSTEM_BASE + 22)
#define ERROR_LOCKED            	(ERROR_SYSTEM_BASE + 23)
#define ERROR_DEVICE_BUSY			(ERROR_SYSTEM_BASE + 24)
#define ERROR_DEVICE_NOT_OPEN		(ERROR_SYSTEM_BASE + 25)
#define ERROR_NO_FREE_SPACE			(ERROR_SYSTEM_BASE + 26)
#define ERROR_NOT_REGISTERED		(ERROR_SYSTEM_BASE + 27)
#define ERROR_OUT_OF_RANGE			(ERROR_SYSTEM_BASE + 28)
#define ERROR_BAD_PARAMETER			(ERROR_SYSTEM_BASE + 29)
#define ERROR_DEVICE_SEM			(ERROR_SYSTEM_BASE + 30)
#define ERROR_NO_FONT_SELECTED		(ERROR_SYSTEM_BASE + 31)
#define ERROR_THREAD_PRIORITY		(ERROR_SYSTEM_BASE + 32)
#define ERROR_THREAD_STACK			(ERROR_SYSTEM_BASE + 33)
#define ERROR_THREAD_OS_CREATE		(ERROR_SYSTEM_BASE + 34)
#define ERROR_HOTKEY_LIST_FULL		(ERROR_SYSTEM_BASE + 35)
#define ERROR_HOTKEY_NOT_REGISTERED	(ERROR_SYSTEM_BASE + 36)
#define ERROR_TIMER_CREATE_PARAM	(ERROR_SYSTEM_BASE + 37)
#define ERROR_TIMER_LIST_FULL		(ERROR_SYSTEM_BASE + 38)
#define ERROR_MEM_POINTER			(ERROR_SYSTEM_BASE + 40)
#define ERROR_MEM_NO_PARTITION		(ERROR_SYSTEM_BASE + 41)
#define ERROR_NULL_POINTER			(ERROR_SYSTEM_BASE + 42)
#define ERROR_NODE_NOT_FOUND		(ERROR_SYSTEM_BASE + 43)
#define ERROR_MENU_NO_ITEMS			(ERROR_SYSTEM_BASE + 44)
#define ERROR_OS_MBOX_CREATE		(ERROR_SYSTEM_BASE + 45)

#define MAKE_DATETIME_UINT(a, b, c)	((UINT)(a) * 10000 + (UINT)(b) * 100 + (UINT)c)

#define LAN_CHINESE					1
#define LAN_ENGLISH					2

// 时间
typedef struct TTime {
	byte	hour;
	byte	minute;
	byte	second;
} TTime;

// 日期
typedef struct TDate {
	ushort	year;
	byte	month;
	byte	day;
	byte	week;
} TDate;

// 日期时间
typedef struct TDateTime {
	ushort	year;
	byte	month;
	byte	day;
	byte	week;
	byte	hour;
	byte	minute;
	byte	second;
} TDateTime;

TDate * GetCurrentDate(TDate *date);
TTime * GetCurrentTime(TTime *time);
TDateTime * GetCurrentDateTime(TDateTime *datetime);
bool SetCurrentDate(const TDate *date);
bool SetCurrentTime(const TTime *time);
bool SetCurrentDateTime(const TDateTime *datetime);

char * TimeToString(char *buffer, const TTime *time, int language);
char * DateToString(char *buffer, const TDate *date, int language);
char *DateTimeToString(char *buffer, const TDateTime *datetime, int language);

int CompareDate(const TDate *date1, const TDate *date2);
int CompareTime(const TTime *time1, const TTime *time2);
int CompareDateTime(const TDateTime *datetime1, const TDateTime *datetime2);

// 蜂鸣器
void enable_beep(bool enable);
void beep();

// 错误处理
int GetLastError();
void SetLastError(int code);
const char *GetErrorMessage(int level, int ErrorCode);
void halt(const char *msg);
void RawDebugOutputString(const char *buffer);
void RawDebugPrintf(const char *fmt, ...);

// 中断管理
bool irq_register(int id, irq_handler_t irq_handler, bool raw);
void irq_set_priority(int id, int priority);
void irq_set_trigger_mode(int id, int mode);
void irq_release(int id);
void irq_enable(int id);
void irq_disable(int id);
void cpu_timer_init(int id, int priority, int interval);

void ARMDisableInt();
void ARMEnableInt();

// 时钟
ulong GetTickCount();
ulong GetTickDistance(ulong oldTick, ulong newTick);
void sleep(ulong ms);
void udelay(ulong us);

// 多任务
typedef enum {
	WAIT_OBJECT,
	WAIT_TIMEOUT,
	WAIT_FAILED,
} TWaitResult;

// 线程最小堆栈大小
#define MIN_THREAD_STACK_SIZE       128

#define KERNEL_THREAD_PRIORITY		0
#define DEBUG_THREAD_PRIORITY		1
#define APP_THREAD_PRIORITY			2

// 线程
THandle CreateThread(void (* routine)(void *), void *arg, UINT stacksize, UINT priority);
bool KillThread(THandle hThread);
UINT GetCurrentThreadId();

// 信号量
THandle CreateSemaphore(UINT count);
void CloseSemaphore(THandle hSemaphore);
TWaitResult WaitSemaphore(THandle hSemaphore, UINT timeout);
bool SignalSemaphore(THandle hSemaphore);
bool SetSemaphore(THandle hSemaphore, UINT count);

// 邮箱
THandle CreateMailBox(UINT size);
void CloseMailBox(THandle hMailBox);
TWaitResult FetchMailBox(THandle hMailBox, void **data, UINT timeout);
bool PostMailBox(THandle hMailBox, void *data);

#endif
