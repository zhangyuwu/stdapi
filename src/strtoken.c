
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
*  函数名：
*      char * strtoken(char * str, const char * delimiters, bool skipsame)
*  功能：
*      从一个字符串取出按照指定分隔字符分隔的子字串，自动跳过多个分隔符
*  参数：
*      str             ----    被分隔的字串
*      delimiters      ----    分隔字符集
*      skipsame        ----    是否不重复处理相同的分隔字符集
*  返回值：
*      得到的子字串
*  说明：
*      对于多个的分隔符不多次处理，例如字符串 "Hello!!! John" 会被 "! " 分隔为
*      { "Hello", "John" }
*      如果str是NULL，那么取出上一条字串的下一条
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
				// 过滤相同的分隔符
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
		
		// 如果整个字符串已经结束，那么下一次返回NULL
		if (i == length) p = NULL;
	}
	
	return result;
}
