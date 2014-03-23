/*	�ⲿ�鲢�ı��ļ�����
 *
 *	�Դ����ļ���������
 *	
 *	1. ����Ҫ�󣺵��������ڴ��80%����400�����ı�
 *	2. �ļ��������1024�飬����ļ���1024*400��=40�����ı�
 *		�ڴ�1G����Ϊ1024*1G*80%= 800G��С
 *	3. �ϲ������ö��������
 *	4. �Ե�ǰĿ¼Ϊ��ʱ�ļ����Ŀ¼
 *	5. �ӱ�׼�����ȡ���ݣ����׼���д������
 */

/*	usage:
 *	sort -v +nn !nn
 *	
 *	-n		�����ַ�ʽ��������
 *	-v		����
 *	+nn		�ӵ�n�ֽڿ�ʼ�Ƚ�
 *	����г�Ϊ2048�ֽ�, ֻ��512M�ڴ���д����������������ʹ��
 *	�����ڴ��80%���д���
 */
#define			_CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <windows.h>

#define	MAX_GROUPS			1024				//�������Ŀ
#define	MAX_LINE_LENGTH		2048				//����󳤶�
#define	MAX_MEMORY_SIZE		(512*1024*1024)		//����ڴ�ʹ����
#define	MAX_LINE_PER_GROUP	(1 << 23)			//һ�δ����������

int  compare_number;							//�Ƚ�����
int  ascending;									//������
int  offset;									//�Ƚϵ�ƫ���ֽ���

int  memory_size;								//һ�δ�����ڴ�ʹ����
char *buffer;									//�ı�����
int	 buffer_length;								//���������ݳ���

int  line_index[MAX_LINE_PER_GROUP];			//��ָ��
int  line_count;								//�м���

int  group_count;								//�����
int  group_order[MAX_GROUPS];					//���������򣨹鲢��
char group_line[MAX_GROUPS][MAX_LINE_LENGTH];	//�鲢ʱ����ǰ�������
int  group_end[MAX_GROUPS];						//���Ƿ����
int  cur_group_count;							//��ǰ�����������ʱ��̬������Ŀ��
FILE *group_files[MAX_GROUPS];					//���ļ����

/*	��������ʱ�ļ�����
 */
char *get_group_files_name(int group_no)
{
	static char group_files_name[0x100];

	sprintf(group_files_name, "%04d-tmp.txt", group_no);
	return group_files_name;
}

/*	����������������
 */
void end()
{
	int i;

	free(buffer);
	for (i = 0; i < group_count; i++)
	{
		if (group_files[i])
			fclose(group_files[i]);
		_unlink(get_group_files_name(i));
	}
}

/*	�����ڴ��ʼ����
 */
void memory_init()
{
	MEMORYSTATUS memory_status;

	GlobalMemoryStatus(&memory_status);
	//Ϊ��������Խ�磬��Ҫ�������������˷�
	if (memory_status.dwTotalPhys / 100 * 80 > MAX_MEMORY_SIZE)
		memory_size = MAX_MEMORY_SIZE;
	else
		memory_size = (int) (memory_status.dwTotalPhys / 100 * 80);

	buffer = (char*)malloc(memory_size);
	if (!buffer)
	{
		fprintf(stderr, "�ڴ治�����");
		end();
		exit(-1);
	}

	memset(group_files, 0, sizeof(group_files));
}

/*	���ļ��ж�ȡһ�����ݡ�
 *	���û�����ݣ�����0�����򷵻�1
 */
int read_line(char *line, int length, FILE *fr)
{
	int str_len;

	fgets(line, length, fr);
	str_len = (int)strlen(line);
	if (line[str_len - 1] == '\n')
	{
		line[str_len - 1] = 0;
		str_len--;
	}

	if (feof(fr))
		return 0;

	return str_len;
}

/*	��buffer�ж�ȡ���ݣ�ֱ��buffer�����������������ֵ��
 */
void read_lines()
{
	int len;

	buffer_length = 0;
	line_count = 0;

	do
	{
		len = read_line(buffer + buffer_length, MAX_LINE_LENGTH, stdin);
		if (feof(stdin))
			return; 				//�ļ�ȫ����ȡ���

		line_index[line_count++] = buffer_length;
		buffer_length += len + 1;
		if (memory_size - buffer_length < MAX_LINE_LENGTH)
			break;				//����һ������
		if (line_count >= MAX_LINE_PER_GROUP)
			break;
	}while(1);
}

int compare_line(const int *index1, const int *index2)
{
	int i;
	char *p1, *p2;

	p1 = &buffer[*index1];
	p2 = &buffer[*index2];

	//����offset
	for (i = 0; i < offset; i++, p1++, p2++)
	{
		if (!*p1)
		{
			if (!*p2)				//�������ȶ�����
				return 0;
			return -1;
		}

		if (!*p2)
			return 1;
	}

	for (; *p1 && *p2; p1++, p2++)
	{
		if (*p1 != *p2)
			return *p1 - *p2;
	}

	if (!*p1)
	{
		if (!*p2)
			return 0;
		return -1;
	}

	return 1;
}

/* ���ı���������
 */
void sort_buffer()
{
	if (line_count < 2)
		return;

	qsort(line_index, line_count, sizeof(line_index[0]), compare_line);
}

void output_group(int group_no)
{
	FILE *fw;
	int  i;

	fw = fopen(get_group_files_name(group_no), "wt");
	if (!fw)
	{
		fprintf(stderr, "��ʱ�ļ��޷�����\n");
		end();
		exit(-1);
	}

	for (i = 0; i < line_count; i++)
	{
		fputs(buffer + line_index[ascending ? i : line_count - 1 - i], fw);
		fputs("\n", fw);
	}

	fclose(fw);
}

/*	����ķ�ʽ���д���
 */
void process_group()
{
	group_count = 0;

	while(!feof(stdin) && group_count < MAX_GROUPS)
	{
		read_lines();

		sort_buffer();			//����
		output_group(group_count);
		group_count++;
	};
}

/*	����鲢����
 */
void process_combine()
{
	int i, j;
	int min_idx;
	int out_group;

	for (i = 0; i < group_count; i++)
	{
		group_files[i] = fopen(get_group_files_name(i), "rt");
		if (!group_files[i])
		{
			fprintf(stderr, "��ʱ�ļ���ʧ�ܡ�%s\n", get_group_files_name(i));
			exit(-1);
		}
		group_end[i] = 0;
	}

	cur_group_count = 0;
	for (i = 0; i < group_count; i++)
	{
		if (group_end[i])
			continue;
		read_line(group_line[i], MAX_LINE_LENGTH, group_files[i]);
		group_order[cur_group_count++] = i;
	}
	
	//��������
	for (i = 0; i < cur_group_count; i++)
	{
		min_idx = i;
		for (j = i + 1; j < cur_group_count; j++)
			if (strcmp(group_line[group_order[min_idx]], group_line[group_order[j]]) > 0)
				min_idx = j;
		group_order[min_idx] = i;
		group_order[i] = min_idx;
	}

	do
	{
		printf("%s\n", group_line[group_order[0]]);
		out_group = group_order[0];
		read_line(group_line[out_group], MAX_LINE_LENGTH, group_files[out_group]);
		if (feof(group_files[out_group]))
		{
			group_end[out_group] = 1;
			cur_group_count--;
			for (i = 0; i < cur_group_count; i++)				//��������ǰ�ƶ�
				group_order[i] = group_order[i + 1];
			continue;
		}

		for (i = 1; i < cur_group_count; i++)
		{
			if (strcmp(group_line[out_group], group_line[group_order[i]]) <= 0)		//�µ�С
				break;
			group_order[i - 1] = group_order[i];
		}

		group_order[i - 1] = out_group;
	}while(cur_group_count > 0);

	for (i = 0; i < group_count; i++)
		if (group_files[i])
			fclose(group_files[i]);
}

void usage()
{
	static char *usage_msg =
		"usage:\n"
		"sort -n -v +nn\n"
		"\n"
		"-n		�����ַ�ʽ��������\n"
		"-v		����\n"
		"+nn		�ӵ�n�ֽڿ�ʼ�Ƚ�\n"
		"����г�Ϊ2048�ֽ�, ֻ��512M�ڴ���д����������������ʹ��\n"
		"�����ڴ��80%���д���\n";

	fprintf(stderr, "%s", usage_msg);
}


int main(int argc, char **argv)
{
	int i;
	char *str;

	compare_number = 0;
	ascending = 1;
	offset = 0;

	for (i = 1; i < argc; i++)
	{
		str = argv[i];
		if (!strcmp(str, "-n"))
		{
			compare_number = 1;
			continue;
		}

		if (!strcmp(str, "-v"))
		{
			ascending = 0;
			continue;
		}

		if (str[0] == '+')		//offset
			if (1 == sscanf(str, "%d", &offset))
				continue;

		usage();
		exit(-1);
	}

	memory_init();
	process_group();
	process_combine();
	end();

	return 0;
}

