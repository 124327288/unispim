#ifndef	_MAP_FILE_H_
#define	_MAP_FILE_H_

#ifdef	__cplusplus
extern "C" {
#endif

#include <windows.h>

typedef struct tagFILEMAPDATA
{
	HANDLE		h_file;			//�ļ����
	HANDLE		h_map;			//ӳ����
	long long   length;			//�ļ��ĳ���
	long long	offset;			//�ļ���ǰƫ��
	int			granularity;	//ӳ�������
	char		*view;			//��ǰ����ͼ
}FILEMAPDATA, *FILEMAPHANDLE;

FILEMAPHANDLE FileMapOpen(const TCHAR *file_name);
int FileMapGetBuffer(FILEMAPHANDLE handle, char **buffer, int length);
int FileMapSetOffset(FILEMAPHANDLE handle, long long offset);
int FileMapClose(FILEMAPHANDLE handle);

#ifdef	__cplusplus
}
#endif

#endif