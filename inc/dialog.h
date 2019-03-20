
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/

#ifndef __Dialog_H__
#define __Dialog_H__

// 消息框可选按钮
#define MB_OK						0x01			// 确认
#define MB_CANCEL					0x02			// 取消
#define MB_RETRY					0x04			// 重试
#define MB_YES						0x08			// 是
#define MB_NO						0x10			// 否
#define MB_PASSWORD					0x20			// 密码输入
#define MB_SHOWBUF					0x40			// 显示原始 buffer 内容

void ShowMessage(const char *title, const char *message, ulong waitTime);
int MessageBox(const char *title, const char *message, int type);
int InputBox(const char *title, char *buffer, int maxlen, int type);
int InputDate(const char *title, TDate *date, int type);
int InputTime(const char *title, TTime *time, int type);
void ShowDateTimeBox();

#endif
