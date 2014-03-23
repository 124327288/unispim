/*	����ncoc���ļ�.
 *	���Ѿ�������ϵ��ļ��ж�ȡ�������Լ�ncocֵ, ����ncoc�����ļ�
 *	�����ļ����ݣ�
 *	����		100
 *	����		200000000
 *	...
 *	
 *	������̣�
 *	һ���ֽڷ���һ��BCOC��TCOC����Ȼ������
 *	TCOC������(x)��
 *		if (x < 4096)
 *			return;
 *		x /= 4096
 *		x = ln(x);
 *		x *= 32;
 *		x = int(x, 255);
 *
 *	��ԭ(x)
 *		x /= 32;
 *		x = exp(x);
 *		x *= 4096;
 *	
 *		
 *	1. ������������������Ƶ�ʽ�����С����������С��2�����ݽ�����
 *	2. ���ݲ�����ȷ������bcoc����tcoc
 *	3. ���ã����countС��256������һ���ֽ����������С��256����
 *		����һ��λ�ã�����ͬ���ĺ��֣����Ѹ�λ����
 *
 *	ʹ�ò�����
 *		make_ncoc -b|-t src_txt reduce_bit min_freq
 *	�磺make_ncoc -b f2.txt 0 4				->��������, 2.8M����
 *		make_ncoc -t f3.txt 0 4				->��������, 13.0M����
 *
 *		ʵ�ʵ�BCOC������10λ�ƶ���BCOC������2�������
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncoc.h>
#include <math.h>

#define	TEST_FILE	"test.txt"

#define	MAX_INDEX0_ITEMS	(1 << 13)
#define	MAX_INDEX1_ITEMS	(1 << 22)
#define	MAX_ITEMS			(1 << 24)

int 	index0_count = 0;							//��һ����������
int		index1_count = 0;							//�ڶ�����������
int		item_count = 0;								//�����

int		min_freq = 1;								//��С��Ƶ��Ŀ
int		multi_factor = 1;							//�����ķŴ���

INDEXITEM	index0[MAX_INDEX0_ITEMS];				//��һ��������
INDEXITEM	index1[MAX_INDEX1_ITEMS];				//�ڶ���������
NCOCITEM	item[MAX_ITEMS];						//����

/**	�Զ����Ӳ�������, ��512��֮һ�ĸ���ѡ�����
 *	����, ��Ȼ��һ�������һ������������.
 *	ע:��str == 0ʱ��ʾ����
 *	����norton��Ե�ʣ�ÿ������д���ļ������ϵͳ������
 *	��ˣ���Ҫ���رա�
 */
void add_test(const char *str, int ncoc)
{
	static FILE *f;
	static first = 1;
	static char last_str[0x20];
	static int last_ncoc;

	if (first)
	{
		_unlink(TEST_FILE);
		f = fopen(TEST_FILE, "a");
	}

	/* �Խӽ�512��֮һ�ĸ��ʽ���ѡ��������� */
	/* ��һ�������һ��������뵽������. */
	if (str && !first && 0)//rand() % 512 != 0)
	{
		strcpy(last_str, str);
		last_ncoc = ncoc;
		return;
	}

	first = 0;

	if (!f)
		return;

	if (str)
	{
		fprintf(f, "%s %d\n", str, ncoc);
		strcpy(last_str, str);
		last_ncoc = ncoc;
	}
	else		/* ���һ�� */
	{
		fprintf(f, "%s %d\n", last_str, last_ncoc);
		fclose(f);
	}
}

/**	���BCOC�ļ�
 */
int output_bcoc()
{
	char buffer[0x400];				//1Kheader
	FILE *fw;
	BCOCHEADER *header = (BCOCHEADER*)buffer;

	fw = fopen(BCOC_NAME, "wb");
	if (!fw)
	{
		fprintf(stderr, "<%s>�ļ�д�򿪴���!\n", BCOC_NAME);
		return -1;
	}

	memset(buffer, 0, sizeof(buffer));
	header->sign = BCOC_SIGN;
	header->index0_count = index0_count;
	header->item_count = item_count;
	header->index0_data_pos = 0;
	header->item_data_pos = header->index0_data_pos + index0_count * sizeof(index0[0]);

	if (sizeof(buffer) != fwrite(buffer, 1, sizeof(buffer), fw) ||
		index0_count != (int)fwrite(index0, sizeof(index0[0]), index0_count, fw) ||
		item_count != (int)fwrite(item, sizeof(item[0]), item_count, fw))
	{
		printf("�ļ��������!\n");
		fclose(fw);
		exit(-1);
	}

	fclose(fw);
	return 0;
}

/**	���TCOC�ļ�
 */
int output_tcoc()
{
	char buffer[0x400];
	TCOCHEADER	*header = (TCOCHEADER*)buffer;
	FILE *fw;

	fw = fopen(TCOC_NAME, "wb");
	if (!fw)
	{
		fprintf(stderr, "<%s>�ļ�д�򿪴���!\n", TCOC_NAME);
		return -1;
	}

	memset(buffer, 0, sizeof(buffer));

	header->sign = TCOC_SIGN;
	header->index0_count = index0_count;
	header->index1_count = index1_count;
	header->item_count = item_count;
	header->index0_data_pos = 0;
	header->index1_data_pos = header->index0_data_pos + index0_count * sizeof(index0[0]);
	header->item_data_pos = header->index1_data_pos + index1_count * sizeof(index1[0]);

	if (sizeof(buffer) != fwrite(buffer, 1, sizeof(buffer), fw) ||
		index0_count != (int)fwrite(index0, sizeof(index0[0]), index0_count, fw) ||
		index1_count != (int)fwrite(index1, sizeof(index1[0]), index1_count, fw) ||
		item_count != (int)fwrite(item, sizeof(item[0]), item_count, fw))
	{
		printf("�ļ��������!\n");
		fclose(fw);
		exit(-1);
	}

	fclose(fw);
	return 0;
}

/**	���BCOC��
 */
void add_bcoc_item(const char *str, int count)
{
	static HZ last_hz0, last_hz1;
	HZ  hz0, hz1;
	double x;
	static int c;

	c++;
	if (!(c & 0x3ff))
		printf("%d\r", c);

	if (!str)		//��������, ������һ��
	{
		index0[index0_count].hz = 0;
		index0[index0_count++].item_pos = item_count;
		item_count++;
		add_test(0, 0);
		return;
	}

	//У�麺�ִ��ĳ���	
	if (strlen(str) != 2 * sizeof(HZ))			//��Ч
		return;

	count /= min_freq;
	if (!count)			//û�б�Ҫ
		return;

	x = 1.0 * count;
	x = log(x);
	x *= multi_factor;

	count = (int)x;
	if (count > 255)
		count = 255;

	hz0 = *(HZ*)str;
	hz1 = *(HZ*)(str + sizeof(HZ));

	if (last_hz0 != hz0)				//��һ���Ͳ����
	{
		index0[index0_count].hz = hz0;
		index0[index0_count].item_pos = item_count;
		index0_count++;
	}

	item[item_count].hz= hz1;
	item[item_count].count = count & 0xff;
	item_count++;
	if (count >= (1 << 8))
	{
		item[item_count].hz = hz1;
		item[item_count].count = (count & 0xff00) >> 8;
		item_count++;
	}

	if (index0_count >= sizeof(index0) / sizeof(index1[0]) ||
		item_count >= sizeof(item) / sizeof(item[0]))
	{
		fprintf(stderr, "���������. index0:%d, item:%d\n", index0_count, item_count);
		exit(-1);
		return;
	}

	add_test(str, count);
	last_hz0 = hz0;
	last_hz1 = hz1;

	return;
}

/**	���TCOC��
 */
void add_tcoc_item(const char *str, int count)
{
	static HZ last_hz0, last_hz1, last_hz2;
	HZ hz0, hz1, hz2;
	double x;
	static int c;

	c++;
	if (!(c & 0xfff))
		printf("%d\r", c);

	if (!str)		/* ��������, ������һ�� */
	{
		index0[index0_count].hz = 0;
		index0[index0_count++].item_pos = index1_count;
		index1[index1_count].hz = 0;
		index1[index1_count++].item_pos = item_count;
		item_count++;
		add_test(0, 0);
		return;
	}

	count /= min_freq;
	if (!count)				//��С��û�б�Ҫ
		return;

	x = 1.0 * count;
	x = log(x);
	x *= multi_factor;

	count = (int)x;
	if (count > 255)		//����һ���ֽ�����
		count = 255;

	//��麺�ִ�����
	if (strlen(str) != 3 * sizeof(HZ))
		return;

	hz0 = *(HZ*)str;
	hz1 = *(HZ*)(str + sizeof(HZ));
	hz2 = *(HZ*)(str + 2 * sizeof(HZ));

	if (last_hz0 != hz0)		//��һ�������
	{
		index0[index0_count].hz = hz0;
		index0[index0_count].item_pos = index1_count;
		index0_count++;
	}

	if (last_hz0 != hz0 || last_hz1 != hz1)		//�ڶ��������
	{
		index1[index1_count].hz = hz1;
		index1[index1_count].item_pos = item_count;
		index1_count++;
	}

	//�������϶�Ҫ����
	item[item_count].hz= hz2;
	item[item_count].count = count;
	item_count++;

	if (index0_count >= sizeof(index0) / sizeof(index0[0]) ||
		index1_count >= sizeof(index1) / sizeof(index1[0]) ||
		item_count >= sizeof(item) / sizeof(item[0]))
	{
		fprintf(stderr, "���������. index0:%d, index1:%d, item:%d\n", index0_count, index1_count, item_count);
		exit(-1);
		return;
	}

	add_test(str, count);

	last_hz0 = hz0;
	last_hz1 = hz1;
	last_hz2 = hz2;
	return;
}

/**	����BCOC��
 *	����Ӳ����ļ��ж�ȡ�������ݡ�
 *	һ��һ������, �Ժ��ִ�ͷ, ncocΪ�ڶ�������.
 *	��: ���� 18
 */
void test_bcoc()
{
	char str[0x20];
	int  bcoc, value;
	FILE *fr;

	if (!open_bcoc(BCOC_NAME))
	{
		printf("BCOC�ļ�<%s>��ʧ��\n", BCOC_NAME);
		exit(-1);
	}

	fr = fopen(TEST_FILE, "rt");
	if (!fr)
	{
		printf("�����ļ�<%s>��ʧ��\n", TEST_FILE);
		exit(-1);
	}

	while(2 == fscanf(fr, "%s %d", str, &bcoc))
	{
		value = get_bcoc((HZ*)str);
		if (value != bcoc)
		{
			fprintf(stderr, "%s��BCOCֵӦΪ%d��ʵ��Ϊ%d\n", str, bcoc, value);
			exit(-1);
		}
	}
	printf("����ͨ��\n");
	fclose(fr);
	return;
}

/**	����TCOC��
 *	����Ӳ����ļ��ж�ȡ�������ݡ�
 *	һ��һ������, �Ժ��ִ�ͷ, ncocΪ�ڶ�������.
 *	��: ���� 18
 */
void test_tcoc()
{
	char str[0x20];
	int  tcoc, value;
	FILE *fr;

	if (!open_tcoc(TCOC_NAME))
	{
		printf("TCOC�ļ�<%s>��ʧ��\n", TCOC_NAME);
		exit(-1);
	}

	fr = fopen(TEST_FILE, "rt");
	if (!fr)
	{
		printf("�����ļ�<%s>��ʧ��\n", TEST_FILE);
		exit(-1);
	}

	while(2 == fscanf(fr, "%s %d", str, &tcoc))
	{
		value = get_tcoc((HZ*)str);
		if (value != tcoc)
		{
			fprintf(stderr, "%s��TCOCֵӦΪ%d��ʵ��Ϊ%d\n", str, tcoc, value);
			exit(-1);
		}
	}
	printf("����ͨ��\n");
	fclose(fr);
	return;
}

void usage(const char *p_name)
{
	fprintf(stdout, "usage:\n"
					"%s -t|-b src_file ��С��Ƶ �����Ŵ���\n"
					"-t           ����TCOC�����ļ�\n"
					"-b           ����BCOC�����ļ�\n"
					"src_file     ������Ƶ��Ϣ���ļ�����\n"
					"��С��Ƶ     ���ڱ���ֵ�Ĵʽ������룬TCOCһ��Ϊ4096��BCOCһ��Ϊ1024\n"
					"�����Ŵ��� �����֮����еķŴ�ϵ����TCOCһ��Ϊ32��BCOCһ��Ϊ...\n",
					p_name);
}

int main(int argc, char **argv)
{
	char str[0x20];
	int  freq;
	FILE *fr;

	if (argc != 5)
	{
		usage(argv[0]);
		return -1;
	}

	fr = fopen(argv[2], "rt");
	if (!fr)
	{
		printf("�޷���<%s>�ļ�\n", argv[2]);
		return -1;
	}

	min_freq = atoi(argv[3]);
	multi_factor = atoi(argv[4]);

	if (!strcmp(argv[1], "-b"))
	{
		while(2 == fscanf(fr, "%s %d", str, &freq))
			add_bcoc_item(str, freq);
		add_bcoc_item(0, 0);
		output_bcoc();
		test_bcoc();
	}
	else if (!strcmp(argv[1], "-t"))
	{
		while(2 == fscanf(fr, "%s %d", str, &freq))
			add_tcoc_item(str, freq);
		add_tcoc_item(0, 0);
		output_tcoc();
		test_tcoc();
	}
	else
	{
		usage(argv[0]);
		return -1;
	}

	fclose(fr);

	printf("��ɣ���%d��\n", item_count);
	return 0;
}
