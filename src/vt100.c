
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/

#include <stdio.h>
#include <vt100.h>

void vt100Init(void)
{
    // initializes terminal to "power-on" settings
    // ESC c
    printf("\x1B\x63");
}

void vt100ClearScreen(void)
{
    // ESC [ 2 J
    printf("\x1B[2J");
}

void vt100SetAttr(unsigned char attr)
{
    // ESC [ Ps m
    printf("\x1B[%dm", attr);
}

void vt100SetCursorMode(unsigned char visible)
{
    if(visible) {
        // ESC [ ? 25 h
        printf("\x1B[?25h");
	}
    else {
        // ESC [ ? 25 l
        printf("\x1B[?25l");
	}
}

void vt100SetCursorPos(unsigned char line, unsigned char col)
{
    // ESC [ Pl ; Pc H
    printf("\x1B[%d;%dH", line, col);
}
