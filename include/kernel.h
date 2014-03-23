/*	���뷨����ģ�顣������뷨����ĵĹ��ܡ�
 *
 *	1��д��������
 *	2����ѡ���ɰ����û�����������
 *
 *	Kernel��һ����״̬�ĺ����飬ֻ�����������봮���д��������¼�ϴ�
 *	������״̬��
 *	Kernel��������ǻ��ڵ�ǰ�����룬����д�����Լ���ѡ�������Ĳ�����
 *	IMEģ����ɣ��磺��������������ѡ��ȡ�
 */

#ifndef	_KERNEL_H_
#define	_KERNEL_H_

//��������
typedef	unsigned short	HZ;					//��׼���ֱ���
typedef unsigned int	UC;					//Unicode����
typedef unsigned short	BIG5;				//̨�庺�ֱ���

#include <platform.h>
#include <syllable.h>
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

extern int	window_logon;

#define		TORGB(x)		((((x) & 0xff0000) >> 16) | (((x) & 0xff) << 16) | ((x) & 0x00ff00))

#define		WORDLIB_WRONG				0							//�ʿ�ǻ���ʿ�
#define		WORDLIB_V3					3							//�ʿ�ΪV3�ʿ�
#define		WORDLIB_V5					5							//�ʿ�ΪV3��V5�ʿ�
#define		WORDLIB_V6					6							//�ʿ�ΪV6�ʿ�
#define		WORDLIB_V6B1				7							//�ʿ�ΪV6B1�ʿ�
#define		WORDLIB_V3_BKUP				8							//V3�ı��ݴʿ��ļ�
#define		WORDLIB_V5_BKUP				9							//V5�ı��ݴʿ��ļ�
#define		WORDLIB_V6B2				10							//�ʿ�ΪV6B2�ʿ�
#define		WORDLIB_V66					66							//�ʿ�ΪV6.6�ʿ�

#define		HYPIM_VERSION					6						//���뷨��汾��
#define		HYPIM_WORDLIB_V66_SIGNATURE		0x14091994				//���뷨�ʿ�V66��־
#define		HYPIM_WORDLIB_V6_SIGNATURE		0x14081994				//���뷨�ʿ�V6��־
#define		HYPIM_WORDLIB_V6B2_SIGNATURE	0x14071994				//���뷨�ʿ�V6B2��־
#define		HYPIM_WORDLIB_V6B1_SIGNATURE	0x14061994				//���뷨�ʿ�V6B1��־
#define		HYPIM_WORDLIB_V5_SIGNATURE		0x19990604				//���뷨�ʿ�V5��־
#define		HYPIM_WORDLIB_V3_SIGNATURE		0x19990604				//���뷨�ʿ�V3��־
#define		HYPIM_SHARED_SEGMENT			".upim_shm"				//ȫ�ֹ���ε�����

//��ѡ���Ͷ���
#define		CAND_TYPE_ICW			1								//������ʺ�ѡ
#define		CAND_TYPE_SPW			2								//�Զ���ʺ�ѡ
#define		CAND_TYPE_CI			3								//�ʺ�ѡ
#define		CAND_TYPE_ZI			4								//�ֺ�ѡ
#define		CAND_TYPE_ZFW			5								//�Դʶ��ֺ�ѡ
//#define     CAND_TYPE_URL           6           					//url candidate

#define     ZI_TYPE_NORMAL          1                               //������������(��С���ڲ��)
#define     ZI_TYPE_OTHER           2                               //С���ڲ�ֵ��֣���xian�е�xi

#define		CI_TYPE_NORMAL			1								//�����Ĵʻ�����(�����������)
#define		CI_TYPE_LETTER			2								//����ĸ����(������ƴ)
#define		CI_TYPE_OTHER			3								//�����������
#define		CI_TYPE_WILDCARD		4								//������������

//��ͬ���ĺ�ѡ�ĳ��ȶ��壨Ĭ���Ժ���Ϊ��λ��
#define		MAX_HZ_IN_PIM			65536							//�����뷨�еĺ�����Ŀ���ֵ
#define		MAX_TOPZI				8								//����ö��ָ���
#define		MAX_PINYIN_LENGTH		8								//���ƴ��������
#define		MAX_CANDIDATES			3000							//���ĺ�ѡ�����
#define		MAX_ICW_CANDIDATES		10								//����������ʸ���
#define		MAX_CANDIDATES_PER_LINE	9								//һ������ѡ��Ŀ
#define		MAX_CANDIDATE_LINES		5								//����ѡ����

#define		MAX_CANDIDATE_STRING_LENGTH	32							//����ѡ�ַ����ĳ��ȣ�������ʾ��
#define		MAX_TRANSLATE_STRING_LENGTH	200							//���Ӣ�ķ����ַ����ĳ���

#define		MAX_SYLLABLE_PER_INPUT	32								//һ���ܹ������������Ŀ
#define		MAX_INPUT_LENGTH		64								//����û������ַ�����
#define		MAX_COMPOSE_LENGTH		128								//���д��������
#define		MAX_RESULT_LENGTH		2048							//�����������
#define		MAX_WORD_LENGTH			32								//�ʵ��������Ŀ
#define		MAX_ICW_LENGTH			16								//������ʵ��������Ŀ
#define		MAX_SPW_LENGTH			2000							//����ʻ����󳤶ȣ���CharΪ��λ��
#define		MAX_SPW_HINT_STRING		64								//SPW��ʾ�ַ�������
#define		MAX_RECENT_LENGTH		16								//�������Ĵ���
#define     MAX_OTHER_PARSE_SYLLABLE  32                            //����������������(����GetCandidates::GetOtherSyllableParse)
#define     MAX_SMALL_SYLLABLES       4                             //һ��ƴ�������ɽ�����С����(���ֵ����ڲ�ֳɵ����ڣ���xian->xi'an)��
                                                                    //�����ֵ��Ӧ����MAX_SYLLABLE_PER_INPUT/2(�����ֺ�����������)

//����ʹ�öȱ�ʶ
#define		HZ_ALL_USED				(HZ_MOST_USED | HZ_MORE_USED | HZ_LESS_USED)	//ȫ������
#define		HZ_MOST_USED			(1 << 0)										//������
#define		HZ_MORE_USED			(1 << 1)										//�γ�����
#define		HZ_LESS_USED			(1 << 2)										//�ǳ�����

//����Cache����
#define		HZ_CACHE_MAX_ID			0x70000000	//���ĺ���cache��־��Ϊ������ַ��Ŵ��󣬲��趨Ϊ7fffffff

#pragma		pack(1)

//����������
typedef struct tagHZITEM
{
	UC				hz;							//��(UNICODE)
	unsigned short	hz_id;						//������Ŀ��ID
	SYLLABLE		syllable;					//����
	int				freq;						//��Ƶ
	int				simplified : 1;				//����
	int				traditional : 1;			//����
	int				other : 1;					//�Ƿ���/������֣�һ��Ϊ�ձ�/���ĺ��֣�
	int				effective : 1;				//��Ч(֧�ֽ�����Ŀ�仯)
	int				show_syllable : 1;			//����(�ں�ѡ������ʾƴ��)
	int				other_data : 26;			//��������(��ʱ�հ�)
	int				icw_hz : 1;					//ICW��(���ִ�)
}HZITEM;

//������Ϣ�ļ��ṹ(hzpy.dat)
typedef struct tagHZDATAHEADER
{
	unsigned int	signature;					//��ʶ
	unsigned int	create_date;				//��������
	unsigned int	modify_date;				//�޸�����
	unsigned int	check_sum;					//У���
	int				hz_count;					//������Ŀ

	HZITEM			hz_item[1];						//������
//	char			reserved[(MAX_HZ_IN_PIM - 1) * sizeof(HZITEM) - sizeof(int) * 5];	//������
}HZDATAHEADER;

//��������
typedef struct tagWORDITEM
{
	unsigned int	effective : 1;				//��Ч��û�б�ɾ����
	unsigned int	ci_length : 6;				//�ʳ���
	unsigned int	syllable_length : 6;		//���ڳ���
	unsigned int	freq : 19;					//��Ƶ��Q: ��Ƶ��ʹ�ö���ʲô����A: ��Ƶ�Ǵʿ���ģ�ʹ�ö���CiCache��ģ���Ƶ��ʹ�öȵļ��㷽ʽ����ʹ��1���ۼ�1
												//(��AddCi��InsertCiToCache)����ʵ׼ȷ��˵����װ�����뷨��ʼʹ���Ժ�ʹ�öȲ���ʹ��1�μ�1�����ʿ��еĴ�Ʒ�ǲ���ģ�
												//(�û��ʿ����)������Ĵʿ��еĴ�Ƶ��Ϣ������ԭʼ���Ͽ��еĴ�Ƶ����õ���(ͨ������0��ʹ�öȵĳ�ʼֵ����0)�����㹫
												//ʽ��AdjustWordFreqToHZFreq))

	//��������Ϊ�䳤�ṹ����length���
	SYLLABLE	syllable[1];					//����, C++Ҫ�����������size
//	HZ			hz[length];						//����
}WORDLIBITEM;

//��������
typedef struct tagWORDITEMV6B2
{
	unsigned int	effective : 1;				//��Ч��û�б�ɾ����
	unsigned int	sys_word : 1;				//ϵͳ����
	unsigned int	ci_length : 5;				//�ʳ���
	unsigned int	syllable_length : 5;		//���ڳ���
	unsigned int	freq : 20;					//��Ƶ
	//��������Ϊ�䳤�ṹ����length���
	SYLLABLE	syllable[1];					//����, C++Ҫ�����������size
//	HZ			hz[length];						//����
}WORDLIBITEMV6B2;

//��������
typedef struct tagWORDITEMV6B1
{
	unsigned int	effective : 1;				//��Ч��û�б�ɾ����
	unsigned int	sys_word : 1;				//ϵͳ�ʻ�
	unsigned int	length : 7;					//�ʻ㳤��
	unsigned int	freq : 20;					//��Ƶ
	//��������Ϊ�䳤�ṹ����length���
	SYLLABLE	syllable[1];					//����, C++Ҫ�����������size
//	HZ			hz[length];						//����
}WORDLIBITEMV6B1;

//�ֺ�ѡ����
typedef struct tagHZCANDIDATE
{
	WORDLIBITEM	*word_item;					//����ָ��
	HZITEM		*item;						//���ֱ��еĺ�����
	int         hz_type;                    //��ѡ������
	int			top_pos;					//�̶��ֱ�ʶ��0���ǹ̶��֣�1-7Ϊ�̶���λ�á�
	int			cache_pos;					//��CACHE�е�λ�ã������ж���������ĺ���
	int			is_word;					//�Ƿ�Ϊ��(�磺����)
	SYLLABLE    origin_syllable;            //С���ڴʶ�Ӧ��ԭʼ���ڣ���xi'an��ԭʼ������xian
}HZCANDIDATE;

//�ʺ�ѡ����
typedef struct tagWORDCANDIDATE
{
	WORDLIBITEM		*item;					//�ʿ��д���ָ��
	int				type;					//����
	int				source;					//�ʻ���Դ�ʿ��ʶ
	int				cache_pos;				//��������cache�е�λ��
	SYLLABLE		*syllable;				//ָ��ʿ������ָ��
	HZ				*hz;					//ָ��������ֵ�ָ��
	int				origin_syllable_length;	//ԭʼ���ڳ���(����˫�������ɵ����ڳ��Ȳ�����)
}WORDCANDIDATE;

//�Դʶ��ֺ�ѡ
typedef struct tagZIFROMWORDCANDIDATE
{
	WORDCANDIDATE	*word;					//ԭ���Ĵ�
	SYLLABLE		syllable;				//����
	HZ				hz;						//����
}ZFWCANDIDATE;


//�������������
typedef struct tagICWCANDIDATE
{
	char		length;						//ICW�ĳ��ȣ��Ժ���Ϊ��λ��
	SYLLABLE	syllable[MAX_ICW_LENGTH];	//ICW���������飬�����Դʶ��ֵ���Ƶ�����Լ�����ʵ��ʿ⡣
	HZ			hz[MAX_ICW_LENGTH + 1];		//ICW�ĺ�������
}ICWCANDIDATE;

//�Զ����������
typedef struct tagSPWCANDIDATE
{
	int			length;						//����
	const void	*string;					//�����ָ��
	const void	*hint;						//��ʾ��Ϣ
	int			type;						//���������
	UC			hz;							//�ʻ�ʹ�õĺ���
}SPWCANDIDATE;

//url candidate
//typedef struct tagURLCANDIDATE
//{
//	int			length;						//����
//	const void	*string;					//�����ָ��
//	const void	*hint;						//��ʾ��Ϣ
//}URLCANDIDATE;

//��ѡ��ṹ
typedef struct tagCANDIDATE
{
	char		type;						//��ѡ���
	union
	{
		ICWCANDIDATE		icw;			//������ʺ�ѡ
		SPWCANDIDATE		spw;			//�Զ���ʺ�ѡ
		WORDCANDIDATE		word;			//�ʺ�ѡ
		HZCANDIDATE			hz;				//���ֺ�ѡ
		ZFWCANDIDATE		zfw;			//�Դʶ��ֺ�ѡ
		//URLCANDIDATE        url;            //url candidate
	};
}CANDIDATE;

#pragma	pack()

struct tagPIMCONTEXT;

//��ȡ��ѡ
extern int GetCandidates(struct tagPIMCONTEXT *context, const TCHAR *input_string, SYLLABLE *syllables, int syllable_count, CANDIDATE *candidate_array, int array_length, int need_spw);

#ifdef __cplusplus
}
#endif

#endif
