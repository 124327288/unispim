/*	����������Ƶ�ļ��ĺϲ�
 *	ǰ�᣺
 *		������Ƶ�ļ����붼���Ѿ�������ɵ�
 *	�鲢����
 *		
 */
#define		_CRT_SECURE_NO_DEPRECATE
#include <time.h>
#include <stdio.h>
#include <string.h>

typedef	unsigned short	HZ;

#define	MAX_FILES		0x20			//����ļ���Ŀ
#define	MAX_FREQ		0x7fffffff		//����Ƶ��
#define	MIN_FREQ		3				//��С��Ƶ��
#define	MAX_LINE_LENGTH	0x200			//����г���

const char *usage = 
    "combine file1 file2 [file3].... out_file\n"
	"\n"
	"�����ļ��ĺϲ����������ڴ�Ƶ����Ƶ����Ԫ�Լ���ԪƵ�ʵĺϲ�\n"
	"\n"
	"�����У����һ���ǽ������ļ�������Ϊ���кϲ����ļ�,\n"
	"������ͬʱ�ϲ�32���ļ���\n"
	"�ļ��У�ֻ�����һ��Ӧ��Ϊ���֣�������Ӧ��Ϊ���Ļ�Ӣ�ķ���\n"
	"\n"
	"file1/file2/file....     ����׼�����кϲ��Ĵ�Ƶ�ļ�\n"
	"out_file	              ����ļ�����\n";

char in_file_buffer[0x20][0x100000];
char out_file_buffer[0x100000];

char *out_file_name;
char *in_file_name[MAX_FILES];
int  in_file_count;
FILE *in_file[MAX_FILES];
FILE *out_file;

void output(FILE *f, const char *string, int count)
{
	fprintf(f, "%s\t%u\n", string, count);
}

/**	���ļ��ж�ȡһ�� 
 */
int get_line(FILE *f, char *token, int *count)
{
	int index = 0;
	int break_pos;
	int no;

	*token = 0;
	*count = 0;

	if (!fgets(token, MAX_LINE_LENGTH, f))
		return 0;				//�ļ���β

	index = 0;
	while(token[index] && (token[index] < '0' || token[index] > '9'))
		index++;

	break_pos = index - 1;

	no = 0;
	while(token[index] && token[index] >= '0' && token[index] <= '9')
	{
		no = no * 10 + (token[index] - '0');
		index++;
	}

	token[break_pos] = 0;
	*count = no;
	return 1;
}

/**	��ʼ�����򿪸����ļ�
 */
int open_all()
{
	int i, j;

	out_file = fopen(out_file_name, "w");
	if (!out_file)
	{
		printf( "�ļ�<%s>����ʧ��\n", out_file_name);
		return 0;
	}
	setvbuf(out_file, out_file_buffer, _IOFBF, sizeof(out_file_buffer));

	for (i = 0; i < in_file_count; i++)
	{
		in_file[i] = fopen(in_file_name[i], "r");
		if (!in_file[i])
		{
			printf("���ļ�<%s>ʧ��!\n", in_file_name[i]);
			for (j = 0; j < i; j++)
				fclose(in_file[j]);
			fclose(out_file);
			return 0;
		}
		
		setvbuf(in_file[i], in_file_buffer[i], _IOFBF, sizeof(in_file_buffer[0]));
	}

	return 1;
}

/**	�رմ򿪵��ļ�
 */
void close_all()
{
	int i;

	fclose(out_file);
	for (i =0; i < in_file_count; i++)
		fclose(in_file[i]);
}

typedef struct tagLINE
{
	char *str;
	long long count;
	FILE *from_file;
	struct tagLINE *next;
}LINE;

char line[MAX_FILES][MAX_LINE_LENGTH];

LINE line_heap[MAX_FILES];
int  line_count;

/**	���뵽���С�����������ظ��Ĵ�����ϲ�������������0
 */
int insert_to_heap(char *str, int count, FILE *from_file)
{
	int i, j, ret;

	for (i = 0; i < line_count; i++)
	{
		ret = strcmp(str, line_heap[i].str);
		if (!ret)
		{
			line_heap[i].count += count;
			return 0;
		}

		if (ret > 0)
			break;
	}

	//��ǰλ��Ϊ����λ��
	for (j = line_count; j > i; j--)
		line_heap[j] = line_heap[j - 1];

	line_heap[i].str = str;
	line_heap[i].from_file = from_file;
	line_heap[i].count = count;

	line_count++;
	return 1;
}

/**	��ʼ���ж�
 */
void init_line_heap()
{
	int  count;
	int  i;

	line_count = 0;
	
	//ȷ�����е��ļ�
	for (i = 0; i < in_file_count; i++)
	{
		while (get_line(in_file[i], line[i], &count))
		{
			if (insert_to_heap(line[i], count, in_file[i]))
				break;
		}
	}
}

/**	����
 */
int process()
{
	int count;
	int c, st;

	c = 0;
	st = clock();

	init_line_heap();

	while(line_count)
	{
		LINE *out_line = &line_heap[line_count - 1];
		line_count--;

		c++;
		if (!(c & 0xffff))
			printf("line:%dM, time:%dS\r", c / 0x100000, (clock() - st) / 1000);
		output(out_file, out_line->str, out_line->count > MAX_FREQ ? MAX_FREQ : (int)out_line->count);

		while (get_line(out_line->from_file, out_line->str, &count))
			if (insert_to_heap(out_line->str, count, out_line->from_file))
				break;
	};

	return 1;
}

int main(int argc, char **argv)
{
	int i;

	if (argc < 4)
	{
		printf(usage);
		return -1;
	}

	in_file_count = argc - 2;
	if (in_file_count > MAX_FILES)
	{
		printf("�ļ�̫�࣬���32��!\n");
		return -1;
	}

	for (i = 0; i < in_file_count; i++)
		in_file_name[i] = argv[1 + i];

	out_file_name = argv[argc - 1];

	if (!open_all())
		return -1;

	process();

	printf("\n");

	close_all();
	return 0;
}
   
