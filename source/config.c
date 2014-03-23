/*	���뷨���ó���
 *	���ó����ע�����װ��������Ϣ����ע����в�����������Ϣʱ������
 *	Ĭ�ϵ����õ�ע���
 */
#include <kernel.h>
#include <config.h>
#include <context.h>
#include <utility.h>
#include <pim_resource.h>
#include <windows.h>
#include <regstr.h>
#include <shlwapi.h>
#include <tchar.h>
#include <fontcheck.h>
#include <share_segment.h>
#include <libfunc.h>

//��������ĺ�����
typedef int (_stdcall *pMAKE_THEME) (PIMCONFIG *);
typedef int (_stdcall *pBACKUP) (const TCHAR *);

PIMCONFIG *pim_config;					//ȫ��ʹ�õ�configָ��

#pragma data_seg(HYPIM_SHARED_SEGMENT)

//int	is_default_ime = 0;					//�Ƿ�ΪĬ�����뷨
//int global_config_update_time = 0;		//����װ��ʱ��
//int config_loaded = 0;					//�Ƿ��Ѿ�װ��Config
//
//PIMCONFIG all_config = { 0 };			//ϵͳ����ʹ�õ�Config
PIMCONFIG default_config =				//Ĭ�ϵ����뷨��������
{
	//������
	STYLE_CSTAR,
//	STYLE_ABC,

	//���������뷽ʽ�����ġ�Ӣ��
	STARTUP_CHINESE,
//	STARTUP_ENGLISH,

	//ƴ��ģʽ
	PINYIN_QUANPIN,

	//�Ƿ���ʾ˫ƴ����ʾ
	0,

	//��ѡѡ��ʽ����ĸ�����֡�
	SELECTOR_DIGITAL,
//	SELECTOR_LETTER,

	//�������������
	0,

	//֧��IME��֪����
	1,

	//��ֱ��ʾ��ѡ
	0,

	//������ʾд������
	0,

	//������ʾ��ѡ����
	0,

	//�Ƿ���ʾ״̬��
	1,

	//�Ƿ�ʹ�ú�����������
	1,

	//�Ƿ�ʹ��ICW��������ʣ�
	1,

	//�Ƿ񱣴�ICW�Ľ�����ʿ�
	1,

	//�Ƿ������
	1,

	//�Ƿ�ֱ��������ַ
	1,

	//����ĸ�������С������Ŀ
	4,

	//˫�����ƴ����
	1,

	//������V����Ӣ�ķ�ʽ��������
	0,			//������V��������ո�

	//�Ƿ������ּ�֮������Ӣ�ķ���
	0,

	//ʹ��TAB��չ���ֵĺ�ѡ
	1,

	//һֱչ����ѡ����
	0,

	//��������ķ�ʽ�����塢���塣ͬʱ�����ְ���δ���຺�֡�
	HZ_OUTPUT_SIMPLIFIED,
//	HZ_OUTPUT_HANZI_ALL,
//	HZ_TRADITIONAL_EQU_OTHERS,		//�����ְ�������Ϊ�������֣�����/���ĺ��֣�

	//�����ֵ�ѡ��
	HZ_RECENT_FIRST			|		//�������������ȣ�Ĭ�ϣ�
	HZ_ADJUST_FREQ_FAST		|		//�����������Ƶ��Ĭ�ϣ�
	HZ_USE_TAB_SWITCH_SET	|		//ʹ��TAB�л����ֵļ���
	HZ_USE_TWO_LEVEL_SET	|		//ʹ�����ּ����зַ�ʽ
	HZ_SYMBOL_CHINESE		|		//ʹ�ú��ַ���
	HZ_SYMBOL_HALFSHAPE		|		//��Ƿ���
	HZ_USE_FIX_TOP,					//ʹ�ù̶���

	//������ѡ��
	CI_AUTO_FUZZY			|		//����ʵ�ʱ���Զ�ʹ��z/zh, c/ch, s/sh��ģ����Ĭ�ϣ�
	CI_SORT_CANDIDATES		|		//��ѡ�ʻ��ڴ�Ƶ��������Ĭ�ϣ�
	CI_ADJUST_FREQ_FAST		|		//���ٵ�����Ƶ��Ĭ�ϣ�
	CI_WILDCARD				|		//����ʵ�ʱ��ʹ��ͨ�����Ĭ�ϣ�
	CI_USE_FIRST_LETTER		|		//ʹ������ĸ����ʣ�Ĭ�ϣ�
	CI_RECENT_FIRST			|		//��������Ĵ����ȣ�Ĭ�ϣ�
	CI_AUTO_VOW_FUZZY,				//ʹ����ĸ�Զ�ƥ��

	//�Ƿ�ʹ��ģ����
	0,

	//ģ����ѡ��
	0,								//ȫ����ģ��

	//�����Ĭ�����
	0,

	//�ʿ��ļ�����
	{
		TEXT("unispim6\\wordlib\\user.uwl"),
		TEXT("unispim6\\wordlib\\sys.uwl"),
		TEXT("unispim6\\wordlib\\names.uwl"),
		TEXT("unispim6\\wordlib\\addr.uwl"),
		TEXT("unispim6\\wordlib\\life.uwl"),
		TEXT("unispim6\\wordlib\\rec.uwl"),
		TEXT("unispim6\\wordlib\\idiom.uwl"),
#ifdef	_VER_SIFA_
		TEXT("unispim6\\wordlib\\sifa.uwl"),
#endif
	},

	//�ʿ�����
#ifndef	_VER_SIFA_
#pragma message("_VER_SIFA_ not defined")
	7,
#else
#pragma message("_VER_SIFA_ defined")
	8,
#endif

	//һ���ܹ�������������Ŀ
	20,

	//��ʾ�ں�ѡҳ�еĺ�ѡ�ĸ���
	8,

	//��ѡ��ҳ����
	KEY_PAIR_1 |					//,.
	KEY_PAIR_3 |					//-=
	KEY_PAIR_6,						//PageUp/PageDown

	//��ѡ���μ���
	KEY_PAIR_5,						//<>

	//�Դʶ��ּ���
	KEY_PAIR_2,						//[]

	//�༭����
	KEY_PAIR_4,						//()

	//�任��Ӣ�����뷽ʽ��
	KEY_SWITCH_SHIFT,				//��SHIFT

	//����ڶ����Լ���������ѡ�ļ�
	KEY_2ND_3RD_CONTROL,			//����CONTROL

	//������֮��"."��ΪӢ�ķ���
	1,

	//��չ��ѡ������
	4,

	//���û�ʹ�ù��Ĵʻ��¼���û��ʿ��У�Ĭ��Ϊ0
	0,

	//��������С�߶ȡ����
	0,
	0,

	0,
	0,

	//��������״̬���ڵ�͸����
	0,
	0,

	//�����ڵ�ê�㣨���Ͻ�λ�ã��������趨������������λ��
	0,
	0,

	0,
	0,

	//״̬�����ϱ߽�
	-1,					//-1��ʾ����

	//����Ӧ�õķ�Χ
	1,
	1,
	1,

	//����Ŀ¼
	TEXT(""),

	//��ֹ��ϼ���������Ϸ
	0,

	//���ð汾
	CONFIG_VERSION,					//���ð汾

	//�������������Ĵ�
	0,

	//�������������ģʽ�л�
	1,

	//��������к���ȫ��ģʽ
	1,

	//ƴ������ѡ��
	0,
	//CORRECT_GN_NG          |
	//CORRECT_MG_NG          |
	////CORRECT_ON_ONG         |
	////CORRECT_IOU_IU         |
	//CORRECT_UEI_UI,         //|
	////CORRECT_UEN_UN,

	//ʹ���Զ������
	1,

	//ʹ���Զ������
	1,

	//ʹ������̿�ݼ�
	1,

	//����̿�ݼ�
	'K',

	//ʹ�ü��л���ݼ�
	1,

	//���л���ݼ�
	'F',

	//ʹ��ȫƴ˫ƴ��ݼ�
	0,

	//ȫƴ˫ƴ�л���ݼ�
	'S',

	//ʹ��״̬���л���ݼ�
	1,

	//״̬���л���ݼ�
	'Z',

	//��ѡ����Ӣ�ĵ���
	0,

	//Ӣ�ĵ��ʺ��Զ����ո�
	1,

	//ʹ��Ӣ�������ݼ�
	1,

	//Ӣ�������ݼ�
	'E',

	//Ӣ������ʱ������ʾ
	1,

	//����ʱ����Ӣ�����뷨
	0,

	//��ʾ��ʾ��Ϣ
	1,

	//������ʾ�ұ߽�
	0,

	//���Ź�����ʾ�ұ߽�
	0,

	//�����ļ�����
	{
		SYS_SPW_FILE_NAME,
		/*TEXT("unispim6\\phrase\\�������.ini"),
		TEXT("unispim6\\phrase\\���ַ���.ini"),
		TEXT("unispim6\\phrase\\ͼ�η���.ini"),
		TEXT("unispim6\\phrase\\�����ַ�.ini"),
		TEXT("unispim6\\phrase\\���ķ���.ini"),
		TEXT("unispim6\\phrase\\�ַ���.ini"),
		TEXT("unispim6\\phrase\\�û�����.ini")*/
	},

	//�����ļ�����
	1,

	//ʹ��Ӣ������
	1,

	//ʹ��Ӣ�ķ���
	1,

	//use url hint
	//1,
	
	//���������ѡʱ����ʾλ�ã�ʹ����������������Ĭ��Ϊ3
	//spw_position
	3,

	//ʹ�ô�������
	1,

	//�ӵ�x�����ڿ�ʼ����
	4,

	//�����λ�ں�ѡ�ʵ�xλ
	2,

	//����ʸ���
	2,

	//use_u_hint
	1,

	//ֻ���GBK���ϣ�scope_gbk
	HZ_SCOPE_GBK,

	//���������촰���ܣ�Ĭ�ϲ�����
	0,

	//ȫ��Ӧ��ʱ����״̬��
	1,

	//Bģʽ����
	1,

	//Dģʽ����
	1,

	//Iģʽ����
	1,

	//Hģʽ����
	1,

	//iģʽ����
	1,

	//uģʽ����
	1,

	//vģʽ����
	1,

	//С��������ѡ��
	0,

	//����������ѡ����ڰ汾�������������¸���ע���
	{ 0, },

	//״̬�������ͣ��°棬��ͳ
	STATUS_TRADITIONAL_STYLE,		//�°汾

	//��ͳ״̬���ϵİ���
	STATUS_BUTTON_MODE		|
	STATUS_BUTTON_CHARSET	|
	STATUS_BUTTON_SYMBOL	|
	STATUS_BUTTON_SHAPE		|
	STATUS_BUTTON_SOFTKBD	|
	STATUS_BUTTON_HELP		|
	STATUS_BUTTON_SETUP,

	//�������ƣ����ڽ��������Ѱ���ж�
	DEFAULT_THEME_NAME,

	//������������
	TEXT("����"),

	//Ӣ���Լ����ŵ���������
	TEXT("Arial"),

	//����߶�
	16,

	//�����ڱ���ͼ�����ƣ����ΪNULL����ʹ��Ĭ����ɫ���л滭
	MAIN_BK_IMAGE_NAME,

	//������ָ���ͼ������
	MAIN_LINE_IMAGE_NAME,

	//�����ڱ���ͼ�����ƣ����ΪNULL����ʹ��Ĭ����ɫ���л滭
	MAIN_BK_IMAGE_NAME,

	//������ָ���ͼ������
	MAIN_LINE_IMAGE_NAME,

	//����ͼ������ò���
	3,							//����ͼ�����߽磬Ĭ��2
	26, 						//����ͼ����ұ߽磬Ĭ��2
	25,							//����ͼ����ϱ߽磬Ĭ��2
	3,							//����ͼ����±߽磬Ĭ��2

	3,							//����ͼ�����߽磬Ĭ��2
	26, 						//����ͼ����ұ߽磬Ĭ��2
	25,							//����ͼ����ϱ߽磬Ĭ��2
	3,							//����ͼ����±߽磬Ĭ��2

	6,							//������߽�
	6,							//�����ұ߽�
	6,							//�����ϱ߽�
	6,							//�����±߽�
	6,							//���ĸ߶ȣ�����ƴ�������ѡ��֮��ĸ߶�
	16,							//��ѡʣ����Ŀ�ұ߽�

	6,							//������߽�
	6,							//�����ұ߽�
	6,							//�����ϱ߽�
	6,							//�����±߽�
	6,							//���ĸ߶ȣ�����ƴ�������ѡ��֮��ĸ߶�
	16,							//��ѡʣ����Ŀ�ұ߽�

	6,							//��������߽�
	6,							//�������ұ߽�

	6,							//��������߽�
	6,							//�������ұ߽�

	4,							//����Բ�ǿ��
	4,							//����Բ�Ǹ߶�

	4,							//����Բ�ǿ��
	4,							//����Բ�Ǹ߶�

	DRAW_MODE_STRETCH, 			//�������Ļ���ģʽ��Ĭ�ϣ�����
	DRAW_MODE_STRETCH, 			//��ֱ����ģʽ��Ĭ�ϣ�����
	DRAW_MODE_STRETCH, 			//ˮƽ����ģʽ��Ĭ�ϣ�����
	DRAW_MODE_STRETCH,			//�����߻���ģʽ��Ĭ�ϣ�����

	DRAW_MODE_STRETCH, 			//�������Ļ���ģʽ��Ĭ�ϣ�����
	DRAW_MODE_STRETCH, 			//��ֱ����ģʽ��Ĭ�ϣ�����
	DRAW_MODE_STRETCH, 			//ˮƽ����ģʽ��Ĭ�ϣ�����
	DRAW_MODE_STRETCH,			//�����߻���ģʽ��Ĭ�ϣ�����

	0,							//tab��չʱʹ�����ű�����Ĭ��Ϊ��

	0,							//״̬����ͼ����߽�
	0,							//״̬����ͼ���ұ߽�
	DRAW_MODE_STRETCH,			//״̬����ͼ�����Ļ���ģʽ
	8,							//״̬����ͼ�����հ׿�ȣ���ť��ʼ�㣩
	4,							//״̬����ͼ���Ҳ�հ׿��
	0,							//״̬����С���

	//������Ч��--�˹����Ѿ�ȥ��������ʹ��
	0,							//�Ƿ���ʾ��Ӱ��Ĭ�ϣ���ʾ

	//״̬����Ч��--�˹����Ѿ�ȥ��������ʹ��
	0,							//�Ƿ���ʾ��Ӱ��Ĭ�ϣ���ʾ

	//������д�������Լ���ѡ���ڵ��ַ�����ɫ
	TORGB(0x0080FF),

	//�����ڹ����ɫ
	TORGB(0xFF8000),

	//�����ں�ѡǰ���������ɫ
	TORGB(0x0094EC),

	//�����ڵ�ǰѡ�еĺ�ѡ����ɫ
	TORGB(0xFF0000),

	//�����ں�ѡ��ɫ
	TORGB(0x0080FF),

	//������ICW��ɫ
	TORGB(0x008040),

	//������ʣ���ѡ��Ŀ��ɫ
	TORGB(0xFF0000),

	//˫ƴ��ʾ����ɫ
	TORGB(0xa0a0a0),

	//˫ƴ��ʾ�򱳾���ɫ
	TORGB(0xf4f4f8),

	//˫ƴ�����ַ�������ɫ
	TORGB(0x000000),

	//״̬���ڱ���ͼ������
	STATUS_BK_IMAGE_NAME,

	//״̬���ڰ�ťͼ������
	STATUS_BUTTONS_IMAGE_NAME,

	//״̬����λ�ã�Ĭ��-1, -1
	-1,	-1,

	//������λ�ã�Ĭ��500, 500
	500, 1800,
};

#pragma data_seg()

int no_main_show = 0;
int no_status_show = 0;
int	no_transparent = 0;
int no_ime_aware = 0;
int no_gdiplus_release = 0;
int no_multi_line = 0;
int no_virtual_key = 0;
int host_is_console = 0;
int no_end_composition = 0;
int no_ppjz = 0;//����ƫ�Բ��׼���

int program_id;

/**	����ض���������õ�Ҫ��
 */
void CheckSpecialConfig()
{
	const TCHAR *p;
	int i;

	static const TCHAR p_name[][0x20] =
	{
		TEXT(""),
		TEXT("opera.exe"),
		TEXT("maxthon.exe"),
		TEXT("theworld.exe"),
		//TEXT("wow.exe"),
		TEXT("maplestory.exe"),
		TEXT("war3.exe"),
		TEXT("conime.exe"),
		TEXT("photoshop.exe"),
		TEXT("imageready.exe"),
		TEXT("mir1.dat"),
		TEXT("iexplore.exe"),
		TEXT("firefox.exe"),
		TEXT("windowsLiveWriter.exe"),
		TEXT("onenote.exe"),
		TEXT("acrobat.exe"),
		TEXT("msnmsgr.exe"),
		TEXT("visio.exe"),
		TEXT("tm.exe"),
		//TEXT("qq.exe"),
		TEXT("foxmail.exe"),
		TEXT("outlook.exe"),
		TEXT("msimn.exe"),		//outlook express
		TEXT("TTraveler.exe")/*,
		TEXT("POWERPNT.EXE")*/
	};

	static const int p_id[] =
	{
		PID_UNKNOWN,
		PID_OPERA,
		PID_MAXTHON,
		PID_WORLD,
		//PID_WOW,
		PID_MXD,
		PID_WAR3,
		PID_CONIME,
		PID_PHOTOSHOP,
		PID_IMAGEREADY,
		PID_CQ,
		PID_IE,
		PID_FIREFOX,
		PID_WRITER,
		PID_ONENOTE,
		PID_ACROBAT,
		PID_MSNMESSENGER,
		PID_VISO,
		PID_TM,
		//PID_QQ,
		PID_FOXMAIL,
		PID_OUTLOOK,
		PID_OUTLOOKEXPRESS,
		PID_TTRAVELER/*,
		PID_POWERPOINT*/
	};

	program_id = PID_UNKNOWN;
	p = GetProgramName();
	if (!p)
		return;

	for (i = 0; i < sizeof(p_name) / _SizeOf(p_name[0]) / sizeof(TCHAR); i++)
		if (!_tcsicmp(p_name[i], p))
		{
			program_id = p_id[i];
			break;
		}

	if (program_id == PID_UNKNOWN)
		return;

	switch(program_id)
	{
	//Opera�����
	case PID_OPERA:
		no_ppjz = 1;
		//no_ime_aware = 1;
		break;

	//maxthon�����
	case PID_MAXTHON:
		no_transparent = 1;
		break;

	//����֮�������
	case PID_WORLD:
		no_transparent = 1;
		no_ime_aware = 1;
		break;

	//ħ������
	//case PID_WOW:
	//	//no_main_show = 1;
	//	break;

	//ð�յ�
	case PID_MXD:
		no_status_show = 1;
		break;

	//ħ������
	case PID_WAR3:
		no_status_show = 1;
		//no_main_show = 1;
		break;

	//Conime
	case PID_CONIME:
		no_status_show = 1;
		host_is_console = 1;
		break;

	//PhotoShop
	case PID_PHOTOSHOP:
		no_multi_line = 1;
		break;

	//ImageReady
	case PID_IMAGEREADY:
		no_multi_line = 1;
		break;

	//��������
	case PID_CQ:
		no_gdiplus_release = 1;
		break;

	//��������
	case PID_FIREFOX:
		no_transparent = 1;
		no_ime_aware = 1;
		break;

	//IE
	case PID_IE:
		no_transparent = 1;
		no_gdiplus_release = 1;
		no_ime_aware = 1;
		break;

	//onenote
	case PID_ONENOTE:
		no_virtual_key = 1;
		break;

	//acrobat
	case PID_ACROBAT:
		no_end_composition = 1;
		break;

	//msn messenger
	case PID_MSNMESSENGER:
		no_ppjz = 1;
		break;
	
	case PID_VISO://viso
	case PID_TM://QQ
	//case PID_QQ://QQ
	case PID_FOXMAIL://FOXMAIL
	case PID_OUTLOOK://OUTLOOK
	case PID_OUTLOOKEXPRESS://OUTLOOK
	case PID_TTRAVELER://QQ BROWSER
//	case PID_POWERPOINT:
		no_ime_aware = 1;
		break;
	}
}

/*	������д�뵽ע����С�
 *	�ڲ����������ڱ�ģ��ĵ��ã������бȽ��Լ�Resource������װ��
 */
void SaveConfigInternal(PIMCONFIG *config)
{
	HKEY		reg_key;		//ע���KEY
	LONG		ret;			//��������ֵ
	HKEY		cur_user_key;

	if (!config)
		return;

	Log(LOG_ID, L"������");
	if (ERROR_SUCCESS != RegOpenCurrentUser(KEY_ALL_ACCESS, &cur_user_key))
	{
		Log(LOG_ID, L"!!! �޷���õ�ǰ�û���key, error=%d", GetLastError());
		cur_user_key = HKEY_CURRENT_USER;
	}

	Log(LOG_ID, L"save theme name:%s", config->theme_name);
	Log(LOG_ID, L"save main image:%s", config->main_image_name);

	ret = RegOpenKeyEx(cur_user_key, REG_CONFIG_KEY_NAME, 0, KEY_READ | KEY_WRITE, &reg_key);
	if (ret != ERROR_SUCCESS)
	{
		ret = RegCreateKey(cur_user_key, REG_CONFIG_KEY_NAME, &reg_key);
		if (ret != ERROR_SUCCESS)				//ʧ��
		{
			Log(LOG_ID, L"����ע���Key����ʧ�ܡ�key��=%s, errcode = %d", REG_CONFIG_KEY_NAME, ret);
			return;
		}
	}

	//��Ĭ����������д�뵽ע�����
	ret = RegSetValueEx(reg_key, REG_CONFIG_ITEM_NAME, 0, REG_BINARY, (char*)config, sizeof(PIMCONFIG));
	if (ret != ERROR_SUCCESS)				//ʧ��
		Log(LOG_ID, L"ע�����д��ʧ�ܣ�����=%s, errcode = %d", REG_CONFIG_ITEM_NAME, ret);
	else
		Log(LOG_ID, L"ע�����д��ɹ�");

	RegCloseKey(reg_key);			//�ر�ע���Key

	//����config����Ҫ���и��µĲ�����
	if (pim_config && pim_config != config)
		memcpy(pim_config, config, sizeof(PIMCONFIG));

	Log(LOG_ID, L"save theme name:%s", pim_config->theme_name);
	Log(LOG_ID, L"save main image:%s", pim_config->main_image_name);
}

void LoadThemeConfig(PIMCONFIG *config)
{
	MakeTheme(config);

	//HMODULE dll;
	//pMAKE_THEME make_theme;
	//int ret;

	//dll = LoadLibrary(UTILITY_DLL_NAME);
	//if (!dll)
	//	return;

	//Log(LOG_ID, L"װ��DLL");
	//make_theme = (pMAKE_THEME) GetProcAddress(dll, "MakeTheme");
	//if (make_theme)
	//{
	//	ret = (*make_theme)(config);
	//	if (!ret)							//װ��ʧ��
	//	{
	//		_tcscpy_s(config->theme_name, _SizeOf(config->theme_name), DEFAULT_THEME_NAME);
	//		ret = (*make_theme)(config);
	//	}
	//}
	//Log(LOG_ID, L"����MakeTheme");

	//FreeLibrary(dll);
	//Log(LOG_ID, L"ж��DLL");

	//Log(LOG_ID, L"theme name:%s", config->theme_name);
	//Log(LOG_ID, L"main image:%s", config->main_image_name);
}

/*	������д�뵽ע����С�
 *	�ⲿDLL�ӿں���������Config�Ƚ��Լ�Resource������װ��
 */
void WINAPI SaveConfig(PIMCONFIG *config, int modify_flag)
{
	int diff,old_hideblackwindow;
	int expand_candidates_save = pim_config->always_expand_candidates;
	old_hideblackwindow = pim_config->hide_black_window;

	Log(LOG_ID, L"������");

	//��ȫ��ͬ������Ҫ����
	diff = memcmp(pim_config, config, sizeof(PIMCONFIG));
	if (!modify_flag && !diff)
		return;

	if (pim_config && diff)
		SaveConfigInternal(config);

	//�ͷ����뷨��Դ���Լ����¼������뷨��Դ
	if (modify_flag & MODIFY_INI)
		PIM_ReloadINIResource();

	if ((expand_candidates_save != pim_config->always_expand_candidates) ||
		(modify_flag & MODIFY_THEME) ||		//ֻ���������Ҫupdate
		(modify_flag & MODIFY_WORDLIB))
		share_segment->global_config_update_time = GetCurrentTicks();

	if (modify_flag & MODIFY_WORDLIB)
		PIM_ReloadWordlibResource();
	else if (modify_flag & MODIFY_WORDLIB_FORCE_RELOAD)
		PIM_ForceReloadWordlibResource();

	if (modify_flag & MODIFY_ZI)
		PIM_ReloadZiResource();

	if (modify_flag & MODIFY_ENGLISH)
		PIM_ReloadEnglishAllResource();

	/*if (modify_flag & MODIFY_URL)
		PIM_ReloadURLResource();*/

	if ( config->hide_black_window == 1 && old_hideblackwindow == 0 )
		PIM_ReloadFontMapResource();

	Log(LOG_ID, L"�������ý��������ʱ��:%d\n", GetCurrentTicks());
}

/**	װ��Ĭ������
 */
const PIMCONFIG * WINAPI LoadDefaultConfig()
{
	LoadThemeConfig(&default_config);			//װ����������
	return &default_config;
}

/**	�ж��Ƿ�ΪĬ�����뷨
 */
void CheckDefaultIME()
{	HKEY		reg_key; 		//ע���KEY
	DWORD		data_type;		//ע����������
	DWORD		data_length;	//�������ݳ���
	LONG		ret;			//��������ֵ
	HKEY		cur_user_key;	//��ǰ�û�key
	TCHAR		ime_id[0x20];	//IME�ı�ʶ
	TCHAR		ime_name[0x100];
	TCHAR		key_str[0x100];

	//����Ѿ����й��жϣ�����Ҫ���½��С�
	if (share_segment->is_default_ime)
		return;

	Log(LOG_ID, L"����Ƿ�ΪĬ�����뷨");
	if (ERROR_SUCCESS != RegOpenCurrentUser(KEY_ALL_ACCESS, &cur_user_key))
	{
		Log(LOG_ID, L"!!! �޷���õ�ǰ�û���key, error=%d", GetLastError());
		cur_user_key = HKEY_CURRENT_USER;
	}

	//��ע���KEY
	ret = RegOpenKey(cur_user_key, REG_PRELOAD_KEY_NAME, &reg_key);
	if (ret != ERROR_SUCCESS)
	{	//ʧ��
		Log(LOG_ID, L"Ĭ�����뷨Key��ʧ�ܣ����ܲ����ڡ�errcode = %d", ret);
		return;
	}

	//��ȡ������Ϣ��
	data_length = _SizeOf(ime_id);
	ret = RegQueryValueEx(reg_key, TEXT("1"), 0, &data_type, (LPBYTE)ime_id, &data_length);

	RegCloseKey(reg_key);			//�ر�ע���Key
	if (ret != ERROR_SUCCESS || data_type != REG_SZ)	//��ȡʧ��
	{
		Log(LOG_ID, L"Ĭ�����뷨���ȡʧ��");
		return;
	}

	//����Ƿ�Ϊ�����Լ������뷨
	_stprintf_s(key_str, _SizeOf(key_str), REGSTR_PATH_CURRENT_CONTROL_SET TEXT("\\Keyboard Layouts\\%s"), ime_id);
	if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, key_str, 0, KEY_READ, &reg_key))
		return;			//�޷���ʧ��

	data_length = _SizeOf(ime_name) - 1;
	ret = RegQueryValueEx(reg_key, TEXT("Ime File"), 0, &data_type, (LPBYTE)ime_name, &data_length);
	RegCloseKey(reg_key);

	if (ret != ERROR_SUCCESS || _tcscmp(ime_name, IME_NAME))
	{
		Log(LOG_ID, L"�����뷨��Ĭ�����뷨");
		return;
	}

	share_segment->is_default_ime = 1;
	Log(LOG_ID, L"�����뷨ΪĬ�����뷨");
}

/**	���Ƥ����Ŀ¼�Ƿ����
 */
int ExistThemeDirectory(const TCHAR *theme_name)
{
	TCHAR name[MAX_PATH] = {0};
	TCHAR t_name[MAX_PATH] = {0};

	_tcscpy_s(t_name, _SizeOf(t_name), THEME_DIR_NAME);
	_tcscat_s(t_name, _SizeOf(t_name), theme_name);

	GetFileFullName(TYPE_ALLAPP, t_name, name);
	if (PathIsDirectory(name))
		return 1;

	return 0;
}

/**	���Ƥ�����ļ��Ƿ����
 */
int ExistThemeFile(const TCHAR *file_name)
{
	TCHAR name[MAX_PATH] = {0};
	TCHAR t_name[MAX_PATH] = {0};

	_tcscat_s(t_name, _SizeOf(t_name), file_name);

	GetFileFullName(TYPE_ALLAPP, t_name, name);
	if (PathFileExists(name))
		return 1;

	return 0;
}

/*	��ע����ж������뷨������Ϣ��
 *	ע�����Ϊһ���ڴ����ݣ����СΪPIMCONFIG��SIZE��
 */
void WINAPI LoadConfig(PIMCONFIG *config)
{
	HKEY		reg_key; 		//ע���KEY
	DWORD		data_type;		//ע����������
	DWORD		data_length;	//�������ݳ���
	LONG		ret;			//��������ֵ
	HKEY		cur_user_key;
	int			theme_exists = 0;

	Log(LOG_ID, L"�����û�Reg����");
	//����XPҪ�����ʹ��RegOpenCurrentUser��������ȡHKCU������
	//���ܷ��صĽ�����ǵ�ǰ�û�
	//���ڽ�� Ĭ�����뷨(default ime)��Ƥ���л��Ĵ���
	if (ERROR_SUCCESS != RegOpenCurrentUser(KEY_ALL_ACCESS, &cur_user_key))
	{
		Log(LOG_ID, L"!!! �޷���õ�ǰ�û���key, error=%d", GetLastError());
		cur_user_key = HKEY_CURRENT_USER;
	}

	data_length = sizeof(PIMCONFIG);

	//��ע���KEY
	ret = RegOpenKey(cur_user_key, REG_CONFIG_KEY_NAME, &reg_key);
	if (ret != ERROR_SUCCESS)
	{	//ʧ��
		Log(LOG_ID, L"����ע���Key��ʧ�ܣ����ܲ����ڡ�errcode = %d", ret);

		LoadThemeConfig(&default_config);			//װ����������

		//����ǰĬ�ϵ����ø��Ƶ�ע�������KEY��
		SaveConfigInternal(&default_config);
		memcpy(config, &default_config, sizeof(PIMCONFIG));
		return;
	}

	//��ȡ������Ϣ��
	ret = RegQueryValueEx(reg_key, REG_CONFIG_ITEM_NAME, 0, &data_type, (LPBYTE)config, &data_length);

	//������������
	//Ϊ�������������ã�spw_position����ֵ��11~19��ʹ��ʱֱ�Ӽ�10
	if(config->spw_position <= 10)
		config->spw_position = 13;
	//if(config->key_pair_candidate_page < 4)
	//	config->key_pair_candidate_page = KEY_PAIR_1 | KEY_PAIR_3;
	//if( config->scope_gbk <= 1 )
	//	config->scope_gbk = HZ_SCOPE_GBK;

	//if(config->key_pair_zi_from_word < 4)
	//	config->key_pair_zi_from_word = KEY_PAIR_2;

	if (ret == ERROR_SUCCESS && data_length == sizeof(PIMCONFIG) && data_type == REG_BINARY)	//�ɹ���ȡ
		Log(LOG_ID, L"�����ļ��ɹ�װ��");
	else
	{
		Log(LOG_ID, L"ע������ȡʧ�ܣ�errcode = %d", ret);		//ʧ��
		LoadThemeConfig(&default_config);			//װ����������
		SaveConfigInternal(&default_config);
		memcpy(config, &default_config, sizeof(PIMCONFIG));	//ʹ��Ĭ�ϵ����ݷ�����
		RegCloseKey(reg_key);			//�ر�ע���Key
		return;
	}

	RegCloseKey(reg_key);			//�ر�ע���Key

	LoadThemeConfig(&default_config);

	if (ExistThemeDirectory(config->theme_name) && ExistThemeFile(config->main_image_name))
		theme_exists = 1;

	//Ƥ�������ڣ����ߵ�ǰʹ�õ���Ĭ�����⣬�������ð汾��ͬ
	if ((!theme_exists) ||
		(theme_exists && (!_tcscmp(DEFAULT_THEME_NAME, config->theme_name)) && (config->config_version != CONFIG_VERSION)))
	{
		_tcscpy_s(config->theme_name, _SizeOf(config->theme_name), DEFAULT_THEME_NAME);
		LoadThemeConfig(config);
		config->config_version = CONFIG_VERSION;
	}

	Log(LOG_ID, L"Reg���ü������");
}

/*	ά�����뷨ϵͳ���á�
 *	�û���ʹ��ƴ�������ʱ�򣬿��ܽ������õ��޸ģ���˵����÷����ı��
 *	���뷨��Ҫ�ڵ�һʱ���ȡ���µ�������Ϣ��
 *	���뷽ʽ��
 *		1. ϵͳ�����ļ����º󣬵������뷨��̬���ӿ��е�HYPIM_ConfigModifed������
 *		���������ļ�������ʱ�䣻
 *		2. �����Ľ������봦���Լ�IME���������ʱ�򣬵���MaintainConfig��
 *		3. ���������ж�Config������޸�ʱ���Ƿ�����������ʱ�䣬�������
 *		�����½����õ����ڴ棻
 *		4. ���ע����в�û�����뷨���������ʹ��Ĭ�ϵ����뷨���ã�����ע����
 *
 *	��������
 *	���أ��ޣ���Զ��ȷ��
 */
void MaintainConfig()
{
	pim_config = &share_segment->all_config;
	CheckSpecialConfig();						//����ض���������õ�Ҫ��

	if (share_segment->config_loaded)
		return;

	CheckDefaultIME();
	LoadConfig(&share_segment->all_config);		//װ������

	share_segment->config_loaded = 1;
	share_segment->global_config_update_time = GetCurrentTicks();
}

/**	������뷨��װĿ¼
 *	������
 *		dir_name	��װĿ¼
 *	���أ�
 *		1���ɹ�
 *		0��ʧ��
 */
int GetInstallDir(TCHAR *dir_name)
{
	HKEY		reg_key;		//ע���KEY
	DWORD		data_type;		//ע����������
	DWORD		data_length;	//�������ݳ���
	LONG		ret;			//��������ֵ

	dir_name[0] = 0;

	//��ע���KEY
	ret = RegOpenKey(HKEY_LOCAL_MACHINE, REG_CONFIG_KEY_NAME, &reg_key);
	if (ret != ERROR_SUCCESS)
	{	//ʧ��
		Log(LOG_ID, L"����ע���Key��ʧ�ܣ����ܲ����ڡ�errcode = %d", ret);
		return 0;
	}

	//��ȡ������Ϣ��
	data_length = MAX_PATH;

	ret = RegQueryValueEx(reg_key, REG_INSTALL_DIR_NAME, 0, &data_type, (LPBYTE)dir_name, &data_length);

	if (!(ret == ERROR_SUCCESS && data_length > 0 && data_type == REG_SZ))	//��ȡʧ��
	{
		Log(LOG_ID, L"ע������ȡʧ�ܣ�errcode = %d", ret);		//ʧ��
		return 0;
	}

	RegCloseKey(reg_key);			//�ر�ע���Key
	return 1;
}

/**	�����չ��ѡ������
 */
int GetExpandCandidateLine()
{
	return pim_config->expand_candidate_lines;
}

/**	����ϵͳ����
 */
void RunBackup()
{
	static int first = 1;
	//HMODULE dll;
	//pBACKUP backup;
	//int ret;

	if (!first)
		return;

	first = 0;

	if (!pim_config->backup_dir[0])
		return;

	BackupUserData(pim_config->backup_dir);

	//dll = LoadLibrary(UTILITY_DLL_NAME);
	//if (!dll)
	//	return;

	//Log(LOG_ID, L"װ��DLL");
	//backup = (pBACKUP) GetProcAddress(dll, "BackupUserData");
	//if (backup)
	//	ret = (*backup)(pim_config->backup_dir);

	//Log(LOG_ID, L"����BackupUserData");

	//FreeLibrary(dll);
	//Log(LOG_ID, L"ж��DLL");
}

void ExecuteConfig(TCHAR *parameter)
{
	TCHAR cmd_line[MAX_PATH] = {0};
	STARTUPINFO	start_info;
	PROCESS_INFORMATION	process_info;

	if (!GetInstallDir(cmd_line))
		return;

	_tcscat_s(cmd_line, _SizeOf(cmd_line), TEXT("\\bin\\"));
	_tcscat_s(cmd_line, _SizeOf(cmd_line), CONFIG_EXE_NAME);
	_tcscat_s(cmd_line, _SizeOf(cmd_line), parameter);

	memset(&start_info, 0, sizeof(start_info));
	start_info.cb = sizeof(start_info);

	CreateProcess(0,
				  cmd_line,
				  0,
				  0,
				  0,
				  CREATE_DEFAULT_ERROR_MODE | NORMAL_PRIORITY_CLASS,
				  0,
				  0,
				  &start_info,
				  &process_info);
}

/**	�������ó���ֱ�����뵽�汾����
 */
void RunConfigNormal()
{
	ExecuteConfig(TEXT(""));
}

/**	�������ó���ֱ�����뵽�汾����
 */
void RunConfigVersion()
{
	ExecuteConfig(TEXT(" -ver"));
}

/**	�������ó���ֱ�����뵽�ʿ����
 */
void RunConfigWordlib()
{
	ExecuteConfig(TEXT(" -wordlib"));
}


void RunAssistTools(HWND hwnd, TCHAR *exe_filename)
{
	TCHAR install_dir[MAX_PATH];	//��װ·��
	TCHAR cmd_line[MAX_PATH];		//�����в���
	STARTUPINFO	start_info;
	PROCESS_INFORMATION	process_info;
	TCHAR slog[0x100];

	_tcscpy_s(slog, _SizeOf(slog), exe_filename);
	_tcscat_s(slog, _SizeOf(slog), TEXT("������"));

	Log(LOG_ID, slog);

	if (!GetInstallDir(install_dir))
		return;

	if(!hwnd){
		_stprintf_s(cmd_line,
				_SizeOf(cmd_line),
				TEXT("%s\\bin\\%s"),
				install_dir,
				exe_filename);
	}else{
		_stprintf_s(cmd_line,
				_SizeOf(cmd_line),
				TEXT("%s\\bin\\%s %d"),
				install_dir,
				exe_filename,
				(no_ppjz) ? 0 : (INT_PTR)hwnd);
	}

	memset(&start_info, 0, sizeof(start_info));
	start_info.cb = sizeof(start_info);
	CreateProcess(0,
				  cmd_line,
				  0,
				  0,
				  0,
				  CREATE_DEFAULT_ERROR_MODE | NORMAL_PRIORITY_CLASS,
				  0,
				  0,
				  &start_info,
				  &process_info);

	return;
}

void RunPianPangJianZi(HWND hwnd)
{
	RunAssistTools(hwnd, PPJZ_EXE_NAME);
}

void RunSPWPlugin(HWND hwnd)
{
	RunAssistTools(hwnd, SPW_EXE_NAME);
}

//void RunURLManager(HWND hwnd)
//{
//	RunAssistTools(hwnd, URL_EXE_NAME);
//}

void RunCFGWizard(HWND hwnd)
{
	RunAssistTools(hwnd, CFGWIZARD_EXE_NAME);
}

void RunBatchWords(HWND hwnd)
{
	RunAssistTools(hwnd, BATCH_WORDS_EXE_NAME);
}

void RunZiEditor(HWND hwnd)
{
	RunAssistTools(hwnd, ZIEDITOR_EXE_NAME);
}

void RunThemeMaker(HWND hwnd)
{
	RunAssistTools(hwnd, THEME_MAKER_EXE_NAME);
}

void RunCompose(HWND hwnd)
{
	RunAssistTools(hwnd, COMPOSE_EXE_NAME);
}

/*void RunImeManager(HWND hwnd)
{
	RunAssistTools(hwnd, IMEMANAGER_EXE_NAME);
}*/