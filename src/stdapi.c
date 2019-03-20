
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
	
	// ��ʼ�� lwip
	lwip_init();
	
	// ��ʼ��������ʱ��
	timers_init();
	
	// ע����������
	RegisterDrivers();

	// ע�Ẻ����
	RegisterChineseFont();
	
	// ���ر�׼���
	SetStdHandle(STD_OUTPUT_HANDLE, OpenDevice("LCD", MAKEWORD32(128, 64)));
	SetStdHandle(STD_ERROR_HANDLE, GetStdHandle(STD_OUTPUT_HANDLE));
	SetStdHandle(STD_DEBUG_HANDLE, OpenDevice("COM1", BAUD_115200));
#if 1
	SetStdHandle(STD_INPUT_HANDLE, OpenDevice("KEYBOARD", 0));
#else
	SetStdHandle(STD_INPUT_HANDLE, GetStdHandle(STD_DEBUG_HANDLE));
#endif

	// ��������ϵͳ��ʱ��
	cpu_timer_init(TC0_ID, IRQ_PRIORITY_LOWEST, 10);		// OS ��ʱ���ж�������ȼ�
	cpu_timer_init(TC1_ID, IRQ_PRIORITY_NORMAL, 10);		// ���涨ʱ��
	
	irq_set_trigger_mode(TC0_ID, IRQ_TRIGGER_NEGATIVE);
	irq_set_trigger_mode(TC1_ID, IRQ_TRIGGER_NEGATIVE);

	// ������������
	RawDebugOutputString("Debug thread now starting...\n");
	StartDebugTask();
	
	// ������������
	RawDebugOutputString("Application now starting...\n");
    OSTaskCreate(main, (void *)0, &AppTaskStack[APP_TASK_STACK_SIZE - 1], APP_THREAD_PRIORITY);
	
	while (true) {
		// ����Һ��ˢ��
		refresh_lcd();

		// ����
		sleep(100);
	}
}

void read_tc_sr()
{
	TC0_BASE->TC_SR;						// �����ʱ�� TC0 ״̬
	TC1_BASE->TC_SR;						// �����ʱ�� TC1 ״̬
	TC2_BASE->TC_SR;						// �����ʱ�� TC2 ״̬
}

void dev_init()
{
	extern void lcd_init();
	extern void ds1302_init();
	
	// Һ����ʼ��
	lcd_init();
	
	// �رշ�����
	enable_beep(false);
	
	// ds1302 ʱ��
	ds1302_init();
	
	// ��ֹ RS485 �շ�
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

	// ��ʼ��uCos-ii
	OSInit();
	
	// ��ʼ�� newlib reent table
	reent_init();

	// ��ʼ����̬�ڴ�
	dynamic_mem_init();

	// �ײ��ʼ��
	irq_init();
	dev_init();
	font_init();
	
	// ע���ж�
	irq_register(TC0_ID, OS_CPU_IRQ_ISR, true);
	irq_register(TC1_ID, isr_timer, false);
	irq_register(US0_ID, isr_serial_port, false);
	irq_register(US1_ID, isr_serial_port, false);
	irq_register(IRQ1_ID, isr_eth, false);

	// �ҹ��ײ㶨ʱ��
	OS_CPU_HookISR(read_tc_sr);	
}

void timers_init()
{
	extern void isr_ds1302();
	THandle hTimer;

	// ��������ɨ��
	hTimer = CreateTimer((timer_handler_t)isr_keyboard, NULL, 30);
	ASSERT(hTimer);
	StartTimer(hTimer);

	// ������̫���쳣����
	hTimer = CreateTimer((timer_handler_t)eth_monitor, NULL, 200);
	ASSERT(hTimer);
	StartTimer(hTimer);
	
	// ���� dallas 1302 ʱ�Ӹ��¶�ʱ��
	hTimer = CreateTimer((timer_handler_t)isr_ds1302, NULL, 1000);
	ASSERT(hTimer);
	StartTimer(hTimer);
}

void __main()
{
	// �ײ��ʼ��
	low_init();
	
	ClearScreen();
	
	// ������������
	RawDebugOutputString("uC/OS-II now starting......\n");
    OSTaskCreate(KernelTask, (void *)0, &KernelTaskStack[KERNEL_TASK_STACK_SIZE - 1], KERNEL_THREAD_PRIORITY);
	
	// ��������ϵͳ
	OSStart();
}

