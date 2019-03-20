
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/


#ifndef __GUtil_H__
#define __GUtil_H__

typedef enum {
	NETWORK_STATUS_NONE		=	0,
	NETWORK_STATUS_LOW		=	1,
	NETWORK_STATUS_MID		=	2,
	NETWORK_STATUS_HIGH		=	3,
	NETWORK_STATUS_FULL		=	4,
} TNetworkStatus;

typedef enum {
	BATTERY_STATUS_EMPTY	=	0,
	BATTERY_STATUS_LOW		=	1,
	BATTERY_STATUS_MID		=	2,
	BATTERY_STATUS_FULL		=	3,
} TBatteryStatus;

void ShowNetworkStatus(int x, int y, TNetworkStatus status);
void ShowBatteryStatus(int x, int y, TBatteryStatus status);

#endif
