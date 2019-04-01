#ifndef _hjlib_h_
#define _hjlib_h_

#ifdef __cplusplus 
extern "C" 
{

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
#define KEY_KJ          '*'   //  *��   
#define KEY_UP           'U'   //���ϵ��� 
#define KEY_DOWN         'D'   //���µ���
#define KEY_LEFT         'L'   //���򵼺�
#define KEY_RIGHT        'R'   //���򵼺�
#define KEY_CANCEL       'C'   //ȡ����
#define KEY_DELETE       'T'   //ɾ����
#define KEY_ENTER        'E'   //ȷ�ϼ�
#define KEY_LIGHT        'G'   //�����
#define KEY_FUNC         'F'   //��ݼ�


#define S_IFMT		0170000
#define S_IFDIR		0040000 
#define S_IFREG		0100000


typedef  struct
{  uint32  left;			// ����λ��(���Ͻǵ�x����)
   uint32  top;			// ����λ��(���Ͻǵ�y����)
    
   uint32  width;		// ���ڿ���
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
   uint8    no;						// ���˵�����
   char     *str[MMENU_NO];			// ���˵��ַ���
   char     *key[MMENU_NO];			// ���˵��ַ���
   void    (*Function[SMENU_NO])(void);		// �Ӳ˵���Ӧ�ķ������
   void   (*FunctionEx)(void);	    //�ǰ����ص����� 
   } _GuiMenuStru;
   

typedef  struct
{  WINDOWS  *win;					 
   uint32   x;                      //��ʼX����
   uint32   y;                      //��ʼY����
   uint8    MaxNum;                 //һ��Ļ���ٸ�
   uint8    no;						//�ܹ��ж��ٸ�
   char    *str[255] ;			        // ���˵��ַ���
   } _GuiLisStru;

 

typedef  struct
{   
	 
   	uint32  top;		// λ��(���Ͻǵ�x����)
    uint32  left;		// λ��(���Ͻǵ�y����)
    uint32  width;		// ����
    uint32  hight;		// �߶�
    uint8   *caption;		// ������ʾ���� (�������ܴ�������) 
    uint8   *context;
    uint8   type;
    uint8   tabindex;
    uint8   datelen;
    uint8   keyUpDown;  //���¼����Ƿ���Ч 0 �ϼ�=������¼�=�Ҽ�
    uint8   IsClear;    //�Ƿ�����������ʾ
}_GuiInputBoxStru;



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
 

typedef struct dirent _dirent; 
typedef struct stat _stat; 
//file
extern int8   _Fopen(char *filename, char *mode);
extern void   _Fclose(int hdl);
extern uint32 _Fread(void *ptr, uint32 size, int hdl);
extern uint32 _Fwrite(void *ptr, uint32 size, int hdl);
extern uint32 _Lseek(int hdl,uint32 offset,uint8 fromwhere);
extern uint32 _Filelenth(int hdl);
extern uint8  _Feof(int hdl);
extern void   _Remove(char *filename);
extern int8   _Access(const char *filenpath, int mode);
extern int8   _MkDir(const char *path);
extern int8   _RmDir(const char *path);
extern _dir * _OpenDir(const char *dirname);
extern _dirent * _ReadDir(_dir *dirp);
extern int    _CloseDir(_dir *dirp);
extern  int   _Rename(const char *oldPath, const char *newPath) ;
extern int32  _GetFreeSpace(void);
extern  int32 _GetFileAtt(const char *,_stat *);
extern  char *  _GetFileList(char * ,char *,char *); //



//dbf	
extern void   _Select(int dbfno );
extern void   _Go(int recno);
extern void   _Skip(int recno);
extern uint8  _Use(char *dbffile);
extern void   _ReadField(uint8 dbfoffset,char *field);
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
extern void _SaveScreen(void);
extern void _RestoreScreen(void);
   
    
extern uint8  _MessageBox(_GuiStrStru *stru);
extern uint8  _Menu(_GuiMenuStru *);
extern uint8  _MenuEx(_GuiMenuStru *);
extern uint8  _List(_GuiLisStru *);
extern uint8  _InputBox(_GuiInputBoxStru *);
extern uint8  _GetStr(_GuiInputBoxStru *);


 
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
extern void   _SetBacklightSys(void);//ϵͳ���öԱȶ�
extern void   _SetLCDContrastSys (void);//ϵͳ����Һ������
extern void   _ShutDownPower(void); //�ػ�����
extern void   _CalendarSys(void); //ϵͳ������
extern void   _BatteryDisplaySys(void); //ϵͳ�����ʾ
extern void   _CalculatorSys(void);//ϵͳ������
extern void   _FormatSys(void);//ϵͳ��ʽ��
extern void   _SoundSwitchSys(void);//ϵͳ�������� 
extern void   _FileTransSys(void);//�ļ�ͨ�� 
extern void   _AutoShutDonwMenuSys(void);//�Զ��ػ��˵�
extern void   _TimeSettingSys(void);//ϵͳʱ��
extern void   _InfoSys(void);//ϵͳ��Ϣ��ʾ
extern void   _DispTimeSys(void);//���˵��е�ʱ����ʾ
//������
extern char *  _DoubleToStr(   char *s1 ,  double s2 ,int n );

extern void   _GetSysVer(char * ver,char *UpdateTime);//��ȡϵͳ�汾��������


//����
extern void   _ComSet(uint8 *Setting,uint8 comport);
extern void   _ComSetTran(uint8 Tran);
extern void   _SendComStr(uint8 *SendStr,uint32 SendSize);
extern uint32 _GetComStr(uint8* Getstr,uint32 GetSize,uint32 GetMs);
extern void   _CloseCom(void);


//��׼

extern void    _strcat(char *s1 ,  const char *s2);
extern void    _strncat(char *s1 ,  const char *s2, size_t n);
extern int     _memcmp(const char *s1 ,  const char *s2, size_t n);
extern int     _strcmp(const char *s1 ,  const char *s2 );
extern int     _strncmp(const char *s1 ,  const char *s2, size_t n);
extern size_t  _strxfrm(char *s1 ,  const char *s2, size_t n);
extern char *  _strchr(char *s1 ,  const char *s2, size_t n);
extern size_t  _strcspn(const char *s1 ,  const char *s2 );
extern char *  _strpbrk(const char *s1 ,  const char *s2 );
extern char *  _strrchr(const char *s1 , int n );
extern char *  _strstr(const char *s1 ,  const char *s2 );
extern void *  memset(void *s1 , int c,size_t n );
extern size_t  _strlen(const char *s1  );
extern char *  _strcpy(  char *s1 ,  const char *s2 );
extern char *  _strncpy(   char *s1 ,  const char *s2 ,size_t n );
extern void   _free(void *ptr);
extern  void *_malloc(size_t len );
  

extern double  _atof(char *Sour);
extern void *  _memcpy(void *s1, const void *s2, size_t n);
}
#endif
#endif