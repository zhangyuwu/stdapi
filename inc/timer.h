
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/

#ifndef __Timer_H__
#define __Timer_H__

#include <types.h>

#define MAX_TIMER_COUNT			8

THandle CreateTimer(timer_handler_t routine, void *arg, UINT interval);
void ReleaseTimer(THandle timer);
void StartTimer(THandle timer);
void StopTimer(THandle timer);
UINT GetTimerInterval(THandle timer);
void SetTimerInterval(THandle timer, UINT interval);

void isr_timer();

#endif
