
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/

#include <stdapi.h>
#include <serial.h>
#include <timer.h>
#include <lcd.h>
#include <console.h>
#include <keyboard.h>
#include <rtl8019.h>
#include <ucos_ii.h>
#include <queue.h>
#include <debug.h>
#include <hardware.h>

#define KERNEL_TASK_STACK_SIZE		1024
#define APP_TASK_STACK_SIZE			1024

static OS_STK KernelTaskStack[KERNEL_TASK_STACK_SIZE];
static OS_STK AppTaskStack[APP_TASK_STACK_SIZE];

// ----------------------------------------------------------------------------
void RegisterChineseFont()
{
	TFont font;
	
	strcpy(font.Name, "SongTi_16x16");
	font.Width = 16;
	font.Height = 16;
	font.StartAddress = (void *)0x01010000;
	RegisterFont(&font);
	SelectCurrentFont(FONT_TYPE_WIDE, "SongTi_16x16");
}

void RegisterDrivers()
{
	TDevice device;

	// register COM1
	memset(&device, 0, sizeof(TDevice));
	strcpy(device.Name, "COM1");
	device.Open = OpenSerialPort;
	device.Read = ReadSerialPort;
	device.Write = WriteSerialPort;
	device.Release = ReleaseSerialPort;
	device.Opened = false;
	RegisterDevice(&device);
	
	// register COM2
	memset(&device, 0, sizeof(TDevice));
	strcpy(device.Name, "COM2");
	device.Open = OpenSerialPort;
	device.Read = ReadSerialPort;
	device.Write = WriteSerialPort;
	device.Release = ReleaseSerialPort;
	RegisterDevice(&device);	
	
	// register Keyboard
	memset(&device, 0, sizeof(TDevice));
	strcpy(device.Name, "KEYBOARD");
	device.Open = OpenKeyboard;
	device.Read = ReadKeyboard;
	RegisterDevice(&device);
	
	// register LCD
	memset(&device, 0, sizeof(TDevice));
	strcpy(device.Name, "LCD");
	device.Open = OpenLCD;
	device.Write = WriteLCD;
	RegisterDevice(&device);
	
}

void KernelTask(void * arg)
{
	extern void main();
	extern void lwip_init();
	extern void timers_init();
	
	// 初始化 lwip
	lwip_init();
	
	// 初始化各个定时器
	timers_init();
	
	// 注册驱动程序
	RegisterDrivers();

	// 注册汉子体
	RegisterChineseFont();
	
	// 加载标准输出
	SetStdHandle(STD_OUTPUT_HANDLE, OpenDevice("LCD", MAKEWORD32(128, 64)));
	SetStdHandle(STD_ERROR_HANDLE, GetStdHandle(STD_OUTPUT_HANDLE));
	SetStdHandle(STD_DEBUG_HANDLE, OpenDevice("COM1", BAUD_115200));
#if 1
	SetStdHandle(STD_INPUT_HANDLE, OpenDevice("KEYBOARD", 0));
#else
	SetStdHandle(STD_INPUT_HANDLE, GetStdHandle(STD_DEBUG_HANDLE));
#endif

	// 启动操作系统定时器
	cpu_timer_init(TC0_ID, IRQ_PRIORITY_LOWEST, 10);		// OS 定时器中断最低优先级
	cpu_timer_init(TC1_ID, IRQ_PRIORITY_NORMAL, 10);		// 常规定时器
	
	irq_set_trigger_mode(TC0_ID, IRQ_TRIGGER_NEGATIVE);
	irq_set_trigger_mode(TC1_ID, IRQ_TRIGGER_NEGATIVE);

	// 创建调试任务
	RawDebugOutputString("Debug thread now starting...\n");
	StartDebugTask();
	
	// 创建核心任务
	RawDebugOutputString("Application now starting...\n");
    OSTaskCreate(main, (void *)0, &AppTaskStack[APP_TASK_STACK_SIZE - 1], APP_THREAD_PRIORITY);
	
	while (true) {
		// 处理液晶刷新
		refresh_lcd();

		// 休眠
		sleep(100);
	}
}

void read_tc_sr()
{
	TC0_BASE->TC_SR;						// 清除定时器 TC0 状态
	TC1_BASE->TC_SR;						// 清除定时器 TC1 状态
	TC2_BASE->TC_SR;						// 清除定时器 TC2 状态
}

void dev_init()
{
	extern void lcd_init();
	extern void ds1302_init();
	
	// 液晶初始化
	lcd_init();
	
	// 关闭蜂鸣器
	enable_beep(false);
	
	// ds1302 时钟
	ds1302_init();
	
	// 禁止 RS485 收发
	outp16(RS485_ADDR, 0x80);
	outp16(RS485_ADDR, 0x00);
}

void low_init()
{
	extern void irq_init();
	extern void isr_eth();
	extern void font_init();
	extern void reent_init();
	extern void dynamic_mem_init();

	// 初始化uCos-ii
	OSInit();
	
	// 初始化 newlib reent table
	reent_init();

	// 初始化动态内存
	dynamic_mem_init();

	// 底层初始化
	irq_init();
	dev_init();
	font_init();
	
	// 注册中断
	irq_register(TC0_ID, OS_CPU_IRQ_ISR, true);
	irq_register(TC1_ID, isr_timer, false);
	irq_register(US0_ID, isr_serial_port, false);
	irq_register(US1_ID, isr_serial_port, false);
	irq_register(IRQ1_ID, isr_eth, false);

	// 挂钩底层定时器
	OS_CPU_HookISR(read_tc_sr);	
}

void timers_init()
{
	extern void isr_ds1302();
	THandle hTimer;

	// 启动键盘扫描
	hTimer = CreateTimer((timer_handler_t)isr_keyboard, NULL, 30);
	ASSERT(hTimer);
	StartTimer(hTimer);

	// 启动以太网异常监视
	hTimer = CreateTimer((timer_handler_t)eth_monitor, NULL, 200);
	ASSERT(hTimer);
	StartTimer(hTimer);
	
	// 启动 dallas 1302 时钟更新定时器
	hTimer = CreateTimer((timer_handler_t)isr_ds1302, NULL, 1000);
	ASSERT(hTimer);
	StartTimer(hTimer);
}

void __main()
{
	// 底层初始化
	low_init();
	
	ClearScreen();
	
	// 创建核心任务
	RawDebugOutputString("uC/OS-II now starting......\n");
    OSTaskCreate(KernelTask, (void *)0, &KernelTaskStack[KERNEL_TASK_STACK_SIZE - 1], KERNEL_THREAD_PRIORITY);
	
	// 启动操作系统
	OSStart();
}

