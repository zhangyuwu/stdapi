
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <system.h>
#include <graphics.h>
#include <dialog.h>
#include <console.h>

/* ------------------------------------------------------------------------
 *  函数名:
 *      int ShowButton(int x, int y, const char *title)
 *  功能:
 *      显示按钮，描绘文字和边框
 *  参数:
 *      x               ----    起始x坐标
 *      y               ----    起始y坐标
 *      title           ----    按钮的标题
 *  返回值:
 *      按钮的宽和高组成的一个32位的数，高16位为宽，低16位为高
 *  说明:
 *      
 * -----------------------------------------------------------------------*/

int ShowButton(int x, int y, const char *title)
{
	int ret;

	ret = DisplayString(x, y, title);
	Rectangle(x, y, x + HI_16(ret), y + LO_16(ret));
	
	return ret;
}

/* ------------------------------------------------------------------------
 *  函数名:
 *      int MessageBox(const char *title, const char *message, int type)
 *  功能:
 *      显示对话框
 *  参数:
 *      title           ----    对话框标题
 *      message         ----    消息内容
 *      type            ----    消息类型
 *                              MB_CANCEL	:	取消
 *                              MB_OK		:	确认
 *                              MB_RETRY	:	重试
 *                              MB_NO		:	 否 
 *                              MB_YES		:	 是 
 *  返回值:
 *      按键的标识
 *  说明:
 *      type 可用组合使用，但是都需要同一个按键的只能有一个
 * -----------------------------------------------------------------------*/

int MessageBox(const char *title, const char *message, int type)
{
	const int board = 1, button_width = 32, button_interval = 16;
	int nButton = 0, i, x, y;
	bool flag_enter = false, flag_esc = false;
	int ret = 0;
	
	for (i = 0; i < 5; i++) {
		if (bit_tst(type, i)) nButton++;
	}

	// button too many
	if (nButton > 2) {
		return 0;
	}
	
	ClearScreen();
	
	// 绘标题和消息内容
	DisplayString(board + 2, board, title);
	DisplayString(board + 2, board + 20, message);

	// 绘各个按钮
	x = board + 2 + (128-board-(board + 2)-(button_width*nButton)-(nButton ? button_interval*(nButton-1) : 0)) / 2;
	y = 64-board-20;
	
	if ((type & MB_CANCEL) && !flag_esc) {
		ShowButton(x, y, "取消");
		x += button_width + button_interval;
		flag_esc = true;
	}
	if ((type & MB_OK) && !flag_enter) {
		ShowButton(x, y, "确认");
		x += button_width + button_interval;
		flag_enter = true;
	}
	if ((type & MB_RETRY) && !flag_enter) {
		ShowButton(x, y, "重试");
		x += button_width + button_interval;
		flag_enter = true;
	}
	if ((type & MB_NO) && !flag_esc) {
		ShowButton(x, y, " 否 ");
		x += button_width + button_interval;
		flag_esc = true;
	}
	if ((type & MB_YES) && !flag_enter) {
		ShowButton(x, y, " 是 ");
		x += button_width + button_interval;
		flag_enter = true;
	}
	
	// 绘边框
	Rectangle(board, board, 128-board, 64-board);
	Line(board, board + 16, 128-board, board + 16);

	// 等待用户输入
	while (!ret) {
		switch (GetKey()) {
		case VK_RETURN:
			if (type & MB_OK) {
				ret = MB_OK;
				break;
			}
			if (type & MB_RETRY) {
				ret = MB_RETRY;
				break;
			}
			if (type & MB_YES) {
				ret = MB_YES;
				break;
			}
			break;
		case VK_ESCAPE:
			if (type & MB_CANCEL) {
				ret = MB_CANCEL;
				break;
			}
			if (type & MB_NO) {
				ret = MB_NO;
				break;
			}
			break;
		}
	}
	
	// 清屏并返回输入结果
	ClearScreen();
	return ret;
}

/* ------------------------------------------------------------------------
 *  函数名:
 *      void ShowMessage(const char *title, const char *message, ulong waitTime)
 *  功能:
 *      显示对话框
 *  参数:
 *      title           ----    对话框标题
 *      message         ----    消息内容
 *      waitTime        ----    等待时间
 *  返回值:
 *      无
 *  说明:
 *      
 * -----------------------------------------------------------------------*/
void ShowMessage(const char *title, const char *message, ulong waitTime)
{
	const int xboard = 1, yboard = 9;
	
	ClearScreen();
	
	// 绘标题和消息内容
	DisplayString(xboard + 2, yboard, title);
	DisplayString(xboard + 2, yboard + 20, message);

	// 绘边框
	Rectangle(xboard, yboard, 128-xboard, 64-yboard);
	Line(xboard, yboard + 16, 128-xboard, yboard + 16);	
	
	sleep(waitTime);
}

/* ------------------------------------------------------------------------
 *  函数名：
 *      int InputBox(const char *title, char *buffer, int maxlen, int type)
 *  功能：
 *      从终端键盘读取字符串
 *  参数：
 *      title           ----    输入框标题
 *      buffer          ----    接收的缓冲区
 *      maxlen          ----    最大接收长度（字节）
 *      type            ----    输入框类型
 *                              MB_CANCEL	:	取消
 *                              MB_OK		:	确认
 *                              MB_PASSWORD	:	密码输入
 *                              MB_SHOWBUF  :   显示缓冲区中的内容
 *  返回值：
 *      按键的标识
 *  说明：
 *      
 * -----------------------------------------------------------------------*/
int InputBox(const char *title, char *buffer, int maxlen, int type)
{
	const int board = 1, button_width = 32, button_interval = 16;
	int nButton = 0, i, x, y;
	bool flag_enter = false, flag_esc = false;
	int ret = 0, ch, nLength = 0, len;
	TFont *font = GetCurrentFont(FONT_TYPE_ANSI);
	
	for (i = 0; i < 5; i++) {
		if (bit_tst(type, i)) nButton++;
	}

	// button too many
	if (nButton > 3) {
		return 0;
	}
	
	ClearScreen();
	
	// 绘各个按钮
	x = board + 2 + (128-board-(board + 2)-(button_width*nButton)-(nButton ? button_interval*(nButton-1) : 0)) / 2;
	y = 64-board-20;
	
	if ((type & MB_CANCEL) && !flag_esc) {
		ShowButton(x, y, "取消");
		x += button_width + button_interval;
		flag_esc = true;
	}
	if ((type & MB_OK) && !flag_enter) {
		ShowButton(x, y, "确认");
		x += button_width + button_interval;
		flag_enter = true;
	}
	
	// 绘标题和输入框
	DisplayString(board + 2, board, title);
	
	x = board + 2;
	y = board + 20;
	Rectangle(x, y, 128-x, y + 17);

	// 绘边框
	Rectangle(board, board, 128-board, 64-board);
	Line(board, board + 16, 128-board, board + 16);

	// 显示原始 buffer 内容
	len = strlen(buffer);
	if ((type & MB_SHOWBUF) && len > 0 && len <= maxlen) {
		DisplayString(x + 2, y + 1, buffer);
		nLength = len;
	}
	
	// 等待用户输入
	while (!ret) {
		switch (ch = GetKey()) {
		case VK_RETURN:
			if (type & MB_OK) {
				ret = MB_OK;
				break;
			}
			break;
		case VK_ESCAPE:
			if (type & MB_CANCEL) {
				ret = MB_CANCEL;
				break;
			}
			break;
			
		case VK_BACK:
			if (nLength > 0) {
				buffer[--nLength] = '\0';
				DisplayChar(x + 2 +  nLength * font->Width, y + 1, ' ');
			}
			break;
			
		default:
			if (isprint(ch)) {
				// 最大长度限制
				if (nLength >= maxlen) {
					continue;
				}
				
				// 防止破坏输入框界面
				if (x + 2 +  nLength * font->Width >= 128-x) {
					continue;
				}
				
				buffer[nLength++] = ch;
				DisplayChar(x + 2 +  (nLength - 1) * font->Width, y + 1, (type & MB_PASSWORD) ? '*' : ch);
			}
			break;
		}
	}
	
	buffer[nLength] = '\0';
	ClearScreen();
	
	return ret;
}

/* ------------------------------------------------------------------------
 *  函数名：
 *      int InputDate(const char *title, TDate *date, int type)
 *  功能：
 *      从终端键盘读取字符串
 *  参数：
 *      title           ----    输入框标题
 *      date            ----    输入的时间存放处
 *      type            ----    输入框类型
 *                              MB_CANCEL	:	取消
 *                              MB_OK		:	确认
 *  返回值：
 *      按键的标识
 *  说明：
 *      
 * -----------------------------------------------------------------------*/
int InputDate(const char *title, TDate *date, int type)
{
	enum {
		STATE_INPUT_YEAR,
		STATE_INPUT_MONTH,
		STATE_INPUT_DAY,
	};
	
	char buffer[32], state = STATE_INPUT_YEAR;
	int maxlen = 0;
	const int board = 1, button_width = 32, button_interval = 16;
	int nButton = 0, i, x, y, x1, x2, x3;
	bool flag_enter = false, flag_esc = false;
	int ret = 0, ch, nLength = 0;
	TFont *font = GetCurrentFont(FONT_TYPE_ANSI);
	
	for (i = 0; i < 5; i++) {
		if (bit_tst(type, i)) nButton++;
	}

	// button too many
	if (nButton > 3) {
		return 0;
	}
	
	ClearScreen();
	
	// 绘各个按钮
	x = board + 2 + (128-board-(board + 2)-(button_width*nButton)-(nButton ? button_interval*(nButton-1) : 0)) / 2;
	y = 64-board-20;
	
	if ((type & MB_CANCEL) && !flag_esc) {
		ShowButton(x, y, "取消");
		x += button_width + button_interval;
		flag_esc = true;
	}
	if ((type & MB_OK) && !flag_enter) {
		ShowButton(x, y, "确认");
		x += button_width + button_interval;
		flag_enter = true;
	}
	
	// 绘标题和输入框
	DisplayString(board + 2, board, title);
	
	x1 = board + 2;
	x2 = x1 + 4 * 8 + 2 + 18;
	x3 = x2 + 2 * 8 + 2 + 18;
	y = board + 20;
	
	DisplayString(x2 - 16 - 1, y, "年");
	DisplayString(x3 - 16 - 1, y, "月");
	DisplayString(x3 + 2 * 8 + 2 + 1, y, "日");
	
	x = x1;
	Rectangle(x, y, x + 4 * 8 + 2, y + 17);
	x = x2;
	Rectangle(x, y, x + 2 * 8 + 2, y + 17);
	x = x3;
	Rectangle(x, y, x + 2 * 8 + 2, y + 17);

	// 绘边框
	Rectangle(board, board, 128-board, 64-board);
	Line(board, board + 16, 128-board, board + 16);

	memset(buffer, 0, sizeof(buffer));
	// 等待用户输入
	while (!ret) {
		switch (state) {
		case STATE_INPUT_YEAR:
			maxlen = 4;
			x = x1;
			break;
		case STATE_INPUT_MONTH:
			maxlen = 2;
			x = x2;
			break;
		case STATE_INPUT_DAY:
			maxlen = 2;
			x = x3;
			break;
		}

		switch (ch = GetKey()) {
		case VK_RETURN:
			if (type & MB_OK) {
				if (state == STATE_INPUT_DAY && nLength == maxlen) {
					date->day = atoi(buffer);
					// 校验合法性
					if (date->month <= 12 && date->day <= 31) {
						ret = MB_OK;
						break;
					}
				}
			}
			break;
		case VK_ESCAPE:
			if (type & MB_CANCEL) {
				ret = MB_CANCEL;
				break;
			}
			break;
			
		case VK_BACK:
			if (nLength == 0) {
				if (state > STATE_INPUT_YEAR) {
					state--;
					
					switch (state) {
					case STATE_INPUT_YEAR:
						maxlen = 4;
						x = x1;
						sprintf(buffer, "%04d", date->year);
						nLength = maxlen;
						break;
					case STATE_INPUT_MONTH:
						maxlen = 2;
						x = x2;
						sprintf(buffer, "%02d", date->month); 
						nLength = maxlen;
						break;
					case STATE_INPUT_DAY:
						maxlen = 2;
						x = x3;
						sprintf(buffer, "%02d", date->day); 
						nLength = maxlen;
						break;
					}
				}
			}

			if (nLength > 0) {
				buffer[--nLength] = '\0';
				DisplayChar(x + 2 +  nLength * font->Width, y + 1, ' ');
			}
			break;
			
		default:
			if (isdigit(ch)) {				
				// 最大长度限制
				if (nLength >= maxlen) {
					continue;
				}
				
				buffer[nLength++] = ch;
				DisplayChar(x + 2 +  (nLength - 1) * font->Width, y + 1, ch);
				
				if (nLength >= maxlen) {
					if (state < STATE_INPUT_DAY) {
						switch (state) {
						case STATE_INPUT_YEAR:
							date->year = atoi(buffer);
							break;
						case STATE_INPUT_MONTH:
							date->month = atoi(buffer);
							break;
						}

						memset(buffer, 0, sizeof(buffer));
						nLength = 0;
						state++;
						continue;
					}
				}
			}
			break;
		}
	}
	
	buffer[nLength] = '\0';
	ClearScreen();
	
	return ret;
}

/* ------------------------------------------------------------------------
 *  函数名：
 *      int InputDate(const char *title, TDate *date, int type)
 *  功能：
 *      从终端键盘读取字符串
 *  参数：
 *      title           ----    输入框标题
 *      date            ----    输入的时间存放处
 *      type            ----    输入框类型
 *                              MB_CANCEL	:	取消
 *                              MB_OK		:	确认
 *  返回值：
 *      按键的标识
 *  说明：
 *      
 * -----------------------------------------------------------------------*/
int InputTime(const char *title, TTime *time, int type)
{
	enum {
		STATE_INPUT_HOUR,
		STATE_INPUT_MINUTE,
		STATE_INPUT_SECOND,
	};
	
	char buffer[32], state = STATE_INPUT_HOUR;
	int maxlen = 0;
	const int board = 1, button_width = 32, button_interval = 16;
	int nButton = 0, i, x, y, x1, x2, x3;
	bool flag_enter = false, flag_esc = false;
	int ret = 0, ch, nLength = 0;
	TFont *font = GetCurrentFont(FONT_TYPE_ANSI);
	
	for (i = 0; i < 5; i++) {
		if (bit_tst(type, i)) nButton++;
	}

	// button too many
	if (nButton > 3) {
		return 0;
	}
	
	ClearScreen();
	
	// 绘各个按钮
	x = board + 2 + (128-board-(board + 2)-(button_width*nButton)-(nButton ? button_interval*(nButton-1) : 0)) / 2;
	y = 64-board-20;
	
	if ((type & MB_CANCEL) && !flag_esc) {
		ShowButton(x, y, "取消");
		x += button_width + button_interval;
		flag_esc = true;
	}
	if ((type & MB_OK) && !flag_enter) {
		ShowButton(x, y, "确认");
		x += button_width + button_interval;
		flag_enter = true;
	}
	
	// 绘标题和输入框
	DisplayString(board + 2, board, title);
	
	x1 = board + 2 + 8;
	x2 = x1 + 2 * 8 + 2 + 18;
	x3 = x2 + 2 * 8 + 2 + 18;
	y = board + 20;
	
	DisplayString(x2 - 16 - 1, y, "时");
	DisplayString(x3 - 16 - 1, y, "分");
	DisplayString(x3 + 2 * 8 + 2 + 1, y, "秒");
	
	x = x1;
	Rectangle(x, y, x + 2 * 8 + 2, y + 17);
	x = x2;
	Rectangle(x, y, x + 2 * 8 + 2, y + 17);
	x = x3;
	Rectangle(x, y, x + 2 * 8 + 2, y + 17);

	// 绘边框
	Rectangle(board, board, 128-board, 64-board);
	Line(board, board + 16, 128-board, board + 16);

	memset(buffer, 0, sizeof(buffer));
	// 等待用户输入
	while (!ret) {
		switch (state) {
		case STATE_INPUT_HOUR:
			maxlen = 2;
			x = x1;
			break;
		case STATE_INPUT_MINUTE:
			maxlen = 2;
			x = x2;
			break;
		case STATE_INPUT_SECOND:
			maxlen = 2;
			x = x3;
			break;
		}

		switch (ch = GetKey()) {
		case VK_RETURN:
			if (type & MB_OK) {
				if (state == STATE_INPUT_SECOND && nLength == maxlen) {
					time->second = atoi(buffer);
					// 校验合法性
					if (time->hour <= 23 && time->minute <= 59 && time->second <= 59) {
						ret = MB_OK;
						break;
					}
				}
			}
			break;
		case VK_ESCAPE:
			if (type & MB_CANCEL) {
				ret = MB_CANCEL;
				break;
			}
			break;
			
		case VK_BACK:
			if (nLength == 0) {
				if (state > STATE_INPUT_HOUR) {
					state--;
					
					switch (state) {
					case STATE_INPUT_HOUR:
						maxlen = 2;
						x = x1;
						sprintf(buffer, "%02d", time->hour); 
						nLength = maxlen;
						break;
					case STATE_INPUT_MINUTE:
						maxlen = 2;
						x = x2;
						sprintf(buffer, "%02d", time->minute); 
						nLength = maxlen;
						break;
					case STATE_INPUT_SECOND:
						maxlen = 2;
						x = x3;
						sprintf(buffer, "%02d", time->second); 
						nLength = maxlen;
						break;
					}
				}
			}

			if (nLength > 0) {
				buffer[--nLength] = '\0';
				DisplayChar(x + 2 +  nLength * font->Width, y + 1, ' ');
			}
			break;
			
		default:
			if (isdigit(ch)) {				
				// 最大长度限制
				if (nLength >= maxlen) {
					continue;
				}
				
				buffer[nLength++] = ch;
				DisplayChar(x + 2 +  (nLength - 1) * font->Width, y + 1, ch);
				
				if (nLength >= maxlen) {
					if (state < STATE_INPUT_SECOND) {
						switch (state) {
						case STATE_INPUT_HOUR:
							time->hour = atoi(buffer);
							break;
						case STATE_INPUT_MINUTE:
							time->minute = atoi(buffer);
							break;
						}

						memset(buffer, 0, sizeof(buffer));
						nLength = 0;
						state++;
						continue;
					}
				}
			}
			break;
		}
	}
	
	buffer[nLength] = '\0';
	ClearScreen();
	
	return ret;
}

void ShowDateTimeBox()
{
	const int board = 1;
	static TDate date;
	static TTime time;
	static char buffer[32];
	static int x, y;

	ClearScreen();
	Rectangle(board, board, 128-board, 64-board);
	
	while (true) {
		x = board + (128 - 2 * board - 10 * 8) / 2;
		y = 16;
		DisplayString(x, y, DateToString(buffer, GetCurrentDate(&date), LAN_ENGLISH));
		x = board + (128 - 2 * board - 8 * 8) / 2;
		y = 32;
		DisplayString(x, y, TimeToString(buffer, GetCurrentTime(&time), LAN_ENGLISH));
		
		if (InKey()) {
			ClearKeyBuffer();
			break;
		}
		else {
			sleep(100);
		}
	}
	ClearScreen();
}
