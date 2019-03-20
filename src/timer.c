
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/

#include <C_m40800.h>
#include <timer.h>
#include <magic.h>
#include <system.h>
#include <string.h>

// ----------------------------------------------------------------------------
typedef struct TTimer {
	UINT			MagicNumber;
	timer_handler_t	Routine;
	void *			Arg;
	UINT			Interval;
	UINT			Remain;
	bool			Enabled;
	bool			Running;
} TTimer;

TTimer timer_table[MAX_TIMER_COUNT];
unsigned long g_TickCount = 0;

// ----------------------------------------------------------------------------
THandle CreateTimer(timer_handler_t routine, void *arg, UINT interval)
{
	static bool timer_inited = false;
	int i;
	
	if (!timer_inited) {
		memset(timer_table, 0, sizeof(timer_table));
		timer_inited = true;
	}
	
	if (routine == NULL || interval == 0) {
		SetLastError(ERROR_TIMER_CREATE_PARAM);
		return NULL;
	}
		
	for (i = 0; i < sizeof(timer_table)/sizeof(TTimer); i++) {
		if (timer_table[i].MagicNumber != MAGIC_NUMBER_TIMER) {
			timer_table[i].MagicNumber = MAGIC_NUMBER_TIMER;
			timer_table[i].Routine = routine;
			timer_table[i].Arg = arg;
			timer_table[i].Interval = interval;
			timer_table[i].Remain = 0;
			timer_table[i].Enabled = false;
			timer_table[i].Running = false;
			
			SetLastError(NO_ERROR);
			return &timer_table[i];
		}
	}
	
	SetLastError(ERROR_TIMER_LIST_FULL);
	return NULL;
}

// ----------------------------------------------------------------------------
void ReleaseTimer(THandle timer)
{
	TTimer *p = (TTimer *)timer;
	
	if (p->MagicNumber == MAGIC_NUMBER_TIMER) {
		p->Enabled = false;
		p->Running = false;
		p->MagicNumber = 0;
		p->Routine = NULL;
		p->Interval = 0;
		p->Remain = 0;
		
		SetLastError(NO_ERROR);
	}
	else {
		SetLastError(ERROR_INVALID_HANDLE);
	}
}

// ----------------------------------------------------------------------------
void StartTimer(THandle timer)
{
	TTimer *p = (TTimer *)timer;
	
	if (p->MagicNumber == MAGIC_NUMBER_TIMER) {
		p->Remain = p->Interval;
		p->Enabled = true;
		SetLastError(NO_ERROR);
	}
	else {
		SetLastError(ERROR_INVALID_HANDLE);
	}
}

// ----------------------------------------------------------------------------
void StopTimer(THandle timer)
{
	TTimer *p = (TTimer *)timer;
	
	if (p->MagicNumber == MAGIC_NUMBER_TIMER) {
		p->Enabled = false;
		SetLastError(NO_ERROR);
	}
	else {
		SetLastError(ERROR_INVALID_HANDLE);
	}
}

// ----------------------------------------------------------------------------
UINT GetTimerInterval(THandle timer)
{
	TTimer *p = (TTimer *)timer;
	
	if (p->MagicNumber == MAGIC_NUMBER_TIMER) {
		SetLastError(NO_ERROR);
		return p->Interval;
	}
	else {
		SetLastError(ERROR_INVALID_HANDLE);
		return -1;
	}
}

// ----------------------------------------------------------------------------
void SetTimerInterval(THandle timer, UINT interval)
{
	TTimer *p = (TTimer *)timer;
	
	if (p->MagicNumber == MAGIC_NUMBER_TIMER) {
		SetLastError(NO_ERROR);
		p->Interval = interval;
	}
	else {
		SetLastError(ERROR_INVALID_HANDLE);
	}
}

#define TC1_INTERVAL		10

// ----------------------------------------------------------------------------
void isr_timer()
{
	static int i;
		
	// 计数
	g_TickCount += TC1_INTERVAL;
	
	for (i = 0; i < sizeof(timer_table)/sizeof(TTimer); i++) {
		if (timer_table[i].MagicNumber == MAGIC_NUMBER_TIMER) {
			if (timer_table[i].Enabled) {
				if (timer_table[i].Remain <= 0 && !timer_table[i].Running) {
					timer_table[i].Running = true;
					timer_table[i].Remain = timer_table[i].Interval;
					timer_table[i].Routine(timer_table[i].Arg);
					timer_table[i].Running = false;
				}
				else {
					timer_table[i].Remain -= TC1_INTERVAL;
				}
			}
		}
	}

	// 清除定时器 TC1 状态
	TC1_BASE->TC_SR;	
}

// ----------------------------------------------------------------------------
unsigned long GetTickCount()
{
	return g_TickCount;
}

// ----------------------------------------------------------------------------
// End of timer.c
