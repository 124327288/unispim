/*	�ʻ㴦��ģ��
 */
#include <assert.h>
#include <tchar.h>
#include <kernel.h>
#include <syllable.h>
#include <wordlib.h>
#include <config.h>
#include <utility.h>
#include <zi.h>
#include <ci.h>
#include <share_segment.h>

//#pragma	data_seg(HYPIM_SHARED_SEGMENT)
//CICACHE	ci_cache = {CI_CACHE_V66_SIGNATURE, 0, 0, 0};
//int		ci_cache_loaded   = 0;				//�Ƿ��Ѿ�װ��
//int		ci_cache_modified = 0;				//�Ƿ�ı�
//
//NEWCI	new_ci = {0, {0},};
//int		new_ci_loaded	= 0;				//�Ƿ�װ��
//int		new_ci_modified = 0;				//�Ƿ��Ѿ��޸�
//#pragma data_seg()

/*	�����Cache���ݡ�
 *	������
 *		ci_cache_file_name			��Cache�ļ�����
 *	���أ�
 *		�ɹ���1
 *		ʧ�ܣ�0
 */
int SaveCiCacheData(const TCHAR *ci_cache_file_name)
{
	if (!share_segment->ci_cache_modified)
		return 1;

	share_segment->ci_cache_modified = 0;
	if (!SaveToFile(ci_cache_file_name, &share_segment->ci_cache, sizeof(share_segment->ci_cache)))
	{
		Log(LOG_ID, L"�����Cache����ʧ�ܣ�file:%s", ci_cache_file_name);
		return 0;
	}

	return 1;
}

void CheckAndUpgradeCiCache()
{
	if (0 == share_segment->ci_cache.length || CI_CACHE_V66_SIGNATURE == share_segment->ci_cache.signature)
		return;

	if (CI_CACHE_SIGNATURE == share_segment->ci_cache.signature)
	{
		int pos = 0;
		int item_length = (char)share_segment->ci_cache.cache[pos] * sizeof(HZ);

		share_segment->ci_cache.signature = CI_CACHE_V66_SIGNATURE;

		//�����ʵ�cache
		while(pos + item_length <= share_segment->ci_cache.length)		//��Ҫ�ж����һ��������Խ��
		{
			char ci_ansi[MAX_WORD_LENGTH * sizeof(HZ)] = {0};
			TCHAR ci_uc[MAX_WORD_LENGTH] = {0};

			memcpy_s(ci_ansi, MAX_WORD_LENGTH * sizeof(HZ), &share_segment->ci_cache.cache[pos + WORDLIB_FEATURE_LENGTH], item_length);

			AnsiToUtf16(ci_ansi, ci_uc, MAX_WORD_LENGTH);

			memcpy_s(&share_segment->ci_cache.cache[pos + WORDLIB_FEATURE_LENGTH], item_length, ci_uc, item_length);

			//��һ����
			pos += WORDLIB_FEATURE_LENGTH + share_segment->ci_cache.cache[pos] * sizeof(HZ);
			item_length = (char)share_segment->ci_cache.cache[pos] * sizeof(HZ);
		}
	}
	else
	{
		share_segment->ci_cache.signature	 = CI_CACHE_V66_SIGNATURE;
		share_segment->ci_cache.length		 = 0;
		share_segment->ci_cache.max_used_id  = 0;
	}

	return;
}

/*	װ�ش�Cache���ݡ�
 *	������
 *		ci_cache_file_name			��Cache�ļ�����
 *	���أ�
 *		�ɹ���1
 *		ʧ�ܣ�0
 */
int LoadCiCacheData(const TCHAR *ci_cache_file_name)
{
	int length;

	assert(ci_cache_file_name);

	//��ʼ�����ݣ������ļ�װ��ʧ����ɳ������
	share_segment->ci_cache.length = share_segment->ci_cache.max_used_id = 0;

	length = LoadFromFile(ci_cache_file_name, (char*)&share_segment->ci_cache, sizeof(share_segment->ci_cache));
	if (length < 0)
	{
		share_segment->ci_cache.length = share_segment->ci_cache.max_used_id = 0;
		Log(LOG_ID, L"��ȡ��Cache�ļ�����file:%s, length=%d", ci_cache_file_name, length);
		return 0;
	}

	CheckAndUpgradeCiCache();

	share_segment->ci_cache_loaded = 1;
	return 1;
}

/**	�ͷŴ�Cache����
 */
int FreeCiCacheData()
{
	share_segment->ci_cache_loaded = 0;
	return 1;
}

/*	�ڴ�Cache���ҳ��ʵ�λ�á�
 *	������
 *		hz			��
 *		length		����
 *	���أ�
 *		-1��û���ҵ�
 *		>0 �ô���Cache�е�λ�ã�ԽСԽ�£�
 */
int GetCiCacheInfo(HZ *hz, int length)
{
	int pos = 0;
	int item_length = WORDLIB_FEATURE_LENGTH + sizeof(HZ) * length;

	//�����ʵ�cache
	while(pos + item_length <= share_segment->ci_cache.length)		//��Ҫ�ж����һ��������Խ��
	{
		if ((char)share_segment->ci_cache.cache[pos] == (char)length &&										//������ͬ
			!memcmp(&share_segment->ci_cache.cache[pos + WORDLIB_FEATURE_LENGTH], hz, length * sizeof(HZ)))	//������ͬ
			return pos;

		//��һ����
		pos += WORDLIB_FEATURE_LENGTH + share_segment->ci_cache.cache[pos] * sizeof(HZ);
	}

	//û���ҵ�
	return -1;
}

/*	��Cache��ʹ�öȵ����ٽ�ֵ����Ҫ�����еĴʵ�ʹ�öȼ��١�
 *	��������
 *	���أ���
 */
void ReduceCiCacheUsedCount()
{
	int pos = 0;

	//��������
	while(pos < share_segment->ci_cache.length)
	{
		int used_count;
		int length = share_segment->ci_cache.cache[pos];

		if ((int)(pos + WORDLIB_FEATURE_LENGTH + sizeof(HZ) * length) >= share_segment->ci_cache.length)	//Խ��
			break;

		used_count = *(int*)&share_segment->ci_cache.cache[pos] >> 8;
		used_count -= CI_MAX_USED_COUNT / 2;		//����һ���ֵ
		*(int*)&share_segment->ci_cache.cache[pos] = (used_count << 8) + length;

		pos += WORDLIB_FEATURE_LENGTH + sizeof(HZ) * length;
	}
}

/*	��ʵ�cache�в����µĴʡ�
 *	������
 *		hz			��
 *		length		����
 *	���أ���
 */
void InsertCiToCache(HZ *hz, int length, int syllable_length, int set_fix_top)
{
	int pos;						//Cache�е�λ��
	int item_length;				//�������ĳ���
	int used_count;					//�ʻ�ʹ�ö�
	int i;
	char item_save[WORDLIB_FEATURE_LENGTH + sizeof(HZ) * MAX_WORD_LENGTH];		//��ʱ������

	//����Ե�ǰ��λ��ȷ���ʵ�˳���������Ƶ����������
	if (pim_config->ci_option & CI_ADJUST_FREQ_NONE)
		return;

	if (length > MAX_WORD_LENGTH)
		return;

	//UCS4�Ĵʲ�����Cache
	if (length != syllable_length)
		return;

	item_length = length * sizeof(HZ) + WORDLIB_FEATURE_LENGTH;

	//����������Ƿ������Cache��?
	pos = GetCiCacheInfo(hz, length);

	if (pos != -1)		//�ҵ�
	{
		used_count = *(int*)&share_segment->ci_cache.cache[pos] >> 8;		//�ҳ��ʵ�ʹ�ö�
		used_count++;
	}
	else		//û���ҵ��Ļ����������뵽cacheĩβ��
	{
		used_count = 1;
		pos = share_segment->ci_cache.length;
		share_segment->ci_cache.length	   = pos + item_length;
		share_segment->ci_cache.cache[pos] = (char)length;

		for (i = 0; i < (int)(length * sizeof(HZ)); i++)
			share_segment->ci_cache.cache[pos + i + WORDLIB_FEATURE_LENGTH] = *((char*)hz + i);	//�����ֿ�����Cache
	}

	if (set_fix_top)
		used_count = CI_TOP_USED_COUNT;
	else if (CI_TOP_USED_COUNT == used_count)
		used_count++;

	*(int*)&share_segment->ci_cache.cache[pos] = (used_count << 8) + share_segment->ci_cache.cache[pos];	//����ʹ�ö�

	//����˳��
	//(0)(1)....(pos-1)(pos)(pos+1)...(cache_length-1)  ====> (pos)(0)(1)...(pos-1)(pos+1)...(cache_length-1)
	//���浱ǰ����
	for (i = 0; i < item_length; i++)
		item_save[i] = share_segment->ci_cache.cache[pos + i];

	for (i = pos - 1; i >= 0; i--)
		share_segment->ci_cache.cache[i + item_length] = share_segment->ci_cache.cache[i];

	for (i = 0; i < item_length; i++)
		share_segment->ci_cache.cache[i] = item_save[i];

	//��Խ��Ĵ�Cache���ȼ��ٻ���
	if (share_segment->ci_cache.length > CI_CACHE_SIZE)
		share_segment->ci_cache.length = CI_CACHE_SIZE;

	//���ʹ�ö�Խ�磬����Ҫ�����д����ʹ�öȼ���
	if (used_count > CI_MAX_USED_COUNT)
		ReduceCiCacheUsedCount();

	share_segment->ci_cache_modified = 1;
	return;
}

/**	���شʵ�ʹ�ö�
 */
int GetCiUsedCount(int cache_pos)
{
	if (cache_pos < 0)
		return 0;

	return *(int*)&share_segment->ci_cache.cache[cache_pos] >> 8;
}

/*	�Ƚϴ��еĺ����Լ���Ƶ�����ڴ���ʵĺ�ѡ˳��
 *	�ȽϹ��̣�
 *		1. �Ƚ���������Cache�е�λ��
 *		2. �Ƚ������ʵĴ�Ƶ
 *	������
 *		cand1, cand2		���ڱȽϵ�������ѡָ��
 *	���أ�
 *		��һ���ȵڶ���С <0 ��ʾcand1����cand2ǰ
 *		��һ�����ڵڶ��� =0
 *		��һ�����ڵڶ��� >0 ��ʾcand2����cand1ǰ
 */
static int CompareCi(const CANDIDATE *cand1, const CANDIDATE *cand2)
{
	int cache_pos1, cache_pos2;
	int used_count1, used_count2;

	//����ʽ���ö������ڷ��ö���ǰ���������ö��ʻ��߶������ö��ʣ����Ƿ�
	//�����˿��ٵ�����Ƶ�������ã���Cache��λ�ÿ�ǰ�Ĵ�����ǰ��(�½�ʹ�ò�
	//����Cache�Ĵ�)��λ�ÿ���Ĵ����ں��棬����Cache�еĴ����������δ��
	//�ã����Ƿ����������ٴ�Ƶ�����������ã�ʹ�öȸߵĴ�����ǰ�棬ʹ�öȵ�
	//�Ĵ����ں��棬����Cache�еĴ����������δ���ã���Ƶ�ߵ�����ǰ�棬��
	//Ƶ�͵����ں���

	//�ر�ע��CiCache��ZiCache�ṹ����ܴ���cache_pos����Ҳ��ͬ����
	//�����Ķ��䶨��

	cache_pos1  = cand1->word.cache_pos;
	cache_pos2  = cand2->word.cache_pos;
	used_count1 = GetCiUsedCount(cache_pos1);
	used_count2 = GetCiUsedCount(cache_pos2);

	if (CI_TOP_USED_COUNT == used_count1 && CI_TOP_USED_COUNT != used_count2)
		return -1;
	else if (CI_TOP_USED_COUNT != used_count1 && CI_TOP_USED_COUNT == used_count2)
		return 1;

	//���ٵ�����Ƶ
	if (pim_config->ci_option & CI_ADJUST_FREQ_FAST)
	{
		//����ͬһ����
		if (cache_pos1 != cache_pos2)
		{
			//cand1����Cache��(cand2������ͬʱ����Cache�У�
			//�������������if����)��cand2��Cache�У�cand1
			//���ں���
			if (cache_pos1 == -1)
				return 1;

			//cand1��Cache�У�cand2����Cache�У�cand1����ǰ��
			//(����-1)
			if (cache_pos2 == -1)
				return -1;

			//��Cache��λ�ÿ�ǰ������ǰ��
			return cache_pos1 - cache_pos2;
		}
	}

	//���ٴ�Ƶ����
	if (pim_config->ci_option & CI_ADJUST_FREQ_SLOW)
	{
		if (cache_pos1 != cache_pos2)
		{
			if (cache_pos1 == -1)
				return 1;

			if (cache_pos2 == -1)
				return -1;

			if (used_count1 != used_count2)
				return used_count2 - used_count1;
		}
	}

	//�Ƚϴ�Ƶ
	return cand2->word.item->freq - cand1->word.item->freq;
}

/*	�Ƚϴ��еĺ����Լ���Ƶ�����ڴ���ʵ�����
 *	������
 *		cand1, cand2		���ڱȽϵ�������ѡָ��
 *	���أ�
 *		��һ���ȵڶ���С <0
 *		��һ�����ڵڶ��� =0
 *		��һ�����ڵڶ��� >0
 */
static int CompareCiCodeFreq(const CANDIDATE *cand1, const CANDIDATE *cand2)
{
	int ret;

	assert(cand1 && cand2);

	//���Ȳ�ͬ
	if (cand1->word.item->ci_length != cand2->word.item->ci_length)
		return cand2->word.item->ci_length - cand1->word.item->ci_length;

	//����ʵ��ֲ�ͬ�򷵻������ʵ������
	ret = memcmp(cand2->word.hz, cand1->word.hz, sizeof(HZ) * cand1->word.item->ci_length);
	if (ret)
		return ret;

	//�������������һ���Ǳ�ɾ���ģ�������ǰ��
	if (cand1->word.item->effective && !cand2->word.item->effective)
		return 1;

	if (cand2->word.item->effective && !cand1->word.item->effective)
		return -1;

	//�Ƚ���Ƶ
	return cand2->word.item->freq - cand1->word.item->freq;
}

/*	�Դʽ�������
 *	������
 *		candidates		�ʺ�ѡ
 *		count			��ѡ����
 *	���أ���
 */
void SortCiCandidates(CANDIDATE *candidates, int count)
{
	if (count < 2)
		return;

	qsort(candidates, count, sizeof(CANDIDATE), CompareCi);
}

/*	�ʺ�ѡ���ء�
 *	������
 *		candidates			�ʺ�ѡ����
 *		count				���鳤��
 *	���أ�
 *		���غ�ĺ�ѡ��Ŀ
 */
int UnifyCiCandidates(CANDIDATE *candidates, int count)
{
	int new_count, i;

	if (!count)		//�������´ʲ�������
		return count;

	//��hz����˳���������������ͬ�Ĵʻ㽫����һ���λ��
	qsort(candidates, count, sizeof(CANDIDATE), CompareCiCodeFreq);

	//����
	for (new_count = 1, i = 1; i < count; i++)
	{
		if (candidates[i].word.item->ci_length == candidates[i - 1].word.item->ci_length &&
			!memcmp(candidates[i].word.hz, candidates[i - 1].word.hz, sizeof(HZ) * candidates[i].word.item->ci_length))
			continue;

		candidates[new_count] = candidates[i];
		new_count++;
	}

	count = new_count;

	//ɾ���Ѿ�ɾ���Ĵʻ�
	for (new_count = 0, i = 0; i < count; i++)
	{
		if (!candidates[i].word.item->effective)	//�ҵ���ɾ���Ĵ���
			continue;

		candidates[new_count++] = candidates[i];
	}

	return new_count;
}

/*	�����һ���ʵ�ָ�롣
 *	������
 *		item		��ǰ��ָ��
 *	���أ�
 *		��һ���ʵ�ָ��
 */
WORDLIBITEM *GetNextCiItem(WORDLIBITEM *item)
{
	//feature + syllable + hz
	return (WORDLIBITEM*)((char*)item +	sizeof(int) + sizeof(SYLLABLE) * item->syllable_length + sizeof(HZ) * item->ci_length);
}

/*	V6B1�ʿ⣬�����һ���ʵ�ָ�롣
 *	������
 *		item		��ǰ��ָ��
 *	���أ�
 *		��һ���ʵ�ָ��
 */
WORDLIBITEMV6B1 *GetNextCiItemV6B1(WORDLIBITEMV6B1 *item)
{
	//feature + syllable + hz
	return (WORDLIBITEMV6B1*)((char*)item +	sizeof(int) + sizeof(SYLLABLE) * item->length +	sizeof(HZ) * item->length);
}

/**	��ôʻ�ĺ���ָ��
 */
HZ *GetItemHZPtr(WORDLIBITEM *item)
{
	return (HZ*)((char*)item + sizeof(int) + sizeof(SYLLABLE) * item->syllable_length);
}

/**	��ôʻ������ָ��
 */
SYLLABLE *GetItemSyllablePtr(WORDLIBITEM *item)
{
	return (SYLLABLE*)((char*)item + sizeof(int));
}

/**	���ڴʳ��Լ����ڳ���ȷ������ĳ���
 */
int GetItemLength(int hz_length, int syllable_length)
{
	return sizeof(HZ) * hz_length + sizeof(SYLLABLE) * syllable_length + WORDLIB_FEATURE_LENGTH;
}

/*	���ҳ�еĴʺ�ѡ��û��ͨ�������
 *	������
 *		page					ҳָ��
 *		syllable_array			��������
 *		syllable_count			���ڳ���
 *		candidate_array			��ѡ����
 *		candidate_length		��ѡ���鳤��
 *		fuzzy_mode				ģ����ѡ��
 *	���أ�
 *		��ѡ�ʻ���Ŀ
 */
int GetCiInPage(PAGE *page,	SYLLABLE *syllable_array, int syllable_count, CANDIDATE *candidate_array, int candidate_length,	int fuzzy_mode)
{
	WORDLIBITEM *item;				//����
	int count = 0;					//��ѡ����

	if (!candidate_length)
		return 0;

	//���ҳ��û�а���������ȵĴʣ���ֱ�ӷ��أ�Ϊ��߼���Ч�ʣ�
	if (!(page->length_flag & (1 << syllable_count)))
		return 0;

	//����ҳ��
	for (item = (WORDLIBITEM*) page->data; (char*)item < (char*) &page->data + page->data_length; item = GetNextCiItem(item))
	{
		if (!(fuzzy_mode & FUZZY_CI_SYLLABLE_LENGTH))
			if (item->ci_length != item->syllable_length)
				continue;

		//�жϳ����Ƿ����
		if (item->syllable_length != syllable_count)
			continue;

		//�ж������Ƿ����
		if (!CompareSyllables(syllable_array, item->syllable, syllable_count, fuzzy_mode))
			continue;

		//�ҵ�!
		candidate_array[count].word.type	  = CI_TYPE_NORMAL;
		candidate_array[count].word.item	  = item;
		candidate_array[count].word.cache_pos = 0;
		candidate_array[count].word.syllable  = (SYLLABLE*)((char*)item + sizeof(int));
		candidate_array[count].word.hz		  = GetItemHZPtr(item);

		count++;
		if (count >= candidate_length)
			break;
	}

	return count;
}

/*	���ҳ�еĴʺ�ѡ��ʹ��ͨ�������
 *	������
 *		page					ҳָ��
 *		syllable_array			��������
 *		syllable_count			���ڳ���
 *		candidate_array			��ѡ����
 *		candidate_length		��ѡ���鳤��
 *		fuzzy_mode				ģ����ѡ��
 *	���أ�
 *		��ѡ�ʻ���Ŀ
 */
int GetCiInPageWild(PAGE *page,	SYLLABLE *syllable_array, int syllable_count, CANDIDATE *candidate_array, int candidate_length,	int fuzzy_mode)
{
	WORDLIBITEM *item;				//����
	int count = 0;					//��ѡ����

	if (!(pim_config->ci_option & CI_WILDCARD))
		return 0;

	if (!candidate_length)
		return 0;

	//���ҳ��û�а���������ȵĴʣ���ֱ�ӷ��أ�Ϊ��߼���Ч�ʣ�
	if ((unsigned int)page->length_flag < (unsigned int)(1 << syllable_count))
	{
		if (page->length_flag != 1)		//1 << 32 = 1, ���Ա��������ж�
			return 0;
	}

	//����ҳ��
	for (item = (WORDLIBITEM*) page->data; (char*)item < (char*) &page->data + page->data_length; item = GetNextCiItem(item))
	{
		if (!(fuzzy_mode & FUZZY_CI_SYLLABLE_LENGTH))
			if (item->ci_length != item->syllable_length)
				continue;

		//�жϳ����Ƿ����
		if ((int)item->syllable_length < syllable_count)
			continue;

		//�ж������Ƿ����
		if (!WildCompareSyllables(syllable_array, syllable_count, item->syllable, item->syllable_length, fuzzy_mode))
			continue;

		//�ҵ�!
		candidate_array[count].word.type	  = CI_TYPE_NORMAL;
		candidate_array[count].word.item	  = item;
		candidate_array[count].word.cache_pos = 0;
		candidate_array[count].word.syllable  = (SYLLABLE*)((char*)item + sizeof(int));
		candidate_array[count].word.hz		  = GetItemHZPtr(item);

		count++;
		if (count >= candidate_length)
			break;
	}

	return count;
}

/*	ʹ������ĸ�����ҳ�еĴʺ�ѡ��û��ͨ�������
 *	������
 *		page					ҳָ��
 *		letters					��������
 *		letter_count			���ڳ���
 *		candidate_array			��ѡ����
 *		candidate_length		��ѡ���鳤��
 *		fuzzy_mode				ģ����ѡ��
 *	���أ�
 *		��ѡ�ʻ���Ŀ
 */
int GetCiInPageByLetter(PAGE *page, const TCHAR *letters, int letter_count, CANDIDATE *candidate_array, int candidate_length)
{
	WORDLIBITEM *item;				//����
	int count = 0;					//��ѡ����

	if (!candidate_length || !letters)
		return 0;

	//���ҳ��û�а���������ȵĴʣ���ֱ�ӷ��أ�Ϊ��߼���Ч�ʣ�
	if (!(page->length_flag & (1 << letter_count)))
		return 0;

	//����ҳ��
	for (item = (WORDLIBITEM*) page->data; (char*)item < (char*) &page->data + page->data_length; item = GetNextCiItem(item))
	{
		//�жϳ����Ƿ����
		if (letter_count != item->syllable_length)
			continue;

		//�ж������Ƿ����
		if (!CompareSyllablesAndLetters(letters, item->syllable, letter_count))
			continue;

		//�ҵ�!
		candidate_array[count].word.type	  = CI_TYPE_LETTER;
		candidate_array[count].word.item	  = item;
		candidate_array[count].word.cache_pos = 0;
		candidate_array[count].word.syllable  = GetItemSyllablePtr(item);
		candidate_array[count].word.hz		  = GetItemHZPtr(item);

		count++;
		if (count >= candidate_length)
			break;
	}

	return count;
}

/*	ʹ������ĸ�����ҳ�еĴʺ�ѡ��ʹ��ͨ�������
 *	������
 *		page					ҳָ��
 *		letters					��������
 *		letter_count			���ڳ���
 *		candidate_array			��ѡ����
 *		candidate_length		��ѡ���鳤��
 *	���أ�
 *		��ѡ�ʻ���Ŀ
 */
int GetCiInPageByWildLetter(PAGE *page, const TCHAR *letters, int letter_count,	CANDIDATE *candidate_array,	int candidate_length)
{
	WORDLIBITEM *item;				//����
	int count = 0;					//��ѡ����

	if (!candidate_length || !letters)
		return 0;

	//���ҳ��û�д��ڵ���������ĸ�����Ĵʣ���ֱ�ӷ��أ�Ϊ��߼���Ч�ʣ�
	if ((unsigned int)page->length_flag < (unsigned int)(1 << letter_count))
	{
		if (page->length_flag != 1)
			return 0;
	}

	//����ҳ��
	for (item = (WORDLIBITEM*) page->data; (char*)item < (char*) &page->data + page->data_length; item = GetNextCiItem(item))
	{
		//�жϳ����Ƿ����
		if (letter_count < pim_config->first_letter_input_min_hz)
		{
			if (item->syllable_length != letter_count)
				continue;
		}
		else if ((int)item->syllable_length < pim_config->first_letter_input_min_hz)
			continue;

		//�ж������Ƿ����
		if (!WildCompareSyllablesAndLetters(letters, (int)_tcslen(letters), item->syllable, item->syllable_length))
			continue;

		//�ҵ�!
		candidate_array[count].word.type	  = CI_TYPE_LETTER;
		candidate_array[count].word.item	  = item;
		candidate_array[count].word.cache_pos = 0;
		candidate_array[count].word.syllable  = GetItemSyllablePtr(item);
		candidate_array[count].word.hz		  = GetItemHZPtr(item);

		count++;
		if (count >= candidate_length)
			break;
	}

	return count;
}

/*	��ôʻ��ѡ��
 *	������
 *		wordlib_id				�ʿ��ʶ
 *		syllable_array			��������
 *		syllable_count			���ڳ���
 *		candidate_array			��ѡ����
 *		candidate_length		��ѡ���鳤��
 *		fuzzy_mode				ģ����ѡ��
 *	���أ�
 *		��ѡ�ʻ���Ŀ
 */
int GetCiCandidates(int wordlib_id, SYLLABLE *syllable_array, int syllable_count, CANDIDATE *candidate_array, int candidate_length, int fuzzy_mode)
{
	WORDLIB *wordlib;			//�ʿ�ָ��
	int page_no;				//ҳ��
	int count = 0;				//��ѡ����
	int i, j;
	SYLLABLE syllable_tmp1, syllable_tmp2;
	int wild_compare = 0;								//�Ƿ�ʹ��ͨ��Ƚ�

	if (!candidate_length || wordlib_id < 0)
		return 0;

	//���ڹ��ٻ��߹���
	if (syllable_count < 2 || syllable_count > MAX_WORD_LENGTH)
		return 0;				

	wordlib = GetWordLibrary(wordlib_id);
	if (!wordlib)				//û������ʿ�
		return 0;

	//�鿴�Ƿ���Ҫ����ͨ��Ƚ�
	for (i = 0; i < syllable_count; i++)
		if (syllable_array[i].con == CON_ANY)		//����ͨ���
			break;

	wild_compare = i != syllable_count;

	//���ڱȽϵ���ʱ���ڳ�ʼ��
	syllable_tmp1.con  = syllable_tmp2.con  = CON_NULL;
	syllable_tmp1.vow  = syllable_tmp2.vow  = VOW_NULL;
	syllable_tmp1.tone = syllable_tmp2.tone = TONE_0;

	//������ģ�����Ĵ��ڣ����Ա��������ĸ�ı��������ҳ�ȫ����ȷ����ĸ��
	for (i = CON_NULL; i < CON_END; i++)
	{
		syllable_tmp1.con = i;
		if (!ContainCon(syllable_array[0], syllable_tmp1, fuzzy_mode))
			continue;

		//�ҵ���һ����ͬ������
		for (j = CON_NULL; j < CON_END; j++)
		{
			syllable_tmp2.con = j;
			if (!ContainCon(syllable_array[1], syllable_tmp2, fuzzy_mode))
				continue;

			//�������еĴʿ�ҳ����
			page_no = wordlib->header.index[i][j];

			//����ҳ���ҳ��ʺ�ѡ
			while(page_no != PAGE_END)
			{
				count += wild_compare
					?
						GetCiInPageWild(			//ʹ��ͨ���
							&wordlib->pages[page_no],
							syllable_array,
							syllable_count,
							candidate_array + count,
							candidate_length - count,
							fuzzy_mode)
					:
						GetCiInPage(				//��ʹ��ͨ���
							&wordlib->pages[page_no],
							syllable_array,
							syllable_count,
							candidate_array + count,
							candidate_length - count,
							fuzzy_mode);

				if (count >= candidate_length)
					break;

				page_no = wordlib->pages[page_no].next_page_no;
			}

			if (count >= candidate_length)
				break;
		}
	}

	//���ôʿ��ʶ
	for (i = 0; i < count; i++)
	{
		candidate_array[i].word.source = wordlib_id;
		candidate_array[i].type		   = CAND_TYPE_CI;
	}

	return count;
}

/*	��ôʻ��ѡ��
 *	������
 *		wordlib_id				�ʿ��ʶ
 *		letters					����ĸ��
 *		candidate_array			��ѡ����
 *		candidate_length		��ѡ���鳤��
 *	���أ�
 *		��ѡ�ʻ���Ŀ
 */
int GetCiCandidatesByLetter(int wordlib_id, const TCHAR *letters, CANDIDATE *candidate_array, int candidate_length)
{
	WORDLIB *wordlib;							//�ʿ�ָ��
	SYLLABLE syllable_tmp1, syllable_tmp2;		//��ʱ����
	int page_no;								//ҳ��
	int count = 0;								//��ѡ����
	int length = 0;								//����ĸ��Ŀ
	int wild_compare = 0;						//�Ƿ�ʹ��ͨ��Ƚ�
	int i, j, k, l;

	assert(letters);

	//�������ĸ�Ϸ���
	while(letters[length])
	{
		if (letters[length] == SYLLABLE_ANY_CHAR)
			wild_compare = 1;
		else if (letters[length] < 'a' || letters[length] > 'z')
			return 0;
		else if (letters[length] == 'i' || letters[length] == 'v' || letters[length] == 'u')
			return 0;

		length++;
	};

	//���ڹ�С������
	if (length < 2 || length > MAX_WORD_LENGTH)
		return 0;

	wordlib = GetWordLibrary(wordlib_id);
	if (!wordlib)				//û������ʿ�
		return 0;

	//������ĸ�ı������ҳ�ȫ����ȷ����ĸ��
	for (i = CON_NULL; i < CON_END; i++)
	{
		for (j = VOW_NULL; j < VOW_END; j++)
		{
			syllable_tmp1.con = i;
			syllable_tmp1.vow = j;
			if (!SyllableStartWithLetter(letters[0], syllable_tmp1))
				continue;

			j = VOW_END;			//һ���ҵ����Ͳ������ڲ�ѭ��

			//�ҵ���һ����ͬ������
			for (k = CON_NULL; k < CON_END; k++)
			{
				for (l = VOW_NULL; l < VOW_END; l++)
				{
					syllable_tmp2.con = k;
					syllable_tmp2.vow = l;

					if (!SyllableStartWithLetter(letters[1], syllable_tmp2))
						continue;

					l = VOW_END;		//һ���ҵ����Ͳ������ڲ�ѭ��

					//�������еĴʿ�ҳ����
					page_no = wordlib->header.index[i][k];

					//����ҳ���ҳ��ʺ�ѡ
					while(page_no != PAGE_END)
					{
						count += wild_compare
							?	GetCiInPageByWildLetter(			//ʹ��ͨ���
									&wordlib->pages[page_no],
									letters,
									length,
									candidate_array + count,
									candidate_length - count)
							:	GetCiInPageByLetter(				//��ʹ��ͨ���
									&wordlib->pages[page_no],
									letters,
									length,
									candidate_array + count,
									candidate_length - count);

						page_no = wordlib->pages[page_no].next_page_no;
					}
				}
			}
		}
	}

	//���ôʿ��ʶ
	for (i = 0; i < count; i++)
	{
		candidate_array[i].word.source = wordlib_id;
		candidate_array[i].type		   = CAND_TYPE_CI;
	}

	return count;
}

/*	����ʻ㱻�û�ѡ�к�Ĳ��������뵽Cache�У������û�ʹ�öȡ�
 *	������
 *		ci_cand			�ʺ�ѡ
 *	���أ���
 */
void ProcessCiSelected(SYLLABLE *syllable, int syllable_length, HZ *hz, int hz_length)
{
/*
	//���뵽�û��ʿ��У����Ҹ��´�����Ϣ
	if (pim_config->insert_used_word_to_user_wl)
		AddCi(syllable, syllable_length, hz, hz_length);
*/
	//���뵽��cache��
	InsertCiToCache(hz, hz_length, syllable_length, 0);
}

/*	��������ɾ������ȷ�Ĵʡ�
 *	һ��ʿ��еĴʶ���������������Ҫ�����ֵķ��顣
 *	�磺z4f �ܹ�����Ĵʰ��������������������ֲ�û����������˱������
 *
 *	������
 *		syllable_array			��������
 *		syllable_count			���ڼ���
 *		candidate_array			��ѡ����
 *		candidate_count			��ѡ��Ŀ
 *	���أ�
 *		ʣ���ѡ��Ŀ
 */
int DeleteCiCandidateByTone(SYLLABLE *syllable_array, int syllable_count, CANDIDATE *candidate_array, int candidate_count)
{
	int i, j;
	int new_count = 0;
	int has_tone = 0, has_wild = 0;

	//��������Ƿ��������
	for (i = 0; i < syllable_count; i++)
	{
		if (syllable_array[i].tone != TONE_0)
			has_tone = 1;

		if (syllable_array[i].con == CON_ANY)
			has_wild = 1;
	}

	if (!has_tone)		//û�д��������������߰���ͨ������Ѿ��ų�����ֱ�ӷ���
		return candidate_count;

	if (has_wild)
	{
		for (i = 0; i < candidate_count; i++)
		{
			if (!WildCompareSyllablesWithCi(syllable_array, syllable_count, candidate_array[i].word.syllable, candidate_array[i].word.hz, candidate_array[i].word.item->syllable_length, 0))
			{
				candidate_array[i] = candidate_array[candidate_count - 1];
				candidate_count--;
				i--;
				continue;
			}
		}

		return candidate_count;
	}

	for (i = 0; i < candidate_count; i++)
	{
		for (j = 0; j < syllable_count; j++)
		{
			if (syllable_array[j].tone != TONE_0 && !ZiContainTone(candidate_array[i].word.hz[j], candidate_array[i].word.syllable[j], syllable_array[j].tone))
				break;
		}

		if (j != syllable_count)			//����������Ҫ��
		{
			candidate_array[i] = candidate_array[candidate_count - 1];
			candidate_count--;
			i--;

			continue;
		}
	}

	return candidate_count;
}

/*	�������ڴ���ʵĺ�ѡ��
 *		1. ��ú�ѡ�����ں�ѡ�Լ�����ĸ��ѡ��
 *		2. ����
 *		3. ����
 *	������
 *		syllable_array			��������
 *		syllable_count			���ڳ���
 *		letters					�û����봮��Ϊ0ʱΪ������
 *		candidate_array			��ѡ����
 *		candidate_length		��ѡ���鳤��
 *		same_ci_syllable_length	�Ƿ���Ҫ�������ڵĳ�����ͬ
 *	���أ�
 *		��ѡ��Ŀ
 */
int ProcessCiCandidate(SYLLABLE *syllable_array, int syllable_count, const TCHAR *letters, CANDIDATE *candidate_array, int candidate_length, int same_ci_syllable_length)
{
	int count = 0;					//��ѡ��Ŀ
	int fuzzy_mode = pim_config->use_fuzzy ? pim_config->fuzzy_mode : 0;			//ģ����ʽ
	int wordlib_id;					//�ʿ��ʶ
	int i;

	assert(syllable_array && candidate_array && syllable_count >= 1);

	//�����а����������Զ�ģ��
	if (pim_config->ci_option & CI_AUTO_FUZZY)
		fuzzy_mode |= FUZZY_ZCS_IN_CI;

	if (pim_config->pinyin_mode == PINYIN_QUANPIN && (pim_config->ci_option & CI_AUTO_VOW_FUZZY))
		fuzzy_mode |= FUZZY_SUPER;

	if (!same_ci_syllable_length)
		fuzzy_mode |= FUZZY_CI_SYLLABLE_LENGTH;

	//���ڴ��е�ȫ���ʿ��в��Һ�ѡ
	wordlib_id = GetNextWordLibId(-1);
	while (wordlib_id != -1)
	{
		count += GetCiCandidates(wordlib_id, syllable_array, syllable_count, candidate_array + count, candidate_length - count, fuzzy_mode);
		wordlib_id = GetNextWordLibId(wordlib_id);
	};

	//����������Ļ�����Ҫ�������������Ĵʳ�ȥ
	count = DeleteCiCandidateByTone(syllable_array, syllable_count, candidate_array, count);

	//��������ĸ�����룬��Ҫ�����ж��Ƿ�Ϊ1�����ڣ����Ϊһ�����ڣ�
	//���ܽ��е���ĸ���루���"�л���Ů"���⣩
	//�����Ժ󣬲��ñ����ڵ������ѡ֮��ķ������С�
	if (letters && (pim_config->ci_option & CI_USE_FIRST_LETTER) &&
		(int)_tcslen(letters) >= pim_config->first_letter_input_min_hz)
	{
		//���ڴ��е�ȫ���ʿ��в��Һ�ѡ
		wordlib_id = GetNextWordLibId(-1);
		while (wordlib_id != -1)
		{
			count += GetCiCandidatesByLetter(wordlib_id, letters, candidate_array + count, candidate_length - count);
			wordlib_id = GetNextWordLibId(wordlib_id);
		};
	}

	//����
	count = UnifyCiCandidates(candidate_array, count);

	for (i = 0; i < count; i++)
		candidate_array[i].word.cache_pos = GetCiCacheInfo(candidate_array[i].word.hz, candidate_array[i].word.item->ci_length);

	//����
	SortCiCandidates(candidate_array, count);

	return count;
}

/*	��ȡ���ܴ��ڻ�Ͻ����ĺ�ѡ��
 *	������
 *		syllable_array			��������
 *		syllable_count			���ڳ���
 *		candidate_array			��ѡ����
 *		candidate_length		��ѡ���鳤��
 *	���أ�
 *		��ѡ��Ŀ
 */
int GetMixedParseCiCandidate(SYLLABLE *syllable_array, int syllable_count, CANDIDATE *candidate_array, int candidate_length)
{
	SYLLABLE  base_syllables[3];    //���������������
	int count = 0;					//��ѡ��Ŀ
	int wordlib_id;					//�ʿ��ʶ
	int i;

	assert(syllable_array && candidate_array && syllable_count >= 1 && candidate_length > 0);

	//��Ͻ�������Ҫ4������
	if (syllable_count < 4)
		return 0;

	//������������
	for (i = 0; i < 2; i++)
		base_syllables[i] = syllable_array[i];
	base_syllables[2].con = CON_ANY;
	base_syllables[2].vow = VOW_ANY;
	base_syllables[2].tone = TONE_0;

	//Q��Ϊ�β�ʹ���κ�ģ����
	//A����kernel.c���������ô�����ע��

	//���ڴ��е�ȫ���ʿ��в��Һ�ѡ
	wordlib_id = GetUserWordLibId();
	if (wordlib_id != -1)
		count += GetCiCandidates(wordlib_id, base_syllables, 3, candidate_array + count, candidate_length - count, 0);

	//���base_syllables���������Ļ�����Ҫ��candidate_array��������ƥ��Ĵʳ�ȥ
	count = DeleteCiCandidateByTone(base_syllables, 3, candidate_array, count);

	//��Ͻ�������Ҫ4������
	for (i = 0; i < count; i++)
	{
		if (candidate_array->word.item->syllable_length < 4)
		{
			candidate_array[i] = candidate_array[count - 1];
			count--;
			i--;
		}
	}

	//ȡ�ô���Cache�е�λ��
	for (i = 0; i < count; i++)
		candidate_array[i].word.cache_pos = GetCiCacheInfo(candidate_array[i].word.hz, candidate_array[i].word.item->ci_length);

	//����
	SortCiCandidates(candidate_array, count);

	return count;
}

/*	����û��ʿ��еĴʻ��ѡ������ICW���û��ʶ�ȡ��
 *	������
 *		syllable_array			��������
 *		syllable_count			���ڳ���
 *		candidate_array			��ѡ����
 *		candidate_length		��ѡ���鳤��
 *		fuzzy_mode				ģ����ѡ��
 *	���أ�
 *		��ѡ�ʻ���Ŀ
 */
int GetUserCiCandidates(SYLLABLE *syllable_array, int syllable_count, CANDIDATE *candidate_array, int candidate_length, int fuzzy_mode)
{
	return GetCiCandidates(GetUserWordLibId(), syllable_array, syllable_count, candidate_array, candidate_length, fuzzy_mode);
}

/*	���û��ʿ����Ӵ�����
 *	������
 *		syllable		����
 *		ci				�ʺ���
 *		length			�ʳ���
 *	���أ�
 *		�ɹ���1
 *		ʧ�ܣ�0
 */
int AddCi(SYLLABLE *syllable, int syllable_length, HZ *hz, int hz_length)
{
	int user_wl_id = GetUserWordLibId();

	//UCS4�Ĵʲ������û��ʿ�
	if (syllable_length != hz_length)
		return 0;

	if (!AddCiToWordLibrary(user_wl_id, hz, hz_length, syllable, syllable_length, USER_BASE_FREQ))
		return 0;		//ʧ��

	return 1;
}

/**	�����´�
 *	���أ�
 *		�鵽������new_ci�����е��±�
 *		û�в鵽���򷵻�-1
 */
//int GetNewWordPosition(HZ *ci_str, int ci_length, SYLLABLE *syllable, int syllable_length)
//{
//	int index = 0;
//
//	while(index < share_segment->new_ci.length)
//	{
//		if (share_segment->new_ci.data[index] == (char)ci_length && share_segment->new_ci.data[index +1] == (char)syllable_length &&
//			!memcmp(&share_segment->new_ci.data[index + 2], ci_str, ci_length * sizeof(HZ)) &&
//			!memcmp(&share_segment->new_ci.data[index + 2 + ci_length * sizeof(HZ)], syllable, syllable_length * sizeof(SYLLABLE)))
//			return index;
//
//		index += 2 + share_segment->new_ci.data[index] * sizeof(HZ) + share_segment->new_ci.data[index + 1] * sizeof(SYLLABLE);
//	}
//
//	return -1;
//}

/**	����´�
 */
//int InsertNewWord(HZ *ci_str, int ci_length, SYLLABLE *syllable, int syllable_length)
//{
//	int size;
//	int i;
//
//	if (ci_length < 2 || ci_length > MAX_WORD_LENGTH ||
//		syllable_length < 2 || syllable_length > MAX_WORD_LENGTH)
//		return 0;
//
//	//����������
//	for (i = 0; i < syllable_length; i++)
//		syllable[i].tone = TONE_0;
//
//	//�Ƿ��Ѿ��ڱ���
//	if (GetNewWordPosition(ci_str, ci_length, syllable, syllable_length) >= 0)
//		return 1;			//�Ѿ�����
//
//	//��ǰ�����
//	size = ci_length * sizeof(HZ) + syllable_length * sizeof(SYLLABLE) + 2;
//	for (i = share_segment->new_ci.length - 1; i >= 0; i--)
//		share_segment->new_ci.data[i + size] = share_segment->new_ci.data[i];
//
//	share_segment->new_ci.data[0] = (char)ci_length;
//	share_segment->new_ci.data[1] = (char)syllable_length;
//
//	memcpy(&share_segment->new_ci.data[2], ci_str, ci_length * sizeof(HZ));
//	memcpy(&share_segment->new_ci.data[2 + ci_length * sizeof(HZ)], syllable, syllable_length * sizeof(SYLLABLE));
//
//	share_segment->new_ci.length += size;
//	if (share_segment->new_ci.length > sizeof(share_segment->new_ci.data))
//		share_segment->new_ci.length = sizeof(share_segment->new_ci.data);
//
//	share_segment->new_ci_modified = 1;
//	return 1;
//}

/**	ɾ���´�
 */
//void DeleteNewWord(HZ *ci_str, int ci_length, SYLLABLE *syllable, int syllable_length)
//{
//	int index, i, size;
//
//	if (ci_length < 0 || ci_length > MAX_WORD_LENGTH ||
//		syllable_length < 0 || syllable_length > MAX_WORD_LENGTH)
//		return;
//
//	index = GetNewWordPosition(ci_str, ci_length, syllable, syllable_length);
//	if (index < 0)
//		return;
//
//	size = 2 + ci_length * sizeof(HZ) + syllable_length * sizeof(SYLLABLE);
//
//	//������ǰ�ƶ�һ��������λ��
//	for (i = index; i < share_segment->new_ci.length - size; i++)
//		share_segment->new_ci.data[i] = share_segment->new_ci.data[i + size];
//
//	share_segment->new_ci.length -= size;
//	if (share_segment->new_ci.length < 0)
//		share_segment->new_ci.length = 0;
//
//	share_segment->new_ci_modified = 1;
//}

/**	���ɴ�����ƴ����
 *	���أ�ƴ��������
 *		���������㣬����-1
 */
//int __stdcall GenerateNewWordString(int index, char *buffer, int buffer_length)
//{
//	int size, length;
//	int ci_length, syllable_length;
//	char tmp[0x100];
//
//	ci_length		= share_segment->new_ci.data[index];
//	syllable_length = share_segment->new_ci.data[index + 1];
//	size			= ci_length * sizeof(HZ) + syllable_length * sizeof(SYLLABLE) + 2;
//
//	if (buffer_length - 1 < size || sizeof(tmp) - 1 < size)
//		return -1;
//
//	memcpy(tmp, &(share_segment->new_ci.data[index]), size);
//	tmp[size] = 0;
//
//	//�洢���������Լ����ڳ���
//	length = ArrayToHexString(tmp, size, buffer, buffer_length);
//	if (!length)
//		return -1;
//
//	return length;
//}

/**	����µĴ���
 *	���أ�
 *		�ַ����ĳ���
 */
//int __stdcall GetNewestUserWords(char *buffer, int buffer_length)
//{
//	int index, buffer_index;
//	short sign;				//�ַ���ǩ��
//
//	buffer_length -= sizeof(sign) + 1;		//Ԥ��ǩ������ֵ
//	index = buffer_index = 0;
//	while(index < share_segment->new_ci.length)
//	{
//		if (share_segment->new_ci.data[index + 2] != 0)		//���Ǳ�ɾ����
//		{
//			int length = GenerateNewWordString(index, buffer + buffer_index, buffer_length - buffer_index);
//
//			if (length < 0)		//��������
//				break;
//
//			buffer_index += length;
//		}
//
//		index += 2 + share_segment->new_ci.data[index] * sizeof(HZ) + share_segment->new_ci.data[index + 1] * sizeof(SYLLABLE);
//	}
//
//	if (buffer_index)
//	{
//		//ֻȡshort��ǩ����Ϣ
//		sign = (short)GetSign(buffer, buffer_index);
//		ArrayToHexString((const char*)&sign, sizeof(sign), buffer + buffer_index, sizeof(sign) * 2 + 1);
//		buffer_index += sizeof(sign) * 2;
//	}
//
//	return buffer_index;
//}

/**	ɾ��һ���´ʣ�UPLOAD��ϣ�
 */
//void __stdcall DelNewestUserWords(const char *buffer)
//{
//	int length;
//	int index;
//	int ci_length, syllable_length;
//	const char *p_ci, *p_syllable;
//	char tmp[0x1000];			//ת������ַ���
//	int  tmp_len;
//	short  sign;					//ǩ������
//
//	if (!(length = (int)strlen(buffer)))
//		return;
//
//	//����
//	if (!(tmp_len = HexStringToArray(buffer, tmp, sizeof(tmp))) || tmp_len < 4)
//		return;
//
//	//�ж�ǩ���Ƿ���ȷ
//	sign = *(short*)(tmp + tmp_len - sizeof(short));
//	if (sign != (short)(GetSign(buffer, length - sizeof(sign) * 2)))
//		return;		//ǩ������
//
//	//������������
//	//B0���ʳ��ȣ�B1�����ڳ���
//	index = 0;
//	while(index < tmp_len)
//	{
//		ci_length = tmp[index];
//		syllable_length = tmp[index + 1];
//
//		if (ci_length < 0 || syllable_length < 0 ||
//			ci_length > MAX_WORD_LENGTH || syllable_length > MAX_WORD_LENGTH)
//			return;
//
//		p_ci = &tmp[index + 2];
//		p_syllable = p_ci + ci_length * sizeof(HZ);
//		DeleteNewWord((HZ*)p_ci, ci_length, (SYLLABLE*)p_syllable, syllable_length);
//		index += 2 + ci_length * sizeof(HZ) + syllable_length * sizeof(SYLLABLE);
//	}
//}

/**	����Ƿ����û��ʿ��д�������ʣ���������ڣ�����뵽�´ʱ���
 */
//void CheckNewUserWord(SYLLABLE *syllables, int syllable_length, HZ *ci_string, int ci_length)
//{
//	WORDLIBITEM *item;
//	int user_wl_id = GetUserWordLibId();
//
//	if (user_wl_id < 0)
//		return;
//
//	//UCS4�Ĵʲ���������û��ʿ�
//	if (syllable_length != ci_length)
//		return;
//
//	if ((item = GetCiInWordLibrary(user_wl_id, ci_string, ci_length, syllables, syllable_length)) != 0)
//		return;		//���ڣ�����
//
//	InsertNewWord(ci_string, ci_length, syllables, syllable_length);
//}

/**	װ���´ʱ��ļ�
 */
//int LoadNewWordTable()
//{
//	int length;
//	TCHAR name[MAX_PATH];
//
//	if (share_segment->new_ci_loaded)
//		return 1;
//
//	GetFileFullName(TYPE_ALLAPP, NEWCI_FILE_NAME, name);
//
//	//��ʼ�����ݣ������ļ�װ��ʧ����ɳ������
//	share_segment->new_ci.length = 0;
//
//	length = LoadFromFile(name, (char*)&share_segment->new_ci, sizeof(share_segment->new_ci));
//	if (length < 0)
//	{
//		Log(LOG_ID, L"��ȡ���´ʱ��ļ�����file:%s, length=%d", name, length);
//		share_segment->new_ci.length = 0;
//		return 0;
//	}
//
//	share_segment->new_ci_loaded = 1;
//	return 1;
//}

/**	�����´ʱ��ļ�
 */
//int SaveNewWordTable()
//{
//	TCHAR name[MAX_PATH];
//
//	if (!share_segment->new_ci_modified)
//		return 1;
//
//	GetFileFullName(TYPE_ALLAPP, NEWCI_FILE_NAME, name);
//	if (!SaveToFile(name, (char*)&share_segment->new_ci, sizeof(share_segment->new_ci)))
//	{
//		Log(LOG_ID, L"д���´ʱ��ļ�ʧ��,file:%s", name);
//		return 0;
//	}
//
//	share_segment->new_ci_modified = 0;
//	return 1;
//}

/**	�ͷ��´ʱ�
 */
//void FreeNewWordTable()
//{
//	SaveNewWordTable();
//	share_segment->new_ci_loaded = 0;
//}