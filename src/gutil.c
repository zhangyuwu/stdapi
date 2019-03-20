
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/


#include <gutil.h>
#include <graphics.h>

void ShowNetworkStatus(int x, int y, TNetworkStatus status)
{
	static TNetworkStatus prev_status = NETWORK_STATUS_NONE;
	const int _y[] = { 6, 5, 4, 2 };
	const int interval = 3;
	int i;
	
	if (status < 0 || status > NETWORK_STATUS_FULL) {
		return;
	}

	if (status < prev_status) {
		ClearRectangle(x, y, x + 12, y + 8);
	}
	
	for (i = NETWORK_STATUS_LOW; i <= NETWORK_STATUS_FULL && i <= status; i++) {
		FillRectangle(x + (i - 1) * interval, y + _y[i-1], x + (i - 1) * interval + 1, y + 7);
	}
	
	prev_status = status;
}

void ShowBatteryStatus(int x, int y, TBatteryStatus status)
{
	static TBatteryStatus prev_status = BATTERY_STATUS_EMPTY;
	int i;
	
	if (status < 0 || status > BATTERY_STATUS_FULL) {
		return;
	}
		
	if (status < prev_status) {
		ClearRectangle(x, y, x + 12, y + 8);
	}
	
	Rectangle(x + 1, y, x + 12, y + 6);
	Line(x, y + 2, x, y + 4);
	
	for (i = BATTERY_STATUS_LOW; i <= BATTERY_STATUS_FULL && i <= status; i++) {
		FillRectangle(x + 3 + (i - 1) * 3, y + 2, x + 3 + (i - 1) * 3 + 1, y + 4);
	}
	
	prev_status = status;
}
