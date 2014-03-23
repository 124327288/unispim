/* �ʿ⹤��ͷ�ļ�
 */

#ifndef	_WORDLIB_H_
#define	_WORDLIB_H_

#include <kernel.h>
#include <syllable.h>

#ifdef __cplusplus
extern "C" {
#endif

//extern int user_wl_modified;
//extern int can_save_user_wordlib;

#define	WORDLIB_MAX_FREQ				((1 << 19) - 1)				//�ʿ��е�����Ƶ
#define	BASE_CI_FREQ					(500000)					//��Ƶ��׼
#define	USER_BASE_FREQ					100000						//�û��Զ��ʵ�Ĭ�ϴ�Ƶ

//�ʿ���ض���
#define	DEFAULT_USER_WORDLIB_NAME	TEXT("�û��ʿ�")
#define	DEFAULT_USER_WORDLIB_AUTHOR	TEXT("����ƴ�����뷨")
#define	J2F_FILE_NAME				TEXT("\\unispim6\\zi\\j2f.dat")		//���己��ת�������ļ�����
#define	WORDLIB_NAME_PREFIX			TEXT("\\unispim6\\wordlib\\")		//�ʿ��ļ���ǰ׺
#define	WORDLIB_USER_SHORT_NAME		TEXT("user.uwl")					//�û��ʿ��ļ��Ķ�����
#define WORDLIB_SYS_FILE_NAME		TEXT("unispim6\\wordlib\\sys.uwl")	//ϵͳ�ʿ��ļ���
#define WORDLIB_V3V5_FILE_NAME		TEXT("unispim6\\wordlib\\v3v5.uwl") //V3V5����ʿ��ļ���
#define WORDLIB_V3V5_NAME			TEXT("V3V5�ʿ�")					//V3V5����ʿ�����

#define MAX_WORDLIBS					32							//�ڴ������Ĵʿ���Ŀ
#define	WORDLIB_PAGE_SIZE				1024						//�ʿ�ҳ��С
#define	WORDLIB_NAME_LENGTH				16							//�ʿ����Ƴ���
#define	WORDLIB_AUTHOR_LENGTH			16							//�ʿ���������
#define	PAGE_END						-1							//����ҳ
#define	WORDLIB_EXTRA_LENGTH			0x100000					//�û��ʿ������ռ�
#define	WORDLIB_PAGE_DATA_LENGTH		(WORDLIB_PAGE_SIZE - 4 * sizeof(int))	//ҳ�п������ݳ���
#define	WORDLIB_FILE_NAME_LENGTH		256							//�ʿ��ļ����Ƶ���󳤶�
#define	WORDLIB_FEATURE_LENGTH			sizeof(int)					//�����е������������ȣ�Ŀǰ��һ������
#define	WORDLIB_CREATE_EXTRA_LENGTH		0x100000					//�����´ʿ�ʱ�����Ӷ����1M���ݡ�
#define	WORDLIB_NORMAL_EXTRA_LENGTH		0x40000						//��ͨ��������������Ӵʻ�

//�ʿ�ͷ����
typedef struct tagWORDLIBHEADER
{
	int			signature;							//�ʿ��ǩ��
	TCHAR		name[WORDLIB_NAME_LENGTH];			//�ʿ������
	TCHAR		author_name[WORDLIB_AUTHOR_LENGTH];	//�ʿ���������
	int			word_count;							//�ʻ���Ŀ
	int			page_count;							//�ѷ����ҳ��
	int			can_be_edit;						//�Ƿ���Ա༭
	int			pim_version;						//���뷨�汾�ţ������Կ��ǣ�
	int			index[CON_NUMBER][CON_NUMBER];		//������
} WORDLIBHEADER;

//ҳ����
typedef	struct tagPAGE
{
	int			page_no;							//ҳ��
	int			next_page_no;						//��һ��ҳ�ţ�-1��ʶ����
	int			length_flag;						//��ҳ�����Ĵʻ㳤�ȵı�־
	int			data_length;						//�Ѿ�ʹ�õ����ݳ���
	char		data[WORDLIB_PAGE_DATA_LENGTH];		//���ݿ�ʼ
} PAGE;

//�ʿⶨ��
typedef	struct tagWORDLIB
{	
	union
	{
		//�ʿ��ļ�ͷ����
		WORDLIBHEADER	header;	

		//���ڶ���ҳ�߽�
		PAGE			header_data[sizeof(WORDLIBHEADER) / WORDLIB_PAGE_SIZE + 1];
	};
	PAGE				pages[1];					//ҳ����
} WORDLIB;

//V5�ʿⶨ��
typedef struct tagWORDLIBV5
{
	int		magic_number;
	int		index_table[8][18][18];
	int		win_time;
	int		free_address;
}WORDLIBV5;

#define	WLUP_OPER_ADD		1				//���Ӵ���
#define	WLUP_OPER_DEL		2				//ɾ������
#define	WLUP_OPER_UPDATE	3				//���´���

typedef struct tagUPDATEITEM 
{
	const TCHAR *ci_string;					//���ַ���
	const TCHAR *py_string;					//ƴ���ַ���
	int freq;								//��Ƶ
	int operation;							//������OPER_ADD (1), OPER_DELETE(2), OPER_UPDATE(3),
}UPDATEITEM;

#define	ENCODERV5		0xfdef				//V5���ֵļ�������

#define	WLF_CONTINUE_ON_ERROR		(1 << 0)
#define	WLF_HALT_ON_ERROR			(1 << 1)

#define	WLF_CLEAR_WORDLIB			(1 << 2)
#define	WLF_DELETE_WORDLIB			(1 << 3)

#define	WLE_CI						(-1)
#define	WLE_YIN						(-2)
#define	WLE_FREQ					(-3)
#define	WLE_NAME					(-4)
#define	WLE_ITEMS					(-5)
#define	WLE_NOCI					(-6)
#define	WLE_OTHER					(-10)

//װ�شʿ��ļ���
extern int LoadWordLibrary(const TCHAR *wordlib_name);

//����ʿ��ļ�
extern int SaveWordLibrary(int wordlib_id);

//��ôʿ��ָ��
extern WORDLIB *GetWordLibrary(int wordlib_id);

//����ҳ��
extern int NewWordLibPage(int wordlib_id);

//��ʿ������Ӵ���
extern int AddCiToWordLibrary(int wordlib_id, HZ *hz, int hz_length, SYLLABLE *syllable, int syllable_length, int freq);

//ȷ�����Ƿ��ڴʿ���
extern WORDLIBITEM *GetCiInWordLibrary(int wordlib_id, HZ *hz, int hz_length, SYLLABLE *syllable, int syllable_length);

//�ڴʿ���ɾ���ʻ㡣
extern int DeleteCiFromWordLib(int wordlib_id, HZ *hz, int hz_length, SYLLABLE *syllable, int syllable_length, int clear_syllable);

//װ�شʿ��ļ���������������ݳ��ȣ�һ�����ڴ����ʿ��ļ�����װ���û��ʿ��ļ���
extern int LoadWordLibraryWithExtraLength(const TCHAR *lib_name, int extra_length, int check_exist);

//װ���û��ʿ��ļ�
extern int LoadUserWordLibrary(const TCHAR *wordlib_name);

//���ڴ��������ļ��������ʿ⡣
extern int CreateWordLibraryWithData(const char *wordlib_file_name, const char *data_file_name, const char *name, const char *author, int can_be_edit);

//�ͷŴʿ����ݡ�
extern void CloseWordLibrary(int wordlib_id);

//�ͷ����еĴʿ����ݡ�
extern void CloseAllWordLibrary();

//�����յĴʿ��ļ�
extern int CreateEmptyWordLibFile(const TCHAR *wordlib_file_name, const TCHAR *name, const TCHAR *author, int can_be_edit);

//�����һ���ʿ��ʶ�����ڴʿ�ı�����
int GetNextWordLibId(int cur_id);
const TCHAR *GetWordLibFileName(int wl_id);

//����û��ʿ��ʶ
int GetUserWordLibId();

//�ʿ�ά��������
int __stdcall UpdateWordLibrary(const TCHAR *wl_name, UPDATEITEM *update_items, int item_count, int flag);
int __stdcall CheckAndUpdateWordLibrary(const TCHAR *wl_name);
int CheckAndUpdateWordLibraryFast(const TCHAR *wl_name);
int ExportWordLibrary(const TCHAR *wordlib_file_name, const TCHAR *text_file_name, int *ci_count, TCHAR *err_file_name, int export_all, void *call_back);
int ImportWordLibrary(const TCHAR *wordlib_file_name, const TCHAR *text_file_name, int *ok_count, int *err_count, TCHAR *err_file_name, void *call_back);
int CreateWordLibrary(const TCHAR *wordlib_file_name, const TCHAR *text_file_name, int *ok_count, int *err_count, TCHAR *err_file_name, void *call_back);
int CheckAndUpgrandWordLibFromV5BKToV6(const TCHAR *wl_name);
int UpgradeWordLibFromV5ToV6(const TCHAR *wl_file_name);
int LoadAllWordLibraries();
void MaintainWordLibPointer();
int GetWordLibraryLoaded(const TCHAR *lib_name);

//Word J2F
void WordJ2F(TCHAR *zi_word);

//ɾ��
int __stdcall DeleteCiFromAllWordLib(TCHAR *ci_str, int ci_length, TCHAR *py_str, int py_length);

#ifdef __cplusplus
}
#endif

#endif