/*	���뷨ϵͳ���ó���
 *	������
 *	imesetup install		--��װ���뷨
 *	imesetup uninstall		--ж�����뷨
 *	ж�ط�ʽ��
 *		����Google���뷨��ж�ط�����
 *		1. Unload�����뷨
 *		2. Delete Preload�еı����뷨
 *		3. ��SHELL����ɾ�������뷨����
 *	�����ֵ���˳���£����뷨��������ж�أ���лIDag.exe���Ǻǡ�
 *
 *	����ֵ��
 *	-1		������
 *	-2		IME��װ����ʧ��
 *	-3		ϵͳĿ¼��ȡ����
 *	-4		װ��IME���뷨�ļ�ʧ��
 *
 *	����Vista��½��������뷨�б�
 *	��HKEY_USERS\.DEFAULT\Keyboard Layout\Preload�м��뱾���뷨
 *	���������б����г����ǵ����뷨
 */
#include <string.h>
#include <stdio.h>
#include <windows.h>
#include <regstr.h>
#include <imm.h>
#include <shlwapi.h>
#include <tchar.h>

#define	MAX_ENTRY			0x100			//���������ע�����Ŀ������ж�أ�
#define	MAX_PRELOADS		0x10			//����Ԥװ���뷨����Ŀ

#define	ERR_PARAMETER		-1
#define	ERR_INSTALL			-2
#define	ERR_DIR				-3
#define	ERR_LOAD			-4

#define _SizeOf(x) (sizeof((x)) / sizeof((x)[0]))

const TCHAR *ime_file_name;
const TCHAR *ime_text = _T("���� - ����ƴ�����뷨V6");

TCHAR *usage_msg = _T("ime_install [/i ime_file_name]\n")
				   _T("            [/u ime_file_name]\n");

void error_exit(int no)
{
	switch(no)
	{
	case ERR_PARAMETER:		//��������
		fprintf(stderr, "%s\n", usage_msg);
		exit(-1);

	case ERR_INSTALL:
		fprintf(stderr, "���뷨��װ���󡣴����:%d\n", GetLastError());
		exit(-2);

	case ERR_DIR:
		fprintf(stderr, "��ȡϵͳĿ¼���󡣴���ţ�%d\n", GetLastError());
		exit(-3);

	case ERR_LOAD:
		fprintf(stderr, "�޷�װ��IME�ļ��������: %d\n", GetLastError());
		exit(-4);
	}

	exit(no);
}

//��Ĭ���û������ñ����뷨��������Vista Login�����пɼ����ǵ����뷨
void set_to_default_user(int hkl_id)
{
	TCHAR id_str[0x10];
	TCHAR no_str[0x10];
	TCHAR hkl_str[0x10];
	HKEY hkey;
	int last_key_no, key_no;
	DWORD size, type;

	_stprintf_s(id_str, _SizeOf(id_str), _T("%X"), hkl_id);

	//��HKEY_USERS\.DEFAULT\Keyboard Layout\Preload�м��뱾���뷨
	if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_USERS, _T(".DEFAULT\\Keyboard Layout\\Preload"), 0, KEY_ALL_ACCESS, &hkey))
		return;

	last_key_no = 0;
	for (key_no = 0; key_no < MAX_PRELOADS; key_no++)
	{
		_stprintf_s(no_str, _SizeOf(no_str), _T("%d"), key_no + 1);

		//����RegQueryValueEx���һ��������MSDN���ͣ�ע�����һ�������ǵ����ڶ�������
		//ָ���buffer�Ĵ�С����һ����RegSetValueEx����������(�������ע��)
		//A pointer to a variable that specifies the size of the buffer pointed to by
		//the lpData parameter, in bytes. When the function returns, this variable contains 
		//the size of the data copied to lpData. If the data has the REG_SZ, REG_MULTI_SZ 
		//or REG_EXPAND_SZ type, this size includes any terminating null character or 
		//characters unless the data was stored without them.
		hkl_str[0] = 0;
		size = sizeof(hkl_str);
		type = REG_SZ;
		if (ERROR_SUCCESS == RegQueryValueEx(hkey, no_str, 0, &type, (LPBYTE)hkl_str, &size))
		{
			last_key_no = key_no + 1;
			//�ҵ������Լ��ģ�����
			if (!_tcsicmp(hkl_str, id_str))
				break;
		}
	}

	if (key_no == MAX_PRELOADS)		//δ�ҵ����ǵ���Ҫ�趨
	{
		//����RegSetValueEx���һ��������MSDN���ͣ�ע�����һ�������ǵ����ڶ�������
		//����Ϣ(���ַ���)���ֽ����������ǵ����ڶ�����������Ӧ��buffer��С
		//The size of the information pointed to by the lpData parameter, in bytes. 
		//If the data is of type REG_SZ, REG_EXPAND_SZ, or REG_MULTI_SZ, cbData must
		//include the size of the terminating null character or characters
		_stprintf_s(no_str, _SizeOf(no_str), _T("%d"), last_key_no + 1);
		RegSetValueEx(hkey, no_str, 0, REG_SZ, (LPBYTE)id_str, (_tcslen(id_str) + 1) * sizeof(TCHAR));
	}

	RegCloseKey(hkey);

	return;
}

int install_ime()
{
	TCHAR sys_dir_name[MAX_PATH];
	TCHAR ime_name[MAX_PATH];
	HKL keyboard_layout_id;

	//���ϵͳĿ¼λ��
	if (!GetSystemDirectory(sys_dir_name, _SizeOf(sys_dir_name)))
		error_exit(ERR_DIR);
  
	_stprintf_s(ime_name, _SizeOf(ime_name), _T("%s\\%s"), sys_dir_name, ime_file_name);

	SetLastError(0);
	keyboard_layout_id = ImmInstallIME(ime_name, ime_text);
	if (!keyboard_layout_id)
		error_exit(ERR_INSTALL);

	set_to_default_user(*(int*)&keyboard_layout_id);

	return 0;
}

int uninstall_ime()
{
	HKEY hkey, found_key;
	DWORD size, type;
	TCHAR key_str[256];
	TCHAR key_str_save[256];
	TCHAR ime_name[256];
	TCHAR hkl_value[256], hkl_str[256], hkl_tag_str[256];
	TCHAR no_str[0x10];
	int deleted;				//�Ѿ�ɾ�������뷨��
	unsigned int key_no, cur_key = 0xe01f0804;
	LONG lres;
	TCHAR *layout_id;

	for (key_no = 0; key_no < MAX_ENTRY; key_no++)
	{
		cur_key += 0x00010000;
		_stprintf_s(key_str, _SizeOf(key_str), REGSTR_PATH_CURRENT_CONTROL_SET _T("\\Keyboard Layouts\\%08X"), cur_key);

		if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, key_str, 0, KEY_READ, &hkey))
		{
			ime_name[0] = 0;
			size = sizeof(ime_name);
			type = REG_SZ;
			RegQueryValueEx(hkey, _T("Ime File"), 0, &type, (LPBYTE)ime_name, &size);
			RegCloseKey(hkey);

			//Ѱ����ͬ��ime�ļ�
			if (!_tcsicmp(ime_name, ime_file_name))
				break;
		}
	}

	_tcscpy_s(key_str_save, _SizeOf(key_str_save), key_str);
	UnloadKeyboardLayout(*(HKL*)&cur_key);

	_stprintf_s(hkl_tag_str, _SizeOf(hkl_tag_str), _T("%08X"), cur_key);

	//��HKEY_CURRENT_USER/Keyboard Layout/Preload��ɾ�����ǵ����뷨
	deleted = 0;
	_stprintf_s(key_str, _SizeOf(key_str), _T("Keyboard Layout\\Preload"));
	if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_CURRENT_USER, key_str, 0, KEY_ALL_ACCESS, &hkey))
	{
		for (key_no = 0; key_no < MAX_PRELOADS; key_no++)
		{
			_stprintf_s(no_str, _SizeOf(no_str), _T("%d"), key_no + 1);
			hkl_str[0] = 0;
			size = sizeof(hkl_str);
			type = REG_SZ;
			if (ERROR_SUCCESS == RegQueryValueEx(hkey, no_str, 0, &type, (LPBYTE)hkl_str, &size))
			{
				//�ҵ������Լ��ģ�ɾ��
				if (!_tcsicmp(hkl_str, hkl_tag_str))
				{
					RegDeleteValue(hkey, no_str);
					deleted++;
					continue;
				}
				//ɾ��֮����Ҫ�Ժ����ע�������ǰ
				//�ҵ�������Ҫ�ж��Ƿ�Ϊ��ȷ�����뷨������Ҳ��Ҫɾ��
				_stprintf_s(key_str, _SizeOf(key_str), REGSTR_PATH_CURRENT_CONTROL_SET _T("\\Keyboard Layouts\\%s"), hkl_str);
				if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, key_str, 0, KEY_READ, &found_key))
				{
					RegCloseKey(found_key);
					//���ǰ��ɾ�������뷨������Ҫ�������뷨��ǰ
					if (deleted)
					{
						//SHDeleteKey֮��ĺ�������������Ч��������������ʱ�򣬲Ž���ɾ��!
						//1. ɾ��ԭ����value
						//SHDeleteValue(HKEY_CURRENT_USER, "Keyboard Layouts\\Preload", no_str);
						RegDeleteValue(hkey, no_str);
						//2. �趨��ǰ��value
						_stprintf_s(no_str, _SizeOf(no_str), _T("%d"), key_no + 1 - deleted);
						//SHSetValue(HKEY_CURRENT_USER, "Keyboard Layouts\\Preload", no_str, type, hkl_str, size); 
						RegSetValueEx(hkey, no_str, 0, type, (LPBYTE)hkl_str, size);
					}
				}
				else
				{
					RegDeleteValue(hkey, no_str);
					deleted++;
				}
			}
		}
	}

	//��HKEY_USERS/.DEFAULT/Keyboard Layout/Preload��ɾ�����ǵ����뷨
	deleted = 0;
	_stprintf_s(key_str, _SizeOf(key_str), _T(".DEFAULT\\Keyboard Layout\\Preload"));
	if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_USERS, key_str, 0, KEY_ALL_ACCESS, &hkey))
	{
		for (key_no = 0; key_no < MAX_PRELOADS; key_no++)
		{
			_stprintf_s(no_str, _SizeOf(no_str), _T("%d"), key_no + 1);
			hkl_str[0] = 0;
			size = sizeof(hkl_str);
			type = REG_SZ;
			if (ERROR_SUCCESS == RegQueryValueEx(hkey, no_str, 0, &type, (LPBYTE)hkl_str, &size))
			{
				//�ҵ������Լ��ģ�ɾ��
				if (!_tcsicmp(hkl_str, hkl_tag_str))
				{
					RegDeleteValue(hkey, no_str);
					deleted++;
					continue;
				}
				//ɾ��֮����Ҫ�Ժ����ע�������ǰ
				//�ҵ�������Ҫ�ж��Ƿ�Ϊ��ȷ�����뷨������Ҳ��Ҫɾ��
				_stprintf_s(key_str, _SizeOf(key_str), REGSTR_PATH_CURRENT_CONTROL_SET _T("\\Keyboard Layouts\\%s"), hkl_str);
				if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, key_str, 0, KEY_READ, &found_key))
				{
					RegCloseKey(found_key);
					//���ǰ��ɾ�������뷨������Ҫ�������뷨��ǰ
					if (deleted)
					{
						//SHDeleteKey֮��ĺ�������������Ч��������������ʱ�򣬲Ž���ɾ��!
						//1. ɾ��ԭ����value
						//SHDeleteValue(HKEY_CURRENT_USER, "Keyboard Layouts\\Preload", no_str);
						RegDeleteValue(hkey, no_str);
						//2. �趨��ǰ��value
						_stprintf_s(no_str, _SizeOf(no_str), _T("%d"), key_no + 1 - deleted);
						//SHSetValue(HKEY_CURRENT_USER, "Keyboard Layouts\\Preload", no_str, type, hkl_str, size); 
						RegSetValueEx(hkey, no_str, 0, type, (LPBYTE)hkl_str, size);
					}
				}
				else
				{
					RegDeleteValue(hkey, no_str);
					deleted++;
				}
			}
		}
	}

	//��HKEY_CURRENT_USER\Control Panel\International\User Profile\zh-Hans-CN��ɾ�����ǵ����뷨
	//ע������ΪWIN8��������WIN7��XPû�и�������еļ�ֵ�������뷨ע��ʱ���룬����ֻҪ�������
	//��\ʱ�ӡ����Ժ�����\����\����ѡ��ʱ��ϵͳ�ͻ��Զ������ǵ����뷨id��Ϊһ����ֵ��ӵ������У�
	//��ж�������ǵ����뷨��ϵͳ����������������ٴδ򿪿������\ʱ�ӡ����Ժ�����\����\����
	//ѡ��ʱ�����в�����һ���������õ����뷨����ע�⣬ÿ�����뷨��Ӧ�ļ�ֵ�����ݱ�ʾ�����뷨����
	//��ѡ�������뷨��������ţ����ǽ���ֵɾ������������Ų������������޹�ϵ��ϵͳ������һ�ο�
	//��������\ʱ�ӡ����Ժ�����\����\����ѡ��ʱ�Զ�����ʣ������뷨����Ϊ��ֵָ�����
	_stprintf_s(key_str, _SizeOf(key_str), _T("Control Panel\\International\\User Profile\\zh-Hans-CN"));
	if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_CURRENT_USER, key_str, 0, KEY_ALL_ACCESS, &hkey))
	{
		for (key_no = 0;; key_no++)
		{
			//ע��˴���size���ַ�������RegSetValueEx��RegQueryValueEx����ͬ
			//A pointer to a variable that specifies the size of the buffer pointed to by the 
			//lpValueName parameter, in characters. When the function returns, the variable
			//receives the number of characters stored in the buffer, not including the terminating
			//null character.
			size = _SizeOf(hkl_value);
			lres = RegEnumValue(hkey, key_no, hkl_value, &size, 0, NULL, NULL, NULL);

			if (ERROR_NO_MORE_ITEMS == lres)
			{
				break;
			}
			else if (ERROR_SUCCESS == lres)
			{
				layout_id = _tcsstr(hkl_value, _T("0804:"));

				if (layout_id && !(layout_id - hkl_value))
				{
					layout_id += _tcslen(_T("0804:"));

					//�ҵ������Լ��ģ�ɾ��
					if (!_tcscmp(layout_id, hkl_tag_str))
					{
						RegDeleteValue(hkey, hkl_value);
					}
				}
			}
		}
	}

	//��HKEY_CURRENT_USER\Control Panel\International\User Profile\zh-Hans��ɾ�����ǵ����뷨
	//��ʱHKEY_CURRENT_USER\Control Panel\International\User Profile��ֻ��zh-Hans-CN��Ŀǰ�в������
	//���ߵ�����
	_stprintf_s(key_str, _SizeOf(key_str), _T("Control Panel\\International\\User Profile\\zh-Hans"));
	if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_CURRENT_USER, key_str, 0, KEY_ALL_ACCESS, &hkey))
	{
		for (key_no = 0;; key_no++)
		{
			size = _SizeOf(hkl_value);
			lres = RegEnumValue(hkey, key_no, hkl_value, &size, 0, NULL, NULL, NULL);

			if (ERROR_NO_MORE_ITEMS == lres)
			{
				break;
			}
			else if (ERROR_SUCCESS == lres)
			{
				layout_id = _tcsstr(hkl_value, _T("0804:"));

				if (layout_id && !(layout_id - hkl_value))
				{
					layout_id += _tcslen(_T("0804:"));

					//�ҵ������Լ��ģ�ɾ��
					if (!_tcscmp(layout_id, hkl_tag_str))
					{
						RegDeleteValue(hkey, hkl_value);
					}
				}
			}
		}
	}

	RegCloseKey(hkey);

	//ɾ�����뷨��
	RegDeleteKey(HKEY_LOCAL_MACHINE, key_str_save);

	return 0;
}

int _tmain(int argc, TCHAR* argv[])
{
	if (argc != 3)
		error_exit(ERR_PARAMETER);

	if (!_tcscmp(argv[1], _T("/u")))
	{
		ime_file_name = argv[2];
		uninstall_ime();
		return 0;
	}

	//��װ���뷨
	if (!_tcscmp(argv[1], _T("/i")))
	{
		ime_file_name = argv[2];
		install_ime();
		return 0;
	}

	error_exit(ERR_PARAMETER);
	return 0;
}
