/**	��������ϵ���Ƶ����Ƶ��Ϣ
 *
 *	��������ָ�Ѿ���ɺ��ִַʴ�������ϡ�
 *	�����д����֮���ÿո�ָ�����á��ָ������
 */

#include <fstream>
#include <iostream>

using namespace std;

#include <stdio.h>
#include <time.h>
#include <hash_table.h>

#define	MAX_FREQ		0x7fffffff

const char *usage =
	"get_freq in_file out_file\n"
	"\n"
	"in_file           �������ļ�����\n"
	"out_file          Ƶ���ļ�����\n"
	"\n";

char buffer[0x800000];
int  buffer_index;
int  buffer_length;
long long total_count;

FILE *in_file;

/**	�򻺳����������
 */
int get_data()
{
	buffer_length = fread(buffer, 1, sizeof(buffer), in_file);
	buffer_index = 0;
	total_count += buffer_length;
	return buffer_length;
}

/**	�ӻ������л��һ���ַ�
 */
char get_char()
{
	if (buffer_index >= buffer_length)
	{
		if (!get_data())
			return 0;
	}

	return buffer[buffer_index++];
}

/**	���һ���ʻ�
 */
int get_word_item(char *word_item)
{
	char ch;
	char *word_item_save = word_item;

	//Ѱ�ҷǿհ��ַ�
	while((ch = get_char()) && (unsigned char)ch < 0x40)
		;

	if (!ch)			//��β
		return 0;

	*word_item++ = ch;
	while((ch = get_char()) && (unsigned char)ch > 0x40)
		*word_item++ = ch;

	if (!ch)			//������־
		return 0;

	*word_item = 0;
	return word_item - word_item_save;
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		cout << usage;
		return -1;
	}

	ofstream out_file;

	fopen_s(&in_file, argv[1], "r");
	if (!in_file)
	{
		cout << "�ļ�<" << argv[1] << ">�޷���!\n";
		return -1;
	}

	out_file.open(argv[2]);
	if (!out_file)
	{
		cout << "�ļ�<" << argv[2] << ">�޷�����!\n";
		return -1;
	}

	cout << "����hash��...\n";
	hash_table table;

	cout << "�����ִ�Ƶ��...\n";

	long long c_count = 0;
	int  st = clock();
	int  c = 0;
	char item[0x100];
	
	while(get_word_item(item))
	{
		c++;
		if (!(c & 0x7ffff))
			cout << total_count / 0x100000 << "M, " << (clock() - st) / 1000 << "s\r";

		table.increase_count(item);
	}

	fclose(in_file);

	cout << "��Ƶ������ʱ:" << (clock() - st) / 1000 << "��\n";
	cout << "���Ƶ�ʽ��\n";
	table.output(out_file);
	out_file.close();

	return 0;
}