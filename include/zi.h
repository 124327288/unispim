/*	���ִ���ͷ�ļ���
 */

#ifndef	_ZI_H_
#define	_ZI_H_

#ifdef __cplusplus
extern "C" {
#endif
#include <kernel.h>
#include <context.h>

#define	MAX_ZI_FREQ			((1 << 23) - 1)						//�����Ƶ
#define	BASE_ZI_FREQ		(8000000)							//��Ƶ��׼

#define	ZICACHE_FILE_NAME	TEXT("unispim6\\zi\\hz_cache.dat")	//����Cache�ļ�����
#define	HZDATA_FILE_NAME	TEXT("unispim6\\zi\\hzpy.dat")		//���������ļ�����
#define HZBH_FILE_NAME		TEXT("unispim6\\zi\\hzbh.dat")		//���ֱʻ������ļ�����
#define	TOPZI_FILE_NAME		TEXT("unispim6\\ini\\�̶���.ini")	//�ö��ֶ����ļ�����
#define	HZJF_FILE_NAME		TEXT("unispim6\\zi\\hzjf.dat")		//���ּ򷱶��ձ������ļ�����
#define	ZICACHE_SIGNATURE	0x20000918							//�ֻ������ı�־

#define _CanInLibrary(x)	((x >= 0x00080 && x <= 0x0D7FF) || (x >= 0x0E000 && x <= 0x0FFEF) || \
							 (x >= 0x20000 && x <= 0x2A6D6) || (x >= 0x2A700 && x <= 0x2B734) || \
							 (x >= 0x2F800 && x <= 0x2FA1D))

//#define IsHanZiInExtA(x)	(x >= 0x3400  && x <= 0x4DB5)
//#define IsHanZiInExtB(x)	(x >= 0x20000 && x <= 0x2A6D6)
//#define IsHanZiInExtC(x)	(x >= 0x2A700 && x <= 0x2B73F)

typedef struct tagHZCACHE
{
	int		signature;							//�ֻ������ı�־ZICACHE_SIGNATURE�����ڰ汾�жϣ�
	int		max_id;								//��ǰ���ĺ���cache���(���Cacheδ��ʱ����Cache���ֵĸ���))
	int		cache[MAX_HZ_IN_PIM];				//ZiCache�Ƚ϶��أ���CiCache��ͬ���ļ��ﲢ���洢���֣�cache[i]��i���ֿ���HZITEM��hz_id����cache[i]��ֵ�Ǹ��ּ���Cacheʱ��˳��ţ����AddHzToCache
	int		used_count[MAX_HZ_IN_PIM];			//�û�����ʹ�öȱ���cache��Ա���ƣ�used_count[i]��i���ֿ���HZITEM��hz_id����used_count[i]��ֵ�Ǹ��ֵ�ʹ�ö�
}HZCACHE;

//�ö��ֶ����
typedef struct tagTOPZITABLE
{
	SYLLABLE	syllable;						//����
	HZ			top_zi[MAX_TOPZI];				//�ö��ָ���
}TOPZIITEM;

//�ʻ���Ϣ��
typedef struct tagBHITEM
{
	UC		zi;									//����
	byte	count;								//һ�����м��ױ�˳����
	byte	order;								//��˳��������ţ���1��ʼ����
	byte	custom;								//�Ƿ��Զ����˳��0��ʾ�淶д����1��ʾ���ݱ�˳
	byte	length;								//�ʻ���Ŀ�����ʻ��ַ�([1-5])���ĳ���
	int  	bh;									//��ǰ��˳�������ַ���ָ��
	int     next;                               // �����ֵ�������˳�������ݵ�ַ
                                                // ppNextΪ�գ���ʾ���ṹ�Ѿ��Ǳ����ֵ�����˳����
												// ���ṹ�Ĵ洢���ձʻ������򣬶�һ�����ֵĶ��ֱ�˳
												// �����ȿ��ܲ�ͬ����ô���ڴ洢���оͲ��ܱ�֤������
												// ������ŵģ����������ָ���γ���ʽ�ṹ
}BHITEM;

//bh file header struct
typedef struct headBHFile
{
	int itemcount;                              //item amount,���ֱ�˳��Ŀ��(һ�����ֿ��ж��ֱ�˳����)
	int charcount;                              //��ͬ���ֵ���Ŀ
	UC  minmcp;                                 // ���ļ�������˳���ݵĺ����У�Unicode��������Сֵ
	UC  maxmcp;                                 // ���ļ�������˳���ݵĺ����У�Unicode���������ֵ
                                                // ���ձ��������������ȼ��㷽����������ʾ
                                                // ( MaxUCP - MinUCP + 1 ) * SizeOf( TStrokeItem )
	int maxstrockes;                            // ���ʻ����������˳���ĳ���
                                                // ���ʻ�����������ʼ��ַ�����ȵļ��㷽����������ʾ
                                                // ( MaxStrokes + 1 ) * SizeOf( PStrokeItem )
	int createdate;                             //��������(��Ϊ0)
	int modifydate;                             //�޸�����(��Ϊ0)
	int checksum;                               //У���(��Ϊ0)
	int index1[1];                              //index1 = maxstrockes
	//int index2[1];							//index2 = maxmcp - minmcp + 1
	//BHITEM data[1];                           //����   = itemcount
}BHDATA;

//�򷱶�����
typedef struct tagJFITEM
{
	UC		JianTi;								//���庺��
	UC		FanTi;								//���庺��
}JFITEM;

//������Ϣ��
extern HZDATAHEADER	*hz_data;

//�ж��Ƿ�Ϊ����
extern int IsAllCanInLibrary(HZ *hz, int length);

//�����ֱ�ѡ������Cache�Լ���Ƶ�ĸ���
extern void ProcessZiSelected(HZITEM *item);

//�����Դʶ��ַ�ʽѡ����֮��ĸ���
extern void ProcessZiSelectedByWord(HZ hz, SYLLABLE syllable);

//��ú�����Cache�е�ʹ�ñ�ʶ��Խ��Խ��ʹ�ã�
extern int GetZiCachePos(HZITEM *item);

//�������ڵ����ֺ�ѡ��һϵ�й���
extern int ProcessZiCandidates(SYLLABLE syllable, CANDIDATE *candidate_array, int array_length, int zi_level);

//�ں��ֺ�ѡ������
extern int UnifyZiCandidates(CANDIDATE *candidate_array, int count);

//��С���ڴʺ�ѡ������
extern int UnifySmallCiCandidates(CANDIDATE *candidate_array, int count);

//�Ժ��ֺ�ѡ��������
extern void SortZiCandidates(CANDIDATE *candidate_array, int count);

//���ں��ֵ��������ּ�������ȫ���еĺ�����
extern HZITEM *GetHzItemBySyllable(SYLLABLE syllable, HZ hz);

//��ú��ֺ�ѡ��
extern int GetZiCandidates(SYLLABLE syllable, CANDIDATE *candidate_array, int array_length, int fuzzy_mode, int set_mode, int output_mode);

//�жϺ����Ƿ�����ض���������
extern int ZiContainTone(HZ hz, SYLLABLE syllable, int tone);

//װ�ء��ͷ��ö����ļ�
extern int LoadTopZiData(const TCHAR *name);
extern int FreeTopZiData();

//װ�ء����桢�ͷź���Cache����
extern int LoadHZData(const TCHAR *hz_data_name);
extern int FreeHZData();
extern int LoadZiCacheData(const TCHAR *zi_cache_name);
extern int FreeZiCacheData(const TCHAR *zi_cache_name);
extern int SaveZiCacheData(const TCHAR *zi_cache_name);

//�ж��Ƿ�ΪBG����
extern int IsGB(HZ hz);
extern int AdjustWordFreqToHZFreq(int ci_freq);
extern int ConvertToRealHZFreq(int zi_freq);
extern int ConvertToRealCIFreq(int ci_freq);

//��ú��ֱʻ�ƴ����
extern void GetZiBHPinyin(UC zi, TCHAR *buffer, int length);
//��ñʻ��ĺ�ѡ��ʾ
extern void GetBHDisplayString(CANDIDATE *candidate, TCHAR *buffer, int length);

//��ñʻ�����ĺ�ѡ
extern int GetBHCandidates(const TCHAR *input_string, CANDIDATE *candidates, int array_length);
//ģ��ģʽ
//extern int GetBHCandidatesFuzzy(const char *input_string, CANDIDATE *candidates, int array_length);

//�趨Ϊ�̶���
extern void SetFixTopZi(PIMCONTEXT *context, int index);

//װ�ء��ͷűʻ������ļ�
extern int LoadBHData(const TCHAR *file_name);
extern int FreeBHData();

extern HZITEM* GetSingleZiCandidate(TCHAR zi);

#ifdef __cplusplus
}
#endif

#endif