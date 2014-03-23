#ifndef	_SYMBOL_H_
#define	_SYMBOL_H_

#ifdef	__cplusplus
extern "C"
{
#endif

#define	SYMBOL_NUMBER	32
#define	SYMBOL_LENGTH	5

#define	SYMBOL_INI_FILE_NAME	TEXT("unispim6\\ini\\���ķ���.ini")

typedef struct tagSYMBOLITEM
{
	TCHAR	english_ch;								//Ӣ�ķ����ַ�
	TCHAR	english_symbol[2];						//Ӣ�ķ��Ŵ�
	TCHAR	chinese_symbol[SYMBOL_LENGTH];			//���ķ��Ŵ�
	TCHAR	default_chinese_symbol[SYMBOL_LENGTH];	//Ĭ�����ķ��Ŵ�
}SYMBOLITEM;

struct tagPIMCONTEXT;

extern const TCHAR *GetSymbol(struct tagPIMCONTEXT *context, TCHAR ch);
extern int IsSymbolChar(TCHAR ch);
extern int LoadSymbolData(const TCHAR*);
extern int FreeSymbolData();
extern void CheckQuoteInput(HZ symbol);
extern void GetFullShapes(const TCHAR *src, TCHAR *dest, int dest_len); 

#ifdef	__cplusplus
}
#endif

#endif