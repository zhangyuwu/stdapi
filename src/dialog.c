
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
 *  ������:
 *      int ShowButton(int x, int y, const char *title)
 *  ����:
 *      ��ʾ��ť��������ֺͱ߿�
 *  ����:
 *      x               ----    ��ʼx����
 *      y               ----    ��ʼy����
 *      title           ----    ��ť�ı���
 *  ����ֵ:
 *      ��ť�Ŀ�͸���ɵ�һ��32λ��������16λΪ����16λΪ��
 *  ˵��:
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
 *  ������:
 *      int MessageBox(const char *title, const char *message, int type)
 *  ����:
 *      ��ʾ�Ի���
 *  ����:
 *      title           ----    �Ի������
 *      message         ----    ��Ϣ����
 *      type            ----    ��Ϣ����
 *                              MB_CANCEL	:	ȡ��
 *                              MB_OK		:	ȷ��
 *                              MB_RETRY	:	����
 *                              MB_NO		:	 �� 
 *                              MB_YES		:	 �� 
 *  ����ֵ:
 *      �����ı�ʶ
 *  ˵��:
 *      type �������ʹ�ã����Ƕ���Ҫͬһ��������ֻ����һ��
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
	
	// ��������Ϣ����
	DisplayString(board + 2, board, title);
	DisplayString(board + 2, board + 20, message);

	// �������ť
	x = board + 2 + (128-board-(board + 2)-(button_width*nButton)-(nButton ? button_interval*(nButton-1) : 0)) / 2;
	y = 64-board-20;
	
	if ((type & MB_CANCEL) && !flag_esc) {
		ShowButton(x, y, "ȡ��");
		x += button_width + button_interval;
		flag_esc = true;
	}
	if ((type & MB_OK) && !flag_enter) {
		ShowButton(x, y, "ȷ��");
		x += button_width + button_interval;
		flag_enter = true;
	}
	if ((type & MB_RETRY) && !flag_enter) {
		ShowButton(x, y, "����");
		x += button_width + button_interval;
		flag_enter = true;
	}
	if ((type & MB_NO) && !flag_esc) {
		ShowButton(x, y, " �� ");
		x += button_width + button_interval;
		flag_esc = true;
	}
	if ((type & MB_YES) && !flag_enter) {
		ShowButton(x, y, " �� ");
		x += button_width + button_interval;
		flag_enter = true;
	}
	
	// ��߿�
	Rectangle(board, board, 128-board, 64-board);
	Line(board, board + 16, 128-board, board + 16);

	// �ȴ��û�����
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
	
	// ����������������
	ClearScreen();
	return ret;
}

/* ------------------------------------------------------------------------
 *  ������:
 *      void ShowMessage(const char *title, const char *message, ulong waitTime)
 *  ����:
 *      ��ʾ�Ի���
 *  ����:
 *      title           ----    �Ի������
 *      message         ----    ��Ϣ����
 *      waitTime        ----    �ȴ�ʱ��
 *  ����ֵ:
 *      ��
 *  ˵��:
 *      
 * -----------------------------------------------------------------------*/
void ShowMessage(const char *title, const char *message, ulong waitTime)
{
	const int xboard = 1, yboard = 9;
	
	ClearScreen();
	
	// ��������Ϣ����
	DisplayString(xboard + 2, yboard, title);
	DisplayString(xboard + 2, yboard + 20, message);

	// ��߿�
	Rectangle(xboard, yboard, 128-xboard, 64-yboard);
	Line(xboard, yboard + 16, 128-xboard, yboard + 16);	
	
	sleep(waitTime);
}

/* ------------------------------------------------------------------------
 *  ��������
 *      int InputBox(const char *title, char *buffer, int maxlen, int type)
 *  ���ܣ�
 *      ���ն˼��̶�ȡ�ַ���
 *  ������
 *      title           ----    ��������
 *      buffer          ----    ���յĻ�����
 *      maxlen          ----    �����ճ��ȣ��ֽڣ�
 *      type            ----    ���������
 *                              MB_CANCEL	:	ȡ��
 *                              MB_OK		:	ȷ��
 *                              MB_PASSWORD	:	��������
 *                              MB_SHOWBUF  :   ��ʾ�������е�����
 *  ����ֵ��
 *      �����ı�ʶ
 *  ˵����
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
	
	// �������ť
	x = board + 2 + (128-board-(board + 2)-(button_width*nButton)-(nButton ? button_interval*(nButton-1) : 0)) / 2;
	y = 64-board-20;
	
	if ((type & MB_CANCEL) && !flag_esc) {
		ShowButton(x, y, "ȡ��");
		x += button_width + button_interval;
		flag_esc = true;
	}
	if ((type & MB_OK) && !flag_enter) {
		ShowButton(x, y, "ȷ��");
		x += button_width + button_interval;
		flag_enter = true;
	}
	
	// �����������
	DisplayString(board + 2, board, title);
	
	x = board + 2;
	y = board + 20;
	Rectangle(x, y, 128-x, y + 17);

	// ��߿�
	Rectangle(board, board, 128-board, 64-board);
	Line(board, board + 16, 128-board, board + 16);

	// ��ʾԭʼ buffer ����
	len = strlen(buffer);
	if ((type & MB_SHOWBUF) && len > 0 && len <= maxlen) {
		DisplayString(x + 2, y + 1, buffer);
		nLength = len;
	}
	
	// �ȴ��û�����
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
				// ��󳤶�����
				if (nLength >= maxlen) {
					continue;
				}
				
				// ��ֹ�ƻ���������
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
 *  ��������
 *      int InputDate(const char *title, TDate *date, int type)
 *  ���ܣ�
 *      ���ն˼��̶�ȡ�ַ���
 *  ������
 *      title           ----    ��������
 *      date            ----    �����ʱ���Ŵ�
 *      type            ----    ���������
 *                              MB_CANCEL	:	ȡ��
 *                              MB_OK		:	ȷ��
 *  ����ֵ��
 *      �����ı�ʶ
 *  ˵����
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
	
	// �������ť
	x = board + 2 + (128-board-(board + 2)-(button_width*nButton)-(nButton ? button_interval*(nButton-1) : 0)) / 2;
	y = 64-board-20;
	
	if ((type & MB_CANCEL) && !flag_esc) {
		ShowButton(x, y, "ȡ��");
		x += button_width + button_interval;
		flag_esc = true;
	}
	if ((type & MB_OK) && !flag_enter) {
		ShowButton(x, y, "ȷ��");
		x += button_width + button_interval;
		flag_enter = true;
	}
	
	// �����������
	DisplayString(board + 2, board, title);
	
	x1 = board + 2;
	x2 = x1 + 4 * 8 + 2 + 18;
	x3 = x2 + 2 * 8 + 2 + 18;
	y = board + 20;
	
	DisplayString(x2 - 16 - 1, y, "��");
	DisplayString(x3 - 16 - 1, y, "��");
	DisplayString(x3 + 2 * 8 + 2 + 1, y, "��");
	
	x = x1;
	Rectangle(x, y, x + 4 * 8 + 2, y + 17);
	x = x2;
	Rectangle(x, y, x + 2 * 8 + 2, y + 17);
	x = x3;
	Rectangle(x, y, x + 2 * 8 + 2, y + 17);

	// ��߿�
	Rectangle(board, board, 128-board, 64-board);
	Line(board, board + 16, 128-board, board + 16);

	memset(buffer, 0, sizeof(buffer));
	// �ȴ��û�����
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
					// У��Ϸ���
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
				// ��󳤶�����
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
 *  ��������
 *      int InputDate(const char *title, TDate *date, int type)
 *  ���ܣ�
 *      ���ն˼��̶�ȡ�ַ���
 *  ������
 *      title           ----    ��������
 *      date            ----    �����ʱ���Ŵ�
 *      type            ----    ���������
 *                              MB_CANCEL	:	ȡ��
 *                              MB_OK		:	ȷ��
 *  ����ֵ��
 *      �����ı�ʶ
 *  ˵����
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
	
	// �������ť
	x = board + 2 + (128-board-(board + 2)-(button_width*nButton)-(nButton ? button_interval*(nButton-1) : 0)) / 2;
	y = 64-board-20;
	
	if ((type & MB_CANCEL) && !flag_esc) {
		ShowButton(x, y, "ȡ��");
		x += button_width + button_interval;
		flag_esc = true;
	}
	if ((type & MB_OK) && !flag_enter) {
		ShowButton(x, y, "ȷ��");
		x += button_width + button_interval;
		flag_enter = true;
	}
	
	// �����������
	DisplayString(board + 2, board, title);
	
	x1 = board + 2 + 8;
	x2 = x1 + 2 * 8 + 2 + 18;
	x3 = x2 + 2 * 8 + 2 + 18;
	y = board + 20;
	
	DisplayString(x2 - 16 - 1, y, "ʱ");
	DisplayString(x3 - 16 - 1, y, "��");
	DisplayString(x3 + 2 * 8 + 2 + 1, y, "��");
	
	x = x1;
	Rectangle(x, y, x + 2 * 8 + 2, y + 17);
	x = x2;
	Rectangle(x, y, x + 2 * 8 + 2, y + 17);
	x = x3;
	Rectangle(x, y, x + 2 * 8 + 2, y + 17);

	// ��߿�
	Rectangle(board, board, 128-board, 64-board);
	Line(board, board + 16, 128-board, board + 16);

	memset(buffer, 0, sizeof(buffer));
	// �ȴ��û�����
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
					// У��Ϸ���
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
				// ��󳤶�����
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
