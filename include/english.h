#ifndef	_ENGLISH_H_
#define	_ENGLISH_H_

#include <kernel.h>

#ifdef __cplusplus
extern "C" {
#endif

#define	ENGLISH_LIB_FILE_NAME		TEXT("unispim6\\english\\english.dat")
#define ENGLISH_TRANS_FILE_NAME		TEXT("unispim6\\english\\english_trans.dat")

#define ENGLISH_MAX_ITEMS			80000
#define ENGLISH_LETTER_COUNT		26
#define ENGLISH_WORD_MAX_SIZE		0x200
#define ENGLISH_TRANS_BUFFER_SIZE	50000

//Ӣ�Ĵʿ�ͷ����
typedef struct tagENGLISHWORDLIB
{
	int			signature;													//�ʿ��ǩ��
	int			count;														//�ʻ���Ŀ
	int			letter_index[ENGLISH_LETTER_COUNT];							//����ĸ������
	int			index[ENGLISH_MAX_ITEMS];									//������
	char		data[1];													//����
} ENGLISHWORDLIB;

//Ӣ�ķ���ͷ����
typedef struct tagENGLISHTRANSLIB
{
	int			signature;													//ǩ��
	int			count;														//��Ŀ��
	int			letter_index[ENGLISH_LETTER_COUNT][ENGLISH_LETTER_COUNT];	//����ĸ����
	int			EngIndex[ENGLISH_TRANS_BUFFER_SIZE];						//Ӣ�ĵ�������
	int			TransIndex[ENGLISH_TRANS_BUFFER_SIZE];						//��������
	TCHAR		data[1];													//����
} ENGLISHTRANSLIB;

extern int LoadEnglishData(const TCHAR *file_name);
extern int FreeEnglishData();
extern int GetEnglishCandidates(const TCHAR *prefix, CANDIDATE *candidate_array, int array_length);

extern int LoadEnglishTransData(const TCHAR *file_name);
extern int FreeEnglishTransData();
extern TCHAR* GetEnglishTranslation(const TCHAR *english_word);

#ifdef __cplusplus
}
#endif

#endif