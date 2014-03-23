/*	���뷨����ͷ�ļ���
 */

//TODO: *****����Ա����ģʽ�����ڷ����ĵ��ַ�֮���/�Լ�*����һֱΪ��ǣ���Ҫ���кܺõı���Լ����飩

//ע����Դ������TAB����Ϊ4��Indent����Ϊ4�ı༭����������ʾ����������
/* Vim ts=4;sw=4 */

#ifndef	_CONFIG_H_
#define	_CONFIG_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <wordlib.h>
#include <windows.h>
#include <spw.h>

//���ó������к󣬸ı��������
#define	MODIFY_NONE					0
#define	MODIFY_WORDLIB				(1 << 0)
#define	MODIFY_THEME				(1 << 1)
#define	MODIFY_INI					(1 << 2)
#define	MODIFY_ZI					(1 << 3)
#define MODIFY_WORDLIB_FORCE_RELOAD	(1 << 4) //�ʿ��б䶯��ǿ������װ�شʿ�(�ָ����ݵ�ʱ��)
#define MODIFY_ENGLISH				(1 << 5)
//#define MODIFY_URL	     			(1 << 6)

//���ó����ļ���
#define	IME_NAME					TEXT("UNISPIM6.IME")
//#define	UPDATE_EXE_NAME				TEXT("upimupd.exe")
#define	PPJZ_EXE_NAME				TEXT("upimrad.exe")
#define	SPW_EXE_NAME				TEXT("upimspw.exe")
//#define	URL_EXE_NAME				TEXT("upimurl.exe")
#define	COMPOSE_EXE_NAME			TEXT("upimcpz.exe")
#define	CONFIG_EXE_NAME				TEXT("upimcfg6.exe")
#define CFGWIZARD_EXE_NAME          TEXT("upimwiz6.exe")
#define THEME_MAKER_EXE_NAME        TEXT("ThemeEditor.exe")
//#define IMEMANAGER_EXE_NAME         TEXT("ImeTool.exe")
#define BATCH_WORDS_EXE_NAME        TEXT("BatchWords.exe")
#define ZIEDITOR_EXE_NAME           TEXT("ZiEditor.exe")

//ע������õ�Key����
#define	REG_CONFIG_KEY_NAME			TEXT("Software\\Thunisoft\\Unispim\\6.9")
//#define REG_OLD_CONFIG_KEY_NAME		TEXT("Software\\Thunisoft\\Unispim\\6.6")
//#define	REG_INSTALL_DIR_KEY_NAME	TEXT("Software\\Thunisoft\\Unispim\\6.8")
#define REG_FULL_CONFIG_KEY_NAME	TEXT("HKEY_CURRENT_USER\\Software\\Thunisoft\\Unispim\\6.9")
#define	REG_PRELOAD_KEY_NAME		TEXT("Keyboard Layout\\Preload")

//ע������õ�������
#define	REG_CONFIG_ITEM_NAME		TEXT("Config")
#define	REG_INSTALL_DIR_NAME		TEXT("InstallDir")

#define	DEFAULT_THEME_NAME			TEXT("Ĭ������")
#define	THEME_DIR_NAME				TEXT("unispim6\\theme\\")
#define	MAIN_BK_IMAGE_NAME			TEXT("unispim6\\theme\\Ĭ������\\bg_main.png")
#define	MAIN_LINE_IMAGE_NAME		TEXT("unispim6\\theme\\Ĭ������\\line.png")
#define	STATUS_BK_IMAGE_NAME		TEXT("unispim6\\theme\\Ĭ������\\bg_status.png")
#define	STATUS_BUTTONS_IMAGE_NAME	TEXT("unispim6\\theme\\Ĭ������\\buttons.png")

#define	MAX_FILE_NAME_LENGTH		256				//�ļ����ֵ���󳤶�
#define	MAX_THEME_NAME_LENGTH		256				//�������Ƶ���󳤶�

#define	MAX_THEME_COUNT				64				//���������Ŀ

//���뷨���
#define STYLE_CSTAR					0				//����֮�Ƿ��Ĭ�ϣ�
#define STYLE_ABC					1				//����ABC���
#define	STYLE_MS2					2				//΢����

//ƴ����ʽ
#define	PINYIN_QUANPIN				0				//ȫƴ
#define	PINYIN_SHUANGPIN			1				//˫ƴ

//�������뷨״̬
#define	STARTUP_CHINESE				0				//����ʱΪӢ������״̬
#define	STARTUP_ENGLISH				1				//����ʱΪ��������״̬��Ĭ�ϣ�

//��ѡѡ��ʽ
#define	SELECTOR_DIGITAL			0				//ʹ������ѡ���ѡ��Ĭ�ϣ�
#define	SELECTOR_LETTER				1				//ʹ����ĸѡ���ѡ

//�򷱶���
#define	HZ_OUTPUT_SIMPLIFIED		(1 << 0)		//��������֣�Ĭ�ϣ�
#define	HZ_OUTPUT_TRADITIONAL		(1 << 1)		//���������
#define	HZ_OUTPUT_HANZI_ALL 		(1 << 2)		//���ȫ��
#define	HZ_OUTPUT_ICW_ZI			(1 << 4)		//���ICWʹ�õĺ��ּ���
#define	HZ_SYMBOL_CHINESE			(1 << 5)		//���ķ���
#define	HZ_SYMBOL_HALFSHAPE			(1 << 6)		//��Ƿ���

#define HZ_SCOPE_GBK                (1 << 0)
#define HZ_SCOPE_UNICODE            (1 << 1)

//��ѡ��
#define	HZ_RECENT_FIRST				(1 << 0)		//�������������ȣ�Ĭ�ϣ�
#define	HZ_ADJUST_FREQ_FAST			(1 << 2)		//���ٵ�����Ƶ��Ĭ�ϣ�
#define	HZ_ADJUST_FREQ_SLOW			(1 << 3)		//���ٵ�����Ƶ
#define	HZ_ADJUST_FREQ_NONE			(1 << 4)		//��������Ƶ
#define	HZ_USE_TAB_SWITCH_SET		(1 << 6)		//ʹ��TAB�л����ֵļ��ϣ�Ĭ�ϣ�
#define	HZ_USE_TWO_LEVEL_SET		(1 << 7)		//ʹ�����ּ����зַ�ʽ��Ĭ�ϣ�
#define	HZ_USE_THREE_LEVEL_SET		(1 << 8)		//ʹ�����ּ����зַ�ʽ
#define	HZ_USE_FIX_TOP				(1 << 9)		//ʹ�ù̶��֣�Ĭ�ϣ�

//��ѡ��
#define	CI_AUTO_FUZZY				(1 << 0)		//����ʵ�ʱ���Զ�ʹ��z/zh, c/ch, s/sh��ģ����Ĭ�ϣ�
#define	CI_SORT_CANDIDATES			(1 << 1)		//��ѡ�ʻ��ڴ�Ƶ��������Ĭ�ϣ�
#define	CI_ADJUST_FREQ_FAST			(1 << 2)		//���ٵ�����Ƶ��Ĭ�ϣ�
#define	CI_ADJUST_FREQ_SLOW			(1 << 3)		//���ٵ�����Ƶ��Ĭ�ϣ�
#define	CI_ADJUST_FREQ_NONE			(1 << 4)		//��������Ƶ��Ĭ�ϣ�
#define	CI_WILDCARD					(1 << 5)		//����ʵ�ʱ��ʹ��ͨ�����Ĭ�ϣ�
#define	CI_RECENT_FIRST				(1 << 6)		//��������Ĵ����ȣ�Ĭ�ϣ�
#define	CI_USE_FIRST_LETTER			(1 << 7)		//ʹ������ĸ����ʣ�Ĭ�ϣ�
#define	CI_AUTO_VOW_FUZZY			(1 << 8)		//����ʵ�ʱ���Զ�������ĸ����ĸģ��ƥ��

//���̶�����
#define	KEY_SWITCH_SHIFT			0				//SHIFT�л�����״̬
#define	KEY_SWITCH_CONTROL			1				//CONTROL�л�����״̬
#define	KEY_SWITCH_NONE				2				//��ʹ�ü����л�����״̬
#define	KEY_2ND_3RD_NONE			0				//��ʹ�õڶ�/�������ٺ�ѡ��
#define	KEY_2ND_3RD_CONTROL			1				//��CONTROLѡ��2��3�ź�ѡ
#define	KEY_2ND_3RD_SHIFT			2				//��SHIFTѡ��2��3�ź�ѡ
//#define KEY_2ND_3RD_SYMBOL          3               //ʹ��;��'ѡ��2��3�ź�ѡ
#define KEY_ZI_FROM_CI_NONE			0				//��ʹ���Դʶ���
#define	KEY_PAIR_1					(1 << 2)		//,.����ѡ��ҳ��
#define	KEY_PAIR_2					(1 << 3)		//[]���Դʶ��֣�
#define	KEY_PAIR_3					(1 << 4)		//-=
#define	KEY_PAIR_4					(1 << 5)		//()�����ܱ༭��
#define	KEY_PAIR_5					(1 << 6)		//<>����ѡ���Σ�
#define KEY_PAIR_6					(1 << 7)		//PageUp/PageDown
#define KEY_PAIR_7					(1 << 8)		//Up/Down

//״̬������ѡ��
#define	STATUS_MORDEN_STYLE			0				//�µ�״̬��
#define	STATUS_TRADITIONAL_STYLE	1				//��ͳ��״̬��

#define	STATUS_BUTTON_NUMBER		9				//״̬������Button����Ŀ

#define	STATUS_BUTTON_MODE			(1 << 0)		//����ģʽ��ť
#define	STATUS_BUTTON_CHARSET		(1 << 1)		//���á�ȫ�����ذ�ť
#define	STATUS_BUTTON_SYMBOL		(1 << 2)		//�����Ű�ť
#define	STATUS_BUTTON_SHAPE			(1 << 3)		//ȫ��Ƿ��Ű�ť
#define	STATUS_BUTTON_SOFTKBD		(1 << 4)		//����̿��ذ�ť
#define	STATUS_BUTTON_SETUP			(1 << 5)		//���ù��߰�ť
#define	STATUS_BUTTON_HELP			(1 << 6)		//������ť

//ͼ��滭��ʽ
#define	DRAW_MODE_TILED				0				//ƽ�̷�ʽ
#define	DRAW_MODE_STRETCH			1				//���췽ʽ

//���ð汾
#define CONFIG_VERSION              0x6500          //���ð汾��

//����ѡ�������ע����п����ڴ�DUMP���ķ�ʽ
//���д洢��������ȡ�Լ�д�����õĹ��̿��Եõ��ܴ�ļ�

//����ѡ��ṹ
typedef struct tagHYPIMCONFIG
{
	//������
	int		input_style;							//Ĭ��STYLE_CSTAR

	//���������뷽ʽ�����ġ�Ӣ��
	int		startup_mode;							//Ĭ��ΪSTARTUP_CHINESE

	//ƴ��ģʽ
	int		pinyin_mode;							//Ĭ��ΪPINYIN_QUANPIN

	//�Ƿ���ʾ˫ƴ����ʾ
	int		show_sp_hint;							//Ĭ��Ϊ0

	//��ѡѡ��ʽ����ĸ�����֡�
	int		select_sytle;							//Ĭ��ΪSELECTOR_DIGITAL

	//�������������
	int		clear_context_always;					//Ĭ��Ϊ1

	//IME�Զ���֪������á����֧��IME�Զ���֪��Ӧ�ó����������
	//��ʾд�����Լ���ѡ�������뷨��������ʾ���������ڡ�����û�
	//����Ҫ����ʾ�������ڣ���ͨ�������ѡ��������á�

	//֧��IME��֪����
	int		support_ime_aware;						//Ĭ��Ϊ1

	//��ֱ��ʾ��ѡ
	int		show_vertical_candidate;				//Ĭ��Ϊ0

	//������ʾд������
	int		show_composition_always;				//Ĭ��Ϊ0

	//������ʾ��ѡ����
	int		show_candidate_always;					//Ĭ��Ϊ0

	//�Ƿ�����״̬��
	int		show_status_window;						//Ĭ��Ϊ1

	//�Ƿ�ʹ�ú�����������
	int		use_hz_tone;							//Ĭ��Ϊ1

	//�Ƿ�ʹ��ICW��������ʣ�
	int		use_icw;								//Ĭ��Ϊ1

	//�Ƿ񱣴�ICW�Ľ�����ʿ�
	int		save_icw;								//Ĭ��Ϊ1

	//�Ƿ������
	int		trace_caret;							//Ĭ��Ϊ1

	//�Ƿ�ֱ��������ַ
	int		__url_direct_input;						//Ĭ��Ϊ1���Ѿ�����

	//����ĸ����ʻ㡣����tagl���������ѡ���᰸������������̫�٣��кܴ�����⡣
	int		first_letter_input_min_hz;				//����ĸ�������С������Ŀ��Ĭ��Ϊ3��

	//���ⷽʽ����ƴ������������⣺fangan -> ���С��������Լ�xian->xi'an
	int		__special_parse_pin_yin;				//˫��������ƴ����������

	//������V����Ӣ�ķ�ʽ��������
	int		input_space_after_v;					//������Vģʽ������ո�

	//�Ƿ������ּ�֮������Ӣ�ķ���
	int		english_symbol_follow_number;			//Ĭ��Ϊ0

	//ʹ��TAB�л����ֵļ���
	int		use_tab_expand_candidates;				//Ĭ��Ϊ1

	//һֱչ����ѡ����
	int		always_expand_candidates;				//Ĭ��Ϊ0

	//��������ķ�ʽ�����塢����
	int		hz_output_mode;							//Ĭ��ΪHZ_OUTPUT_SIMPLIFIED

	//�����ֵ�ѡ��
	int		hz_option;								//Ĭ��Ϊȫ��

	//������ѡ��
	int		ci_option;								//Ĭ��Ϊȫ��

	//�Ƿ�ʹ��ģ�����������û���������ģ��������ʹ�õ�ʱ�򣬻�Ӧ�ñ��������Լ��������ѡ�
	int		use_fuzzy;								//Ĭ��Ϊ0

	//ģ����ѡ��
	int		fuzzy_mode;								//Ĭ��Ϊ0��ȫ����ģ��

	//�����Ĭ�����
	int		soft_kbd_index;							//Ĭ��Ϊ0

	//�ʿ��ļ�����
	TCHAR	wordlib_name[MAX_WORDLIBS][MAX_FILE_NAME_LENGTH];
			//��һ��Ĭ��Ϊ"wordlib/usr.uwl"(userapp)	�û��ʿ�
			//�ڶ���Ĭ��Ϊ"wordlib/sys.uwl"(alluserapp)	ϵͳ�ʿ�
			//�ڶ����Ժ��������б༭

	//�ʿ�����
	int		wordlib_count;							//Ĭ��Ϊ2��ϵͳ�ʿ����û��ʿⲻ�ڱ����м�¼

	//һ���ܹ�������������Ŀ
	int		max_hz_per_input;						//Ĭ��Ϊ20

	//��ʾ�ں�ѡҳ�еĺ�ѡ��������
	int		candidates_per_line;					//Ĭ��Ϊ5

	//��ѡ��ҳ����
	int		key_pair_candidate_page;				//Ĭ��ΪKEY_PAIR_1

	//��ѡ���μ���
	int		__key_pair_candidate_browse;				//Ĭ��ΪKEY_PAIR_5������ûɶ�á���������

	//�Դʶ��ּ���
	int		key_pair_zi_from_word;					//Ĭ��ΪKEY_PAIR_2

	//�༭����
	int		key_prev_edit;							//Ĭ��ΪKEY_PAIR_4

	//�任��Ӣ�����뷽ʽ��
	int		key_change_mode;						//Ĭ��ΪKEY_SWICH_LSHIFT

	//����ڶ����Լ���������ѡ�ļ�
	int		key_candidate_2nd_3rd;					//Ĭ��ΪKEY_2ND_3RD_CONTROL

	//������֮��"."��ΪӢ�ķ���
	int		english_dot_follow_number;				//Ĭ��Ϊ1

	//��չ��ѡ������
	int		expand_candidate_lines;					//Ĭ��Ϊ5

	//���û�ʹ�ù��Ĵʻ��¼���û��ʿ��У�
	int		insert_used_word_to_user_wl;			//Ĭ��Ϊ0

	//��������С�߶ȡ����
	int		main_window_min_width;					//Ĭ��Ϊ0
	int		main_window_min_height;					//Ĭ��Ϊ0

	int		main_window_vert_min_width;				//Ĭ��Ϊ0
	int		main_window_vert_min_height;			//Ĭ��Ϊ0

	//��������״̬���ڵ�͸����
	int		main_window_transparency;				//Ĭ��Ϊ0
	int		status_window_transparency;				//Ĭ��Ϊ0

	//�����ڵ�ê�㣨���Ͻ�λ�ã��������趨������������λ��
	int		main_window_anchor_x;					//Ĭ��Ϊ0
	int		main_window_anchor_y;					//Ĭ��Ϊ0

	int		main_window_vert_anchor_x;				//Ĭ��Ϊ0
	int		main_window_vert_anchor_y;				//Ĭ��Ϊ0

	//״̬�����ϱ߽�
	int		status_window_top_margin;				//Ĭ��Ϊ0

	//����Ӧ�õķ�Χ��ˮƽ����ֱ����չ
	int		theme_use_for_horizon_mode;				//Ĭ��Ϊ1
	int		theme_use_for_vertical_mode;			//Ĭ��Ϊ1
	int		theme_use_for_expand_mode;				//Ĭ��Ϊ1

	TCHAR	backup_dir[MAX_FILE_NAME_LENGTH];		//�û�����Ŀ¼

	//��ֹ��ϼ���������Ϸ
	int		disable_combined_key;					//Ĭ��Ϊ0

	int		config_version;							//���ð汾

	int     trace_recent;                           //�������������Ĵ�

	int     post_after_switch;                      //ģʽ�л�ʱ������������

	int     ignore_fullshape;                       //��������к���ȫ��ģʽ(ȫ��ģʽ��Ҳ����������)

	//ƴ������ѡ��
	int     __correct_option;                       //Ĭ��ȫ������;����

	//ʹ���Զ������
	int		use_special_word;                       //Ĭ��Ϊ1

	//ʹ���Զ������ķ���
	//int     __use_chinese_symbol;                     //Ĭ��Ϊ1���Ѿ�����
	//˫ƴ������ţ�ʹ�þɵ�����__use_chinese_symbolλ�ô洢
	int shuangpin_selItem;

	//ʹ������̿�ݼ�
	int		use_key_soft_kbd;                       //Ĭ��Ϊ1

	//����̿�ݼ�
	int		key_soft_kbd;                           //Ĭ��ΪK

	//ʹ�ü��л���ݼ�
	int		use_key_jian_fan;						//Ĭ��Ϊ1

	//���л���ݼ�
	int		key_jian_fan;							//Ĭ��ΪF

	//ʹ��ȫƴ˫ƴ��ݼ�
	int		use_key_quan_shuang_pin;				//Ĭ��Ϊ0

	//ȫƴ˫ƴ�л���ݼ�
	int		key_quan_shuang_pin;					//Ĭ��ΪS

	//ʹ��״̬���л���ݼ�
	int		use_key_status_window;                  //Ĭ��Ϊ1

	//״̬���л���ݼ�
	int		key_status_window;						//Ĭ��ΪZ

	//��ѡ����Ӣ�ĵ���
	int		candidate_include_english;				//Ĭ��Ϊ1

	//Ӣ�ĵ��ʺ��Զ����ո�
	int		space_after_english_word;				//Ĭ��Ϊ1

	//ʹ��Ӣ�������ݼ�
	int		use_key_english_input;					//Ĭ��Ϊ1

	//Ӣ�������ݼ�
	int		key_english_input;						//Ĭ��ΪE

	//Ӣ������ʱ������ʾ
	int		__english_candidate_vertical;				//Ĭ��Ϊ1����6.8.0.23��ʼ����

	//����ʱ����Ӣ�����뷨
	int		startup_with_english_input;				//Ĭ��Ϊ0

	//��ʾ��������ʾ��Ϣ
	int		show_hint_msg;							//Ĭ��Ϊ1

	//���Ź�����ʾ�ұ߽�
	int		hint_right_margin;						//Ĭ��Ϊ0

	//���Ź�����ʾ�ұ߽�
	int		hint_vert_right_margin;					//Ĭ��Ϊ0

	//�����ļ�����
	TCHAR	spw_name[MAX_SPW_COUNT][MAX_FILE_NAME_LENGTH];

	//�����ļ�����
	int		__spw_count;								//Ĭ��Ϊ1,���Է�����

	//ʹ��Ӣ������
	int		use_english_input;						//Ĭ��Ϊ1

	//ʹ��Ӣ�ķ��빦��
	int		use_english_translate;					//Ĭ��Ϊ1

	//ʹ��url hint����
	//int		__use_url_hint;					        //Ĭ��Ϊ1,disuse

	//���������ѡʱ����ʾλ�ã�ʹ�����������������
	////Ϊ�������������ã�spw_position����ֵ��11~19��ʹ��ʱֱ�Ӽ�10
	int    spw_position;                            //Ĭ��Ϊ13

	//ʹ�ô�������
	int		use_word_suggestion;					//Ĭ��Ϊ1

	//�ӵ�x�����ڿ�ʼ����
	int		suggest_syllable_location;				//Ĭ��Ϊ2

	//�����λ�ں�ѡ�ʵ�xλ
	int		suggest_word_location;					//Ĭ��Ϊ2

	//����ʸ���
	int		suggest_word_count;						//Ĭ��Ϊ2

	//ʹ��u command hint����
	int		use_u_hint;			    		        //Ĭ��Ϊ1

	//����ַ�����Χ��gbk��unicode
	int		scope_gbk;							    //�Ƿ�ֻ���gbk����

	//��ʾUnicode Ext-B��Ext-C����
	int		hide_black_window;					    //���������촰����

	//ȫ��Ӧ��ʱ����״̬��
	int		hide_status_when_full_screen;			//Ĭ��Ϊ1

	//Bģʽ����
	int		B_mode_enabled;							//Ĭ��Ϊ1

	//Dģʽ����
	int		__D_mode_enabled;							//Ĭ��Ϊ1

	//Iģʽ����
	int		I_mode_enabled;							//Ĭ��Ϊ1

	//Hģʽ����
	int		__H_mode_enabled;							//Ĭ��Ϊ1

	//iģʽ����
	int		i_mode_enabled;							//Ĭ��Ϊ1

	//uģʽ����
	int		u_mode_enabled;							//Ĭ��Ϊ1

	//vģʽ����
	int		v_mode_enabled;							//Ĭ��Ϊ1

	//С��������ѡ��
	int		numpad_select_enabled;					//Ĭ��Ϊ1

	//����������ѡ����ڰ汾�������������¸���ע���
	int		reserved[239];							//Ĭ��Ϊ{0}

	//״̬�������ͣ��°棬��ͳ
	int		status_style;							//Ĭ��ΪSTATUS_MORDEN_STYLE

	//��ͳ״̬���ϵİ���
	int		status_buttons;							//Ĭ��ΪSTATUS_BUTTON_MODE | STATUS_BUTTON_SYMBOL | STATUS_BUTTON_SHAPE | STATUS_BUTTON_SOFTKBD | STATUS_BUTTON_MENU

	//�������ƣ����ڽ��������Ѱ���ж�
	TCHAR	theme_name[MAX_THEME_NAME_LENGTH];		//Ĭ��ֵΪ""

	//������������
	TCHAR	chinese_font_name[MAX_FILE_NAME_LENGTH];	//Ĭ��Ϊ�����塱

	//Ӣ���Լ����ŵ���������
	TCHAR	english_font_name[MAX_FILE_NAME_LENGTH];	//Ĭ��Ϊ"Arial"

	//����߶�
	int		font_height;							//Ĭ��Ϊ16

	//�����ڣ�д������ѡ���ڣ����塣�ڱ����뷨�У�Compose��Candidate����û�����֣�һֱ��ͬʱ��ʾ
	//��Ӧ�ô����ܹ�����Compose����Candidateʱ��ͨ���趨����ʾһ�������ݼ��ɡ������������Ϊmain_window��
	//˫ƴʹ�õĴ���������main������ȫ��ͬ��

	//�����ڱ���ͼ�����ƣ����ΪNULL����ʹ��Ĭ����ɫ���л滭
	TCHAR	main_image_name[MAX_FILE_NAME_LENGTH];					//Ĭ��Ϊ""

	TCHAR	main_vert_image_name[MAX_FILE_NAME_LENGTH];				//Ĭ��Ϊ""

	//������ָ���ͼ������
	TCHAR	main_line_image_name[MAX_FILE_NAME_LENGTH];				//Ĭ��Ϊ""

	TCHAR	main_vert_line_image_name[MAX_FILE_NAME_LENGTH];		//Ĭ��Ϊ""

	//����ͼ������ò���
	int		main_left_margin;						//����ͼ�����߽磬Ĭ��2
	int		main_right_margin;						//����ͼ����ұ߽磬Ĭ��2
	int		main_top_margin;						//����ͼ����ϱ߽磬Ĭ��2
	int		main_bottom_margin;						//����ͼ����±߽磬Ĭ��2

	int		main_vert_left_margin;					//����ͼ�����߽磬Ĭ��2
	int		main_vert_right_margin;					//����ͼ����ұ߽磬Ĭ��2
	int		main_vert_top_margin;					//����ͼ����ϱ߽磬Ĭ��2
	int		main_vert_bottom_margin;				//����ͼ����±߽磬Ĭ��2

	int		main_text_left_margin;					//������߽�
	int		main_text_right_margin;					//�����ұ߽�
	int		main_text_top_margin;					//�����ϱ߽�
	int		main_text_bottom_margin;				//�����±߽�
	int		main_center_gap_height;					//���ĸ߶ȣ�����ƴ�������ѡ��֮��ĸ߶�
	int		main_remain_number_margin;				//��ѡʣ����Ŀ�ұ߽�

	int		main_vert_text_left_margin;				//������߽�
	int		main_vert_text_right_margin;			//�����ұ߽�
	int		main_vert_text_top_margin;				//�����ϱ߽�
	int		main_vert_text_bottom_margin;			//�����±߽�
	int		main_vert_center_gap_height;			//���ĸ߶ȣ�����ƴ�������ѡ��֮��ĸ߶�
	int		main_vert_remain_number_margin;			//��ѡʣ����Ŀ�ұ߽�

	int		main_line_left_margin;					//��������߽�
	int		main_line_right_margin;					//�������ұ߽�

	int		main_vert_line_left_margin;				//��������߽�
	int		main_vert_line_right_margin;			//�������ұ߽�

	int		main_round_width;						//����Բ�ǿ��
	int		main_round_height;						//����Բ�Ǹ߶�

	int		main_vert_round_width;					//����Բ�ǿ��
	int		main_vert_round_height;					//����Բ�Ǹ߶�

	int		main_center_mode;						//�������Ļ���ģʽ��Ĭ�ϣ�����
	int		main_vertical_mode;						//��ֱ����ģʽ��Ĭ�ϣ�����
	int		main_horizontal_mode;					//ˮƽ����ģʽ��Ĭ�ϣ�����
	int		main_line_mode;							//�����߻���ģʽ��Ĭ�ϣ�����

	int		main_vert_center_mode;					//�������Ļ���ģʽ��Ĭ�ϣ�����
	int		main_vert_vertical_mode;				//��ֱ����ģʽ��Ĭ�ϣ�����
	int		main_vert_horizontal_mode;				//ˮƽ����ģʽ��Ĭ�ϣ�����
	int		main_vert_line_mode;					//�����߻���ģʽ��Ĭ�ϣ�����

	int		use_vert_bk_when_expanding;				//tab��չʱʹ�����ű���

	int		status_left_margin;						//״̬����ͼ����߽磬Ĭ�ϣ�0
	int		status_right_margin;					//״̬����ͼ���ұ߽磬Ĭ�ϣ�0
	int		status_center_mode;						//״̬����ͼ�����Ļ���ģʽ��Ĭ�ϣ�����
	int		status_left_space;						//״̬����ͼ�����հ׿�ȣ���ť��ʼ�㣩��Ĭ�ϣ�8
	int		status_right_space;						//״̬����ͼ���Ҳ�հ׿�ȣ�Ĭ�ϣ�4
	int		status_min_width;						//״̬����С���

	int		main_effect;							//����������Ч����NONE, SHADOW, TRANSPARENT
	int		status_effect;							//״̬������Ч��

	//������д�������Լ���ѡ���ڵ��ַ�����ɫ
	int		main_string_color;						//Ĭ��Ϊ0x000000

	//�����ڹ����ɫ
	int		main_caret_color;						//Ĭ��Ϊ0xd00000

	//�����ں�ѡǰ���������ɫ
	int		main_number_color;						//Ĭ��Ϊ0x000000

	//�����ڵ�ǰѡ�еĺ�ѡ����ɫ
	int		main_selected_color;					//Ĭ��Ϊ0xd000a0

	//��ѡ��ɫ
	int		main_candidate_color;					//Ĭ��Ϊ0x0040c0

	//������ICW��ɫ
	int		main_icw_color;							//Ĭ��Ϊ0x008000

	//������ʣ���ѡ��Ŀ��ɫ
	int		main_remains_number_color;				//Ĭ��Ϊ0xff0000

	//˫ƴ��ʾ����ɫ
	int		main_sp_hint_border_color;				//Ĭ��Ϊ0xa0a0a0

	//˫ƴ��ʾ�򱳾���ɫ
	int		main_sp_hint_back_color;				//Ĭ��Ϊ0xf4f4f8

	//˫ƴ�����ַ�������ɫ
	int		main_sp_full_string_color;				//Ĭ��Ϊ0x000000

	//״̬���ڱ���ͼ������
	TCHAR	status_image_name[MAX_FILE_NAME_LENGTH];	//Ĭ��Ϊ""

	//״̬���ڰ�ťͼ������
	TCHAR	status_buttons_image_name[MAX_FILE_NAME_LENGTH];	//Ĭ��Ϊ""

	int		status_window_x;						//״̬����Xλ�ã�Ĭ��-1
	int		status_window_y;						//״̬����Yλ�ã�Ĭ��-1

	int		main_window_x;							//������xλ�ã�Ĭ��500
	int		main_window_y;							//������yλ�ã�Ĭ��500

} PIMCONFIG;

//ȫ�ֹ���������Ϣ
extern PIMCONFIG *pim_config;
extern PIMCONFIG default_config;
//extern int global_config_update_time;		//ȫ������װ��ʱ��
//extern int is_default_ime;				//�Ƿ�ΪĬ�����뷨
extern int no_transparent;
extern int no_ime_aware;
extern int no_gdiplus_release;
extern int no_status_show;
extern int no_main_show;
extern int no_multi_line;
extern int no_virtual_key;
extern int host_is_console;
extern int no_end_composition;
extern int no_ppjz;

extern void MaintainConfig();
extern int  ConfigTest();
extern int  GetInstallDir(TCHAR *dir_name);
extern void SaveConfigInternal(PIMCONFIG *config);
extern void LoadThemeConfig(PIMCONFIG *config);
extern int  GetExpandCandidateLine();
extern void RunBackup();

extern void RunConfigNormal();
extern void RunConfigVersion();
extern void RunConfigWordlib();

extern void RunPianPangJianZi(HWND hwnd);
extern void RunSPWPlugin(HWND hwnd);
extern void RunCompose(HWND hwnd);
extern void RunCFGWizard(HWND hwnd);
//extern void RunURLManager(HWND hwnd);
extern void RunThemeMaker(HWND hwnd);
//extern void RunImeManager(HWND hwnd);
extern void RunBatchWords(HWND hwnd);
extern void RunZiEditor(HWND hwnd);

//��������ʶ
extern int program_id;
#define	PID_UNKNOWN			0
#define	PID_OPERA			1
#define	PID_MAXTHON			2
#define	PID_WORLD			3
//#define	PID_WOW				4
#define	PID_MXD				5
#define	PID_WAR3			6
#define	PID_CONIME			7
#define	PID_PHOTOSHOP		8
#define	PID_IMAGEREADY		9
#define	PID_CQ				10
#define	PID_IE				11
#define	PID_FIREFOX			12
#define	PID_WRITER			13
#define PID_ONENOTE			14
#define PID_ACROBAT			15
#define PID_MSNMESSENGER	16
#define PID_VISO			17
#define PID_TM				18
//#define PID_QQ				19
#define PID_FOXMAIL			20
#define PID_OUTLOOK         21
#define PID_OUTLOOKEXPRESS  22
#define PID_TTRAVELER       23
//#define PID_POWERPOINT      24

#ifdef __cplusplus
}
#endif

#endif