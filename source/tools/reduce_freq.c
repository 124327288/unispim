/*	���д�Ƶ�ļ��ļ��ٴ�������
 *	���ݵ�ǰ��Ƶ�Ƿ񳬹���С��Ƶ�������Ƿ�
 *	�����ô���
 *	
 *	Usgae:
 *		reduce in_file out_file min_freq 
 */
#define		_CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <string.h>

typedef	unsigned short	HZ;

#define	MAX_FREQ		0x7fffffff		//����Ƶ��
#define	MIN_FREQ		3				//��С��Ƶ��
#define	MAX_LENGTH		8				//���ʳ���

char *file_in_name;
char *file_out_name;
int	 min_freq = 2;

void output(FILE *f, const char *string, unsigned int count)
{
	fprintf(f, "%s\t%u\n", string, count);
}

void get_line(FILE *f, char *token, int *count)
{
	static int line;

	line++;
	if (!(line & 0x3ff))
		printf("%d\r", line);
	if (2 != fscanf(f, "%s %d", token, count))
		*count = -1;
}

char file_buffer0[0x100000];
char file_buffer1[0x100000];

int process()
{
	char token[0x40];
	int  count;
	FILE *fr, *fw;

	fw = fopen(file_out_name, "w");
	if (!fw)
	{
		printf("<%s>�޷�����\n", file_out_name);
		return -1;
	}

	//���û�������С���ӿ�������ٶ�
	if (setvbuf(fw, file_buffer0, _IOFBF, sizeof(file_buffer0)))
		printf("���û���������\n");

	fr = fopen(file_in_name, "r");
	if (!fr)
	{
		printf("<%s>�޷���\n", file_in_name);
		return -1;
	}

	//���û�������С���ӿ�������ٶ�
	if (setvbuf(fr, file_buffer1, _IOFBF, sizeof(file_buffer1)))
		printf("���û���������\n");

	get_line(fr, token, &count);

	while(count >= 0)
	{
		if (count >= min_freq)
			output(fw, token, count);
		get_line(fr, token, &count);
	};

	fclose(fw);
	fclose(fr);

	return 0;
}

const char *usage = 
	"reduce_freq in_file out_file min_freq\n";

int main(int argc, char **argv)
{
	if (argc != 4)
	{
		printf(usage);
		return -1;
	}

	file_in_name = argv[1];
	file_out_name = argv[2];
	min_freq = atoi(argv[3]);

	return process();
}
   
