/*	�������ͷ�ļ�
 */

#ifndef	_ICW_H_
#define	_ICW_H_

#include <kernel.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <windows.h>

#define	BIGRAM_FILE_NAME		TEXT("unispim6\\wordlib\\bigram.dat")

#define	ICW_MAX_ITEMS			1024					//ÿһ��ICW�������ѡ��Ŀ
#define	ICW_MAX_CI_ITEMS		256						//ÿ�����Ĵ���Ŀ
#define	ICW_MAX_PART_SYLLABLES	5						//���5����ȫ����

extern int GetIcwCandidates(SYLLABLE *syllable, int syllable_count, CANDIDATE *candidate);
extern int LoadBigramData(const TCHAR *name);
extern int FreeBigramData();
extern int MakeBigramFaster();

#ifdef __cplusplus
}
#endif

#endif