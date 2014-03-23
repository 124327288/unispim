/*	ϵͳ����Դ����
 *		����ƴ�����뷨��װĿ¼���
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

#include <zi.h>
#include <ci.h>
#include <icw.h>
#include <spw.h>
#include <config.h>
#include <utility.h>
#include <wordlib.h>
#include <symbol.h>
#include <english.h>
//#include <url.h>
#include <fontcheck.h>
#include <gbk_map.h>
#include <share_segment.h>

//#pragma	data_seg(HYPIM_SHARED_SEGMENT)
//int		resource_loaded = 0;				//��Դ��δװ��
//#pragma data_seg()

/**	װ��ȫ���ʿ��ļ���
 *	���У��û��ʿ���ϵͳ�ʿ�ΪĬ�ϼ��أ���Ҵʿ����config�е��趨���м���
 *	���أ�
 *		0��ʧ�ܣ�һ��Ϊϵͳ�ʿ�����û��ʿ����ô���
 *		1���ɹ��������ܴ���ĳЩ��Ҵʿ��������
 */
int LoadWordLibraryResource()
{
	return LoadAllWordLibraries();
}

/**	�ͷŴʿ����Դ
 */
int FreeWordLibraryResource()
{
	//�����û��ʿ��ļ�
	SaveWordLibrary(GetUserWordLibId());

	//�ر����дʿ��ļ�
	CloseAllWordLibrary();

	return 1;
}

/**	װ���û��Զ�������ļ�������.ini��
 *	������USERAPPĿ¼��Ѱ�ң����û������ALLUSERAPPĿ¼��Ѱ��
 */
int LoadSpwResource()
{
	if (!pim_config->use_special_word)
		return 0;

	return LoadAllSpwData();
}

int FreeSpwResource()
{
	return FreeSpwData();
}

//int WINAPI PIM_ReloadSPWResource()
//{
//	FreeSpwResource();
//	LoadSpwResource();
//
//	return 1;
//}

/**	װ���ö��ֶ����ļ�
 */
int LoadTopZiResource()
{
	TCHAR name[MAX_PATH];

	if (!(pim_config->hz_option & HZ_USE_FIX_TOP))
		return 0;

	GetFileFullName(TYPE_USERAPP, TOPZI_FILE_NAME, name);
	if (LoadTopZiData(name))
		return 1;

	GetFileFullName(TYPE_ALLAPP, TOPZI_FILE_NAME, name);
	return LoadTopZiData(name);
}

/**	�ͷ��ö�����Դ
 */
int FreeTopZiResource()
{
	return FreeTopZiData();
}

/**	װ�غ���Cache����
 */
int LoadZiCacheResource()
{
	TCHAR name[MAX_PATH];

	GetFileFullName(TYPE_USERAPP, ZICACHE_FILE_NAME, name);

	return LoadZiCacheData(name);
}

/*	�ͷź���Cache����
 */
int FreeZiCacheResource()
{
	TCHAR name[MAX_PATH];

	GetFileFullName(TYPE_USERAPP, ZICACHE_FILE_NAME, name);

	return FreeZiCacheData(name);
}

int LoadHZDataResource()
{
	TCHAR name[MAX_PATH];

	GetFileFullName(TYPE_USERAPP, HZDATA_FILE_NAME, name);
	if (LoadHZData(name))
		return 1;

	GetFileFullName(TYPE_ALLAPP, HZDATA_FILE_NAME, name);
	return LoadHZData(name);
}

int FreeHZDataResource()
{
	return FreeHZData();
}

/**	װ�غ���Cache����
 */
int SaveZiCacheResource()
{
	TCHAR name[MAX_PATH];

	GetFileFullName(TYPE_USERAPP, ZICACHE_FILE_NAME, name);

	return SaveZiCacheData(name);
}

/**	װ�ش�Cache����
 */
int LoadCiCacheResource()
{
	TCHAR name[MAX_PATH];

	GetFileFullName(TYPE_USERAPP, CICACHE_FILE_NAME, name);

	return LoadCiCacheData(name);
}

/**	�����Cache����
 */
int SaveCiCacheResource()
{
	TCHAR name[MAX_PATH];

	GetFileFullName(TYPE_USERAPP, CICACHE_FILE_NAME, name);

	return SaveCiCacheData(name);
}

/**	�ͷŴ�Cache����
 */
int FreeCiCacheResource()
{
	SaveCiCacheResource();
	return FreeCiCacheData();
}

/**	װ��bigram��Դ
 */
int LoadBigramResource()
{
	TCHAR name[MAX_PATH];

	GetFileFullName(TYPE_ALLAPP, BIGRAM_FILE_NAME, name);

	return LoadBigramData(name);
}

/**	�ͷ�bigram��Դ
 */
int FreeBigramResource()
{
	return FreeBigramData();
}

/*	����˫ƴ��Դ
 */
int LoadSPResource()
{
	TCHAR name[MAX_PATH];

	GetFileFullName(TYPE_USERAPP, SP_INI_FILE_NAME, name);
	if (LoadSPIniData(name))
		return 1;

	GetFileFullName(TYPE_ALLAPP, SP_INI_FILE_NAME, name);
	return LoadSPIniData(name);
}

int FreeSPResource()
{
	FreeSPIniData();
	return 1;
}

int FreeSymbolResource()
{
	FreeSymbolData();
	return 1;
}

/**	װ�����ķ���INI�ļ�
 */
int LoadSymbolResource()
{
	TCHAR name[MAX_PATH];

	GetFileFullName(TYPE_USERAPP, SYMBOL_INI_FILE_NAME, name);
	if (LoadSymbolData(name))
		return 1;

	GetFileFullName(TYPE_ALLAPP, SYMBOL_INI_FILE_NAME, name);
	return LoadSymbolData(name);
}

/**	װ��url data�ļ�
 */
/*int LoadURLResource()
{
	TCHAR name[MAX_PATH];

	if (!pim_config->use_url_hint)
		return 0;

	GetFileFullName(TYPE_USERAPP, URL_FILE_NAME, name);
	if(LoadURLData(name))
		return 1;

	GetFileFullName(TYPE_ALLAPP, URL_FILE_NAME, name);
	return LoadURLData(name);
}

int FreeURLResource()
{
	FreeURLData();
	return 1;
}*/

/**	װ��font map data�ļ�
 */
int LoadFontMapResource()
{
	TCHAR name[MAX_PATH];

	GetFileFullName(TYPE_USERAPP, FONTMAP_FILE_NAME, name);

	return LoadFontMapData(name);
}

int FreeFontMapResource()
{
	FreeFontMapData();
	return 1;
}

/**	װ��gbk map data�ļ�
 */
int GenGBKMapResource()
{
	return GenGBKMapData();
}

int FreeGBKMapResource()
{
	FreeGBKMapData();
	return 1;
}

/**	װ��Ӣ�Ĵʵ��ļ�
 */
int LoadEnglishResource()
{
	TCHAR name[MAX_PATH];

	if (!pim_config->use_english_input)
		return 0;

	GetFileFullName(TYPE_ALLAPP, ENGLISH_LIB_FILE_NAME, name);

	return LoadEnglishData(name);
}

int FreeEnglishResource()
{
	FreeEnglishData();

	return 1;
}

int PIM_ReloadEnglishResource()
{
	FreeEnglishResource();
	LoadEnglishResource();

	return 1;
}

//װ��Ӣ�ķ��������ļ�
int LoadEnglishTransResource()
{
	TCHAR name[MAX_PATH];

	if (!pim_config->use_english_input || !pim_config->use_english_translate)
		return 0;

	GetFileFullName(TYPE_ALLAPP, ENGLISH_TRANS_FILE_NAME, name);

	return LoadEnglishTransData(name);
}

int FreeEnglishTransResource()
{
	FreeEnglishTransData();

	return 1;
}

int PIM_ReloadEnglishTransResource()
{
	FreeEnglishTransResource();
	LoadEnglishTransResource();

	return 1;
}

int PIM_ReloadEnglishAllResource()
{
	FreeEnglishResource();
	LoadEnglishResource();

	FreeEnglishTransResource();
	LoadEnglishTransResource();

	return 1;
}

/** װ�رʻ������ļ�
*/
int LoadBHResource()
{
	TCHAR name[MAX_PATH];

	GetFileFullName(TYPE_USERAPP, HZBH_FILE_NAME, name);
	if(LoadBHData(name))
		return 1;

	GetFileFullName(TYPE_ALLAPP, HZBH_FILE_NAME, name);
	return LoadBHData(name);
}

int FreeBHResource()
{
	FreeBHData();
	return 1;
}

int WINAPI PIM_ReloadBHResource()
{
	FreeBHResource();
	LoadBHResource();
	return 1;
}

/**	�ͷ����뷨��Դ
 */
int PIM_FreeResources()
{
	if (!share_segment->resource_loaded)
		return 1;

	share_segment->resource_loaded = 0;

	FreeWordLibraryResource();
	FreeSpwResource();
	FreeTopZiResource();
	FreeZiCacheResource();
	FreeCiCacheResource();
	FreeBigramResource();
	FreeSymbolResource();
	//FreeNewWordTable();
	FreeEnglishResource();
	FreeEnglishTransResource();
//	FreeURLResource();
	FreeBHResource();

	return 1;
}

/**	���浱ǰ��������Դ
 */
int PIM_SaveResources()
{
	SaveWordLibrary(GetUserWordLibId());
	SaveZiCacheResource();
	SaveCiCacheResource();
	//SaveNewWordTable();

	return 1;
}

/**	װ�����뷨ʹ�õ���Դ�����ʧ�ܣ������뷨��������
 *	���أ�
 *		�ɹ���1��ʧ�ܣ�0
 */
int PIM_LoadResources()
{
	LoadWordLibraryResource();			//�ʿ�
	LoadBigramResource();				//Bigram����

	if (share_segment->resource_loaded)
		return 1;

	share_segment->resource_loaded = 1;

	LoadSpwResource();					//����
	LoadTopZiResource();				//�ö���
	LoadZiCacheResource();				//����Cache
	LoadHZDataResource();				//װ�غ�������
	LoadCiCacheResource();				//��Cache
	//LoadNewWordTable();					//�´ʱ�
	LoadSPResource();					//װ��˫ƴ��Դ
	LoadSymbolResource();				//װ�����ķ�����Դ
	LoadFontMapResource();              //load font map
	GenGBKMapResource();                //load gbk map
//	LoadEnglishResource();				//װ��Ӣ�Ĵʵ�����
//	LoadEnglishTransResource();			//װ��Ӣ�ķ�������
//	LoadURLResource();				    //װ��URL����
//	LoadBHResource();					//װ�رʻ������ļ�

	return 1;
}

int PIM_ReloadResources()
{
	PIM_FreeResources();
	PIM_LoadResources();

	return 1;
}

/**	����װ��INI������ݣ��������ö��֡�˫ƴ�����š�����
 */
int PIM_ReloadINIResource()
{
	FreeSpwResource();
	FreeTopZiResource();
	FreeSymbolResource();
	FreeSPResource();

	LoadSpwResource();		//���ﶨ��
	LoadSPResource();		//˫ƴ��Դ
	LoadTopZiResource();	//�ö���
	LoadSymbolResource();	//���ķ���

	return 1;
}

/**	Reload Wordlib resource
 */
int PIM_ReloadWordlibResource()
{
	LoadWordLibraryResource();			//�ʿ�

	return 1;
}

int PIM_ForceReloadWordlibResource()
{
	CloseAllWordLibrary();
	LoadWordLibraryResource();			//�ʿ�

	return 1;
}

/**	Reload Zi resource
 */
int WINAPI PIM_ReloadZiResource()
{
	FreeHZDataResource();
	LoadHZDataResource();

	return 1;
}

/*int WINAPI PIM_ReloadURLResource()
{
	FreeURLResource();
	LoadURLResource();

	return 1;
}*/

int WINAPI PIM_ReloadFontMapResource()
{
	FreeFontMapResource();
	LoadFontMapResource();

	return 1;
}
