
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
#include <math.h>
#include <util.h>

char * float2str(double f, char *buffer)
{
	const int N = 8;
	int i = 0, len;
	char ch, *ret = buffer;
	int value;
	int n_zero;
	int n_base = 1;

	// С������
	if (f < 0) {
		f = -f;
		buffer[0] = '-';
		buffer++;
	}
	
	for (i = 0; i < N; i++) n_base *= 10;
	value = (int)((double)n_base * (f - floor(f)));

	// ����С��������0����
	n_zero = N;
	i = value;
	while (i > 0) {
		i /= 10;
		n_zero--;
	}

	// ���С�����������β��λ��
	while (value && value % 10 == 0) {
		value /= 10;
	}

	i = 0;
	do {
		buffer[i++] = (value % 10) + '0';
		value /= 10;
	} while (value > 0);
	
	// ���С�����ֵ�0
	while (n_zero > 0) {
		buffer[i++] = '0';
		n_zero--;
	}
	
	// С����
	buffer[i++] = '.';

	// ��������
	value = (int)f;
	do {
		buffer[i++] = (value % 10) + '0';
		value /= 10;
	} while (value > 0);

	buffer[i] = '\0';
	
	// ��ת�ַ���
	len = strlen(buffer);
	for (i = 0; i < len / 2; i++) {
		ch = buffer[i];
		buffer[i] = buffer[len - i - 1];
		buffer[len - i - 1] = ch;
	}
	
	return ret;
}
