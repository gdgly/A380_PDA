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

#define VerInfo_Name    (char *)"ɣ��6009�ֳֻ�"     // ������
#define VerInfo_RevNo   (char *)"2.2"               // �汾��
#define VerInfo_RevDate (char *)"2019-6-3"         // �汾����
//#define VerInfo_Release                             // ����ʱ���붨��ú꣬ ����ʱע��


#ifndef VerInfo_Release
#define Log_On      1           // ������־����
#define LogName     "debug.txt" // ��־�ļ���
#define LogScom_On  1           // ��־���ڿ��أ�1- ��������ڣ�0 -������ļ�
#define RxBeep_On   1       // ���ڽ������-������ʾ���أ� ��һ��- �����ɹ��� ������ - ����ʧ��
#else
#define Log_On      0 
#define RxBeep_On   1       
#endif

#define TXTBUF_LEN	20      // �ı���������ַ���
#define RELAY_MAX   3       // ����м̸���
#define UI_MAX      10      // ���UI�ؼ���


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
	#define CurrBaud    (uint8 *)"1200,E,8,1"
    #define TransType   "����͸��"      // ͨ�ŷ�ʽ	
    #define AddrLen     8
#else //defined(Project_6009_RF)
	#define CurrPort    Trans_IR_Quick          
	#define CurrBaud    (uint8 *)"9600,E,8,1"
    #define Timeout     Trans_IR 
    #define TransType   "Lora͸��"      // ͨ�ŷ�ʽ	
    #define AddrLen     6
#endif

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
    UI_CombBox,
    UI_Lable
}UI_Type;

typedef struct{
    
    uint8 x;        // UI title position
    uint8 y;
    char *title;
    uint8 x1;       // UI text position
    uint8 y1;
    char *text;
    uint8 width;    // UI width
    uint8 height;   // UI high
    UI_Type type;
    
    struct{
        char *strs[10];
		uint8 cnt;
		uint8 *currIdx;
	}combox;

	struct {
		uint8 dataLen;
        uint8 isClear;
        uint8 dotEnable;
	}txtbox;
}UI_Item;

typedef struct{
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
uint8 BcdToDec(uint8 bcd);
uint8 DecToBcd(uint8 dec);
int GetStringHexFromBytes(char * strHex, uint8 bytes[], int iStart, int iLength, char separate, bool reverse);
int GetBytesFromStringHex(uint8 bytes[], int iStart, int iLength, const char * strHex, char separate, bool reverse);
void StringPadLeft(const char * srcStr, int totalLen, char padChar);
int StringTrimStart(const char * srcStr, char trimChar);
void ShowProgressBar(uint8 y, uint32 maxValue, uint32 currValue);
void LableCreate(UI_Item *item, uint8 x, uint8 y, const char * title);
void TextBoxCreate(UI_Item *item, uint8 x, uint8 y, const char * title, char * text, uint8 maxLen, uint8 width, bool isClear);
void CombBoxCreate(UI_Item *item, uint8 x, uint8 y, const char * title, uint8 * currIdx, uint32 maxCnt, ...);
uint8 ShowUI(UI_ItemList inputList, uint8 *itemNo);
void PrintfXyMultiLine_VaList(uint8 x, uint8 y, const char * format, ...);
void PrintfXyMultiLine(uint8 x, uint8 y, const char * buf, uint8 maxLines);
void PrintXyTriangle(uint8 x, uint8 y, uint8 direction);
uint8 GetPrintLines(uint8 x, const char * buf, char * lines[]);
void LogPrint(const char * format, ...);
void LogPrintBytes(const char *title, uint8 *buf, uint16 size);

//--------------------------------		ȫ�ֱ���	 ---------------------------------------
extern char Screenbuff[160*(160/3+1)*2]; 
extern uint8 TmpBuf[1080];
extern uint8 TxBuf[1080];
extern uint8 RxBuf[1080];
extern uint8 DispBuf[2048];
extern uint32 RxLen, TxLen;
extern const uint8 LocalAddr[9];
extern uint8 DstAddr[9];
extern uint8 VerInfo[41];
extern char StrBuf[10][TXTBUF_LEN];    // extend input buffer
extern char StrDstAddr[TXTBUF_LEN];
extern char StrRelayAddr[RELAY_MAX][TXTBUF_LEN];
extern UI_ItemList UiList;
extern uint16 CurrCmd;
extern ParamsBuf Addrs;		
extern ParamsBuf Args;

#endif
