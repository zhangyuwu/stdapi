
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/
 
#include <graphics.h>
#include <stdio.h>
#include <system.h>

// ----------------------------------------------------------------------------
void Polygon(TPoint pt[], int count)
{
    int i;
    if (count > 2) {
        for (i = 0; i < count - 1; i++)
            Line(pt[i].x, pt[i].y, pt[i+1].x, pt[i+1].y);
        Line(pt[count-1].x, pt[count-1].y, pt[0].x, pt[0].y);
    }
}
