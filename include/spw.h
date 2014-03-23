#ifndef	_SPW_H_
#define	_SPW_H_

#include <kernel.h>
#include <context.h>

#ifdef __cplusplus
extern "C" {
#endif

#define		SYS_SPW_FILE_NAME				TEXT("unispim6\\phrase\\ϵͳ�����.ini")	//�����ļ�����
#define		SPW_COMMENT_CHAR			';'							//����ע�ͷ���
#define		SPW_ASSIGN_CHAR				'='							//���︳ֵ����
#define		SPW_HINT_LEFT_CHAR			'['							//������ʾ����_��
#define		SPW_HINT_RIGHT_CHAR			']'							//������ʾ����_��
#define		SPW_HINT_NULL_STR			TEXT("[]")					//������ʾ�մ�
#define		SPW_NAME_LENGTH				16							//����������󳤶�
#define		SPW_HINT_LENGTH				64							//������ʾ��󳤶�
#define		SPW_CONTENT_LENGTH			MAX_SPW_LENGTH				//����������󳤶�
#define		SPW_BUFFER_SIZE				0x500000					//����洢����С
#define		SPW_MAX_ITEMS				400000						//���������Ŀ

#define		SPW_TYPE_NAME				1							//��������
#define		SPW_TYPE_CONTENT			2							//��������
#define		SPW_TYPE_COMMENT			4							//����ע��
#define		SPW_TYPE_NONE				8							//�����Ƕ������

#define		SPW_STRING_NORMAL			1							//��ͨ�Ķ���
#define		SPW_STRING_EXEC				2							//ִ�г�������
#define		SPW_STRING_SPECIAL			3							//�������ͣ���I,D,H��
#define		SPW_STRING_BH				4							//�ʻ���ѡ
#define		SPW_STIRNG_ENGLISH			5							//Ӣ�ĵ���

#define		MAX_SPW_COUNT				32							//�����ļ������Ŀ

//u����ı����ؼ���
static const TCHAR u_reserved_word[][MAX_SPW_HINT_STRING] =
{
	TEXT("setup"),		TEXT("�������뷨���ó���*"),		TEXT(""),
	TEXT("abc"),		TEXT("��������ABC���뷽ʽ*"),		TEXT(""),
	TEXT("bsjz"),		TEXT("�򿪲��׼��ֹ���*"),			TEXT(""),
	TEXT("qj"),			TEXT("����ȫ������*"),         		TEXT(""),
	//TEXT("ckgl"),		TEXT("�򿪴ʿ�������*"),			TEXT(""),
	TEXT("cstar"),		TEXT("����CStar���뷽ʽ*"),			TEXT(""),
	TEXT("czsr"),		TEXT("�򿪲������빦��*"),			TEXT(""),
	TEXT("dos"),		TEXT("��Dos����"),				TEXT("cmd"),
	TEXT("dxdiag"),		TEXT("DirectX ��Ϲ���"),			TEXT("dxdiag"),	
	TEXT("dy"),			TEXT("�򿪷���(����)����*"),		TEXT(""),
	TEXT("excel"),		TEXT("��MS Excel"),				TEXT("excel"),
	TEXT("ft"),			TEXT("������������*"),				TEXT(""),
	TEXT("ht"),			TEXT("�򿪻�ͼ���"),				TEXT("mspaint"),
	TEXT("hs"),			TEXT("�л���������ʾ*"),			TEXT(""),
	TEXT("hzgl"),		TEXT("���ֹ���*"),					TEXT(""),
	TEXT("ie"),			TEXT("�������"),					TEXT("iexplore"),
//	TEXT("imetool"),	TEXT("���뷨������*"),				TEXT(""),
	TEXT("jsb"),		TEXT("�򿪼��±�"),					TEXT("notepad"),
	TEXT("jsq"),		TEXT("�򿪼�����"),					TEXT("calc"),
	TEXT("jt"),			TEXT("������������*"),				TEXT(""),
	TEXT("msconfig"),	TEXT("�鿴��������"),				TEXT("msconfig"),
	TEXT("pathalluser"),TEXT("�������û�����Ŀ¼*"),		TEXT(""),
	TEXT("pathapp"),	TEXT("�����뷨��װĿ¼*"),		TEXT(""),
	TEXT("pathime"),	TEXT("��ime����Ŀ¼*"),			TEXT(""),
	TEXT("pathtemp"),	TEXT("��ϵͳ��ʱĿ¼"),			TEXT(""),
	TEXT("pathuser"),	TEXT("�򿪵�ǰ�û�����Ŀ¼*"),		TEXT(""),
	TEXT("pfzz"),		TEXT("��������*"),					TEXT(""),
	TEXT("plzc"),		TEXT("�������*"),					TEXT(""),
	TEXT("qjsr"),		TEXT("��/�ر�ȫ���������Ĺ���*"),	TEXT(""),
	TEXT("qp"),			TEXT("����ȫƴ����*"),				TEXT(""),
	TEXT("services"),	TEXT("�鿴��������"),				TEXT("services.msc"),
	TEXT("sp"),			TEXT("����˫ƴ����*"),				TEXT(""),
	TEXT("status"),		TEXT("�л�״̬����ʾ�벻��ʾ*"),	TEXT(""),
	TEXT("szxd"),		TEXT("��������*"),				TEXT(""),
	//TEXT("url"),		TEXT("����ַ����*"),				TEXT(""),
	TEXT("ver"),		TEXT("�鿴���뷨�汾��Ϣ*"),		TEXT(""),
	//TEXT("version"),	TEXT("�鿴���뷨�汾��Ϣ*"),		TEXT(""),
	TEXT("word"),		TEXT("��MS Word"),				TEXT("winword"),
	TEXT("yczm"),		TEXT("��Զ���������"),			TEXT("mstsc"),
	TEXT("zcb"),		TEXT("��ע���"),					TEXT("regedit"),
	TEXT("ztzz"),		TEXT("��������*"),					TEXT(""),
};

#define RESERVED_WORD_COUNT	 (sizeof(u_reserved_word) / _SizeOf(u_reserved_word[0]) / sizeof(TCHAR))

//��ö����ѡ
int GetSpwCandidates(PIMCONTEXT *context, const TCHAR *name, CANDIDATE *candidate_array, int array_length);

//��������������ʾ��Ϣ
const TCHAR *GetSPWHintString(const TCHAR *input_string);

//���U�������ʾ�ַ���
void GetUDisplayString(CANDIDATE *candidate, TCHAR *buffer, int length);

//�����û��Զ�����ﵽ�ڴ�
//extern int LoadSpwData(const TCHAR *spw_file_name);
extern int LoadAllSpwData();
extern int FreeSpwData();

#ifdef __cplusplus
}
#endif

#endif