/**	������ɷ���ʽ�ṹ��ͷ�ļ�
 */

#ifndef	_GRAM_H_
#define	_GRAM_H_

#define	SJX_FREQ					(200000000)

#define	MAX_BCOUNT					(1.0e+11)

#define	BIGRAM_SIGN					0x20072222
#define	TRIGRAM_SIGN				0x20073333

#define	MAX_WORDS_BIT				18
#define	ONE_COUNT_BIT				14

#define	MAX_SEG_WORD_LENGTH			16						//�ִʴʵ��е���󳤶�
#define	MAX_SEG_WORDS				(1 << MAX_WORDS_BIT)	//�ʵ������Ĵʸ���

#define	BIGRAM_WORD_LIST_SIZE		(1 << 22)				//�ʱ�Ԥ���ռ�
#define	BIGRAM_INDEX0_SIZE			(1 << MAX_WORDS_BIT)	//��һ����������
#define	BIGRAM_ITEM_SIZE			(1 << 25)				//Bigram��ռ�

#define	ENCODE(x)		((((x) >> 7) | ((x) << 9)) ^ 0xaa55)
#define	DECODE(x)		((((x) ^ 0xaa55) << 7) | (((x) ^ 0xaa55) << 7))

#define	XM				(0.618)
//#define	XM				(1.0)
#define	RT				(330.0)						//�������Ӻ���û���ô�

typedef unsigned short HZ;
typedef unsigned short WORD;
typedef unsigned char  BYTE;

#pragma pack(1)

//����������
typedef struct tagGRAM_INDEX			//Bigram, Trigram����
{
	int		word_pos;					//�ڴʱ��е�λ��
	int		word_freq;					//�ôʴ�Ƶ
	int		item_index;					//�������ڵ�λ��
	int		start_count;				//������ͷ����Ŀ
}GRAM_INDEX;

//������
typedef struct tagGRAM_ITEM				//Bigram��Trigram��
{
	unsigned int	word_index : MAX_WORDS_BIT;		//���ڴʱ��е�λ��
	unsigned int	count : ONE_COUNT_BIT;			//���ּ����������ݲ����Ա���ʱ����������ITEM
}GRAM_ITEM;

//GRAM�ļ�ͷ��
typedef struct tagGRAM_HEADER
{
	int			sign;								//gram��ʶ

	long long	total_word_freq;					//ȫ���ʻ�ļ���

	long long	total_bigram_count;					//ȫ��bigram����
	long long	total_bigram_in_count;				//����ģ�͵�bigram�������������ڼ���ʣ�����

	int			end_token_count;					//�������ż�����������������β�ĸ��ʼ��㣩

	int			word_list_size;						//�ʱ��С

	int			index0_count;						//��һ��������Ŀ
	int			index0_size;						//��һ��������С

	int			index1_count;						//�ڶ���������Ŀ
	int			index1_size;						//�ڶ���������С

	int			item_count;							//��������Ŀ
	int			item_size;							//������ռ��С

	int			word_list_pos;						//�ʱ���ʼλ��
	int			index0_data_pos;					//�������ڼ���ָ��Ŀռ�
	int			index1_data_pos;					//�������ڼ���ָ��Ŀռ�
	int			item_data_pos;						//�����û�����ָ��Ŀռ�

	//������̣�����ָ��
	//index0_data = (GRAM_INDEX*)gram_data.index0_data;
	//index1_data = (GRAM_INDEX*)((char*)index0_data + sizeof(GRAM_INDEX) * index0_count);
	//item_data   = (GRAM_ITEM*) ((char*)index1_data + sizeof(GRAM_INDEX) * index2_count);

}GRAM_HEADER;

//gram�ļ��ṹ
typedef struct tagGRAM_DATA
{
	GRAM_HEADER		header;										//Bigram/Trigramͷ	
	char			align_data[0x400 - sizeof(GRAM_HEADER)];	//����Ԥ���ռ�

//	char			data[];									//���ݿ�ʼλ��

//	GRAM_WORD_INDEX	word_index[GRAM_WORD_SIZE];					//�ʱ�����Ŀ
//	char			words[BIGRAM_WORD_LIST_SIZE];				//�ʱ�����	

//	GRAM_INDEX		index0[BIGRAM_INDEX0_SIZE];					//index0�Ĵ洢��
//	GRAM_INDEX		index1[TRIGRAM_INDEX1_SIZE];				//index1�Ĵ洢��
//	GRAM_ITEM		items[BIGRAM_ITEM_SIZE];					//Bigram���ݣ����������Լ�������
}GRAM_DATA;

#pragma pack()

#ifdef	__cplusplus
extern "C" 
{
#endif

int GetBigramCount(GRAM_DATA *bigram_data, const char *ci1, const char *ci2);
double GetBigramValue(GRAM_DATA *bigram_data, const char *ci1, const char *ci2);
double GetBackOffProbability(GRAM_DATA *bigram_data, int index1, int index2);

#define	GetGramWordList(bigram)	((char*)bigram + bigram->header.word_list_pos)
#define	GetGramIndex(bigram)	((GRAM_INDEX*) ((char*)bigram + bigram->header.index0_data_pos))
#define	GetGramItem(bigram)		((GRAM_ITEM*) ((char*)bigram + bigram->header.item_data_pos))

void encode_word_list(char *word_list, int word_list_pos);
void decode_word_list(char *word_list, int word_list_pos);

#ifdef	__cplusplus
}
#endif

#endif
