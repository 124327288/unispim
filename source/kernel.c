/*	���뷨���ĳ��򡣹��ܰ�����
 *	1. ���ֺ�ѡ�Ĵ���
 *	2. ������Դ��װ�������
 *	3. ��Ƶ����Ƶ�ĸ��¹���
 */
#include <assert.h>
#include <kernel.h>
#include <syllable.h>
#include <config.h>
#include <spw.h>
#include <ci.h>
#include <zi.h>
#include <icw.h>
#include <context.h>
#include <utility.h>
#include <english.h>
#include <time.h>
//#include <url.h>
#include <tchar.h>
#include <editor.h>

#define	MIN_LETTER_WORD_POS		5		//��С�ĵ���ĸ�ʵ�λ��

int		window_logon = 0;				//ϵͳ��¼״̬

/**	�Ժ�ѡ����������ҪĿ����Ҫ�õ����ڵĴ������ֺ���
 */
void SortCandidates(CANDIDATE *candidate_array, int count, int syllable_count)
{
	int i, j, k;
	int spw_count = 0;
	CANDIDATE cand_saves[MIN_LETTER_WORD_POS];

	//Q: ʲôʱ�����ڵĺ�ѡ���дʣ�
	//A����ֻ�뵽��������1)xian��lian�ȿ��Բ�ֵ����ڣ�"����""����"����Ϊ��ѡ�ʣ�
	//2)�Զ��������������fang=����3)������ƴ�������Σ��������ַ����������Σ�
	//a)��sa��"����""�永"��Ϊ��ѡ��(���賬����ƴ��������Ϊ2)��b)��tang��������
	//t'a'n'g�Լ���һ���ʣ���"�찢�ָ�"��������tang(���賬����ƴ��������Ϊ4)����
	//ע�⣬��1)������ĺ�ѡ����ΪCAND_TYPE_ZI�����Ҳ������ֵ�һ�֣���2)�����
	//�ĺ�ѡ����ΪCAND_TYPE_SPW����3)�����a)b)�ĺ�ѡ���;�ΪCAND_TYPE_CI��ֻ��
	//a)��������ΪCI_TYPE_LETTER����������������Ȼ�����1)����Ϊ1)���Կ���������
	//���֣�Ҳ�����2)����Ϊ�Զ������������ں�����SortSPWCandidates��ᵥ����
	//��ֻ���3)
	if (syllable_count != 1)
		return;

	//������һ�����ڣ���ˣ����еĴʶ����ǵ���ĸ����ģ�������ǰ���ɡ�
	for (i = 0; i < count; i++)
	{
		//��¼���Զ������ĸ������Զ�����ﲻ���뱾����������
		if (candidate_array[i].type == CAND_TYPE_SPW)
		{
			spw_count++;
		}

		//������ͷ���Զ�������Ϊ������SortSPWCandidates�ﴦ��
		//����������ƴ�ʣ���Ϊ������ƴ���ֻ���ŵ�MIN_LETTER_WORD_POS
		//�Ժ��λ��(׼ȷ��˵��spw_count+MIN_LETTER_WORD_POS��λ��!)
		if (!((candidate_array[i].type == CAND_TYPE_CI && candidate_array[i].word.type == CI_TYPE_LETTER) ||
			candidate_array[i].type == CAND_TYPE_SPW))
			break;
	}

	//��ͷ�ľ�����
	if (!i)		
		return;

	//���ֺ�ѡ��ǰ(�����2)�еĴ������ŵ�MIN_LETTER_WORD_POS֮��)
	for (j = 0; j < MIN_LETTER_WORD_POS && j + i < count; j++)
		cand_saves[j] = candidate_array[j + i];

	//�Ӻ���ǰ���ƺ�ѡ
	for (k = i + j; k > j + spw_count; k--)
		candidate_array[k - 1] = candidate_array[k - 1 - j];

	//���ǰ��ĺ�ѡ
	for (i = 0; i < j; i++)
		candidate_array[i + spw_count] = cand_saves[i];
}

/**	�Ժ�ѡ����������ҪĿ����Ҫ��Ӣ�ĵ������ڵ�ǰҳ���һ��
 */
void SortEnglishCandidates(PIMCONTEXT *context, CANDIDATE *candidate_array, int count)
{
	int i, j, index;
	CANDIDATE cand_temp[1];

	for (i = 0; i < count; i++)
	{
		if ((candidate_array[i].type == CAND_TYPE_SPW) && (candidate_array[i].spw.type == SPW_STIRNG_ENGLISH))
		{
			cand_temp[0] = candidate_array[i];
			break;
		}
	}

	//û�ҵ�
	if (i == count)
		return;

	index = pim_config->candidates_per_line;
	if (index > count)
		index = count;

	index--;

	//�����ں��ʵ�λ�ã������ƶ�
	if (i == index)
		return;

	//��ǰ���������
	for (j = i; j < index; j++)
		candidate_array[j] = candidate_array[j + 1];

	//�Ѻ������ǰ��
	for (j = i; j > index; j--)
		candidate_array[j] = candidate_array[j - 1];

	candidate_array[index] = cand_temp[0];

	return;
}

/**	�Ժ�ѡ����������ҪĿ����Ҫ�õ��������ѡ���ڵ�ǰҳ��3��
 */
void SortSPWCandidates(PIMCONTEXT *context, CANDIDATE *candidate_array, int count, int eng_count)
{
	int i, index, maxIndex;
	CANDIDATE cand_temp[1];
	cand_temp[0] = candidate_array[0];

	maxIndex = pim_config->candidates_per_line;
	if (maxIndex > count)
		maxIndex = count;

	maxIndex--;
	if(eng_count == 1)
		maxIndex--;
	index = maxIndex;
	//Ϊ�������������ã�spw_position����ֵ��11~19��ʹ��ʱֱ�Ӽ�10
	if(maxIndex > pim_config->spw_position - 10 - 1)
		index = pim_config->spw_position - 10 - 1;
	if(candidate_array->spw.hint 
		&& _tcslen((TCHAR *)candidate_array->spw.hint)==3 
		&& ((TCHAR *)candidate_array->spw.hint)[1]>TEXT('0') 
		&& ((TCHAR *)candidate_array->spw.hint)[1]<=TEXT('9')){
		index = ((TCHAR *)candidate_array->spw.hint)[1] - TEXT('0') - 1;
		if(index > maxIndex)
			index = maxIndex;
	}
	//�����ں��ʵ�λ�ã������ƶ�
	if (0 == index)
		return;
	//�Ѻ������ǰ��
	for (i = 0; i < index; i++)
		candidate_array[i] = candidate_array[i + 1];
	candidate_array[index] = cand_temp[0];
	return;
}

/**	�Ժ�ѡ����������ҪĿ����Ҫ������ʴ������ʵ���λ��
 */
void SortWildcardCandidates(PIMCONTEXT *context, CANDIDATE *candidate_array, int count, int wildcard_count)
{
	int i, j, k, index, last = 0;
	CANDIDATE cand_temp[1];

	index = pim_config->suggest_word_location - 1;

	//������Զ������������Ĵ�λ���ƺ�
	for ( i = index; i < count; i++ )
	{
		if ( candidate_array[i].type == CAND_TYPE_SPW )
			index++;
		else
			break;
	}

	for (i = 0; i < wildcard_count; i++)
	{
		for (j = last; j < count; j++)
		{
			if (candidate_array[j].word.type == CI_TYPE_WILDCARD)
			{
				cand_temp[0] = candidate_array[j];
//				cand_temp[0].word.type = CI_TYPE_NORMAL;
				break;
			}
		}

		//û�ҵ�
		if (j == count)
			return;

		last = j;

		//�����ں��ʵ�λ�ã������ƶ�
		if (j == index)
		{
			index++;
			continue;
		}

		//��ǰ���������
		for (k = j; k < index; k++)
			candidate_array[k] = candidate_array[k + 1];

		//�Ѻ������ǰ��
		for (k = j; k > index; k--)
			candidate_array[k] = candidate_array[k - 1];

		candidate_array[index] = cand_temp[0];

		index++;
	}

	return;
}

int UnifyCandidates(PIMCONTEXT *context, CANDIDATE *candidate_array, int count, int spw_count)
{
	int i, j, k, ret;

	for (i = 0; i < spw_count; i++)
	{
		if (candidate_array[i].type != CAND_TYPE_SPW && candidate_array[i].spw.type != SPW_STRING_NORMAL)
			continue;

		for (j = spw_count; j < count; j++)
		{
			ret = 0;

			switch (candidate_array[j].type)
			{
			case CAND_TYPE_ZI:
				if (candidate_array[j].hz.is_word)
				{
					if (candidate_array[j].hz.word_item->ci_length == candidate_array[i].spw.length)
						ret = !_tcsncmp(candidate_array[i].spw.string,
										GetItemHZPtr(candidate_array[j].hz.word_item),
										candidate_array[i].spw.length);
				}
				else if (1 == candidate_array[i].spw.length)
					ret = !_tcsncmp(candidate_array[i].spw.string,
									&(TCHAR)candidate_array[j].hz.item->hz,
									1);

				break;

			case CAND_TYPE_CI:
				if (candidate_array[j].word.item->ci_length == candidate_array[i].spw.length)
					ret = !_tcsncmp(candidate_array[i].spw.string,
									candidate_array[j].word.hz,
									candidate_array[i].spw.length);

				break;
			}

			if (ret)
			{
				//�Ӻ���ǰŲ
				for (k = j; k < count - 1; k++)
					candidate_array[k] = candidate_array[k + 1];

				count--;
				j--;
			}
		}
	}

	return count;
}

//�ж϶����ǲ����ں�ѡ��λ
int IsFirstPosSPW(CANDIDATE *candidate_array)
{
	int index;
	if(!candidate_array)
		return 0;
	if(candidate_array[0].type != CAND_TYPE_SPW)
		return 0;
	if(candidate_array[0].spw.hint 
		&& _tcslen((TCHAR *)candidate_array[0].spw.hint)==3 
		&& ((TCHAR *)candidate_array[0].spw.hint)[1]>TEXT('0') 
		&& ((TCHAR *)candidate_array[0].spw.hint)[1]<=TEXT('9'))
	{
		index = ((TCHAR *)candidate_array[0].spw.hint)[1] - TEXT('0');
		if(index == 1)
			return 1;
	}
	else if(pim_config->spw_position == 11){
		return 1;
	}
	return 0;
}

/**	�����û���ǰ�����룬��ȡ��ѡ��
 *	������
 *		context				����������
 *		input_string		�û���������
 *		syllables			��������
 *		syllable_count		�������鳤��
 *		candidate_array		��ѡָ��
 *		array_length		��ѡ����ĳ���
 *		need_spw			�Ƿ���Ҫ�����ѡ
 *	���أ�
 *		��ѡ��Ŀ��
 *	ע�⣺
 *		��ȡICW��ʱ�������������Ѿ��ڴʿ��У���ICW����Ϊ0��
 *		��ȡ�ʵ�ʱ���ȴ����ƴ����ʼ�����εݼ�ֱ���ʵĳ��ȵ���2
 *	��ѡ���ȼ���
 *		1. SPW
 *      2. url
 *		3. FULL_CI������ƴ���Ĵʣ�
 *		4. ICW����ǰ���������ڵ�����£�
 *		5. PART_CI���������ڳ��ȵĴʣ�
 *		6. ZI
 */
int GetCandidates(PIMCONTEXT *context,
				  const TCHAR *input_string,
				  SYLLABLE *syllables,
				  int syllable_count,
				  CANDIDATE *candidate_array,
				  int array_length,
				  int need_spw)
{
	CANDIDATE *candidate_array_save = candidate_array;
	int count, icw_count, spw_count, /*url_count,*/ ci_count, zi_count, eng_count, wildcard_count;
	int has_star;				//�Ƿ����ͨ���
	int i, j, syllable_index;
	TCHAR new_input_string[MAX_INPUT_LENGTH + 0x10] = {0};
	SYLLABLE new_syllables[MAX_SYLLABLE_PER_INPUT + 0x10] = {0};
	int new_syllable_count = 0;
	int legal_length;

	//�����������ƴ������FULL_CI��ICW��Ҫ�����������
	SYLLABLE other_syllables[MAX_SYLLABLE_PER_INPUT] = {0};
	int other_count = 0;

	//���ڽ����ֵ����ڲ��(С���ڲ��)�������´ʣ���xian'shi->xi'an'shi(������)������
	//small_syllables_arrays������Ų�ֽ�������ƴ��������k���ֿ��Բ�֣���������Ӧ
	//��2^k�ֲ�ֽ����small_arrays_lengths��small_arrays_count�и���ƴ��������������
	//small_arrays_count��ʵ�ʲ�ֽ������Ŀ(����ƴ�����ĳ����յ����ƣ�ĳЩ��ַ�����
	//��ƴ�������ȹ���Ӧ�ų�������ʵ�ʲ�ֽ����һ����2^k��)������other�ı���������
	//�ƣ�ֻ����Ե�����������Ľ��(��Ϊ�������ҲҪʹ��С���ڲ�ֵĽ�������������
	//����Ҫʹ�ã������FULL_CI����Ϊ������С���ڲ���䲻ͬ���ڴ棬�������໥����)
	SYLLABLE *small_syllables_arrays = (SYLLABLE*)malloc((2 << MAX_SMALL_SYLLABLES) * MAX_SYLLABLE_PER_INPUT * sizeof(SYLLABLE));
	SYLLABLE *small_other_syllables_arrays = (SYLLABLE*)malloc((2 << MAX_SMALL_SYLLABLES) * MAX_SYLLABLE_PER_INPUT * sizeof(SYLLABLE));
	int *small_arrays_lengths = (int*)malloc((2 << MAX_SMALL_SYLLABLES) * sizeof(int));
	int *small_other_arrays_lengths = (int*)malloc((2 << MAX_SMALL_SYLLABLES) * sizeof(int));
	int small_arrays_count = 0;
	int small_other_arrays_count = 0;

	//����ñ�����Ϊ�˽���������⣺
	//������liang'ang����һ����"����"��������liangangren����ʱ���뷨�Զ�����ֽ�Ϊ
	//lian'gang'ren��������left�������λ��gangǰ������ʱ�ĺ�ѡ��ȴ��"����"��ԭ����
	//GetOtherSyllableParse�ĵ�2������ԭ��Ϊsyllables - context->syllables�����ڷ�
	//���ܱ༭״̬�����ֵ����context->syllable_pos������δת��Ϊ���ֵĵ�һ������λ
	//�ã�����������left�����������û���κ�����ת��Ϊ���֣����context->syllable_pos
	//Ϊ0������GetOtherSyllableParse��lian��ʼ�������������������liang'ang�������
	//�ʡ�����ʹ����start_pos�Ժ����������start_posΪ1����gang��ʼ�������ȽϺ���
	int start_pos = 0;

	assert(candidate_array && array_length);
	count = icw_count = spw_count = ci_count = zi_count = eng_count = wildcard_count = 0;
	legal_length = GetLegalPinYinLength(context->input_string, context->state, context->english_state);
	if (context->input_pos >= legal_length)
		return 0;
	has_star = 0;
	for (i = 0; i < syllable_count; i++)
		if (syllables[i].con == CON_ANY && syllables[i].vow == VOW_ANY)
		{
			has_star = 1;
			break;
		}

	//0. Ӣ������/��ѡģʽ
	//Ӣ������ģʽ�ͺ�ѡģʽ������Ӣ������ģʽ��һֱʹ��Ӣ�����룬
	//Ӣ�ĺ�ѡģʽ����������ʱ��ʱ�л���ȥ�ģ�Ŀ������ʱ����һ��Ӣ��
	//���ʣ�ѡ�񵥴��Ժ��´������뻹����ʾ���ĺ�ѡ
	context->has_english_candidate = 0;
	if (input_string && context->english_state != ENGLISH_STATE_NONE)
	{
		eng_count = GetEnglishCandidates(input_string, candidate_array, array_length);
		count += eng_count;

		context->has_english_candidate = 1;

		//Ӣ�����뷨�ʹӲ���1��ʼ���������뷨�Ƕ����ģ�
		//�������Ӣ�����뷨������Է�����
		if (count >= array_length)
			return array_length;
		else
			return count;
	}

	//1. SPW
	if (need_spw && input_string)
	{
		spw_count = GetSpwCandidates(context, input_string, candidate_array, array_length);
		count += spw_count;
		if (count >= array_length)
			return array_length;
	}

	//��ȡ���֮ǰ����δת��Ϊ���ֲ��ֵ�������
	if (context->state == STATE_IEDIT)
		syllable_index = 0;
	else
		syllable_index = (context->compose_cursor_index) ? GetSyllableIndexByComposeCursor(context, context->compose_cursor_index) : 0;
	for (i = 0; i < context->selected_item_count; i++)
		syllable_index -= context->selected_items[i].syllable_length;

	//������ȷ��ֻ֪��syllable_count��ʾ������δת��Ϊ���ֵ�����������syllables������Щ���ڡ�
	//����������/��ǰ����new_syllables����syllables������겻�����/��ǰ����new_syllables��
	//���֮�������(��ʵ���Կ�������ֵ�������compose_stringΪ"����xiawuhui|jiama"��|��ʾ
	//��꣬���ǿ��Կ�����ѡ����ʾ����"����"��)
	i = (!context->compose_cursor_index || context->compose_cursor_index == context->compose_length) ? 0 : syllable_index;
    start_pos = i; //new_syllables������ȷ�ĺ�ѡ���ڣ���¼����ƫ��ֵ(�������GetOtherSyllableParseʱ��ʹ��)
	for (j = 0; i < syllable_count; i++, j++, new_syllable_count++)
		new_syllables[j] = syllables[i];

	//ͬ�ϣ�����������/��ǰ��new_input_string����input_string������new_input_string��input_string
	//���֮��Ĳ���
	if (input_string)
	{
		i = (!context->cursor_pos || context->cursor_pos == context->input_length) ? 0 : context->cursor_pos;
		for (j = 0; i < context->input_length; i++, j++)
			new_input_string[j] = input_string[i];
	}

	if (!context->selected_item_count && !has_star)
		context->syllable_mode = 1;
	if (spw_count)
		context->syllable_mode = 0;

	//2. FULL_CI�������Ĵ�(����zhongguogongchandang����ѡ��ʾ"�й�������"��ͨ����ѡ����ʾ˳����
	//�����ʡ����ִʡ�����)
	if (new_syllable_count /*syllable_count*/ > 0)
	{
		int last_count = count;
		int ci_count_sav;

		//����������
		ci_count = ProcessCiCandidate(
			new_syllables, //syllables,
			new_syllable_count, //syllable_count,
			context->selected_item_count ? 0 : new_input_string, //input_string,
			candidate_array + count,
			array_length - count,
			context->selected_item_count ? 1 : 0); //���һ���������岻��
		count += ci_count;

		//��4�ֽ�����ʽ������Ҫ��¼ԭʼ����
		ci_count_sav = count;

		//С���ڲ��
		small_arrays_count = GetSmallSyllablesParse(new_syllables, new_syllable_count, small_syllables_arrays, small_arrays_lengths);
		for (i = 0; i < small_arrays_count; i++)
		{
			ci_count = ProcessCiCandidate(
				small_syllables_arrays + i * MAX_SYLLABLE_PER_INPUT,
				small_arrays_lengths[i], 
				0,                       //�����ǳ�����ƴ������������ڸ���
				candidate_array + count,
				array_length - count,
				context->selected_item_count || context->state == STATE_IEDIT ? 1 : 0); //���һ���������岻��
			count += ci_count;
		}

		//����������
		//Q��Ϊ��ji'er'c(�����)�޷������������0��
		//A����ƴ���������������ڣ���GetOtherSyllableParse::IsFullSyllable
		other_count = GetOtherSyllableParse(
			context,
			//syllables - context->syllables�����滻Ϊcontext->syllable_pos�����ܱ༭
			//״̬ʱ����ȡֵ��ͬ(��MakeCandidate��context->state == STATE_IEDIT�����)
			start_pos + syllables - context->syllables, 
			new_syllable_count, 
			other_syllables,
			MAX_SYLLABLE_PER_INPUT);
		if (other_count)
		{
			ci_count = ProcessCiCandidate(
				other_syllables,
				other_count,        //����ԭ������new_syllable_count��������������Ϊ��other_count�ź���
				0,                  //�����ǳ�����ƴ��֮ǰ�������ʱӦ���Ѿ��������
				candidate_array + count,
				array_length - count,
				context->selected_item_count || context->state == STATE_IEDIT ? 1 : 0); //���һ���������岻��
			count += ci_count;

			//С���ڲ��
			small_other_arrays_count = GetSmallSyllablesParse(other_syllables, other_count, small_other_syllables_arrays, small_other_arrays_lengths);

			for (i = 0; i < small_other_arrays_count; i++)
			{
				ci_count = ProcessCiCandidate(
					small_other_syllables_arrays + i * MAX_SYLLABLE_PER_INPUT,
					small_other_arrays_lengths[i], 
					0,                       //�����ǳ�����ƴ������������ڸ���
					candidate_array + count,
					array_length - count,
					context->selected_item_count || context->state == STATE_IEDIT ? 1 : 0); //���һ���������岻��
				count += ci_count;
			}

			//��Ͻ�������(ֻ���������������ͬʱ�ſ��ܴ��ڻ�Ͻ���)
			//�е����ڲ�������������ģ���������������ģ�����"�����������������"������fanganzhegefangan��
			//��ѡ��"����"����ѡ��"�������"���ôʱ����뵽�û��ʿ⣬���´�������fanganzhegefangan����ѡ��ȴ
			//û��"�����������"����ɸ������ԭ���ǣ�fanganzhegefangan����������Ϊfang'an'zhe'ge'fang'an
			//������������Ϊfan'gan'zhe'ge'fan'gan�����Ƕ���fan'gan'zhe'ge'fang'an(��ȷ��ƴ����Ҳ��context
			//->result_syllables��ֵ(��PostResult))��ͬ�������Ȼ�ô���ͬ��ȷ�����ڱ��������û��ʿ⣬���´�
			//������fanganzhegefangan������������Ľ�����޷�������fan'gan'zhe'ge'fang'an����ȻҲ���޷��Ӵ�
			//���в鵽�ô��ˡ�������������ֱ�ӵķ������Ǳ������н������ڷ�ʽ����ϣ�������ѡ�ʣ���������Ч
			//��̫�͡����ǲ�ȡ�ķ�������new_syllables��small_syllables_arrays��other_syllables��small_other_syllables_arrays
			//��ͷ��������Ϊ�������д�������(ȡͷ����������Ϊ�˱�֤û�л�Ͻ��������������fanganfangan����
			//ȡ3�����ڣ����Ѿ��漰����Ͻ���(fan'gan'fang��fan'gan'fan��fang'an'fang��fang'an'fan)����Ͻ�
			//���������������ֽ������������һ�֣����ܽ�����⣻��ȡ������(fan'gan��fang'an)���ֱ�����new_syllables
			//��other_syllables)��ͬʱ��������ķ�Χ�����û��ʿ���4���������ϵĴ�(����Ҫfanganfangan����4��
			//���ڵ�ƴ�����ſ��ܲ�����Ͻ���)�����������Խ�С�Ĵ����ҳ���Ͻ����ĺ�ѡ��
			if (!has_star && !HasSyllableSeparator(context->input_string + context->input_pos))
			{
				int mixed_save_count = count;
				int per_syllable_count;
				TCHAR syllable_string[MAX_SYLLABLE_PER_INPUT * MAX_PINYIN_LENGTH + 1] = {0};

				//��ȡ��ѡ��
				count += GetMixedParseCiCandidate(
					new_syllables, 
					new_syllable_count, 
					candidate_array + count, 
					array_length - count);

				for (i = 0; i < small_arrays_count; i++)
				{
					count += GetMixedParseCiCandidate(
						small_syllables_arrays + i * MAX_SYLLABLE_PER_INPUT,
						small_arrays_lengths[i],
						candidate_array + count,
						array_length - count);
				}

				count += GetMixedParseCiCandidate(
					other_syllables, 
					other_count, 
					candidate_array + count, 
					array_length - count);

				for (i = 0; i < small_other_arrays_count; i++)
				{
					count += GetMixedParseCiCandidate(
						small_other_syllables_arrays + i * MAX_SYLLABLE_PER_INPUT,
						small_other_arrays_lengths[i],
						candidate_array + count,
						array_length - count);
				}

				//��֤ƴ���Ƿ�һ��
				for (i = mixed_save_count; i < count; i++)
				{	
					per_syllable_count = 0;
					for (j = 0; j < (int)candidate_array[i].word.item->syllable_length; j++)
					{
						//�ʿ��������Ϊ����ֵ��Ӧ���Ե�������GetSyllableString���ܻ����
						//Q��ΪʲôҪʹ�ñ���syllable��
						//A����candidate_array[i].word.item->syllable[j].tone��ֵ��Σ�յ�
						//�ٶ�����Ϊitemָ����Ǵʿ��е����ݣ����ʿ����ڹ����ڴ��һ���֣�
						//����ж��������ʹ�����뷨���ܿ������ڹ����ڴ���ʳ�ͻ��������
						SYLLABLE syllable = candidate_array[i].word.item->syllable[j];
						syllable.tone = TONE_0;
						per_syllable_count += GetSyllableString(
							syllable, 
							syllable_string + per_syllable_count, 
							MAX_SYLLABLE_PER_INPUT * MAX_PINYIN_LENGTH + 1 - per_syllable_count, 
							0);
					}

					//����Ҫ����ȫ��ȶ��������κ�ģ����������������fanganzhegefangan��Ȼ������
					//h->fģ����������hanganzhegefangan����ѡ�ʲ������"�����������"����Ϊ����
					//��û�л������ڵ�ƴ������Ҫ����ģ�����ж�ʮ�����ѣ���ʱû�뵽�õĽ��������
					//ͬ��Ҳ�������κμ�ƴ������fangganzgfangan����ѡ��Ҳ�������"�����������"
					if (_tcscmp(syllable_string, context->input_string + context->input_pos))
					{
						candidate_array[i] = candidate_array[count - 1];
						count--;
						i--;
					}
				}

				//ȥ�أ����ǵ��㷨����ƴ������ͷ�������ڽ������룬�ܿ��ܣ�����new_syllables��
				//other_syllables��ͷ������������ͬ�ģ���˵õ��������Ҳ��ͬ����Ҫ����ȥ��
				count = last_count + UnifyCiCandidates(candidate_array + last_count, count - last_count);
			}
		}

		//��¼ԭʼ��������(���������������)��ò����Ҫ���ڵ������λ�ã���Ϊ�û��ڱ��봰
		//���п�������������������
		for (i = ci_count_sav; i < count; i++)
		{
			candidate_array[i].word.type = CI_TYPE_OTHER;
			candidate_array[i].word.origin_syllable_length = syllable_count;
		}

		//ProcessCiCandidate�ڲ������򣬵���Ҫ��������һ�Σ�����"xianshi(������)"�������
		//����С���ڲ�ֽ������ģ���������һ��"������"���´�������"������"����CiCacheԭ
		//��������Ӧ����������ƴ��Ϊxianshi�Ĵʵ���ǰ�棬�����ֻ��ProcessCiCandidate��
		//������Ļ�����ֻ������ƴ��Ϊxi'an'shi�Ĵʵ���ǰ�棬�������к�ѡ�д��ڽϺ����λ
		//�ã����Եò�������
		SortCiCandidates(candidate_array + last_count, count - last_count);

		if (count >= array_length)
		{
			//����ǰ���ͷ�
			free(small_syllables_arrays);
			free(small_other_syllables_arrays);
			free(small_arrays_lengths);
			free(small_other_arrays_lengths);

			return array_length;
		}
	}

	if (!has_star)
	{
		//3. ICW��ǰ�᣺full ciû�к�ѡ��spw����к�ѡ������ֻ��һ����
		//�������ҲҪ�����������ڲ�ֵĴ���������ʽ���ϲ���磺
		//wo'bu'tai'xi'huan'jia'pin'gao'de'shu->wo'bu'tai'xi'huan'jia'ping'ao'de'shu �Ҳ�̫ϲ����ƽ������
		//yi'jia'nan'zhuang->yi'jian'an'zhuang һ����װ
		//xian'zai'mei'you'min'ge'le->xian'zai'mei'you'ming'e'le ����û��������
		//tan'lia'nai'de'ren->tan'lian'ai'de'ren ̸��������
		if((count == 0 || (count == 1 && spw_count == 1)) && (pim_config->use_icw && new_syllable_count >= 2))
		{
			//ֻ��һ����ѡ�������ں�ѡ��λ����ʹ���������
			if(!(count == 1 && IsFirstPosSPW(candidate_array)))
			{ 
				extern int NewGetIcwCandidates(SYLLABLE *, int, CANDIDATE *, double *);

				int has_icw_candidate = 0;
				double current_value, max_value;

				//��ͨ����������
				icw_count = NewGetIcwCandidates(new_syllables, new_syllable_count, candidate_array + count, &current_value); //icw_countֻ��ȡ0��1
				has_icw_candidate = icw_count; //����һ�����ڽ������������������ʺ�ѡ����ֵ��Ϊ1
				max_value = current_value;
				count += icw_count;

				//����С���ڲ��
				for (i = 0; i < small_arrays_count; i++)
				{
					icw_count = NewGetIcwCandidates(small_syllables_arrays + i * MAX_SYLLABLE_PER_INPUT,
						small_arrays_lengths[i], candidate_array + count, &current_value);

					//֮ǰ�Ƿ��к�ѡ
					if (!has_icw_candidate)
					{
						if (icw_count)
						{
							has_icw_candidate = icw_count;
							count += icw_count;
						}
					}
					else
					{
						//֮ǰ����ǰ��ѡ�����ڵ����
						if (icw_count)
						{
							//ȡ���ʽϴ���
							if (current_value > max_value)
							{
								max_value = current_value;
								candidate_array[count - 1] = candidate_array[count];
							}

							//����������յ�candidate_array��ֻ����һ��������ʽ����
							//���ڼ���֮ǰ��ѡʱcount�Ѿ�����1�����ﲻ��count--
						}
					}
				}

				//��ͨ����������
				if (other_count)
				{
					icw_count = NewGetIcwCandidates(other_syllables, other_count, candidate_array + count, &current_value);

					//֮ǰ�Ƿ��к�ѡ
					if (!has_icw_candidate)
					{
						if (icw_count)
						{
							has_icw_candidate = icw_count;
							count += icw_count;
						}
					}
					else
					{
						//֮ǰ����ǰ��ѡ�����ڵ����
						if (icw_count)
						{
							//ȡ���ʽϴ���
							if (current_value > max_value)
							{
								max_value = current_value;
								candidate_array[count - 1] = candidate_array[count];
							}

							//����������յ�candidate_array��ֻ����һ��������ʽ����
							//���ڼ���֮ǰ��ѡʱcount�Ѿ�����1�����ﲻ��count--
						}
					}

					//����С���ڲ��
					for (i = 0; i < small_other_arrays_count; i++)
					{
						icw_count = NewGetIcwCandidates(small_other_syllables_arrays + i * MAX_SYLLABLE_PER_INPUT,
							small_other_arrays_lengths[i], candidate_array + count, &current_value);

						//֮ǰ�Ƿ��к�ѡ
						if (!has_icw_candidate)
						{
							if (icw_count)
							{
								has_icw_candidate = icw_count;
								count += icw_count;
							}
						}
						else
						{
							//֮ǰ����ǰ��ѡ�����ڵ����
							if (icw_count)
							{
								//ȡ���ʽϴ���
								if (current_value > max_value)
								{
									max_value = current_value;
									candidate_array[count - 1] = candidate_array[count];
								}

								//����������յ�candidate_array��ֻ����һ��������ʽ����
								//���ڼ���֮ǰ��ѡʱcount�Ѿ�����1�����ﲻ��count--
							}
						}
					}
				}

				if (count >= array_length)
				{
					//����ǰ���ͷ�
					free(small_syllables_arrays);
					free(small_other_syllables_arrays);
					free(small_arrays_lengths);
					free(small_other_arrays_lengths);

					return array_length; 
				}
			}
		}

		//�����Ƿ�ʹ���˽���������ʣ�small_syllables_arrays��small_arrays_lengths
		//���涼���������ˣ�Ӧ�����ͷ�(ע�ⲻҪ������ͷţ���Ϊ��;�кܶ�return���)
		free(small_syllables_arrays);
		free(small_other_syllables_arrays);
		free(small_arrays_lengths);
		free(small_other_arrays_lengths);

		//4. PART_CI
		for (i = new_syllable_count/*syllable_count*/ - 1; i >= 2; i--)
		{
			int other_count;
			int last_count = count;
			SYLLABLE other_syllables[MAX_SYLLABLE_PER_INPUT];

            //����������ִ�
			ci_count = ProcessCiCandidate(
				new_syllables, //syllables,
				i,
				0,
				candidate_array + count,
				array_length - count,
				context->selected_item_count || context->state == STATE_IEDIT ? 1 : 0);

			count += ci_count;

			//����������ִ�
			//Q��Ϊʲô��ֱ�����������other_syllables��
			//A������������˵ֱ����other_syllablesû��ʲô���⣬�����߼��ϲ����ã�����
			//��2��������������ڣ�������������Ϊ3������(ʵ���п��ܲ����������������
			//����Ժ�Ҫ����С���ڣ������������Ǻ����)����ô���򲿷ִ�ӦΪ2���֣�����
			//�򲿷ִ�ӦΪ3����(������2���֣�Ϊ�˱���ĳ�������ϵĶ�Ӧ)
			other_count = GetOtherSyllableParse(
				context, 
				//syllables - context->syllables�����滻Ϊcontext->syllable_pos�����ܱ༭
				//״̬ʱ����ȡֵ��ͬ(��MakeCandidate��context->state == STATE_IEDIT�����)
				start_pos + syllables - context->syllables, 
				i, 
				other_syllables,
				MAX_SYLLABLE_PER_INPUT);

			if (other_count)
			{
				int ci_count_sav = count;

				ci_count = ProcessCiCandidate(
					other_syllables,
					other_count,
					0,
					candidate_array + count,
					array_length - count,
					context->selected_item_count || context->state == STATE_IEDIT ? 1 : 0);

				count += ci_count;

				for (j = ci_count_sav; j < count; j++)
				{
					candidate_array[j].word.type = CI_TYPE_OTHER;
					candidate_array[j].word.origin_syllable_length = i;
				}

				if (ci_count)
					SortCiCandidates(candidate_array + last_count, count - last_count);
			}

			if (count >= array_length)
				return array_length;
		}

		//5. ZI
		if (new_syllable_count/*syllable_count*/)
		{
			int zi_count_save = count;
			int small_ci_count = 0;
			int small_ci_count_save = 0;
			int normal_zi_count = 0;   //��ͨ�֣���qiangang�е�qian��qiang
			int small_zi_count = 0;    //С���ڲ���֣���qiangang�е�qi
			CANDIDATE candidate_temp;

			//�������(qiangang����ѡ����Ϊqian)
			zi_count = ProcessZiCandidates(
							new_syllables[0], //syllables[0],
							candidate_array + count,
							array_length - count,
							context->zi_set_level);
			count += zi_count;
			
			//�������(qiangang����ѡ����Ϊqiang)
			if (other_count && !SameSyllable(new_syllables[0], other_syllables[0]))
			{
				zi_count = ProcessZiCandidates(
					other_syllables[0], //syllables[0],
					candidate_array + count,
					array_length - count,
					context->zi_set_level);
				count += zi_count;

				//���ֺ�С���ڴ�ȥ�أ����������qian��qiang����ֺ���qi��
				//��˵������ظ���qi'an��qi'ang��������²�������ظ���С��
				//�ڴʣ�����������an->ang����ĸģ�����Ի�����ظ���ѡ�����
				//Ҳ����ȥ�ء����潫С���ڴʷ���ǰ�棬���ַ��ں��棬Ȼ���
				//��������ֱ�ȥ��
				for (i = zi_count_save; i < count; i++)
				{
					if (candidate_array[i].hz.is_word)
					{
						small_ci_count_save++;
					}
				}

				i = zi_count_save;
				j = count - 1;

				while (1)
				{
					while (i < j && candidate_array[i].hz.is_word)
						i++;
					while(i < j && !candidate_array[j].hz.is_word)
						j--;
					if (i < j)
					{
						candidate_temp = candidate_array[i];
						candidate_array[i] = candidate_array[j];
						candidate_array[j] = candidate_temp;
					}
					else
					{
						break;
					}
				}
				
				small_ci_count = UnifySmallCiCandidates(
					candidate_array + zi_count_save,
					small_ci_count_save);

				//��ǰ�ƶ���ѡ
				memmove(candidate_array + zi_count_save + small_ci_count, 
					candidate_array + zi_count_save + small_ci_count_save, 
					(count - small_ci_count_save) * sizeof(CANDIDATE));
				count -= small_ci_count_save - small_ci_count;

				zi_count = UnifyZiCandidates(
					candidate_array + zi_count_save + small_ci_count, 
					count - zi_count_save - small_ci_count);

				count = zi_count_save + small_ci_count + zi_count;
			}
			
			//����ͨ��(����С���ڴʷ���ǰ��)��С�����ַ��ں��棬����
			//�����������ڲ��ٽ�������
			for (i = zi_count_save; i < count; i++)
			{
				if (!candidate_array[i].hz.is_word && candidate_array[i].hz.hz_type == ZI_TYPE_OTHER)
				{
					small_zi_count++;
				}
			}

			i = zi_count_save;
			j = count - 1;
			
			while (1)
			{
				while (i < j && (candidate_array[i].hz.is_word || (candidate_array[i].hz.hz_type == ZI_TYPE_NORMAL)))
					i++;
				while(i < j && (!candidate_array[j].hz.is_word && (candidate_array[j].hz.hz_type == ZI_TYPE_OTHER)))
					j--;
				if (i < j)
				{
					candidate_temp = candidate_array[i];
					candidate_array[i] = candidate_array[j];
					candidate_array[j] = candidate_temp;
				}
				else
				{
					break;
				}
			}

			normal_zi_count = count - zi_count_save - small_zi_count; //����С���ڴ�

			SortZiCandidates(candidate_array + zi_count_save, normal_zi_count);
			SortZiCandidates(candidate_array + zi_count_save + normal_zi_count, small_zi_count);

			if (count >= array_length)
				return array_length;
		}
	}
	else
	{
		//������ܽ���������ʣ���ôsmall_syllables_arrays��small_arrays_lengths
		//�����ͷ���(ע�ⲻҪ������ͷţ���Ϊ��;�кܶ�return���)
		free(small_syllables_arrays);
		free(small_other_syllables_arrays);
		free(small_arrays_lengths);
		free(small_other_arrays_lengths);
	}


	//6. Ӣ�ĵ���
	//Q�������Ӣ�ĵ��ʺ͵�0���е��к�����
	//A�����ﲻ�Ǵ���Ӣ�����뷨�ģ����������������еĸ߼�����/Ӣ������/���������ѡ�ʰ���Ӣ�ĵ��ʣ�
	//������Ҫע����Ǵ���ģʽ���������һ��Ӣ�ĵ���(��Ϊ�����GetEnglishCandidates�����һ������Ϊ1)��
    //��������he'l'l��hell����Ϊ��ѡ�ʣ���hello����
	if (pim_config->use_english_input && input_string &&  
		(!context->compose_cursor_index || context->compose_cursor_index == context->compose_length))
	{
		eng_count = GetEnglishCandidates(input_string, candidate_array + count, 1);

		//��Ҫ��һ��ɸѡ��һЩ̫�̵Ĵʿ��ܱ��ų�����������we����ѡ�п��ܲ������we
		if (context->input_length > 2 && (STATE_EDIT == context->state || STATE_ABC_SELECT == context->state) && eng_count > 0)
			context->has_english_candidate = 1;
		if (0 == count && eng_count > 0 && context->input_length > 1)
			context->has_english_candidate = 1;

		//������ж�û��ͨ���������eng��ѡû���ϣ���eng_count����
		if(!context->has_english_candidate)
			eng_count = 0;
		else{
			if(pim_config->candidate_include_english)
				count += eng_count;
			else
				eng_count = 0;
		}
		if (count >= array_length)
			return array_length;
	}

	if (!has_star)
	{
		//7. ��������
		if (pim_config->use_word_suggestion && pim_config->suggest_word_count > 0 &&
			syllable_count == context->syllable_count && syllable_count >= pim_config->suggest_syllable_location &&
			syllables[syllable_count - 1].con != CON_ANY && syllables[syllable_count - 1].vow != VOW_ANY)
		{
			int i;
			SYLLABLE  new_syllables[MAX_SYLLABLE_PER_INPUT + 1] = {0};
			CANDIDATE new_candidates[MAX_ICW_CANDIDATES] = {0};

			for (i = 0; i < syllable_count; i++)
				new_syllables[i] = syllables[i];

			//��ĩβ����ͨ���
			new_syllables[syllable_count].con  = CON_ANY;
			new_syllables[syllable_count].vow  = VOW_ANY;
			new_syllables[syllable_count].tone = TONE_0;
			wildcard_count = ProcessCiCandidate(new_syllables, syllable_count + 1, 0, new_candidates, MAX_ICW_CANDIDATES, 0);
			for (i = 0; i < pim_config->suggest_word_count && i < wildcard_count; i++)
			{
				candidate_array[count] = new_candidates[i];
				candidate_array[count].word.type = CI_TYPE_WILDCARD;
				count++;
			}
		}
	}

	//ǰ���ProcessCiCandidate�Ⱥ�����Ҳ�������裬��SortCiCandidates��
	//����ֻ�����ĳ�ֺ�ѡ����(���)�ڲ��������������������漰����ͬ
	//�ĺ�ѡ����

	//8.�Ժ�ѡ��������
	SortCandidates(candidate_array, count, syllable_count);

	//9.�����������������
	if (wildcard_count > 0)
	{
		int nCount;
		if (wildcard_count < pim_config->suggest_word_count)
			nCount = wildcard_count;
		else
			nCount = pim_config->suggest_word_count;
		SortWildcardCandidates(context, candidate_array, count, nCount);
	}

	//10.���spw��ѡȥ��(spw����ǡ�ö�������֡��ʺ�ѡ��ͬ)
	if (spw_count)
		count = UnifyCandidates(context, candidate_array, count, spw_count);

	//�Զ���λ�ý����趨��Ĭ��Ϊ3(��Ϊpim_config->spw_positionĬ��Ϊ13)��
	//��ʱ��֧���޸ġ��������ж����ѡ������ѡ�в�ֻ�Ƕ���������ֻ�Ե�
	//һ�����ѡ��Ч��Ϊ�������������ã�spw_position����ֵ��11~19��ʹ��ʱ
	//ֱ�Ӽ�10
	if(spw_count == 1 && spw_count != count && count > 0 && candidate_array[0].spw.type == SPW_STRING_NORMAL){
		SortSPWCandidates(context, candidate_array, count, eng_count);
	}

	//11.��Ӣ�ĺ�ѡ��������
	if (1 == eng_count && (count > 0 || context->state == STATE_IINPUT) && context->has_english_candidate)
	{
		SortEnglishCandidates(context, candidate_array, count);
	}

	//���ֻ��һ����ѡ��������Ӣ�ĵģ�����ʾ����Ϊ������������MCC��GR13���֣���Ӣ�ĺ�ѡ��ʹ�ÿո��������������Ĳ�һ��
	//˫ƴʱ��E��Ϊֱͨ��������ĸ�����Գ���
	//ȫƴʱ��u����
	if(1 == eng_count && count == 1 && spw_count == 0 )
		count = 0;

	return count;
}