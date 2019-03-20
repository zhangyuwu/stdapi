
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/

#include <system.h>
#include <magic.h>
#include <font.h>
#include <string.h>
#include <fixedsys_8x16.h>
#include <songti_8x8.h>

TFont g_Fonts[MAX_FONTS];
TFont *g_CurrentFont[2] = { NULL, NULL };		// 0: ansi, 1: wide

// ----------------------------------------------------------------------------
bool RegisterFont(TFont *font)
{
	int i, freeIndex = -1;
	
	// �ж�Ҫע����豸�Ƿ�ϸ�
	if (font->Name[0] == '\0') {
		SetLastError(ERROR_INVALID_NAME);
		return false;
	}
	
	// ���������Ƿ�ע��
	for (i = 0; i < sizeof(g_Fonts) / sizeof(TFont); i++) {
		if (g_Fonts[i].MagicNumber == MAGIC_NUMBER_FONT) {
			if (!strncmp(g_Fonts[i].Name, font->Name, MAX_FONT_NAME_LENGTH)) {
				SetLastError(ERROR_FILE_EXISTS);
				return false;
			}
		}
		else {
			if (freeIndex == -1) {
				freeIndex = i;
			}
		}
	}
	
	// û�п��е��豸�ṹ
	if (freeIndex == -1) {
		SetLastError(ERROR_NO_FREE_SPACE);
		return false;
	}
	
	g_Fonts[freeIndex] = *font;
	g_Fonts[freeIndex].MagicNumber = MAGIC_NUMBER_FONT;
	g_Fonts[freeIndex].ItemSize = (g_Fonts[freeIndex].Width * g_Fonts[freeIndex].Height / 8);
	
	SetLastError(NO_ERROR);
	
	return true;
}

// ----------------------------------------------------------------------------
bool UnregisterFont(const char *name)
{
	int i;
	
	// ����ע�������
	for (i = 0; i < sizeof(g_Fonts) / sizeof(TFont); i++) {
		if (g_Fonts[i].MagicNumber == MAGIC_NUMBER_FONT) {
			if (!strncmp(g_Fonts[i].Name, name, MAX_FONT_NAME_LENGTH)) {
				memset(&g_Fonts[i], 0, sizeof(TFont));
				SetLastError(NO_ERROR);
				return true;
			}
		}
	}

	// ָ������δע��
	SetLastError(ERROR_NOT_REGISTERED);
	return false;
}

// ----------------------------------------------------------------------------
TFont *GetCurrentFont(int type)
{
	switch (type) {
	case FONT_TYPE_ANSI:
	case FONT_TYPE_WIDE:
		return g_CurrentFont[type];
	default:
		SetLastError(ERROR_BAD_PARAMETER);
		return NULL;
	}
}

// ----------------------------------------------------------------------------
bool SetCurrentFont(int type, TFont *font)
{
	if (font->MagicNumber == MAGIC_NUMBER_FONT) {
		switch (type) {
		case FONT_TYPE_ANSI:
			g_CurrentFont[type] = font;
			break;
		case FONT_TYPE_WIDE:
			g_CurrentFont[type] = font;
			break;
		default:
			SetLastError(ERROR_BAD_PARAMETER);
			return false;
		}
		
		return true;
	}
	
	return false;
}

// ----------------------------------------------------------------------------
bool SelectCurrentFont(int type, const char *name)
{
	int i;
	
	// ����ע�������
	for (i = 0; i < sizeof(g_Fonts) / sizeof(TFont); i++) {
		if (g_Fonts[i].MagicNumber == MAGIC_NUMBER_FONT) {
			if (!strncmp(g_Fonts[i].Name, name, MAX_FONT_NAME_LENGTH)) {
			
				switch (type) {
				case FONT_TYPE_ANSI:
					break;
				case FONT_TYPE_WIDE:
					break;
				default:
					SetLastError(ERROR_BAD_PARAMETER);
					return false;
				}

				g_CurrentFont[type] = &g_Fonts[i];
				SetLastError(NO_ERROR);
				return true;
			}
		}
	}

	// ָ������δע��
	SetLastError(ERROR_NOT_REGISTERED);
	return false;
}

// ----------------------------------------------------------------------------
// ��������ĵ�����Ϣ����������
bool LoadCharacter(wchar_t ch, unsigned char *buffer, int *width, int *height)
{
	int offset;
	const int WIDTH = 0xFE - 0xA1 + 1;
	TFont *font;
	
	if (ch > 0x100) {
		if (g_CurrentFont[FONT_TYPE_WIDE] != NULL) {
			font = g_CurrentFont[FONT_TYPE_WIDE];
			offset = ((((ch & 0xFF00) >> 8) - 0xA1) * WIDTH + (ch & 0xFF) - 0xA1) * font->ItemSize;
			memcpy(buffer, font->StartAddress + offset, font->ItemSize);
			*width = font->Width;
			*height = font->Height;
			SetLastError(NO_ERROR);
			return true;
		}
	}
	else {
		if (g_CurrentFont[FONT_TYPE_ANSI] != NULL) {
			font = g_CurrentFont[FONT_TYPE_ANSI];
			offset = font->ItemSize * ch;
			memcpy(buffer, font->StartAddress + offset, font->ItemSize);
			*width = font->Width;
			*height = font->Height;
			SetLastError(NO_ERROR);
			return true;
		}
	}
	
	// û���ҵ�ָ��������
	SetLastError(ERROR_NO_FONT_SELECTED);
	return false;
}

// ----------------------------------------------------------------------------
// ��װϵͳ���壺fixedsys
void font_init()
{
	TFont font;
	
	memset(g_Fonts, 0, sizeof(g_Fonts));
	
	// register fixedsys_8x16
	strcpy(font.Name, "Fixedsys_16x8");
	font.Width = 8;
	font.Height = 16;
	font.StartAddress = (void *)g_FixedsysFont_8x16;
	RegisterFont(&font);
	SelectCurrentFont(FONT_TYPE_ANSI, "Fixedsys_16x8");

	strcpy(font.Name, "SongTi_8x8");
	font.Width = 8;
	font.Height = 8;
	font.StartAddress = (void *)g_SongTiFont_8x8;
	RegisterFont(&font);
}
