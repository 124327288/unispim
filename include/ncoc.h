/* tcoc_lib.h
   ��Ԫ���ָ��ʿ⹹��ͷ�ļ�
*/

#ifndef	_NCOC_LIB_H_
#define	_NCOC_LIB_H_

typedef unsigned short HZ;
typedef unsigned short WORD;
typedef unsigned char  BYTE;

#define	BCOC_SIGN		0x2c0c1999
#define	BCOC_NAME		"bcoc.dat"
#define	TCOC_SIGN		0x3c0c1999
#define	TCOC_NAME		"tcoc.dat"
#define	BCOC_SIZE		0x400000
#define	TCOC_SIZE		0x2000000
#define	END				0
#define IsGB(hz) 		(((hz & 0xff) >= 0xb0) && ((hz & 0xff) <= 0xf7) && ((hz >> 8) >= 0xa1) && ((hz >> 8) <= 0xfe))
#define	HZ				unsigned short
#define	TERMINAL		(*(HZ*)"��")

#pragma pack(1)

//����������
typedef struct 
{
	HZ		hz;
	int		item_pos;
}INDEXITEM;

//NCOC������
typedef struct
{
	HZ		hz;									//����
	BYTE	count;								//��ֵ�����ֽ�����ѹ��������
}NCOCITEM;

//NCOC�ļ�ͷ��
typedef struct tagTCOCHEADER
{
	int			sign;								//tcoc��ʶ
	int			index0_count;						//��һ��������Ŀ
	int			index1_count;						//�ڶ���������Ŀ
	int			item_count;							//bcoc/tcoc)����Ŀ
	int			index0_data_pos;					//�������ڼ���ָ��Ŀռ�
	int			index1_data_pos;					//�������ڼ���ָ��Ŀռ�
	int			item_data_pos;						//�����û�����ָ��Ŀռ�

	//�������
	//index0_data = (INDEXITEM*)tcoc_data.index0_data;
	//index1_data = (INDEXITEM*)((char*)index0_data + sizeof(INDEXITEM) * index0_count);
	//item_data   = (NCOCITEM*)((char*)index1_data + sizeof(INDEXITEM) * index1_count);

}TCOCHEADER;

//BCOC�ļ�ͷ��
typedef struct tagBCOCHEADER
{
	int			sign;								//bcoc��ʶ
	int			index0_count;						//��һ��������Ŀ
	int			item_count;							//bcoc/tcoc)����Ŀ
	int			index0_data_pos;					//�������ڼ���ָ��Ŀռ�
	int			item_data_pos;						//�����û�����ָ��Ŀռ�

	//�������
	//index0_data = (INDEXITEM*)bcoc_data.index0_data;
	//item_data   = (BCOCITEM*)((char*)index1_data + sizeof(INDEXITEM) * index0_count);

}BCOCHEADER;

//TCOC�ļ��ṹ
typedef struct tag_TCOCDATA
{
	//����1K����������
	union
	{
		TCOCHEADER header;
		char align_data[0x400];					//1K��������
	};
	char	data[0x10];							//TCOC���ݿ�ʼλ��
}TCOCDATA;

//BCOC�ļ��ṹ
typedef struct tag_BCOCDATA
{
	//����1K����������
	union
	{
		BCOCHEADER header;
		char align_data[0x400];					//1K��������
	};
	char	data[0x10];							//BCOC���ݿ�ʼλ��
}BCOCDATA;

#pragma pack()

int get_bcoc(const HZ *hz);
int get_tcoc(const HZ *hz);

int open_bcoc(const char *name);
int open_tcoc(const char *name);

#endif
