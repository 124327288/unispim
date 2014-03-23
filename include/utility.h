/*	����ͷ�ļ���
 *	װ��log��file���Լ��ڴ�����ͷ�ļ���
 */

#ifndef	_UTILITY_H_
#define	_UTILITY_H_

#include <stdio.h>
#include <kernel.h>
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

#define _SizeOf(x)		(sizeof((x)) / sizeof((x)[0]))
#define _IsNoneASCII(x)	((WORD)x >= 0x2B0)
#define _HanZiLen		1

//�������Ĺ����ڴ�����
#define	MAX_SHARED_MEMORY_COUNT			1024			//���Ĺ������

//���̶���
#define	KEY_LSHIFT				(1 << 0)
#define	KEY_RSHIFT				(1 << 1)
#define	KEY_SHIFT				(1 << 2)
#define	KEY_LCONTROL			(1 << 3)
#define	KEY_RCONTROL			(1 << 4)
#define	KEY_CONTROL				(1 << 5)
#define	KEY_LALT				(1 << 6)
#define	KEY_RALT				(1 << 7)
#define	KEY_ALT					(1 << 8)
#define	KEY_CAPITAL				(1 << 9)

//�����ڴ����Ϣ
typedef struct tagSHAREDMEMORYINFO
{
	HANDLE	handle;
	void	*pointer;
} SHAREDMEMORYINFO;

#define	TYPE_USERAPP		0				//documents and setting/{username}/application data
#define	TYPE_ALLAPP			1				//documents and setting/all users/application data
#define	TYPE_PROGRAM		2				//program files/unispim6
#define	TYPE_TEMP			3				//��ʱ�ļ�Ŀ¼

//LOG���
//����LOG��¼�ı�ʶ���ɳ����ļ������뺯��������϶��ɡ�
#define WIDEN(x)    L ## x
#define WIDEN2(x)   WIDEN(x)
#define LOG_ID		WIDEN2(__FUNCTION__)

//#define	LOG_ID		__FUNCTION__

extern void	Log(const TCHAR *id, const TCHAR *format, ...);
extern int	LogInit(int restart);
extern int FreeLog();

//LOG��ؽ���

//�ļ��������
//���ļ���������
extern int LoadFromFile(const TCHAR *file_name, void *buffer, int buffer_length);

//���������ļ�
extern int SaveToFile(const TCHAR *file_name, void *buffer, int buffer_length);

//����ļ�����
extern int GetFileLength(const TCHAR *file_name);

//�ļ��������

//��õ�ǰϵͳtick
extern int GetCurrentTicks();

//���Ʋ����ַ���
extern void CopyPartString(TCHAR *target, const TCHAR *source, int length);

//�������
extern void OutputHz(HZ hz);

//��ù����ڴ�����ָ��
extern void *GetSharedMemory(const TCHAR *shared_name);
extern void *GetReadOnlySharedMemory(const TCHAR *shared_name);

//���������ڴ���
extern void *AllocateSharedMemory(const TCHAR *shared_name, int length);

//�ͷŹ����ڴ���
extern void FreeSharedMemory(const TCHAR *shared_name, void *pointer);

//�����ƶ���ʼ
extern void DragStart(HWND window);

//�����ƶ�
extern void DragMove(HWND window);

//�����ƶ�����
extern void DragEnd(HWND window);

//��õ�ǰ��Ļ������
extern RECT GetMonitorRectFromPoint(POINT point);

//����һ�����η���ڶ���������
extern void MakeRectInRect(RECT *in_rect, RECT out_rect);

//ת��VK
extern void TranslateKey(UINT virtual_key, UINT scan_code, CONST LPBYTE key_state, int *key_flag, TCHAR *ch, int no_virtual_key);

//�ж��ļ��Ƿ����
extern int FileExists(const TCHAR *file_name);

//��õ�ǰprogram filesĿ¼
extern TCHAR *GetProgramDirectory(TCHAR *dir);

//��õ�ǰ�û���ApplicationĿ¼
extern TCHAR *GetUserAppDirectory(TCHAR *dir);

//��������û���ApplicationĿ¼
extern TCHAR *GetAllUserAppDirectory(TCHAR *dir);

//���Ŀ¼���ļ���
extern TCHAR *CombineDirAndFile(const TCHAR *dir, const TCHAR *file, TCHAR *result);

//����ļ���·����App/allApp/program
TCHAR *GetFileFullName(int type, const TCHAR *file_name, TCHAR *result);

//Ansi�ַ���ת����UTF16
extern void AnsiToUtf16(const char *name, wchar_t *wname, int nSize);

extern void Utf16ToAnsi(const wchar_t *wchars, char *chars, int nSize);

extern void UCS32ToUCS16(const UC UC32Char, TCHAR *buffer);

//�ж�һ��4�ֽ�TChar���飬���ɼ���������ɵġ�����ֵ��0��1��2
extern int UCS16Len(TCHAR *buffer);

//��ucs16תΪucs32��ֻתһ�����֣�����һ�����֣�����0��
extern UC UCS16ToUCS32(TCHAR *buffer);

//���ļ��ж�ȡһ������
extern int GetLineFromFile(FILE *fr, TCHAR *line, int length);

//���ļ��ж�ȡһ���ַ�����û�зָ�����
extern int GetStringFromFile(FILE *file, TCHAR *string, int length);

//�����ַ�����β�Ŀհ׷���
extern void TrimString(TCHAR *line);

//ʹ�ַ������ϻ�������С������
extern void MakeStringFitLength(TCHAR *string, int length);

//��õ�ǰϵͳʱ��
extern void GetTimeValue(int *year, int *month, int *day, int *hour, int *minute, int *second, int *msecond);

//ִ�г���
extern void ExecuateProgram(const TCHAR *program_name, const TCHAR *args, const int is_url);
extern void ExecuateProgramWithArgs(const TCHAR *cmd_line);

//��ò���ϵͳ�İ汾��
extern int GetWindowVersion();

//��ʾϵͳ��Ϣ����
extern void ShowWaitingMessage(const TCHAR *message, HINSTANCE instance, int min_time);

//��GZip�ļ����н�ѹ��
extern int UncompressFile(const TCHAR *name, const TCHAR *tag_name, int stop_length);
extern int CompressFile(const char *name, const char *tag_name);

//�ַ���/16�����ַ���ת������
extern int ArrayToHexString(const char *src, int src_length, char *tag, int tag_length);
extern int HexStringToArray(const char *src, char *tag, int tag_length);
extern int GetSign(const char *buffer, int buffer_length);

extern int IsFullScreen();

extern const TCHAR *GetProgramName();

extern void Lock();
extern void Unlock();

extern int PackStringToBuffer(TCHAR *str, int str_len, TCHAR *buffer, int buf_len);
extern int IsNumberString(TCHAR *candidate_str);
extern int LastCharIsAtChar(TCHAR *str);
extern char strMatch(char *src,char * pattern);

extern int IsNumpadKey(int virtual_key);

//�ж��Ƿ�Ϊ64λϵͳ
extern BOOL IsIME64();

#ifdef __cplusplus
}
#endif

#endif