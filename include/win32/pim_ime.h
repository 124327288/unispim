/*	���뷨IME�ӿ�ͷ�ļ�
 */

#ifndef	_PIM_IME_H_
#define	_PIM_IME_H_

#include <windows.h>
#include <win32/immdev.h>
#include <context.h>

#ifdef __cplusplus
extern "C" {
#endif

#define	UIM_MODE_CHANGE				(WM_USER + 0x101)		//�ı�����ģʽ��Ϣ
#define	UIM_INPUT_HZ				(WM_USER + 0x102)		//���뺺����Ϣ��ƫ�Բ��׼��֣�
#define	UIM_NOTIFY					(WM_USER + 0x103)		//�ڲ����ڿ�����Ϣ
//#define	UIM_UPDATE_NOTIFY			(WM_USER + 0x104)		//����֪ͨ��Ϣ
#define	UIM_SEL_CANDIDATE			(WM_USER + 0x105)		//���ѡ���ѡ
#define	UIM_NEXT_PAGE				(WM_USER + 0x106)		//��������һҳ
#define	UIM_PREV_PAGE				(WM_USER + 0x107)		//��������һҳ
#define UIM_RECENT_CI				(WM_USER + 0x108)		//�������Ĵ�
#define UIM_ENGLISH_INPUT			(WM_USER + 0x109)		//�л�Ӣ�����뷨
#define UIM_STATUS_WINDOW_CHANGED	(WM_USER + 0x110)		//״̬����λ�øı�

//#define	UPDATE_NOTIFY_FINISHED		0						//�������
//#define	UPDATE_NOTIFY_WAIT			1						//��Ҫ�ȴ�
//#define	UPDATE_RESOLVE_FAILED		2						//��ַ����ʧ��

#define	IMN_OPENSTATUSWINDOW_ONLY		0x101
#define	IMN_CLOSESTATUSWINDOW_ONLY		0x102

#define	UI_WINDOW_CLASS_NAME		TEXT("V6UNISPIM")
#define	UNISPIM_MUTEX				"UNISPIM6_MUTEX"
	
extern int last_key;

extern TCHAR ui_window_class_name[];
extern HINSTANCE global_instance;

//��õ�ǰ���뷨������
extern PIMCONTEXT *LockContext(HIMC hIMC, LPINPUTCONTEXT *pIMC);
//�ͷŵ�ǰ���뷨������
extern void UnlockContext(HIMC hIMC, LPINPUTCONTEXT pIMC);
//ͨ��UI���ھ��������뷨������
extern HIMC GetIMC(HWND ui_window);

extern void SetContextToWindow(PIMCONTEXT *, HWND);

extern int  Ime_InitContext(HWND ui_window);
extern void Ime_DeleteContext(HWND ui_window);
extern void Ime_SetContext(HWND ui_window, WPARAM wParam, LPARAM lParam);
extern void Ime_Notify(HWND ui_window, WPARAM wParam, LPARAM lParam);
extern void Ime_Select(HWND ui_window, WPARAM wParam, LPARAM lParam);
extern void Ime_Control(HWND ui_window, WPARAM wParam, LPARAM lParam);

extern void RunConfigWordlib();
extern void RunConfigVersion();

//ע��IME UI������
extern int RegisterUIWindowClass(HINSTANCE instance);
//���UI������
extern void UnregisterUIWindowClass(HINSTANCE instance);

extern void SetConversionStatus(HIMC hIMC, LPINPUTCONTEXT pIMC, PIMCONTEXT *context);
extern void GetConversionStatus(HIMC hIMC, LPINPUTCONTEXT pIMC, PIMCONTEXT *context);

extern void GenerateImeMessage(HIMC hIMC, UINT uMsg, WPARAM wParam, LPARAM lParam);
extern int SetIMCContext(LPINPUTCONTEXT pIMC);

#ifdef __cplusplus
}
#endif

#endif
