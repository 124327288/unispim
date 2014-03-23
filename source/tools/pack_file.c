/**	����ļ���
 *	���ǳ�����ļ�����������ļ��С�
 *	������������ļ��ķ�ʽ��������ͬ�����ļ��ܼ�������ĳһ���ļ��С�
 *	��������Ϊ�˷������������϶�׼���ġ�
 */

#define		_CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <windows.h>
#include "fi.h"

#define	MAX_FILES		100

#define	ERR_NO_MEMORY			-1
#define	ERR_ITEM_FULL			-2
#define	ERR_FILE_OPEN			-3
#define	ERR_FILE_READ			-4
#define	ERR_FILE_WRITE			-5

int last_err = 0;

const char *usage =
	"pack_file files_dir file_prefix paced_file_number\n"
	"  files_dir: �����ļ�Ŀ¼������Ŀ¼��ȫ���ļ��������\n"
	"  file_prefix: ����ļ�����ǰ׺, ��:yl\n"
	"  packed_file_number: ȫ������ļ�����Ŀ1-100���磺10\n"
	"\n"
	"  ����ļ�����txtΪ��β\n";

static char buffer[0x100000];			//1M�Ļ�����
int append_file(const char *tag_file, const char *src_file)
{
	FILE *fr, *fa;
	int  r_length, w_length;
	static int count;

	printf("src:%s, tag:%s\n", src_file, tag_file);
	count++;
	if (!(count & 0xff))
		printf("%d\r", count);

	fr = fopen(src_file, "rb");
	if (!fr)
	{
		last_err = ERR_FILE_OPEN;
		return 0;
	}

	fa = fopen(tag_file, "ab");
	if (!fa)
	{
		fclose(fr);
		last_err = ERR_FILE_OPEN;
		return 0;
	}

	do
	{
		r_length = fread(buffer, 1, sizeof(buffer), fr);
		if (r_length < 0)
		{
			fclose(fr);
			fclose(fa);
			last_err = ERR_FILE_READ;
			return 0;
		}
		if (!r_length)
			break;

		w_length = fwrite(buffer, 1, r_length, fa);
		if (w_length < 0 ||w_length != r_length)
		{
			fclose(fr);
			fclose(fa);
			last_err = ERR_FILE_WRITE;
			return 0;
		}
	}while(1);

	fclose(fr);
	fclose(fa);
	return 1;
}

int process(const char *files_dir, const char *prefix, int file_number)
{
	struct fi_data_t f_data;
	int  f_handle;
	char cur_file[0x100];
	char cwd[0x100];
	char tag_names[MAX_FILES][0x100];
	int  no;

	_getcwd(cwd, sizeof(cwd));
	printf("cwd=%s\n", cwd);
	for (no = 0; no < MAX_FILES; no++)
	{
		sprintf(tag_names[no], "%s\\%s-%03d.txt", cwd, prefix, no);
		_unlink(tag_names[no]);
	}

	f_handle = fi_first(files_dir, &f_data);
	printf("0\n");
	if (!f_handle)
	{
		last_err = ERR_FILE_OPEN;
		return 0;
	}

	printf("0\n");
	do
	{
		sprintf(cur_file, "%s\\%s", f_data.dir, f_data.name);
		no = rand() % file_number;
//		printf("no:%d, src:%s, tag:%s\n", no, cur_file, tag_names[no]);
		if (!append_file(tag_names[no], cur_file))
		{
			fi_close(f_handle);
			return 0;
		}
	}while(-1 != fi_next(f_handle, &f_data));

	fi_close(f_handle);
	return 1;
}

int main(int argc, char**argv)
{
	int  file_number;

	if (argc != 4)
	{
		printf("%s", usage);
		return -1;
	}

	file_number = atoi(argv[3]);
	if (!file_number || file_number < 0 || file_number > MAX_FILES)
	{
		printf("%s", usage);
		return -1;
	}

	if (!process(argv[1], argv[2], file_number))
	{
		printf("err = %d\n", last_err);
		return -1;
	}

	return 0;
}



