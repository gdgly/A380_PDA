#ifndef COMMON_H
#define COMMON_H


// --------------------------------		���Ͷ���	-----------------------------------------

typedef unsigned char bool;
#ifndef true
#define true    1
#endif
#ifndef false
#define false   0
#endif

#define VERSION_Name    "ɣ��6009�ֳֻ�"     // ������
#define VERSION_RevNo   "1.0"               // �汾��
#define VERSION_Date    "2019-5-9"          // �汾����

#define TXTBUF_LEN	20      // �ı���������ַ���
#define RELAY_MAX   3       // ����м̸���
#define UI_MAX      10

/*  ��������˿ڣ� NO.1 / NO.2 / NO.3
        ����NO.1 (TP_PORT_TXD / TP_PORT_RXD)
        ����NO.2 (TP_COM_TXD / TP_COM_RXD)
        ����NO.3 (TP_SCAN_TXD / TP_SCAN_RXD) 

    �����߼��˿ڣ�1.���� / 2.��ͨ���� / 3.���ٺ���
        _ComSetTran(logicPort)
        ��ʹ��ǰ��������Ч������˿� NO.1 / NO.2 / No.3��
        ��ʹ�á����ٺ��⡰,����ѡ������˿�NO.1 �� No.3�� 
            ϵͳ����->ģ������->��������->���ٺ���->
*/
#define Trans_Scom          1   // ɨ��˿�
#define Trans_IR            2   // ��ͨ���� 
#define Trans_IR_Quick      3   // ���ٺ���

#define RfPort  Trans_IR_Quick  // RF͸��ģ��ʹ�õ�ͨ�Ŷ˿�

typedef enum{
    Color_White = 0,
    Color_Black = 1
}GUI_COLOR;

typedef struct{
	uint8 buf[200];
	uint8 *items[10];
	uint8 itemCnt;
    uint8 lastItemLen;
}ParamsBuf;

typedef enum{
    UI_TxtBox,
    UI_CombBox
}UI_Type;

typedef struct{
    
    uint8 x;        // title position
    uint8 y;
    char *title;
    uint8 x1;       // text position
    uint8 y1;
    char *text;
    uint8 width;    // text width
    UI_Type type;

    struct{
		uint8 cnt;
		uint8 currIdx;
	}combox;

	struct {
		uint8 dataLen;
	}txtbox;
}UI_Item;

typedef struct{
    uint8 uiBuf[200];
    UI_Item items[UI_MAX];
    uint8 cnt;
}UI_ItemList;


//---------------------------------		��������	 -----------------------------------------
int IndexOf(const uint8 * srcArray, int srcLen, const uint8 * dstBytes, int dstLen, int startIndex, int offset);
uint16 GetCrc16(uint8 *Buf, uint16 Len, uint16 Seed);
uint8 GetCrc8(uint8 *Buf, int len);
uint8 GetSum8(uint8 *buf, uint16 len);
char HexToChar(uint8 b);
uint8 CharToHex(char c);
int GetStringHexFromBytes(char * strHex, uint8 bytes[], int iStart, int iLength, char separate, bool reverse);
int GetBytesFromStringHex(uint8 bytes[], int iStart, int iLength, const char * strHex, char separate, bool reverse);
void StringPadLeft(const char * srcStr, int totalLen, char padChar);
int StringTrimStart(const char * srcStr, char trimChar);
void ShowProgressBar(uint8 y, uint32 maxValue, uint32 currValue);
void TextBoxCreate(UI_Item *item, uint8 x, uint8 y, const char * title, char * text, uint8 maxLen, uint8 width);
uint8 TextBoxGetStr(uint8 x, uint8 y, const char * title, char * text, uint8 maxLen);
uint8 ShowUI(UI_ItemList inputList, uint8 *itemNo);
void PrintfXyMultiLine_VaList(uint8 x, uint8 y, const char * format, ...);
void PrintfXyMultiLine(uint8 x, uint8 y, const char * buf, uint8 maxLines);
uint8 GetPrintLines(uint8 x, const char * buf, char * lines[]);

//--------------------------------		ȫ�ֱ���	 ---------------------------------------
extern uint8 TmpBuf[];
extern uint8 LocalAddr[];
extern uint8 DstAddr[];
extern uint8 StrDstAddr[];

#endif
