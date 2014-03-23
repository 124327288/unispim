/*	������ͷ�ļ���
 */

#ifndef	_MAIN_WINDOW_H_
#define	_MAIN_WINDOW_H_

#include <commctrl.h>
#include <context.h>

typedef struct tagUICONTEXT
{
	PIMCONTEXT	*context;								//ָ��context��ָ��

	HWND	ui_window;									//���뷨��Ϣ����
	HWND	main_window;								//д������
	HWND	status_window;								//״̬����
	HWND	hint_window;								//��ʾ����
	HWND	tooltip_window;								//ToolTip����
	HWND	theme_main_window;							//mainԤ������
	HWND	theme_status_window;						//statusԤ������

	POINT	main_window_pos;							//д�����ڵ�λ��
	SIZE	main_window_size;							//д�����ڵĴ�С
	SIZE	hint_window_size;							//��ʾ���ڵĴ�С
	int		compose_frame_height;						//д�����ֵĸ߶�
	int		candidate_line_height;						//��ѡ�и߶�

	int		have_caret;									//�Ƿ��Ѿ��й��λ����Ϣ��
														//�����ж��Ƿ�ͨ��GetCaretPos���й�궨λ
	POINT	caret_pos;									//���λ��

	int		menu_showing;								//�Ƿ�������ʾ�˵�
	int		process_show_or_hide;						//����״̬������ʾ������

#ifdef __cplusplus
	Font	*zi_font;                                   //��������
	Font    *ascii_font;						        //Ӣ������
	Font	*no_font;									//��������
	Font	*tone_font;									//��������
	Font	*hint_font;									//��ʾ���壬ָ˫ƴ����ʾ����
	Font	*guide_font;								//������ʾ����
	Bitmap  *image_main_bk;								//д�����ڱ���ͼ��
	Bitmap	*image_main_line;							//д����������ͼ��
	Bitmap  *image_main_vert_bk;						//д���������ű���ͼ��
	Bitmap	*image_main_vert_line;						//д��������������ͼ��
	Bitmap	*image_status_bk;							//״̬���ڱ���ͼ��
	Bitmap	*image_status_buttons;						//״̬���ڰ�ťͼ��
#endif
}UICONTEXT;

#ifdef __cplusplus
extern "C" {
#endif

//�������ƫ��λ��
#define	CARET_X_OFFSET		10
#define	CARET_Y_OFFSET		20
#define	CARET_HINT_Y_OFFSET	40

//IMEʹ�õĴ��ڱ�־
#define	IME_WINDOW_FLAG			(SWP_NOACTIVATE | SWP_NOCOPYBITS)
#define	IME_WINDOW_STYLE		(CS_IME | CS_VREDRAW | CS_HREDRAW)

//������ʾ��־
#define	SWF_PAINT				(1 << 0)	//�ػ�����
#define	SWF_SETPOS				(1 << 1)	//�趨����λ��
#define	SWF_HIDE				(1 << 2)	//���ش���
#define	SWF_SHOW				(1 << 3)	//��ʾ����

//��������������ϵ�����
#define	ZONE_NONE				0
#define	ZONE_CANDIDATE			1
#define	ZONE_BUTTON				2			//״̬�����ϵİ�ťͼ��
#define	ZONE_DRAG				4			//״̬�����϶���
#define	ZONE_COMPOSE_LEFT		5			//д�������
#define	ZONE_COMPOSE_RIGHT		6			//д�����ұ�

#define	MIN_MAIN_WINDOW_WIDTH	100

//ToolTip��������
#define	MAX_HINT_LENGTH				0x200	//Hint��Ϣ����
#define	TOOLTIP_BASE_ID				0x100	//ToolTip����ʼID
#define	TOOLTIP_DELAYTIME_AUTOPOP	5000	//ToolTip��ʾʱ��5����
#define	TOOLTIP_DELAYTIME_INITIAL	600		//��ʼ��ʾ
#define	TOOLTIP_DELAYTIME_RESHOW	200		//��ʾ��һ��tip���ӳ�ʱ�䣨û���ô���

#define	MAX_STATUS_BUTTON_NUMBER	0x10	//����״̬���ڰ�ť��Ŀ
//#define	CARET_TIMER_ID				0x100	//����ʱ����ʶ

#define	ROUND_WIDTH					1		//Բ�ǿ��
#define	ROUND_HEIGHT				1		//Բ�Ǹ߶�

#define	CAND_NUMBER_GAP_WIDTH		2		//��ѡ������֮��ľ���
#define	CAND_GAP_WIDTH				6		//��ѡ֮���ˮƽ��϶
#define	CAND_GAP_HEIGHT				2		//��ѡ��֮��Ĵ�ֱ��϶
#define	COMPOSE_STATUS_WIDTH		80		//д�����ڵ�״̬���
#define	COMPOSE_NO_GAP_WIDTH		40		//д�������ѡ��Ŀָʾ��֮�����С���
#define	COMPOSE_SP_HINT_GAP			20		//˫ƴ��ʾ��д����֮��ľ���
#define	MIN_COMPOSE_NO_GAP			20		//ʣ��������д��������С����
#define MIN_COMPOSE_HINT_GAP		20		//��ʾ��Ϣ��д��������С����
#define MIN_HINT_NO_GAP				10		//��ʾ��Ϣ��ʣ�����ֵ���С����
#define COMPOSE_CARET_GAP_WIDTH		3		//д��������֮��ľ���

#define	STATUS_BUTTON_WIDTH			22		//״̬����ͼ����
#define	STATUS_BUTTON_HEIGHT		22		//״̬����ͼ��߶�

#define POS_DEFAULT_X				500		//Ĭ��λ��X
#define POS_DEFAULT_Y				1800	//Ĭ��λ��Y

#define CAND_POPUP_MENU_SET_TOP		1		//�ö�
#define CAND_POPUP_MENU_DELETE		2		//ɾ��
#define CAND_POPUP_MENU_BAIDU		3		//�ٶ�����

#define MAX_CAND_POPUP_MENU_LENGTH	0x200   //���˵����
#define MAX_SEARCH_URL_LENGTH		0x200	//���������URL����

#define MAX_CAND_HINT_SIZE			60		//��ʾ��󳤶�

#define CAND_HINT_TYPE_ENGLISH_CN		1	//Ӣ��������ʾ_����״̬
#define CAND_HINT_TYPE_ENGLISH_EN		2	//Ӣ��������ʾ_Ӣ��״̬
#define CAND_HINT_TYPE_ENGLISH_INPUT	3	//Ӣ��������ʾ_Ӣ�����뷨
#define	CAND_HINT_TYPE_ABC				4	//����ABC�ո���ʾ
#define CAND_HINT_TYPE_USE_TAB			5	//ʹ��Tab��չ��ʾ
#define CAND_HINT_TYPE_NUMBER_STRING	6	//���ִ���i/i��ʾ
#define CAND_HINT_TYPE_AT_CHAR			7   //@��ʾ
#define CAND_HINT_TYPE_U_CHAR			8   //u��ʾ
#define CAND_HINT_TYPE_RANDOM		   99   //�����ʾ

#define STATUS_BUTTON_HINT_INPUT_MODE		1   //����/Ӣ������
#define STATUS_BUTTON_HINT_OUTPUT_MODE		2   //����ַ���
#define STATUS_BUTTON_HINT_MARK_MODE		3	//����/Ӣ�ı��
#define STATUS_BUTTON_HINT_SHAP_MODE		4	//ȫ/���
#define STATUS_BUTTON_HINT_SOFTKEYBOARD		5	//�����
#define STATUS_BUTTON_HINT_SETUP			6	//���ù���
#define STATUS_BUTTON_HINT_HELP				7	//����

#define MAX_STATUS_LIST_SIZE			100		//״̬���б���

#define	MAIN_WINDOW_CLASS_NAME			TEXT("V6MAIN")
#define	STATUS_WINDOW_CLASS_NAME		TEXT("V6STATUS")
#define	HINT_WINDOW_CLASS_NAME			TEXT("V6HINT")
#define THEME_MAIN_WINDOW_CLASS_NAME	TEXT("V6THEMEMAIN")
#define THEME_STATUS_WINDOW_CLASS_NAME	TEXT("V6THEMESTATUS")

//#define	PIM_UPDATE_NOSTART				0	//��δ����
//#define	PIM_UPDATE_RUNNING				1	//������
//#define	PIM_UPDATE_WAITING				2	//�ȴ��´�����
//#define	PIM_UPDATE_FINISHED				3	//�Ѿ����
//#define	PIM_UPDATE_ERROR				4	//���󣨿����������򲻴��ڣ�

//extern HWND main_window_list[MAX_STATUS_LIST_SIZE];
//extern HWND status_window_list[MAX_STATUS_LIST_SIZE];
//extern HWND hint_window_list[MAX_STATUS_LIST_SIZE];

void CreateIMEWindows(PIMCONTEXT *context, UICONTEXT *ui_context);
void DestroyIMEWindows(PIMCONTEXT *context, UICONTEXT *ui_context);

//�������������
extern void PIM_CreateMainWindow(PIMCONTEXT *context, UICONTEXT *ui_context, HINSTANCE instance);
extern void PIM_CreateStatusWindow(PIMCONTEXT *context, UICONTEXT *ui_context, HINSTANCE instance);
extern void PIM_DestroyMainWindow(PIMCONTEXT *context, UICONTEXT *ui_context, HINSTANCE instance);
extern void PIM_DestroyStatusWindow(PIMCONTEXT *context, UICONTEXT *ui_context, HINSTANCE instance);
extern void PIM_DestroyHintWindow(PIMCONTEXT *context, UICONTEXT *ui_context, HINSTANCE instance);
extern void PIM_DestroyToolTipWindow(PIMCONTEXT *context, UICONTEXT *ui_context, HINSTANCE instance);
extern void PIM_DestroyThemeWindow(PIMCONTEXT *context, UICONTEXT *ui_context, HINSTANCE instance);

extern void MainWindowTraceCaret(PIMCONTEXT *context, UICONTEXT *ui_context, POINT point);

extern void LoadThemeResource(UICONTEXT *ui_context);
extern void DeleteThemeResource(UICONTEXT *ui_context);
extern void CheckThemeResource(UICONTEXT *ui_context);

extern void HideMainWindow(PIMCONTEXT *context, UICONTEXT *ui_context);			//����������
extern void ShowMainWindow(PIMCONTEXT *context, UICONTEXT *ui_context);			//��ʾ������

extern void HideStatusWindow(PIMCONTEXT *context, UICONTEXT *ui_context);		//����״̬����
extern void ShowStatusWindow(PIMCONTEXT *context, UICONTEXT *ui_context);		//��ʾ״̬����
extern void UpdateStatusWindow(PIMCONTEXT *context, UICONTEXT *ui_context);
extern void CheckHintState(PIMCONTEXT *context, UICONTEXT *ui_context);			//���Hint���ڵ�״̬
extern void HideHintWindow(PIMCONTEXT *context, UICONTEXT *ui_context);

extern void UpdateWinResource(PIMCONTEXT *context, UICONTEXT *ui_context);		//���´�����Դ

extern void InitGdiplus();
extern void FreeGdiplus();

extern void CheckMainWindowPosition(PIMCONTEXT *context, UICONTEXT *ui_context, HWND win);

extern PIMCONTEXT *GetPIMContextByWindow(HWND ui_window);

extern UICONTEXT *AllocateUIContext();
extern void FreeUIContext(UICONTEXT *context);

extern UICONTEXT default_ui_context;

extern void WINAPI PaintDemoWindow(PIMCONFIG *config, HDC dc_status, HDC dc_main, int *status_width, int *status_height, int *main_width, int *main_height,	int expand, int vertical, int draw_assist_line);

extern void ShowThemeWindow(PIMCONTEXT *context, UICONTEXT *ui_context);
extern void HideThemeWindow(PIMCONTEXT *context, UICONTEXT *ui_context);

extern int LoadRandomHintMessage();

extern void EncodeSearchURL(TCHAR* search_url, TCHAR* key, int is_baidu);

#ifdef __cplusplus
}
#endif

#endif