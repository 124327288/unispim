/**	��QQƴ�����뷨��ϵͳ�ʿ���
 *
 *	
 *	QQ�ʿ�����ı��Ʒ�����
 *
 *	{
 *		short	ci_count;				//��ͬƴ���Ĵ�����Ŀ 
 *		char	pinyin[];				//ƴ��asciiz��
 *		{
 *			int		freq;				//��Ƶ
 *			char	ci_length;			//����������Ŀ
 *			short	ci[];				//�����������ֵ�λ�ó������
 *		}
 *		{
 *			...							//������д���
 *		}
 *
 *	
 *	ƴ�����ı�ʶ������
 *		letter		: a-z
 *					;
 *
 *		syllable	: letter
 *					| syllable letter
 *					;
 *		
 *		pinyin		: syllable ' syllable
 *					| pinyin ' syllable
 *					;
 *
 *
 */
#define	_CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <string.h>

#define	FILE_NAME		"qq.dat"

char buffer[0xa00000];					//10M������
char *buffer_end;						//��������β
int  file_length;						//�ʿ��ļ�����

int load_data()
{
	FILE *fr;

	fr = fopen(FILE_NAME, "rb");
	if (!fr)
		return 0;

	file_length = fread(buffer, sizeof(buffer[0]), sizeof(buffer), fr);
	buffer_end = buffer + file_length;
	fclose(fr);

	return 1;
}

char *get_next_pinyin(char *p)
{
	int  has_sep;
	char *q;

	//�ж��Ƿ񵽽�β
	for (; p < buffer_end; p++)
	{
		//�ҵ���һ��Ӣ����ĸ
		if (*p < 'a' || *p > 'z')
			continue;

		if (*(p - 1) != 0)			//������0��ͷ
			continue;

		//Ѱ��ƴ����
		has_sep = 0;
		for (q = p; q < buffer_end; q++)
		{
			if (*q == '\'')
			{
				if (*(q - 1) == '\'')
					break;
				has_sep = 1;
				continue;
			}

			if (*q >= 'a' && *q <= 'z')
				continue;
			break;
		}

		if (!has_sep || *q != 0)		//������0��β
		{
			p = q;
			continue;
		}
		return p;			//�ҵ�һ��ƴ��
	}

	return 0;
}

void output_ci(char *p, int length)
{
	int i;
	char ch;

	for (i = 0; i < length; i++)
	{
		ch = p[i];
		if (!(i % 4))
			ch ^= (char)length;
		printf("%c", ch);
	}
}

void process()
{
	char *p = buffer, *q;
	char ci_count;
	char ci_length;
	int  i;

	if (!load_data())
		return;

	p += 0xcef50;		//����ǰ�治��Ҫ������

	while(p = get_next_pinyin(p))
	{
		//�ж�ƴ��ǰ���Ƿ�Ϊ��ѡ����Ŀ��ע�⣺a'aǰ��Ϊ0
		ci_count = *(p - 2);
		if (ci_count <= 0 || ci_count > 0x20)		//�жϴʼ����ĺϷ���
			continue;

		q = p + strlen(p) + 1;		//ָ��ƴ����β
		for (i = 0; i < ci_count; i++)
		{
			q += sizeof(int);				//������Ƶ
			ci_length = *q++;
			output_ci(q, ci_length);
			q += ci_length;
			printf("\t");
			//���ƴ��
			printf("%s\n", p);
		}
		p = q;
	}
}

void main()
{
	process();
}
