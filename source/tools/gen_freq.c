/**	�������м����ض����ȵĴʻ�Ĵ�Ƶ��
 *	�����Ӵ��ڴ��С��������ñȽϽ����Ŀ�������
 *
 *	���㷨����Hash��ķ�ʽ���У�
 *	����Hashɢ�еĳ�ͻ�����ԣ�ÿһ���а���
 *	��ͬKey����һ�����ݡ�
 *
 *	�����в�����
 *		�����ļ����ֿ���Ϊ����ͨ������ļ�����
 */
#define		_CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <time.h>
#include <windows.h>

#define	MIN_ITEM_LENGTH			2						//��С�ʳ���
#define	MAX_ITEM_LENGTH			16						//���ʳ���

#define	MAX_FREQ				0x7fffffff				//����Ƶ��Ŀ
#define	LOW_FREQ				2						//��Ƶ�ʵ�����Ƶ
#define	MIN_SWAP_DIFF			0						//���������Ĵ�Ƶ����

#define	DEFAULT_FILE_NAME		"freq.txt"				//����ļ�Ĭ������
#define	DEFAULT_HASH_SIZE		(32 * 0x100000)			//Ĭ��Hash������Ŀ
#define	DEFAULT_ITEM_SIZE		(640 * 0x100000)		//Ĭ�ϴ�����Ŀ

#define	END						0
#define	TERMINAL				(*(HZ*)"��")

#define	ERR_NO_MEMORY			-1
#define	ERR_ITEM_FULL			-2
#define	ERR_FILE_OPEN			-3

typedef	unsigned short HZ;

typedef struct tagITEM
{
	struct tagITEM	*next;						//��һ���ͻ����
	unsigned int	count;						//��Ƶ
	HZ				ci[2];						//�ʻ�
}ITEM;

int		verbose = 1;							//�Ƿ���ʾ��ʾ��Ϣ

int		need_terminator = 1;					//�Ƿ���Ҫ�ָ���������
char	*output_file = DEFAULT_FILE_NAME;		//����ļ�����

unsigned int keys[0x100][0x10];					//��ASCII����Ϊ��λ��ɢ��key�������ڲ�ͬλ�õ�key����ͬ��
int		max_item_length = MAX_ITEM_LENGTH;		//������
int		min_item_length = MIN_ITEM_LENGTH;		//��С���

char	*item_table;							//Item������
int		item_size = DEFAULT_ITEM_SIZE;			//Item����Ŀ
char*	item_pointer;							//��ǰitem��ָ��

ITEM 	**hash_table;							//ɢ���õ�hash��ÿһ��ָ��Item������
const int hash_size = DEFAULT_HASH_SIZE;		//ɢ�б������Ŀ

int		file_size;								//�������ļ��ĳ��ȣ����ڼ��㴦��ٷֱȣ�
int		last_err;								//�����
int		percent = 0;							//������Ȱٷֱ�
long long char_processed = 0;					//��������ַ���Ŀ
long long total_chars = 0;						//ȫ���ַ���Ŀ

/**	��ô���ĳ���
 */
__inline int get_item_size()
{
	return sizeof(int) + sizeof(int) + sizeof(HZ) * max_item_length;
}

/**	�ж��Ƿ���������
 */
__inline int is_item_table_full()
{
	return item_pointer + get_item_size() >= item_table + item_size;
}

/**	��ʼ������
 */
int init()
{
	int i, j;

	if (verbose)
		printf("ϵͳ��ʼ��...\n");
	//��ʼ��hash��ֵ��
	srand(12345);
	for (i = 0; i < 0x10; i++)
		for (j = 0; j < 0x100; j++)
			keys[j][i] = (unsigned int)
				((rand() & 0xff) | ((rand() & 0xff) << 8) | ((rand() & 0xff) << 16) | ((rand() & 0xff) << 24));

	//��ʼ��hash��
	hash_table = (ITEM**) malloc(hash_size * sizeof(ITEM*));
	if (!hash_table)
	{
		last_err = ERR_NO_MEMORY;
		return 0;
	}

	//��ʼ�������
	item_table = (char*) malloc(item_size);
	item_pointer = item_table;
	if (!item_table)
	{
		last_err = ERR_NO_MEMORY;
		return 0;
	}

	memset(item_table, 0, item_size);
	memset(hash_table, 0, hash_size * sizeof(ITEM*));
	return 1;
}

/**	�ͷ��ڴ�
 */
void free_all()
{
	free(hash_table);
	free(item_table);
}

/**	�������
 *	Ϊ�ӿ�����ٶȣ����ܹ�ʹ��printf������һ����ʮ������
 */
void output_item(FILE *f, ITEM *item)
{
	int i;
	char *ci = (char*)item->ci;
	char buffer[0x100];
	char *p = buffer;

	if (!ci[0])
		return;

	for (i = 0; ci[i] && i < (int)(max_item_length * sizeof(HZ)); i++)
		*p++ = ci[i];

	*p++ = 9;	//TAB

	_ltoa(item->count, p, 10);

	p = buffer;
	while(*p)
		_fputc_nolock(*p++, f);
	_fputc_nolock(0xa, f);
}

/**	���ڴ��е��������
 */
int flush_memory()
{
	FILE *fw;
	ITEM *item;
	int  item_size;
	static int flush_count = -1;
	char file_name[0x400];

	flush_count++;

	if (verbose)
		printf("�����Ƶ���...\n");

	sprintf(file_name, "%d-%s", flush_count, output_file);
	fw = fopen(file_name, "w");
	if (!fw)
	{
		last_err = ERR_FILE_OPEN;
		return 0;
	}

	item_size = get_item_size();
	for (item = (ITEM*)item_table; (char*)item < item_pointer; item = (ITEM*)((char*)item + item_size))
		output_item(fw, item);

	fclose(fw);
	return 1;
}

/**	��ô�����hash��
 */
__inline unsigned int get_hash_key(const HZ *ci, int length)
{
	unsigned int key, x;
	int i;

	key = 0;
	for (i = 0; i < length; i++)
	{
		x = ci[i];
		key ^= keys[x & 0xff][i * 2];
		key ^= keys[x >> 8][i * 2 + 1];
	}
	return key;
}

/**	��ô�����ɢ��ָ��
 */
ITEM **get_hash_item(const HZ *ci, int length)
{
	unsigned int key;

	key = get_hash_key(ci, length);
	key &= (hash_size - 1);

	return &hash_table[key];
}

/**	Item���Ƿ��������
 */
int ci_in_item(ITEM *item, const HZ *ci, int length)
{
	int i;

	//ǰ���Ƿ���ȫ��ͬ
	for (i = 0; i < length; i++)
		if (item->ci[i] != ci[i])
			return 0;

	//���������󳤶ȣ�����item�л����������֣�����ͬ
	if (length != max_item_length)
		if (item->ci[i])
			return 0;

	//��ȫ��ͬ
	return 1;
}

/**	������������һ��
 */
ITEM *insert_item(const HZ *ci, int length)
{
	ITEM *item;
	int  i;

	if (is_item_table_full())
	{	//���������
		last_err = ERR_ITEM_FULL;
		return 0;
	}

	//����´�
	item = (ITEM*)item_pointer;
	item->count = 1;
	item->next = 0;
	for (i = 0; i < length; i++)
		item->ci[i] = ci[i];
	for (; i < max_item_length; i++)
		item->ci[i] = 0;
	item_pointer += get_item_size();
	return item;
}

/**	��������
 */
void swap_item(ITEM *item1, ITEM *item2)
{
	int i, count;
	HZ hz;

	count = item1->count;
	item1->count = item2->count;
	item2->count = count;

	for (i = 0; i < max_item_length; i++)
	{
		hz = item1->ci[i];
		item1->ci[i] = item2->ci[i];
		item2->ci[i] = hz;
	}
}

/**	����һ��
 */
int process_item(const HZ *ci, int length)
{
	ITEM **hash_item;
	ITEM *item, *last_item;
	int  i;

	if (length == 1)
	{
		if (ci[0] == TERMINAL)
			return 1;
	}
	else if (length == 2)
	{
		if (*(int*)ci == *(int*)"����")
			return 1;
	}
	else if (length == 3)
	{
		if (ci[1] == TERMINAL)
			return 1;
		if (ci[0] == TERMINAL && ci[2] == TERMINAL)
			return 1;
	}
	else if (length == 4)
	{
		if (ci[0] == TERMINAL || ci[1] == TERMINAL ||
			ci[2] == TERMINAL || ci[3] == TERMINAL)
			return 1;
	}
	else
	{
		for (i = 0; i < length; i++)
			if (ci[i] == TERMINAL)
				return 1;
	}

	hash_item = get_hash_item(ci, length);

	if (!*hash_item)					//û�д洢����
	{
		//������δ���룬���뵽Item������
		item = insert_item(ci, length);
		if (!item)
			return 0;					//ʧ��

		*hash_item = item;
		return 1;
	}


	//��ͬһ��Slot��Ѱ����ͬ����
	item = *hash_item;
	last_item = 0;
	while(item)
	{
		if (ci_in_item(item, ci, length))
			break;
		last_item = item;
		item = item->next;
	}

	if (item)					//�ҵ�
	{
		if (item->count < MAX_FREQ)
			item->count++;
		//�����һ������һ��СһЩ������н���
		//СһЩ��ԭ����Ϊ�˱�����
		if (last_item)
			if (last_item->count + MIN_SWAP_DIFF < item->count)		//��������
				swap_item(last_item, item);
		return 1;
	}
	//û���ҵ�����Ҫ�ں���׷��

	item = insert_item(ci, length);
	if (!item)					//������ʧ��
		return 0;

	item->next = *hash_item;	//���뵽��ͻ�����ǰ��
	*hash_item = item;			//��hash���е�ָ��ָ�����������
	return 1;
}

/**	�ж��Ƿ�ΪGB2312�֣������Ƚϳ��õ�GBK�֣�
 */
__inline int is_gb(HZ zi)
{
	int  i;
	char special_zi[] = "����h�h���t�����F";

	if ((zi & 0xff) >= 0xb0 && ((zi & 0xff) <= 0xf7) && (zi >> 8) >= 0xa1 && (zi >> 8) <= 0xfe)
		return 1;		//GB2312����

	for (i = 0; i < sizeof(special_zi); i += 2)
		if (zi == *(HZ*)(special_zi + i))
			return 1;

	return 0;
}

/**	���ļ��ж��뺺��
 */
__inline HZ get_hz(FILE *f)
{
	unsigned char char1, char2;
	int ch;
	HZ hz;

	ch = _fgetc_nolock(f);
	if (ch == EOF)
		return END;

	char1 = (unsigned char)ch;
	char_processed++;

	if (char1 < 0x80)
		return TERMINAL;

	ch = _fgetc_nolock(f);
	if (ch == EOF)
		return END;

	char2 = (unsigned char)ch;
	char_processed++;

	hz = char2 * 256 + char1;

	if (!(char_processed & 0xff) && (char_processed * 100 / file_size) > percent)
	{
		percent = (int)(char_processed * 100 / file_size);
		printf("%%%d\r", percent);
	}

	if (is_gb(hz))
		return hz;

	return TERMINAL;
}

/**	�Ƚϴ���
 */
int compare_item(const ITEM *item1, const ITEM *item2)
{
	int i;

	//�ڽ���BCOC��TCOC���ߴ�Ƶ����ʱ���������ݴ�Ƶ��������
	if (0 && item1->count != item2->count)
		return item2->count - item1->count;
	for (i = 0; i < max_item_length; i++)
		if (item1->ci[i] != item2->ci[i])
			return item1->ci[i] - item2->ci[i];
	return 0;
//	return memcmp(item1->ci, item2->ci, max_item_length * sizeof(HZ));
}

/**	��������
 */
void sort_items()
{
	if (verbose)
		printf("�Դ����������...\n");
	qsort(item_table, (item_pointer - item_table) / get_item_size(), get_item_size(), compare_item);
	return;
}

/**	�����һ������
 */
__inline ITEM *get_next_item(ITEM *item)
{
	return (ITEM*)((char*)item + get_item_size());
}

/**	ɾ����Ƶ�ϵ͵Ĵ�������ƵΪ1��2��Ϊ��Ƶ�ϵ͡�
 *	���÷�ʽ��
 *		1. ����ȫ����������1��2��Ƶ�Ĵ���ɾ��������ά��ԭ�е����ӹ�ϵ
 *		2. �����ȫ���ն�����Ŀ
 *		3. �ٴμ���hash����hash�����ڿն���Ŀ֮��ʱ�������ƶ�
 *		4. ȫ����ɺ󣬿���ȷ���ܹ����з���Ŀռ䡣
 *	���أ�
 *		��������ɹ�������0���ɹ���1.
 */
int reduce_item()
{
	int i;
	int hole_count;					//�ն�����
	int moved_count;				//�ƶ����Ĵ�����Ŀ
	ITEM *item;						//����
	ITEM *hole_item;				//�ն���
	ITEM *last_item;				//�ϲ��ն������һ�������ַ
	ITEM **last_ptr;				//��һ��
	int  st, et;

	st = clock();
	//1. ���������Ƿ�Ϊ��Ƶ����������ǣ���ɾ������
	for (i = 0, hole_count = 0; i < hash_size; i++)
	{
		item = hash_table[i];
		last_ptr = &hash_table[i];
		while(item)						//������������
		{
			if (item->count <= LOW_FREQ)
			{
				item->ci[0] = 0;
				*last_ptr = item->next;
				item = item->next;
				hole_count++;
				continue;
			}

			last_ptr = &item->next;
			item = item->next;
		}
	}

	//�������Ĵ����ռ����128M���򷵻ز�������
	//��Ȼ��ɨ������Ƶ�ȹ���
	if (hole_count * get_item_size() < 0x8000000)
		return 0;

	//2. �ٴν��м�飬���ִ��ں󲿵Ĵ�������ǰ�ƶ�
	last_item = (ITEM*)(item_pointer - hole_count * get_item_size());
	for (i = moved_count = 0, hole_item = (ITEM*)item_table; 
			i < hash_size && moved_count < hole_count; 
			i++)
	{
		item = hash_table[i];
		last_ptr = &hash_table[i];
		while(item)						//������������
		{
			if (item >= last_item)		//�ں�
			{
				while(hole_item->count > LOW_FREQ)
					hole_item = get_next_item(hole_item);
				//���ƴ���
				memcpy(hole_item, item, get_item_size());
				*last_ptr = hole_item;
				item = hole_item;
				moved_count++;
			}

			last_ptr = &item->next;
			item = item->next;
		}
	}

	//3. ������ϣ���item_pointer���õ���ȷ��λ��
	item_pointer = (char*)last_item;

	et = clock();

	if (verbose)
		printf("���տռ�:%dM����ʱ%d\n", hole_count * get_item_size() / 0x100000, et - st);

	return 1;
}

/**	�����׼�����γ���
 */
int process_all(FILE *f)
{
	HZ  hz_tokens[MAX_ITEM_LENGTH];
	int i;
	int ret;

	for (i = 0; i < max_item_length; i++)
		hz_tokens[i] = TERMINAL;

	do
	{
		//���������ƶ�һ������
		for (i = 0; i < max_item_length - 1; i++)
			hz_tokens[i] = hz_tokens[i + 1];

		hz_tokens[i] = get_hz(f);
		if (hz_tokens[i] == END)
			break;

		for (i = min_item_length; i <= max_item_length; i++)
		{
			ret = process_item(hz_tokens, i);
			if (!ret)		//����
			{
				if (last_err != ERR_ITEM_FULL)
					return 0;
				last_err = 0;
				//������
				if (!reduce_item())
				{
					sort_items();
					flush_memory();
					free_all();
					if (!init())
						return 0;
				}
				i--;
			}
		}
	}while(1);

	return 1;
}

/**	�����������Ϣ���ж��Ƿ��������
 */
void check_chain_length()
{
	int i, length;
	int length_count[0x10];
	ITEM *item;

	if (!verbose)
		return;

	memset(length_count, 0, sizeof(length_count));
	for (i = 0; i < hash_size; i++)
	{
		item = hash_table[i];
		length = 0;
		while(item)
		{
			length++;
			item = item->next;
		}
		if (length >= 0x10)
			length = 0x10 - 1;
		length_count[length]++;
	}

	printf("������������Ŀ\n");
	for (i = 0; i < sizeof(length_count) / sizeof(length_count[0]); i++)
		printf("%02d:%d\n", i, length_count[i]);
}

const char *usage =
	"gen_freq input_files min_length max_length output_file [item_size]\n"
	"  input_files: �����ļ����֣����Ը���ͨ���\n"
	"  min_length:  ��С�ʳ��ȣ���С2��\n"
	"  max_length:  ���ʳ��ȣ����16��\n"
	"  output_file: ����ļ�����\n"
	"  item_size:   �������ݵ��ڴ���������MΪ��λ\n";

static char file_buffer[0x100000];

int main(int argc, char **argv)
{
	HANDLE f_handle;
	WIN32_FIND_DATA f_data;
	char *in_file;
	FILE *fr;
	int  st0, st, et;

	if (argc < 5)
	{
		printf("%s", usage);
		return -1;
	}

	st0 = st = clock();
	min_item_length = atoi(argv[2]);
	max_item_length = atoi(argv[3]);
	output_file = argv[4];

	if (argc == 6)	//��item_size
		item_size = atoi(argv[5]) * 0x100000;
	if (item_size < 0x200000)
	{
		printf("�ڴ�̫С���޷�����������2M\n");
		return -1;
	}

	_unlink(output_file);				//���Ƚ�ԭʼ��¼ɾ��

	//��ʼ��
	if (!init())
		return 0;
	et = clock();
	if (verbose)
		printf("��ʼ����ʱ:%d\n", et - st);

	f_handle = FindFirstFile(argv[1], &f_data);
	if (!f_handle)
	{
		printf("err:%d\n", ERR_FILE_OPEN);
		return -1;
	}

	do
	{
		int st1, et1;

		st1 = clock();
		in_file = f_data.cFileName;
		printf("����<%s>�ļ�\n", in_file);
		file_size = f_data.nFileSizeLow;			//���������2G���ļ�
		char_processed = percent = 0;
		fr = fopen(in_file, "rb");
		if (!fr)
			printf("<%s>�ļ��޷��򿪣�����\n");

		//���û�������С���ӿ�������ٶ�
		if (setvbuf(fr, file_buffer, _IOFBF, sizeof(file_buffer)))
			printf("���û���������\n");

		if (!process_all(fr))
		{
			printf("err:%d\n", last_err);
			return -1;
		}
		fclose(fr);
		total_chars += char_processed;

		et1 = clock();
		if (verbose)
			printf("������ʱ:%d, %4.2fM/s\n", 
					et1 - st1, 
					1000.0 * char_processed / 1024.0 / 1024.0 / (et1 - st1));
	}while(FindNextFile(f_handle, &f_data));

	FindClose(f_handle);
	et = clock();
	if (verbose)
		printf("������ʱ:%d, %4.2fM/s\n", 
				et - st, 
				1000.0 * total_chars / 1024.0 / 1024.0 / (et - st));

	//����������
	st = et;
	check_chain_length();
	et = clock();
	if (verbose)
		printf("�������������ʱ:%d\n", et - st);

	//���ٴ�Ƶ���ʹ���
	reduce_item();
	st = et;
	et = clock();
	if (verbose)
		printf("�����ռ���ʱ:%d\n", et - st);

	//����
	sort_items();
	st = et;
	et = clock();
	if (verbose)
		printf("������ʱ:%d\n", et - st);

	//���
	flush_memory();
	st = et;
	et = clock();
	if (verbose)
		printf("д���ļ���ʱ:%d\n", et - st);

	//�ͷ�
	free_all();

	et = clock();
	if (verbose)
		printf("��������ʱ:%d\n", et - st0);

	return 0;
}
