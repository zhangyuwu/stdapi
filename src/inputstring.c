
#include <ctype.h>
#include <system.h>
#include <console.h>
#include <device.h>

/* ------------------------------------------------------------------------
 *  ��������
 *      bool InputString(char *buffer, int maxlen, int *length)
 *  ���ܣ�
 *      ���ն˼��̶�ȡ�ַ���
 *  ������
 *      buffer          ----    ���յĻ�����
 *      type            ----    ��������
 *      maxlen          ----    �����ճ��ȣ��ֽڣ�
 *      length          ----    ʵ�ʽ��յĳ��ȣ��ֽڣ�
 *  ����ֵ��
 *      true            ----    �û�����ȷ�Ϸ���
 *      false           ----    �û�����ȡ������
 *  ˵����
 *      ����û�����ȡ������ȷ�϶��᷵��
 * -----------------------------------------------------------------------*/

bool InputString(char *buffer, int type, int maxlen, int *length)
{
	char ch;
	int nLength = 0;
	
	while (true) {
		ch = GetKey();
		
		switch (ch) {
		case VK_ESCAPE:		// ȡ��
			buffer[nLength] = '\0';
			*length = nLength;
			return false;
			
		case VK_RETURN:		// ȷ��
			buffer[nLength] = '\0';
			*length = nLength;
			return true;
			
		case VK_BACK:		// ɾ��
			if (nLength > 0 && wherex() > 0) {
				buffer[--nLength] = '\0';
				WriteDevice(GetStdHandle(STD_OUTPUT_HANDLE), &ch, 1);
			}
			break;
			
		default:			// ��������
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
