
#include <ctype.h>
#include <system.h>
#include <console.h>
#include <device.h>

/* ------------------------------------------------------------------------
 *  函数名：
 *      bool InputString(char *buffer, int maxlen, int *length)
 *  功能：
 *      从终端键盘读取字符串
 *  参数：
 *      buffer          ----    接收的缓冲区
 *      type            ----    输入类型
 *      maxlen          ----    最大接收长度（字节）
 *      length          ----    实际接收的长度（字节）
 *  返回值：
 *      true            ----    用户按下确认返回
 *      false           ----    用户按下取消返回
 *  说明：
 *      如果用户按下取消或者确认都会返回
 * -----------------------------------------------------------------------*/

bool InputString(char *buffer, int type, int maxlen, int *length)
{
	char ch;
	int nLength = 0;
	
	while (true) {
		ch = GetKey();
		
		switch (ch) {
		case VK_ESCAPE:		// 取消
			buffer[nLength] = '\0';
			*length = nLength;
			return false;
			
		case VK_RETURN:		// 确认
			buffer[nLength] = '\0';
			*length = nLength;
			return true;
			
		case VK_BACK:		// 删除
			if (nLength > 0 && wherex() > 0) {
				buffer[--nLength] = '\0';
				WriteDevice(GetStdHandle(STD_OUTPUT_HANDLE), &ch, 1);
			}
			break;
			
		default:			// 正常输入
			if (nLength >= maxlen) {
				continue;
			}
			if (isprint(ch)) {
				buffer[nLength++] = ch;
				
				switch (type) {
				case INPUT_STRING_PASSWORD:
					ch = '*';
					break;
				default:
					;
				}
				WriteDevice(GetStdHandle(STD_OUTPUT_HANDLE), &ch, 1);
			}
			break;
		}
	}
}
