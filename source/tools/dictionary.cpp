#include <iostream>
#include <fstream>
#include <dictionary.h>

using namespace std;

dictionary::~dictionary()
{
	delete table;
}

dictionary::dictionary(const char *dict_name)
{
	ci_count = 1024;
	table = new hash_table();
	memset(hz_freq, 0, sizeof(hz_freq));
	make_item(dict_name);
}

void dictionary::make_item(const char *dict_name)
{
	ifstream in_file;
	char ci_item[0x100];
	int  freq;

	in_file.open(dict_name);

	do
	{
		in_file >> ci_item;
		in_file >> freq;

		if (ci_item[0] == '#')			//ע����
			continue;

		if (!in_file.good())
			break;
		//����Ƿ�Ϊ�������֣�����ǣ���Ժ��ֵ�Ƶ�ʽ��и�ֵ
		int ci_len = (int)strlen(ci_item);

		if (ci_len == 2)		//��������
			hz_freq[*(unsigned short*)ci_item] = freq;
		else if (ci_len > 16)
			continue;
		table->add(ci_item, ci_count++);
	}while(1);

	in_file.close();
}

/**	�ַ����Ƿ�Ϊ�ʻ�
 */
int dictionary::is_word(const char *str, int length)
{
	if (length == 2)
	{
		if (str[0] & 0x80)
			return 1;
		return 0;
	}

	if (table->get_value(str, length))
		return 1;
	return 0;
}

//��������ƥ��
int dictionary::max_match_length(const char *str, int length)
{
	//ʹ��ż�����Ƚ��в��Ҽ�Ϊ����ģʽ
	int len, last_len;

	last_len = 0;
	for (len = 2; len <= length && len < MAX_WORD_LENGTH; len += 2)
	{
		if (table->get_value(str, len))
			last_len = len;
	}

	return last_len;
}

int dictionary::max_match_length(const char *str)
{
	//ʹ��ż�����Ƚ��в��Ҽ�Ϊ����ģʽ
	int len, last_len;

	if (!str[0])
		return 0;

	last_len = 0;
	for (len = 2; str[len] && len < MAX_WORD_LENGTH; len += 2)
	{
		if (table->get_value(str))
			last_len = len;
	}

	return last_len;
}

/**	��ú��ֵ��������ض�
 */
int dictionary::get_free_morphemic(const char *str)
{
	return hz_freq[*(unsigned short*)str];
}

/**	��ôӵ�ǰ��ʼ�Ĵʻ㳤�ȣ�������8����������Ǵʻ㣩
 */
int dictionary::get_words_length(const char *str, int length, int *word_length)
{
	//�˴�Ҫ����׵ĺ��ֱ����ڴʱ��У�����Ͳ������ģ��
	//������ƣ���ӿ���Ȳ��ֽܷ���ȷ�Ĵ���
	if (!hz_freq[*(unsigned short*)str])
		return 0;

	length &= 0xfffffffe;
	length = MAX_WORD_LENGTH < length ? MAX_WORD_LENGTH : length;

	word_length[0] = 2;					//��һ��Ԥ������
	int count = 1;
	hash_node *node;

	for (int i = 4; i <= length; i += 2)
	{
		node = table->get_node(str, i);
		if (node)
		{
			word_length[count++] = i;
			if (!node->close_word_length)
				break;
			i = node->close_word_length - 2;
		}
	}

//	if (count == 1 && !hz_freq[*(unsigned short*)str])
//		return 0;

	return count;
}

