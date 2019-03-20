
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/

#include <hotkey.h>
#include <system.h>
#include <magic.h>
#include <types.h>
#include <stdio.h>

#define MAX_HOTKEY				8;

// ----------------------------------------------------------------------------
typedef struct {
	UINT	MagicNumber;
	wchar_t	KeyValue;
	Event	Routine;
	void *	Arg;
} THotkey;

// ----------------------------------------------------------------------------
THotkey g_Hotkeys[8] = {{0}};
bool g_HotkeysEnabled = true;

// ----------------------------------------------------------------------------
void EnableHotkeys()
{
	g_HotkeysEnabled = true;
}

// ----------------------------------------------------------------------------
void DisableHotkeys()
{
	g_HotkeysEnabled = false;
}

// ----------------------------------------------------------------------------
bool RegisterHotkey(wchar_t key, Event routine, void *arg)
{
	int i;
	
	for (i = 0; i < sizeof(g_Hotkeys)/sizeof(THotkey); i++) {
		if (g_Hotkeys[i].MagicNumber != MAGIC_NUMBER_HOTKEY) {
			g_Hotkeys[i].MagicNumber = MAGIC_NUMBER_HOTKEY;
			g_Hotkeys[i].KeyValue = key;
			g_Hotkeys[i].Routine = routine;
			g_Hotkeys[i].Arg = arg;
			
			SetLastError(NO_ERROR);
			return true;
		}
	}
	
	SetLastError(ERROR_HOTKEY_LIST_FULL);
	return false;
}

// ----------------------------------------------------------------------------
bool UnregisterHotkey(wchar_t key)
{
	int i;
	
	for (i = 0; i < sizeof(g_Hotkeys)/sizeof(THotkey); i++) {
		if (g_Hotkeys[i].MagicNumber == MAGIC_NUMBER_HOTKEY) {
			if (g_Hotkeys[i].KeyValue == key) {
				g_Hotkeys[i].MagicNumber = ~MAGIC_NUMBER_HOTKEY;
				g_Hotkeys[i].Routine = NULL;
				
				SetLastError(NO_ERROR);
				return true;
			}
		}
	}
	
	SetLastError(ERROR_HOTKEY_NOT_REGISTERED);
	return false;
}

// ----------------------------------------------------------------------------
bool IsHotkeyRegistered(wchar_t key)
{
	int i;
	
	for (i = 0; i < sizeof(g_Hotkeys)/sizeof(THotkey); i++) {
		if (g_Hotkeys[i].MagicNumber == MAGIC_NUMBER_HOTKEY) {
			if (g_Hotkeys[i].KeyValue == key) {
				return true;
			}
		}
	}
	
	return false;
}

// ----------------------------------------------------------------------------
void ProcessHotkey(wchar_t key)
{
	int i;
	
	if (g_HotkeysEnabled) {
		for (i = 0; i < sizeof(g_Hotkeys)/sizeof(THotkey); i++) {
			if (g_Hotkeys[i].MagicNumber == MAGIC_NUMBER_HOTKEY) {
				if (g_Hotkeys[i].KeyValue == key) {
					if (g_Hotkeys[i].Routine != NULL) {
						g_Hotkeys[i].Routine(g_Hotkeys[i].Arg);
					}
				}
			}
		}
	}
}
// ----------------------------------------------------------------------------
// E N D