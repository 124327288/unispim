/*********************************************************************
* ��Ȩ���� 
* 
* �ļ����ƣ� libfunc.h
* �ļ���ʶ�� 
* ����ժҪ�� ����ʵ��upimlib.dproj�ļ��е�����������
* ����˵���� ��
* ��ǰ�汾�� V1.0
* ��    �ߣ� xiongh
* �������ڣ� 2010��4��21��
* ������ڣ� 2010��5��23��
**********************************************************************/

#ifndef	_LibFun_H_
#define	_LibFun_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <string.h>
#include <config.h>

//����Theme���ƣ���ȡTheme��Ϣ��Config
int MakeTheme(PIMCONFIG *Config);

//����Theme�б�
int GetThemeList(TCHAR ThemeList[MAX_THEME_COUNT][MAX_THEME_NAME_LENGTH]);

//�����û�����
int BackupUserData(const TCHAR *BackupDir);// BackUpDir:���뱣��·��

//�ܷ������Զ����³���(�жϵ�ǰ�Ƿ�������¡����Ƿ��Ѹ���)
//int CanRunUpdateApp(INT_PTR handlIme);

//��ȡ������ʾ����
int GetHintMessage(TCHAR *MsgBuffer,int BufferSize);//�������һ����ʾ��Ϣ��MsgBuffer��BufferSize;������ʾ��Ϣ�Ĵ�С

#ifdef __cplusplus
}
#endif

#endif