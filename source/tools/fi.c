/*	file iterator
 *	�ļ�ϵͳ������. ������Ŀ¼������, ����
 *	ÿһ���ļ�, ������������(��findfirst����
 *	��ͬ).
 *	ע: Ŀǰ����thread safe�汾.
 *	WangChuan, 2004-04-12.
 *	ʹ��:
 *		1.����fi_data_t �ṹ����.
 *		2.����fi_first(dir, fi_data_t*), ����ֵΪ���β���
 *		  ��handle.
 *		3.����fi_next(handle, fi_data_t*);
 *		4.ȫ��������, ����fi_close(handle);
 */
#define		_CRT_SECURE_NO_DEPRECATE
#include "fi.h"
#include <string.h>

#define	FI_STAGE_START		0	//��ʼ״̬
#define	FI_STAGE_ENTER		1	//����Ŀ¼
#define	FI_STAGE_SEEK_BEGIN	2	//�����ļ�, ��Ҫ��ʼ��
#define	FI_STAGE_SEEKING	3	//�����ļ�, �����ʼ��
#define	FI_STAGE_SEEK_END	5	//�˳�Ŀ¼
#define	FI_STAGE_JUDGE_FILE	4	//ƥ���ļ�
#define	FI_STAGE_RETURN		6	//���ؽ��
#define	FI_STAGE_END		7	//����״̬

static int fi_error;

struct fi_data
{
	int stage;					//����׶�
	int fd_handle[FI_MAX_DEPTH];	//findfirst/findnextʹ�õľ��/
	int depth;					//Ŀ¼���
	char *cur_dir;				//��ǰĿ¼
	char *next_dir;				//��һ��Ŀ¼
	char *name;					//��ǰ�ļ�����
};

/* construct inner fi data, check flags, do first find 
 *	input:
 *		dir		-> starting directory, 0->current dir
 *		p_fd	-> a fi_data_t struct, used for returning
 *					file information.
 *	output:
 *		n		-> handle of inner data structure
 *		0		-> failed.
 */
int fi_first(const char *dir, struct fi_data_t *p_fd)
{
	char cwd[_MAX_PATH];
	struct fi_data *p_fi;

	p_fi = (struct fi_data*)malloc(sizeof(struct fi_data));
	if (!p_fi)
	{
		fi_error = FIE_NOMEMORY;
		return 0;
	}
	memset(p_fi, 0, sizeof(*p_fi));

	if (!dir)			//current dir
		dir = _getcwd(cwd, sizeof(cwd));
	p_fi->depth = 0;
	p_fi->stage = FI_STAGE_START;
	p_fi->cur_dir = _strdup(dir);
	p_fi->next_dir = _strdup(dir);

	//����ڴ治�������ߵ�ǰĿ¼�������β��������ء�
	if (!p_fi->cur_dir || !p_fi->next_dir)
	{
		fi_error = FIE_NOMEMORY;
		fi_close((int)p_fi);
		return 0;
	}

	if (-1 == fi_next((int)p_fi, p_fd))
	{	//fi_error ��fi_next�б�����
		fi_close((int)p_fi);
		return 0;
	}

	return (int)p_fi;
}

/* ����ʱ�����е����ݸ��Ƶ��û����ݹ����Ĳ�����,���ѵ�ǰ�ṹp_fi��
 * ��file����.
 *	Input:
 *		p_fi	->ϵͳ���ݽṹ
 *		p_fd	->�û���������
 *		p_file	->��ʱ������
 */
void fi_make_file(struct fi_data *p_fi, struct fi_data_t *p_fd, struct _finddata_t *p_file)
{
	int len;

	free(p_fi->name);
	p_fi->name        = _strdup(p_file->name);
	p_fd->attrib      = p_file->attrib;
	p_fd->time_create = p_file->time_create;
	p_fd->time_access = p_file->time_access;
	p_fd->time_write  = p_file->time_write;
	p_fd->size        = p_file->size;
	p_fd->dir         = p_fi->cur_dir;
	p_fd->name        = p_fi->name;

	//���һ����\��ɾ��
	if (len = strlen(p_fd->dir))
		if (p_fd->dir[len - 1] == '\\')
			p_fd->dir[len - 1] = 0;
}

/* ���뵽һ����Ŀ¼
 *	input:
 *		p_fi	->ϵͳʹ�õ����ݽṹ
 *	output:
 *		-1		->ʧ��
 *		0		->�ɹ�
 */
int fi_into_dir(struct fi_data *p_fi)
{
	if (p_fi->depth >= FI_MAX_DEPTH) //���������Ⱥ�, �������µ�Ŀ¼
		return -1;

	if (!_chdir(p_fi->next_dir))	 	//Ŀ¼����,���+1
	{
		char cwd[_MAX_PATH];
		p_fi->depth++;
		free(p_fi->cur_dir);
		p_fi->cur_dir = _strdup(_getcwd(cwd, sizeof(cwd)));
		free(p_fi->next_dir);
		p_fi->next_dir = 0;
		return 0;
	}
	//û�ܹ�����Ŀ¼, ����
	return -1;
}
	
/* search next file
 *	input:
 *		handle	-> fi handle which returned by fi_first
 *		p_ifi		-> fi_data_t struct point
 *	output:
 *		-1		-> error occurs, or end of search
 *		0		-> ok.
 */
int fi_next(int handle, struct fi_data_t *p_fd)
{
	char cwd[_MAX_PATH];
	struct fi_data *p_fi;
	struct _finddata_t c_file;

	p_fi = (struct fi_data *)handle;
	if (!(p_fi && p_fi->cur_dir && p_fd))
	{
		fi_error = FIE_HANDLE_ILLEGAL;
		return -1;
	}

	//�ڶ��ν���,��Ҫ�趨��ǰĿ¼
	if (p_fi->stage != FI_STAGE_START && -1 == _chdir(p_fi->cur_dir))
	{	//ϵͳĿ¼�޷�����,�϶���������.
		fi_error = FIE_CHDIR_FAILED;
		return -1;
	}

	while(1)
	{
		switch(p_fi->stage)
		{
		case FI_STAGE_START:		//���е�һ�εĳ�ʼ��
			if (-1 == _chdir(p_fi->cur_dir))
			{	//ϵͳĿ¼�޷�����,�϶���������.
				fi_error = FIE_CHDIR_FAILED;
				return -1;
			}
			free(p_fi->cur_dir);
			p_fi->cur_dir = _strdup(_getcwd(cwd, sizeof(cwd)));
			if (!p_fi->cur_dir)
			{
				fi_error = FIE_NOMEMORY;
				return -1;
			}
			p_fi->stage = FI_STAGE_SEEK_BEGIN;
			break;

		case FI_STAGE_ENTER:
			//����Ŀ¼
			if (!fi_into_dir(p_fi))
				p_fi->stage = FI_STAGE_SEEK_BEGIN;
			else
				p_fi->stage = FI_STAGE_SEEKING;
			break;

		case FI_STAGE_SEEK_BEGIN:
			//��Ŀ¼��û�н��й�Seek, findfirst��Ҫ������
			p_fi->fd_handle[p_fi->depth] = _findfirst(FI_ALLFILE, &c_file);
			if (-1 == p_fi->fd_handle[p_fi->depth])
			{
				fi_error = FIE_NOMEMORY;
				return -1;
			}
			p_fi->stage = FI_STAGE_JUDGE_FILE;
			break;

		case FI_STAGE_SEEKING:		//��ʼ���Ѿ���Ͽ��Խ�����һ�εļ���
			//û���ļ���,�˻�һ��Ŀ¼
			if (-1 == _findnext(p_fi->fd_handle[p_fi->depth], &c_file)) 
				p_fi->stage = FI_STAGE_SEEK_END;
			else
				p_fi->stage = FI_STAGE_JUDGE_FILE;
			break;

		case FI_STAGE_JUDGE_FILE:
			if(!strcmp(".", c_file.name) || !strcmp("..", c_file.name))
			{	//ignore . and ..
				p_fi->stage = FI_STAGE_SEEKING;
				break;
			}
			if (c_file.attrib & _A_SUBDIR)
			{	//�ҵ�һ����Ŀ¼, �趨��������
				free(p_fi->next_dir);
				p_fi->next_dir = _strdup(c_file.name);
				p_fi->stage = FI_STAGE_ENTER;
			}
			else 	//��ͨ�ļ����������
				p_fi->stage = FI_STAGE_SEEKING;

			fi_make_file(p_fi, p_fd, &c_file);
			return 0;

		case FI_STAGE_SEEK_END:
			//�˻�һ��Ŀ¼���ͷ�filefind�ڴ�
			_findclose(p_fi->fd_handle[p_fi->depth]);
			if (!p_fi->depth)		//ȫ���������.
			{
				fi_error = FIE_NOFILE;
				return -1;
			}
			if (-1 == _chdir(".."))
			{
				fi_error = FIE_CHDIR_FAILED;
				return -1;
			}
			free(p_fi->cur_dir);
			p_fi->cur_dir = _strdup(_getcwd(cwd, sizeof(cwd)));
			if (!p_fi->cur_dir)
			{
				p_fi->stage = FI_STAGE_END;
				fi_error = FIE_NOMEMORY;
				return -1;
			}
			p_fi->depth--;
			p_fi->stage = FI_STAGE_SEEKING;
			break;
		case FI_STAGE_END:
			return -1;
		}
	}
}

/* close file iterator, release memory.
 *	input:
 *		handle	-> file iterator handle
 *	output:
 *		none
 */
void fi_close(int handle)
{
	struct fi_data *pfi = (struct fi_data*) handle;
	free(pfi->name);
	free(pfi->cur_dir);
	free(pfi);
}

