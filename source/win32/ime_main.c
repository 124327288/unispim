/*	IME��������
 */
#include <config.h>
#include <utility.h>
#include <pim_resource.h>
#include <win32/pim_ime.h>
#include <time.h>
#include <icw.h>
#include <share_segment.h>

//ȫ��instance
HINSTANCE	global_instance = 0;

//#pragma data_seg(HYPIM_SHARED_SEGMENT)
//static int	process_count = 0;			//��IME���ӵĽ��̼���
//static int	first_use	  = 1; 			//��һ��ʹ��IME
//#pragma data_seg()

int resource_thread_finished = 0;

DWORD WINAPI LoadResources(LPVOID dummy)
{
	Log(LOG_ID, L"����װ����Դ�߳�");
	PIM_LoadResources();
	resource_thread_finished = 1;
	Log(LOG_ID, L"����װ����Դ�߳�");

	//�ӿ�bigram��װ���ٶ�
	MakeBigramFaster();

	return 0;
}

/**	DLL���
 */
int WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID dummy)
{
	static HANDLE h_thread;
	extern int LoadSPResource();		//����Hint��Ϣ��������Դû��װ��֮ǰ��ʾ�����Ա������ڵ��ñ�����

	if (!share_segment)		//������share_segment�ͳ�ʼ��log
		LogInit(1);

	LoadSharedSegment();

	switch (reason)
	{
		case DLL_PROCESS_ATTACH:
			Log(LOG_ID, L"Dll attach, instance:0x%x", instance);

			GetProgramName();

			//����ȫ�ֻ�����
			global_instance = instance;

			//���ø���
			MaintainConfig();

			LoadSPResource();					//װ��˫ƴ��Դ

			//װ����Դ�߳�
			h_thread = CreateThread(0, 0, LoadResources, 0, 0, 0);

			Log(LOG_ID, L"res thread return:%x", h_thread);

			//���ӽ��̼���
			share_segment->process_count++;

			//ע��UI����Class
			RegisterUIWindowClass(instance);

			break;

		case DLL_PROCESS_DETACH:
			Log(LOG_ID, L"Dll detach, instance:0x%x", instance);

			share_segment->process_count--;			//���ٽ��̼���

			//���UI������
			UnregisterUIWindowClass(instance);

			//����RunDll32֮��ĳ���Load��Free���뷨DLL���ٶ�̫��
			//������DETACH��ʱ���̲߳�û�н��������Ա���ر��Ѿ�
			//���е��߳�
			CloseHandle(h_thread);

			//Ϊ����U�̲���ʱ��RunDll32������������Ӧ�ö�resource_thread_finished
			//�����ж�
			if (resource_thread_finished)
			{
				//�������뷨��������������
				if (share_segment->process_count)
					PIM_SaveResources();
				else
					PIM_FreeResources();
			}

			SaveConfigInternal(pim_config);

			if (!share_segment->process_count)
			{
				FreeSharedSegment();
				FreeLog();
			}

			break;
    }

    return 1;
}