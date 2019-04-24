#ifndef _HJLIB_H
#define _HJLIB_H

typedef unsigned char  uint8;                
typedef signed   char  int8;                     
typedef unsigned short uint16;                  
typedef signed   short int16;                   /* defined for signed 16-bits integer variable 		�з���16λ���ͱ��� */
typedef unsigned int   uint32;                  /* defined for unsigned 32-bits integer variable 	�޷���32λ���ͱ��� */
typedef signed   int   int32;                   /* defined for signed 32-bits integer variable 		�з���32λ���ͱ��� */
typedef float          fp32;                    /* single precision floating point variable (32bits) �����ȸ�������32λ���ȣ� */
typedef double         fp64;                    /* double precision floating point variable (64bits) ˫���ȸ�������64λ���ȣ� */
typedef unsigned char  UINT8;                   /* defined for unsigned 8-bits integer variable 	�޷���8λ���ͱ���  */
typedef signed   char  INT8;                    /* defined for signed 8-bits integer variable		�з���8λ���ͱ���  */
typedef unsigned short UINT16;                  /* defined for unsigned 16-bits integer variable 	�޷���16λ���ͱ��� */
typedef signed   short INT16;                   /* defined for signed 16-bits integer variable 		�з���16λ���ͱ��� */
typedef unsigned int   UINT32;                  /* defined for unsigned 32-bits integer variable 	�޷���32λ���ͱ��� */
typedef signed   int   INT32;                   /* defined for signed 32-bits integer variable 		�з���32λ���ͱ��� */
typedef unsigned char BOOLEAN;
typedef unsigned int size_t;

typedef struct 
{
unsigned int  bit0_1 : 2;
unsigned int   bit2 : 1;
unsigned int bit3 : 1;
unsigned int bit4 : 1;
unsigned int bit5 : 1;
unsigned int bit6 : 1;
unsigned int bit7 : 1;
unsigned int bit8_16 ;
}ChBit;


//��������
#define KEY_0            '0'   //  0��
#define KEY_1            '1'   //  1��
#define KEY_2            '2'   //  2��
#define KEY_3            '3'   //  3��
#define KEY_4            '4'   //  4��
#define KEY_5            '5'   //  5��
#define KEY_6            '6'   //  6��
#define KEY_7            '7'   //  7��
#define KEY_8            '8'   //  8��
#define KEY_9            '9'   //  9��
#define KEY_DOT          '.'   //  #��
#define KEY_KJ           '*'   //  *��   
#define KEY_UP           'U'   //���ϵ��� 
#define KEY_DOWN         'D'   //���µ���
#define KEY_LEFT         'L'   //���򵼺�
#define KEY_RIGHT        'R'   //���򵼺�
#define KEY_CANCEL       'C'   //ȡ����
#define KEY_DELETE       'T'   //ɾ����
#define KEY_ENTER        'E'   //ȷ�ϼ�
#define KEY_LIGHT        'G'   //�����
#define KEY_FUNC         'F'   //��ݼ�
#define KEY_ONOFF        'O'   //���ػ�����
#define KEY_NOHOOK       0xFF  //û�а��� 

#define KEY_SCAN         'S'   //
#define KEY_TAB          'B'   // 



#define S_IFMT		0170000
#define S_IFDIR		0040000 
#define S_IFREG		0100000

 struct _Field{
  
    uint16  offset;            
    uint16  len;                
    uint8   type;              
    uint8   adix  ;                  
  };
   
   

typedef  struct
{  uint32  left;			// ����λ��(���Ͻǵ�x����)
   uint32  top;			// ����λ��(���Ͻǵ�y����)
    
   uint32  width;		// ���ڿ��
   uint32  hight;		// ���ڸ߶�
   
   uint8   *title;		// ���������ָ�� (�����ַ�ΪASCII�ַ������������ܴ�������)
   uint8   *state;		// ����״̬��ָ�� (��Ϊ��ʱ����ʾ״̬��)   
} WINDOWS;


typedef struct { 
  int  x;
  int  y;
  int  x1;
  int  y1;
  uint8 color;
  char *str;
  char *lpCaption;
  uint8 itype;
}_GuiStrStru;

#define MMENU_NO 8
#define  SMENU_NO					8

typedef  struct
{  WINDOWS  *win;		
   uint32  left;			// ����λ��(���Ͻǵ�x����)
   uint32   top;			// ����λ��(���Ͻǵ�y����);			
   char     *title;
   uint8    no;						// ���˵�����   ע�⣬��350���������ֵ��8
   char     *str[MMENU_NO];			// ���˵��ַ���  ע�⣬��350���������ֵ��8
   char     *key[MMENU_NO];			// ���˵��ַ���  ע�⣬��350���������ֵ��8
   void    (*Function[SMENU_NO])(void);		// �Ӳ˵���Ӧ�ķ������  ע�⣬��350���������ֵ��8
   void    (*FunctionEx)(void);	    //�ǰ����ص����� 
   } _GuiMenuStru;
   
typedef  struct
{  WINDOWS  *win;					// ��������
   uint32   x;                      //��ʼX����
   uint32   y;                      //��ʼY����
   uint8    isRt;                  //�Ƿ������
   uint8    with;                   //���
   uint8    MaxNum;                 //һ��Ļ���ٸ�
   uint8    no;						//�ܹ��ж��ٸ�
   char     *title;                    //����
   char     *str[255] ;			        // ���˵��ַ���
  // uint8    defbar;                  //Ĭ��ѡ��
   } _GuiLisStru;

 typedef  struct
{  WINDOWS  *win;					// ��������
   uint32   x;                      //��ʼX����
   uint32   y;                      //��ʼY����
   uint8    isRt;                  //�Ƿ������
   uint8    with;                   //���
   uint8    MaxNum;                 //һ��Ļ���ٸ�
   uint32   no;						//�ܹ��ж��ٸ�
   char     *title;                    //����
   char     *str[255] ;			        // ���˵��ַ���
   uint32    defbar;                  //Ĭ��ѡ��
   } _GuiLisStruEx;
   
   
typedef  struct
{     
   uint32   x;                      //��ʼX����
   uint32   y;                      //��ʼY����   
   uint8    isRt;                  //�Ƿ������
   uint8    with;                   //���
   uint8    MaxNum;                 //һ��Ļ���ٸ�
   uint32   no;						//�ܹ��ж��ٸ�
   char     *title;                 //����
   char     **str;			        // ���˵��ַ���
   uint32   defbar;                //Ĭ��ѡ��
   void    (*Function)(uint32 *Key,uint32 *Serial);	    
   //����������ݿ�ģʽ Ƕ����������� key ,����, Serial ��ǰ��  �ص�������������޸Ĺؼ��ֺ����
   //��������ݿ�ģʽ ���ݿⷽʽ,key��λ8 �ǰ�����ֵ,��24λ�ǵ��������¼��ַ+0x81000000 Serial: �����8λ,��24λ�ǵڼ���Ļ
   
   uint32   type;                   //0 ����֮ǰ�ķ��,1,�����Ұ�������, 2�������ݿⷽʽ
   uint8   fieldname;  //�ֶ�����
   uint8   condition;  //���� '>'�� '<'�� '='�� '#'�����ڡ� '@' ���� ��'>'+'=' ���ڵ��� ��'<'+'='С�ڵ���
   char    *Contect;//��������
   
   } _DBListStr;
  
   

//  ����0 ����ȡ������, 
//      -1 KEYUP 
//      -2 KEYDOWN 
typedef  struct
{   
	 
   	uint32  top;		// λ��(���Ͻǵ�x����)
    uint32  left;		// λ��(���Ͻǵ�y����)
    uint32  width;		// ���
    uint32  hight;		// �߶�
    uint8   *caption;		// ������ʾ���� (�������ܴ�������) 
    uint8   *context;
    uint8   type;
    uint8   tabindex;
    uint8   datelen;
    uint8   keyUpDown;  //���¼����Ƿ���Ч 0 �ϼ�=������¼�=�Ҽ�
    uint8   IsClear;    //�Ƿ�����������ʾ
}_GuiInputBoxStru;

 
typedef  struct
{  
	WINDOWS  *win;					// ��������
   	uint32  top;			// λ��(���Ͻǵ�x����)
    uint32  left;			// λ��(���Ͻǵ�y����)
    uint32  width;		// ���
    uint32  hight;		// �߶�
    uint32  max;  	// ���ֵ
    uint32  min;  	// ��Сֵ
    uint32  value;  	// ��Сֵ
    uint8   *caption;		// ������ʾ���� (�������ܴ�������) 
    uint8   tabindex;
    void    (*Function)(void);		// ��Ӧ�ķ������
    uint32   stepPt;
    uint8	step;
} _ProGressBar;
  
 




typedef struct __opaque _dir;
struct dirent{
    long d_ino;              
    long d_off;             
    unsigned short d_reclen;  
    char d_name [256]; 
};


struct  stat{
    int		  yst_dev;       
    int           yst_ino;      
    uint32        yst_mode;      
    int           yst_nlink;    
    int           yst_uid;       
    int           yst_gid;      
    unsigned      yst_rdev;      
    int32         yst_size;     
    unsigned long yst_blksize;   
    unsigned long yst_blocks;   
    unsigned long yst_atime;    
    unsigned long yst_mtime;     
    unsigned long yst_ctime;    
};
 
typedef struct {
uint32 x;
uint32 y;
uint8 *dat;
uint32 hno ;
uint32 lno;
 uint8 diff;
} _PicStru;

struct t9PY_index
{
    char  *t9PY_T9;
    char  *PY;
    const char  *PY_mb;
};


typedef struct 
{
    char uSSID[32];
    uint8 uMode;
    uint8 uRSSI;   
}Access_Points;




typedef struct dirent _dirent; 
typedef struct stat _stat; 
//file
extern int8   _Fopen(char *filename, char *mode);
extern int8   _Fclose(int hdl);
extern uint32 _Fread(void *ptr, uint32 size, int hdl);
extern  int32 _Fwrite(void *ptr, uint32 size, int hdl);
extern uint32 _Lseek(int hdl,uint32 offset,uint8 fromwhere);
extern uint32 _Filelenth(int hdl);
extern uint8  _Feof(int hdl);
extern int   _Remove(char *filename);
extern int8   _Access(const char *filenpath, int mode);
extern int8   _MkDir(const char *path);
extern int8   _RmDir(const char *path);
extern _dir * _OpenDir(const char *dirname);
extern _dirent * _ReadDir(_dir *dirp);
extern int    _CloseDir(_dir *dirp);
extern  int   _Rename(const char *oldPath, const char *newPath) ;
extern int32  _GetFreeSpace(void);
extern  int32 _GetFileAtt(const char *,_stat *);
extern  char *  _GetFileList(char * ,char *,char *); 
extern   int32 _Tell(int8);



//dbf	
extern void   _Select(int dbfno );
extern void   _Go(int recno);
extern void   _Skip(int recno);
extern int    _Use(char *dbffile);
extern char * _ReadField(uint8 dbfoffset,char *field);
extern uint32 _Recno(void);
extern uint32 _Reccount(void);
extern uint8  _Bof(void);
extern uint8  _Eof(void);
extern void   _Replace(uint8 dbfoffset,char *field);
extern void   _App(void); 
extern void   _Del(void);
extern uint8  _Deleted(void);
extern void   _Pack(void);
extern void   _Zap(void);
extern void   _GetFieldStr(uint8 fieldname, struct _Field *dbfstr);
extern char * _ReadFieldEx(uint8 dbfoffset,char *field);
extern int    _Locate(uint8 FieldName,char condition,char Contect[],int32 RecStart,int32 RecEnd,int fn); //Fn=0 ��ȷ,1 ģ��,2,ǰģ��,3��ģ��
extern int    _LocateEx(uint8 FieldName,char condition,char Contect[],int32 RecStart,int32 RecEnd,int fn); //Fn=0 ��ȷ,1 ģ��,2,ǰģ��,3��ģ��



//time
extern void    _GetTime(char *time,char div);
extern void    _GetDate(char *date,char div);
extern void    _SetTime(char *time);
extern void    _SetDate(char *date);
extern uint8   _GetWeek(void);
extern uint8   _GetDay(void);
extern uint8   _GetMonth(void);
extern uint32  _GetYear(void);
extern void    _GetDateTime(char *time,char time1,char time2); 
extern void    _SetDateTime(char *datetime);
extern uint8   _GetSec(void);             
extern uint8   _GetMin(void);   
extern uint8   _GetHour(void);
extern  volatile uint32 _GetTickCount(void); 
extern void  _Sleep(uint32);
extern uint32 _GetStartTime(void);
 
 
//disp 
 
extern void _ClearScreen(void);
extern void _Printfxy(int x,int y, char *str,int);
extern void _LCMXorDataXY(uint8 x, uint8 y,uint8 x1,uint8 y1);
extern void _GUIHLine(uint8 x0, uint8 y0, uint8 x1, uint8 color);
extern void _GUIVLine(uint8 x0, uint8 y0, uint8 y1, uint8 color);
extern void _GUILine(uint32 x0, uint32 y0, uint32 x1, uint32 y1, uint8 color);
extern void _GUIRectangle(uint32 x0 , uint32 y0, uint32 x1, uint32 y1 , uint8 color);
extern void _GUIRectangleFill(uint32 x, uint32 y, uint32 x1, uint32 y1, uint8 color);
extern void _GUIPoint(uint8 x, uint8 y, uint8 color);
 
extern void _SaveScreenToBuff(uint8 *buff);
extern void _RestoreBuffToScreen(uint8 *buff);
extern void _Printfxy12(int x,int y, char *str,int); //��ʾ12����0��9 
extern void _GUILoadPic(_PicStru *); //��ʾͼƬ
extern char _T9PY_Get_Match_PY_MB(char *p_PadInput,const struct t9PY_index   ** List_match_PY_Mb);  
extern void _toxy(unsigned char X, unsigned char Y);
extern void _HideCur(void);
extern void _ShowCur(void);
extern void _GetCursor(uint8 *,uint8 *);



#define MSG_OK 0     
#define MSG_OKCANCEL 1		
#define IDOK 1
#define IDCANCEL 2

#define DIDOK 0
#define DCANCEL 2

extern uint8  _MessageBox(_GuiStrStru *stru);
extern uint8  _MessageBoxEx(char *,char *,int8);
extern uint8  _Menu(_GuiMenuStru *);
extern uint8  _MenuEx(_GuiMenuStru *);
extern uint8  _List(_GuiLisStru *);
extern uint32 _ListEx(_GuiLisStruEx *);//�������_LIST���������Ĭ��ѡ��
 
extern uint32  _DBList(_DBListStr *,char * ,int );
extern uint8  _InputBox(_GuiInputBoxStru *); 
extern uint8  _GetStr(_GuiInputBoxStru *); 
extern uint8  _CreateProgressBar(_ProGressBar *);
extern uint8  _IncrementProgressBar(_ProGressBar *); 


extern void   _SetInputMode(uint32); //�������뷽ʽ  Ĭ������Ϊ��ֵ 1��ֵ��2СдӢ�ģ�3��дӢ��
extern void   _DisInputMode(uint32); //���뷨�Ƿ������л�

extern void   _SetFrontSize(uint8 size);//���õ��� 12 16;2012 5 11
extern uint8  _GetFronSize(void);//��ȡ��ǰϵͳ����

 
 //orterh       
extern uint32 _ReadKey(void);  
extern uint32 _GetKeyExt(void);
//������
extern void   _OpenRedLight(void);
extern void   _CloseRedLight(void);
extern void   _OpenGreenLight(void);
extern void   _CloseGreenLight(void);
extern uint32 _GetShutDonwTime(void);
extern void   _SetShutDonwTime(uint32 time);
extern void   _SetBeepOnOff(uint8 onoff);
extern uint32 _GetMbt(void);
extern uint32 _GetSbt(void);
extern void   _Beep(void);//������
extern void   _OpenLcdBackLight(void);//�������
extern void   _CloseLcdBackLight(void);//�ر����
extern void   _OpenKeyBackLight(void);//�����̵�
extern void   _CloseKeyBackLight(void);//�ؼ��̵�
extern void   _OpenFlashLight(void);//���ֵ�Ͳ
extern void   _CloseFlashLight(void);//�ر��ֵ�Ͳ 
extern void   _EnableArmIrqInt(void);//��ʱ���ж�
extern void   _DisableArmIrqInt(void);//�ر�ʱ���ж� 
//����ϵͳ��
 
 
 
 
 
   
  
extern  uint32  _SetBacklightSys(void);//ϵͳ���öԱȶ� 380,�汾�� 1.00.08�Ժ����
extern  void  _SetLCDContrastSys (void);//ϵͳ����Һ������
extern void   _ShutDownPower(void); //�ػ�����
extern void   _CalendarSys(void); //ϵͳ������
extern void   _BatteryDisplaySys(void); //ϵͳ�����ʾ
extern void   _CalculatorSys(void);//ϵͳ������
extern void   _FormatSys(void);//ϵͳ��ʽ��
extern uint32   _SoundSwitchSys(void);//ϵͳ��������  380,�汾�� 1.00.08�Ժ����
extern void   _FileTransSys(void);//�ļ�ͨ�� 
extern uint32   _AutoShutDonwMenuSys(void);//�Զ��ػ��˵�  380,�汾�� 1.00.08�Ժ����
extern uint32   _TimeSettingSys(void);//ϵͳʱ�� 380,�汾�� 1.00.08�Ժ����
 
extern void   _DispTimeSys(void);//���˵��е�ʱ����ʾ
extern void   _GetMachineSn(char *);//��ȡ������� 
extern void   _InfoSys(void);//����ϵͳͨѶ���ò˵� 380,�汾�� 1.00.08�Ժ����
extern void   _Reset(void); //��λ����
extern void   _SetFileTran(char,char);//����Ĭ���ļ�ͨѶ��ʽ
extern void   _SoundOn(void);//������ 
extern void   _SoundOff(void); //������ 
extern void   _RunHex(char *filename);//���г���
extern void   _GetTransCommPort(uint32 *,uint32 *);//��ȡϵͳ�ļ�ͨѶ�˿ں��ٶ�
extern void   _SetShutDownTimeEx(uint32);//���ö�ʱ�ػ�������λ����Ч
extern void   _SetKey(uint8 *);//�Զ��尴��
extern void   _IsPowerKey(uint8);//���ж��ο����󣬹ػ������Ƿ���Ч 0 ʧЧ����0��Ч 
extern void   _SetPowOffPro(uint32  (* pro)(void));//�ػ�ǰ����
    
//������
extern char *  _DoubleToStr(   char *s1 ,  double s2 ,int n );
extern void   _GetSysVer(char * ver,char *UpdateTime);//��ȡϵͳ�汾��������



//����
extern void   _ComSet(uint8 *Setting,uint8 comport);
extern void   _ComSetTran(uint8 Tran);
extern void   _SendComStr(uint8 *SendStr,uint32 SendSize);
extern uint32 _GetComStr(uint8* Getstr,uint32 GetSize,uint32 GetMs);
extern void   _CloseCom(void);
extern void _ComChannel(uint8 val);

//��׼

extern void    _strcat(char *s1 ,  const char *s2);
extern void    _strncat(char *s1 ,  const char *s2, size_t n);
extern int     _memcmp(const char *s1 ,  const char *s2, size_t n);
extern int     _strcmp(const char *s1 ,  const char *s2 );
extern int     _strncmp(const char *s1 ,  const char *s2, size_t n);
extern size_t  _strxfrm(char *s1 ,  const char *s2, size_t n);
extern char *  _strchr(const char *s1 ,   char  s2);
extern size_t  _strcspn(const char *s1 ,  const char *s2 );
extern char *  _strpbrk(const char *s1 ,  const char *s2 );
extern char *  _strrchr(const char *s1 , int n );
extern char *  _strstr(const char *s1 ,  const char *s2 );
extern void *   memset(void *s1 , int c,size_t n );
extern size_t  _strlen(const char *s1  );
extern char *  _strcpy(  char *s1 ,  const char *s2 );
extern char *  _strncpy(   char *s1 ,  const char *s2 ,size_t n );
extern void    _free(void *ptr);
extern void *  _malloc(size_t len );
extern void *  _realloc(void *mem_address, unsigned int newsize);
extern unsigned long _strtoul(const char *,char **,int );
extern char *  _strtok (char *s , const char *s1 );

	
extern char *  _ltrim(char *dest);
extern char *  _rtrim(char *dest); 
extern char *  _trim(char *dest); 
extern void _leftspace(char * ,uint16 ,char );//��߲��ַ�,Դ�ַ�,���ݳ���,Ҫ���ַ�


extern size_t strnlen(const char * s, size_t count) ; //20120616
extern uint32    _sprintfEx(char *buff,const char *fmt, ...) ; //20110902
extern uint32    _sprintfxy(int x,int y,int color,const char *fmt, ... );//20110902 ֻ���ŵ�400���ֽ�

extern double  _atof(char *Sour);
extern void *  _memcpy(void *s1, const void *s2, size_t n);
extern int32  _abs(int32 );
//RFIC
 
#define RFID_TIMEOUT    0xFF
#define RFID_ERROR      0xEE
#define RFID_OK      	0x00
#define RFID_ALL_CARD	0x52    
    
extern void   _RfidInit(void);
extern uint16 _RfidFind(uint8 active);
extern uint16 _RfidReadCardId(uint8 cardid[]);
extern uint16 _RfidSelectCard(uint8 cardid[]);


extern uint16 _RfidCheckPwd(uint8 pwd[],uint8 cardid[],uint8 address,uint8 type);
extern uint16 _RfidWriteData(uint8 data[],uint8 address);
extern uint16 _RfidReadData(uint8 data[],uint8 address);
extern uint16 _RfidCharge(uint32 value,uint8 address,uint8 type);
extern uint16 _RfidBackup(uint8 Saddress,uint8 Taddress);
extern uint16 _RfidDormancy(void);
extern uint16 _RfidReset(void);
extern uint16 _RfidOpenAntenna(void);
extern uint16 _RfidCloseAntenna(void);
extern void   _RfidClose(void);
extern int16  _RfidInitWallet(uint32 value,uint8 address);
extern uint32 _RfidReadWallet(uint8 address);
extern int8   _Rfid125Read(uint8 *id,int32 len,int32 ms);
extern int    _Rfid125Start(void);
extern void   _Rfid125End(void);


extern uint8 _ScanBarcodeKey(uint8 *barcode,uint8 evkey); //ֻ���ڴ��������
extern uint8 _ScanBarcode(uint8 *barcode);  //ֻ���ڴ��������
extern int   _ScanStart(void);//A380�������� 20121123
extern void  _ScanEnd(void);//A380�������� 20121123
extern void _ShockOpen(void);
extern void _ShockClose(void);
extern void	_Shock(int ms);
extern uint8	_ScanBarcodeEx(uint8 *barcode,uint8 *type);//��ȡ���룬����ȡ����,�������ǳ���
		
 
extern uint8 _BtOpen(void); //A380��������
extern void _BtClose(void);  //A380��������
extern void _BtSetModel(uint8 atmodel); //A380��������
extern uint8 _BtGetModel (void); //A380��������
extern int8 _BtScan(uint8 num,char *cClass,char *cIac); //A380��������
extern int8 _BtName (char *name,uint8 op); //A380��������
extern int8 _BtRName(char*name ,char *addr); //A380��������
extern int8 _BtAddr(char *addr); //A380��������
extern int8 _BtPair(char *addr,uint16 ms); //A380��������
extern int8 _BtLink(char *addr); //A380��������
extern int8 _BtPswd(char *pswd,uint8 op); //A380��������
extern int8 _BtDefault(void); //A380��������
extern int16 _BtSendData(uint8 * Buff,uint32 Len); //A380��������
extern uint32 _BtGetData(uint8 *RecBuff,uint32 len,uint16 ms); //A380��������
extern int8 _BtMrad(char *addr); //A380��������
extern int8 _BtFsad(char *addr); //A380��������

extern void _BtGetList(void); //A380��������

extern int8 _WifiAbrd(void); //A380��������
extern int8 _WifiUpgrade(void); //A380��������
extern int8 _WifiLoadDone(void); //A380��������
extern int8 _WifiBand(uint8 band_val); //A380��������
extern int8 _WifiInit(void); //A380��������
extern int8 _WifiPassscan(uint16 bit_map); //A380��������
extern int8 _WifiSetNetworkType(char *Type); //A380��������
extern int8 _WifiPreSharedKey(char *psk); //A380��������
extern int8 _WifiScan(uint8 chan_num,char *ssid,Access_Points *ap,uint8 ap_num); //A380��������
extern int8 _WifiAuthmode(uint8 value); //A380��������
extern int8 _WifiSetIp(uint8 dhcp,char *ip,char *subnet,char *gateway); //A380��������
extern int8 _WifiCreateSockets(int8 type ,char *ipaddr,char *portno,char *lport); //A380��������
extern int8 _WifiCloseSocket(int8 socketno); //A380��������
extern int16 _WifiSendData(int8 sockno,uint16 slength,char *sdata,char *dip,char *dport); //A380��������
extern int8 _WifiClose(void); //A380��������
extern int8 _WifiOpen(void); //A380��������
extern int8 _WifiJoin(char *ssid_name,uint8 TxRate,uint8 TxPower);//A380��������
extern int16 _WifiReceiveData(int8 *sockno,char *sdata,uint16 ms);//A380��������

//GPRS
extern int16 _GprsGetSignal(char *cResult);
extern int16 _GprsGetRegistration(char *cResult);
extern int16 _GprsGetCurrentSelection(char *cResult);
extern int16 _GprsGetAllSelection(char *cResult);
extern int16 _GprsGetSimPinStatus(char *cResult);
extern int16 _GprsSetSimPin(char *cPin,char *cNewPin);
extern int16 _GprsSetService(uint8 uOperate);
extern int16 _GprsGetService(char *cResult);
extern int16 _GprsSetMultiIp(uint8 uOperate);
extern int16 _GprsGetMultiIp(void);
extern int16 _GprsStartTask(char *cApn,char *cName,char *cPassword);
extern int16 _GprsActivatePDP(void);
extern int16 _GprsGetLocalIp(char *ip);
extern int16 _GprsSetConnection(char*cMode,char *cIp, char * cPort,uint16 ms);
extern int16 _GprsGetIpStatus(int8 iMultiIp,char *cStatus);
extern int16 _GprsSendData(uint8 *cData,uint16 iLength);
extern int16 _GprsIpClose(void);
extern int16 _GprsDeactivatePDP(void);
extern int16 _GprsSetTcpipMode(uint8 uMode);
extern int16 _GprsGetTcpipMode(void);
extern int16 _GprsGetAtStatus(void);
extern int16 _GprsSetDefault(void);
extern int8  _GprsOpen(uint32 iBaud);
extern int8  _GprsClose(void);
extern int16 _GprsSetSingleConnection(char *cApn,char*cMode,char *cIp, char *iPort);


/*
���� ����A380 PORT0 PORT1 ��IO J2(USB��IO) ���ֵ
pin �ܽ�
val  1Ϊ�����0Ϊ����
��ȷ����Ϊ1
��������Ϊ0 */
extern int  _SetIo(int pin,int val);
/****
_GetIo 
���� ��ȡA380 PORT0 PORT1 ��IO J2(USB��IO) ��ֵ
��ȷ����Ϊ������ֵ
��������Ϊ-1 
*****/
extern int  _GetIo(int pin);

/****
_SetIoDir 
���� ����A380 PORT0 PORT1  J2(USB��IO) ��IO����, 
����
PIN  �ܽ�
DIR  1 Ϊ�����0Ϊ����

��ȷ����Ϊ1
��������Ϊ0 
*****/
extern int _SetIoDir(int pin,int dir);

#endif
