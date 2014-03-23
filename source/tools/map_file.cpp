#include <map_file.h>

/**	���ֻ���ļ��ľ����ע��ֻ�ʺ���ֻ���ļ���
 *	���ٷ����ļ�����FileMapping��ʽ�Դ��ļ��ķ���
 *	������Ϊ��ݡ�
 *	˳���������
 */
FILEMAPHANDLE FileMapOpen(const TCHAR *file_name)
{
	FILEMAPHANDLE handle;

	handle = (FILEMAPHANDLE) malloc(sizeof(FILEMAPDATA));
	if (!handle)
		return 0;

	//���ļ�
	handle->h_file = CreateFile(file_name, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (handle->h_file == INVALID_HANDLE_VALUE)
	{
		free(handle);
		return 0;
	}

	handle->length = 0;
	GetFileSizeEx(handle->h_file, (PLARGE_INTEGER)&handle->length);

	//����ӳ��
	handle->h_map = CreateFileMapping(handle->h_file, 0, PAGE_READONLY,	0, 0, 0);
	if (!handle->h_map)
	{
		CloseHandle(handle->h_file);
		free(handle);

		return 0;
	}

	SYSTEM_INFO sys_info;
	GetSystemInfo(&sys_info);

	handle->granularity = (int)sys_info.dwAllocationGranularity;
	handle->offset		= 0;
	handle->view		= 0;

	return handle;
}

/**	ͨ��ƫ�ƻ���ļ���ָ��
 *	�������Ϊ0����ӳ�������ļ�
 */
int FileMapGetBuffer(FILEMAPHANDLE handle, char **buffer, int length)
{
	int remains;
	long long offset;

	if (!handle || length < 0)
		return 0;

	if (handle->view)			//�������ӳ��
	{
		UnmapViewOfFile(handle->view);
		handle->view = 0;
	}

	if (!length)
		length = (int)(handle->length - handle->offset);

	offset  = handle->offset;
	remains = (int)(offset & (handle->granularity - 1));
	offset -= remains;

	if (length + handle->offset > handle->length)
		length = (int)(handle->length - handle->offset);

	//����ӳ��
	handle->view = (char*)MapViewOfFile(handle->h_map,
										FILE_MAP_READ,
										(DWORD)(offset >> 32),				//ƫ�Ƶ�ַ
										(DWORD)(offset & 0xffffffff),
										length + remains);

	if (!handle->view)
	{
		int err = GetLastError();
		return 0;
	}

	*buffer = handle->view + remains;
	if (handle->length > handle->offset + length)
	{
		handle->offset += length;
		return length;
	}

	remains = (int)(handle->length - handle->offset);
	handle->offset = handle->length;

	return remains;
}

/**	�����ļ�ָ��
 */
int FileMapSetOffset(FILEMAPHANDLE handle, long long offset)
{
	if (!handle)
		return 0;

	handle->offset = offset;
	return 1;
}

/**	�ر��ļ�ӳ��
 */
int FileMapClose(FILEMAPHANDLE handle)
{
	if (handle)
	{
		UnmapViewOfFile(handle->view);
		CloseHandle(handle->h_map);
		CloseHandle(handle->h_file);

		return 1;
	}

	return 0;
}