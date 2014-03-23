/*	�����ģ��
 */

#include <config.h>
#include <utility.h>
#include <win32/softkbd.h>
#include <assert.h>

static int current_index = -1;	/* ��ǰ��������� */

#pragma data_seg(HYPIM_SHARED_SEGMENT)

/* ����̶�Ӧ���� */
static const TCHAR softkbd_vkey_map[] =
{
	' ', '0', '1', '2', '3', '4', '5','6', '7', '8', '9',
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 
	'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
	0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf, 0xc0, 0xdb, 0xdc, 0xdd, 0xde
};

/* �����ַ� */
static TCHAR softkbd_char_map[SOFTKBD_NUMBER * 2][SOFTKBD_KEY_NUMBER + 1] =
{
/*  "���������������������������������������������������������������������ۣܣݣ�", */
	TEXT(" �΢ŢƢǢȢɢʢˢ̢͡����ơ��������������æ¢��������������좲�뢶�磻������������ۡ��ݡ�"),
	TEXT(" ���������������ǡ�������������¡����������������������������ߡ�������������"),
	TEXT(" �������������������������������������á������ۣ������������������������������ݡ��������ࡨ����"),
	TEXT("                                                "),
	TEXT("           ���ޣ��������������������ߡ����������ܡ���           "),
	TEXT("                                                "),
	TEXT(" ���������������� ���Щ��ҩ©өԩթǩ֩� ��� �����éѩĩƩ����ũ�  �穬  ���� �� "),
	TEXT(" ���������������� ���ة��کʩ۩ܩݩϩީ� ��� ���ȩ˩٩̩Ω�ɩ�ͩ�  �舘  ���� �� "),
	TEXT(" ��������������������ɢǢ碳���ꢸ����ˢʢ��������梵���Ȣ��Ƣ��Ţ� �̢��͢΢�    "),
	TEXT("           �٢ӢѢۢ��ܢݢޢ¢ߢ��բԢâĢ����ڢ����Ң��Т��Ϣ� �� �ע�     "),
	TEXT("  �ԡ٣��ܡݣ����ڡۡġѡϡƣ��ǡȡɡҡ� �ߡסաӡء����š� �У��Σ��͡�  �ˡ� �֡�   "),
	TEXT("                                                "),
	TEXT(" ������꣤�룥��� ��������׼���̫�İ�  �˾š���ǧ����΢һ�����   ��   ʮ   "),
	TEXT("           ��   ��   ��     �ƾ�����Ǫ��½ Ҽ ��        ʰ   "),
	TEXT("           �� �������� ���������������� ������ ������������  �� ���� ��   "),
	TEXT("                                                "),
	TEXT(" ��Ũɡ��A�Ө@�B�ڨިǨ֨ϨΨͨѨըبۨ�ܨ��٨ߨ�ƨШ˨Ԩ�Ҩʨ̨רȨ� �ݨ���    "),
	TEXT("                                                "),
	TEXT("  �������������������ʤ��̤��ͤΤϤĤҤդؤ��äƤ����ˤ����ᤷ�ߤ��ޤ� ����򤡤� ���"),
	TEXT("  �������������������Ѥ��פ��ڤݤФŤӤ֤٤�� �Ǥ����Ԥ��¤줸������ �� �� ����   "),
	TEXT(" ���������������������ʥ��̥��ͥΥϥĥҥեإ��åƥ����˥����᥷�ߥ��ޥۥ������򥡥ȥ���"),
	TEXT(" ������ ������ �����ѥ��ץ��ڥݥХťӥ֥٥�� �ǥ����ԥ��¥쥸�����ܥ��� �� ����   "),
	TEXT("           �ʦ֦Ԧ̦æͦΦϦȦЦ� �ئצ� ���Ħ˦ŦǦզ¦ӦƦ�           "),
	TEXT("           ���������������������� ������ ��������������������           "),
	TEXT("           �ݧ��ߧӧ���ا�����٧ڧѧԧާէק�ҧ�֧�� �� ��� �� �ܧ�"),
	TEXT("           ������������������������������������������������������ �� ���� �� ����"),
    TEXT(" 0123456789abcdefghijklmnopqrstuvwxyz;=,-./`[\\]'"),
	TEXT(" )!@#$%^&*(ABCDEFGHIJKLMNOPQRSTUVWXYZ:+<_>?~{|}\""),
    TEXT(" 0123456789abcdefghijklmnopqrstuvwxyz;=,-./`[\\]'"),
	TEXT(" )!@#$%^&*(ABCDEFGHIJKLMNOPQRSTUVWXYZ:+<_>?~{|}\""),
};
#pragma data_seg()

static HWND	softkbd_window;

/*	����������ַ�
 *	������
 *		index		��ǰ����̵�����
 */
static void SetSoftKBDData(PIMCONTEXT *context, int index)
{
	int i;
	SOFTKBDDATA softkbd_data[2];
	LPSOFTKBDDATA data;

	if (current_index == index)
		return;

	current_index = index;

	data = &softkbd_data[0];
	data->uCount = 2;
    for (i = 0; i < SOFTKBD_KEY_NUMBER; i++)
	{
		data->wCode[0][softkbd_vkey_map[i]] = softkbd_char_map[index * 2][i];
		data->wCode[1][softkbd_vkey_map[i]] = softkbd_char_map[index * 2 + 1][i];
    }

	pim_config->soft_kbd_index = index;
	SaveConfigInternal(pim_config);
    SendMessage(softkbd_window, WM_IME_CONTROL, IMC_SETSOFTKBDDATA, (LPARAM) data);
}

/*	����������̴���
 *	������
 *		ui_window		UI���ھ��
 */
void CreateSoftKBDWindow(PIMCONTEXT *context)
{
	RECT softkbd_rect, status_rect, work_rect;
	POINT point;
	int	 x, y;

	softkbd_window = ImmCreateSoftKeyboard(SOFTKEYBOARD_TYPE_C1, context->ui_context->ui_window, 0, 0);

	GetWindowRect(softkbd_window, &softkbd_rect);
	GetWindowRect(context->ui_context->status_window, &status_rect);

	point.x = status_rect.left;
	point.y = status_rect.top;

	work_rect = GetMonitorRectFromPoint(point);

	x = work_rect.right - (softkbd_rect.right - softkbd_rect.left);
	y = status_rect.top - (softkbd_rect.bottom - softkbd_rect.top);

	if (y < work_rect.top)
		y = status_rect.bottom;

	SetWindowPos(softkbd_window, 0, x, y, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER);
}

/* ��ʾ�����
 */
void ShowSoftKBDWindow()
{
	if (softkbd_window)
		ImmShowSoftKeyboard(softkbd_window, SW_SHOWNOACTIVATE);
}

/*	���������
 */
void HideSoftKBDWindow()
{
	if (softkbd_window)
		ImmShowSoftKeyboard(softkbd_window, SW_HIDE);
}

/*	��ѡ�������
 */
void DeSelectSoftKBD()
{
	if (softkbd_window)
		ImmDestroySoftKeyboard(softkbd_window);

	softkbd_window = 0;
	current_index  = -1;
}

/* ѡ������� */
void SelectSoftKBD(PIMCONTEXT *context, HIMC hIMC, int index)
{
	if (current_index != index)
	{
		if (softkbd_window)
			DeSelectSoftKBD();
	}

	if (!softkbd_window)
		CreateSoftKBDWindow(context);

	SetSoftKBDData(context, index);
}

/*	��������̵ķ���
 */
int GetSoftKeyboardSymbol(PIMCONTEXT *context, int virtual_key, int key_flag, TCHAR *symbol)
{
	int i;

	if (key_flag & KEY_CONTROL)
		return 0;

	for (i = 0; i < SOFTKBD_KEY_NUMBER; i++)
		if ((BYTE)virtual_key == softkbd_vkey_map[i])
			break;

	if (i == SOFTKBD_KEY_NUMBER)
		return 0;

	if (((key_flag & KEY_SHIFT) && !(key_flag & KEY_CAPITAL)) ||
		(!(key_flag & KEY_SHIFT) && (key_flag & KEY_CAPITAL)))
	{
		symbol[0] = softkbd_char_map[context->softkbd_index * 2 + 1][i];
		symbol[1] = 0;
	}
	else
	{
		symbol[0] = softkbd_char_map[context->softkbd_index * 2][i];
		symbol[1] = 0;
	}

	if (symbol[0] == ' ' && symbol[1] == ' ')
		symbol[0] = 0;

	//Ӣ�ļ���
	if (context->softkbd_index == SOFTKBD_NUMBER - 2)
		symbol[1] = 0;

	return 1;
}