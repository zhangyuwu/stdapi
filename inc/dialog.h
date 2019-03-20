
/*-----------------------------------------------------------------------------
 *                          Embedded System SDK 2.00                          *
 *                               Copyright (c)                                *
 *                             2005-12-15 by Ken                              *
 *                               Beijing.China                                *
 *                            cpp.china@gmail.com                             *
 -----------------------------------------------------------------------------*/

#ifndef __Dialog_H__
#define __Dialog_H__

// ��Ϣ���ѡ��ť
#define MB_OK						0x01			// ȷ��
#define MB_CANCEL					0x02			// ȡ��
#define MB_RETRY					0x04			// ����
#define MB_YES						0x08			// ��
#define MB_NO						0x10			// ��
#define MB_PASSWORD					0x20			// ��������
#define MB_SHOWBUF					0x40			// ��ʾԭʼ buffer ����

void ShowMessage(const char *title, const char *message, ulong waitTime);
int MessageBox(const char *title, const char *message, int type);
int InputBox(const char *title, char *buffer, int maxlen, int type);
int InputDate(const char *title, TDate *date, int type);
int InputTime(const char *title, TTime *time, int type);
void ShowDateTimeBox();

#endif
