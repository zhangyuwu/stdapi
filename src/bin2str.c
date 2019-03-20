
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/


#include <types.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <util.h>

/* ------------------------------------------------------------------------
 *  ��������
 *      int bin2str(const byte * bin, char * str, int length)
 *  ���ܣ�
 *      ��һ���ֽ���ת�����ַ���
 *  ������
 *      bin             ----    ������ֽ���
 *      str             ----    ������ַ�����
 *      length          ----    Ҫת�����ֽ����ĳ���
 *  ����ֵ��
 *      ת������ַ����ܳ���
 *  ˵����
 *      bin --> {A1, B2, C3, D4, FF...}, str --> "A1 B2 C3 D4 FF..."
 * -----------------------------------------------------------------------*/

int bin2str(const byte * bin, char * str, int length)
{
	int i, pos = 0;
	byte b;

	for (i = 0; i < length; i++) {
		b = bin[i] / 0x10;
		if (b <= 9) {
			str[pos] = '0' + b;
		}
		else {
			str[pos] = 'A' + b - 0xA;
		}
		pos++;

		b = bin[i] % 0x10;
		if (b <= 9) {
			str[pos] = '0' + b;
		}
		else {
			str[pos] = 'A' + b - 0xA;
		}
		pos++;

		if (i != length - 1) {
			str[pos] = ' ';
			pos++;
		}
	}
	str[pos] = '\0';
	return pos;
}

