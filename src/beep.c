
#include <hardware.h>
#include <system.h>
#include <timer.h>

void enable_beep(bool enable)
{
	outp16(BEEP_ADDR, enable ? 1 : 0);
}

void beep_timer(void *arg)
{
	THandle hTimer = arg;
	enable_beep(false);
	StopTimer(hTimer);
}

void beep()
{
	static THandle hTimer = NULL;
	if (hTimer == NULL) {
		hTimer = CreateTimer(beep_timer, hTimer, 100);
	}
	
	if (hTimer) {
		enable_beep(true);
		StartTimer(hTimer);
	}
}
