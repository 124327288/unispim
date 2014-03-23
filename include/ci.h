/*	�ʴ���ͷ�ļ�
 */

#ifndef	_CI_H_
#define	_CI_H_

#ifdef __cplusplus
extern "C"
{
#endif

//��Cache���壬һ��һ����ռ��5-7���ֽ�
//256K�ռ䣬���Է���256000/5 ~= 51200���ʻ㣬Ӧ�ÿ������ˡ�
#define	CI_CACHE_SIZE				0x80000				//��cache��size
#define	CI_MOVE_SIZE				(CI_CACHE_SIZE / 4)	//Cache������ƶ�����
#define	CI_MAX_USED_COUNT			0x700000			//�ʵ����ʹ�ö�
#define CI_TOP_USED_COUNT			0x5FFFFF			//�ö��ʵ�ʹ�ö�
#define	CI_CACHE_EXTRA_LENGTH		0x100				//��Cache���ٽ绺��������
#define CI_CACHE_V66_SIGNATURE		0x20000919			//�ʻ������ı�ʶ�����ڰ汾�жϣ�
#define	CI_CACHE_SIGNATURE			0x20000918			//�ʻ������ı�ʶ�����ڰ汾�жϣ�
#define	CI_NEW_BUFFER_LENGTH		0x8000				//32K���´ʻ�����
#define	CI_NEW_EXTRA_LENGTH			0x100				//�´ʵĻ�����

//��Cache˵����
// B0			���ʵĳ���
// B1 B2 B3		���ʵ�ʹ�ö�
// ����....		����������
typedef struct tagCICACHE
{
	int		signature;				//�ʻ�������ʶ
	int		length;					//cache��ʹ�ó���
	int		max_used_id;			//�ʵ����ʹ�ô����������ʱ����Ҫ���м�������
	char	cache[CI_CACHE_SIZE + CI_CACHE_EXTRA_LENGTH];		//��cache
}CICACHE;

//�´ʼ�¼�ṹ
//B0�ʳ��ȣ�B1���ڳ���
//B2 B3...  ��
//.....		����
//typedef struct tagNEWCI
//{
//	int		length;
//	char	data[CI_NEW_BUFFER_LENGTH + CI_NEW_EXTRA_LENGTH];
//}NEWCI;

//#define	NEWCI_FILE_NAME				TEXT("unispim6\\wordlib\\new_ci.dat")
#define	CICACHE_FILE_NAME			TEXT("unispim6\\wordlib\\ci_cache.dat")

//����û��ʿ�Ĵʺ�ѡ
int GetUserCiCandidates(SYLLABLE *syllable_array, int syllable_count, CANDIDATE *candidate_array, int candidate_length, int fuzzy_mode);

//ͨ�����ڻ�ôʺ�ѡ
int GetCiCandidates(int wordlib_id, SYLLABLE *syllable_array, int syllable_count, CANDIDATE *candidate_array, int candidate_length, int fuzzy_mode);

//ͨ������ĸ��ôʻ��ѡ��
int GetCiCandidatesByLetters(int wordlib_id, const char *letters, int letters_count, CANDIDATE *candidate_array, int candidate_length);

//����ʻ㱻�û�ѡ�к���������뵽Cache�У������û�ʹ�öȡ�
extern void ProcessCiSelected(SYLLABLE *syllable, int syllable_length, HZ *hz, int hz_length);

//�ʺ�ѡ���ء�
extern int UnifyCiCandidates(CANDIDATE *candidates, int count);

//�����´�
extern int AddCi(SYLLABLE *syllable, int syllable_length, HZ *ci, int hz_length);

//�ʺ�ѡ����
extern void SortCiCandidates(CANDIDATE *candidates, int count);

//�������ڴ���ʵĺ�ѡ��
extern int ProcessCiCandidate(SYLLABLE *syllable_array, int syllable_count, const TCHAR *letters, CANDIDATE *candidate_array, int candidate_length, int same_ci_syllable_length);

//��ȡ��Ͻ�����ѡ��
extern int GetMixedParseCiCandidate(SYLLABLE *syllable_array, int syllable_count, CANDIDATE *candidate_array, int candidate_length);

//��ôʵ�cache��Ϣ
extern int GetCiCacheInfo(HZ *hz, int length);
extern int GetCiUsedCount(int cache_pos);
extern void InsertCiToCache(HZ *hz, int length, int syllable_length, int set_fix_top);

//װ�ء������Cache����
extern int LoadCiCacheData(const TCHAR *ci_cache_file_name);
extern int SaveCiCacheData(const TCHAR *ci_cache_file_name);
extern int FreeCiCacheData();
//extern int LoadNewWordTable();
//extern int SaveNewWordTable();
//extern void FreeNewWordTable();

extern int UpgradeWordLibFromV5ToV6(const TCHAR *);
extern int CheckAndUpgradeWordLibFromV5BKToV6(const char *);
extern WORDLIBITEMV6B1 *GetNextCiItemV6B1(WORDLIBITEMV6B1 *item);
extern WORDLIBITEM *GetNextCiItem(WORDLIBITEM *item);
extern HZ *GetItemHZPtr(WORDLIBITEM *item);
extern int GetItemLength(int hz_length, int syllable_length);

//����Ƿ�Ϊ�´ʣ������뵽�´ʱ���
//extern void CheckNewUserWord(SYLLABLE *syllables, int syllable_length, HZ *ci_string, int ci_length);
//extern void DeleteNewWord(HZ *ci_str, int ci_length, SYLLABLE *syllable, int syllable_length);
//extern int  __stdcall GetNewestUserWords(char *buffer, int buffer_length);
//extern void __stdcall DelNewestUserWords(const char *buffer);
extern int TestNewWordTable();

#ifdef __cplusplus
}
#endif

#endif