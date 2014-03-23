/**	�ʻ�����ģ��
 *	�ṩ�����ʻ�����Ĺ��ܣ�
 *	1. ˳��ʻ�����ģʽ
 *	2. ��˳��ʻ�����ģʽ
 */

#include <zi.h>
#include <assert.h>
#include <string.h>
#include <spw.h>
#include <config.h>
#include <utility.h>
#include <win32/pim_ime.h>
#include <string.h>
#include <tchar.h>
#include <fontcheck.h>
#include <gbk_map.h>
#include <share_segment.h>

static BHDATA *bh_data      = 0;  //�ʻ��ļ�ӳ���ڴ��
static TCHAR *bh_share_name = TEXT("HYPIM_BH_SHARED_NAME");

//#pragma data_seg(HYPIM_SHARED_SEGMENT)
//static int	bh_loaded = 0;			//�ʻ��Ƿ��Ѿ����ڴ���
//#pragma	data_seg()

/**	��ñʻ�����ĺ�ѡ
 */
int GetBHCandidates(const TCHAR *input_string, CANDIDATE *candidates, int array_length)
{
	char bh_string[0x100];
	int  i, bh_length, min_bhs, hz_count, idx;
	INT_PTR maxp;
	int *index1, *index2;
	BHITEM *data;

	extern int LoadBHResource();

	//û������������벻ΪB��ֱ�ӷ���
	if (!array_length || !input_string || *input_string != 'B' || !input_string[1])
		return 0;

	if(!share_segment->bh_loaded)
		LoadBHResource();

	if (!bh_data)
	{
		bh_data = GetReadOnlySharedMemory(bh_share_name);

		//���ܴ������������Ѿ�װ���ˣ������˳������ڴ汻�ͷŵ�����
		if (!bh_data && share_segment->bh_loaded)
		{
			share_segment->bh_loaded = 0;
			LoadBHResource();
		}
	}

	if (!bh_data)
		return 0;

	//����Ƿ�Ϊ�ʻ���ĸ�����ֹ��ɣ����򷵻أ���ȷ���Ƿ�Ϊģ��ƥ��
	min_bhs = 0;
	for (bh_length = 0, i = 1; input_string[i] && i < sizeof(bh_string) - 1; i++)
	{
		switch (input_string[i])
		{
			case 'h':
				bh_string[bh_length] = '1';
				break;

			case 's':
				bh_string[bh_length] = '2';
				break;

			case 'p':
				bh_string[bh_length] = '3';
				break;

			case 'n':
				bh_string[bh_length] = '4';
				break;

			case 'd':
				bh_string[bh_length] = '4';
				break;

			case 'z':
				bh_string[bh_length] = '5';
				break;

			case '*':
				bh_string[bh_length] = '*';
				min_bhs--;
				break;

			case '?':
				bh_string[bh_length] = '?';
				break;

			default:
				return 0;
		}

		bh_length++;
	}

	bh_string[bh_length] = 0;
	if (!bh_length)
		return 0;

	min_bhs += bh_length;

	if (min_bhs <= 0)
		return 0;

	index1 = bh_data->index1;
	index2 = (int*)(bh_data->index1 + bh_data->maxstrockes);
	data   = (BHITEM*)(index2 + (bh_data->maxmcp - bh_data->minmcp + 1));
	idx    = bh_data->index1[min_bhs - 1];

	maxp = (INT_PTR)data + sizeof(BHITEM) * bh_data->itemcount;

	//ȡ�ú��ֵĺ�ѡ
	for (hz_count = 0, i = 0; i < bh_data->itemcount && hz_count <array_length; i++)
	{
		BHITEM *p = (BHITEM*)((INT_PTR)bh_data + idx + i * sizeof(BHITEM));

		if((INT_PTR)p >= maxp)
			break;

		if (pim_config->scope_gbk == HZ_SCOPE_UNICODE)
		{
			if ( pim_config->hide_black_window && !FontCanSupport(p->zi))
				continue;
		}
		else
		{
			//if it is not gbk,then continue
			if(!IsGBK(p->zi))
				continue;
		}

		if (!strncmp(bh_string, (char*)((INT_PTR)bh_data + (int)p->bh), bh_length) ||
			strMatch((char*)((INT_PTR)bh_data + (int)p->bh), bh_string))
		{
			//delete duplicated
			if(hz_count > 0 && p->zi == candidates[hz_count-1].spw.hz)
				continue;
			//�ҵ�
			candidates[hz_count].type		= CAND_TYPE_SPW;
			candidates[hz_count].spw.type	= SPW_STRING_BH;
			candidates[hz_count].spw.hz		= p->zi;
			candidates[hz_count].spw.string = &candidates[hz_count].spw.hz;
			candidates[hz_count].spw.length = (int)_tcslen(candidates[hz_count].spw.string);

			hz_count++;
		}
	}

	return hz_count;
}

/**	��ñʻ��ĺ�ѡ��ʾ
 */
void GetBHDisplayString(CANDIDATE *candidate, TCHAR *buffer, int length)
{
	if (!buffer || !length)
		return;

	if (candidate->type != CAND_TYPE_SPW ||	candidate->spw.type != SPW_STRING_BH)
	{
		*buffer = 0;
		return;
	}

	//����Ѱ�Һ��ֵķ���
	GetZiBHPinyin(candidate->spw.hz, buffer, length);
}

/**	���رʻ������ļ����ڴ档
 *	������
 *		file_name			�ʻ������ļ�ȫ·��
 *	���أ�
 *		�ɹ���1
 *		ʧ�ܣ�0
 */
int LoadBHData(const TCHAR *file_name)
{
	int file_length;

	assert(file_name);

	if (share_segment->bh_loaded)
		return 1;

	file_length = GetFileLength(file_name);
	if (file_length <= 0)
		return 0;

	bh_data = AllocateSharedMemory(bh_share_name, file_length);
	if (!bh_data)
		return 0;

	if ((file_length = LoadFromFile(file_name, bh_data, file_length)) == -1)
	{
		FreeSharedMemory(bh_share_name, bh_data);
		Log(LOG_ID, L"�ʻ������ļ���ʧ�ܡ�name=%s", file_name);
		return 0;
	}

	if (!file_length)
		return 0;

	share_segment->bh_loaded = 1;

	return 1;
}

/**	�ͷűʻ������ļ�
 */
int FreeBHData()
{
	share_segment->bh_loaded = 0;

	if (bh_data)
	{
		FreeSharedMemory(bh_share_name, bh_data);
		bh_data = 0;
	}

	return 1;
}