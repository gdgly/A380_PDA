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

#define LogEnable   false       // ������־����
#define LogName     "debug.log"  // ��־�ļ���

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


// ��ǰ͸��ģ��ʹ�õ�ͨ�Ŷ˿� �� ������

#if defined(Project_6009_IR)
	#define CurrPort    Trans_IR                
	#define CurrBaud    (uint8 *)"4800,E,8,1"	
#else //defined(Project_6009_RF)
	#define CurrPort    Trans_IR_Quick          
	#define CurrBaud    (uint8 *)"9600,E,8,1"	
#endif

typedef enum{
    Color_White = 0,
    Color_Black = 1
}GUI_COLOR;

typedef struct{
	uint8 buf[2000];
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
void PrintXyTriangle(uint8 x, uint8 y, uint8 direction);
uint8 GetPrintLines(uint8 x, const char * buf, char * lines[]);
void LogToFile(const char * fileName, const char * format, ...);

//--------------------------------		ȫ�ֱ���	 ---------------------------------------
extern char Screenbuff[160*(160/3+1)*2]; 
extern uint8 TmpBuf[1080];
extern uint8 TxBuf[1080];
extern uint8 RxBuf[1080];
extern uint32 RxLen, TxLen;
extern const uint8 LocalAddr[7];
extern uint8 DstAddr[7];
extern uint8 VersionInfo[40];
extern uint8 StrDstAddr[TXTBUF_LEN];
extern uint8 StrRelayAddr[RELAY_MAX][TXTBUF_LEN];
extern UI_ItemList UiList;
extern uint8 CurrCmd;
extern ParamsBuf Addrs;		
extern ParamsBuf Args;
extern ParamsBuf Disps;

#endif
