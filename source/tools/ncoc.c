#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ncoc.h>

TCOCDATA *tcoc_data;
BCOCDATA *bcoc_data;

/**	���BCOC
 */
int get_bcoc(const HZ *hz)
{
	int start, end, mid, start_sav, end_sav;
	HZ hz0, hz1;
	INDEXITEM *index0;
	NCOCITEM  *item;

	hz0 = hz[0], hz1 = hz[1];
	index0 = (INDEXITEM*)((char*)bcoc_data->data + bcoc_data->header.index0_data_pos);
	item = (NCOCITEM*)((char*)bcoc_data->data + bcoc_data->header.item_data_pos);

	//�Ե�һ���������м��������һ��Ϊ0
	start = 0, end = bcoc_data->header.index0_count - 2;
	while(start <= end)
	{
		mid = (start + end) / 2;
		if (index0[mid].hz == hz0)
			break;
		if (hz0 > index0[mid].hz)
			start = mid + 1;
		else
			end = mid - 1;
	}
	if (start > end)		//û�и���
		return 1;

	start_sav = start = index0[mid].item_pos;			//�ڶ���������ʼ/����λ��
	end_sav = end = index0[mid + 1].item_pos - 1;		//��Ҫ���������䱣��, �����ظ����ж�

	//����������м���(�����ظ���Ķ��ַ�����)
	while(start <= end)
	{
		mid = (start + end) / 2;
		if (item[mid].hz == hz1)
			break;
		if (hz1 > item[mid].hz)
			start = mid + 1;
		else
			end = mid - 1;
	}
	if (start > end)			/* û�и��� */
		return 1;

	//���ڿ��ܳ����ظ���, ����Ҫ�����ߵ����ݽ����ж�
	if (mid > start_sav && item[mid - 1].hz == hz1)	//ǰ���뵱ǰ����ͬ
		return item[mid].count * 0x100 + item[mid - 1].count;
	if (mid < end_sav && item[mid + 1].hz == hz1)		//�����뵱ǰ����ͬ
		return item[mid + 1].count * 0x100 + item[mid].count;
	return item[mid].count;
}

/**	��ú��ִ���TCOC
 */
int get_tcoc(const HZ *hz)
{
	int start, end, mid, start_sav, end_sav;
	HZ hz0, hz1, hz2;
	INDEXITEM *index0, *index1;
	NCOCITEM  *item;

	hz0 = hz[0], hz1 = hz[1], hz2 = hz[2];
	item = (NCOCITEM*)((char*)tcoc_data->data + tcoc_data->header.item_data_pos);
	index0 = (INDEXITEM*)((char*)tcoc_data->data + tcoc_data->header.index0_data_pos);
	index1 = (INDEXITEM*)((char*)tcoc_data->data + tcoc_data->header.index1_data_pos);

	//�Ե�һ���������м���
	start = 0, end = tcoc_data->header.index0_count - 2;
	while(start <= end)
	{
		mid = (start + end) / 2;
		if (index0[mid].hz == hz0)
			break;
		if (index0[mid].hz < hz0)
			start = mid + 1;
		else
			end = mid - 1;
	}
	if (start > end)		//û�и���
		return 1;

	start = index0[mid].item_pos;		
	end   = index0[mid + 1].item_pos - 1;	//�ڶ���������ʼ/����λ��

	//�Եڶ����������м���
	while(start <= end)
	{
		mid = (start + end) / 2;
		if (index1[mid].hz == hz1)
			break;
		if (index1[mid].hz < hz1)
			start = mid + 1;
		else
			end = mid - 1;
	}
	if (start > end)		//û�и���
		return 1;

	start_sav = start = index1[mid].item_pos;			//�����ʼ/����λ��
	end_sav = end = index1[mid + 1].item_pos - 1;

	//����������м���(�����ظ���Ķ��ַ�����)
	while(start <= end)
	{
		mid = (start + end) / 2;
		if (item[mid].hz == hz2)
			break;
		if (item[mid].hz < hz2)
			start = mid + 1;
		else
			end = mid - 1;
	}
	if (start > end)			//û�и���
		return 1;

	//���ڿ��ܳ����ظ���, ����Ҫ�����ߵ����ݽ����ж�
	if (mid > start_sav && item[mid - 1].hz == hz2)	//ǰ���뵱ǰ����ͬ
		return item[mid].count * 0x100 + item[mid - 1].count;
	if (mid < end_sav && item[mid + 1].hz == hz2)	//�����뵱ǰ����ͬ
		return item[mid + 1].count * 0x100 + item[mid].count;
	return item[mid].count;
}

/**	��BCOC�ļ�
 */
int open_bcoc(const char *name)
{
	FILE *f;

	bcoc_data = (BCOCDATA*)malloc(BCOC_SIZE);
	if (!bcoc_data)
	{
		fprintf(stderr, "�ڴ治��.\n");
		return 0;
	}

	f = fopen(name, "rb");
	if (!f)
	{
		fprintf(stdout, "�ļ�(%s)�򿪳���.\n");
		return 0;
	}
	fread(bcoc_data, 1, BCOC_SIZE, f);
	fclose(f);

	return 1;
}

/**	��TCOC�ļ�
 */
int open_tcoc(const char *name)
{
	FILE *f;

	tcoc_data = (TCOCDATA*)malloc(TCOC_SIZE);
	if (!tcoc_data)
	{
		fprintf(stderr, "�ڴ治��.\n");
		return 0;
	}

	f = fopen(name, "rb");
	if (!f)
	{
		fprintf(stdout, "�ļ�(%s)�򿪳���.\n", name);
		return 0;
	}
	fread(tcoc_data, 1, TCOC_SIZE, f);
	fclose(f);

	return 1;
}

