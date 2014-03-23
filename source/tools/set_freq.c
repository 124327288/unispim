/**	ͨ���������м�����Ĵʻ�Ĵ�Ƶ����ϵͳ�ʿ⡢�����ʿ��
 *	���е�����ȷ����Ϊ׼ȷ�Ĵ�Ƶ��
 *
 *	ʹ�ñ������Ҫ��
 *		���������ļ����밴�պ��֣�Short���ķ�ʽ��������
 *		(���ڱ��˵���������������ַ�ʽ��ʵ���ϣ�Ӧ����string
 *		������ʽ)
 *
 *	�����в�����
 *		set_freq ci_file freq_file out_file
 *
 *	ci_file��ʽ��
 *		����		wo'men		1
 *	
 *	����ʡ�Դ�Ƶ��ϵͳ��ʡ�ԵĴ�Ƶ����Ϊ1
 *
 *	��Ƶת����ʽ��
 *		���ڴʣ�����19λ��Ƶ���֣�����23λ��Ƶ��
 *	��Ƶ���Ƚ���������λ�Ĳ����������Ƶ��Ŀ����500000�Ļ�����
 *	����500000�Ĳ���ӳ�䵽2000���ڡ�
 *	��Ƶ��������һλ���������8000000�򣬽��߳�����ӳ�䵽300000
 *	���ڡ�
 *		
 */
#define		_CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <string.h>

typedef	unsigned short	HZ;

//��
#define	FREQ_SHIFT		2
#define	HIGHER_DIVIDER	2000
#define	MAX_FREQ		((1 << 19) - 1)
#define	MAX_BASE_FREQ	500000

//��
//#define	FREQ_SHIFT		1
//#define	HIGHER_DIVIDER	2000
//#define	MAX_FREQ		((1 << 23) - 1)
//#define	MAX_BASE_FREQ	8000000

char *ci_file_name;
char *freq_file_name;
char *out_file_name;

/**	������Ĵ�Ƶ
 */
int get_adjusted_freq(unsigned int freq)
{
	int new_freq;

	new_freq = (int)(freq >> 2);
	if (new_freq > MAX_BASE_FREQ)
		new_freq = MAX_BASE_FREQ + new_freq / HIGHER_DIVIDER;

	if (new_freq > MAX_FREQ)
		new_freq = MAX_FREQ;

	return new_freq;
}

/**	�������
 */
void output(FILE *f, const char *hz, const char *py, unsigned int freq)
{
	freq = get_adjusted_freq(freq);
	fprintf(f, "%s\t%s\t%u\n", hz, py, freq);
}

/**	��ȡ�����ļ��е�һ��
 */
int get_ci_line(FILE *f, char *hz, char *py, int *freq)
{
	static char buffer[0x400];
	char   *p;
	int    ret;

	do
	{
		p = fgets(buffer, sizeof(buffer), f);
		if (!p)
			return 0;
	}while(*p == '#');

	ret = sscanf(p, "%s %s %d", hz, py, freq);
	if (ret == 3)
		return 1;
	if (ret == 2)
	{
		*freq = 0;
		return 1;
	}

	return 0;
}

/**	�������Ƶ���е�һ��
 */
int get_freq_line(FILE *f, char *hz, int *freq)
{
	static char buffer[0x400];
	char   *p;
	int    ret;
	static int line;

	line++;
	if (!(line & 0xffff))
		printf("%d\r", line);

	do
	{
		p = fgets(buffer, sizeof(buffer), f);
		if (!p)
			return 0;
	}while(*p == '#');

	ret = sscanf(p, "%s %d", hz, freq);
	return ret == 2;
}

/**	�ȽϺ���
 *	��Ϊansi����
 */
int compare_hz(HZ *hz1, HZ *hz2)
{
	unsigned char *p, *q;
	p = (unsigned char*)hz1, q = (unsigned char*)hz2;
	while(*p && *q)
	{
		/*
		if (*(HZ*)p != *(HZ*)q)
			return *(HZ*)p - *(HZ*)q;
		p += sizeof(HZ);
		q += sizeof(HZ);
		*/
		if (*p != *q)
			return *p - *q;
		p++;
		q++;
	}
	if (*p)
		return 1;
	if (*q)
		return -1;
	return 0;
}

/**	�ļ�������
 */
char file_buffer0[0x100000];
char file_buffer1[0x100000];
char file_buffer2[0x100000];

/**	�����ļ�
 */
int process()
{
	char ci_hz[0x100], freq_hz[0x100], py[0x100];
	int  old_freq, new_freq;
	FILE *f_ci, *f_freq, *fw;
	int  ret, freq_over, ci_over;

	fw = fopen(out_file_name, "w");
	if (!fw)
	{
		printf("<%s>�޷�����\n", out_file_name);
		return -1;
	}

	//���û�������С���ӿ�������ٶ�
	if (setvbuf(fw, file_buffer0, _IOFBF, sizeof(file_buffer0)))
		printf("���û���������\n");

	f_ci = fopen(ci_file_name, "r");
	if (!f_ci)
	{
		printf("<%s>�޷���\n", ci_file_name);
		return -1;
	}

	//���û�������С���ӿ�������ٶ�
	if (setvbuf(f_ci, file_buffer1, _IOFBF, sizeof(file_buffer1)))
		printf("���û���������\n");

	f_freq = fopen(freq_file_name, "r");
	if (!f_freq)
	{
		printf("<%s>�޷���\n", freq_file_name);
		return -1;
	}

	//���û�������С���ӿ�������ٶ�
	if (setvbuf(f_freq, file_buffer2, _IOFBF, sizeof(file_buffer2)))
		printf("���û���������\n");

	if (!get_freq_line(f_freq, freq_hz, &new_freq) || !get_ci_line(f_ci, ci_hz, py, &old_freq))
	{
		printf("�ļ���ȡʧ��\n");
		fclose(f_ci);
		fclose(f_freq);
		fclose(fw);
		return -1;
	}

	freq_over = 0;
	ci_over = 0;
	//���������ļ�
	while(!freq_over && !ci_over)
	{
		ret = compare_hz((HZ*)ci_hz, (HZ*)freq_hz);
		if (!ret)		//��ͬ
		{
			output(fw, ci_hz, py, new_freq);
			//���ڿ����ڴʿ�������ͬ�Ĵʣ�����Ҫ������ȡ����
			if (!get_ci_line(f_ci, ci_hz, py, &old_freq))
				ci_over = 1;
			continue;
		} 

		if (ret < 0)		//�����ļ�С�ڴ�Ƶ�ļ�
		{
			output(fw, ci_hz, py, 0);		//��ʱ�������0
			if (!get_ci_line(f_ci, ci_hz, py, &old_freq))
				ci_over = 1;
			continue;
		}

		//�����ļ����ڴ�Ƶ�ļ�
		if (!get_freq_line(f_freq, freq_hz, &new_freq))
			freq_over = 1;
	};

	//���freq�ļ������������ʣ��Ĵ�����Ŀ
	if (freq_over)
	{
		output(fw, ci_hz, py, 0);
		while(get_ci_line(f_ci, ci_hz, py, &old_freq))
			output(fw, ci_hz, py, old_freq);
	}

	fclose(fw);
	fclose(f_ci);
	fclose(f_freq);

	return 0;
}

const char *usage = 
	"set_freq ci_file freq_file out_file\n"
	"\n"
	"ci_file     ׼�����д�Ƶ���õ��ļ�\n"
	"freq_file   �����������ɵĴ�Ƶ�ļ�\n"
	"out_file    ����ļ�\n";

int main(int argc, char **argv)
{
	if (argc != 4)
	{
		printf(usage);
		return -1;
	}

	ci_file_name = argv[1];
	freq_file_name = argv[2];
	out_file_name = argv[3];

	return process();
}
   
