/*	����غ�����
 *	����
 *		1. ��ú��ֺ�ѡ
 *		2. �Ժ�ѡ��������
 *		3. �Ժ�ѡ���ֽ�������
 *
 *	����Ƶ�Ĵ���
 *		1. ��Ƶ��Ϣ��Ϊԭʼ��Ƶ���û�����ʹ�ö�������
 *		2. ԭʼ��Ƶ�������ٸı�
 *		3. �û���ʹ�ö����ڱ�ʶ�û���������ֵĴ���
 *		4. �ڱȽ���Ƶ��ʱ�����Ƚ����û���ʹ�öȵıȽϣ�
 *			�����ͬ��ʹ�ö���ߵ�����
 *		5. ���ʹ�ö���ͬ�������ԭʼ��Ƶ���ж�
 *		6. �����������������ȷ�ʽ����Ƚ�Cache�е�ʹ�ñ�ʶ��
 *
 */

#include <assert.h>
#include <tchar.h>
#include <kernel.h>
#include <config.h>
#include <zi.h>
#include <ci.h>
#include <utility.h>
#include <editor.h>
#include <fontcheck.h>
#include <gbk_map.h>
#include <share_segment.h>

#define	DEFAULT_TOP_POS		100

static HZDATAHEADER *hz_data	 = 0;
static TCHAR *hz_data_share_name = TEXT("HYPIM_HZ_DATA_SHARED_NAME");

//#pragma data_seg(HYPIM_SHARED_SEGMENT)
//
//HZCACHE		hz_cache = { ZICACHE_SIGNATURE, 1, {0}, {0}, };	//����Cache�ṹ
//int			zi_cache_loaded   = 0;							//�Ƿ��Ѿ�װ������Cache
//int			zi_cache_modified = 0;							//�Ƿ�ı�
//
//TOPZIITEM	topzi_table[MAX_SYLLABLES] =  { 0 };			//�ö��ֱ�
//int			topzi_table_items = 0;							//�ö��ֱ�����Ŀ
//int			topzi_loaded      = 0;							//�Ƿ��Ѿ�װ�뵽�ڴ�
//
//static int  hz_data_loaded = 0;								//������Ϣ���Ƿ��Ѿ�װ��
//#pragma	data_seg()


/**	�жϺ����ܷ����ʿ�
 */
int IsAllCanInLibrary(HZ *hz, int length)
{
	int i;

	for (i = 0; i < length; i++)
		if (!_CanInLibrary(hz[i]))
			return 0;

	return 1;
}

#define	IsGBHanzi(hz)

/**	�ж��Ƿ�ΪGB����
 */
int IsGB(HZ hz)
{
	return (((hz) % 0x100) >= 0xb0 && ((hz) % 0x100) <= 0xf7 && ((hz) / 0x100) >= 0xa1 && ((hz) / 0x100) <= 0xfe);
}

/**	��������Ѱ���ö���
 *	���أ�
 *		�ö��ֵ���Ŀ
 */
int GetTopZi(SYLLABLE syllable, HZ *zi)
{
	int i, j;

	if (!(pim_config->hz_option & HZ_USE_FIX_TOP))
		return 0;

	zi[0] = 0;

	//��������Ѱ��
	for (i = 0; i < share_segment->topzi_table_items; i++)
	{
		if (*(short*)&syllable == *(short*)&share_segment->topzi_table[i].syllable)	//�ҵ�
		{
			for (j = 0; j < MAX_TOPZI; j++)
			{
				if (!share_segment->topzi_table[i].top_zi[j])				//����
					break;

				zi[j] = share_segment->topzi_table[i].top_zi[j];
			}

			if (j < MAX_TOPZI)
				zi[j] = 0;

			return j;
		}
	}

	return 0;
}


/*	��ú������ں��ּ����е���š�
 */
static int GetHzItemIndex(HZITEM *item)
{
	assert(item - hz_data->hz_item >= 0 && item - hz_data->hz_item < hz_data->hz_count);

	return (int)item->hz_id;
}

/*	���ں����Լ���Ƶ�����������ں��ֺ�ѡ�����ء�
 */
int CompareHzCodeFreq(const CANDIDATE *cand1, const CANDIDATE *cand2)
{
	assert(cand1 && cand2);

	//����������
	if (cand1->hz.item->hz != cand2->hz.item->hz)
		return cand1->hz.item->hz - cand2->hz.item->hz;

	//����Ƶ����
	return cand2->hz.item->freq - cand1->hz.item->freq;
}

/*	����С���ڴʵĺ��ֽ�����������С���ڴʺ�ѡ�����ء�
 */
int CompareSmallCiCode(const CANDIDATE *cand1, const CANDIDATE *cand2)
{
	int ret;

	assert(cand1 && cand2);

	//�Ȱ���������
	if (cand1->hz.word_item->ci_length != cand2->hz.word_item->ci_length)
		return cand2->hz.word_item->ci_length - cand1->hz.word_item->ci_length;

	//�ٰ���������
	ret = memcmp(
		GetItemHZPtr(cand2->hz.word_item), 
		GetItemHZPtr(cand1->hz.word_item),
		sizeof(HZ) * cand1->hz.word_item->ci_length);

	return ret;
}

//ȥ���޷���ʾ�ĺ���
int DeleteUnreadableZiCandidates(CANDIDATE *candidate_array, int count)
{
	int i, new_count = 0;

	for (i = 0; i < count; i++)
	{
		if ( pim_config->scope_gbk == HZ_SCOPE_UNICODE )
		{
			if ( pim_config->hide_black_window && !FontCanSupport( candidate_array[i].hz.item->hz ) )
				continue;
		}
		else
		{
			//if it is not gbk,then continue
			if(!IsGBK( candidate_array[i].hz.item->hz ))
				continue;			
		}

		candidate_array[new_count] = candidate_array[i];
		new_count++;
	}

	return new_count;
}

/*	�ں�ѡ�����У��ų����ظ��ĺ��֣�������ǰ�ƶ��������������
 *	Ϊ�˼ӿ��ٶȣ�ʹ��bitmap�ķ�ʽ���в��ء�Unicode�ַ���������
 *	���ַ�������
 *	������
 *		candidate_array			��ѡ����
 *		count					��ѡ��Ŀ
 *	���أ�
 *		���غ�ĺ�ѡ��Ŀ
 */
int UnifyZiCandidates(CANDIDATE *candidate_array, int count)
{
	int i;
	int new_count;

	if (count < 2)			//�����������ܽ��бȽ�
		return count;

	//���ں��ֵ������Լ���Ƶ��������������ͬ�ĺ��ֽ�
	//������һ���ţ�Ȼ�����ɾ������
	qsort(candidate_array, count, sizeof(CANDIDATE), CompareHzCodeFreq);

	new_count = 1;
	for (i = 1; i < count; i++)
	{
		if (candidate_array[i].hz.item->hz == candidate_array[i - 1].hz.item->hz)
			continue;

		candidate_array[new_count] = candidate_array[i];
		new_count++;
	}

	return new_count;
}

/*	�ں�ѡ�����У��ų����ظ���С���ڴ�
 *	������
 *		candidate_array			��ѡ����
 *		count					��ѡ��Ŀ
 *	���أ�
 *		���غ�ĺ�ѡ��Ŀ
 */
int UnifySmallCiCandidates(CANDIDATE *candidate_array, int count)
{
	int i;
	int new_count;

	if (count < 2)			//�����������ܽ��бȽ�
		return count;

	//����С���ڴʵ����������������������ͬ�ĺ��ִʽ�
	//������һ���ţ�Ȼ�����ɾ������
	qsort(candidate_array, count, sizeof(CANDIDATE), CompareSmallCiCode);
	
	new_count = 1;
	for (i = 1; i < count; i++)
	{
		if (candidate_array[i].hz.word_item->ci_length == candidate_array[i - 1].hz.word_item->ci_length &&
			!memcmp(GetItemHZPtr(candidate_array[i].hz.word_item), GetItemHZPtr(candidate_array[i - 1].hz.word_item), 
			sizeof(HZ) * candidate_array[i].hz.word_item->ci_length))
			continue;

		candidate_array[new_count] = candidate_array[i];
		new_count++;
	}

	return new_count;
}

/**	������Ƶ���ݵ���Ƶ���ݣ����ڽ�������ıȽϡ�
 *	�ִʶ���ͬһ����׼�Ͻ��С�
 *
 *	��Ƶ���㷽����
 *		freq = freq_ori / 4;
 *		if (freq > 500000)
 *			freq = 500000 + freq / 2000;
 *		if (freq > (1 << 19) - 1)
 *			freq = (1 << 19) - 1;
 *
 *	��Ƶ���㷽����
 *		freq = freq_ori / 2;
 *		if (freq > 8000000)
 *			freq = 8000000 + freq / 2000;
 *		if (freq > (1 << 23) - 1)
 *			freq = (1 << 23) - 1;
 */
int AdjustWordFreqToHZFreq(int ci_freq)
{
	int zi_freq;

	//����Ƶת����ԭʼ���ϴ�Ƶ
	if (ci_freq > BASE_CI_FREQ)
		ci_freq = (ci_freq - BASE_CI_FREQ) * 2000;

	ci_freq *= 4;

	//��ԭʼ��Ƶת��Ϊ��Ƶ
	zi_freq = ci_freq / 2;
	if (zi_freq > BASE_ZI_FREQ)
		zi_freq = BASE_ZI_FREQ + zi_freq / 2000;

	if (zi_freq > MAX_ZI_FREQ)
		zi_freq = MAX_ZI_FREQ;

	return zi_freq;
}

/**	��������Ƶת��Ϊԭʼ��Ƶ
 */
int ConvertToRealHZFreq(int zi_freq)
{
	if (zi_freq > 8000000)
		return (zi_freq - 8000000) * 2000 * 2;

	return zi_freq * 2;
}

/**	����Ƶת��Ϊԭʼ��Ƶ
 */
int ConvertToRealCIFreq(int ci_freq)
{
	if (ci_freq > 500000)
		return (ci_freq - 500000) * 2000 * 4;

	return ci_freq * 4;
}

/*	��ú��ֵ�ʹ�ö�
 */
int GetHzUsedCount(HZITEM *item)
{
	return share_segment->hz_cache.used_count[GetHzItemIndex(item)];
}

//�Ժ��ֽ������򣬼򵥱Ƚ���Ƶ�Լ��̶����Լ��Ƿ���cache�С�
/*
static int CompareZi(const CANDIDATE *cand1, const CANDIDATE *cand2)
{
	int used_count1, used_count2;
	int cache_pos1, cache_pos2;
	int freq1, freq2;

	//����ֵ����0ʱcand2��cand1ǰ������ֵС��0ʱcand1��cand2ǰ

	//����ʽ���̶�������ǰ(�̶���֮�䰴ini�ļ����˳��)�����ǹ̶��֣�
	//�������˿��ٵ�����Ƶ���������֣���Cache����Ŵ��(�����Cache)��
	//ǰ�����������֣�ʹ�öȸߵ���ǰ�������������ٵ�����Ƶ��ʹ�öȸߵ�
	//��ǰ����û��������Ƶ�������������֣���Ƶ�ߵ���ǰ�����������֣���
	//�Ǵʵĺ�ѡ��Ĵ�Ƶ����Ϊ��Ƶ�ٱȽϣ���Ƶ�ߵ���ǰ

	//�ر�ע��CiCache��ZiCache�ṹ����ܴ���cache_pos����Ҳ��ͬ����
	//�����Ķ��䶨��

	//xian->������xian->��
	//���֡��ʻ�������ʱ���á�ʹ�öȡ����У���û��ʹ�öȵ�����£���
	//������Ĵ�Ƶ����Ƶ�����ж�
	if (cand1->hz.is_word)
	{
		freq1		= cand1->hz.word_item->freq;
		freq1		= AdjustWordFreqToHZFreq(freq1);
		cache_pos1	= cand1->hz.cache_pos;  //CiCache�е�λ��

		if (cand1->hz.cache_pos != -1)
			used_count1 = GetCiUsedCount(cache_pos1);
		else
			used_count1 = 0;
	}
	else
	{
		freq1		= cand1->hz.item->freq;
		cache_pos1	= GetZiCachePos(cand1->hz.item);  //ZiCache�е����(���pos������λ�ã����HZCACHE�е�ע��)
		used_count1 = GetHzUsedCount(cand1->hz.item);

		//��ǰ��ȫ��ģʽ and ��ѡ�Ƿ����ֲ��Ǽ����֣���С��Ƶ
		if (pim_config->hz_output_mode == HZ_OUTPUT_HANZI_ALL &&
			cand1->hz.item->traditional && !cand1->hz.item->simplified)
			freq1 = freq1 >> 10;
	}

	if (cand2->hz.is_word)
	{
		freq2		= cand2->hz.word_item->freq;
		freq2		= AdjustWordFreqToHZFreq(freq2);
		cache_pos2	= cand2->hz.cache_pos;

		if (cand2->hz.cache_pos != -1)
			used_count2 = GetCiUsedCount(cache_pos2);
		else
			used_count2 = 0;
	}
	else
	{
		freq2		= cand2->hz.item->freq;
		cache_pos2	= GetZiCachePos(cand2->hz.item);
		used_count2 = GetHzUsedCount(cand2->hz.item);

		//��ǰ��ȫ��ģʽ and ��ѡ�Ƿ����ֲ��Ǽ����֣���С��Ƶ
		if (pim_config->hz_output_mode == HZ_OUTPUT_HANZI_ALL &&
			cand2->hz.item->traditional && !cand2->hz.item->simplified)
			freq2 = freq2 > 10;
	}

	//�жϹ̶��֡�ע�������뷱���ʱ��ʧЧ������
	if (!(pim_config->hz_output_mode & HZ_OUTPUT_TRADITIONAL) && (pim_config->hz_option & HZ_USE_FIX_TOP))
	{
		//������������������ж���ͬ
		if (cand1->hz.top_pos != cand2->hz.top_pos)
			return cand1->hz.top_pos - cand2->hz.top_pos;
	}

	//���ٵ�����Ƶ
	if (pim_config->hz_option & HZ_ADJUST_FREQ_FAST)
	{
		if (!cand1->hz.is_word && !cand2->hz.is_word)
		{
			if (cache_pos1 != cache_pos2)
				return cache_pos2 - cache_pos1;
		}
		else
		{
			if (used_count1 != used_count2)
				return used_count2 - used_count1;
		}
	}

	//���ٵ�����Ƶ
	if (pim_config->hz_option & HZ_ADJUST_FREQ_SLOW)
	{
		if (used_count1 != used_count2)
			return used_count2 - used_count1;
	}

	//�Ƚ���Ƶ
	if (pim_config->hz_output_mode & HZ_OUTPUT_TRADITIONAL)
		if (!cand1->hz.is_word && !cand2->hz.is_word)
			return (cand2->hz.item->freq - cand1->hz.item->freq);

	return freq2 - freq1;
}
*/

static int CompareZi(const CANDIDATE *cand1, const CANDIDATE *cand2)
{
	int used_count1, used_count2;
	int freq1, freq2;

	if (cand1->hz.is_word)
	{
		freq1		= cand1->hz.word_item->freq;
		freq1		= AdjustWordFreqToHZFreq(freq1);

		if (cand1->hz.cache_pos != -1)
			used_count1 = GetCiUsedCount(cand1->hz.cache_pos);
		else
			used_count1 = 0;
	}
	else
	{
		freq1		= cand1->hz.item->freq;
		used_count1 = GetHzUsedCount(cand1->hz.item);

		//��ǰ��ȫ��ģʽ and ��ѡ�Ƿ����ֲ��Ǽ����֣���С��Ƶ
		if (pim_config->hz_output_mode == HZ_OUTPUT_HANZI_ALL &&
			cand1->hz.item->traditional && !cand1->hz.item->simplified)
			freq1 = freq1 >> 10;
	}

	if (cand2->hz.is_word)
	{
		freq2		= cand2->hz.word_item->freq;
		freq2		= AdjustWordFreqToHZFreq(freq2);

		if (cand2->hz.cache_pos != -1)
			used_count2 = GetCiUsedCount(cand2->hz.cache_pos);
		else
			used_count2 = 0;
	}
	else
	{
		freq2		= cand2->hz.item->freq;
		used_count2 = GetHzUsedCount(cand2->hz.item);

		//��ǰ��ȫ��ģʽ�Һ�ѡ�Ƿ����ֲ��Ǽ����֣���С��Ƶ
		if (pim_config->hz_output_mode == HZ_OUTPUT_HANZI_ALL &&
			cand2->hz.item->traditional && !cand2->hz.item->simplified)
			freq2 = freq2 > 10;
	}

	//�жϹ̶��֡�ע�������뷱���ʱ��ʧЧ������
	if (!(pim_config->hz_output_mode & HZ_OUTPUT_TRADITIONAL) && (pim_config->hz_option & HZ_USE_FIX_TOP))
	{
		//������������������ж���ͬ
		if (cand1->hz.top_pos != cand2->hz.top_pos)
			return cand1->hz.top_pos - cand2->hz.top_pos;
	}

	//���ٻ����ٵ�����Ƶ
	if (pim_config->hz_option & HZ_ADJUST_FREQ_FAST || pim_config->hz_option & HZ_ADJUST_FREQ_SLOW)
	{
		if (used_count1 != used_count2)
			return used_count2 - used_count1;
	}

	//�Ƚ���Ƶ
	if (pim_config->hz_output_mode & HZ_OUTPUT_TRADITIONAL)
		if (!cand1->hz.is_word && !cand2->hz.is_word)
			return (cand2->hz.item->freq - cand1->hz.item->freq);

	return freq2 - freq1;
}

static void SortZiCandidatesInternal(CANDIDATE *candidate_array, int count)
{
	int i, j, k;
	CANDIDATE temp;

	//�����ֲ����ǹ̶�������
	if (!(pim_config->hz_output_mode & HZ_OUTPUT_TRADITIONAL) && (pim_config->hz_option & HZ_USE_FIX_TOP))
	{
		for (i = 0; i < count; i++)
		{
			if (candidate_array[i].hz.top_pos == DEFAULT_TOP_POS)
			{
				break;
			}
		}

		//�̶��ֲ��������������
		candidate_array += i;
		count -= i;
	}

	for (i = 0; i < count; i++)
	{
		if (candidate_array[i].hz.is_word)
		{
			continue;
		}

		k = i;
		for (j = i + 1; j < count; j++)
		{
			if (candidate_array[j].hz.is_word)
			{
				continue;
			}

			//ע��hz.cache_pos��ֻ��С���ڴʵ�cacheλ��
			if (GetZiCachePos(candidate_array[j].hz.item) > GetZiCachePos(candidate_array[k].hz.item))
			{
				k = j;
			}
		}

		if (k != i)
		{
			//�ƶ����ǽ���(��֤ѡ����������������ǰ�ʹ�ö������)
			temp = candidate_array[k];
			memcpy(candidate_array + i + 1, candidate_array + i, (k - i) * sizeof(CANDIDATE));
			candidate_array[i] = temp;
		}
	}
}

//�Ժ��ֺ�ѡ��������
void SortZiCandidates(CANDIDATE *candidate_array, int count)
{
	if (count < 2)			//�����������ܽ��бȽ�
		return;

	qsort(candidate_array, count, sizeof(CANDIDATE), CompareZi);

	//���ǿ�����Ƶ��������Ҫ��һ���������bugsys A00000022
	if (pim_config->hz_option & HZ_ADJUST_FREQ_FAST)
	{
		SortZiCandidatesInternal(candidate_array, count);
	}
}

/*	�жϺ����Ƿ�����ض���������
 *	������
 *		hz				����
 *		syllable		��������
 *		tone			���жϵ�����
 *
 *	���أ�
 *		��ѡ������Ŀ��
 */
int ZiContainTone(HZ hz, SYLLABLE syllable, int tone)
{
	int start = 0;
	int end = hz_data->hz_count - 1;
	int mid;

	if (tone == TONE_0)				//û��ָ������
		return 1;

	//�ö��ַ����ҵ��������
	while(start <= end)
	{
		mid = (start + end) / 2;

		//��ĸ������
		if (hz_data->hz_item[mid].syllable.con > syllable.con)
		{
			end = mid - 1;
			continue;
		}
        else if (hz_data->hz_item[mid].syllable.con < syllable.con)
		{
			start = mid + 1;
			continue;
		}

		//��ĸ������
		if (hz_data->hz_item[mid].syllable.vow > syllable.vow)
		{
			end = mid - 1;
			continue;
		}
		else if (hz_data->hz_item[mid].syllable.vow < syllable.vow)
		{
			start = mid + 1;
			continue;
		}

		//��������
		if (hz_data->hz_item[mid].hz > hz)
		{
			end = mid - 1;
			continue;
		}

		if (hz_data->hz_item[mid].hz < hz)
		{
			start = mid + 1;
			continue;
		}

		//�ҵ���
		break;
	}

	if (start > end)		//û���ҵ�
		return 0;

	return (hz_data->hz_item[mid].syllable.tone & tone) != 0;
}

/*	��ú��ֺ�ѡ��
 *	������
 *		syllable		����
 *		candidate_array	��ѡ����
 *		array_length	��ѡ���鳤��
 *		fuzzy_mode		ģ������
 *		set_mode		���ּ���ѡ����ã���������ȫ��
 *		output_mode		�����ѡ����塢����
 *		ע�����Ϊ���壬��϶���ȫ�����ּ��ϡ�
 *	���أ�
 *		��ѡ������Ŀ��
 */
int GetZiCandidates(SYLLABLE syllable, CANDIDATE *candidate_array, int array_length, int fuzzy_mode, int set_mode, int output_mode)
{
	int i, j, count;		//��ѡ������
	int topzi_count;		//�ö�����Ŀ
	HZ  top_zi[MAX_TOPZI];	//�ö���
	int check_top_zcs_fuzzy = 0;	//�ö���zcsģ��
	extern int LoadHZDataResource();

	if (!array_length)
		return 0;

	if (!share_segment->hz_data_loaded)
		LoadHZDataResource();

	if (!hz_data)
	{
		hz_data = GetReadOnlySharedMemory(hz_data_share_name);

		//���ܴ������������Ѿ�װ���ˣ������˳������ڴ汻�ͷŵ�����
		if (!hz_data && share_segment->hz_data_loaded)
		{
			share_segment->hz_data_loaded = 0;
			LoadHZDataResource();
		}
	}

	if (!hz_data)
		return 0;

	topzi_count = GetTopZi(syllable, top_zi);

	//����s=�ǣ�c=����z=���� �����ã����Ա����zcs��ģ�����д����ҳ�
	//���й̶��ֵĺ�ѡ
	if (topzi_count && syllable.vow == VOW_NULL &&
		(syllable.con == CON_Z || syllable.con == CON_C || syllable.con == CON_S))
		check_top_zcs_fuzzy = 1;

	count = 0;
	//��������
	for (i = 0; i < hz_data->hz_count; i++)
	{
		//�жϼ����Ƿ����
		if (!(
			((output_mode & HZ_OUTPUT_HANZI_ALL)) ||								//���ȫ������
			((output_mode & HZ_OUTPUT_ICW_ZI) && hz_data->hz_item[i].icw_hz) ||			//���ICWʹ�õĺ���
			((output_mode & HZ_OUTPUT_SIMPLIFIED) && hz_data->hz_item[i].simplified) ||	//��������֣����Ǽ���
			((output_mode & HZ_OUTPUT_TRADITIONAL) && (hz_data->hz_item[i].traditional || hz_data->hz_item[i].other))	//��������֣����Ƿ�������δ����
			))
			continue;

		if (check_top_zcs_fuzzy)
		{
			if (!ContainSyllableWithTone(syllable, hz_data->hz_item[i].syllable, fuzzy_mode | FUZZY_Z_ZH | FUZZY_C_CH | FUZZY_S_SH))
				continue;

			//�ж��Ƿ�Ϊ�ö���
			for (j = 0; j < topzi_count; j++)
				if (top_zi[j] == LOWORD(hz_data->hz_item[i].hz))
					break;

			//�����ö��ֵĻ�����Ҫ���½����������ж�
			if (j == topzi_count)
				if (!ContainSyllableWithTone(syllable, hz_data->hz_item[i].syllable, fuzzy_mode))
					continue;			//ƴ�������
		}
		else if (!ContainSyllableWithTone(syllable, hz_data->hz_item[i].syllable, fuzzy_mode))
			continue;			//ƴ�������

		candidate_array[count].type		  = CAND_TYPE_ZI;
		candidate_array[count].hz.is_word = 0;
		candidate_array[count].hz.item	  = &hz_data->hz_item[i];
		candidate_array[count].hz.hz_type = ZI_TYPE_NORMAL;

		//�ж��Ƿ�Ϊ�ö���
		for (j = 0; j < topzi_count; j++)
			if (top_zi[j] == LOWORD(hz_data->hz_item[i].hz))
				break;

		//�����ö�����ţ����û�У�����Ϊ���ֵ
		if (j == topzi_count)
			j = DEFAULT_TOP_POS;

		candidate_array[count].hz.top_pos = j;

		count++;
		if (count >= array_length)			//�������
			break;
	}

	return count;
}

HZITEM* GetSingleZiCandidate(TCHAR zi)
{
	int i;

	for (i = 0; i < hz_data->hz_count; i++)
	{
		if (zi == hz_data->hz_item[i].hz)
			return &hz_data->hz_item[i];
	}

	return 0;
}

/*	�����������ڵ����ֺ�ѡ��
 *	������̣�
 *		1. �������ֺ�ѡ
 *		2. ���ֺ�ѡ����
 *		3. ��ѡ����
 *	������������ϵͳ������IC�����еĵ�ǰģ����������������Լ��������
 *	���ý��д���
 *
 *	������
 *		syllable			����
 *		*candidate_array	��ѡ����
 *		array_length		��ѡ���鳤��
 *	����ֵ��
 *		���غ�ѡ��Ŀ
 */
int ProcessZiCandidates(SYLLABLE syllable, CANDIDATE *candidate_array, int array_length, int zi_level)
{
	int zi_count = 0, normal_zi_count = 0, small_count = 0, small_ci_count = 0, small_zi_count = 0;
	SYLLABLE small_syllables[2];
	CANDIDATE small_word_candidates[0x10];

	//Ѱ������xian���������ĺ�ѡ��˫ƴ���ÿ���
	if (/*pim_config->special_parse_pin_yin && */pim_config->pinyin_mode != PINYIN_SHUANGPIN && GetSmallSyllables(syllable, small_syllables))
	{
		int i;

		small_ci_count = ProcessCiCandidate(small_syllables,
										 2,
										 0,
										 small_word_candidates,
										 _SizeOf(small_word_candidates),
										 1);

		for (i = 0; i < small_ci_count && i < array_length; i++)
		{
			candidate_array[i].type			= CAND_TYPE_ZI;
			candidate_array[i].hz.is_word	= 1;
			candidate_array[i].hz.word_item = small_word_candidates[i].word.item;
			candidate_array[i].hz.origin_syllable = syllable;
			candidate_array[i].hz.top_pos	= DEFAULT_TOP_POS;
			candidate_array[i].hz.cache_pos =
				GetCiCacheInfo(GetItemHZPtr(candidate_array[i].hz.word_item), candidate_array[i].hz.word_item->ci_length);
		}
		
		//����С���ڲ����
		small_zi_count = GetZiCandidates(small_syllables[0],
			candidate_array + small_ci_count,
			array_length - small_ci_count,
			pim_config->use_fuzzy ? pim_config->fuzzy_mode : 0,
			zi_level,
			pim_config->hz_output_mode);

		//���û���ҵ����֣���eng������������ֵķ�Χ���в���
		if (!small_zi_count)			
			small_zi_count = GetZiCandidates(small_syllables[0],
			candidate_array + small_ci_count,
			array_length - small_ci_count,
			pim_config->use_fuzzy ? pim_config->fuzzy_mode : 0,
			HZ_ALL_USED,
			HZ_OUTPUT_HANZI_ALL);

		for (i = small_ci_count; i < small_ci_count + small_zi_count; i++)
		{
			candidate_array[i].hz.hz_type = ZI_TYPE_OTHER;
		}
		
		small_count = small_ci_count + small_zi_count;
	}

	//������ͨ��
	normal_zi_count = GetZiCandidates(syllable,
							candidate_array + small_count,
							array_length - small_count,
							pim_config->use_fuzzy ? pim_config->fuzzy_mode : 0,
							zi_level,
							pim_config->hz_output_mode);

	//���û���ҵ����֣���eng������������ֵķ�Χ���в���
	if (!normal_zi_count)			
		normal_zi_count = GetZiCandidates(syllable,
								candidate_array + small_count,
								array_length - small_count,
								pim_config->use_fuzzy ? pim_config->fuzzy_mode : 0,
								HZ_ALL_USED,
								HZ_OUTPUT_HANZI_ALL);

	zi_count = small_zi_count + normal_zi_count;

	//ȥ���޷���ʾ�ĺ���
	zi_count = DeleteUnreadableZiCandidates(candidate_array + small_ci_count, zi_count);

	//����(ֻ�����)
	zi_count = UnifyZiCandidates(candidate_array + small_ci_count, zi_count);

	//����(�ֺ�С���ڴʶ�Ҫ��)
	SortZiCandidates(candidate_array, small_ci_count + zi_count);

	return small_ci_count + zi_count;
}

/*	��Cache�����Ӻ����
 *	������
 *		item			������
 *	���أ���
 */
static void AddHzToCache(HZITEM *item)
{
	int i;

	//�����Ϊ�̶���ʽ������Ҫ����д��
	if (pim_config->hz_option & HZ_ADJUST_FREQ_NONE)
		return;

	share_segment->hz_cache.cache[GetHzItemIndex(item)] = share_segment->hz_cache.max_id;	//�������ָ������±�־
	share_segment->hz_cache.max_id++;														//�ۼƵ�ǰ����id

	//�ﵽ���ֵ�󣬽����е�ֵ����ȥ���ֵ����
	if (share_segment->hz_cache.max_id >= HZ_CACHE_MAX_ID)
	{
		for (i = 0; i < _SizeOf(share_segment->hz_cache.cache); i++)
			share_segment->hz_cache.cache[i] -= HZ_CACHE_MAX_ID;

		share_segment->hz_cache.max_id -= HZ_CACHE_MAX_ID;
	}

	share_segment->zi_cache_modified = 1;
}

/*	��ú�������Cache�е�ID��
 *	������
 *		item			������ָ��
 *	���أ�
 *		��Cache�е�ʹ��ID��Խ��Խ��ʹ�ã�
 */
static int GetZiCachePos(HZITEM *item)
{
	return share_segment->hz_cache.cache[GetHzItemIndex(item)];
}

/*	�����ֵ�ʹ�ö�
 */
static void AddHzToUsedCount(HZITEM *item)
{
	//�����Ϊ�̶���ʽ������Ҫ����д��
	if (pim_config->hz_option & HZ_ADJUST_FREQ_NONE)
		return;

	share_segment->hz_cache.used_count[GetHzItemIndex(item)]++;
	share_segment->zi_cache_modified = 1;
}

/*	�����ֱ�ѡ�к�ĸ�������
 *	������Ƶ������Cache��
 *	������
 *		item		������
 *	���أ���
 */
void ProcessZiSelected(HZITEM *item)
{
	//���º���ʹ�ö���Ϣ
	AddHzToUsedCount(item);

	//���º���Cache��Ϣ
	AddHzToCache(item);
}

/**	�����Դʶ��ֵ��ֵĸ���
 *	�ҵ�����ֶ�Ӧ��HZITEM�������Ƶ�������д���
 */
void ProcessZiSelectedByWord(HZ hz, SYLLABLE syllable)
{
	int i, index = -1;
	int max_freq = -1;

	//todo:������Ҫ���������д���

	//��������
	for (i = 0; i < hz_data->hz_count; i++)
	{
		if (hz_data->hz_item[i].hz != hz)
			continue;

		if (!ContainSyllableWithTone(syllable, hz_data->hz_item[i].syllable, 0))
			continue;			//ƴ�������

		if ((int)hz_data->hz_item[i].freq > max_freq)
		{
			index = i;
			max_freq = hz_data->hz_item[i].freq;
		}
	}

	if (index != -1)		//�ҵ�
		ProcessZiSelected(&hz_data->hz_item[index]);
}

/**	װ���ö����ļ�
 *	���أ�
 *		�ɹ���1
 *		ʧ�ܣ�0
 */
int LoadTopZiData(const TCHAR *name)
{
	TCHAR *buffer;
	TCHAR line[0x400];
	int  length, count, index;
	TCHAR *syllable_string, *zi_string;
	int  syllable_length, zi_count, i;
	SYLLABLE syllable;

	if (share_segment->topzi_loaded)
		return 1;

	length = GetFileLength(name);
	if (length <= 0)
		return 0;

	buffer = (TCHAR*)malloc(length);
	if (!LoadFromFile(name, buffer, length))
	{
		free(buffer);
		return 0;
	}

	index = 1;
	length = length / sizeof(TCHAR);

	share_segment->topzi_table_items = 0;
	while(share_segment->topzi_table_items < MAX_SYLLABLES && index < length)
	{
		//���һ������
		count = 0;
		while(index < length && count < _SizeOf(line) - 1)
		{
			line[count++] = buffer[index++];
			if (buffer[index - 1] == 0xa)
				break;
		}

		line[count] = 0;		//�õ�һ������

		if ('#' == line[0])		//ע��
			continue;

		if (count && line[count - 1] == 0xa)
			line[--count] = 0;

		if (count && line[count - 1] == 0xd)
			line[--count] = 0;

		syllable_string = _tcstok(line, TEXT("="));
		zi_string = _tcstok(0, TEXT("="));

		//û�������ֻ��������ֳ���Ϊ0������
		if (!zi_string || zi_string[0] == 0 || !syllable_string || syllable_string[0] == 0)
			continue;

		//���ڽ���ʧ�ܣ�����
		if (!GetSyllable(syllable_string, &syllable, &syllable_length, 0/*, 0, 0*/))
			continue;

		//����ֵĳ���
		zi_count = (int)_tcslen(zi_string);

		if (zi_count >= MAX_TOPZI)
			zi_count = MAX_TOPZI;

		if (!zi_count)			//û����
			continue;

		//��䵽topzi����
		share_segment->topzi_table[share_segment->topzi_table_items].syllable = syllable;
		for (i = 0; i < MAX_TOPZI; i++)
		{
			if (i < zi_count)
				share_segment->topzi_table[share_segment->topzi_table_items].top_zi[i] = *((HZ*)zi_string + i);
			else
				share_segment->topzi_table[share_segment->topzi_table_items].top_zi[i] = 0;
		}

		share_segment->topzi_table_items++;
	}

	free(buffer);
	share_segment->topzi_loaded = 1;

	return 1;
}

/**	�ͷ��ö�����Դ
 */
int FreeTopZiData()
{
	share_segment->topzi_loaded = 0;

	return 1;
}

/**	װ����Cache�����ļ�
 *	�ɹ���1��ʧ�ܣ�0
 */
int LoadZiCacheData(const TCHAR *zi_cache_name)
{
	if (share_segment->zi_cache_loaded)
		return 1;

	if (LoadFromFile(zi_cache_name, (char*)&share_segment->hz_cache, sizeof(share_segment->hz_cache)) > 0)
		share_segment->zi_cache_loaded = 1;
	else
		share_segment->zi_cache_loaded = 0;

	return share_segment->zi_cache_loaded;
}

/**	���溺��Cache����
 *	���أ�
 *		�ɹ���1��ʧ�ܣ�0
 */
int SaveZiCacheData(const TCHAR *zi_cache_name)
{
	if (share_segment->zi_cache_modified)
	{
		share_segment->zi_cache_modified = 0;
		return SaveToFile(zi_cache_name, (char*)&share_segment->hz_cache, sizeof(share_segment->hz_cache)) != -1;
	}
	return 1;
}

/**	�ͷ�ZiCacheData
 */
int FreeZiCacheData(const TCHAR *zi_cache_name)
{
	SaveZiCacheData(zi_cache_name);
	share_segment->zi_cache_loaded = 0;

	return 1;
}

int LoadHZData(const TCHAR *hz_data_name)
{
	int file_length;

	assert(hz_data_name);

	if (share_segment->hz_data_loaded)
		return 1;

	file_length = GetFileLength(hz_data_name);
	if (file_length <= 0)
		return 0;

	hz_data = AllocateSharedMemory(hz_data_share_name, file_length);
	if (!hz_data)
		return 0;

	if ((file_length = LoadFromFile(hz_data_name, hz_data, file_length)) == -1)
	{
		FreeSharedMemory(hz_data_share_name, hz_data);
		Log(LOG_ID, L"������Ϣ���ļ���ʧ�ܡ�name=%s", hz_data_name);
		return 0;
	}

	if (!file_length)
		return 0;

	share_segment->hz_data_loaded = 1;

	return 1;
}

/**	�ͷź�����Ϣ���ļ�
 */
int FreeHZData()
{
	share_segment->hz_data_loaded = 0;

	if (hz_data)
	{
		FreeSharedMemory(hz_data_share_name, hz_data);
		hz_data = 0;
	}

	return 1;
}

/**	��ú��ֱʻ���ƴ������������ʾ�ʻ���ѡ
 */
void GetZiBHPinyin(UC zi, TCHAR *buffer, int length)
{
	int  i, first;
	TCHAR s_str[0x30];
	int  s_len;
	SYLLABLE s;

	if (!buffer || !length || length < sizeof(HZ) + 1)
		return;

	if (!share_segment->hz_data_loaded)
		LoadHZDataResource();

	if (!hz_data)
	{
		hz_data = GetReadOnlySharedMemory(hz_data_share_name);

		//���ܴ������������Ѿ�װ���ˣ������˳������ڴ汻�ͷŵ�����
		if (!hz_data && share_segment->hz_data_loaded)
		{
			share_segment->hz_data_loaded = 0;
			LoadHZDataResource();
		}
	}

	if (!hz_data)
	{
		buffer[0] = 0;
		return;
	}

	UCS32ToUCS16(zi, buffer);

	first = 1;
	for (i = 0; i < hz_data->hz_count; i++)
	{
		if (hz_data->hz_item[i].hz != zi)
			continue;

		s	   = hz_data->hz_item[i].syllable;
		s_len  = GetSyllableString(s, s_str, _SizeOf(s_str),/* 0,*/ 1);

		if (s_len + (int)_tcslen(buffer) + 2 >= length - 1)
			return;

		if (first)
			_tcscat_s(buffer, length, TEXT("("));
		else
			_tcscat_s(buffer, length, TEXT(","));

		_tcscat_s(buffer, length, s_str);

		first = 0;
	}

	if (!first)
		_tcscat_s(buffer, length, TEXT(")"));
}

/**	�Ƚ��ö��ֵ�˳�����ʹ��
 */
int CompareTopZiItem(const TOPZIITEM *item1, const TOPZIITEM *item2)
{
	if (item1->syllable.con != item2->syllable.con)
		return item1->syllable.con - item2->syllable.con;

	if (item1->syllable.vow != item2->syllable.vow)
		return item1->syllable.vow - item2->syllable.vow;

	return 0;
}

/**	д���ö����ļ�
 */
void SaveTopZiData()
{
	TCHAR name[MAX_PATH];
	FILE *fw;
	int  i, j;
	HZ   top_zi[MAX_TOPZI + 1];
	TCHAR head_msg[500] = {0xFEFF, 0};

	static TCHAR *topzi_header_msg =
		TEXT("#�̶��ֶ����ļ�\n")
		TEXT("#��ʽ��\n")
		TEXT("#	ƴ��=�̶���(��)\n")
		TEXT("#	һ�����ݿ��Զ������8���̶��֣���8��֮��ģ���������\n")
		TEXT("#�磺\n")
		TEXT("#	wo=��\n")
		TEXT("#	d=�ĵص�\n")
		TEXT("#	de=�ĵص�\n\n")
		TEXT("#������ʱ,����ֱ�Ӱ� ctrl+shift+���� �����������ֵĿ��ٹ̶���\n")
		TEXT("#���̶������ڵ�һλ,����˳�����С�\n");

	if (!share_segment)
		return;

	_tcscat_s(head_msg, _SizeOf(head_msg), topzi_header_msg);

	//����û�topzi�ļ�����
	GetFileFullName(TYPE_USERAPP, TOPZI_FILE_NAME, name);
	if (!SaveToFile(name, head_msg, (int)_tcslen(head_msg) * sizeof(TCHAR)))
		return;

	fw = _tfopen(name, TEXT("ab"));
	if (!fw)
		return;

	//��topzi�����������
	qsort(share_segment->topzi_table, share_segment->topzi_table_items, sizeof(share_segment->topzi_table[0]), CompareTopZiItem);

	for (i = 0; i < share_segment->syllable_map_items; i++)
	{
		SYLLABLE s;
		if (share_segment->syllable_map[i].fuzzy_flag)			//ģ�����������
			continue;
		//if (share_segment->syllable_map[i].correct_flag)
		//	continue;
		s.con  = share_segment->syllable_map[i].con;
		s.vow  = share_segment->syllable_map[i].vow;
		s.tone = TONE_0;

		GetTopZi(s, top_zi);
		_ftprintf(fw, TEXT("%s="), share_segment->syllable_map[i].pin_yin);

		for (j = 0; top_zi[j] && j < MAX_TOPZI; j++)
			_ftprintf(fw, TEXT("%c"), top_zi[j]);

		_ftprintf(fw, TEXT("\n"));
	}

	fclose(fw);
}


/**	����ǰ��ѡ�趨Ϊ�̶��֣���һ����
 */
void SetFixTopZi(PIMCONTEXT *context, int no)
{
	int index = context->candidate_index + no;
	int i;
	HZ  zi;
	TOPZIITEM *item;
	SYLLABLE syllable;

	//��ʹ���ö���
	if (!(pim_config->hz_option & HZ_USE_FIX_TOP))
		return;

	if (no >= context->candidate_page_count)
		return;

	//��ѡ����
	if (index >= context->candidate_count)
		return;

	//��֪�ֵ�����
	i = context->syllable_pos;
	if (i >= context->syllable_count)
		return;

	//�Ƿ�Ϊ��
	if (context->candidate_array[index].type != CAND_TYPE_ZI)
		return;

	//�ҵ�����
	syllable = context->syllables[i];

	zi = LOWORD(context->candidate_array[index].hz.item->hz);

	//Ѱ���ö��ֵ�λ��
	//��������Ѱ��
	for (item = 0, i = 0; i < share_segment->topzi_table_items; i++)
	{
		if (*(short*)&syllable == *(short*)&share_segment->topzi_table[i].syllable)	//�ҵ�
		{
			item = &share_segment->topzi_table[i];
			break;
		}
	}

	if (!item)			//û���ҵ�����ʹ�����һ��
	{
		if (share_segment->topzi_table_items >= _SizeOf(share_segment->topzi_table))
			return;		//û�п�λ��

		item = &share_segment->topzi_table[share_segment->topzi_table_items++];
		item->syllable = syllable;
		memset(item->top_zi, 0, sizeof(item->top_zi));
	}

	//���ּӵ�ǰ�棬����������ͬ����
	for (i = 0; i < MAX_TOPZI; i++)
		if (item->top_zi[i] == zi)
		{
			item->top_zi[i] = 0;
			break;
		}

	for (; i > 0; i--)
		item->top_zi[i] = item->top_zi[i - 1];

	item->top_zi[0] = zi;

	SaveTopZiData();

	context->modify_flag |= MODIFY_COMPOSE;
	MakeCandidate(context);

	return;
}