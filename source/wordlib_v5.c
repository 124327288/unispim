/* V5�ʿ⴦����
 * ��ҪĿ��Ϊ��V5�ʿ��е����û�����ʻ㣬Ȼ��ϲ����û�V6�ʿ���

�ʿ�ṹ˵�� 

1.ͷ��
	Magic Number:	0x19990604
	Word Index Table(WIT)
	WinTime
	Free Address:	�ʿ�Ĵ�С

2. ������˵����WORD index table)

	+-------------+         +-------------------------+
	|   ˫�ִ�    |-------> | ��������ĸΪKey��hash�� |
	+-------------+         +-------------------------+
	|   ���ִ�    |
	+-------------+
	|   ......    |
	+-------------+
	|   ......    |
	+-------------+
	|   ���ִ�    |
	+-------------+

2.1 hash��˵��
	һ��18x18�����飬��ʾ��b����f����ĸ����
	ע��ģ��������Ϊ����ͬ�ġ������жϣ����ôʵķ������С�

	+------+     +------------------+
	| b, b |---->| ���б�(wordlist) |
    +------+     +------------------+
	| b, j | 
	+------+
	| .... | 
	+------+
	| .... | 
	+------+
	| f, f | 
	+------+
2.2 ���б�(wordlist)˵��	

	2.2.1 ����˵��(worditem)
		ע��ÿ����λ��Ϊ16λ����(short)

		���������ָ�������Ϣ
		������(ÿ��16λ)��
			1st����
			2nd����
			.......
			9th����
		������
		��һ���ʣ�
			1st��
			2nd��
			.....
			9th��
		ע������ͨ��MN_NUMBER����xor���ܣ�MN_NUMBER:0xfdef
		ָ������
			next WORD address(NWA):		��һ����ͬ�ʵĵ�ַ	24λ
			next entry address(NEA):	��һ����ͬ�ʵĵ�ַ	24λ
		�ڶ����ʣ�
			1st��
			2nd��
			.....
			9th��
		ָ������
			next entry address(NEA):	��һ����ͬ�ʵĵ�ַ	24λ
			��������:	8λ�����λ��ʾϵͳ�ʻ㣬����λ��ʾ��Ƶ��
		......
		......
		......
		......
		���һ����
			1st��
			2nd��
			.....
			9th��
		ָ������
			next entry address(NEA):	��һ����ͬ�ʵĵ�ַ	24λ��ֵΪ0.
			��������:	8λ�����λ��ʾϵͳ�ʻ㣬����λ��ʾ��Ƶ��


	������˵����
		λ��
		0-3	:	��������
		4-6	:	reserved
		7	:	��ʱ�־	1:y,0:n
		8-e	:	�ʻ�Ƶ��
        f	:	ϵͳ�ʻ�
		ע����ɾ���Ĵʻ��Ƶ��Ϊ0.

	2.2.2 �б�˵��

		+------------+
		|feature val |		�ʵ�������Ϣ
		+------------+
		| 1st entry  |
		+------------+
   +---<|    NWA     |		24λ, next WORD address
   |	+------------+
   | +-<|    NEA     |		24λ��next entry address
   | |  +------------+
   | +->| 2nd entry  | >-----+
   | 	+------------+       |		
   | +-<|    NEA     |       |-----> �ڶ���Ŀ����Ϣ			
   | |	+------------+       |		
   | |	|  feature   | >-----+	
   | |	+------------+
   | +->| 3rd entry  | >-----+
   |	+------------+       |		
   |	|    NEA     |       |-----> ������Ŀ����Ϣ
   |	+------------+       |		
   |	|  feature   | >-----+	
   |	+------------+
   |	|   ......   |
   |	+------------+
   |	|   ......   |
   |	+------------+
   |	| last entry | >-----+
   |	+------------+       |		
   |	|    000     |       |-----> �����Ŀ����Ϣ
   |	+------------+       |		
   |	|  feature   | >-----+	
   |	+------------+
   +--->|  next WORD |
		+------------+
	
*/

#include <assert.h>
#include <utility.h>
#include <wordlib.h>
#include <kernel.h>
#include <tchar.h>
#include <share_segment.h>

//ÿ�η�����ڴ���Ŀ
#define	WORDS_PER_ALLOC		0x10000
#define	SYLLABLEV5			WORD

//V5�ʿ�ṹ
typedef struct 
{
	WORD feature;								//��Ƶ
	WORD length;								//�ʳ���
	int  freq_v6;								//V6��Ƶ
	SYLLABLEV5 syllable[MAX_WORD_LENGTH];		//��������
	SYLLABLE   syllable_v6[MAX_WORD_LENGTH];	//V6��������
	HZ hz[MAX_WORD_LENGTH];						//��������
}ITEMV5;

ITEMV5	*user_word_array = 0;				//�û��ʻ�����

int max_words_now = 0;
int user_word_count = 0;	
int wl_end;										//WORD library buffer's length

//��������ֽڵ�ֵ
int Get3Bytes(BYTE *p)
{
	DWORD x, y, z;
	DWORD v;

	assert(p);

	z = *p++;
	y = *p++;
	x = *p++;
	v = (x << 16) | (y << 8) | z;

	return (int)v;
}

/**	װ�شʿ⵽�ڴ�
 *	ע�⣺��ʹ�õ�ʱ����Ҫ�ͷ�
 *	������
 *		wlname		�ʿ�����
 *	���أ�
 *		�ɹ������شʿ��ָ��
 *		ʧ�ܣ�0
 */
WORDLIBV5 *LoadWordLibraryV5(const TCHAR *wl_name)
{
	char *buffer;
	int  length;

	assert(wl_name);
	length = GetFileLength(wl_name);
	if (length <= 0)
		return 0;

	buffer = (char*)malloc(length);
	if (!buffer)
		return 0;

	if (length != LoadFromFile(wl_name, buffer, length))
	{
		free(buffer);
		return 0;
	}

	return (WORDLIBV5*) buffer;
}

/**	����һ���û��ʻ�
 *	������
 *		length		�ʻ㳤��
 *		feature		�ʻ�������
 *		syllable	��������
 *		ci			��������
 */
void AddWordToArray(int length, int feature, SYLLABLEV5 *syllable, HZ *ci)
{
	ITEMV5 *new_user_words;
	static count;


	assert(syllable);
	assert(ci);

	count++;
	//if (!(count % 0x10))
	//	printf("%d\r", count);

	//�ж��û��ʻ������Ƿ�Խ��
	if (user_word_count >= max_words_now)   //MAX_USER_WORDS)
	{
		new_user_words = (ITEMV5*) malloc((max_words_now + WORDS_PER_ALLOC) * sizeof(ITEMV5));
		if (!new_user_words)		//memory is not enough, so ignore words
			return;

		if (user_word_count)
			memcpy(new_user_words, user_word_array, sizeof(ITEMV5) * max_words_now);

		max_words_now += WORDS_PER_ALLOC;
		free(user_word_array);
		user_word_array = new_user_words;
	}

	//�����û�����
	user_word_array[user_word_count].length = length;
	user_word_array[user_word_count].feature = (WORD)(feature & 0x7f);	

	memcpy((char *)(user_word_array[user_word_count].syllable), syllable, length * sizeof(SYLLABLE));
	memcpy((char *)(user_word_array[user_word_count].hz), ci, length * sizeof(HZ));
	user_word_array[user_word_count].syllable[length] = 0;
	user_word_array[user_word_count].hz[length] = 0;
	user_word_count++;
}

/**	�ڴʿ���Ѱ���û����������뵽�û�����������
 *	������
 *		length			�ʳ���
 *		con1, con2		��ĸ��ʶ
 *		wl_data			�ʿ�����ָ��
 */
void ExtractUserWordFromLibrary(int length, WORD con1, WORD con2, WORDLIBV5 *wl_data)
{
	int i;
	WORD *p;
	WORD feature;
	DWORD next_word_address, next_entry_address = 0, old_word_address, old_entry_address;
	SYLLABLEV5 syllable[MAX_WORD_LENGTH];
	WORD user_word[MAX_WORD_LENGTH];

	assert(wl_data);

	next_word_address = wl_data->index_table[length - 2][con1][con2];
	if (!next_word_address)		//û�д���
		return;

	//����
	while(next_word_address)
	{
		p = (WORD*)((char*)wl_data + next_word_address);		//pָ���鿪ʼ�Ĵ���

		if (IsBadReadPtr(p, sizeof(WORD)))
			break;
		
		feature = *p++;									//��õ�һ��������������
		
		//�����������������
		for (i = 0; i < length; i++)
			syllable[i] = *p++;
		syllable[i] = 0;		

		for (i = 0; i < length; i++)
			user_word[i] = *p++ ^ ENCODERV5;

		user_word[i] = 0;

		//���Ϊ�û�����Ĵʣ����뵽����
		if (!(feature & 0x8000) && (feature & 0x7f00))
			AddWordToArray(length, (feature >> 8) & 0x7f, syllable, user_word);

		//�����һ��ָ��
		old_word_address  = next_word_address;
		next_word_address = Get3Bytes((BYTE*)p);

		if (next_word_address && old_word_address == next_word_address)
			break;

		p = (WORD*)((BYTE*)p + 3);

		old_entry_address  = next_entry_address;
		next_entry_address = Get3Bytes((BYTE*)p);
	
		if (next_entry_address && old_entry_address == next_entry_address)
			break;

		p = (WORD*)((BYTE*)p + 3);

		//����
		while(next_entry_address)
		{
			p = (WORD*)((char*)wl_data + next_entry_address);	//ָ�����

			if (IsBadReadPtr(p, sizeof(WORD)))
				break;

			for (i = 0; i < length; i++)
				user_word[i] = *p++ ^ ENCODERV5;
			user_word[i] = 0;

			//�����һ���ĵ�ַ
			old_entry_address = next_entry_address;
			next_entry_address = Get3Bytes((BYTE*)p);
			
			if (next_entry_address && old_entry_address == next_entry_address)
				break;

			p = (WORD*)((BYTE*)p + 3);

			//���������
			feature = *(BYTE*)p;
			if (feature && !(feature & 0x80))
				AddWordToArray(length, feature, syllable, user_word);
		}
	}
}

/**	���ȫ���û������
 *	������
 *		wl_data			�ʿ�ָ��
 *	���أ�
 *		
 */
void ExtractAllWordsFromLibrary(WORDLIBV5 *wl_data)
{
	WORD i, j, k;

	assert(wl_data);

	user_word_count = 0;		//�趨��ֵ

	for (i = 2; i <= 9; i++)
		for (j = 0; j < 18; j++)
			for (k = 0; k < 18; k++)
				ExtractUserWordFromLibrary(i, j, k, wl_data);
}

/**	����û�����ʻ�
 *	�û�����ʣ����ᱣ����user_word_array�����У�������ĿΪuser_word_count
 *	������
 *		wl_name			�ʿ�����
 *	���أ�
 *		�ɹ���1
 *		ʧ�ܣ�0
 */
int LoadWordsArrayFromLibrary(const TCHAR *wl_name)
{
	WORDLIBV5 *wl_data;

	assert(wl_name);

	wl_data = LoadWordLibraryV5(wl_name);
	if (!wl_data)
		return 0;

	ExtractAllWordsFromLibrary(wl_data);
	
	free(wl_data);
	return 1;
}

/**	��V5�����ڱ任ΪV6������
 *	������
 *		syllable_v5		V5��������
 *		syllable_v6		V6��������
 *		length			���ڳ���
 *	���أ�
 *		�ɹ���1
 *		ʧ�ܣ�0
 */
int SetV6Syllable(SYLLABLEV5 *syllable_v5, SYLLABLE *syllable_v6, int length)
{
	int i;

	if (!share_segment)
		return 0;

	for (i = 0; i < length; i++)
	{
		int index = (int)syllable_v5[i];

		if (index < 0 || index >= share_segment->syllablev5_map_items)
			return 0;

		//V5�ʿ�����û����ĸ�������Ӧ�øɵ�
		if (share_segment->syllablev5_map[index].vow == VOW_NULL)
			return 0;

		syllable_v6[i].con = share_segment->syllablev5_map[index].con;
		syllable_v6[i].vow = share_segment->syllablev5_map[index].vow;
		syllable_v6[i].tone = TONE_0;
	}

	return 1;
}

/**	����V5�Ĵ�Ƶ��V6�Ĵ�Ƶ��
 *	ע��V5���û���Ƶ���120�����100��ϵͳ��Ƶ����Ը���
 *	������ʽ��
 *		v6_freq = (v5_freq - 100) * 20 + 100; (v5_freq >=100)�����
 *		v6_freq = v5_freq; (v5_freq < 100�����)
 */
int AdjustV5Freq(int v5_freq)
{
	if (v5_freq < 100)
		return v5_freq * 100;

	v5_freq = (v5_freq - 100) * 200000;			//�����ӽ�50��
	if (v5_freq > WORDLIB_MAX_FREQ)
		v5_freq = WORDLIB_MAX_FREQ;
	return v5_freq;
}

/**	��V5��V3�Ĵʿ��е��û��������뵽V6�е��û��ʿ��С�
 *	ע�⣺���ڴʿ⴦��ʽ�Ĳ�ͬ��V3�Լ�V5�е��û��ʻ�Ż��������µĴʿ�����С�
 *	������
 *		wl_file_name			ԭʼ�ʿ��ļ�����
 *	���أ�
 *		�ɹ�	1
 *		ʧ��	0
 */
int UpgradeWordLibFromV5ToV6(const TCHAR *wl_file_name)
{
	int  i, err, wl_reloaded;
	int  uwl_id;
	TCHAR uwl_name[MAX_PATH];
	extern HINSTANCE global_instance;
	extern int resource_thread_finished;

	while (!resource_thread_finished)
		Sleep(0);

	ShowWaitingMessage(TEXT("���ڵ���V3/V5�û��ʻ�......"), global_instance, 500);

	err = 1;
	do
	{
		//���V3V5�ʿ�
		GetFileFullName(TYPE_ALLAPP, WORDLIB_V3V5_FILE_NAME, uwl_name);
		if (!FileExists(uwl_name))
		{
			if (!CreateEmptyWordLibFile(uwl_name, WORDLIB_V3V5_NAME, DEFAULT_USER_WORDLIB_AUTHOR, 1))
				return 0;
		}

		//װ�ص��ڴ�
		uwl_id = LoadWordLibraryWithExtraLength(uwl_name, WORDLIB_CREATE_EXTRA_LENGTH, 1);
		if (uwl_id == -1)
			break;

		//1. Ѱ�����е�V3�ʿ���û�����
		if (!LoadWordsArrayFromLibrary(wl_file_name))
			break;

		wl_reloaded = 0;
		//3.���뵽�ʿ���
		for (i = 0; i < user_word_count; i++)
		{
			HZ hz_uc[MAX_WORD_LENGTH] = {0};

			//�任���ڣ�ע�����ڵĴ���
			if (!SetV6Syllable(user_word_array[i].syllable, user_word_array[i].syllable_v6, user_word_array[i].length))
				continue;

			user_word_array[i].freq_v6 = AdjustV5Freq(user_word_array[i].feature & 0x7f);

			AnsiToUtf16((char*)user_word_array[i].hz, hz_uc, MAX_WORD_LENGTH);

			if (AddCiToWordLibrary(
						uwl_id,
						hz_uc, 
						user_word_array[i].length, 
						user_word_array[i].syllable_v6, 
						user_word_array[i].length,
						user_word_array[i].freq_v6))
				continue;

			//ʧ�ܣ����ܴʿ���
 			SaveWordLibrary(uwl_id);					//����ʿ�����
			CloseWordLibrary(uwl_id);					//�رմʿ�
			uwl_id = LoadWordLibraryWithExtraLength(uwl_name, WORDLIB_CREATE_EXTRA_LENGTH, 0);
			if (uwl_id == -1)							//װ��ʧ��
				break;

			wl_reloaded = 1;
			share_segment->can_save_user_wordlib = 0;

			if (AddCiToWordLibrary(
						uwl_id,
						user_word_array[i].hz, 
						user_word_array[i].length, 
						user_word_array[i].syllable_v6, 
						user_word_array[i].length,
						user_word_array[i].freq_v6))
				continue;

			break;
		}
		if (i != user_word_count)
			break;

		//4. ����ʿ�
		SaveWordLibrary(uwl_id);

		if (wl_reloaded)
			CloseWordLibrary(uwl_id);			//ɾ������

		err = 0;

	}while(0);

	ShowWaitingMessage(0, 0, 500);

	if (err)
		return 0;

	return 1;
}

#define	TMP_FILE_NAME	TEXT("upim.tmpfile")

/**	�ж��Ƿ�ΪV3/V5�Ĵʿⱸ���ļ�
 *	���裺
 *		1. ��ѹ��
 *		2. װ��
 *		3. �ж�
 *		4. ɾ��
 *	���أ�
 *		1���ǣ�0����
 */
int IsV3WordLibBK(const TCHAR* wl_name)
{
	WORDLIBHEADER header;
	TCHAR tag_name[MAX_PATH];

	if (!GetFileFullName(TYPE_TEMP, TMP_FILE_NAME, tag_name))
		return 0;

	if (!UncompressFile(wl_name, tag_name, sizeof(header)))
		return 0;

	if ((LoadFromFile(tag_name, (char*)&header, sizeof(header)) < sizeof(header)) ||
		(header.signature != HYPIM_WORDLIB_V5_SIGNATURE))
	{
		_tunlink(tag_name);
		return 0;
	}

	_tunlink(tag_name);
	return 1;
}

/**	��V5�ı����ļ�������V6�ʿ�
 */
int CheckAndUpgrandWordLibFromV5BKToV6(const TCHAR *wl_name)
{
	WORDLIBHEADER header;
	int ret;
	TCHAR tag_name[MAX_PATH];

	if (!GetFileFullName(TYPE_TEMP, TMP_FILE_NAME, tag_name))
		return 0;

	if (!UncompressFile(wl_name, tag_name, -1))
		return 0;

	if ((LoadFromFile(tag_name, (char*)&header, sizeof(header)) < sizeof(header)) ||
		(header.signature != HYPIM_WORDLIB_V5_SIGNATURE))
	{
		_tunlink(tag_name);
		return 0;
	}

	ret = UpgradeWordLibFromV5ToV6(tag_name);
	_tunlink(tag_name);

	return ret;
}
