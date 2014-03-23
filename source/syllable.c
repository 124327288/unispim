/*	�������ں����顣
 */
#include <config.h>
#include <utility.h>
#include <assert.h>
#include <zi.h>
#include <tchar.h>
#include <share_segment.h>

//#pragma data_seg(HYPIM_SHARED_SEGMENT)

//int sp_used_u = 1;			//˫ƴ�Ƿ�ʹ��u��Ϊ��ͷ����ĸ
//int sp_used_v = 0;			//˫ƴ�Ƿ�ʹ��V��Ϊ��ͷ����ĸ
//int sp_used_i = 0;			//˫ƴ�Ƿ�ʹ��I��Ϊ��ͷ����ĸ
//int sp_loaded = 0;

//˫ƴ��ĸ��
//TCHAR con_sp_string_save[][8] =
//{
//	TEXT(""),						//CON_NULL
//	TEXT("b"),						//CON_B
//	TEXT("c"),						//CON_C
//	TEXT("ch"),						//CON_CH
//	TEXT("d"),						//CON_D
//	TEXT("f"),						//CON_F
//	TEXT("g"),						//CON_G
//	TEXT("h"),						//CON_H
//	TEXT("j"),						//CON_J
//	TEXT("k"),						//CON_K
//	TEXT("l"),						//CON_L
//	TEXT("m"),						//CON_M
//	TEXT("n"),						//CON_N
//	TEXT("p"),						//CON_P
//	TEXT("q"),						//CON_Q
//	TEXT("r"),						//CON_R
//	TEXT("s"),						//CON_S
//	TEXT("sh"),						//CON_SH
//	TEXT("t"),						//CON_T
//	TEXT("w"),						//CON_W
//	TEXT("x"),						//CON_X
//	TEXT("y"),						//CON_Y
//	TEXT("z"),						//CON_Z
//	TEXT("zh"),						//CON_ZH
//	TEXT(""),						//CON_END
//	TEXT("*"),						//CON_ANY
//};
//TCHAR con_sp_string[sizeof(con_sp_string_save) / _SizeOf(con_sp_string_save[0]) / sizeof(TCHAR)][8] = { 0 };

//˫ƴ��ĸ��
//TCHAR vow_sp_string_save[][8] =
//{
//	TEXT(""),						//VOW_NULL,
//	TEXT("a"),						//VOW_A,
//	TEXT("ai"),						//VOW_AI
//	TEXT("an"),						//VOW_AN
//	TEXT("ang"),					//VOW_ANG
//	TEXT("ao"),						//VOW_AO
//	TEXT("e"),						//VOW_E
//	TEXT("ei"),						//VOW_EI
//	TEXT("en"),						//VOW_EN
//	TEXT("eng"),					//VOW_ENG
//	TEXT("er"),						//VOW_ER
//	TEXT("i"),						//VOW_I
//	TEXT("ia"),						//VOW_IA
//	TEXT("ian"),					//VOW_IAN
//	TEXT("iang"),					//VOW_IANG
//	TEXT("iao"),					//VOW_IAO
//	TEXT("ie"),						//VOW_IE
//	TEXT("in"),						//VOW_IN
//	TEXT("ing"),					//VOW_ING
//	TEXT("iong"),					//VOW_IONG
//	TEXT("iu"),						//VOW_IU
//	TEXT("o"),						//VOW_O
//	TEXT("ong"),					//VOW_ONG
//	TEXT("ou"),						//VOW_OU
//	TEXT("u"),						//VOW_U
//	TEXT("ua"),						//VOW_UA
//	TEXT("uai"),					//VOW_UAI
//	TEXT("uan"),					//VOW_UAN
//	TEXT("uang"),					//VOW_UANG
//	TEXT("ue"),						//VOW_UE
//	TEXT("ui"),						//VOW_UI
//	TEXT("un"),						//VOW_UN
//	TEXT("uo"),						//VOW_UO
//	TEXT("v"),						//VOW_V
//	TEXT(""),						//VOW_END
//	TEXT(""),						//VOW_ANY
//};
//TCHAR vow_sp_string[sizeof(vow_sp_string_save) / _SizeOf(vow_sp_string_save[0]) / sizeof(TCHAR)][8] = { 0 };

//˫ƴ���ڱ�
//TCHAR vow_sp_string_single_save[][8] =
//{
//	TEXT(""),						//VOW_NULL,
//	TEXT("a"),						//VOW_A,
//	TEXT("ai"),						//VOW_AI
//	TEXT("an"),						//VOW_AN
//	TEXT("ang"),					//VOW_ANG
//	TEXT("ao"),						//VOW_AO
//	TEXT("e"),						//VOW_E
//	TEXT("ei"),						//VOW_EI
//	TEXT("en"),						//VOW_EN
//	TEXT("eng"),					//VOW_ENG
//	TEXT("er"),						//VOW_ER
//	TEXT("i"),						//VOW_I
//	TEXT("ia"),						//VOW_IA
//	TEXT("ian"),					//VOW_IAN
//	TEXT("iang"),					//VOW_IANG
//	TEXT("iao"),					//VOW_IAO
//	TEXT("ie"),						//VOW_IE
//	TEXT("in"),						//VOW_IN
//	TEXT("ing"),					//VOW_ING
//	TEXT("iong"),					//VOW_IONG
//	TEXT("iu"),						//VOW_IU
//	TEXT("o"),						//VOW_O
//	TEXT("ong"),					//VOW_ONG
//	TEXT("ou"),						//VOW_OU
//	TEXT("u"),						//VOW_U
//	TEXT("ua"),						//VOW_UA
//	TEXT("uai"),					//VOW_UAI
//	TEXT("uan"),					//VOW_UAN
//	TEXT("uang"),					//VOW_UANG
//	TEXT("ue"),						//VOW_UE
//	TEXT("ui"),						//VOW_UI
//	TEXT("un"),						//VOW_UN
//	TEXT("uo"),						//VOW_UO
//	TEXT("v"),						//VOW_V
//	TEXT(""),						//VOW_END
//	TEXT(""),						//VOW_ANY
//};
//TCHAR vow_sp_string_single[sizeof(vow_sp_string_single_save) / _SizeOf(vow_sp_string_single_save[0]) / sizeof(TCHAR)][8] = { 0 };

//#pragma data_seg()

/*	ƴ����������
	����������
	e��������
	i��������
	o��������
	u��������
	����������
	���@���A
*/

//SPECIALSYLLABLE special_syllables[] =
//{
//	//wa'nan->wan'an
//	2, CON_W,  VOW_A,    TONE_0, CON_N,    VOW_AN,  TONE_0, CON_NULL, VOW_NULL, TONE_0, CON_NULL, VOW_NULL, TONE_0, CON_NULL, VOW_NULL, TONE_0, 
//	2, CON_W,  VOW_AN,   TONE_0, CON_NULL, VOW_AN,  TONE_0, CON_NULL, VOW_NULL, TONE_0, CON_NULL, VOW_NULL, TONE_0, CON_NULL, VOW_NULL, TONE_0, 
//
//	//nian'pai->ni'an'pai
//	2, CON_N,  VOW_IAN,  TONE_0, CON_P,    VOW_AI,  TONE_0, CON_NULL, VOW_NULL, TONE_0, CON_NULL, VOW_NULL, TONE_0, CON_NULL, VOW_NULL, TONE_0, 
//	3, CON_N,  VOW_I,    TONE_0, CON_NULL, VOW_AN,  TONE_0, CON_P,    VOW_AI,   TONE_0, CON_NULL, VOW_NULL, TONE_0, CON_NULL, VOW_NULL, TONE_0, 
//
//	//la'qiao->la'qi'ao
//	//2, CON_L,  VOW_A,    TONE_0, CON_Q,    VOW_IAO, TONE_0, CON_NULL, VOW_NULL, TONE_0, CON_NULL, VOW_NULL, TONE_0, CON_NULL, VOW_NULL, TONE_0,
//	//3, CON_L,  VOW_A,    TONE_0, CON_Q,	   VOW_I,   TONE_0, CON_NULL, VOW_AO,   TONE_0, CON_NULL, VOW_NULL, TONE_0, CON_NULL, VOW_NULL, TONE_0, 
//
//	//xi'nan'zhuang->xin'an'zhuang
//	3, CON_X,  VOW_I,    TONE_0, CON_N,    VOW_AN,  TONE_0, CON_ZH,   VOW_UANG, TONE_0, CON_NULL, VOW_NULL, TONE_0, CON_NULL, VOW_NULL, TONE_0, 
//	3, CON_X,  VOW_IN,   TONE_0, CON_NULL, VOW_AN,  TONE_0, CON_ZH,   VOW_UANG, TONE_0, CON_NULL, VOW_NULL, TONE_0, CON_NULL, VOW_NULL, TONE_0, 
//
//	//shan'xian'kang->shan'xi'an'kang
//	3, CON_SH, VOW_AN,   TONE_0, CON_X,    VOW_IAN, TONE_0, CON_K,    VOW_ANG,  TONE_0, CON_NULL, VOW_NULL, TONE_0, CON_NULL, VOW_NULL, TONE_0, 
//	4, CON_SH, VOW_AN,   TONE_0, CON_X,    VOW_I,   TONE_0, CON_NULL, VOW_AN,   TONE_0, CON_K,    VOW_ANG,  TONE_0, CON_NULL, VOW_NULL, TONE_0, 
//
//	//gong'suan'jian->gong'su'an'jian
//	3, CON_G,  VOW_ONG,  TONE_0, CON_S,    VOW_UAN, TONE_0, CON_J,    VOW_IAN,  TONE_0, CON_NULL, VOW_NULL, TONE_0, CON_NULL, VOW_NULL, TONE_0, 
//	4, CON_G,  VOW_ONG,  TONE_0, CON_S,    VOW_U,   TONE_0, CON_NULL, VOW_AN,   TONE_0, CON_J,	  VOW_IAN,  TONE_0, CON_NULL, VOW_NULL, TONE_0, 
//
//	//qi'suan'jian->qi'su'an'jian
//	3, CON_Q,  VOW_I,    TONE_0, CON_S,    VOW_UAN, TONE_0, CON_J,    VOW_IAN,  TONE_0, CON_NULL, VOW_NULL, TONE_0, CON_NULL, VOW_NULL, TONE_0, 
//	4, CON_Q,  VOW_I,    TONE_0, CON_S,    VOW_U,   TONE_0, CON_NULL, VOW_AN,   TONE_0, CON_J,	  VOW_IAN,  TONE_0, CON_NULL, VOW_NULL, TONE_0, 
//
//	//qi'suan'jian->qi'su'an'jian
//	3, CON_SH, VOW_ANG,  TONE_0, CON_S,    VOW_UAN, TONE_0, CON_J,    VOW_IAN,  TONE_0, CON_NULL, VOW_NULL, TONE_0, CON_NULL, VOW_NULL, TONE_0, 
//	4, CON_SH, VOW_ANG,  TONE_0, CON_S,    VOW_U,   TONE_0, CON_NULL, VOW_AN,   TONE_0, CON_J,	  VOW_IAN,  TONE_0, CON_NULL, VOW_NULL, TONE_0, 
//
//	//sui'yue'ran->sui'yu'er'an
//	3, CON_S,  VOW_UI,   TONE_0, CON_Y,    VOW_UE,  TONE_0, CON_R,    VOW_AN,   TONE_0, CON_NULL, VOW_NULL, TONE_0, CON_NULL, VOW_NULL, TONE_0, 
//	4, CON_S,  VOW_UI,   TONE_0, CON_Y,    VOW_U,   TONE_0, CON_NULL, VOW_ER,   TONE_0, CON_NULL, VOW_AN,   TONE_0, CON_NULL, VOW_NULL, TONE_0, 
//
//	//xin'gao'qiao->xin'gao'qi'ao
//	3, CON_X,  VOW_IN,   TONE_0, CON_G,    VOW_AO,  TONE_0, CON_Q,    VOW_IAO,  TONE_0, CON_NULL, VOW_NULL, TONE_0, CON_NULL, VOW_NULL, TONE_0, 
//	4, CON_X,  VOW_IN,   TONE_0, CON_G,    VOW_AO,  TONE_0, CON_Q,    VOW_I,    TONE_0, CON_NULL, VOW_AO,   TONE_0, CON_NULL, VOW_NULL, TONE_0, 
//
//	//xiang'gua'nan'li->xiang'guan'an'li
//	4, CON_X,  VOW_IANG, TONE_0, CON_G,    VOW_UA,  TONE_0, CON_N,    VOW_AN,   TONE_0, CON_L,    VOW_I,    TONE_0, CON_NULL, VOW_NULL, TONE_0, 
//	4, CON_X,  VOW_IANG, TONE_0, CON_G,    VOW_UAN, TONE_0, CON_NULL, VOW_AN,   TONE_0, CON_L,    VOW_I,    TONE_0, CON_NULL, VOW_NULL, TONE_0, 
//
//	//xin'xian'quan->xin'xi'an'quan
//	3, CON_X,  VOW_IN,	 TONE_0, CON_X,	   VOW_IAN, TONE_0, CON_Q,	  VOW_UAN,  TONE_0, CON_NULL, VOW_NULL, TONE_0, CON_NULL, VOW_NULL, TONE_0, 
//	4, CON_X,  VOW_IN,	 TONE_0, CON_X,	   VOW_I,   TONE_0, CON_NULL, VOW_AN,   TONE_0, CON_Q,	  VOW_UAN,  TONE_0, CON_NULL, VOW_NULL, TONE_0, 
//
//	//zhen'zhen'gai'ni->zhen'zheng'ai'ni
//	4, CON_ZH, VOW_EN,   TONE_0, CON_ZH,   VOW_EN,  TONE_0, CON_G,    VOW_AI,   TONE_0, CON_N,    VOW_I,    TONE_0, CON_NULL, VOW_NULL, TONE_0, 
//	4, CON_ZH, VOW_EN,   TONE_0, CON_ZH,   VOW_ENG, TONE_0, CON_NULL, VOW_AI,   TONE_0, CON_N,    VOW_I,    TONE_0, CON_NULL, VOW_NULL, TONE_0, 
//
//	//zhen'gan'bu'jiu'ban->zheng'an'bu'jiu'ban
//	5, CON_ZH, VOW_EN,   TONE_0, CON_G,    VOW_AN,  TONE_0, CON_B,    VOW_U,    TONE_0, CON_J,    VOW_IU,   TONE_0, CON_B,    VOW_AN,   TONE_0, 
//	5, CON_ZH, VOW_ENG,  TONE_0, CON_NULL, VOW_AN,  TONE_0, CON_B,    VOW_U,    TONE_0, CON_J,    VOW_IU,   TONE_0, CON_B,    VOW_AN,   TONE_0, 
//};

typedef struct tagSMALLSYLLABLEITEM
{
	SYLLABLE syllable;
	SYLLABLE small_syllables[2];
}SMALLSYLLABLEITEM;

//�ƺ���δ���������������shuang->shu'ang�����Կ�����������
static SMALLSYLLABLEITEM small_syllable_items[] =
{
	//ao -> a'o
	{ { CON_NULL, VOW_AO, },{ { CON_NULL, VOW_A, },	{ CON_NULL, VOW_O,  },  }, },
	//bian -> bi'an
	{ { CON_B, VOW_IAN, },	{ { CON_B, VOW_I, },	{ CON_NULL, VOW_AN, },  }, },
	//biao -> bi'ao
	{ { CON_B, VOW_IAO, },	{ { CON_B, VOW_I, },	{ CON_NULL, VOW_AO, },  }, },
	//cuan -> cu'an
	{ { CON_C, VOW_UAN, },	{ { CON_C, VOW_U, },	{ CON_NULL, VOW_AN, },  }, },
	//dia -> di'a
	{ { CON_D, VOW_IA, },	{ { CON_D, VOW_I, },	{ CON_NULL, VOW_A, },   }, },
	//dian -> di'an
	{ { CON_D, VOW_IAN, },	{ { CON_D, VOW_I, },	{ CON_NULL, VOW_AN, },  }, },
	//diao -> di'ao
	{ { CON_D, VOW_IAO, },	{ { CON_D, VOW_I, },	{ CON_NULL, VOW_AO, },	}, },
	//die -> di'e
	{ { CON_D, VOW_IE, },	{ { CON_D, VOW_I, },	{ CON_NULL, VOW_E, },	}, },
	//duan -> du'an
	{ { CON_D, VOW_UAN, },	{ { CON_D, VOW_U, },	{ CON_NULL, VOW_AN, },	}, },
	//gua -> gu'a
	{ { CON_G, VOW_UA, },	{ { CON_G, VOW_U, },	{ CON_NULL, VOW_A, },	}, },
	//guai -> gu'ai
	{ { CON_G, VOW_UAI, },	{ { CON_G, VOW_U, },	{ CON_NULL, VOW_AI, },	}, },
	//guan -> gu'an
	{ { CON_G, VOW_UAN, },	{ { CON_G, VOW_U, },	{ CON_NULL, VOW_AN, },	}, },
	//huan -> hu'a
	{ { CON_H, VOW_UA, },	{ { CON_H, VOW_U, },	{ CON_NULL, VOW_A, },	}, },
	//huai -> hu'ai
	{ { CON_H, VOW_UAI, },	{ { CON_H, VOW_U, },	{ CON_NULL, VOW_AI, },	}, },
	//huan -> hu'an
	{ { CON_H, VOW_UAN, },	{ { CON_H, VOW_U, },	{ CON_NULL, VOW_AN, },	}, },
	//huo -> hu'o
	{ { CON_H, VOW_UO, },	{ { CON_H, VOW_U, },	{ CON_NULL, VOW_O, },	}, },
	//jia -> ji'a
	{ { CON_J, VOW_IA, },	{ { CON_J, VOW_I, },	{ CON_NULL, VOW_A, },	}, },
	//jian -> ji'an
	{ { CON_J, VOW_IAN, },	{ { CON_J, VOW_I, },	{ CON_NULL, VOW_AN, },	}, },
	//jiang -> ji'ang
	{ { CON_J, VOW_IANG, }, { { CON_J, VOW_I, },	{ CON_NULL, VOW_ANG, }, }, },
	//jiao -> ji'ao
	{ { CON_J, VOW_IAO, },  { { CON_J, VOW_I, },	{ CON_NULL, VOW_AO, },  }, },
	//jie -> ji'e
	{ { CON_J, VOW_IE,  },	{ { CON_J, VOW_I, },	{ CON_NULL, VOW_E,  },  }, },
	//jue -> ju'e
	{ { CON_J, VOW_UE,  },	{ { CON_J, VOW_U, },	{ CON_NULL, VOW_E,  },  }, },
	//juan -> ju'an
	{ { CON_J, VOW_UAN, },	{ { CON_J, VOW_U, },	{ CON_NULL, VOW_AN, },	}, },
	//kua -> ku'a
	{ { CON_K, VOW_UA,  },  { { CON_K, VOW_U, },    { CON_NULL, VOW_A,  },  }, },
	//kuai -> ku'ai
	{ { CON_K, VOW_UAI,  }, { { CON_K, VOW_U, },	{ CON_NULL, VOW_AI,  }, }, },
	//kuo -> ku'o
	{ { CON_K, VOW_UO,  },  { { CON_K, VOW_U, },	{ CON_NULL, VOW_O,  },  }, },
	//lia -> li'a
	{ { CON_L, VOW_IA, },	{ { CON_L, VOW_I, },	{ CON_NULL, VOW_A, },  },  },
	//lian -> li'an
	{ { CON_L, VOW_IAN, },	{ { CON_L, VOW_I, },	{ CON_NULL, VOW_AN, },  }, },
	//liang -> li'ang
	{ { CON_L, VOW_IANG, }, { { CON_L, VOW_I, },	{ CON_NULL, VOW_ANG, }, }, },
	//liao -> li'ao
	{ { CON_L, VOW_IAO, },	{ { CON_L, VOW_I, },	{ CON_NULL, VOW_AO, },  }, },
	//lie -> li'e
	{ { CON_L, VOW_IE, },	{ { CON_L, VOW_I, },	{ CON_NULL, VOW_E, },  },  },
	//luan -> lu'an
	{ { CON_L, VOW_UAN, },	{ { CON_L, VOW_U, },	{ CON_NULL, VOW_AN, },  }, },
	//mian -> mi'an
	{ { CON_M, VOW_IAN, },	{ { CON_M, VOW_I, },	{ CON_NULL, VOW_AN, },  }, },
	//miao -> mi'ao
	{ { CON_M, VOW_IAO, },	{ { CON_M, VOW_I, },	{ CON_NULL, VOW_AO, },  }, },
	//mie -> mi'e
	{ { CON_M, VOW_IE, },	{ { CON_M, VOW_I, },	{ CON_NULL, VOW_E, },   }, },
	//nao -> na'o
	{ { CON_N, VOW_AO, },	{ { CON_N, VOW_A, },	{ CON_NULL, VOW_O, },  },  },
	//nian -> ni'an
	{ { CON_N, VOW_IAN, },	{ { CON_N, VOW_I, },	{ CON_NULL, VOW_AN, },  }, },
	//niao -> ni'ao
	{ { CON_N, VOW_IAO, },	{ { CON_N, VOW_I, },	{ CON_NULL, VOW_AO, },  }, },
	//nie -> ni'e
	{ { CON_N, VOW_IE, },	{ { CON_N, VOW_I, },	{ CON_NULL, VOW_E, },   }, },
	//pian -> pi'an
	{ { CON_P, VOW_IAN, },	{ { CON_P, VOW_I, },	{ CON_NULL, VOW_AN, },  }, },
	//piao -> pi'ao
	{ { CON_P, VOW_IAO, },	{ { CON_P, VOW_I, },	{ CON_NULL, VOW_AO, },  }, },
	//pie -> pi'e
	{ { CON_P, VOW_IE, },	{ { CON_P, VOW_I, },	{ CON_NULL, VOW_E, },   }, },
	//qia -> qi'a
	{ { CON_Q, VOW_IA,  }, { { CON_Q, VOW_I,  },	{ CON_NULL, VOW_A,  }, },  },
	//qian -> qi'an
	{ { CON_Q, VOW_IAN,  }, { { CON_Q, VOW_I, },	{ CON_NULL, VOW_AN,  }, }, },
	//qiang-> qi'ang
	{ { CON_Q, VOW_IANG, }, { { CON_Q, VOW_I, },	{ CON_NULL, VOW_ANG, }, }, },
	//qiao -> qi'ao
	{ { CON_Q, VOW_IAO,  }, { { CON_Q, VOW_I, },	{ CON_NULL, VOW_AO,  }, }, },
	//qie -> qi'e
	{ { CON_Q, VOW_IE,  },	{ { CON_Q, VOW_I, },	{ CON_NULL, VOW_E,  },  }, },
	//quan -> qu'an
	{ { CON_Q, VOW_UAN,  },	{ { CON_Q, VOW_U, },	{ CON_NULL, VOW_AN,  }, }, },
	//que -> qu'e
	{ { CON_Q, VOW_UE,  },	{ { CON_Q, VOW_U, },	{ CON_NULL, VOW_E,  },  }, },
	//suan -> su'an
	{ { CON_S, VOW_UAN, },  { { CON_S, VOW_U, },	{ CON_NULL, VOW_AN, },	}, },
	//shao -> sha'o
	{ { CON_SH, VOW_AO, },  { { CON_SH, VOW_A,},	{ CON_NULL, VOW_O, },	}, },
	//shua -> shu'a
	{ { CON_SH, VOW_UA, },  { { CON_SH, VOW_U,},	{ CON_NULL, VOW_A, },	}, },
	//shuan -> shu'an
	{ { CON_SH, VOW_UAN, }, { { CON_SH, VOW_U,},	{ CON_NULL, VOW_AN, },	}, },
	//shuo -> shu'o
	{ { CON_SH, VOW_UO, },  { { CON_SH, VOW_U,},	{ CON_NULL, VOW_O, },	}, },
	//tian -> ti'an
	{ { CON_T, VOW_IAN, },	{ { CON_T, VOW_I, },	{ CON_NULL, VOW_AN, },  }, },
	//tie -> ti'e
	{ { CON_T, VOW_IE, },	{ { CON_T, VOW_I, },	{ CON_NULL, VOW_E, },  },  },
	//tuan -> tu'an
	{ { CON_T, VOW_UAN, },	{ { CON_T, VOW_U, },	{ CON_NULL, VOW_AN, },  }, },
	//tuo -> tu'o
	{ { CON_T, VOW_UO, },	{ { CON_T, VOW_U, },	{ CON_NULL, VOW_O, },   }, },
	//xia -> xi'a
	{ { CON_X, VOW_IA, },	{ { CON_X, VOW_I, },	{ CON_NULL, VOW_A, },  },  },
	//xian -> xi'an
	{ { CON_X, VOW_IAN, },	{ { CON_X, VOW_I, },	{ CON_NULL, VOW_AN, },  }, },
	//xiao -> xi'ao
	{ { CON_X, VOW_IAO, },	{ { CON_X, VOW_I, },	{ CON_NULL, VOW_AO, },  }, },
	//xiang -> xi'ang
	{ { CON_X, VOW_IANG, }, { { CON_X, VOW_I, },	{ CON_NULL, VOW_ANG, }, }, },
	//xie -> xi'e
	{ { CON_X, VOW_IE, },	{ { CON_X, VOW_I, },	{ CON_NULL, VOW_E, },  },  },
	//xuan -> xu'an
	{ { CON_X, VOW_UAN, },  { { CON_X, VOW_U, },	{ CON_NULL, VOW_AN, },  }, },
	//xue -> xu'e
	{ { CON_X, VOW_UE, },   { { CON_X, VOW_U, },	{ CON_NULL, VOW_E, },   }, },
	//yao -> ya'o
	{ { CON_Y, VOW_AO, },	{ { CON_Y, VOW_A, },	{ CON_NULL, VOW_O, },   }, },
	//yuan -> yu'an
	{ { CON_Y, VOW_UAN, },	{ { CON_Y, VOW_U, },	{ CON_NULL, VOW_AN, },  }, },
	//yue -> yu'e
	{ { CON_Y, VOW_UE, },	{ { CON_Y, VOW_U, },	{ CON_NULL, VOW_E, },   }, },
	//zao -> za'o
	{ { CON_Z, VOW_AO, },	{ { CON_Z, VOW_A, },	{ CON_NULL, VOW_O, },  },  },
	//zuan -> zu'an
	{ { CON_Z, VOW_UAN, },	{ { CON_Z, VOW_U, },	{ CON_NULL, VOW_AN, },  }, },
	//zhao -> zha'o
	{ { CON_ZH, VOW_AO, },	{ { CON_ZH, VOW_A,},	{ CON_NULL, VOW_O, },   }, },
	//zhua -> zhu'a
	{ { CON_ZH, VOW_UA, },	{ { CON_ZH, VOW_U,},	{ CON_NULL, VOW_A, },   }, },
	//zhuan -> zhu'an
	{ { CON_ZH, VOW_UAN, },	{ { CON_ZH, VOW_U,},	{ CON_NULL, VOW_AN, },  }, },
};

//�󲿷�˫ƴ����������ͬһ������Ӧ������ͬ��ĸ���������ͨ����������ĸ���ܺ�ͬһ����ĸ���
//���ڣ���˴�˫ƴ�����ڵĽ��������Ψһ�ġ���Ҳ��һЩ˫ƴ����(������˫ƴ)ͬһ����(k)��Ӧ��
//������ĸ(en��ia)���Ժ�ͬһ����ĸ�������(den��dia)������˫ƴ�����ڵĽ��������Ψһ��Ŀǰ
//����Ľ�������ParsePinYinStringReverse�����㷵�ض��ֽ���������Ķ��������۹��󣬿��ǵ���
//һ�������˫ƴ�����У����ֽ��������Ψһ�����Ӧ�������������Խ�����Ϊһ�������ģ������
//����den->dia��ģ��(��Ŀǰ�㷨���ǽ�����den��������õ���ģ������)������ͬ��������е�
//wang->huangģ��һ����ÿһ��˫ƴƴ��������Ϊsp_fuzzy_syllables�е�һ��(���32��)�����ƴ��
//���ڶ��ֽ���������Ϊģ������ÿ�����֧��4��ģ����
#define MAX_SP_FUZZY_SYLLABLE_COUNT    4

typedef struct tagSPFUZZYMAPITEM 
{
	SYLLABLE sp_fuzzy_syllables[MAX_SP_FUZZY_SYLLABLE_COUNT];    //�໥ģ��������
	int      syllable_map_index[MAX_SP_FUZZY_SYLLABLE_COUNT];    //��ǰ������share_segment->syllable_map�е��±�
	int      sp_fuzzy_syllable_count;                            //fuzzy_syllable��ʵ�����ڸ���
} SPFUZZYMAPITEM;

SPFUZZYMAPITEM sp_fuzzy_map[MAX_SYLLABLE_PER_INPUT];
int sp_fuzzy_map_items;

/*	�жϵ�һ�����ڲ�����ָʾ����ĸ�����Ƿ�����ڶ������ڲ�������ĸ��
 *	������
 *		syllable				Դ����
 *		checked_syllable		���������
 *		fuzzy_mode				ģ������
 *	���أ�
 *		1			����
 *		0			������
 */
int ContainCon(SYLLABLE syllable, SYLLABLE checked_syllable, int fuzzy_mode)
{
//��������д�����������ꡣ
#define	CHECK_FUZZY(fuzzy_bit, con_value)	((fuzzy_mode & fuzzy_bit) && (checked_syllable.con == con_value))

	if (syllable.con == checked_syllable.con)
		return 1;

	if (syllable.con == CON_ANY)
		return 1;

	//��������ĸΪ����Ϊ������
	if (checked_syllable.con == CON_NULL)
		return 0;

	switch(syllable.con)
	{
	case CON_Z:
		if (syllable.vow == VOW_NULL &&	CHECK_FUZZY(FUZZY_ZCS_IN_CI, CON_ZH))
			return 1;

		return CHECK_FUZZY(FUZZY_Z_ZH, CON_ZH);

	case CON_ZH:
		return CHECK_FUZZY(FUZZY_REV_Z_ZH, CON_Z);

	case CON_C:
		if (syllable.vow == VOW_NULL &&	CHECK_FUZZY(FUZZY_ZCS_IN_CI, CON_CH))
			return 1;

		return CHECK_FUZZY(FUZZY_C_CH, CON_CH);

	case CON_CH:
		return CHECK_FUZZY(FUZZY_REV_C_CH, CON_C);

	case CON_S:
		if (syllable.vow == VOW_NULL &&	CHECK_FUZZY(FUZZY_ZCS_IN_CI, CON_SH))
			return 1;

		return CHECK_FUZZY(FUZZY_S_SH, CON_SH);

	case CON_SH:
		return CHECK_FUZZY(FUZZY_REV_S_SH, CON_S);

	case CON_G:
		return CHECK_FUZZY(FUZZY_G_K, CON_K);

	case CON_K:
		return CHECK_FUZZY(FUZZY_REV_G_K, CON_G);

	case CON_F:		//����F��H��HU������������ģ��������Ҫ���⴦��
		if (CHECK_FUZZY(FUZZY_F_H, CON_H))		//���F��Hģ�������ұ������ĸΪH����Ϊ����
			return 1;

		//ģ��F��HU��Ϊ���ܹ�ʹ�����ϡ�����ͨ��"fn"�鵽������ٶ�f = h��������ĸ�����жϡ�
		return (fuzzy_mode & FUZZY_F_HU) && IS_START_WITH_HU(checked_syllable);

	case CON_H:		//��CON_Fͬ����������Ҫ���⴦��
		if (CHECK_FUZZY(FUZZY_REV_F_H, CON_F))
			return 1;

		//����������ĸ����F����û������F��HU��ģ����Ϊ������
		return ((fuzzy_mode & FUZZY_REV_F_HU) && IS_START_WITH_HU(syllable) && checked_syllable.con == CON_F);

	case CON_L:		//L������N��R�������໥ģ�����������⴦��
		if (CHECK_FUZZY(FUZZY_L_R, CON_R))		//L��R
			return 1;

		return CHECK_FUZZY(FUZZY_L_N, CON_N);	//L��N

	case CON_N:		//�����N-L/L-R��ӳ�䡣
		return CHECK_FUZZY(FUZZY_REV_L_N, CON_L);

	case CON_R:		//�����R-L/L-N��ӳ��
		return CHECK_FUZZY(FUZZY_REV_L_R, CON_L);
	}

	return 0;

//����ʹ����������
#undef	CHECK_FUZZY
}

/*	�ж�HUANG��WANG֮���ģ��
 *	������
 *		syllable				Դ����
 *		checked_syllable		���������
 *		fuzzy_mode				ģ������
 *	���أ�
 *		1			����
 *		0			������
 */
INLINE static int ProcessHUANGAndWANG(SYLLABLE syllable, SYLLABLE checked_syllable, int fuzzy_mode)
{
	if (!(fuzzy_mode & FUZZY_HUANG_WANG))
		return 0;

	//Դ��ĸ��Ŀ����ĸ������W��H
	return (syllable.con == CON_H && syllable.vow == VOW_UANG &&
			checked_syllable.con == CON_W && checked_syllable.vow == VOW_ANG);
}

/*	�ж�WANG��HUANG֮���ģ��
 *	������
 *		syllable				Դ����
 *		checked_syllable		���������
 *		fuzzy_mode				ģ������
 *	���أ�
 *		1			����
 *		0			������
 */
INLINE static int ProcessWANGAndHUANG(SYLLABLE syllable, SYLLABLE checked_syllable, int fuzzy_mode)
{
	if (!(fuzzy_mode & FUZZY_REV_HUANG_WANG))
		return 0;

	//Դ��ĸ��Ŀ����ĸ������W��H
	return (checked_syllable.con == CON_H && checked_syllable.vow == VOW_UANG &&
			syllable.con == CON_W && syllable.vow == VOW_ANG);
}

/*	�жϴ���F��HU��ģ����������ϵ
 *	U��ͷ����ĸֻ�У�UA, UAI, UAN, UANG, UE, UI, UN, UO���֡�
 *	�������ڻ�ȡ�������ص����ڱ�֤��ȷ�ԣ����Ա������ڲ��ٽ������ںϷ��Ե��жϡ�
 *	������
 *		syllable				Դ����
 *		checked_syllable		���������
 *		fuzzy_mode				ģ������
 *	���أ�
 *		1			����
 *		0			������
 */
INLINE static int ProcessFAndHU(SYLLABLE syllable, SYLLABLE checked_syllable, int fuzzy_mode)
{
	//û������ģ���������߱��Ƚϵ����ڲ�����Ҫ��F��HU��
	if (!((fuzzy_mode & FUZZY_F_HU) && syllable.con == CON_F && IS_START_WITH_HU(checked_syllable)))
		return 0;

	if (syllable.vow == VOW_NULL)
		return 1;

	switch(checked_syllable.vow)
	{
	case VOW_U:
		if (syllable.vow == VOW_NULL)
			return 1;

		return 0;

	case VOW_UA:
		return syllable.vow == VOW_A;

	case VOW_UAI:
		return syllable.vow == VOW_AI;

	case VOW_UAN:
		return syllable.vow == VOW_AN;

	case VOW_UANG:
		return syllable.vow == VOW_ANG;

	case VOW_UE:
		return syllable.vow == VOW_E;

	case VOW_UI:
		return syllable.vow == VOW_EI;

	case VOW_UN:
		return syllable.vow == VOW_EN;

	case VOW_UO:
		return syllable.vow == VOW_O;
	}

	return 0;
}

/*	�ж�HU��F��ģ����������ϵ��
 *	U��ͷ����ĸֻ�У�UA, UAI, UAN, UANG, UE, UI, UN, UO���֡�
 *	�������ڻ�ȡ�������ص����ڱ�֤��ȷ�ԣ����Ա������ڲ��ٽ������ںϷ��Ե��жϡ�
 *	������
 *		syllable				Դ����
 *		checked_syllable		���������
 *		fuzzy_mode				ģ������
 *	���أ�
 *		1			����
 *		0			������
 */
INLINE int ProcessHUAndF(SYLLABLE syllable, SYLLABLE checked_syllable, int fuzzy_mode)
{
	//ģ����û�����û����������������Ҫ��HU��F��
	if (!((fuzzy_mode & FUZZY_REV_F_HU) && IS_START_WITH_HU(syllable) && checked_syllable.con == CON_F))
		return 0;

	switch(syllable.vow)
	{
	case VOW_U:				//HU�����κ�һ����F��ͷ������
		return 1;

	case VOW_UA:
		return checked_syllable.vow == VOW_A;

	case VOW_UAI:
		return checked_syllable.vow == VOW_AI;

	case VOW_UAN:
		return checked_syllable.vow == VOW_AN;

	case VOW_UANG:
		return checked_syllable.vow == VOW_ANG;

	case VOW_UE:
		return checked_syllable.vow == VOW_E;

	case VOW_UI:
		return checked_syllable.vow == VOW_EI;

	case VOW_UN:
		return checked_syllable.vow == VOW_EN;

	case VOW_UO:
		return checked_syllable.vow == VOW_O;
	}

	return 0;
}

/*	�ж��Ƿ���˫ƴ�������������ģ��(��sp_fuzzy_syllables���崦ע��)
 *	������
 *		syllable				Դ����
 *		checked_syllable		���������
 *	���أ�
 *		1			����
 *		0			������
 */
INLINE static int ProcessSPFuzzy(SYLLABLE syllable, SYLLABLE checked_syllable)
{
	int i, j;
	
	for(i = 0; i < sp_fuzzy_map_items; i++)
	{
		if (syllable.con == sp_fuzzy_map[i].sp_fuzzy_syllables[0].con &&
			syllable.vow ==  sp_fuzzy_map[i].sp_fuzzy_syllables[0].vow)
		{
			//�����ڶ��ֽ���
			if (sp_fuzzy_map[i].sp_fuzzy_syllable_count <= 1)
				return 0;

			for(j = 1; j < sp_fuzzy_map[i].sp_fuzzy_syllable_count; j++)
				if (checked_syllable.con == sp_fuzzy_map[i].sp_fuzzy_syllables[j].con && 
					checked_syllable.vow == sp_fuzzy_map[i].sp_fuzzy_syllables[j].vow)
					return 1;
		}
	}

	return 0;
}

/*	�жϵ�һ�����ڲ�����ָʾ����ĸ�����Ƿ�����ڶ������ڲ�������ĸ��
 *	������
 *		syllable				Դ����
 *		checked_syllable		���������
 *		fuzzy_mode				ģ������
 *	���أ�
 *		1			����
 *		0			������
 */
int ContainVow(SYLLABLE syllable, SYLLABLE checked_syllable, int fuzzy_mode)
{
#define	CHECK_FUZZY(fuzzy_bit, vow_value)	((fuzzy_mode & fuzzy_bit) && (checked_syllable.vow == vow_value))

	//���jqxy��u��v��ͬ������
	if (syllable.con == CON_J || syllable.con == CON_Q || syllable.con == CON_X || syllable.con == CON_Y)
	{
		if (syllable.vow == VOW_V && checked_syllable.vow == VOW_U)
			return 1;

		if (syllable.vow == VOW_U && checked_syllable.vow == VOW_V)
			return 1;
	}

	//Ϊ�˽��fang���뵽hu������
	if (syllable.vow == checked_syllable.vow)
	{
		if (syllable.con == checked_syllable.con)
			return 1;

		if (!(fuzzy_mode & FUZZY_F_HU) && !(fuzzy_mode & FUZZY_REV_F_HU))
			return 1;

		if (fuzzy_mode & FUZZY_F_HU)
			if (syllable.con != CON_F || checked_syllable.con != CON_H)
				return 1;

		if (fuzzy_mode & FUZZY_REV_F_HU)
			if (syllable.con != CON_H || checked_syllable.con != CON_F)
				return 1;
	}

	if (syllable.vow == VOW_ANY)
		return 1;

	//����F��HU
	if (ProcessFAndHU(syllable, checked_syllable, fuzzy_mode))
		return 1;

	//����HU��F
	if (ProcessHUAndF(syllable, checked_syllable, fuzzy_mode))
		return 1;

	switch(syllable.vow)
	{
	case VOW_NULL:					//û����ĸ��ʶ���϶�����
		return 1;

	case VOW_A:
		if (checked_syllable.con != CON_NULL)
			return 0;

		return (CHECK_FUZZY(FUZZY_SUPER, VOW_AI))  || (CHECK_FUZZY(FUZZY_SUPER, VOW_AN)) ||
			   (CHECK_FUZZY(FUZZY_SUPER, VOW_ANG)) || (CHECK_FUZZY(FUZZY_SUPER, VOW_AO));

	case VOW_E:
		if (checked_syllable.con != CON_NULL)
			return 0;

		return (CHECK_FUZZY(FUZZY_SUPER, VOW_EI)) || (CHECK_FUZZY(FUZZY_SUPER, VOW_EN)) ||
			   (CHECK_FUZZY(FUZZY_SUPER, VOW_ER)) || (CHECK_FUZZY(FUZZY_SUPER, VOW_ENG));

	case VOW_O:
		if (checked_syllable.con != CON_NULL)
			return 0;

		return (CHECK_FUZZY(FUZZY_SUPER, VOW_OU));

	case VOW_AN:
		return CHECK_FUZZY(FUZZY_AN_ANG, VOW_ANG);

	case VOW_IAN:
		return CHECK_FUZZY(FUZZY_AN_ANG, VOW_IANG);

	case VOW_UAN:
		return CHECK_FUZZY(FUZZY_AN_ANG, VOW_UANG);

	case VOW_ANG:
		return CHECK_FUZZY(FUZZY_REV_AN_ANG, VOW_AN);

	case VOW_IANG:
		return CHECK_FUZZY(FUZZY_REV_AN_ANG, VOW_IAN);

	case VOW_UANG:
		return CHECK_FUZZY(FUZZY_REV_AN_ANG, VOW_UAN);

	case VOW_EN:
		return CHECK_FUZZY(FUZZY_EN_ENG, VOW_ENG);

	case VOW_ENG:
		return CHECK_FUZZY(FUZZY_REV_EN_ENG, VOW_EN);

	case VOW_IN:
		return CHECK_FUZZY(FUZZY_IN_ING, VOW_ING);

	case VOW_ING:
		return CHECK_FUZZY(FUZZY_REV_IN_ING, VOW_IN);
	}

	return 0;

//����ʹ�������
#undef	CHECK_FUZZY
}

/*	��ʹ�������ıȽϣ��жϵ�һ�����ڲ�����ָʾ�����ڼ����Ƿ�����ڶ������ڲ�����
 *	��Դ����ֻ����ĸʱ������ͬ��ĸ���κ����ڶ�����ͬ�ġ�
 *	������
 *		syllable				Դ����
 *		checked_syllable		���������
 *		fuzzy_mode				ģ������
 *	���أ�
 *		1			����
 *		0			������
 */
int ContainSyllable(SYLLABLE syllable, SYLLABLE checked_syllable, int fuzzy_mode)
{
	if (syllable.con == CON_F && syllable.vow == VOW_AN &&
		checked_syllable.con == CON_H && checked_syllable.vow == VOW_ANG)
		fuzzy_mode = fuzzy_mode;

	//�����һ������Ϊ�գ���϶�Ϊ����������û��������ȫ��������
	if (syllable.con == CON_NULL && syllable.vow == VOW_NULL)
		return 1;

	//��Ҫ�����жϡ����������ơ���ģ����
	if (ProcessHUANGAndWANG(syllable, checked_syllable, fuzzy_mode))
		return 1;

	if (ProcessWANGAndHUANG(syllable, checked_syllable, fuzzy_mode))
		return 1;

	//����˫ƴ����������Ψһ�����
	if (pim_config->pinyin_mode == PINYIN_SHUANGPIN && ProcessSPFuzzy(syllable, checked_syllable))
		return 1;

	//�����ж�����������ʡʱ��
	//���Դ���ڴ�����������������ڲ�����������Ϊ�����ϡ�
	if (syllable.tone != TONE_0 && checked_syllable.tone != TONE_0 &&	//��������
		!(syllable.tone & checked_syllable.tone))						//����������
		return 0;

	//�����ĸ����ͬ�򷵻ز�ͬ
	if (!ContainCon(syllable, checked_syllable, fuzzy_mode))
		return 0;

	//���Դ���ڵ���ĸΪ�գ���ȽϽ��Ϊ��ͬ
	if (syllable.vow == VOW_NULL)
		return 1;

	return ContainVow(syllable, checked_syllable, fuzzy_mode);
}

/*	�������������ıȽϣ��жϵ�һ�����ڲ�����ָʾ�����������Ƿ�����ڶ������ڲ�����������
 *	������
 *		syllable				Դ����
 *		checked_syllable		���������
 *	���أ�
 *		1			����
 *		0			������
 */
__inline static int ContainTone(SYLLABLE syllable, SYLLABLE checked_syllable)
{
	//������������е�һ��Ϊû��ָ����������Ϊ������ϵ
	if (syllable.tone == TONE_0 || checked_syllable.tone == TONE_0)
		return 1;

	return syllable.tone & checked_syllable.tone;
}

/*	ʹ�ô��������ıȽϣ��жϵ�һ�����ڲ�����ָʾ�����ڼ����Ƿ�����ڶ������ڲ�����
 *	��Դ����ֻ����ĸʱ������ͬ��ĸ���κ����ڶ�����ͬ�ġ�
 *	������
 *		syllable				Դ����
 *		checked_syllable		���������
 *		fuzzy_mode				ģ������
 *	���أ�
 *		1			����
 *		0			������
 */
int ContainSyllableWithTone(SYLLABLE syllable, SYLLABLE checked_syllable, int fuzzy_mode)
{
	return ContainTone(syllable, checked_syllable) && ContainSyllable(syllable, checked_syllable, fuzzy_mode);
}

/*	���ƴ������������
 *	���ƴ��������Ϊ0��������Ϊȫ��������
 *	������
 *		pin_yin			ƴ����
 *		length			������
 *	���أ�
 *		TONE_ERROR		�Ƿ���������
 *		����			������ʶ
 */
static int GetTone(const TCHAR *pin_yin)
{
	switch(*pin_yin)
	{
	case TONE_CHAR_1:
		return TONE_1;

	case TONE_CHAR_2:
		return TONE_2;

	case TONE_CHAR_3:
		return TONE_3;

	case TONE_CHAR_4:
		return TONE_4;
	}

	return TONE_0;
}

/*	���ƴ�������ڱ�ʶ����ǰƴ����Ϊ������ƴ�����м������ҳ����ڱ�
 *	��ƴ����������а����Ŵ����磺v��u��i��ʼ����϶�����ʧ�ܡ�
 *	�磺"wom"�����ش���"pin4" -> ��ĸ��CON_P����ĸ��VOW_IN��������4����10000B����
 *
 *	���ڲ���ƴ��ֻ����ģ���ķ�ʽ�²�����ȷ�ģ����Ա��뽫ģ�����ô��ݹ�����
 *	�磺cuang����c��chģ�������������ȷ�ķ�����
 *
 *	������
 *		pin_yin					ƴ����
 *		syllable				����
 *		*syllable_str_length	����ʹ�õ�ƴ�����ȣ�����Ϊ0
 *		fuzzy_mode				ģ����ѡ��
 *	���أ�
 *		1				�ɹ�
 *		0				���ڽ���ʧ�ܡ��磺vabcd�ȡ�
 */
int GetSyllable(const TCHAR *pin_yin, SYLLABLE *syllable, int *syllable_str_length, int fuzzy_mode/*, int *correct_flag, int *separator_flag*/)
{
	int hi, low, mid, ret;
	int py_length = (int)_tcslen(pin_yin);
	int has_tone = 0, has_separator = 0;	//�Ƿ������������ڷָ���
	TCHAR py[MAX_PINYIN_LENGTH + 1];		//����ƴ����

	if (!py_length || py_length > MAX_PINYIN_LENGTH)				//û��ƴ��������Ϊ�Ƿ�����
		return 0;

	if (!share_segment)
		return 0;

	//����һ��
	_tcscpy(py, pin_yin);

	//���ĩβΪ'����Ϊ��ȷ������
	if (py[py_length - 1] == SYLLABLE_SEPARATOR_CHAR)
	{
		py[py_length - 1] = 0;
		has_separator	  = 1;
		py_length--;

		if (!py_length)				//û��ƴ��������Ϊ�Ƿ�����
			return 0;
	}

	//ͨ���
	if (py_length == 1 && *py == SYLLABLE_ANY_CHAR)
	{
		syllable->con		 = CON_ANY;
		syllable->vow		 = VOW_ANY;
		syllable->tone		 = TONE_0;
		*syllable_str_length = 1;

		return 1;
	}

	//�ж�β���Ƿ���������־
	if (IS_TONE(py[py_length - 1]))
	{
		has_tone = 1;
		py_length--;

		if (!py_length)				//�ٴ��ж��Ƿ���ƴ����
			return 0;
	}

	//���ַ�����ƴ��
	low = 0;
	hi = share_segment->syllable_map_items - 1;
	while(low <= hi)
	{
		mid = (low + hi) / 2;

		//������Ȳ���ͬ����϶������ҵ��������������ƴ���ıȽ�
		//�磺��������������Ƚ�men����meng��ͬ��
		//��������Ϊ���֣����ԱȽ�ʱ�����������
		if (py_length != share_segment->syllable_map[mid].pin_yin_length)
			ret = _tcscmp(py, share_segment->syllable_map[mid].pin_yin);
		else
			ret = _tcsncmp(py, share_segment->syllable_map[mid].pin_yin, py_length);

		if (!ret)			//�ҵ�
			break;

		if (ret < 0)
			hi = mid - 1;
		else
			low = mid + 1;
	}

	if (low > hi)			//û���ҵ�
		return 0;

	//���������Ҫ��ģ��������²�����Ч�ģ�����Ҫ�ж�ģ����־�Ƿ�����
	if (share_segment->syllable_map[mid].fuzzy_flag && !(share_segment->syllable_map[mid].fuzzy_flag & fuzzy_mode))
		return 0;

	//���������Ҫ�ھ���������²�����Ч�ģ�����Ҫ�жϾ�����־�Ƿ�����
	//if (share_segment->syllable_map[mid].correct_flag && !(share_segment->syllable_map[mid].correct_flag & pim_config->correct_option))
	//	return 0;

	//�洢���
	syllable->con  = share_segment->syllable_map[mid].con;
	syllable->vow  = share_segment->syllable_map[mid].vow;
	syllable->tone = TONE_0;				//û��������ʶ

	//if (correct_flag)
	//	*correct_flag = share_segment->syllable_map[mid].correct_flag;

	//if (separator_flag)
	//	*separator_flag = has_separator;

	if (has_tone)
		syllable->tone = GetTone(py + share_segment->syllable_map[mid].pin_yin_length);

	if (syllable_str_length)
	{
		*syllable_str_length = share_segment->syllable_map[mid].pin_yin_length;

		if (has_tone)
			(*syllable_str_length)++;

		if (has_separator)
			(*syllable_str_length)++;
	}

	return 1;
}

/*	���˫ƴ������
 */
int GetSPSyllable(const TCHAR *pin_yin, SYLLABLE *syllable, int *syllable_str_length, int fuzzy_mode)
{
	int  i, max_length, index;
	int  syllable_length;
	int  py_length = (int)_tcslen(pin_yin);
	int  has_tone = 0, has_separator = 0;		//�Ƿ������������ڷָ���
	TCHAR py[MAX_PINYIN_LENGTH + 1];			//����ƴ����

	if (!py_length || py_length > 3)			//û��ƴ��������Ϊ�Ƿ�����
		return 0;

	if (!share_segment)
		return 0;

	//����һ��
	_tcscpy(py, pin_yin);

	//���ĩβΪ'����Ϊ��ȷ������
	if (py[py_length - 1] == SYLLABLE_SEPARATOR_CHAR)
	{
		py[py_length - 1] = 0;
		has_separator	  = 1;
		py_length--;

		if (!py_length)				//û��ƴ��������Ϊ�Ƿ�����
			return 0;
	}

	//ͨ���
	if (py_length == 1 && *py == SYLLABLE_ANY_CHAR)
	{
		syllable->con		 = CON_ANY;
		syllable->vow		 = VOW_ANY;
		syllable->tone		 = TONE_0;
		*syllable_str_length = 1;

		return 1;
	}

	//�ж�β���Ƿ���������־
	if (pim_config->use_hz_tone && IS_TONE(py[py_length - 1]))
	{
		has_tone = 1;
		py[py_length - 1] = 0;
		py_length--;

		if (!py_length)				//�ٴ��ж��Ƿ���ƴ����
			return 0;
	}

	//����ƥ��pin_yin_length���ģ��������ж��ƥ�������ǵ�pin_yin_length
	//�����ͬ�������������뵽sp_fuzzy_syllables(Ŀǰͬһ��˫ƴ������֧
	//��4������ģ���������������˫ƴ�����е�˫ƴ���dk�����Խ���Ϊden��dia
	//����2������ģ��)

	//����ƴ��
	max_length = 0;
	index = -1;

	//�ҵ���һ�����ƥ�����λ��
	for (i = 0; i < share_segment->syllable_map_items; i++)
	{
		if (!_tcscmp(py, share_segment->syllable_map[i].sp_pin_yin) &&
			max_length < share_segment->syllable_map[i].pin_yin_length)
		{
			max_length = share_segment->syllable_map[i].pin_yin_length;
			index = i;
		}
	}

	//û���ҵ�
	if (index == -1)	
		return 0;

	//��������˵��Ȼ�����if��֧
	if (sp_fuzzy_map_items < MAX_SYLLABLE_PER_INPUT)
	{
		//�����к������ҳ������ƥ�������ȵ�ƥ����(����������)���뵽sp_fuzzy_map
		for (i = 0; i < share_segment->syllable_map_items; i++)
		{
			//�״ν��������ifʱ����i==index
			if (!_tcscmp(py, share_segment->syllable_map[i].sp_pin_yin) &&
				max_length == share_segment->syllable_map[i].pin_yin_length)
			{
				int count = sp_fuzzy_map[sp_fuzzy_map_items].sp_fuzzy_syllable_count;
				if (count < MAX_SP_FUZZY_SYLLABLE_COUNT)
				{
					sp_fuzzy_map[sp_fuzzy_map_items].sp_fuzzy_syllables[count].con = share_segment->syllable_map[i].con;
					sp_fuzzy_map[sp_fuzzy_map_items].sp_fuzzy_syllables[count].vow = share_segment->syllable_map[i].vow;
					sp_fuzzy_map[sp_fuzzy_map_items].sp_fuzzy_syllables[count].tone = TONE_0;
					sp_fuzzy_map[sp_fuzzy_map_items].syllable_map_index[count] = i;
					sp_fuzzy_map[sp_fuzzy_map_items].sp_fuzzy_syllable_count++; 
				}
			}
		}

		for (i = 0; i < sp_fuzzy_map[sp_fuzzy_map_items].sp_fuzzy_syllable_count; i++)
		{
			index = sp_fuzzy_map[sp_fuzzy_map_items].syllable_map_index[i];

			//���������Ҫ��ģ��������²�����Ч�ģ�����Ҫ�ж�ģ����־�Ƿ����ã���δ���ý����sp_fuzzy_syllables��ɾ��
			if (share_segment->syllable_map[index].fuzzy_flag && !(share_segment->syllable_map[index].fuzzy_flag & fuzzy_mode))
			{
				if (i < sp_fuzzy_map[sp_fuzzy_map_items].sp_fuzzy_syllable_count - 1)
				{
					memmove(sp_fuzzy_map[sp_fuzzy_map_items].sp_fuzzy_syllables + i,
						sp_fuzzy_map[sp_fuzzy_map_items].sp_fuzzy_syllables + i + 1,
						sp_fuzzy_map[sp_fuzzy_map_items].sp_fuzzy_syllable_count - i - 1);
					memmove(sp_fuzzy_map[sp_fuzzy_map_items].syllable_map_index + i, 
						sp_fuzzy_map[sp_fuzzy_map_items].syllable_map_index + i + 1,
						sp_fuzzy_map[sp_fuzzy_map_items].sp_fuzzy_syllable_count - i - 1);
				}

				sp_fuzzy_map[sp_fuzzy_map_items].sp_fuzzy_syllable_count--;
				i--;
			}
		}
		
		if (!sp_fuzzy_map[sp_fuzzy_map_items].sp_fuzzy_syllable_count)
			return 0;

		//������ڶ�������ƥ������ý��������׸�Ϊ���������������Ϊģ����
		i = sp_fuzzy_map[sp_fuzzy_map_items].syllable_map_index[0];

		sp_fuzzy_map_items++;
	}
	else
	{
		//���������Ҫ��ģ��������²�����Ч�ģ�����Ҫ�ж�ģ����־�Ƿ�����
		if (share_segment->syllable_map[index].fuzzy_flag && !(share_segment->syllable_map[index].fuzzy_flag & fuzzy_mode))
			return 0;

		i = index;
	}


	//�洢���
	syllable->con  = share_segment->syllable_map[i].con;
	syllable->vow  = share_segment->syllable_map[i].vow;
	syllable->tone = TONE_0;			//û��������ʶ

	syllable_length = (int)_tcslen(share_segment->syllable_map[i].sp_pin_yin);
	if (has_tone)
		syllable->tone = GetTone(pin_yin + syllable_length);

	if (syllable_str_length)
	{
		*syllable_str_length = syllable_length;

		if (has_tone)
			(*syllable_str_length)++;

		if (has_separator)
			(*syllable_str_length)++;
	}

	return 1;
}

int GetLegalPinYinLength(const TCHAR *pin_yin, int state, int english_state)
{
	int i;

	if (STATE_EDIT == state && ENGLISH_STATE_NONE == english_state)
	{
		for (i = 0; pin_yin[i]; i++)
		{
			if (pin_yin[i] >= 'A' && pin_yin[i] <= 'Z')
				break;
		}
	}
	else
		i = (int)_tcslen(pin_yin);

	return i;
}

/*	�õݹ�ķ�ʽ�������ƴ������
 *	������
 *		pin_yin				ƴ����
 *		*syllables			���ڴ洢����
 *		array_length		�������鳤��
 *		fuzzy_mode			ģ������
 *	����ֵ��
 *		0��û�н�������ȷ�Ľ��
 *		> 0����������������Ŀ
 */
static int ProcessParse(const TCHAR *pin_yin, SYLLABLE *syllables, int array_length, int fuzzy_mode/*, int *correct_flag, int *separator_flag*/)
{
	int i, ret;
	int py_length;									//ƴ��������
	int syllable_string_length;						//�����ַ�������
	TCHAR cur_py[MAX_PINYIN_LENGTH + 1];			//��ǰ�������ʱƴ����
	TCHAR new_pin_yin[MAX_INPUT_LENGTH + 0x10];

	assert(pin_yin && syllables);

	//���������зַ���
	if (*pin_yin == SYLLABLE_SEPARATOR_CHAR)
		pin_yin++;

	//���ٵݹ����ֱ�ӷ��ִ���
	if (*pin_yin == 'i' || *pin_yin == 'u' || *pin_yin == 'v')
		return 0;

	if (IS_TONE(*pin_yin))
		return 0;

	//py_length = (int)_tcslen(pin_yin);
	py_length = GetLegalPinYinLength(pin_yin, STATE_EDIT, ENGLISH_STATE_NONE);
	//���ƴ���ַ����ȴ�����󳤶����ƣ����˳�
	if ( py_length > _SizeOf(new_pin_yin) )
		return 0;
	_tcsncpy_s(new_pin_yin, _SizeOf(new_pin_yin), pin_yin, py_length);

	for (i = min(MAX_PINYIN_LENGTH, py_length); i > 0; i--)
	{
		//���Ƶ�һ�����ܵ�����
		CopyPartString(cur_py, new_pin_yin, i);

		//���ҺϷ���ƴ��
		if (!GetSyllable(cur_py, syllables, &syllable_string_length, fuzzy_mode/*, correct_flag, separator_flag*/))
			continue;			//û���ҵ�������һ���������ٴμ���

		//û����Ҫ����������ƴ���ˣ�������ݹ� || û�п������������ռ�
		if (py_length == i || array_length == 1)
			return 1;

		//�ҵ���һ�����ڣ�����ʣ������
		ret = ProcessParse(new_pin_yin + syllable_string_length, 
						   syllables + 1, 
						   array_length - 1, 
						   fuzzy_mode//, 
						   /*correct_flag ? correct_flag + 1 : 0,*/
						   /*separator_flag ? separator_flag + 1 : 0*/);

		if (!ret)
			continue;			//û�кϷ������ڣ��磺li�������һ��Ϊl����Զû�кϷ�������

		//���ڻ����������
		return ret + 1;
	}

	//û���ҵ���
	return 0;
}

/*	��˫ƴ�����н����ĵݹ����
 */
static int ProcessParseSPInternal(const TCHAR *pin_yin, SYLLABLE *syllables, int array_length, int fuzzy_mode)
{
	int i, ret;
	int py_length;									//ƴ��������
	int syllable_string_length;						//�����ַ�������
	TCHAR cur_py[MAX_PINYIN_LENGTH + 1];			//��ǰ�������ʱƴ����

	assert(pin_yin && syllables);

	//���������зַ���
	if (*pin_yin == SYLLABLE_SEPARATOR_CHAR)
		pin_yin++;

	//���ٵݹ����ֱ�ӷ��ִ���
	if (IS_TONE(*pin_yin))
		return 0;

	py_length = (int)_tcslen(pin_yin);
	for (i = min(3, py_length); i > 0; i--)
	{
		//���Ƶ�һ�����ܵ�����
		CopyPartString(cur_py, pin_yin, i);

		//���ҺϷ���ƴ��
		if (!GetSPSyllable(cur_py, syllables, &syllable_string_length, fuzzy_mode))
			continue;			//û���ҵ�������һ���������ٴμ���

		//û����Ҫ����������ƴ���ˣ�������ݹ� || û�п������������ռ�
		if (py_length == i || array_length == 1)
			return 1;

		//�ҵ���һ�����ڣ�����ʣ������
		ret = ProcessParseSPInternal(pin_yin + syllable_string_length, syllables + 1, array_length - 1, fuzzy_mode);
		if (!ret)
			continue;			//û�кϷ������ڣ��磺li�������һ��Ϊl����Զû�кϷ�������

		//���ڻ����������
		return ret + 1;
	}

	//û���ҵ���
	return 0;
}

/*	��˫ƴ�����н���
 *  ����sp_fuzzy_map����0��Ե�������ƴ�����������ǵ������ڣ�
 *  ��˲��ܷ��ڵݹ麯�����Ҫ��ProcessParseSP��ĵݹ鲿��
 *  ��������һ������ProcessParseSPInternal
 */
static int ProcessParseSP(const TCHAR *pin_yin, SYLLABLE *syllables, int array_length, int fuzzy_mode)
{
	//�ڽ���˫ƴƴ��֮ǰ���ѱ�Ҫ�����ݽṹ��0
	memset(sp_fuzzy_map, 0, sizeof(sp_fuzzy_map));
	sp_fuzzy_map_items = 0;

	return ProcessParseSPInternal(pin_yin, syllables, array_length, fuzzy_mode);
}

/*	�õݹ�ķ�ʽ�������ƴ������
 *	������
 *		pin_yin				ƴ����
 *		*syllables			���ڴ洢����
 *		array_length		�������鳤��
 *		fuzzy_mode			ģ������
 *	����ֵ��
 *		0��û�н�������ȷ�Ľ��
 *		> 0����������������Ŀ
 */
static int ProcessParseReverse(const TCHAR *pin_yin, SYLLABLE *syllables, int array_length, int fuzzy_mode/* ,int *correct_flag, int *separator_flag*/)
{
	int i, ret;
	int py_length;													//ƴ��������
	int syllable_string_length;										//�����ַ�������
	TCHAR cur_py[MAX_PINYIN_LENGTH * MAX_SYLLABLE_PER_INPUT + 1];	//��ǰ�������ʱƴ����
	TCHAR new_pin_yin[MAX_INPUT_LENGTH + 0x10];

	assert(pin_yin && syllables);

	//���������зַ���
	if (*pin_yin == SYLLABLE_SEPARATOR_CHAR)
		pin_yin++;

	//���ٵݹ����ֱ�ӷ��ִ���
	if (*pin_yin == 'i' || *pin_yin == 'u' || *pin_yin == 'v')
		return 0;

	if (IS_TONE(*pin_yin))
		return 0;

	//py_length = (int)_tcslen(pin_yin);
	py_length = GetLegalPinYinLength(pin_yin, STATE_EDIT, ENGLISH_STATE_NONE);
	//���ƴ���ַ����ȴ�����󳤶����ƣ����˳�
	if ( py_length > _SizeOf(new_pin_yin) )
		return 0;
	_tcsncpy_s(new_pin_yin, _SizeOf(new_pin_yin), pin_yin, py_length);

	for (i = min(MAX_PINYIN_LENGTH, py_length); i > 0; i--)
	{
		//���ҺϷ���ƴ��
		if (!GetSyllable(new_pin_yin + py_length - i, syllables, &syllable_string_length, fuzzy_mode/*, correct_flag, separator_flag*/))
			continue;			//û���ҵ�������һ���������ٴμ���

		//û����Ҫ����������ƴ���ˣ�������ݹ� || û�п������������ռ�
		if (py_length == i || array_length == 1)
			return 1;

		//���Ƶ�һ�����ܵ�����
		CopyPartString(cur_py, new_pin_yin, py_length - i);

		//�ҵ���һ�����ڣ�����ʣ������
		ret = ProcessParseReverse(cur_py, 
								  syllables + 1, 
								  array_length - 1, 
								  fuzzy_mode//, 
								  /*correct_flag ? correct_flag + 1 : 0, */
								  /*separator_flag ? separator_flag + 1 : 0*/);

		if (!ret)
			continue;			//û�кϷ������ڣ��磺li�������һ��Ϊl����Զû�кϷ�������

		//���ڻ����������
		return ret + 1;
	}

	//û���ҵ���
	return 0;
}

/*	�ж�ƴ�����Ƿ�Ϊ�Ϸ��ġ�
 *	������
 *		pin_yin				ƴ����
 *	����ֵ��
 *		1���Ϸ�
 *		0���Ƿ�
 */
static int LegalPinYin(const TCHAR *pin_yin)
{
	int i, nlen = GetLegalPinYinLength(pin_yin, STATE_EDIT, ENGLISH_STATE_NONE);

	if (!pin_yin)
		return 0;

	//���бȽϴ�ǳ�ĺϷ��Լ�飬��Ҫ���������봮���Ƿ�������Ϸ���
	//�ַ��Լ�uu��vv��ii��uv��vi��vu��
	//�ü���Ŀ�����ڱ���ݹ�������ݵ�ʱ�������
	//v�ĺϷ�ǰ׺��jlnqxy
	//u�ķǷ�ǰ׺��{NUL}aeuv{TONE}
	//i�ķǷ�ǰ׺��{NUL}fgikovw{TONE}		h��Ҫ���⴦��shi,zhi,chi�Ϸ���
	//TONE�ķǷ�ǰ׺��{NUL}{TONE}{SEPARATOR}
	//SEPARATOR�ķǷ�ǰ׺��{NUL}{SEPARATOR}

	//��ͷ��Ӧ����a-z�����Ҳ���uvi
	if (*pin_yin < 'a' || *pin_yin > 'z' ||	*pin_yin == 'u' || *pin_yin == 'v' || *pin_yin == 'i')
		return 0;

	for (i = 1; pin_yin[i] && i < nlen; i++)
	{
		//ƴ�����г��ַǷ����ַ�
		if (!((pin_yin[i] >= 'a' && pin_yin[i] <= 'z') || (pin_yin[i] == SYLLABLE_SEPARATOR_CHAR) ||
			  (pin_yin[i] == SYLLABLE_ANY_CHAR) || IS_TONE(pin_yin[i])))
			return 0;

		switch(pin_yin[i])
		{
		case SYLLABLE_ANY_CHAR:
			if (!(pim_config->ci_option & CI_WILDCARD))
				return 0;

			if (pin_yin[i - 1] == SYLLABLE_ANY_CHAR)
				return 0;

			break;

		case SYLLABLE_SEPARATOR_CHAR:
			if (pin_yin[i - 1] == SYLLABLE_SEPARATOR_CHAR)
				return 0;

			break;

		case 'v':
			{
				int wrong = 1;
				switch(pin_yin[i - 1])
				{
				case  'j':	case  'l':	case  'n':	case  'q':	case  'x':	case  'y':
					wrong = 0;
				}

				if (wrong)
					return 0;
			}
			break;

		case 'u':
			switch(pin_yin[i - 1])
			{
			case 'a':	case 'e':	case 'v':	case 'u':
				return 0;
			}

			break;

		case 'i':
			switch(pin_yin[i - 1])
			{
			case 'f':
				if (pin_yin[i + 1] == 'a' && pin_yin[i + 2] == 'o')
					break;

				return 0;

			case 'g':	case 'i':	case 'o':	case 'v':	case 'w':
				return 0;

			case 'h':
				if (i < 2 || (pin_yin[i - 2] != 's' && pin_yin[i - 2] != 'c' &&	pin_yin[i - 2] != 'z'))
					return 0;

				break;

			default:
				if (IS_TONE(pin_yin[i - 1]))
					return 0;
			}

			break;

		case TONE_CHAR_1:
		case TONE_CHAR_2:
		case TONE_CHAR_3:
		case TONE_CHAR_4:
			if (!pim_config->use_hz_tone)
				return 0;

			if (IS_TONE(pin_yin[i - 1]))
				return 0;

			if (pin_yin[i - 1] == SYLLABLE_SEPARATOR_CHAR || pin_yin[i - 1] == SYLLABLE_ANY_CHAR)
				return 0;

			break;
		}
	}

	return 1;
}

/*	����ƴ�������������顣�����������ƥ��ķ�ʽ���С�
 *	������
 *		pin_yin				ƴ���������ܰ������ڷָ�����
 *		*syllable_array		��������������飬��0Ϊ��ʶ
 *		syllable_length		�������鳤��
 *		fuzzy_mode			ģ��������
 *	���أ�
 *		���ڵ���Ŀ������0��ʱ��һ��Ϊ��������
 */
int ParsePinYinString(const TCHAR *pin_yin, SYLLABLE *syllables, int array_length, int fuzzy_mode/*, int *correct_flag, int *separator_flag*/)
{
	if (pim_config->pinyin_mode == PINYIN_SHUANGPIN)
		return ProcessParseSP(pin_yin, syllables, array_length, fuzzy_mode);

	//ȫƴ
	if (!LegalPinYin(pin_yin))
		return 0;

	return ProcessParse(pin_yin, syllables, array_length, fuzzy_mode/*, correct_flag, separator_flag*/);
}

//�Ƿ�����Ҫ�������������
//int HasSpecialSyllable(SYLLABLE *syllables, int *separator_flag, int syllable_count)
//{
//	int i, j, found;
//	
//	if (syllable_count < special_syllables[0].old_syllable_count)
//		return -1;
//
//	for (i = 0; i < _SizeOf(special_syllables); i++)
//	{
//		if (syllable_count < special_syllables[i].old_syllable_count)
//			continue;
//
//		found = 1;
//
//		for (j = 0; j < syllable_count && j < special_syllables[i].old_syllable_count; j++)
//		{
//			if (syllables[j].con != special_syllables[i].old_syllables[j].con ||
//				syllables[j].vow != special_syllables[i].old_syllables[j].vow || separator_flag[j])
//			{
//				found = 0;
//				break;
//			}
//		}
//
//		if (found)
//			return i;
//	}
//
//	return -1;
//}

//int ProcessSpecialSyllables(SYLLABLE *syllables, int *separator_flag, int array_length, int syllable_count)
//{
//	int i, j, index, count;
//
//	if (syllable_count < special_syllables[0].old_syllable_count)
//		return syllable_count;
//
//	for (i = 0; i < syllable_count; i++)
//	{
//		index = HasSpecialSyllable(&syllables[i], &separator_flag[i], syllable_count - i);
//		if (index < 0)
//			continue;
//
//		count = special_syllables[index].new_syllable_count - special_syllables[index].old_syllable_count;
//		if ((syllable_count + count) > array_length)
//			break;
//
//		//����ƶ�
//		if (count > 0)
//		{
//			for (j = syllable_count - 1 + count; j > i + count; j--)
//				syllables[j] = syllables[j - 1];
//		}
//
//		for (j = 0; j < special_syllables[index].new_syllable_count; j++)
//		{
//			syllables[i + j].con = special_syllables[index].new_syllables[j].con;
//			syllables[i + j].vow = special_syllables[index].new_syllables[j].vow;
//		}
//
//		return syllable_count + count;
//	}
//
//	return syllable_count;
//}

/*	����ƴ�������������顣���÷������ƥ��ķ�ʽ���С�
 *	������
 *		pin_yin				ƴ���������ܰ������ڷָ�����
 *		*syllable_array		��������������飬��0Ϊ��ʶ
 *		syllable_length		�������鳤��
 *		fuzzy_mode			ģ��������
 *		pinyin_mode			ȫƴ��˫ƴ
 *	���أ�
 *		���ڵ���Ŀ������0��ʱ��һ��Ϊ��������
 */
int ParsePinYinStringReverse(const TCHAR *pin_yin, SYLLABLE *syllables, int array_length, int fuzzy_mode, int pinyin_mode/* ,int *correct_flag,int *separator_flag*/)
{
	int i;
	int count, other_count;		//����������Ŀ
	SYLLABLE tmp_syllable;		//������ʱ����
	SYLLABLE tmp_syllables[MAX_SYLLABLE_PER_INPUT];

	//int tmp_correct_flag = 0;
	//int tmp_correct_flags[MAX_SYLLABLE_PER_INPUT] = {0};
	//int tmp_separator_flag = 0;
	//int tmp_separator_flags[MAX_SYLLABLE_PER_INPUT] = {0};

	if (pinyin_mode == PINYIN_SHUANGPIN)
		return ProcessParseSP(pin_yin, syllables, array_length, fuzzy_mode);

	//ȫƴ
	if (!LegalPinYin(pin_yin))
		return 0;

	//�������������ֽ����ķ������У���һ�ֵ�������Ŀ��ʹ����һ���������ͬ��ʹ�÷���ġ�
	count = ProcessParseReverse(pin_yin, syllables, array_length, fuzzy_mode/*, correct_flag, separator_flag*/);
	other_count = ProcessParse(pin_yin, tmp_syllables, array_length, fuzzy_mode/*, tmp_correct_flags, tmp_separator_flags*/);

	if ((other_count < count)) //IsSpecialPinYinStr(pin_yin))		//����Ľ����
	{
		for (i = 0; i < other_count; i++)
		{
			syllables[i] = tmp_syllables[i];
			/*if (correct_flag)
				correct_flag[i] = tmp_correct_flags[i];*/
			//if (separator_flag)
			//	separator_flag[i] = tmp_separator_flags[i];
		}

		//if (separator_flag)
		//	other_count = ProcessSpecialSyllables(syllables, tmp_separator_flags, array_length, other_count);

		return other_count;
	}

	//����Ľ���ã��������������鵹��
	for (i = 0; i < count / 2; i++)
	{
		tmp_syllable			 = syllables[i];
		syllables[i]			 = syllables[count - 1 - i];
		syllables[count - 1 - i] = tmp_syllable;
		//if (correct_flag)
		//{
		//	tmp_correct_flag			= correct_flag[i];
		//	correct_flag[i]				= correct_flag[count - 1 - i];
		//	correct_flag[count - 1 - i] = tmp_correct_flag;
		//}
		//if (separator_flag)
		//{
		//	tmp_separator_flag 			  = separator_flag[i];
		//	separator_flag[i]			  = separator_flag[count - 1 - i];
		//	separator_flag[count - 1 - i] = tmp_separator_flag;
		//}
	}

	//if (separator_flag)
	//	count = ProcessSpecialSyllables(syllables, separator_flag, array_length, count);

	return count;
}

//��ĸת�����ַ���
static TCHAR *con_to_string[] =
{
	TEXT(""),						//CON_NULL
	TEXT("b"),						//CON_B
	TEXT("c"),						//CON_C
	TEXT("ch"),						//CON_CH
	TEXT("d"),						//CON_D
	TEXT("f"),						//CON_F
	TEXT("g"),						//CON_G
	TEXT("h"),						//CON_H
	TEXT("j"),						//CON_J
	TEXT("k"),						//CON_K
	TEXT("l"),						//CON_L
	TEXT("m"),						//CON_M
	TEXT("n"),						//CON_N
	TEXT("p"),						//CON_P
	TEXT("q"),						//CON_Q
	TEXT("r"),						//CON_R
	TEXT("s"),						//CON_S
	TEXT("sh"),						//CON_SH
	TEXT("t"),						//CON_T
	TEXT("w"),						//CON_W
	TEXT("x"),						//CON_X
	TEXT("y"),						//CON_Y
	TEXT("z"),						//CON_Z
	TEXT("zh"),						//CON_ZH
	TEXT(""),						//CON_END
	TEXT("*"),						//CON_ANY
};

//��ĸת�����ַ���
static TCHAR *vow_to_string[] =
{
	TEXT(""),						//VOW_NULL,
	TEXT("a"),						//VOW_A,
	TEXT("ai"),						//VOW_AI
	TEXT("an"),						//VOW_AN
	TEXT("ang"),					//VOW_ANG
	TEXT("ao"),						//VOW_AO
	TEXT("e"),						//VOW_E
	TEXT("ei"),						//VOW_EI
	TEXT("en"),						//VOW_EN
	TEXT("eng"),					//VOW_ENG
	TEXT("er"),						//VOW_ER
	TEXT("i"),						//VOW_I
	TEXT("ia"),						//VOW_IA
	TEXT("ian"),					//VOW_IAN
	TEXT("iang"),					//VOW_IANG
	TEXT("iao"),					//VOW_IAO
	TEXT("ie"),						//VOW_IE
	TEXT("in"),						//VOW_IN
	TEXT("ing"),					//VOW_ING
	TEXT("iong"),					//VOW_IONG
	TEXT("iu"),						//VOW_IU
	TEXT("o"),						//VOW_O
	TEXT("ong"),					//VOW_ONG
	TEXT("ou"),						//VOW_OU
	TEXT("u"),						//VOW_U
	TEXT("ua"),						//VOW_UA
	TEXT("uai"),					//VOW_UAI
	TEXT("uan"),					//VOW_UAN
	TEXT("uang"),					//VOW_UANG
	TEXT("ue"),						//VOW_UE
	TEXT("ui"),						//VOW_UI
	TEXT("un"),						//VOW_UN
	TEXT("uo"),						//VOW_UO
	TEXT("v"),						//VOW_V
	TEXT(""),						//VOW_END
	TEXT(""),						//VOW_ANY
};

//����ת�����ַ���
static TCHAR *tone_to_string[] =
{
/* 0 */		TEXT(""),		//û����
/* 1 */		TEXT("1"),		//(1 << 0)	1
/* 2 */		TEXT("2"),		//(1 << 1)	2
/* 3 */		TEXT(""),
/* 4 */		TEXT("3"),		//(1 << 2)	3
/* 5 */		TEXT(""),
/* 6 */		TEXT(""),
/* 7 */		TEXT(""),
/* 8 */		TEXT("4"),		//(1 << 3)	4
};

/*	������ڵ�ƴ������
 *	������
 *		syllable		����
 *		str				�����
 *		length			���봮�ĳ���
 *		correct_flag	������
 *		tone_char		ʹ������(ע�ⲻ��ʹ��syllable.h�е�TONE_CHAR_1���ַ������ǽ�ƴ����ʾΪSYLLABLEMAP�еĴ�������ʽ���稡��)
 *	����ֵ��
 *		�ַ������ȣ�0��ʶû���ַ���
 */
int GetSyllableString(SYLLABLE syllable, TCHAR *str, int length, /*int correct_flag, */int tone_char)
{
	TCHAR *con_str, *vow_str;
	TCHAR tones[10] = {0};
	TCHAR py_str[0x10] = {0};
 	int count = 0;
	int i, j;

	if (!share_segment)
		return 0;

	//CON_ERROR��VOW_ERROR
	if (syllable.con < CON_NULL || syllable.vow < VOW_NULL || syllable.con > CON_ANY || syllable.vow > VOW_ANY)
	{
		*str++ = '?';
		*str++ = 0;
		return 1;
	}

	//��������ƴ����ֱ��ת��������*
	if ((CON_NULL == syllable.con || CON_END == syllable.con || CON_ANY == syllable.con) &&
		(VOW_NULL == syllable.vow || VOW_END == syllable.vow || VOW_ANY == syllable.vow))
	{
		con_str = con_to_string[syllable.con];
		vow_str = vow_to_string[syllable.vow];

		while(*con_str && count < length - 1)
			str[count++] = *con_str++;

		while(*vow_str && count < length - 1)
			str[count++] = *vow_str++;
	}
	//������ƴ��������ȡ����Ҫ��Ϊ��ʵ��ƴ���ݴ�
	else
	{
		str[0] = 0;

		for (i = 0; i < share_segment->syllable_map_items; i++)
		{
			if (syllable.con != share_segment->syllable_map[i].con || 
				syllable.vow != share_segment->syllable_map[i].vow/* ||
				correct_flag != share_segment->syllable_map[i].correct_flag*/)
				continue;

			for (j = 0; j <= 3; j++)
			{
				if (!(syllable.tone & (1 << j)))
					continue;

				if (count++)
					_tcscat_s(str, length, TEXT(","));

				if (tone_char)
				{
					switch (j)
					{
					case 0:
						_tcscat_s(str, length, share_segment->syllable_map[i].pin_yin_tone1);
						break;

					case 1:
						_tcscat_s(str, length, share_segment->syllable_map[i].pin_yin_tone2);
						break;

					case 2:
						_tcscat_s(str, length, share_segment->syllable_map[i].pin_yin_tone3);
						break;

					case 3:
						_tcscat_s(str, length, share_segment->syllable_map[i].pin_yin_tone4);
						break;
					}
				}
				else
				{
					_tcscat_s(str, length, share_segment->syllable_map[i].pin_yin);
					_tcscat_s(str, length, tone_to_string[1i64 << j]);
				}
			}

			if (!count)
				_tcscpy_s(str, length, share_segment->syllable_map[i].pin_yin);

			count = (int)_tcslen(str);
			break;
		}
	}

	str[count] = 0;

	return count;
}

//�õ�ȫƴ���ַ�����ƴ��ȫ����
//������������ã�������ʱ����Ҫ����ע�͵�
//void GetAllSyllableStringQP(PIMCONTEXT *context, TCHAR *pinyin ,unsigned int pinyin_len)
//{
//	int i;
//	TCHAR py[0x10];
//	for (i = 0; i < context->syllable_count; i++)
//	{
//		GetSyllableString(context->syllables[i], py, _SizeOf(py), context->syllable_correct_flag[i], 0);
//		if (_tcslen(pinyin) + _tcslen(py) < pinyin_len - 1)
//			_tcscat_s(pinyin, pinyin_len, py);
//	}
//}

/*	������ڵ�˫ƴƴ������
 *	������
 *		syllable		����
 *		str				�����
 *		length			���봮�ĳ���
 *	����ֵ��
 *		�ַ������ȣ�0��ʶû���ַ���
 */
int GetSyllableStringSP(SYLLABLE syllable, TCHAR *str, int length)
{
	TCHAR *tone_str;
 	int  count = 0;
	int  i;

	if (!share_segment)
		return 0;

	//CON_ERROR��VOW_ERROR
	if (syllable.con < CON_NULL || syllable.vow < VOW_NULL || syllable.con > CON_ANY || syllable.vow > VOW_ANY ||
		syllable.tone < 0 || syllable.tone > 8)
	{
		*str++ = '?';
		*str++ = 0;
		return 1;
	}

	if (syllable.con == CON_ANY && syllable.vow == VOW_ANY)
	{
		*str++ = '*';
		*str++ = 0;
		return 1;
	}

	for (i = 0; i < share_segment->syllable_map_items; i++)
	{
		if (share_segment->syllable_map[i].con == syllable.con && 
			share_segment->syllable_map[i].vow == syllable.vow)
			break;
	}

	if (i == share_segment->syllable_map_items)
	{
		str[0] = 0;
		return 0;
	}

	_tcscpy_s(str, length, share_segment->syllable_map[i].sp_pin_yin);
	tone_str = tone_to_string[syllable.tone];
	_tcscat_s(str, length, tone_str);

	return (int)_tcslen(str);
}

/*	�ж���������ĸ�Ƿ��������ĸ��ͬ�����ڵ���ȡ�ʡ�
 *	������
 *		ch				�����жϵ���ĸ
 *		syllable		�����жϵ�����
 *	���أ�
 *		1��������0��������
 */
int SyllableStartWithLetter(TCHAR ch, SYLLABLE syllable)
{
	if (ch == SYLLABLE_ANY_CHAR)
		return 1;

	if (syllable.con != CON_NULL)			//����ĸ
		return ch == con_to_string[syllable.con][0];

	if (syllable.vow != VOW_NULL)			//û����ĸ
		return ch == vow_to_string[syllable.vow][0];

	return 0;
}

/*	�Ƚ�һ�����ڣ�����ͨ����ıȽϣ������������Ƚϡ�
 *	������ڿ��ܺ���ͨ�����������������в�������Щ��
 *	��Ӧ�������������ϵ�ͨ�����
 *	������
 *		syllables					�Ƚϵ�����
 *		length						�Ƚ����ڳ���
 *		checked_syllables			���Ƚϵ���������
 *		ci							���ȽϵĴ�����
 *		checked_length				���ȽϵĴʳ���
 *		fuzzy_mode					ģ����ʽ
 *	���أ�
 *		�����1
 *		�������0
 */
int WildCompareSyllablesWithCi(SYLLABLE *syllables, int length, SYLLABLE *checked_syllables, HZ *ci, int checked_length, int fuzzy_mode)
{
	int i, j;

	//���Ƚ����ڳ��ȴ��ڵ��ڱȽ�����
	if (length > checked_length)
		return 0;

	for (i = 0; i < length; i++)
	{
		if (syllables[i].con != CON_ANY)		//����ͨ���
		{
			//��ƥ��
			if (syllables[i].tone != TONE_0 && !ZiContainTone(ci[i], checked_syllables[i], syllables[i].tone))
				return 0;

			continue;
		}

		//ͨ���
		if (i == length - 1)		//���һ����ͨ������϶�ƥ��
			return 1;

		//��Ҫʹ�õݹ�������
		for (j = i + 1; j < checked_length; j++)
		{
			if (WildCompareSyllablesWithCi(
				syllables + i + 1,				//����ͨ���
				length - i - 1,					//���ȼ���
				checked_syllables + j,			//���Ƚ�����
				ci + j,							//���ȽϵĴ�
				checked_length - j,				//���Ƚϳ���
				fuzzy_mode
				))
				return 1;
		}
		break;
	}

	//���ıȽϱ�����ϱȽϳ̶ȵ��ڱ��Ƚϳ��ȣ������
	//���ֲ���ƥ�䣬�磺zg*j  �� zgrmjcy�໥ƥ�䡣
	return i == length && length == checked_length;
}

/*	�Ƚ�һ�����ڣ�����ͨ����ıȽϣ������������Ƚϡ�
 *	������ڿ��ܺ���ͨ�����������������в�������Щ��
 *	��Ӧ�������������ϵ�ͨ�����
 *	������
 *		syllables					�Ƚϵ�����
 *		length						�Ƚ����ڳ���
 *		checked_syllables			���Ƚ�����
 *		checked_length				���Ƚ����ڵĳ���
 *		fuzzy_mode					ģ����ʽ
 *	���أ�
 *		�����1
 *		�������0
 */
int WildCompareSyllables(SYLLABLE *syllables, int length, SYLLABLE *checked_syllables, int checked_length, int fuzzy_mode)
{
	int i, j;

	//���Ƚ����ڳ��ȴ��ڵ��ڱȽ�����
	if (length > checked_length)
		return 0;

	for (i = 0; i < length; i++)
	{
		if (syllables[i].con != CON_ANY)		//����ͨ���
		{
			if (!ContainSyllable(syllables[i], checked_syllables[i], fuzzy_mode))
				return 0;		//��ƥ��
			continue;
		}

		//ͨ���
		if (i == length - 1)		//���һ����ͨ������϶�ƥ��
			return 1;

		//��Ҫʹ�õݹ�������
		for (j = i + 1; j < checked_length; j++)
		{
			if (WildCompareSyllables(
				syllables + i + 1,				//����ͨ���
				length - i - 1,					//���ȼ���
				checked_syllables + j,			//����ͨ���
				checked_length - j,				//���ȼ���
				fuzzy_mode))
				return 1;
		}
		break;
	}

	//���ıȽϱ�����ϱȽϳ̶ȵ��ڱ��Ƚϳ��ȣ������
	//���ֲ���ƥ�䣬�磺zg*j  �� zgrmjcy�໥ƥ�䡣
	return i == length && length == checked_length;
}

/*	�Ƚ�һ�����ڣ�������ͨ����ıȽϣ������������Ƚϡ�
 *	ע���������������ϵ�ͨ�����
 *	������
 *		syllables					�Ƚϵ�����
 *		checked_syllables			���Ƚ�����
 *		length						�Ƚ����ڳ���
 *		fuzzy_mode					ģ����ʽ
 *	���أ�
 *		�����1
 *		�������0
 */
int CompareSyllables(SYLLABLE *syllables, SYLLABLE *checked_syllables, int length, int fuzzy_mode)
{
	int i;

	//���ν��бȽ�
	for (i = 0; i < length; i++)
		if (!ContainSyllable(syllables[i], checked_syllables[i], fuzzy_mode))
			return 0;		//��ƥ��

	return 1;
}

/*	�Ƚ�һ�����ڵ�����ĸ�Ƿ���Ŀ�����������ͨ����ıȽϣ�
 *	�����������Ƚϡ�
 *	������
 *		letters						�Ƚϵ�����ĸ����
 *		length						�Ƚ�����ĸ����
 *		checked_syllables			���Ƚ�����
 *		checked_length				���Ƚ����ڵĳ���
 *	���أ�
 *		�����1
 *		�������0
 */
int WildCompareSyllablesAndLetters(const TCHAR *letters, int length, SYLLABLE *checked_syllables, int checked_length)
{
	int i, j;

	//���Ƚ����ڳ��ȱ�����ڵ��ڱȽ�����
	if (checked_length < length)
		return 0;

	for (i = 0; i < length; i++)
	{
		if (letters[i] != SYLLABLE_ANY_CHAR)		//����ͨ���
		{
			if (!SyllableStartWithLetter(letters[i], checked_syllables[i]))
				return 0;			//��ƥ��
			continue;
		}

		if (i == length - 1)		//���һ����ͨ���
			return 1;

		//ͨ���
		for (j = i + 1; j < checked_length; j++)
			if (WildCompareSyllablesAndLetters(
				letters + i + 1,
				length - i - 1,
				checked_syllables + j,
				checked_length - j))
				return 1;

		//��ƥ��
		return 0;
	}

	return i == length && length == checked_length;
}

/*	�Ƚ�һ�����ڵ�����ĸ�Ƿ���Ŀ�������������ͨ����ıȽϣ������������Ƚϡ�
 *	������
 *		letters						�Ƚϵ�����ĸ����
 *		checked_syllables			���Ƚ�����
 *		length						�Ƚ�����ĸ����
 *	���أ�
 *		�����1
 *		�������0
 */
int CompareSyllablesAndLetters(const TCHAR *letters, SYLLABLE *checked_syllables, int length)
{
	int i;

	for (i = 0; i < length; i++)
		if (!SyllableStartWithLetter(letters[i], checked_syllables[i]))
			return 0;			//��ƥ��

	return 1;
}

/*	�ж��Ƿ�Ϊ�������ַ�
 *	������
 *		ch			�ַ�
 *	���أ�
 *		�ǣ�1
 *		��0
 */
int IsToneChar(char ch)
{
	return (ch == TONE_CHAR_1 || ch == TONE_CHAR_2 || ch == TONE_CHAR_3 || ch == TONE_CHAR_4);
}

/**	�ж����������Ƿ���ȫ���
 */
int SameSyllable(SYLLABLE s1, SYLLABLE s2)
{
	return *(unsigned short*)&s1 == *(unsigned short*)&s2;
}

/**	�Ƿ�������ڷָ���
 */
int HasSyllableSeparator(const TCHAR *str)
{
	while(*str)
	{
		if (*str++ == SYLLABLE_SEPARATOR_CHAR)
			return 1;
	}

	return 0;
}

#define	SECTION1		TEXT("[��ĸ]")
#define	SECTION2		TEXT("[��ĸ]")
#define	SECTION3		TEXT("[����ĸ���ڵ���ĸ]")

/*	װ��˫ƴINI�ļ�����
 */
int LoadSPIniData(const TCHAR *ini_name)
{
	TCHAR *buffer;
	TCHAR line[0x400];
	int  length, count, index, i;
	TCHAR *sp_string1, *sp_string2;
	int  section = 0;

	if (share_segment->sp_loaded)
		return 1;

	memcpy(share_segment->vow_sp_string_single, share_segment->vow_sp_string_single_save, sizeof(share_segment->vow_sp_string_single));
	memcpy(share_segment->vow_sp_string, share_segment->vow_sp_string_save, sizeof(share_segment->vow_sp_string));
	memcpy(share_segment->con_sp_string, share_segment->con_sp_string_save, sizeof(share_segment->con_sp_string));

	share_segment->sp_used_v = share_segment->sp_used_i = 0;

	length = GetFileLength(ini_name);
	if (length <= 0)
		return 0;

	buffer = (TCHAR *)malloc(length);
	if (!LoadFromFile(ini_name, buffer, length))
	{
		free(buffer);
		return 0;
	}

	//���˫ƴ������Ŀ
	index = 1;
	length = length / sizeof(TCHAR);

	while(index < length)
	{
		count = 0;
		while(index < length && count < _SizeOf(line) - 1)
		{
			line[count++] = buffer[index++];
			if (buffer[index - 1] == 0xa)
			{
				count--;
				break;
			}
		}

		line[count] = 0;		//�õ�һ������

		if ('#' == line[0] || ';' == line[0])		//ע��
			continue;

		if (count && (line[count - 1] == 0xd || line[count - 1] == 0xa))
			line[count - 1] = 0;

		if (!_tcscmp(SECTION1, line))
			section = 1;
		else if (!_tcscmp(SECTION2, line))
			section = 2;
		else if (!_tcscmp(SECTION3, line))
			section = 3;

		if (!section)
			continue;

		sp_string1 = _tcstok(line, TEXT("="));
		sp_string2 = _tcstok(0, TEXT("="));
		if (!sp_string1 || !sp_string2)
			continue;

		_tcslwr_s(sp_string2, 8);	//��ΪСд

		//����CON��VOW�������滻
		if (section == 1)
		{
			for (i = 0; i < CON_ANY; i++)
			{
				if (!_tcscmp(sp_string1, share_segment->con_sp_string_save[i]))
				{
					_tcscpy_s(share_segment->con_sp_string[i], 8, sp_string2);
					if (sp_string2[0] == 'v')
						share_segment->sp_used_v = 1;
					else if (sp_string2[0] == 'i')
						share_segment->sp_used_i = 1;
					else if (sp_string2[0] == 'u')
						share_segment->sp_used_u = 1;

					break;
				}
			}

			continue;
		}

		if (section == 2)
		{
			for (i = 0; i < VOW_ANY; i++)
			{
				if (!_tcscmp(sp_string1, share_segment->vow_sp_string_save[i]))
				{
					_tcscpy_s(share_segment->vow_sp_string[i], 8, sp_string2);
					if (sp_string2[0] == 'v')
						share_segment->sp_used_v = 1;
					else if (sp_string2[0] == 'i')
						share_segment->sp_used_i = 1;
					else if (sp_string2[0] == 'u')
						share_segment->sp_used_u = 1;

					break;
				}
			}

			continue;
		}

		if (section == 3)
		{
			for (i = 0; i < VOW_ANY; i++)
			{
				if (!_tcscmp(sp_string1, share_segment->vow_sp_string_single_save[i]))
				{
					_tcscpy_s(share_segment->vow_sp_string_single[i], 8, sp_string2);
					break;
				}
			}

			continue;
		}
	}

	free(buffer);

	//�����ڱ�����ַ������
	for (i = 0; i < share_segment->syllable_map_items; i++)
	{
		int con = share_segment->syllable_map[i].con;
		int vow = share_segment->syllable_map[i].vow;

		if (con == CON_NULL)
		{
			_tcscpy_s(line, _SizeOf(line), share_segment->vow_sp_string_single[vow]);
		}
		else
		{
			_tcscpy_s(line, _SizeOf(line), share_segment->con_sp_string[con]);
			_tcscat_s(line, _SizeOf(line), share_segment->vow_sp_string[vow]);
		}

		if (_tcslen(line) <= 2)
			_tcscpy_s(share_segment->syllable_map[i].sp_pin_yin, _SizeOf(share_segment->syllable_map[0].sp_pin_yin), line);
		else
			Log(LOG_ID, L"˫ƴ������ڴ���, %s", share_segment->con_sp_string[con], share_segment->vow_sp_string[vow]);
	}

	share_segment->sp_loaded = 1;
	return 1;
}

/*	ж��˫ƴ����
 */
int FreeSPIniData()
{
	share_segment->sp_used_v = share_segment->sp_used_i = share_segment->sp_loaded = 0;
	return 1;
}

void InsertHint(TCHAR *hint_buffer, int buffer_length, const TCHAR *src, const TCHAR *tag)
{
	TCHAR upper_str[0x100];
	int  i;
	static int count;

	if (!*hint_buffer)
		count = 0;

	if (!_tcscmp(src, tag))
		return;

	_tcscpy_s(upper_str, _SizeOf(upper_str), src);
	for (i = 0; upper_str[i]; i++)
		if (upper_str[i] >= 'a' && upper_str[i] <= 'z')
			upper_str[i] = upper_str[i] ^ 0x20;

	_tcscat_s(hint_buffer, buffer_length, tag);
	_tcscat_s(hint_buffer, buffer_length, TEXT("="));
	_tcscat_s(hint_buffer, buffer_length, upper_str);
	_tcscat_s(hint_buffer, buffer_length, TEXT(" "));

	count++;
	if (!(count % 8))
		_tcscat_s(hint_buffer, buffer_length, TEXT("\n"));
}

/**	��ȡ˫ƴ��ʾ��Ϣ
 */
void GetSPHintString(PIMCONTEXT *context, TCHAR *hint_buffer, int buffer_length)
{
	TCHAR last_ch = 0;
	int  mode;		//0: input start, 1: single vow, 2: has con
	int  i;
	int  con, vow;
	const TCHAR *src_string, *tag_string;

	if (!share_segment)
		return;

	*hint_buffer = 0;

	if (!context->input_length)
	{
		mode = 0;
	}
	else if (context->syllable_count)
	{
		con = context->syllables[context->syllable_count - 1].con;
		vow = context->syllables[context->syllable_count - 1].vow;

		if (vow == VOW_NULL)
			mode = 2;
		else
			mode = 0;
	}
	else
	{
		last_ch = context->input_string[context->input_length - 1];
		if (last_ch < 'a' || last_ch > 'z')
			return;

		mode = 1;
	}

	if (mode == 0)		//Ŀǰû������
	{
		for (i = 0; i < share_segment->syllable_map_items; i++)
		{
			/*if (share_segment->syllable_map[i].correct_flag)
				continue;*/

			if (share_segment->syllable_map[i].con == CON_NULL)	//����ĸ
			{
				vow = share_segment->syllable_map[i].vow;

				src_string = share_segment->vow_sp_string_single[vow];
				tag_string = share_segment->vow_sp_string_single_save[vow];
				InsertHint(hint_buffer, buffer_length, src_string, tag_string);
				continue;
			}

			if (share_segment->syllable_map[i].vow != VOW_NULL)
				continue;

			//������ĸ���
			con = share_segment->syllable_map[i].con;
			src_string = share_segment->con_sp_string[con];
			tag_string = share_segment->con_sp_string_save[con];

			InsertHint(hint_buffer, buffer_length, src_string, tag_string);
		}

		return;
	}

	//mode1, ����ĸ���
	if (mode == 1)
	{
		for (i = 0; i < share_segment->syllable_map_items; i++)
		{
			/*if (share_segment->syllable_map[i].correct_flag)
				continue;*/

			if (share_segment->syllable_map[i].con != CON_NULL)
				continue;

			vow = share_segment->syllable_map[i].vow;
			src_string = share_segment->vow_sp_string_single[vow];
			tag_string = share_segment->vow_sp_string_single_save[vow];

			if (src_string[0] != last_ch)
				continue;

			InsertHint(hint_buffer, buffer_length, src_string, tag_string);
		}

		return;
	}

	//mode2, ������ĸ�����
	if (mode == 2)
	{
		for (i = 0; i < share_segment->syllable_map_items; i++)
		{
			/*if (share_segment->syllable_map[i].correct_flag)
				continue;*/

			if (share_segment->syllable_map[i].con != con)
				continue;

			if (share_segment->syllable_map[i].vow == VOW_UE && 
				share_segment->syllable_map[i].pin_yin_length >= 2)
			{
				if (share_segment->syllable_map[i].pin_yin[share_segment->syllable_map[i].pin_yin_length - 2] == 'v' &&
					share_segment->syllable_map[i].pin_yin[share_segment->syllable_map[i].pin_yin_length - 1] == 'e')
					continue;
			}

			vow = share_segment->syllable_map[i].vow;
			src_string = share_segment->vow_sp_string[vow];
			tag_string = share_segment->vow_sp_string_save[vow];

			InsertHint(hint_buffer, buffer_length, src_string, tag_string);
		}

		return;
	}

	return;
}

/**	�ж��Ƿ�Ϊ������ƴ�����ڡ�
 *	�������ڣ����ܰ���ͨ������Լ���ĸ��ƴ������
 */
int IsFullSyllable(SYLLABLE *syllable_array, int syllable_count)
{
	int i;

	for (i = 0; i < syllable_count; i++)
	{
		if (syllable_array[i].vow == VOW_NULL)		//����������
			return 0;

		if (syllable_array[i].con == CON_ANY)		//ͨ�����ֱ�ӷ���
			return 0;
	}
	return 1;
}

/**	�ж��Ƿ��������һ�ֽ�����ʽ��
 *	�磺lianai  -> lia'nai(ori)
 *				-> lian'ai(other)
 *		dangan	-> dan'gan(ori)
 *				-> dang'an(other)
 *		xian	-> xi'an
 *		xiang	-> xi'ang
 *	���÷�ʽ������ƴ�����÷������ƥ���㷨�������ڲ�֣�����
 *	��Ҫ������������ƥ�䣻������ĵĽ����Ҫ�ж��Ƿ�Ϊ����
 *	ƴ����
 *
 *	������
 *		context
 *		syllable_start
 *		syllable_count
 *		other_syllable_array
 *		other_syllable_count
 *	���أ�
 *		������һ�ֲ�ַ�ʽ������������Ŀ
 *		�����ڣ�0
 */
int GetOtherSyllableParse(PIMCONTEXT *context, int syllable_start, int syllable_count, SYLLABLE *other_syllables, int other_length)
{
	TCHAR syllable_string[MAX_SYLLABLE_PER_INPUT * MAX_PINYIN_LENGTH];
	int  string_length = 0;
	int  i, start_pos, end_pos;
	int  other_count;

	//˫ƴ����Ҫ���м���
	if (pim_config->pinyin_mode == PINYIN_SHUANGPIN/* || !pim_config->special_parse_pin_yin*/)
		return 0;

	//ĿǰMAX_OTHER_PARSE_SYLLABLEΪ32����δ������
	if (syllable_count > MAX_OTHER_PARSE_SYLLABLE)
		return 0;

	//��ȡ�����ַ���
	start_pos = context->syllable_start_pos[syllable_start];
	end_pos   = (syllable_count == context->syllable_count - syllable_start) ?
				 -1 : context->syllable_start_pos[syllable_start + syllable_count];

	if (end_pos == -1)
		_tcscpy_s(syllable_string, _SizeOf(syllable_string), context->input_string + start_pos);
	else
		_tcsncpy_s(syllable_string, _SizeOf(syllable_string), context->input_string + start_pos, end_pos - start_pos);

	//���зָ��������ڴ����������
	if (HasSyllableSeparator(syllable_string))
		return 0;

	//�����Ѿ�ӵ����ƴ���������Խ��з�������ˡ�
	other_count = ParsePinYinString(syllable_string, other_syllables, other_length, pim_config->use_fuzzy ? pim_config->fuzzy_mode : 0/*, 0, 0*/);

	//������ƴ����
	if (!IsFullSyllable(other_syllables, other_count))
		return 0;

	if (other_count != syllable_count)		//�����������ͬ
		return other_count;

	//�ж��Ƿ�������ȫ��ͬ
	for (i = 0; i < syllable_count; i++)
		if (*(short*)&other_syllables[i] != *(short*)&context->syllables[i + syllable_start])
			return other_count;

	return 0;
}

/**	�ж�һ�������Ƿ���Բ��Ϊ����С����
 *
 *	���أ�
 *		�ɲ�֣�1
 *		���ɣ�  0
 */
static int IsSmallSyllable(SYLLABLE syllable)
{
	int i;

	for (i = 0; i < _SizeOf(small_syllable_items); i++)
	{
		if (small_syllable_items[i].syllable.con == syllable.con &&
			small_syllable_items[i].syllable.vow == syllable.vow)
		{
			return 1;
		}
	}
	
	return 0;
}

/** ����GetSmallSyllablesParse����ɵݹ��㷨
*/
static int ProcessSmallSyllables(SYLLABLE *syllables, int syllable_count, SYLLABLE *other_arrays, int *other_lengths, int syllable_pos, int other_count)
{
	SYLLABLE small_syllables[2];
	SYLLABLE ori_syllable;
	int i, j;

	for (i = syllable_pos; i < syllable_count; i++)
	{
		//ÿһ���ɲ�ֵĵ������ڶ��ɿ�����һ����㣬�ɴ˽�㿪ʼ�ֳ�����
		//������еݹ飬�γ�һ�������ڶ������Ľṹ
		if (IsSmallSyllable(syllables[i]))
		{
			other_count = ProcessSmallSyllables(syllables, syllable_count, other_arrays, other_lengths, i + 1, other_count);

			GetSmallSyllables(syllables[i], small_syllables);

			//�����в�֣������������ֱ�ӷ���
			if (syllable_count + 1 > MAX_SYLLABLE_PER_INPUT)
			{
				return other_count;
			}

			//��small_syllables�滻��ԭ����
			ori_syllable = syllables[i];
			for (j = syllable_count - 1; j >= i + 1; j--)
			{
				syllables[j + 1] = syllables[j];
			}
			syllables[i] = small_syllables[0];
			syllables[i + 1] = small_syllables[1];

			other_count = ProcessSmallSyllables(syllables, syllable_count + 1, other_arrays, other_lengths, i + 2, other_count);

		    //��ԭ�����滻��small_syllables(�������ǳ���Ҫ����Ϊ���еݹ麯��
			//ʹ�õ�ƴ��������syllables���������ԭ�����������ݹ��֧����Ӱ��)
			for (j = i + 1; j <= syllable_count + 1 - 1; j++) //ע���ʱʵ�ʵ�������Ϊsyllable_count+1
			{
				syllables[j - 1] = syllables[j];
			}
			syllables[i] = ori_syllable;

			//�ر�ע�⣬�ݹ���ɺ�������ֹѭ������Ϊ��ʱ�Ե�ǰ���Ϊ���ķ�֧
			//�Ѿ�������ɣ�Ӧ��������������һ�㣻��������ظ�������������
			//������
			break;
		}
	}

	//����ƴ����ĩβ������һ���������
	if (i >= syllable_count)
	{
		for (i = 0; i < syllable_count; i++)
		{
			(other_arrays + other_count * MAX_SYLLABLE_PER_INPUT)[i] = syllables[i];
		}
		*(other_lengths + other_count) = syllable_count;
		other_count++;
	}

	return other_count;
}

/**	�ж��Ƿ����ͨ����ֵ��ֵ����ڻ�����ڵ���һ�ֽ�����ʽ
 *	�磺lianaiting  -> lianting(ori)
 *				    -> lian'an'ting(other)
 *		xianshi  	-> xian'shi(ori)
 *				    -> xi'an'shi(other)
 *		jieruchou   -> jie'ru'chou(ori)
 *                  -> ji'e'ru'chou(other)
 *	���÷�ʽ�����μ��ÿһ�����ڣ����Ƿ���small_syllable_items�У�
 *  ����������Ž������ڲ�ֳ��������ڣ����ڲ��ǰ���ƴ��������
 *  ����Ϊһ�����������һ�����ڿ�ʼ�ݹ飬���ձ��������еĲ�ֽ��
 *
 *	������
 *		syllables               ��������ƴ����
 *		syllable_count          syllables�����ڵ���Ŀ
 *		syallable_arrays        ������飬ÿ��Ԫ����һ��ƴ����
 *		other_length            syallable_arraysÿ������������Ŀ
 *
 *	���أ�
 *		��������Ľ�����ʽ��    ������ʽ����Ŀ
 *		�����ڣ�                0
 */
int GetSmallSyllablesParse(SYLLABLE *syllables, int syllable_count, SYLLABLE *other_arrays, int *other_lengths)
{
	int  small_syllable_count = 0;
	int  other_count = 0;
	int  i;

	//˫ƴ����Ҫ���м���
	if (pim_config->pinyin_mode == PINYIN_SHUANGPIN/* || !pim_config->special_parse_pin_yin*/)
		return 0;

	//���ô����ֵ����GetCandidates::ProcessZiCandidates����ר�Ŵ��������ڵĴ���
	if (syllable_count <= 1)
	{
		return 0;
	}

	//����ܴ���Ŀɲ�������������һ�����ֵ����������ֲ�ַ�ʽ��
	//ĳ������k�����ֿɲ�֣�����2^k�ֲ�ͬ�Ľ�����ʽ��Ҫ��飬��
	//���ɻ���ɼ���������������ҵ����ŵ��㷨ǰ��Ҫ����һЩ��
	//��
	for (i = 0; i < syllable_count; i++)
	{
		if (IsSmallSyllable(syllables[i]))
		{
			small_syllable_count++;
		}
	}
	if (small_syllable_count > MAX_SMALL_SYLLABLES)		
		return 0;

	//�ݹ��㷨��ע������ĺ����ڵ��ù����п��ܸı�syllables��������
	//��ɺ�Ὣ��ָ����͵���֮ǰһ��
	other_count = ProcessSmallSyllables(syllables, syllable_count, other_arrays, other_lengths, 0, 0);

	if (other_count)
	{
		other_count--; //���ݵݹ��㷨����һ�������������ԭƴ������Ӧ�ų�
		memmove(other_arrays, other_arrays + MAX_SYLLABLE_PER_INPUT, other_count * MAX_SYLLABLE_PER_INPUT * sizeof(SYLLABLE));
		memmove(other_lengths, other_lengths + 1, other_count * sizeof(int));
	}

	return other_count;
}

//��õ������ڲ�ֳɵ�����С����
int GetSmallSyllables(SYLLABLE syllable, SYLLABLE *small_syllables)
{
	int i;

	for (i = 0; i < sizeof(small_syllable_items) / sizeof(small_syllable_items[0]); i++)
	{
		if (small_syllable_items[i].syllable.con == syllable.con &&
			small_syllable_items[i].syllable.vow == syllable.vow)
		{
			*(short*)&small_syllables[0] = *(short*)&small_syllable_items[i].small_syllables[0];
			*(short*)&small_syllables[1] = *(short*)&small_syllable_items[i].small_syllables[1];
			small_syllables[1].tone = syllable.tone;

			return 1;
		}
	}

	return 0;
}