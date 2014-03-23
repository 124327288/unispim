/*	�����ϲ�ֳ�Ϊ����/����/����.../N�ֵ�Token�����ڽ���
 *	��ԪToken���ִ�����ͳ�ơ�
 *	
 *	����ӱ�׼�����ж�ȡ���ݣ����׼����������
 *
 *	Usage:
 *	token [max] | [min max]
 *	min		��Сtoken���ȣ�Ĭ��Ϊ2����СֵΪ2
 *	max		���token���ȣ�Ĭ��Ϊ8�����ֵΪ8
 *	
 */	
#include <stdio.h>
#include <stdlib.h>

#define		MIN_LENGTH		2
#define		MAX_LENGTH		8

#define IsGB(hz) 		(((hz & 0xff) >= 0xb0) && ((hz & 0xff) <= 0xf7) && ((hz >> 8) >= 0xa1) && ((hz >> 8) <= 0xfe))
#define	TERMINAL		(*(HZ*)"��")
#define	END				0

typedef unsigned short	HZ;

int min_length, max_length;

HZ get_hz()
{
	unsigned char char1, char2;
	HZ hz;

	if (1 != fscanf_s(stdin, "%c", &char1, sizeof(char)))
		return END;

	if (char1 < 0xa0)
		return TERMINAL;

	if (1 != fscanf_s(stdin, "%c", &char2, sizeof(char)))
		return END;

	hz = char2 * 256 + char1;

	if (IsGB(hz))
		return hz;

	return TERMINAL;
}

int is_correct_token(HZ *hz, int count)
{
	int i;

	for (i = 0; i < count; i++)
		if (hz[i] == TERMINAL || hz[i] == END)
			return 0;
	return 1;
}

void output_token(HZ *hz, int count)
{
	int i;

	for (i = 0; i < count; i++)
		printf("%c%c", hz[i] % 0x100, hz[i] / 0x100);

	printf("\n");
}

void output_correct_token(HZ *hz, int count)
{
	if (is_correct_token(hz, count))
		output_token(hz, count);
}

int process_token()
{
	HZ hz_set[MAX_LENGTH];
	int i;

	for (i = 0; i < max_length; i++)
		hz_set[i] = TERMINAL;

	do
	{
		for (i = min_length; i <= max_length; i++)
			output_correct_token(hz_set, i);

		for (i = 0; i < max_length - 1; i++)
			hz_set[i] = hz_set[i + 1];

		hz_set[i] = get_hz();
	}while(hz_set[0] != END);

	return 1;
}

void usage()
{
	static const char *usage_string = 
		"Usage:\n"
		"token [max] | [min max]\n"
		"min		��Сtoken���ȣ�Ĭ��Ϊ2����СֵΪ2\n"
		"max		���token���ȣ�Ĭ��Ϊ8�����ֵΪ8\n"
		"����\n"
		"token 5\n"
		"      ���token����Ϊ5\n"
		"token 3 7\n"
		"      ��Сtoken����Ϊ3����󳤶�Ϊ7\n";

	printf("%s\n", usage_string);
}

int main(int argc, char **argv)
{
	min_length = MIN_LENGTH;				//Ĭ�ϵ�token����
	max_length = MAX_LENGTH;

	if (argc == 2)
		max_length = atoi(argv[1]);
	else if (argc == 3)
	{
		min_length = atoi(argv[1]);
		max_length = atoi(argv[2]);
	}

	if (min_length < MIN_LENGTH || max_length > MAX_LENGTH || min_length > max_length)
	{
		usage();
		exit(-1);
	}

	process_token();

	return 0;
}
