
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/

#include <types.h>

bool RegisterHotkey(wchar_t key, Event routine, void *arg);
bool UnregisterHotkey(wchar_t key);
bool IsHotkeyRegistered(wchar_t key);
void ProcessHotkey(wchar_t key);
void EnableHotkeys();
void DisableHotkeys();
