
#ifndef __VT100_H__
#define __VT100_H__

#define VT100_ATTR_OFF      0
#define VT100_BOLD          1
#define VT100_USCORE        4
#define VT100_BLINK         5
#define VT100_REVERSE       7
#define VT100_BOLD_OFF      21
#define VT100_USCORE_OFF    24
#define VT100_BLINK_OFF     25
#define VT100_REVERSE_OFF   27

// functions

//! vt100Init() initializes terminal and vt100 library
///     Run this init routine once before using any other vt100 function.
void vt100Init();

//! vt100ClearScreen() clears the terminal screen
void vt100ClearScreen();

//! vt100SetAttr() sets the text attributes like BOLD or REVERSE
///     Text written to the terminal after this function is called will have
///     the desired attribuutes.
void vt100SetAttr(unsigned char attr);

//! vt100SetCursorMode() sets the cursor to visible or invisible
void vt100SetCursorMode(unsigned char visible);

//! vt100SetCursorPos() sets the cursor position
///     All text which is written to the terminal after a SetCursorPos command
///     will begin at the new location of the cursor.
void vt100SetCursorPos(unsigned char line, unsigned char col);

#endif
