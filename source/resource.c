/*	ϵͳ����Դ����
 *		�������뷨��װĿ¼���
 *
 *
 *		���뷨��װĿ¼һ���漰�ĸ�Ŀ¼��
 *		SYSTEM			-> windows\system32
 *		PROGRAM			-> program files\unispim6
 *		USERAPP			-> documents and setting\{user}\application data\unispim6
 *		ALLUSERAPP		-> documents and setting\all user\applicateion data\unispim6
 *
 *		���뷨��Դ��Ŀ¼�����Ŀ¼��š�
 *		�磺c:\program files\unispim6\wordlib\sys.uwl���Ϊ
 *		wordlib\sys.uwl
 *
 *		��Ŀ							Ŀ¼					˵��
 *		1.	����						ALLUSERAPP\theme\		ÿһ������һ��Ŀ¼
 *		2.	ϵͳ�ʿ�					ALLUSERAPP\wordlib\		ϵͳ�ʿ⣺sys.uwl
 *		3.	��Ҵʿ�					ALLUSERAPP\wordlib\		ÿһ���ʿ�һ���ļ�
 *		4.	�û��ʿ�					USERAPP\wordlib\		�û�����ʿ�ֻ��һ���ļ�
 *		5.	˫ƴ�����ļ�				USERAPP\ini\			˫ƴ.ini
 *		6.	����(����)�����ļ�			USERAPP\ini\			����.ini
 *		7.	�ö��ֶ����ļ�				USERAPP\ini\			�ö���.ini
 *		8.	BCOC�����ļ�				ALLUSERAPP\wordlib\		bcoc.dat
 *		9.	����Cache�����ļ�			USERAPP\wordlib\		zi_cache.dat
 *		10.	�ʻ�Cache�����ļ�			USERAPP\wordlib\		ci_cache.dat
 *		11.	��ִ�г���					PROGRAM\				���ó��򡢴ʿ�ά�������
 *
 *		����������ÿһ����Դ������Ӧ��Ŀ¼�н��м�����
 *		������
 *		5/6/7����INI�ļ������USERAPP�в����ڣ�����ALLUSERAPP�м�����
 *		���ǿ��ǵ��û����ܲ�û�б��ϵͳ������Ĭ�����á�
 *
 *		�磺
 *		main_bk.png����Ϊ��				theme\{themename}\main_bk.png
 *		��Ҵʿ⡰����.uwl������Ϊ��	wordlib\����.uwl
 *		˫ƴ�����ļ���					ini\˫ƴ.ini 
 */

#include <config.h>
#include <utility.h>
