/**	���������зֳ���
 *	��������
 *	���ڴ��ж����Ե��з���䣬�����ami.txt�ļ��У�����
 *	���ڼ��Ľ���
 */
#include <iostream>
#include <fstream>
#include <string.h>
#include "dictionary.h"

using namespace std;

//#define	OUTPUT_AMBI

#define	DEFAULT_DICT_NAME		"dict-seg.txt"
#define	MAX_MID_RESULTS			0x40

struct MID_RESULT					//�ִ��м���
{
	int length0;					//��һ�鳤��
	int length1;					//�ڶ��鳤��
	int length2;					//�����鳤��
	int count;						//����Ŀ
	int total_length;				//ȫ������
	int avg_length;					//ƽ������
	int avg_diff2;					//������
	int free_morphemic;				//�������ض�
};

class mmseg
{
	public:
		const char	*ambi_name;
		dictionary	*dict;

		mmseg(const char *dict_name, const char *ambi_name);
		~mmseg();
		int get_word_length(const char *str, int length);
		int get_word_length(const char *str);
		int get_mid_result(const char *str, int length, MID_RESULT result[]);
		int new_get_mid_result(const char *str, int length, MID_RESULT result[]);
		void output_result(MID_RESULT *result, int index, const char *str);
		int segment(const char *str, int length);
		int segment(const char *str);
		void ambi_out(const char *str, MID_RESULT *result);
};

