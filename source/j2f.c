/*	���嵽�����ת��ģ��
 */
#include <zi.h>
#include <wordlib.h>
#include <utility.h>
#include <tchar.h>
#include <share_segment.h>

#define	JF_MAXSTRINGLEN			40		/* ����󳤶� */
#define	JF_SLOTMAXITEMS			10		/* һ��SLOT�����10����ͻ */
#define	JF_HASHMULT				41		/* ɢ�г��� */
#define	JF_HASHSLOTS			16384	/* ɢ������ */
#define	JF_BASEADDRESS			(sizeof(int) * JF_HASHSLOTS)	/* ����index����ʼ��ַ */

#define	JF_OK					0		/* �ɹ� */
#define	JF_OPENSRCFILEFAIL		1		/* Դ�ļ����ܴ� */
#define	JF_OPENDSTFILEFAIL		2		/* Ŀ���ļ����ܴ� */
#define	JF_WRITEDSTFILEFAIL		3		/* Ŀ���ļ�д���� */
#define	JF_SRCFILEFORMATWRONG	4		/* Դ�ļ������� */
#define	JF_NOMEMORY				5		/* �ڴ治�� */

static JFITEM *jf_info		= 0;
static TCHAR *jf_share_name = TEXT("HYPIM_JF_SHARED_NAME");
static jf_count  = 0;
static jf_loaded = 0;

//#pragma data_seg(HYPIM_SHARED_SEGMENT)
//static int	jf_loaded	= 0;			//�򷱶��ձ��Ƿ��Ѿ����ڴ���
//static int	jf_count	= 0;			//�򷱶�������Ŀ
//#pragma	data_seg()

int LoadJFResource()
{
	TCHAR file_name[MAX_PATH];
	int file_length;

	if (share_segment && share_segment->jf_loaded)
		return 1;

	GetFileFullName(TYPE_ALLAPP, HZJF_FILE_NAME, file_name);

	file_length = GetFileLength(file_name);
	if (file_length <= 0)
		return 0;

	jf_info = AllocateSharedMemory(jf_share_name, file_length);
	if (!jf_info)
		return 0;

	if ((file_length = LoadFromFile(file_name, jf_info, file_length)) == -1)
	{
		FreeSharedMemory(jf_share_name, jf_info);
		Log(LOG_ID, L"�򷱶��ձ��ļ���ʧ�ܡ�name=%s", file_name);
		return 0;
	}

	if (!file_length)
		return 0;

	if (share_segment)
	{
		share_segment->jf_count  = file_length / sizeof(JFITEM);
		share_segment->jf_loaded = 1;
	}
	else
	{
		jf_count  = file_length / sizeof(JFITEM);
		jf_loaded = 1;
	}

	return 1;
}

/**	�ͷ�j2f�ļ�
 */
int FreeJFResource()
{
	if (share_segment)
		share_segment->jf_loaded = 0;
	else
		jf_loaded = 0;

	if (jf_info)
	{
		FreeSharedMemory(jf_share_name, jf_info);
		jf_info = 0;
	}

	return 1;
}

//reload j2f resource
int ReloadJFResource()
{
	LoadJFResource();
	FreeJFResource();
	return 1;
}

/**	���ּ�ת��
 */
static UC ZiJ2F(UC zi)
{
	int i;

	if (share_segment)
	{
		if (!share_segment->jf_loaded)
			LoadJFResource();
	}
	else
	{
		if (!jf_loaded)
			LoadJFResource();
	}

	if (!jf_info)
	{
		jf_info = GetReadOnlySharedMemory(jf_share_name);

		//���ܴ������������Ѿ�װ���ˣ������˳������ڴ汻�ͷŵ�����
		if (!jf_info && 
			((share_segment && share_segment->jf_loaded) || (!share_segment && jf_loaded)))
		{
			if (share_segment)
				share_segment->jf_loaded = 0;
			else
				jf_loaded = 0;

			LoadJFResource();
		}
	}

	if (!jf_info)
		return zi;

	if (share_segment)
		jf_count = share_segment->jf_count;

	for (i = 0; i < jf_count; i++)
	{
		JFITEM *p = (JFITEM*)((INT_PTR)jf_info + i * sizeof(JFITEM));

		if (p->JianTi == zi)
			return p->FanTi;
	}

	return zi;
}

/*	��������ת��
 */
void StringJ2F(TCHAR *zi_string)
{
	int i, len = (int)_tcslen(zi_string);

	for (i = 0; i < len; i++)
		zi_string[i] = ZiJ2F(zi_string[i]);
}


//��ú��ִʴ��Ĺ�ϣɢ��Key. ע��: �ڳ����в��ж��ַ����ĳ���
static int GetHashKey(const TCHAR *str)
{
	unsigned int key = 0;

	while (*str)
		key = key * JF_HASHMULT + (TCHAR) *str++;

	return (int)(key % JF_HASHSLOTS);
}

//�ʼ�ת������
static int ProcessWordJ2F(TCHAR *zi_word)
{
	FILE *fr;
	TCHAR filename[MAX_PATH];
	int  key, offset[2] = {0, 0}, len, ls;
	TCHAR buffer[0x201];		// 513�ֽ�
	TCHAR *p, *pb = buffer;

	GetFileFullName(TYPE_ALLAPP, J2F_FILE_NAME, filename);

	fr = _tfopen(filename, TEXT("rb"));
    if (!fr)
		return FALSE;

	//��ϣֵ
	key = GetHashKey(zi_word);
	fseek(fr, key * sizeof(int), SEEK_SET);

	if (sizeof(offset) != fread(offset, 1, sizeof(offset), fr))
	{
		int err = GetLastError();
		fclose(fr);
		return 0;
	}

	len = sizeof(buffer) - 1;

	//len�ǵ�ǰslot�Ĵ�С(��ϣֵ��ͬ�Ĵ���ͬһ��slot��)
	if ((key != JF_HASHSLOTS) && (len > offset[1] - offset[0]))
		len = offset[1] - offset[0];

	//buffer������ݾ���"��׼,�˃�,����,����..."������������ʽ
	fseek(fr, JF_BASEADDRESS + offset[0], SEEK_SET);
	len = (int)fread(buffer, 1, len, fr);
	fclose(fr);

	if (-1 == len)		// ����
		return 0;

	buffer[len] = '\0';
	ls = (int)_tcslen(zi_word);

	while(p = _tcsstr(pb, zi_word))
	{
		// �鵽�Ĵ��ǵ��������ж�, ����ӳ�䵽ͬһSlot�еĴ��а��������.
		if ((p != pb && *(p - 1) != (UC)'.') || *(p + ls) != (UC)',')
		{
			pb = p + ls;
			continue;
		}

		p += ls + 1;

		while (*zi_word)		// ����
			*zi_word++ = *p++;

		return 1;
	}

	return 0;
}

/*	����ʴӼ��嵽�����ת��
 */
void WordJ2F(TCHAR *zi_word)
{
	if (!ProcessWordJ2F(zi_word))
		StringJ2F(zi_word);
}