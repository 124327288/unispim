/**	�ִʳ���Dll�汾
 */
#include <iostream>
#include <fstream>

using namespace std;

#include <stdio.h>
#include <time.h>
#include <string.h>

#include "mmseg.h"
#include <windows.h>

#define	AMBI_FILE_NAME		"ambi.txt"

extern "C" __declspec(dllexport) int Segment(const char *dict_name, const char *str, char *result);

/**	�ִʹ���
 *	������
 *		dict_name			�ִʴʵ����ƣ�����Ѿ����ù��������Ϊ0
 *		str					�ȴ��ִʵ��ַ�������������ʱ����0���ã����ͷŷ�����ڴ�
 *		result				�ִʽ��
 *	���أ�
 *		1		�ɹ�
 *		0		ʧ��
 */
int Segment(const char *dict_name, const char *str, char *result)
{
	static mmseg *seg = 0;

	if (result)
		*result = 0;

	if (!str)
	{
		if (seg)
			delete seg;
		seg = 0;
	}

	if (dict_name)
	{
		if (seg)
			delete seg;
		seg = new mmseg(dict_name, AMBI_FILE_NAME);
	}

	if (!seg || !str || !result)
		return 0;

	int index = 0;
	int str_len = (int)strlen(str);
	int len = seg->segment(str, str_len);
	while(len)
	{
		strncat_s(result, 1024, str + index, len);
		strcat_s(result, 1024, " ");
		index += len;
		len = seg->segment(str + index, str_len - index);
	}
	
	return 1;
}