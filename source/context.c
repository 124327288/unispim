/*	���������Ĺ���
 */
#include <kernel.h>
#include <editor.h>
#include <context.h>
#include <config.h>
#include <utility.h>
#include <win32/pim_ime.h>
#include <win32/main_window.h>
#include <tchar.h>

//ȫ�����������ݣ�Ҳ��Ĭ�ϵ�����
//PIMCONTEXT *context;			//ȫ�֣��̼߳���Context�����ڴ��ڵ���ʾ

PIMCONTEXT demo_context =
{
	STATE_START,

	//����״̬
	CHINESE_MODE | QP_MODE,		//����ģʽ
	0,							//�����
	HZ_LESS_USED,				//��ǰ���ּ��ϵ�level
	0,							//��չ��ѡ״̬
	0,							//������ʾ��ȫ��ǿ��������ʾ

	//����״̬
	1, 							//�Ƿ���ʾд������
	1,							//�Ƿ���ʾ��ѡ����
	1,							//�Ƿ���ʾ״̬����
	VIEW_MODE_HORIZONTAL,		//��ѡ��ʾģʽ��Ĭ�Ϻ���

	//���ؽ��
	TEXT(""),					//���ؽ���ַ���
	0,							//���ؽ������
	{ {0}, },					//�����������
	0,							//������ڼ���
	0,							//��ǰѡ��ĺ�ѡ����

	//����������
	TEXT("hua'yu'pin'yin"),	//�û����������
	14,							//���봮����
	0,							//���ڴ�������봮λ��
	9,							//���λ���������Ե�ǰ���ڴ����syllableΪ׼��
	0,							//�û�����ķ���
	0,							//�ϴ������Ƿ�Ϊ����
	0,							//ʣ����ַ�
	0,							//�ϴ�������Ƿ�Ϊ��
	0,							//���ϴ�������Ƿ�Ϊ��

	//��ǰ����
	{ 0, },						//Syllables
	//{ 0, },						//syllable correct flags
	//{ 0, },						//���ڷָ������
	{ 0, },						//syllable start position
	4,							//���ڼ���
	0,							//��ǰ��������λ��
	0,
	{ 0, },                     //Ĭ�Ϻ��ִ�
	{ 0, },                     //Ĭ�Ϻ��ִ�������

	//�Ѿ�ѡ�����Ŀ
	{ {0}, },					//�Ѿ�������ѡ�������
	0,							//���������

	//���ܱ༭״̬����
	{ 0, },						//�Ѿ�ѡ�õĺ���
	{ 0, }, 					//���ܱ༭״̬������λ��

	//д��
	TEXT("hua2'yu3'pin'yin"),	//д�����ڵ�����
	TEXT(""),					//�Ѿ�ѡ���д����������
	TEXT(""),					//˫ƴ��ʾ����
	TEXT(""),					//����������ʾ����
	12,							//д��������
	8,							//���λ��

	//��ѡ
	{
		{ CAND_TYPE_ICW },
	},							//��ѡ����
	42,							//��ѡ��Ŀ
	0,							//��ʾ��ѡ�ĵ�һ������
	1,	 						//��ѡ�еĺ�ѡ����

	//��ǰҳ��Ҫ��ʾ�ĺ�ѡ
	{
		TEXT("����ƴ��"),	TEXT("����"),	TEXT("��"), TEXT("��"), TEXT("��"), TEXT("��"), TEXT("��"), TEXT("��"), TEXT("��"),
		TEXT("��"), TEXT("��"), TEXT("��"), TEXT("�N"), TEXT("��"),	TEXT("��"),	TEXT("�O"),	TEXT("��"), TEXT("�@"), 
		TEXT("��"), TEXT("��"), TEXT("��"), TEXT("�y"), TEXT("��"), TEXT("��"),	TEXT("�_"),	TEXT("�i"), TEXT("Δ"), 
		TEXT("��"), TEXT("��"), TEXT("�h"), TEXT("��"), TEXT("�s"), TEXT("��"),	TEXT("��"),	TEXT("�X"), TEXT("��"), 
		TEXT("�")
	},
	45,							//��ǰҳ�еĺ�ѡ��Ŀ
	{ 0, },

	0,							//IME��Ϣ��־
	0,							//��ǰ����Ĵ��ھ��

	0,							//���������
	0,							//��д������
	-1,							//���ñ任ʱ��

	//Ӣ���������ѡ��
	0,							//Ӣ�Ĳ�ȫģʽ
	0,							//�Ƿ���Ӣ�ĺ�ѡ

	0,    						//u����״̬

	0,                          //ָ��uicontext��ָ��           
};

/**	�����һ�εĽ��
 */
void ClearResult(PIMCONTEXT *context)
{
	context->result_length		   = 0;
	context->result_string[0]	   = 0;
	context->result_syllable_count = 0;
}

/**	��ʼ������������
 */
void ResetContext(PIMCONTEXT *context)
{
	context->candidate_count		= 0;
	context->candidate_index		= 0;
	context->candidate_page_count	= 0;
	context->compose_cursor_index	= 0;
	context->compose_length			= 0;
	context->compose_string[0]		= 0;
	context->cursor_pos				= 0;
	context->last_symbol			= 0;
	context->input_length			= 0;
	context->input_pos				= 0;
	context->input_string[0]		= 0;
	context->selected_digital		= 0;
	context->selected_item_count	= 0;
	context->syllable_count			= 0;
	context->syllable_pos			= 0;
	context->show_candidates		= 0;
	context->show_composition		= 0;
	context->spw_hint_string[0]		= 0;
	context->sp_hint_string[0]		= 0;
	context->has_english_candidate	= 0;
	context->force_vertical			= 0;
	//context->url_state				= URL_STATE_NONE;
	context->u_state				= 0;
	context->candidates_view_mode	= VIEW_MODE_HORIZONTAL;
	context->expand_candidate		= (pim_config->always_expand_candidates && CanSwitchToExpandMode(context)) ? 1 : 0;
	context->syllable_mode			= 0;
	context->last_dot				= 0;
	context->next_to_last_dot		= 0;

	if (context->soft_keyboard)
		context->state = STATE_SOFTKBD;
	else if (context->capital)
		context->state = STATE_CAPITAL;
	else if (context->input_mode & CHINESE_MODE)
		context->state = STATE_START;
	else if (context->input_mode & ENGLISH_MODE)
		context->state = STATE_ENGLISH;

	if (context->english_state == ENGLISH_STATE_CAND)
		context->english_state = ENGLISH_STATE_NONE;

	if (!pim_config->use_english_input)
		context->english_state = ENGLISH_STATE_NONE;
}

/**	��һ�ν��������ĵĳ�ʼ��
 */
void FirstTimeResetContext(PIMCONTEXT *context)
{
	BYTE key_state[0x100];

	memcpy(context, &demo_context, sizeof(PIMCONTEXT));
	GetKeyboardState(key_state);
	if (key_state[VK_CAPITAL] & 0x1)
		context->capital = 1;

	if (pim_config->startup_mode == STARTUP_ENGLISH)
		context->input_mode = ENGLISH_MODE;

	if (pim_config->use_english_input && pim_config->startup_with_english_input)
	{
		context->english_state = ENGLISH_STATE_INPUT;
		if (pim_config->hz_option & HZ_SYMBOL_CHINESE)
			pim_config->hz_option ^= HZ_SYMBOL_CHINESE;
	}
	//else
	//	pim_config->hz_option |= HZ_SYMBOL_CHINESE;

	//����ʱĬ�ϰ������
	pim_config->hz_option |= HZ_SYMBOL_HALFSHAPE;

	ResetContext(context);

	context->local_config_update_time = GetCurrentTicks();
	context->show_status			  = pim_config->show_status_window;
}

/*	��תȫ�ǡ���Ƿ���
 */
void ToggleFullShape(PIMCONTEXT *context)
{
	Log(LOG_ID, L"��תȫ�ǡ����");
	pim_config->hz_option ^= HZ_SYMBOL_HALFSHAPE;
	SaveConfigInternal(pim_config);
	context->modify_flag |= MODIFY_STATUS;
}

/*	��תȫƴ��˫ƴ
 */
void ToggleQuanShuang(PIMCONTEXT *context)
{
	Log(LOG_ID, L"��תȫƴ˫ƴ");
	pim_config->pinyin_mode = (PINYIN_QUANPIN + PINYIN_SHUANGPIN) - pim_config->pinyin_mode;
	SaveConfigInternal(pim_config);
	context->modify_flag |= MODIFY_STATUS;
}

/*  ��ת���š�������ʾ
 */
void ToggleHorizonalVertical(PIMCONTEXT *context)
{
	Log(LOG_ID, L"��ת��������");
	pim_config->show_vertical_candidate = !pim_config->show_vertical_candidate;
	SaveConfigInternal(pim_config);
	context->modify_flag |= MODIFY_STATUS;
}

void ToggleIgnoreFullShape(PIMCONTEXT *context)
{
	Log(LOG_ID, L"��תȫ������");
	pim_config->ignore_fullshape = !pim_config->ignore_fullshape;
	SaveConfigInternal(pim_config);
	context->modify_flag |= MODIFY_STATUS;
}

/**	�趨Ϊ����
 */
void SetToFanti(PIMCONTEXT *context)
{
	if (!context || !pim_config)
		return;

	pim_config->hz_output_mode = HZ_OUTPUT_TRADITIONAL;
	SaveConfigInternal(pim_config);
	context->modify_flag |= MODIFY_STATUS;
}

/**	�趨Ϊ����
 */
void SetToJianti(PIMCONTEXT *context)
{
	if (!context || !pim_config)
		return;

	pim_config->hz_output_mode = HZ_OUTPUT_SIMPLIFIED;
	SaveConfigInternal(pim_config);
	context->modify_flag |= MODIFY_STATUS;
}

/**	�趨Ϊȫ��
 */
void SetToLargeSet(PIMCONTEXT *context)
{
	if (!context || !pim_config)
		return;

	pim_config->hz_output_mode = HZ_OUTPUT_HANZI_ALL;
	SaveConfigInternal(pim_config);
	context->modify_flag |= MODIFY_STATUS;
}

/**	�趨Ϊ˫ƴ
 */
void SetToShuangPin(PIMCONTEXT *context)
{
	Log(LOG_ID, L"�趨Ϊ˫ƴ");
	if (pim_config->pinyin_mode == PINYIN_SHUANGPIN)
		return;

	pim_config->pinyin_mode = PINYIN_SHUANGPIN;
	SaveConfigInternal(pim_config);
	context->modify_flag |= MODIFY_STATUS;
}

/**	�趨���뷽ʽ
 */
void SetInputStyle(PIMCONTEXT *context, int style)
{
	Log(LOG_ID, L"�趨������:%d", style);
	if (pim_config->input_style == style)
		return;

	pim_config->input_style = style;
	SaveConfigInternal(pim_config);
	context->modify_flag |= MODIFY_STATUS;
}

/**	�趨Ϊȫƴ
 */
void SetToQuanPin(PIMCONTEXT *context)
{
	Log(LOG_ID, L"�趨Ϊȫƴ");
	if (pim_config->pinyin_mode == PINYIN_QUANPIN)
		return;

	pim_config->pinyin_mode = PINYIN_QUANPIN;
	SaveConfigInternal(pim_config);
	context->modify_flag |= MODIFY_STATUS;
}

/**	�趨���ܡ���������ϼ�����
 */
void SetCombineKeyMode(PIMCONTEXT *context, int yes)
{
	pim_config->disable_combined_key = !yes;
	SaveConfigInternal(pim_config);
}

/*	��ת�����뷱��
 */
void ToggleFanJian(PIMCONTEXT *context)
{
	int mode;

	if (!context || !pim_config)
		return;

	Log(LOG_ID, L"��ת�򡢷���ȫ��");

	mode = pim_config->hz_output_mode;
	if (mode & HZ_OUTPUT_HANZI_ALL)
		mode = HZ_OUTPUT_SIMPLIFIED;
	else if (mode & HZ_OUTPUT_SIMPLIFIED)
		mode = HZ_OUTPUT_TRADITIONAL;
	else
		mode = HZ_OUTPUT_HANZI_ALL;

	if (pim_config)
	{
		pim_config->hz_output_mode = mode;
		SaveConfigInternal(pim_config);
	}

	context->modify_flag |= MODIFY_STATUS;
}

/*	��תӢ�ġ����ķ���
 */
void ToggleEnglishSymbol(PIMCONTEXT *context)
{
	Log(LOG_ID, L"��תӢ�ġ����ķ���");

	//�Ӵ�if��Ϊ�˽������bug��
	//0.��ʼ״̬������+���ı��
	//1.��Shift�л�ΪӢ�ģ������㰴ť����Shift�л�Ϊ���ģ�
	//��ʱ״̬������+Ӣ�ı��
	//2.��Shift�л�ΪӢ�ģ������㰴ť����Shift�л�Ϊ���ģ�
	//��ʱ״̬ʱ����+���ı��
	//2��3������ͬ�������ȴ��һ��
	if (context->input_mode & CHINESE_MODE)
	{
		pim_config->hz_option ^= HZ_SYMBOL_CHINESE;
	}

	SaveConfigInternal(pim_config);
	context->modify_flag |= MODIFY_STATUS;
}

/*	��ת��Ӣ������
 */
void ToggleChineseMode(PIMCONTEXT *context)
{
	Log(LOG_ID, L"��תӢ�ġ���������");
	if (context->input_mode & CHINESE_MODE)
		context->input_mode = ENGLISH_MODE;
	else
		context->input_mode = CHINESE_MODE;

	SaveConfigInternal(pim_config);

	context->ui_context->have_caret = -1;
	context->modify_flag |= MODIFY_ENDCOMPOSE;
	context->modify_flag |= MODIFY_STATUS;
	ResetContext(context);

	CheckHintState(context, context->ui_context);
}

/*	�����ѡ��Ϣ
 */
void ClearCandidateInfo(LPINPUTCONTEXT pIMC, PIMCONTEXT *context)
{
	LPCANDIDATEINFO candidate_info;
	LPCANDIDATELIST candidate_list;

	if (0 != pIMC && 0 != (candidate_info = (LPCANDIDATEINFO)ImmLockIMCC(pIMC->hCandInfo)) &&
		0 != (candidate_list = (LPCANDIDATELIST)((char*)candidate_info + candidate_info->dwOffset[0])))
	{
		candidate_list->dwCount		= 0;
		candidate_list->dwSelection = 0;
		candidate_list->dwPageSize	= 0;
		candidate_list->dwPageStart = 0;
	}

	if (0 != context)
	{
		context->candidate_count		  = 0;
		context->candidate_index		  = 0;
		context->candidate_selected_index = -1;
	}

	if (candidate_info)
		ImmUnlockIMCC(pIMC->hCandInfo);
}

/*	���д����Ϣ
 */
void ClearComposeInfo(LPINPUTCONTEXT pIMC, PIMCONTEXT *context)
{
	LPCOMPOSITIONSTRING compose_string;

	if (0 != pIMC &&
		0 != (compose_string = (LPCOMPOSITIONSTRING)ImmLockIMCC(pIMC->hCompStr)))
	{
		compose_string->dwCompStrLen = 0;
		compose_string->dwCursorPos  = 0;
	}

	if (0 != context)
	{
		context->compose_cursor_index = 0;
		context->compose_length		  = 0;
		context->compose_string[0]	  = 0;
	}

	if (compose_string)
		ImmUnlockIMCC(pIMC->hCompStr);
}

/**	�����������������ֽ�֮��Ĺ�ϵ
 */
void MakeSyllableStartPosition(PIMCONTEXT *context)
{
	int i, index = 0;
	TCHAR pinyin[0x10];

	for (i = 0; i < context->syllable_count; i++)
	{
		context->syllable_start_pos[i] = index;

		if (pim_config->pinyin_mode == PINYIN_SHUANGPIN)
			index += GetSyllableStringSP(context->syllables[i], pinyin, _SizeOf(pinyin));
		else
			index += GetSyllableString(context->syllables[i], pinyin, _SizeOf(pinyin), /*context->syllable_correct_flag[i],*/ 0);

		if (context->input_string[index] == SYLLABLE_SEPARATOR_CHAR)
			index++;
	}

	context->syllable_start_pos[i] = index;
}

/**	����û�ѡ�����д�������ַ���
 */
TCHAR *GetSelectedComposeString(PIMCONTEXT *context)
{
	static TCHAR string[MAX_COMPOSE_LENGTH + 4];

	_tcscpy_s(string, _SizeOf(string), context->selected_compose_string);
	return string;
}

/**	��õ�ǰ�ĺ�ѡ�ַ���
 */
TCHAR *GetCurrentCandidateString(PIMCONTEXT *context)
{
	static TCHAR string[MAX_COMPOSE_LENGTH + 4];
	int cand_index = context->candidate_selected_index + context->candidate_index;

	GetCandidateDisplayString(context, &context->candidate_array[cand_index], string, MAX_COMPOSE_LENGTH / 2, 0);
	return string;
}

/** ��õ�ǰʣ������ڴ�
 */
TCHAR *GetReaminComposeString(PIMCONTEXT *context)
{
	static TCHAR string[MAX_COMPOSE_LENGTH + 4];
	int cand_index = context->candidate_selected_index + context->candidate_index;
	SYLLABLE syllables[MAX_SYLLABLE_PER_INPUT];
	TCHAR py[0x10];
	int syllable_length, i;

	string[0] = 0;
	//��Ҫ�ж��Ƿ���ʣ������ڣ������жϷ��������Ľ����������ԭʼ����ͬ��
	if (context->candidate_array[cand_index].type == CAND_TYPE_CI &&
		context->candidate_array[cand_index].word.type == CI_TYPE_OTHER)
		syllable_length = context->candidate_array[cand_index].word.origin_syllable_length;
	else
		syllable_length = GetCandidateSyllable(&context->candidate_array[cand_index], syllables, sizeof(syllables) / sizeof(syllables[0]));

	if (!syllable_length)	//SPW����Ҫ�ٴμ���
		return string;

	for (i = context->syllable_pos + syllable_length; i < context->syllable_count; i++)
	{
		if (_tcslen(string) > _SizeOf(string) - 8)
			break;

		GetSyllableString(context->syllables[i], py, _SizeOf(py), /*context->syllable_correct_flag[i],*/ 0);
		if (i != context->syllable_pos + syllable_length)
			_tcscat_s(string, _SizeOf(string), TEXT("'"));

		_tcscat_s(string, _SizeOf(string), py);
	}

	return string;
}

/**	��ʾ������״̬����
 */
void ToggleShowStatusWindow(PIMCONTEXT *context)
{

	if (!context || !context->ui_context)
		return;

	pim_config->show_status_window = !pim_config->show_status_window;
	Log(LOG_ID, L"��ʾ״̬��:%d", pim_config->show_status_window);

	if (!pim_config->show_status_window)
		HideStatusWindow(context, context->ui_context);

	SaveConfigInternal(pim_config);
	PostMessage(context->ui_context->ui_window, UIM_MODE_CHANGE, 0, 0);
}

/*	�л���Ӣ������ģʽ
 */
void ToggleEnglishInput(PIMCONTEXT *context)
{
	if (!context || !context->ui_context || !pim_config)
		return;

	Log(LOG_ID, L"�л�Ӣ������ģʽ");

	if (ENGLISH_STATE_INPUT == context->english_state)
		context->english_state = ENGLISH_STATE_NONE;
	else
		context->english_state = ENGLISH_STATE_INPUT;

	PostMessage(context->ui_context->ui_window, UIM_ENGLISH_INPUT, 0, 0);
}

/*	�л�Ӣ�ĺ�ѡģʽ
 */
void ToggleEnglishCandidate(PIMCONTEXT *context)
{
	if (!context || !context->ui_context || !pim_config)
		return;

	Log(LOG_ID, L"�л�Ӣ�ĺ�ѡģʽ");

	if (ENGLISH_STATE_CAND == context->english_state)
		context->english_state = ENGLISH_STATE_NONE;
	else
		context->english_state = ENGLISH_STATE_CAND;

	PostMessage(context->ui_context->ui_window, UIM_ENGLISH_INPUT, 0, 0);
}

//���ú�ѡ��ʾģʽ
void SetCandidatesViewMode(PIMCONTEXT *context)
{
	int view_mode = VIEW_MODE_HORIZONTAL;

	switch (pim_config->input_style)
	{
	case STYLE_CSTAR:
		//��Ӣ��ģʽ
		if (context->english_state == ENGLISH_STATE_NONE)
		{
			//uģʽ or iģʽ ����
			if (STATE_UINPUT == context->state || STATE_IINPUT == context->state)
				view_mode = VIEW_MODE_VERTICAL;
			//������ʾ��ѡ
			else if (pim_config->show_vertical_candidate)
				view_mode = VIEW_MODE_VERTICAL;
			//ǿ��������ʾ��ѡ
			else if (context->force_vertical)
				view_mode = VIEW_MODE_VERTICAL;
			//��չ
			else if (pim_config->use_tab_expand_candidates && context->expand_candidate)
				view_mode = VIEW_MODE_EXPAND;
			else
				view_mode = VIEW_MODE_HORIZONTAL;
		}
		else
		{
			//Ӣ������
			view_mode = VIEW_MODE_VERTICAL;
		}

		break;

	case STYLE_ABC:
		view_mode = VIEW_MODE_VERTICAL;
		break;
	}

	context->candidates_view_mode = view_mode;
}

int CanSwitchToExpandMode(PIMCONTEXT *context)
{
	if (STYLE_CSTAR == pim_config->input_style &&					//����֮��
		VIEW_MODE_HORIZONTAL == context->candidates_view_mode &&	//��ǰ����
		pim_config->use_tab_expand_candidates &&					//������չ
		pim_config->theme_use_for_expand_mode &&					//����������չ
		!IsFullScreen())
		return 1;
	else
		return 0;
}