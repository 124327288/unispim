/* ���дʻ�ĺϲ�
   ��: 
   	���ǵ�
	���ǵ�		8
	���ǵ�		-3

   �ϲ�Ϊ: ���ǵ� 6
*/
#include <stdio.h>

#define	MAX_COUNT		4000000000		//����Ƶ��
#define	MAX_LENGTH		8				//���ʳ���
#define	MIN_FREQ		3				//��С��Ƶ
#define	LINE_LENGTH		0x400			//����г���

int min_freq;

void output(const char *string, unsigned int count)
{
	if (count > MAX_COUNT)
		count = MAX_COUNT;

	if (count >= min_freq)
		fprintf(stdout, "%-20s  %12u\n", string, count);
}

void process_line(const char *line, char *token, int *count)
{
	//���ֻ��token����countΪ1
	if (1 == sscanf_s(line, "%s%u", token, LINE_LENGTH, count, sizeof(int)))
		*count = 1;
}

void process()
{
	char line[LINE_LENGTH], tmp1[LINE_LENGTH], tmp2[LINE_LENGTH];
	char *cur_token, *last_token, *p;
	unsigned int  last_count, cur_count;

	last_token = tmp1;
	cur_token = tmp2;

	fgets(line, LINE_LENGTH, stdin);
	if (feof(stdin))
		return;

	process_line(line, last_token, &last_count);
	do
	{
		fgets(line, LINE_LENGTH, stdin);
		if (feof(stdin))
			break;

		process_line(line, cur_token, &cur_count);

		if (!strcmp(last_token, cur_token))
		{
			int new_count;

			new_count = last_count + cur_count;
			if (new_count < last_count)				//���
				last_count = MAX_COUNT;
			else
				last_count = new_count;
			continue;
		}

		output(last_token, last_count);
		//������ǰtoken����һ�ε�token
		p = cur_token;
		cur_token = last_token;
		last_token = p;
		last_count = cur_count;
	}while(1);

	output(last_token, last_count);
}

int main(int argc, char **argv)
{
	min_freq = MIN_FREQ;

	if (argc > 1)
		min_freq = atoi(argv[1]);

	if (min_freq < MIN_FREQ)
		min_freq = MIN_FREQ;

	process();
	return 0;
}
   
