#ifndef _hjlib_h_
#define _hjlib_h_

#ifdef __cplusplus 
extern "C" 
{

typedef unsigned char  uint8;                
typedef signed   char  int8;                     
typedef unsigned short uint16;                  
typedef signed   short int16;                   /* defined for signed 16-bits integer variable 		有符号16位整型变量 */
typedef unsigned int   uint32;                  /* defined for unsigned 32-bits integer variable 	无符号32位整型变量 */
typedef signed   int   int32;                   /* defined for signed 32-bits integer variable 		有符号32位整型变量 */
typedef float          fp32;                    /* single precision floating point variable (32bits) 单精度浮点数（32位长度） */
typedef double         fp64;                    /* double precision floating point variable (64bits) 双精度浮点数（64位长度） */
typedef unsigned char  UINT8;                   /* defined for unsigned 8-bits integer variable 	无符号8位整型变量  */
typedef signed   char  INT8;                    /* defined for signed 8-bits integer variable		有符号8位整型变量  */
typedef unsigned short UINT16;                  /* defined for unsigned 16-bits integer variable 	无符号16位整型变量 */
typedef signed   short INT16;                   /* defined for signed 16-bits integer variable 		有符号16位整型变量 */
typedef unsigned int   UINT32;                  /* defined for unsigned 32-bits integer variable 	无符号32位整型变量 */
typedef signed   int   INT32;                   /* defined for signed 32-bits integer variable 		有符号32位整型变量 */
typedef unsigned char BOOLEAN;
typedef unsigned int size_t;
//按键定义
#define KEY_0            '0'   //  0键
#define KEY_1            '1'   //  1键
#define KEY_2            '2'   //  2键
#define KEY_3            '3'   //  3键
#define KEY_4            '4'   //  4键
#define KEY_5            '5'   //  5键
#define KEY_6            '6'   //  6键
#define KEY_7            '7'   //  7键
#define KEY_8            '8'   //  8键
#define KEY_9            '9'   //  9键
#define KEY_DOT          '.'   //  #键
#define KEY_KJ          '*'   //  *键   
#define KEY_UP           'U'   //向上导航 
#define KEY_DOWN         'D'   //向下导航
#define KEY_LEFT         'L'   //左向导航
#define KEY_RIGHT        'R'   //右向导航
#define KEY_CANCEL       'C'   //取消键
#define KEY_DELETE       'T'   //删除键
#define KEY_ENTER        'E'   //确认键
#define KEY_LIGHT        'G'   //背光灯
#define KEY_FUNC         'F'   //快捷键


#define S_IFMT		0170000
#define S_IFDIR		0040000 
#define S_IFREG		0100000


typedef  struct
{  uint32  left;			// 窗口位置(左上角的x坐标)
   uint32  top;			// 窗口位置(左上角的y坐标)
    
   uint32  width;		// 窗口宽度
   uint32  hight;		// 窗口高度
   
   uint8   *title;		// 定义标题栏指针 (标题字符为ASCII字符串，最大个数受窗口限制)
   uint8   *state;		// 定义状态栏指针 (若为空时则不显示状态栏)   
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
   uint32  left;			// 窗口位置(左上角的x坐标)
   uint32   top;			// 窗口位置(左上角的y坐标);			
   char     *title;
   uint8    no;						// 主菜单个数
   char     *str[MMENU_NO];			// 主菜单字符串
   char     *key[MMENU_NO];			// 主菜单字符串
   void    (*Function[SMENU_NO])(void);		// 子菜单对应的服务程序
   void   (*FunctionEx)(void);	    //非按键回调函数 
   } _GuiMenuStru;
   

typedef  struct
{  WINDOWS  *win;					 
   uint32   x;                      //开始X坐标
   uint32   y;                      //开始Y坐标
   uint8    MaxNum;                 //一屏幕多少个
   uint8    no;						//总共有多少个
   char    *str[255] ;			        // 主菜单字符串
   } _GuiLisStru;

 

typedef  struct
{   
	 
   	uint32  top;		// 位置(左上角的x坐标)
    uint32  left;		// 位置(左上角的y坐标)
    uint32  width;		// 宽度
    uint32  hight;		// 高度
    uint8   *caption;		// 定义显示内容 (最大个数受窗口限制) 
    uint8   *context;
    uint8   type;
    uint8   tabindex;
    uint8   datelen;
    uint8   keyUpDown;  //上下键盘是否有效 0 上键=左键，下键=右键
    uint8   IsClear;    //是否输入后清除显示
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
//控制类
extern void   _OpenRedLight(void);
extern void   _CloseRedLight(void);
extern void   _OpenGreenLight(void);
extern void   _CloseGreenLight(void);
extern uint32 _GetShutDonwTime(void);
extern void   _SetShutDonwTime(uint32 time);
extern void   _SetBeepOnOff(uint8 onoff);
extern uint32 _GetMbt(void);
extern uint32 _GetSbt(void);
extern void   _Beep(void);//发声音
extern void   _OpenLcdBackLight(void);//开背光灯
extern void   _CloseLcdBackLight(void);//关背光灯
extern void   _OpenKeyBackLight(void);//开键盘灯
extern void   _CloseKeyBackLight(void);//关键盘灯
extern void   _OpenFlashLight(void);//打开手电筒
extern void   _CloseFlashLight(void);//关闭手电筒 
extern void   _EnableArmIrqInt(void);//打开时钟中断
extern void   _DisableArmIrqInt(void);//关闭时钟中断 
//调用系统类
extern void   _SetBacklightSys(void);//系统设置对比读
extern void   _SetLCDContrastSys (void);//系统设置液晶亮度
extern void   _ShutDownPower(void); //关机函数
extern void   _CalendarSys(void); //系统万年历
extern void   _BatteryDisplaySys(void); //系统电池显示
extern void   _CalculatorSys(void);//系统计算器
extern void   _FormatSys(void);//系统格式化
extern void   _SoundSwitchSys(void);//系统声音管理 
extern void   _FileTransSys(void);//文件通信 
extern void   _AutoShutDonwMenuSys(void);//自动关机菜单
extern void   _TimeSettingSys(void);//系统时间
extern void   _InfoSys(void);//系统信息显示
extern void   _DispTimeSys(void);//主菜单中的时间显示
//其他类
extern char *  _DoubleToStr(   char *s1 ,  double s2 ,int n );

extern void   _GetSysVer(char * ver,char *UpdateTime);//获取系统版本更新日期


//串口
extern void   _ComSet(uint8 *Setting,uint8 comport);
extern void   _ComSetTran(uint8 Tran);
extern void   _SendComStr(uint8 *SendStr,uint32 SendSize);
extern uint32 _GetComStr(uint8* Getstr,uint32 GetSize,uint32 GetMs);
extern void   _CloseCom(void);


//标准

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
