
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
*      char * strtoken(char * str, const char * delimiters, bool skipsame)
*  ���ܣ�
*      ��һ���ַ���ȡ������ָ���ָ��ַ��ָ������ִ����Զ���������ָ���
*  ������
*      str             ----    ���ָ����ִ�
*      delimiters      ----    �ָ��ַ���
*      skipsame        ----    �Ƿ��ظ�������ͬ�ķָ��ַ���
*  ����ֵ��
*      �õ������ִ�
*  ˵����
*      ���ڶ���ķָ�������δ��������ַ��� "Hello!!! John" �ᱻ "! " �ָ�Ϊ
*      { "Hello", "John" }
*      ���str��NULL����ôȡ����һ���ִ�����һ��
* -----------------------------------------------------------------------*/

char * strtoken(char * str, const char * delimiters)
{
	static char * p = NULL, result[256];
	int i, length, skip = 0;
	bool first = true;
	
	if (str) p = str;
	else str = p;
	if (!p || strlen(p) == 0) return NULL;
	
	memset(result, 0, sizeof(result));
	
	if (p) {
		for (i = 0, length = strlen(p); i < length; i++) {
			if (strchr(delimiters, str[i])) {
				// ������ͬ�ķָ���
				if (first) {
					p++;
					skip++;
				}
				else {
					strncpy(result, p, i - skip);
					p += (i - skip + 1);
					return result;
				}
			}
			else {
				first = false;
			}
		}
		strcpy(result, p);
		
		// ��������ַ����Ѿ���������ô��һ�η���NULL
		if (i == length) p = NULL;
	}
	
	return result;
}
