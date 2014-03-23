/*	���ڶ���ͷ�ļ�
 */

#ifndef	_SYLLABLE_H_
#define	_SYLLABLE_H_

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

//extern int sp_used_v;			//˫ƴ�Ƿ�ʹ����v
//extern int sp_used_i;
//extern int sp_used_u;

#define	SP_INI_FILE_NAME	TEXT("unispim6\\ini\\˫ƴ.ini")

#define	MAX_SYLLABLES		512			//����������Ŀ

//�ж������Ƿ���HU��ͷ
#define	IS_START_WITH_HU(syllable)		(syllable.con == CON_H && syllable.vow >= VOW_U && syllable.vow <= VOW_UO)
//�ж������Ƿ���ȫ��ͬ
#define	IS_SAME_SYLLABLE(s1, s2)		(*(short*)(&(s1)) == *(short*)(&(s2)))

#define	IS_TONE(ch)						((ch) == TONE_CHAR_1 || (ch) == TONE_CHAR_2 || (ch) == TONE_CHAR_3 || (ch) == TONE_CHAR_4)

//����ͨ����ַ�
#define	SYLLABLE_ANY_CHAR	'*'		

//�����ַ�
#define	TONE_CHAR_1			'!'				//һ��
#define	TONE_CHAR_2			'@'				//����
#define	TONE_CHAR_3			'#'				//����
#define	TONE_CHAR_4			'$'				//����
#define	TONE_CHAR_CHANGE	'~'				//�任����

#define	TONE_0				0
#define	TONE_1				(1 << 0)
#define	TONE_2				(1 << 1)
#define	TONE_3				(1 << 2)
#define	TONE_4				(1 << 3)

//�з�������
#define	SYLLABLE_SEPARATOR_CHAR		'\''			//���ڷָ����ţ��û����룩
#define	SYLLABLE_SEPARAOTR_CHAR		9				//���ڷָ����ţ�ϵͳ��ӣ�

//��ĸ����
#define	CON_NUMBER			24			//�Ϸ���ĸ����Ŀ

#define	CON_ERROR			-1			//�������ĸ
#define	CON_NULL			0			//û����ĸ
#define	CON_B				1
#define CON_C				2
#define CON_CH				3
#define	CON_D				4
#define CON_F				5
#define CON_G				6
#define CON_H				7
#define	CON_J				8
#define CON_K				9
#define CON_L				10
#define	CON_M				11
#define CON_N				12
#define	CON_P				13
#define	CON_Q				14
#define	CON_R				15
#define CON_S				16
#define CON_SH				17
#define	CON_T				18
#define	CON_W				19
#define	CON_X				20
#define	CON_Y				21
#define CON_Z				22
#define CON_ZH				23
#define	CON_END				24
//��ĸͨ���
#define	CON_ANY				25

//��ĸ����
#define	VOW_NUMBER			34			//�Ϸ���ĸ����Ŀ

#define	VOW_ERROR			-1			//�������ĸ
#define	VOW_NULL			0			//û����ĸ
#define	VOW_A				1 
#define	VOW_AI				2 
#define	VOW_AN				3
#define	VOW_ANG				4
#define	VOW_AO				5
#define	VOW_E				6
#define	VOW_EI				7
#define	VOW_EN				8
#define	VOW_ENG				9
#define	VOW_ER				10
#define	VOW_I				11
#define	VOW_IA				12
#define	VOW_IAN				13	
#define	VOW_IANG			14
#define	VOW_IAO				15
#define	VOW_IE				16
#define	VOW_IN				17
#define	VOW_ING				18
#define	VOW_IONG			19
#define	VOW_IU				20
#define	VOW_O				21
#define	VOW_ONG				22
#define	VOW_OU				23
#define	VOW_U				24
#define	VOW_UA				25
#define	VOW_UAI				26
#define	VOW_UAN				27
#define	VOW_UANG			28
#define	VOW_UE				29
#define	VOW_UI				30
#define	VOW_UN				31
#define	VOW_UO				32
#define	VOW_V				33
#define	VOW_END				34
//��ĸͨ���
#define	VOW_ANY				35

//��������
typedef struct tagSYLLABLE
{
	unsigned short	con : 5;				//��ĸ��ʶ
	unsigned short	vow : 6;				//��ĸ��ʶ
	unsigned short	tone : 5;				//����0-5����λ����������1-4λ�ֱ��ʶ�ĸ�������5��ʶ����
} SYLLABLE;

//����ת����ṹ
typedef struct tagSYLLABLEMAP
{
	TCHAR	pin_yin[8];				//ƴ����
	int		pin_yin_length : 8;		//ƴ��������
	unsigned int con : 5;			//��ĸ
	unsigned int vow : 6;			//��ĸ
	int		fuzzy_flag;				//ģ����־���������ģ����־���ڵ�����£�������ȷ��ƴ����
									//�磺cuang��ֻ��c��chģ��������£�������ȷ��ƴ�������磺len��
									//��l��r��l��nģ��������£�������ȷ�ġ�
	//int     correct_flag;           //�����־��������������־���ڵ�����£�������ȷ��ƴ����
	TCHAR	sp_pin_yin[4];			//˫ƴƴ����
	TCHAR	pin_yin_tone1[8];		//��������ƴ����(1��)
	TCHAR	pin_yin_tone2[8];		//��������ƴ����(2��)
	TCHAR	pin_yin_tone3[8];		//��������ƴ����(3��)
	TCHAR	pin_yin_tone4[8];		//��������ƴ����(4��)
} SYLLABLEMAP;

typedef struct tagSYLLABLEV5MAP
{
	int con : 5;
	int vow : 6;
}SYLLABLEV5MAP;

typedef struct tagSPECIALSYLLABLE
{
	int old_syllable_count;
	SYLLABLE old_syllables[5];
	int new_syllable_count;
	SYLLABLE new_syllables[5];
}SPECIALSYLLABLE;

typedef struct tagNOREVERSESYLLABLE
{
	int count;
	SYLLABLE syllables[2];
}NOREVERSESYLLABLE;

//ģ����ѡ�TODO: ��Ҫ����ian <-> iang��uan <-> uang��
//�������򣺺���Ϊ�������ѵģ�ǰ��Ϊ�����
#define	FUZZY_Z_ZH					(1 << 0)	
#define	FUZZY_C_CH					(1 << 1)
#define	FUZZY_S_SH					(1 << 2)
#define	FUZZY_G_K					(1 << 3)
#define	FUZZY_L_N					(1 << 4)
#define	FUZZY_L_R					(1 << 5)
#define	FUZZY_F_H					(1 << 6)
#define	FUZZY_F_HU					(1 << 7)
#define	FUZZY_HUANG_WANG			(1 << 8)
#define	FUZZY_AN_ANG				(1 << 9)
#define	FUZZY_EN_ENG				(1 << 10)
#define	FUZZY_IN_ING				(1 << 11)

#define	FUZZY_REV_Z_ZH				(1 << 12)				//Ĭ��Ϊ0
#define	FUZZY_REV_C_CH				(1 << 13)				//Ĭ��Ϊ0
#define	FUZZY_REV_S_SH				(1 << 14)				//Ĭ��Ϊ0
#define	FUZZY_REV_G_K				(1 << 15)				//Ĭ��Ϊ0
#define	FUZZY_REV_L_N				(1 << 16)				//Ĭ��Ϊ0
#define	FUZZY_REV_L_R				(1 << 17)				//Ĭ��Ϊ0
#define	FUZZY_REV_F_H				(1 << 18)				//Ĭ��Ϊ0
#define	FUZZY_REV_F_HU				(1 << 19)				//Ĭ��Ϊ0
#define	FUZZY_REV_HUANG_WANG		(1 << 20)				//Ĭ��Ϊ0
#define	FUZZY_REV_AN_ANG			(1 << 21)				//Ĭ��Ϊ0
#define	FUZZY_REV_EN_ENG			(1 << 22)				//Ĭ��Ϊ0
#define	FUZZY_REV_IN_ING			(1 << 23)				//Ĭ��Ϊ0

#define	FUZZY_ZCS_IN_CI				(1 << 24)				//Ĭ��Ϊ0������ʱ�������������û�����
#define	FUZZY_SUPER					(1 << 25)				//������ĸģ��
#define	FUZZY_CI_SYLLABLE_LENGTH	(1 << 26)				//ģ���ʳ��������ڳ���

//ƴ������ѡ��
//#define CORRECT_GN_NG               (1 << 0)
//#define CORRECT_MG_NG               (1 << 1)
//#define CORRECT_IOU_IU              (1 << 2)
//#define CORRECT_UEI_UI              (1 << 3)
//#define CORRECT_UEN_UN              (1 << 4)
//#define CORRECT_ON_ONG              (1 << 5)

//ƴ��������ת�������������У����ڶ��ּ���
//extern SYLLABLEMAP syllable_map[];
//extern SYLLABLEV5MAP syllablev5_map[];

struct tagPIMCONTEXT;

//ƴ��������ת������
//extern const int syllable_map_items;
//extern const int syllablev5_map_items;

//��ʹ�������ıȽϣ��жϵ�һ�����ڲ�����ָʾ�����ڼ����Ƿ�����ڶ������ڲ�����
extern int ContainSyllable(SYLLABLE syllable, SYLLABLE checked_syllable, int fuzzy_mode);

//ʹ�������ıȽϣ��жϵ�һ�����ڲ�����ָʾ�����ڼ����Ƿ�����ڶ������ڲ�����
extern int ContainSyllableWithTone(SYLLABLE syllable, SYLLABLE checked_syllable, int fuzzy_mode);

//���ƴ�������ڱ�ʶ��
extern int GetSyllable(const TCHAR *pin_yin, SYLLABLE *syllable, int *syllable_str_length, int fuzzy_mode/*, int *correct_flag, int *separator_flag*/);

//��õ������ڲ�ֳɵ�����С����
extern int GetSmallSyllables(SYLLABLE syllable, SYLLABLE *small_syllables);

//����ƴ�������������顣
extern int ParsePinYinString(const TCHAR *pin_yin, SYLLABLE *syllables, int array_length, int fuzzy_mode/*, int *correct_flag, int *separator_flag*/);

//ͨ������ֵ���������ȡ�����ƴ��������ʽ
extern int GetSmallSyllablesParse(SYLLABLE *syllables, int syllable_count, SYLLABLE *other_arrays, int *other_lengths);

//�������ƴ�������������顣
extern int ParsePinYinStringReverse(const TCHAR *pin_yin, SYLLABLE *syllables, int array_length, int fuzzy_mode, int pinyin_mode/*, int *correct_flag, int *separator_flag*/);
//��ȡ�����ƴ��������ʽ
extern int GetOtherSyllableParse(struct tagPIMCONTEXT *context, int syllable_start, int syllable_count, SYLLABLE *other_syllables, int other_length);

//�ж���������ĸ�Ƿ��������ĸ��ͬ�����ڵ���ȡ�ʡ�
extern int SyllableStartWithLetter(TCHAR ch, SYLLABLE syllable);

//�жϵ�һ�����ڲ�����ָʾ����ĸ�����Ƿ�����ڶ������ڲ�������ĸ��
extern int ContainCon(SYLLABLE syllable, SYLLABLE checked_syllable, int fuzzy_mode);

//�жϵ�һ�����ڲ�����ָʾ����ĸ�����Ƿ�����ڶ������ڲ�������ĸ��
extern int ContainVow(SYLLABLE syllable, SYLLABLE checked_syllable, int fuzzy_mode);

//�Ƚ�һ�����ڣ�������ͨ����ıȽϣ������������Ƚϡ�
extern int CompareSyllables(SYLLABLE *syllables, SYLLABLE *checked_syllables, int length, int fuzzy_mode);

//�Ƚ�һ�����ڣ�����ͨ����ıȽϣ������������Ƚϡ�
extern int WildCompareSyllables(SYLLABLE *syllables, int length, SYLLABLE *checked_syllables, int checked_length, int fuzzy_mode);
extern int WildCompareSyllablesWithCi(SYLLABLE *syllables, int length, SYLLABLE *checked_syllabe, HZ *ci, int checked_length, int fuzzy_mode);

//�Ƚ�һ�����ڵ�����ĸ�Ƿ���Ŀ�������������ͨ����ıȽϣ������������Ƚϡ�
extern int CompareSyllablesAndLetters(const TCHAR *letters, SYLLABLE *checked_syllables, int length);
//�Ƚ�һ�����ڵ�����ĸ�Ƿ���Ŀ�����������ͨ����ıȽϡ�
extern int WildCompareSyllablesAndLetters(const TCHAR *letters, int letter_length, SYLLABLE *checked_syllables, int checked_length);

//�ж��Ƿ�Ϊ�������ַ�
extern int IsToneChar(char ch);
//�ж��Ƿ�Ϊ���������ڴ�
extern int IsFullSyllable(SYLLABLE *syllable_array, int syllable_count);
//�ж��Ƿ�������ڷָ���
extern int HasSyllableSeparator(const TCHAR *str);

//�ж����������Ƿ���ȫ��ͬ
extern int SameSyllable(SYLLABLE s1, SYLLABLE s2);

//������ڵ�ƴ����
extern int GetSyllableString(SYLLABLE syllable, TCHAR *str, int length,/* int correct_flag,*/ int tone_char);
//�õ�ȫƴ���ַ�����ƴ��ȫ����
//extern void GetAllSyllableStringQP(struct tagPIMCONTEXT *context, TCHAR *pinyin, unsigned int pinyin_len);
extern int GetSyllableStringSP(SYLLABLE syllable, TCHAR *str, int length);
extern void GetSPHintString(struct tagPIMCONTEXT *context, TCHAR *hint_buffer, int buffer_length);

//˫ƴ��Դ����
extern int LoadSPIniData(const TCHAR *ini_name);
extern int FreeSPIniData();

extern int GetLegalPinYinLength(const TCHAR *pin_yin, int state, int english_state);

#ifdef __cplusplus
}
#endif

#endif