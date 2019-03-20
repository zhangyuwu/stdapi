
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/

#include <system.h>
#include <device.h>
#include <queue.h>

// ----------------------------------------------------------------------------
#define KEYBOARD_PORT					0x02200000
#define MAX_KEYBOARD_BUFFER_LENGTH		64

// ----------------------------------------------------------------------------
typedef struct {
	TQueue		Queue;
	byte		Buffer[MAX_KEYBOARD_BUFFER_LENGTH];
	bool		Enabled;
	int			RepeatInterval;
} TKeyboardConfig;

TKeyboardConfig g_KeyboardConfig = {
	.Enabled = false,
	.RepeatInterval = 500,		// 100 ms by default
};

// ----------------------------------------------------------------------------
bool OpenKeyboard(struct TDevice *dev)
{
	InitializeQueue(&g_KeyboardConfig.Queue, g_KeyboardConfig.Buffer, sizeof(char), sizeof(g_KeyboardConfig.Buffer)/sizeof(char));
	g_KeyboardConfig.Enabled = true;

	return true;
}

// ----------------------------------------------------------------------------
int ReadKeyboard(struct TDevice *dev, byte *buffer, UINT count)
{
	int i = 0;
	char ch;
	
	while (i < count) {
		if (!IsQueueEmpty(&g_KeyboardConfig.Queue)) {
			PopQueue(&g_KeyboardConfig.Queue, &ch);
			buffer[i++] = ch;
		}
		else {
			break;
		}
	}
	
	return i;
}

// ----------------------------------------------------------------------------
// 8x8 矩阵键盘扫描程序
int ScanKeyboardCode()
{
	static unsigned short value;
	static int i, j;
	
	outp16(KEYBOARD_PORT, 0x00);
	
	if (inp16(KEYBOARD_PORT) == 0xff) {
		outp16(KEYBOARD_PORT, 0xff);
		return -1;
	}
	
	for (i = 0; i < 8; i++) {
		outp16(KEYBOARD_PORT, ~(1 << i));
		value = inp16(KEYBOARD_PORT);
		
		if (value != 0xff) {
			for (j = 0; j < 8; j++) {
				if (!bit_tst(value, j)) {
					outp16(KEYBOARD_PORT, 0xff);
					return (i << 4) + j;
				}
			}
		}
	}
	
	outp16(KEYBOARD_PORT, 0xff);
	return -1;
}

// ----------------------------------------------------------------------------
// 键值映射
int MapKeyboardCode(int code)
{
	struct TCodeMapItem {
		int RawCode;
		int NewCode;
	};
	
	const struct TCodeMapItem codemap[] = {
		{ 0x10,		'.' },
		{ 0x00,		'0' },
		{ 0x03,		'1' },
		{ 0x13,		'2' },
		{ 0x23,		'3' },
		{ 0x02,		'4' },
		{ 0x12,		'5' },
		{ 0x22,		'6' },
		{ 0x01,		'7' },
		{ 0x11,		'8' },
		{ 0x21,		'9' },
		{ 0x33,		'A' },
		{ 0x43,		'B' },
		{ 0x32,		'C' },
		{ 0x42,		'D' },
		{ 0x31,		'E' },
		{ 0x41,		'F' },
		{ 0x53,		VK_UP },
		{ 0x52,		VK_DOWN },
		{ 0x50,		VK_LEFT },
		{ 0x51,		VK_RIGHT },
		{ 0x73,		VK_BACK },
		{ 0x72,		VK_RETURN },
		{ 0x71,		VK_ESCAPE },
		{ 0x20,		VK_F1 },
		{ 0x30,		VK_F2 },
		{ 0x40,		VK_F3 },
		{ 0x62,		VK_MENU },
		{ 0x63,		VK_CONFIG },
	};
	
	static int i;
	
	for (i = 0; i < sizeof(codemap) / sizeof(struct TCodeMapItem); i++) {
		if (codemap[i].RawCode == code) {
			return codemap[i].NewCode;
		}
	}
	
	return -1;
}

// ----------------------------------------------------------------------------
// 键盘中断处理程序
void isr_keyboard()
{
	static int value[2] = {0};			// [0] means old, [1] means new
	static ulong tick[2] = {0};			// [0] means old, [1] means new
	
	if (g_KeyboardConfig.Enabled) {
		value[1] = MapKeyboardCode(ScanKeyboardCode());
		
		// 没有键按下
		if (value[1] == -1) return;
		
		tick[1] = GetTickCount();
		
		if (value[1] != value[0]) {
			value[0] = value[1];
		}
		else {
			if (tick[1] - tick[0] < g_KeyboardConfig.RepeatInterval) {
				return;
			}
		}
		
		tick[0] = tick[1];
		PushQueue(&g_KeyboardConfig.Queue, &value[1]);
		
		beep();
	}
}

// ----------------------------------------------------------------------------
// E N D
