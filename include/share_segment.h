#ifndef	_SHARE_SEGMENT_H_
#define	_SHARE_SEGMENT_H_

#include <windows.h>
#include <config.h>
#include <ci.h>
#include <kernel.h>
#include <win32/main_window.h>
#include <spw.h>
#include <symbol.h>
#include <wordlib.h>
#include <zi.h>

#ifdef __cplusplus
extern "C" {
#endif

static TCHAR *segment_share_name = TEXT("HYPIM_SEGMENT_SHARED_NAME");

typedef struct tagSHARE_SEGMENT
{
	int	process_count;												//��IME���ӵĽ��̼���
	//int	first_use; 													//��һ��ʹ��IME

	int	is_default_ime;												//�Ƿ�ΪĬ�����뷨
	int global_config_update_time;									//����װ��ʱ��
	int config_loaded;												//�Ƿ��Ѿ�װ��Config

	PIMCONFIG all_config;											//ϵͳ����ʹ�õ�Config

	int q1_index;													//������
	int q2_index;													//˫����
	int symbol_loaded;												//�Ƿ��Ѿ�װ�ط��ű�
	SYMBOLITEM symbol_table[SYMBOL_NUMBER];							//���ű�Ĭ������

	int user_wordlib_id;											//�û��ʿ��ʶ
	int user_wl_modified;											//�Ƿ�ı�
	int can_save_user_wordlib;										//�Ƿ���Ա����û��ʿ⣨���û��ʿ�Import�ʻ���ܱ����ǣ�
	TCHAR wordlib_name[MAX_WORDLIBS * 2][WORDLIB_FILE_NAME_LENGTH];	//�ʿ������
	int	wordlib_length[MAX_WORDLIBS * 2];							//�ʿⳤ������
	int wordlib_deleted[MAX_WORDLIBS * 2];							//�ʿ��Ƿ��Ѿ���ɾ��
	TCHAR wordlib_shared_name[MAX_WORDLIBS * 2][0x20];				//�����ڴ�ı�ʶ

	int	bh_loaded;													//�ʻ��Ƿ��Ѿ����ڴ���

	CICACHE	ci_cache;												//��cache
	int	ci_cache_loaded;											//�Ƿ��Ѿ�װ��
	int	ci_cache_modified;											//�Ƿ�ı�

	//NEWCI new_ci;													//�´ʱ�
	//int	new_ci_loaded;												//�Ƿ�װ��
	//int	new_ci_modified;											//�Ƿ��Ѿ��޸�

	int english_loaded;												//Ӣ�Ĵʵ��Ƿ��Ѿ����ڴ���
	int engtrans_loaded;											//Ӣ�ķ����Ƿ��Ѿ����ڴ���

	TCHAR szRecentResult[MAX_RECENT_LENGTH][MAX_WORD_LENGTH + 1];	//�������Ĵ�
	int nCurRecent;													//���������һ����±�

	int	jf_loaded;													//�򷱶��ձ��Ƿ��Ѿ����ڴ���
	int	jf_count;													//�򷱶�������Ŀ

	unsigned __int64 main_window_list[MAX_STATUS_LIST_SIZE];		//д�����������
	unsigned __int64 status_window_list[MAX_STATUS_LIST_SIZE];		//״̬���������
	unsigned __int64 hint_window_list[MAX_STATUS_LIST_SIZE];		//��ʾ���������

	int	resource_loaded;											//��Դ��δװ��

	int	spw_index[SPW_MAX_ITEMS];									//����������
	int	spw_count;													//������Ŀ
	int	spw_length;													//���������ݳ���
	int	spw_loaded;													//�����Ƿ��Ѿ����ڴ���

	SYLLABLEMAP syllable_map[460];									//ƴ��������ת����
	int syllable_map_items;
	SYLLABLEV5MAP syllablev5_map[451];								//ƴ��������ת����(V5)
	int syllablev5_map_items;
	int sp_used_u;													//˫ƴ�Ƿ�ʹ��u��Ϊ��ͷ����ĸ
	int sp_used_v;													//˫ƴ�Ƿ�ʹ��V��Ϊ��ͷ����ĸ
	int sp_used_i;													//˫ƴ�Ƿ�ʹ��I��Ϊ��ͷ����ĸ
	int sp_loaded;													//˫ƴ�����Ƿ��Ѽ���

	TCHAR con_sp_string_save[26][8];								//˫ƴ��ĸ��
	TCHAR con_sp_string[26][8];
	TCHAR vow_sp_string_save[46][8];								//˫ƴ��ĸ��
	TCHAR vow_sp_string[46][8];
	TCHAR vow_sp_string_single_save[94][8];							//˫ƴ���ڱ�
	TCHAR vow_sp_string_single[94][8];

	//int pim_update_flag;											//�ʿ��Ƿ��Ѿ��������
	//int next_pim_update_time;										//��һ�εĴʿ���µ�ʱ��

	//int url_loaded;													//url�Ƿ��Ѿ����ڴ���

	HZCACHE	hz_cache;												//����Cache�ṹ
	int	zi_cache_loaded;											//�Ƿ��Ѿ�װ������Cache
	int	zi_cache_modified;											//�Ƿ�ı�
	TOPZIITEM topzi_table[MAX_SYLLABLES];							//�ö��ֱ�
	int	topzi_table_items;											//�ö��ֱ�����Ŀ
	int	topzi_loaded;												//�Ƿ��Ѿ�װ�뵽�ڴ�
	int hz_data_loaded;												//������Ϣ���Ƿ��Ѿ�װ��

	int fontmap_loaded;                                             //font map loaded?
	int gbkmap_loaded;                                              //gbk map genernate?
}SHARE_SEGMENT;

extern SHARE_SEGMENT *share_segment;

extern int LoadSharedSegment();
extern int FreeSharedSegment();

#ifdef __cplusplus
}
#endif

#endif