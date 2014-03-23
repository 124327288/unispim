#ifndef	_HASH_H_
#define	_HASH_H_

#include <fstream>

using namespace std;

#include <stdlib.h>
#include <string.h>

#define	HASH_SIZE	0x400000

/**	hash��ڵ�
 */
class hash_node
{
	public:
		char 			str[24];				//������ַ���
		int				value;					//�����ֵ
		int				close_word_length;		//��ӽ�����һ���ʻ�ĳ���
};

/**	�ַ���hash��
 */
class hash_table
{
	public:
		unsigned int ascii_keys[0x100][0x40];		//�ַ�ɢ��key

		hash_node	*table;							//hash����

		~hash_table();
		hash_table();
		void add(const char *str, int value);
		void add(const char *str);
		void pure_add(const char *str, int value);
		void increase_count(const char *str);
		hash_node *get_node(const char *str, int length);
		hash_node *get_node(const char *str);
		unsigned int get_rand_byte();
		void init_ascii_keys();
		int get_value(const char *str);
		int get_value(const char *str, int length);
		static int is_same_string(const char *s1, int length, const char *s2);
		void output(ofstream &out_file);
		void output_conflics();
		static int is_same_string(const char *s1, const char *s2);
		unsigned int get_key(const char *str);
		unsigned int get_key(const char *str, int length);
};

#endif
