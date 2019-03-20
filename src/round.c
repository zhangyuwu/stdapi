
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/

#include <math.h>

int round(double f)
{
	if (f - floor(f) >= 0.5)
		return (int)ceil(f);
		
	return (int)floor(f);
}
