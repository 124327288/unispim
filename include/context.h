/*	����������ͷ�ļ�
 */

#ifndef	_INPUTCONTEXT_H_
#define	_INPUTCONTEXT_H_

#include <kernel.h>
#include <windows.h>
#include <win32/immdev.h>

#ifdef __cplusplus
extern "C" {
#endif

#define		CHINESE_MODE		(1 << 0)		//��������ģʽ
#define		ENGLISH_MODE		(1 << 1)		//Ӣ������ģʽ
#define		CAPTICAL_MODE		(1 << 2)		//��д��ʽ
#define		QP_MODE				(1 << 3)		//ȫƴģʽ
//#define		SP_MODE				(1 << 4)		//˫ƴģʽ

//״̬�ı����ã����ڷ���IME_NOTIFY��Ϣ
#define		MODIFY_COMPOSE			(1 << 0)	//д�����ı�
#define		MODIFY_CANDIDATE		(1 << 1)	//��ѡ���ı�
#define		MODIFY_STATUS			(1 << 2)	//״̬�ı�
#define		MODIFY_STARTCOMPOSE		(1 << 3)	//��ʼд��
#define		MODIFY_ENDCOMPOSE		(1 << 4)	//����д��
#define		MODIFY_RESULT			(1 << 5)	//���ؽ��
#define		MODIFY_RESULT_CONTINUE	(1 << 6)	//���ؽ�����Ҽ���
#define		MODIFY_SENDBACK_CTRL	(1 << 7)	//����Control����ħ���������⣩
#define		MODIFY_SENDBACK_SHIFT	(1 << 8)	//����Shift��
#define		MODIFY_DONT_SEND_CLEAR	(1 << 9)	//shift�л���ʱ�򣬲��ܷ���COMPOSE��Ϣ��Dreamweaver��Ҫ��

#define		STATE_START			0				//��ʼ״̬
#define		STATE_EDIT			1				//�༭״̬
#define		STATE_SELECT		2				//ѡ���ѡ״̬
#define		STATE_ENGLISH		3				//Ӣ��״̬
#define		STATE_RESULT		4				//��ѡ����״̬
#define		STATE_ILLEGAL		5				//�Ƿ�����״̬
#define		STATE_VINPUT		6				//V����״̬
#define		STATE_UINPUT		7				//U����״̬
#define		STATE_IINPUT		8				//I����״̬
#define		STATE_SOFTKBD		9				//���������״̬
#define		STATE_CAPITAL		10				//��д������״̬
#define		STATE_ABC_SELECT	11				//ABC���ѡ��״̬
#define		STATE_IEDIT			12				//���ܱ༭״̬

#define		ENGLISH_STATE_NONE	0				//��Ӣ��ģʽ
#define		ENGLISH_STATE_INPUT	1				//Ӣ������ģʽ
#define		ENGLISH_STATE_CAND	2				//Ӣ�ĺ�ѡģʽ

//#define     URL_STATE_NONE      0               //not url state
//#define     URL_STATE_INPUT     1               //url state

#define		VIEW_MODE_HORIZONTAL	0			//������ʾ
#define		VIEW_MODE_VERTICAL		1			//��ֱ��ʾ
#define		VIEW_MODE_EXPAND		2			//��չ��ʾ

typedef struct tagEditItem
{
	int			syllable_start_pos;				//��ʼ����λ��
	int			syllable_length;				//ռ�õ�������Ŀ
	CANDIDATE	candidate;						//����ĺ�ѡ��Ϣ
	int			left_or_right;					//�Ƿ�Ϊ�Դʶ��֣�����ʶ��ࡢ�Ҳ�
}SELECT_ITEM;

struct tagUICONTEXT;

//���������Ľṹ
typedef struct tagPIMCONTEXT
{
	int			state;													//���뷨��ǰ״̬

	//����״̬
	int			input_mode;												//����ģʽ
	int			soft_keyboard;											//�����
	int			zi_set_level;											//��ǰ���ּ��ϵ�level
	int			expand_candidate;										//��չ��ѡ״̬
	int			force_vertical;											//������ʾ��ȫ��ǿ��������ʾ

	//����״̬
	int			show_composition;										//�Ƿ���ʾд������
	int			show_candidates;										//�Ƿ���ʾ��ѡ����
	int			show_status;											//�Ƿ���ʾ״̬����
	int			candidates_view_mode;									//��ѡ��ʾģʽ�����š����š���չ��

	//���ؽ��
	TCHAR		result_string[MAX_RESULT_LENGTH + 1];					//�����Ľ��
	int			result_length;											//������ȣ����ֽ�Ϊ��λ��
	SYLLABLE	result_syllables[MAX_SYLLABLE_PER_INPUT];				//�������
	int			result_syllable_count;									//���ڼ���
	int			selected_digital;										//��ǰѡ��ĺ�ѡ���֣�0��ʶû��ѡ��

	//����������
	TCHAR		input_string[MAX_INPUT_LENGTH + 0x10];					//�û����������
	int			input_length;											//���봮����
	int			input_pos;												//���ڴ�������봮λ��
	int			cursor_pos;												//���λ���������Ե�ǰ���ڴ����syllableΪ׼��
	TCHAR		last_symbol;											//�û�����ķ���
	int			last_digital;											//�ϴ�������Ƿ�Ϊ����
	int			last_char;												//�ϴ�����ʣ�����ĸ�����������������룩
	int			last_dot;												//�ϴ�������Ƿ�Ϊ��
	int			next_to_last_dot;										//���ϴ�������Ƿ�Ϊ��

	//��ǰ����
	SYLLABLE	syllables[MAX_SYLLABLE_PER_INPUT + 0x10];				//���ڱ�
	//int         syllable_correct_flag[MAX_SYLLABLE_PER_INPUT + 0x10];   //���ھ�����
	//int		  syllable_separator_flag[MAX_SYLLABLE_PER_INPUT + 0x10]; //���ڷָ������
	int			syllable_start_pos[MAX_SYLLABLE_PER_INPUT + 0x10];		//���������봮�е���ʼλ�ñ�
	int			syllable_count;											//���ڼ���
	int			syllable_pos;											//��ǰ����������λ��
	int			syllable_mode;                                          //����ģʽ������ѡΪ����ʱ������ƴ�����а���ͨ���ʱ����״̬Ϊ0
	HZ			default_hz[MAX_SYLLABLE_PER_INPUT + 0x10];              //Ĭ�Ϻ��ִ�
	SYLLABLE    default_hz_syllables[MAX_SYLLABLE_PER_INPUT + 0x10];    //Ĭ�Ϻ��ִ�������(��Ĭ�Ϻ��ִ��е�ÿ����������ȷ��һһ��Ӧ��ϵ)

	//�Ѿ�ѡ�����Ŀ
	SELECT_ITEM	selected_items[MAX_SYLLABLE_PER_INPUT];					//�Ѿ�������ѡ�������
	int			selected_item_count;									//���������

	//���ܱ༭״̬����
	HZ			iedit_hz[MAX_SYLLABLE_PER_INPUT + 0x10];				//�Ѿ�ѡ�õĺ���
	int			iedit_syllable_index;									//���ܱ༭״̬������λ��

	//д��
	TCHAR		compose_string[MAX_COMPOSE_LENGTH];						//д�����ڵ�����
	TCHAR		selected_compose_string[MAX_COMPOSE_LENGTH];			//�Ѿ�ѡ�����д����������
	TCHAR		sp_hint_string[MAX_COMPOSE_LENGTH * 2];					//˫ƴ��ʾ����
	TCHAR		spw_hint_string[MAX_SPW_HINT_STRING];					//����������ʾ����
	int			compose_length;											//д��������
	int			compose_cursor_index;									//�������λ��

	//��ѡ
	CANDIDATE	candidate_array[MAX_CANDIDATES];						//��ѡ����
	int			candidate_count;										//��ѡ��Ŀ
	int			candidate_index;										//��ʾ��ѡ�ĵ�һ������
	int			candidate_selected_index;								//��ѡ�еĺ�ѡ����

	//��ǰҳ��Ҫ��ʾ�ĺ�ѡ
	TCHAR		candidate_string[MAX_CANDIDATES_PER_LINE * MAX_CANDIDATE_LINES][MAX_CANDIDATE_STRING_LENGTH + 2];
	int			candidate_page_count;									//��ǰҳ�еĺ�ѡ��Ŀ
	TCHAR		candidate_trans_string[MAX_CANDIDATES_PER_LINE * MAX_CANDIDATE_LINES][MAX_TRANSLATE_STRING_LENGTH + 2];

	int			modify_flag;											//�����޸�״̬
	HWND		current_input_window;									//��ǰ����Ĵ���

	int			softkbd_index;											//���������
	int			capital;												//��д������
	int			local_config_update_time;								//�����ļ��ı�ʱ��

	//Ӣ���������ѡ��
	int			english_state;											//Ӣ�Ĳ�ȫģʽ
	int			has_english_candidate;									//�Ƿ���Ӣ�ĺ�ѡ

	//int         url_state;                      						//1:input string is url
	int			u_state;												//u����״̬

	struct tagUICONTEXT *ui_context;									//ָ��uicontext��ָ��
}PIMCONTEXT;

extern PIMCONTEXT demo_context;

//extern void ClearContext();
extern void ClearResult(PIMCONTEXT *context);
extern void ResetContext(PIMCONTEXT *context);
extern void FirstTimeResetContext(PIMCONTEXT *context);
extern void ToggleChineseMode(PIMCONTEXT *context);
extern void SetToFanti(PIMCONTEXT *context);
extern void SetToJianti(PIMCONTEXT *context);
extern void SetToLargeSet(PIMCONTEXT *context);
extern void SetToShuangPin(PIMCONTEXT *context);
extern void SetToQuanPin(PIMCONTEXT *context);
extern void SetCombineKeyMode(PIMCONTEXT *context, int yes);
extern void SetInputStyle(PIMCONTEXT *context, int style);
extern void ToggleFanJian(PIMCONTEXT *context);
extern void ToggleFullShape(PIMCONTEXT *context);
extern void ToggleEnglishSymbol(PIMCONTEXT *context);
extern void ToggleEnglishInput(PIMCONTEXT *context);
extern void ToggleEnglishCandidate(PIMCONTEXT *context);
extern void ToggleShowStatusWindow(PIMCONTEXT *context);
extern void ToggleQuanShuang(PIMCONTEXT *context);
extern void ToggleHorizonalVertical(PIMCONTEXT *context);
extern void ToggleIgnoreFullShape(PIMCONTEXT *context);
extern void ProcessInputChar(PIMCONTEXT *context, char ch);
extern void ClearCandidateInfo(LPINPUTCONTEXT pIMC, PIMCONTEXT *context);
extern void ClearComposeInfo(LPINPUTCONTEXT pIMC, PIMCONTEXT *context);
extern void MakeSyllableStartPosition(PIMCONTEXT *context);

extern TCHAR *GetSelectedComposeString(PIMCONTEXT *context);
extern TCHAR *GetReaminComposeString(PIMCONTEXT *context);
extern TCHAR *GetCurrentCandidateString(PIMCONTEXT *context);

extern void SetCandidatesViewMode(PIMCONTEXT *context);
extern int CanSwitchToExpandMode(PIMCONTEXT *context);

#ifdef __cplusplus
}
#endif

#endif