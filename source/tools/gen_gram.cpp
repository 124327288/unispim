/**	��������ϵ�bigram��Ϣ��������������
 *
 *	��������ָ�Ѿ���ɺ��ִַʴ�������ϡ�
 *	�����д����֮���ÿո�ָ�����á��ָ������
 *
 *	�ó����������������1.5G���ڴ�Ļ����ϡ�
 */

#include <fstream>
#include <iostream>

using namespace std;

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <map_file.h>

#define	MAX_FREQ		0x7fffffff
#define	MIN_FREQ		3

const char *usage =
	"gen_gram in_file out_file [min_freq]\n"
	"\n"
	"in_file           �������ļ�����\n"
	"out_file          gram�ļ����ƣ���0-9��������Ϊ�����ļ��ĺ�׺\n"
	"min_freq          ��С�Ĵ�Ƶ��Ĭ��Ϊ3\n"
	"\n"
	"ע�⣡������ʹ���ڴ�ܴ󣬽�����2G���ϵĻ���������\n"
	"\n";

char *buffer;
int  buffer_index;
int  buffer_length;
int  buffer_size = 0x800000;
long long total_count;

char file_buffer[0x100000];				//�ļ����������

FILEMAPHANDLE in_file;					//�����ļ����

int min_freq = MIN_FREQ;

//BIGRAM������
struct B_ITEM
{
	int index0, index1;			//�������
	int count;					//����
	struct B_ITEM *next;		//ͬһ��index0��ʼ����һ��λ��
};

//TRIGRAM������
struct T_ITEM
{
	int index0, index1, index2;	//�������
	int count;					//����
	struct T_ITEM *next;		//��һ��λ��
};

//ɢ�б���
struct W_NODE
{
	char	str[28];			//������ַ���
	int		is_gb_word;			//�Ƿ�ΪGB����
};

#define	W_HASH_SIZE			0x100000			//����hash���С
#define	B_HASH_SIZE			0x1000000			//B��hash���С
#define	B_ITEM_SIZE			0x2000000			//��Ԫ��Ŀ��Ŀ
#define	T_HASH_SIZE			0x2000000			//T��hash���С
#define	T_ITEM_SIZE			0x2000000			//��Ԫ��Ŀ��Ŀ

W_NODE		*w_hash_table;						//��hash��

B_ITEM		**b_hash_table;						//��Ԫhash��
B_ITEM		*b_item_table;						
int			b_item_count;

T_ITEM		**t_hash_table;						//��Ԫhash��
T_ITEM		*t_item_table;
int			t_item_count;

int	process_bigram = 1;						//�Ƿ���bigram
int process_trigram = 0;					//�Ƿ���trigram

unsigned int ascii_keys[0x100][0x20];		//�ַ�ɢ��key

__inline void swap(int *x, int *y)
{
	int t;
	t = *x;
	*x = *y;
	*y = t;
}

/**	�򻺳����������
 */
int get_data()
{
	buffer_length = FileMapGetBuffer(in_file, &buffer, buffer_size);
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

/**	��ʼ������
 */
int init()
{
	cout << "����hash���ڴ�...\n";
	w_hash_table = new W_NODE[W_HASH_SIZE];
	if (!w_hash_table)
	{
		cout << "����ʧ��\n";
		return 0;
	}

	memset(w_hash_table, 0, W_HASH_SIZE * sizeof(W_NODE));
	b_item_count = t_item_count = 0;

	if (process_bigram)
	{
		cout << "�����Ԫ�����ڴ�...\n";
		b_hash_table = new B_ITEM * [B_HASH_SIZE];
		if (!b_hash_table)
		{
			cout << "����b_hash_tableʧ��\n";
			return 0;
		}
		b_item_table = new B_ITEM[B_ITEM_SIZE];
		if (!b_item_table)
		{
			cout << "����b_item_tableʧ��\n";
			return 0;
		}

		cout << "�ڴ�����\n";
		memset(b_hash_table, 0, B_HASH_SIZE * sizeof(B_ITEM*));
		memset(b_item_table, 0, B_ITEM_SIZE * sizeof(B_ITEM));
	}

	if (process_trigram)
	{
		cout << "������Ԫ�����ڴ�...\n";
		t_hash_table = new T_ITEM*[T_HASH_SIZE];
		if (!t_hash_table)
		{
			cout << "����t_hash_tableʧ��\n";
			return 0;
		}
		t_item_table = new T_ITEM[T_ITEM_SIZE];
		if (!t_item_table)
		{
			cout << "����t_item_tableʧ��\n";
			return 0;
		}

		cout << "�ڴ�����\n";
		memset(t_hash_table, 0, T_HASH_SIZE * sizeof(T_ITEM*));
		memset(t_item_table, 0, T_ITEM_SIZE * sizeof(T_ITEM));
	}

#define	RAND_BYTE	(((unsigned int) rand()) & 0xff)

	cout << "����hash����\n";
	srand(12345);
	for (int i = 0; i < sizeof(ascii_keys) / sizeof(ascii_keys[0]); i++)
		for (int j = 0; j < sizeof(ascii_keys[0]) / sizeof(ascii_keys[0][0]); j++)
			ascii_keys[i][j] = 
				(RAND_BYTE) |
				(RAND_BYTE << 8) |
				(RAND_BYTE << 16) |
				(RAND_BYTE << 24);

	return 1;
}

/**	�ͷ�ȫ���ڴ�
 */
void free_all()
{
	delete [] w_hash_table;
	if (process_bigram)
	{
		delete [] b_hash_table;
		delete [] b_item_table;
	}

	if (process_trigram)
	{
		delete [] t_hash_table;
		delete [] t_item_table;
	}
}

/**	����ַ�����ɢ�м�
 */
unsigned int get_string_key(const char *str)
{
	unsigned int key = 0;
	
	while(*str)
	{
		key ^= ascii_keys[(unsigned char)*str][(unsigned char)*str];
		str++;
	}

	return key;
}

/**	�ж��Ƿ�ΪGB2312�֣������Ƚϳ��õ�GBK�֣�
 */
__inline int is_gb(unsigned short zi)
{
	int  i;
	static const char special_zi[] = "������h�h���t�����F";

	if ((zi & 0xff) >= 0xb0 && ((zi & 0xff) <= 0xf7) && (zi >> 8) >= 0xa1 && (zi >> 8) <= 0xfe)
		return 1;		//GB2312����

	for (i = 0; i < sizeof(special_zi); i += 2)
		if (zi == *(unsigned short*)(special_zi + i))
			return 1;

	return 0;
}

/**	�ж��Ƿ�Ϊgb����
 */
int is_all_gb(const char *str)
{
	while(*str)
	{
		if (!is_gb(*(unsigned short*)str))
			return 0;
		str += 2;
	}
	return 1;
}

/**	��ô�ɢ�����������ڣ��򴴽�һ��
 *	������ַ�����ɢ�м�ֵ��
 */
int get_word_index(const char *str, unsigned int *ret_key)
{
	unsigned int key = get_string_key(str);
	int index = key % W_HASH_SIZE;

	while(*(int*)w_hash_table[index].str)
	{
		if (!strcmp(str, w_hash_table[index].str))
		{
			*ret_key = key;
			return index;
		}

		index = (index + 1) & (W_HASH_SIZE - 1);
	}

	//û���ҵ�������һ��
	strcpy(w_hash_table[index].str, str);
	w_hash_table[index].is_gb_word = is_all_gb(str);
	*ret_key = key;
	return index;
}

/**	���Ӷ�Ԫ�����
 *	���أ�
 *		1���ɹ�
 *		0��ʧ�ܣ�ӦΪ�洢������
 */
int inc_binary_item(int index0, unsigned int key0, int index1, unsigned int key1)
{
	unsigned int key = key0 ^ ((key1 << 5) | (key1 >> 27));
	int index = key % B_HASH_SIZE;
	B_ITEM *p, *last_p;

	p = b_hash_table[index];
	last_p = 0;
	while(p)
	{
		if (p->index0 == index0 && p->index1 == index1)			//��ͬ����
		{
			p->count++;
			//Ϊ�˼ӿ촦���ٶȣ��Ը�Ƶ�ʵ�����ǰ�ƶ���
			if (last_p && p->count > last_p->count)// + 0x10)
			{
				swap(&p->count, &last_p->count);
				swap(&p->index0, &last_p->index0);
				swap(&p->index1, &last_p->index1);
			}
			return 1;
		}
		last_p = p;
		p = p->next;
	}

	//û���ҵ�������һ��
	b_item_table[b_item_count].index0 = index0;
	b_item_table[b_item_count].index1 = index1;
	b_item_table[b_item_count].count = 1;

	//�������
	b_item_table[b_item_count].next = b_hash_table[index];
	b_hash_table[index] = &b_item_table[b_item_count];
	b_item_count++;
	
	//�ж��Ƿ�Խ��
	if (b_item_count >= B_ITEM_SIZE)
		return 0;
	return 1;
}

/**	������Ԫ�����
 *	���أ�
 *		1���ɹ�
 *		0��ʧ�ܣ�ӦΪ�洢������
 */
int inc_triple_item(int index0, unsigned int key0, int index1, unsigned int key1, int index2, unsigned int key2)
{
	unsigned int key = key0 ^ key1 ^ key2;
	int index = key % T_HASH_SIZE;
	T_ITEM *p;

	p = t_hash_table[index];
	while(p)
	{
		if (p->index0 == index0 && p->index1 == index1 && p->index2 == index2)			//��ͬ����
		{
			p->count++;
			return 1;
		}
		p = p->next;
	}

	//û���ҵ�������һ��
	t_item_table[t_item_count].index0 = index0;
	t_item_table[t_item_count].index1 = index1;
	t_item_table[t_item_count].index2 = index2;
	t_item_table[t_item_count].count = 1;

	//�������
	t_item_table[t_item_count].next = t_hash_table[index];
	t_hash_table[index] = &t_item_table[t_item_count];
	t_item_count++;
	
	//�ж��Ƿ�Խ��
	if (t_item_count >= T_ITEM_SIZE)
		return 0;
	return 1;
}

/**	�Ƚ�Binary��Ĵ�С
 */
int compare_bnode(const void *vp1, const void *vp2)
{
	int ret;
	const B_ITEM *p1;
	const B_ITEM *p2;

	p1 = (const B_ITEM*) vp1, p2 = (const B_ITEM*) vp2;

	ret = strcmp(w_hash_table[p1->index0].str, w_hash_table[p2->index0].str);
	if (ret)
		return ret;

	return strcmp(w_hash_table[p1->index1].str, w_hash_table[p2->index1].str);
}

/**	������ 
 */
int output_result(const char *out_file_name)
{
	static int count;
	char name[0x200];
	int  break_pos;

	//Ѱ�����һ��.��������չ��ǰ���������
	strcpy(name, out_file_name);
	int len = (int)strlen(out_file_name);
	for (int i = len - 1; i >= 0; i--)
		if (name[i] == '.')
		{
			break_pos = i;
			break;
		}

	count++;
	strncpy(name, out_file_name, break_pos);
	sprintf(name + break_pos, "-%02d", count);
	strcat(name, out_file_name + break_pos);

	cout << "\n��<" << name << ">�ļ�����м�����\n";

	int st = clock();
	FILE *fw;
	fw = fopen(name, "w");
	if (!fw)
	{
		cout << "�޷����ļ�<" << name << ">����д��\n";
		return 0;
	}
	setvbuf(fw, file_buffer, _IOFBF, sizeof(file_buffer));

	cout << "��item��������...\n";
	qsort(b_item_table, b_item_count, sizeof(B_ITEM), compare_bnode);
	cout << "������ʱ:" << (clock() - st) / 1000 << "s\n";

	for (int i = 0; i < b_item_count; i++)
	{
		B_ITEM *p = &b_item_table[i];
		char *str;

		if (p->count < min_freq)
			continue;

		if (!w_hash_table[p->index0].is_gb_word ||
			!w_hash_table[p->index1].is_gb_word)
			continue;

		str = w_hash_table[p->index0].str;
		while(*str)
			_fputc_nolock(*str++, fw);
		_fputc_nolock('\t', fw);

		str = w_hash_table[p->index1].str;
		while(*str)
			_fputc_nolock(*str++, fw);
		_fputc_nolock('\t', fw);

		char no[0x20];
		_ltoa(p->count, no, 10);

		str = no;
		while(*str)
			_fputc_nolock(*str++, fw);
		_fputc_nolock('\n', fw);

//		out_file << w_hash_table[p->index0].str << "\t";
//		out_file << w_hash_table[p->index1].str << "\t";
//		out_file << p->count << endl;
	}

	//����hash���Լ�item��
	memset(w_hash_table, 0, W_HASH_SIZE * sizeof(W_NODE));
	memset(b_hash_table, 0, B_HASH_SIZE * sizeof(B_ITEM*));
	b_item_count = 0;

	cout << "�����ʱ:" << (clock() - st) / 1000 << "s\n";

	fclose(fw);
	return 1;
}

int main(int argc, char **argv)
{
	if (argc < 3)
	{
		cout << usage;
		return -1;
	}

	if (argc >= 4)
		min_freq = atoi(argv[3]);

	ofstream out_file;

	in_file = FileMapOpen(argv[1]);
	if (!in_file)
	{
		cout << "�ļ�<" << argv[1] << ">�޷���!\n";
		return -1;
	}

	//ɾ���������ɵ��ļ�
	_unlink(argv[2]);

	cout << "��ʼ��...\n";
	init();

	cout << "����bigram...\n";

	long long c_count = 0;
	int  st = clock();
	int  c = 0;
	char item0[0x100], item1[0x100];
	int  index0, index1;
	unsigned int key0, key1;
	
	get_word_item(item0);
	index0 = get_word_index(item0, &key0);

	while(get_word_item(item1))
	{
		c++;
		if (!(c & 0xffff))
		{
			cout << "data:" << (total_count - buffer_length + buffer_index) / 0x100000 << "M, ";
			cout << "item:" << int((float)b_item_count / B_ITEM_SIZE * 100) << "%, ";
			cout << "time:" << (clock() - st) / 1000 << "s\r";
		}

		index1 = get_word_index(item1, &key1);

		//ֻ����GB�Ĵʻ�
		if (w_hash_table[index0].is_gb_word && w_hash_table[index1].is_gb_word)
			if (!inc_binary_item(index0, key0, index1, key1))			//��������������Ҫ����
				output_result(argv[2]);

		index0 = index1;
		key0 = key1;
	}

	FileMapClose(in_file);

	output_result(argv[2]);

	cout << "��Ƶ������ʱ:" << (clock() - st) / 1000 << "��\n";
	cout << "���Ƶ�ʽ��\n";

	return 0;
}