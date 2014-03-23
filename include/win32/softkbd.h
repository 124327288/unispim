#ifndef	_SOFTKBD_H_
#define	_SOFTKBD_H_

#include <platform.h>
#include <win32/main_window.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SOFTKBD_NUMBER		15	/* �������Ŀ */
#define SOFTKBD_KEY_NUMBER	48	/* ������ַ���Ŀ */

extern int GetSoftKeyboardSymbol(PIMCONTEXT *context, int virtual_key, int key_flag, TCHAR *symbol);
extern void SelectSoftKBD(PIMCONTEXT *context, HIMC hIMC, int index);
extern void DeSelectSoftKBD();
extern void HideSoftKBDWindow();
extern void ShowSoftKBDWindow();

#ifdef	__cplusplus
}
#endif

#endif