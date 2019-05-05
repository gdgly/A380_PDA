#include <HJLIB.H>
#include "string.h"
#include "dbf.h"
#include "stdio.h"
//A350 �õ�
//uint8 Screenbuff[5000];
//A3480


char Screenbuff[160 * (160 / 3 + 1) * 2];
/*
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
extern _DIR * _OpenDir(const char *dirname);
extern _dirent * _ReadDir(_DIR *dirp);
extern int    _CloseDir(_DIR *dirp);
extern  int   _Rename(const char *oldPath, const char *newPath) ;
extern int32  _GetFreeSpace(void);
extern  int32 _GetFileAtt(const char *,_stat *);
extern  char *  _GetFileList(char * ,char *,char *);

extern uint8  _MessageBox(_GuiStrStru *stru);
*/

void FileTestError(char *Content)
{
	_GuiStrStru str;
	str.lpCaption = "��Ҫ��ʾ!";
	str.str = Content;
	str.itype = 0;
	_MessageBox(&str);
}

void ListDir(const char *dname)
{
	_dir *d;
	_dirent *de;
	_stat s;
	char str[100];

	d = _OpenDir(dname);

	if (!d)
	{
		return;
	}
	else
	{
		while ((de = _ReadDir(d)) != NULL)
		{
			_ClearScreen();

			sprintf(str, "%s%s", dname, de->d_name);
			_Printfxy(0, 0, "�ļ�����", 0);

			_GetFileAtt(str, &s);
			_Printfxy(0, 16, de->d_name, 0);

			sprintf(str, "�ļ�����:%d ", s.yst_size);
			_Printfxy(0, 32, str, 0);

			switch (s.yst_mode & S_IFMT)
			{
			case S_IFREG:
				_Printfxy(0, 48, "���ԣ��ļ�", 0);
				break;
			case S_IFDIR:
				_Printfxy(0, 48, "���ԣ�Ŀ¼", 0);
				break;

			default:
				_Printfxy(0, 48, "unknown", 0);
				break;
			}

			_ReadKey();
		}

		_CloseDir(dname);
	}
}

void File(void)
{
	int8 hdl, hdl1;
	uint32 i = 0;
	char tmp[10];
	char *p;
	_ClearScreen();
	_Printfxy(0, 0, "FIELSYS TEST", 0);
	p = _GetFileList("��|\n  |\n��|\n  |\n��|\n  |\n��|\n  |\n", "", "");
	_ClearScreen();
	_Printfxy(0, 2 * 16, p, 0);
	_ReadKey();

	_ClearScreen();
	_Printfxy(0, 0, "FIELSYS TEST", 0);

	i = _GetFreeSpace();
	sprintf(tmp, "%d", i);
	_Printfxy(0, 1 * 16, "space:", 0);
	_Printfxy(5 * 8, 1 * 16, tmp, 0);

	_Printfxy(0, 2 * 16, "mkdir,fopen/fwrite", 0);

	if (_MkDir("test") == -1)
	{
		FileTestError("build test folder error!");
		return;
	}

	if (_MkDir("test/sbutest") == -1)
	{

		FileTestError("build sbutest folder error!");
		return;
	}
	//�ļ���
	hdl = _Fopen("test/1.tmp", "W");
	if (hdl == -1)
	{

		FileTestError("1.tmp file create error! ");
		return;
	}
	hdl1 = _Fopen("test/2.tmp", "W");
	if (hdl == -1)
	{
		FileTestError("2.tmp file create error!");
		return;
	}
	//�ļ�д��
	for (i = 0; i < 10; i++)
	{
		_Fwrite("123", 3, hdl);
		_Fwrite("456", 3, hdl1);
	}
	//�ļ��ر�
	_Fclose(hdl);
	_Fclose(hdl1);

	_Printfxy(0, 3 * 16, "file read/close", 0);

	hdl = _Fopen("test/1.tmp", "R");
	//�ļ���ȡ
	for (i = 0; i < 10; i++)
	{
		if (_Fread(tmp, 3, hdl) != 3)
			break;
		tmp[3] = 0;
		if (_strcmp(tmp, "123") != 0)
			break;
	}
	_Fclose(hdl);

	if (i != 10)
	{
		FileTestError("file read erro!");
		return;
	}
	else

		//�ļ�ɾ��/������
		_Printfxy(0, 4 * 16, "remove/rename/access", 0);

	_Remove("test/1.tmp");

	if (_Rename("test/2.tmp", "test/3.tmp") == -1)
	{
		FileTestError("2.tmp rename erro");
		return;
	}

	//����ļ��Ƿ����
	if (_Access("test/3.tmp", 0) == -1)
	{
		FileTestError("3.tmp is not eisit");
		return;
	}
	hdl = _Fopen("test/3.tmp", "RW");
	if (hdl == -1)
	{
		FileTestError("3.tmp open error");
		return;
	}

	//�ļ���С
	_Printfxy(0, 5 * 16, "file 2 size:", 0);

	i = _Filelenth(hdl);
	sprintf(tmp, "%d", i);

	_Printfxy(90, 5 * 16, tmp, 0);

	//�ļ��ƶ�
	_Lseek(hdl, -2, 2);
	i = _Fread(tmp, 4, hdl);
	tmp[i] = 0;
	_Printfxy(0, 6 * 16, "after seek:", 0);
	_Printfxy(_strlen("after seek:") * 8, 6 * 16, tmp, 0);

	//����ļ��Ƿ񵽽�β
	if (_Feof(hdl))
		_Printfxy(0, 7 * 16, "ָ���ѵ���β��:", 0);
	else
		_Printfxy(0, 7 * 16, "ָ��δ����β��:", 0);

	_Fclose(hdl);

	_ReadKey();

	ListDir("test/");

	_GetFileList("��|\n  |\n��|\n  |\n��|\n  |\n��|\n  |\n", "", "");

	_ReadKey();

	_Remove("test/3.tmp");
	if (_RmDir("test/sbutest") == -1)
	{
		FileTestError("test/sbutestĿ¼ɾ��ʧ��");
		return;
	}
	if (_RmDir("test") == -1)
	{
		FileTestError("test Ŀ¼ɾ��ʧ��");
		return;
	}

	FileTestError("file test ok !");
}

/*****************************************
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
extern void   _Pack(void);
extern void   _Zap(void);
*****************************************/
int DispOneFiled(int row, int col, char *FieldTile, int8 fieldNum) //��ʾһ������¼,��,��,��ͷ,�ֶ�
{
	int x = col * 8, y = row * 16;
	int tmp = 0;
	char Field[256];

	tmp = _strlen(FieldTile) * 8;
	if (tmp > 0)
		_Printfxy(x, y, FieldTile, 0);

	_ReadField(fieldNum, Field);

	_Printfxy(x + tmp, y, Field, 0);

	return (_strlen(FieldTile) + _strlen(Field) + 2);
}

void dbf(void)
{
	char tmp[50];
	int rec = 0, recount = 0, cnt = 0, start = 0;
	int len = 0;

	_ClearScreen();
	_Select(1);
	_Use("test.dbf");
	_Go(2);
	rec = _Recno();
	recount = _Reccount();
	sprintf(tmp, "��¼:%d/%d", rec, recount);
	_Printfxy(0, 0, tmp, 0);
	len = DispOneFiled(1, 0, "ID:", TEST_ID);
	DispOneFiled(1, len, "����:", TEST_Name);
	_Skip(1);
	len = DispOneFiled(2, 0, "ID:", TEST_ID);
	DispOneFiled(2, len, "����:", TEST_Name);


	// _Skip(-2);
	// _Replace(TEST_Name, "����");
	// len = DispOneFiled(3, 0, "ID:", TEST_ID);
	// DispOneFiled(3, len, "����:", TEST_Name);
	// _Del();
	// _Pack();
	// len = DispOneFiled(4, 0, "ID:", TEST_ID);
	// DispOneFiled(4, len, "����:", TEST_Name);
	// _Go(1);
	// if (_Bof())
	// 	_Printfxy(0, 5 * 16, "��һ����¼", 0);
	// _Go(_Reccount());
	// if (_Eof())
	// 	_Printfxy(0, 6 * 16, "���ļ�¼", 0);
	// _App();
	// _Replace(TEST_Name, "����");

	cnt = 4;
	start = 1;
	while(start < 12){
		if(_Locate((uint8)TEST_Name, '=', "����", start, 12, 0) > 0){
			len = DispOneFiled(cnt, 0, "ID:", TEST_ID);
			DispOneFiled(cnt, len, "����:", TEST_Name);
			start = _Recno();
			cnt++;
		}
		start++;
	}
	
	start = 1;
	while(start < 12){
		if(_Locate((uint8)TEST_Name, '=', "����", start, 12, 1) > 0){
			len = DispOneFiled(cnt, 0, "ID:", TEST_ID);
			DispOneFiled(cnt, len, "����:", TEST_Name);
			start = _Recno();
			cnt++;
		}
		start++;
	}
	_ReadKey();
	//_Zap();
	_Use("");
}
/*****************************************
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
extern  volatile uint32 _GetTickCount(void); // ��������: ��ȡʵʱʱ�� 1 = 1/32768 ��
extern void  _Sleep(uint32); // ��������: ���ߺ���
*****************************************/

void time(void)
{
	uint32 time;
	char tmp[30];

	uint32 stime = 0;
	volatile uint32 etime;

	_ClearScreen();

	_GetTime(tmp, ':'); //��ȡʱ��
	_Printfxy(0, 0, tmp, 0);
	_GetDate(tmp, '/'); //��ȡʱ��
	_Printfxy(0, 1 * 16, tmp, 0);
	_SetTime("13:12:11"); //��ʱ��
	_SetDate("2009-8-5"); //������

	time = _GetYear();
	sprintf(tmp, "%d", time);
	time = _GetMonth();
	if (time < 9)
		sprintf(&tmp[4], "0%d", time);
	else
		sprintf(&tmp[4], "%d", time);

	time = _GetDay();
	if (time < 9)
		sprintf(&tmp[6], "0%d", time);
	else
		sprintf(&tmp[6], "%d", time);

	time = _GetHour();
	if (time < 9)
		sprintf(&tmp[8], "0%d", time);
	else
		sprintf(&tmp[8], "%d", time);

	time = _GetMin();
	if (time < 9)
		sprintf(&tmp[10], "0%d", time);
	else
		sprintf(&tmp[10], "%d", time);

	time = _GetSec();
	if (time < 9)
		sprintf(&tmp[12], "0%d", time);
	else
		sprintf(&tmp[12], "%d", time);

	tmp[15] = _GetWeek() + 0x30; //��ȡ��
	tmp[16] = 0;
	_Printfxy(0, 2 * 16, tmp, 0);

	//   _SetDateTime("2010-01-01 12:12:12");
	_GetDateTime(tmp, '/', ':');
	tmp[10] = 0;

	_Printfxy(0, 3 * 16, tmp, 0);
	_Printfxy(0, 4 * 16, &tmp[11], 0);

	_ReadKey();

	_ClearScreen();

	time = _GetSec();
	if (time < 9)
		sprintf(tmp, "0%d", time);
	else
		sprintf(tmp, "%d", time);
	_Printfxy(0, 3 * 16, tmp, 0);

	_Sleep(3000);
	time = _GetSec();
	if (time < 9)
		sprintf(tmp, "0%d", time);
	else
		sprintf(tmp, "%d", time);
	_Printfxy(0, 4 * 16, tmp, 0);

	_ReadKey();

	_ClearScreen();
	time = _GetSec();
	if (time < 9)
		sprintf(tmp, "0%d", time);
	else
		sprintf(tmp, "%d", time);
	_Printfxy(0, 3 * 16, tmp, 0);

	/*
	   stime = _GetTickCount();
		while(1)
		{

			etime = _GetTickCount();
			if (etime<stime)
		    {
		         if(stime-etime > (1000<<5))
		         {
		         	break;
		         }

			}
		    else if (( etime- stime) > (1000<<5))
		    {
		    	break;
		    }
	    }


		 time= _GetSec();
	    if (time<9)
	       sprintf(tmp,"0%d",time);
	     else
		   sprintf(tmp,"%d", time);
	     _Printfxy(0,4*16,tmp,0);
		    _ReadKey();


	   */
}

/******************************************
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
******************************************/

void disp(void)
{
	_ClearScreen();
}
/*
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
*/

void Hardware(void)
{
	char tmp[30];
	double mb, bb;
	int i;
	_ClearScreen();
	_SetBeepOnOff(0);
	_OpenRedLight();
	_OpenGreenLight();
	_OpenLcdBackLight();
	_OpenKeyBackLight();
	_OpenFlashLight();

	_Printfxy(0, 0, "����:", 0);
	mb = _GetMbt() / 100;
	bb = _GetSbt() / 100;
	_DoubleToStr(tmp, mb, 2);
	_Printfxy(5 * 8, 0, tmp, 0);
	_Printfxy(0, 1 * 16, "����:", 0);
	_DoubleToStr(tmp, bb, 2);
	_Printfxy(5 * 8, 1 * 16, tmp, 0);

	mb = _GetShutDonwTime();
	_Printfxy(0, 2 * 16, "�ػ�ʱ��1:", 0);
	_DoubleToStr(tmp, mb, 0);
	_Printfxy(10 * 8, 2 * 16, tmp, 0);

	_SetShutDonwTime(210);

	mb = _GetShutDonwTime();
	_Printfxy(0, 3 * 16, "�ػ�ʱ��2:", 0);
	_DoubleToStr(tmp, mb, 0);
	_Printfxy(10 * 8, 3 * 16, tmp, 0);

	_SetShutDonwTime(mb);
	_Beep();
	while (1)
		if (_ReadKey() == KEY_CANCEL)
			break;
	_SetBeepOnOff(1);
	_CloseRedLight();
	_CloseGreenLight();
	_CloseLcdBackLight();
	_CloseKeyBackLight();
	_CloseFlashLight();
	_SoundOn();
	for (i = 0; i < 0xffff; i++)
		;
	for (i = 0; i < 0xffff; i++)
		;
	for (i = 0; i < 0xffff; i++)
		;
	for (i = 0; i < 0xffff; i++)
		;
	for (i = 0; i < 0xffff; i++)
		;
	for (i = 0; i < 0xffff; i++)
		;
	for (i = 0; i < 0xffff; i++)
		;
	for (i = 0; i < 0xffff; i++)
		;
	for (i = 0; i < 0xffff; i++)
		;
	_SoundOff();
}
/***********************
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
extern void   _GetMachineSn(char *);//��ȡ�������
extern void   _SetComSever(void);//����ϵͳͨѶ���ò˵�
extern void   _Reset(void); //��λ����
extern void   _SetFileTran(char,char);//����Ĭ���ļ�ͨѶ��ʽ
***************************/

void GetSys(void)
{
	char ver[20], update[20];
	char Sn[13];
	_SetBacklightSys();
	_SetLCDContrastSys();
	//  _ShutDownPower();
	_CalendarSys();
	_BatteryDisplaySys();
	_CalculatorSys();
	_FormatSys();
	_SoundSwitchSys();
	_FileTransSys();
	_AutoShutDonwMenuSys();
	_TimeSettingSys();
	_InfoSys();
	_DispTimeSys();
	_SetFileTran(1, 4);
	//   _SetComSever();

	// _Reset();
	_ReadKey();
	_ClearScreen();
	_GetSysVer(ver, update);
	_GetMachineSn(Sn);
	_Printfxy(0, 0, "  ϵ ͳ �� ��", 0);
	_Printfxy(0, 16, ver, 0);
	_Printfxy(0, 32, "  ����ʱ��", 0);
	_Printfxy(0, 48, update, 0);
	_Printfxy(0, 64, " �������", 0);
	_Printfxy(0, 80, Sn, 0);
	_ReadKey();
}

/*****************************
extern void   _ComSet(uint8 *Setting,uint8 comport);
extern void   _ComSetTran(uint8 Tran);
extern void   _SendComStr(uint8 *SendStr,uint32 SendSize);
extern uint32 _GetComStr(uint8* Getstr,uint32 GetSize,uint32 GetMs);
extern void   _CloseCom(void);

extern uint8  _MenuEx(_GuiMenuStru *);
******************************/

void COMTEST(void)
{

	uint8 tmp[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'}, tmp1[100], m, m1[10];
	uint32 t1;
	uint32 key = 0, Send = 1;
	uint8 disp[3][20] = {{"   ���ڲ���    "}, {"��ͨ�������"}, {"���ٺ������"}};
	_GuiMenuStru MainMenu;
	MainMenu.left = 0;
	MainMenu.top = 0;
	MainMenu.no = 2;
	MainMenu.title = "    ����ѡ��    ";
	MainMenu.str[0] = "���ղ���";
	MainMenu.str[1] = "���Ͳ���";
	MainMenu.key[0] = "1";
	MainMenu.key[1] = "2";
	MainMenu.FunctionEx = 0;

	while (1)
	{
		_ClearScreen();

		m = _MenuEx(&MainMenu);
		if (m == KEY_CANCEL)
			return;
		if (m == '1')
			Send = 0;
		else
			Send = 1;

		_ClearScreen();

		_CloseCom();
		_ComSetTran(1);
		_ComSet((uint8 *)"115200,N,8,1", 2);
		_Printfxy(0, 0, disp[0], 1);
		if (m == '1')
			_Printfxy(16, 4 * 16, "<<���ղ���>>", 1);
		else
			_Printfxy(16, 4 * 16, "<<���Ͳ���>>", 1);

		_Printfxy(0, 5 * 16, "<1>�� ��    ��", 0);
		_Printfxy(0, 6 * 16, "<2>�� ��ͨ����", 0);
		_Printfxy(0, 7 * 16, "<3>�� ���ٺ���", 0);
		key = 0;
		while (1)
		{
			switch (key)
			{
			case '1':
				_CloseCom();
				_ComSetTran(1);
				_ComSet((uint8 *)"115200,N,8,1", 1);
				_Printfxy(0, 0, disp[0], 1);
				break; //����
			case '2':
				_CloseCom();
				_ComSetTran(2);
				_ComSet((uint8 *)"4800,N,8,1", 2);
				_Printfxy(0, 0, disp[1], 1);
				break; //��ͨ����
			case '3':
				_CloseCom();
				_ComSetTran(3);
				_ComSet((uint8 *)"4800,N,8,1", 2);
				_Printfxy(0, 0, disp[2], 1);
				break; //���ٺ���
			case 'C':
				break;
				;
			}
			if (key == 'C')
				break;
			if (key == 'E' && Send == 1)
			{
				//�����һ�½��ջ�����
				_GetComStr(tmp1, 100, 1);
				_SendComStr(tmp, 10);
				t1 = _GetComStr(tmp1, 10, 50);
				if (t1 == 10 && _strncmp(tmp, tmp1, 10) == 0)
					_Printfxy(0, 32, "״̬:���ճɹ�!", 1);
				else
					_Printfxy(0, 32, "״̬:����ʧ��!", 1);
			}
			if (Send == 0)
			{
				if (key == 'E')
				{
					_Printfxy(0, 32, "״̬:          ", 1);
					_Printfxy(0, 48, "                ", 0);
					//�����һ�½��ջ�����
					_GetComStr(tmp1, 100, 1);
				}
				t1 = _GetComStr(tmp1, 1, 0);
				if (t1 > 0)
				{
					//��ͨ����ʱ��Ҫ�ӳ�
					t1 = _GetComStr(&tmp1[1], 9, 50);

					if (t1 == 9 && _strncmp(tmp, tmp1, 10) == 0)
					{
						_Printfxy(0, 32, "״̬:���ճɹ�!", 1);
						_Printfxy(0, 48, "           ", 0);
						_SendComStr(tmp, 10);
					}

					if (t1 != 9 || _strncmp(tmp, tmp1, 10) != 0)
					{
						_Printfxy(0, 32, "״:����ʧ��", 1);
						_Printfxy(0, 48, "           ", 0);
						_DoubleToStr(m1, t1, 0);
						_Printfxy(6 * 16, 32, m1, 0);
						tmp1[t1] = 0;

						_Printfxy(0, 48, tmp1, 0);
						//�����һ�½��ջ�����
						_GetComStr(tmp1, 100, 1);
					}
				}
			}
			key = _GetKeyExt();
		}
	}
}

/*****************************
extern uint8  _List(_GuiLisStru *);
extern uint8  _InputBox(_GuiInputBoxStru *);
extern uint8  _GetStr(_GuiInputBoxStru *);
*****************************/

void DispComponents(void)
{
	char byds[20] = {0};
	uint8 key, i;
	char filed[20][50] = {{"1.��ѧ��"}, {"2.���ǳ�"}, {"3.���»����»����»�"}, {"4.����"}, {"5.������"}, {"6.��־��"}, {"7.�ž޻�"}, {"8.����Ѹ"}, {"9.����"}, {"10.�Ź���"}};

	_GuiInputBoxStru inputbox;
	_GuiLisStru ThisList;
	WINDOWS win;

	_ClearScreen();
	inputbox.top = 5 * 16;
	inputbox.left = 5 * 8;
	inputbox.caption = "";
	inputbox.context = byds;
	inputbox.datelen = 10;
	inputbox.type = 1;			//����
	inputbox.keyUpDown = 1; //���¼�����Ч
	inputbox.IsClear = 0;
	_SetInputMode(1); //�������뷽ʽ
	_DisInputMode(1); //���뷨�Ƿ������л�
	key = _InputBox(&inputbox);
	if (key == KEY_CANCEL)
		return;
	_ClearScreen();
	inputbox.IsClear = 1;
	key = _GetStr(&inputbox);

	if (key == KEY_CANCEL)
		return;

	_ClearScreen();

	_Printfxy(0, 0, "һ�����������߰˾�ʰ", 1);

	ThisList.title = "    �б���ʾ    ";
	ThisList.no = 10;
	ThisList.MaxNum = 6;
	ThisList.x = 0;
	ThisList.y = 2 * 16;
	ThisList.with = 16 * 10;
	for (i = 0; i < ThisList.no; i++)
		ThisList.str[i] = filed[i];
	i = _List(&ThisList);

	if (i == 0)
		return;

	_Printfxy(0, 9 * 16, "ѡ����:", 1);
	_Printfxy(60, 9 * 16, filed[i - 1], 1);
	_ReadKey();
}

/*

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
extern void _Printfxy12(int x,int y, char *str,int);//��ʾ12����0��9
extern void _GUILoadPic(_PicStru *);//��ʾͼƬ
//extern void _GetT9PYMatchMB(const struct t9PY_index *,const struct t9PY_index *);
 char _T9PY_Get_Match_PY_MB(char *p_PadInput,const struct t9PY_index   ** List_match_PY_Mb);
*/

void dispbase(void)
{
	const unsigned char batt0[] = {
			0X3F, 0XFF, 0XC0, 0X20, 0X00, 0X40, 0XE0, 0X00, 0X40, 0XE0, 0X00, 0X40, 0XE0, 0X00, 0X40, 0X20,
			0X00, 0X40, 0X20, 0X00, 0X40, 0X3F, 0XFF, 0XC0, 0X00, 0X00, 0X00};
	int tablen;
	char pynum[8] = {"2"};									//������������6����
	const struct t9PY_index *cpt9PY_Mb[10]; //ƴ���ṹ
	_PicStru pic;

	_ClearScreen();
	_GUIHLine(0, 20, 127, 1);
	_GUIVLine(20, 0, 127, 1);
	_GUILine(20, 0, 127, 127, 1);
	_GUIRectangle(20, 0, 127, 127, 1);
	_GUIPoint(25, 20, 1);
	_GUIPoint(26, 21, 1);
	_GUIPoint(26, 22, 1);
	_LCMXorDataXY(40, 40, 50, 50);
	_GUIRectangleFill(42, 42, 48, 48, 0);
	_Printfxy12(0, 0, "0123456789 :-", 0);
	pic.x = 10 + 16 * 6;
	pic.y = 7 * 16 + 6;
	pic.dat = batt0;
	pic.hno = 18;
	pic.lno = 8;
	pic.diff = 0;
	_GUILoadPic(&pic);

	tablen = _T9PY_Get_Match_PY_MB(pynum, cpt9PY_Mb);
	if (tablen)
	{
		// _Printfxy(0,32  ,"����",0);
		_Printfxy(0, 32, cpt9PY_Mb[0]->PY_mb, 0);
	}

	tablen = _T9PY_Get_Match_PY_MB("4", cpt9PY_Mb);
	if (tablen)
	{
		// _Printfxy(0,32  ,"����",0);
		_Printfxy(0, 48, cpt9PY_Mb[2]->PY_mb, 0);
	}

	//  _SaveScreen();
	_SaveScreenToBuff(Screenbuff);
	_ReadKey();
	_ClearScreen();
	_Printfxy(0, 32, "������ָ̻�", 0);
	_ReadKey();

	// _RestoreScreen();

	//   _Printfxy(0,32  ,"������ָ̻�1",0);
	// _ReadKey();
	_ClearScreen();
	_RestoreBuffToScreen(Screenbuff);
	_ReadKey();

	DispComponents();
}

/*
extern void   _RfidInit(void);
extern uint16 _RfidFind(uint8 active);
extern uint16 _RfidReadCardId(uint8 cardid[]);
extern uint16 _RfidSelectCard(uint8 cardid[]);
extern uint16 RfidCheckPwd(uint8 pwd[],uint8 cardid[],uint8 address,uint8 type);
extern uint16 _RfidWriteData(uint8 data[],uint8 address);
extern uint16 _RfidReadData(uint8 data[],uint8 address);
extern uint16 _RfidCharge(uint32 value,uint8 address,uint8 type);
extern uint16 _RfidBackup(uint8 Saddress,uint8 Taddress);
extern uint16 _RfidDormancy(void);
extern uint16 _RfidReset(void);
extern uint16 _RfidOpenAntenna(void);
extern uint16 _RfidCloseAntenna(void);
*/

void Rfic125k(void)
{

	int i, RxLen;
	char RxData[20] = {0}, t1[20], ls_eid[20] = {0};

	_ComSetTran(4);
	_ComSet((uint8 *)"19200,N,8,1", 2);

	for (i = 0; i < 0xffff; i++)
		;
	for (i = 0; i < 0xffff; i++)
		;
	for (i = 0; i < 0xffff; i++)
		;
	for (i = 0; i < 0xffff; i++)
		;
	for (i = 0; i < 0xffff; i++)
		;
	for (i = 0; i < 0xffff; i++)
		;
	for (i = 0; i < 0xffff; i++)
		;
	_ComSetTran(1);
	_ComSet((uint8 *)"9600,N,8,1", 1);

	RxLen = _GetComStr(RxData, 10, 100);
	_CloseCom();
	_ComSetTran(4);
	_ComSet((uint8 *)"19200,N,8,1", 2);
	_CloseCom();

	for (i = 0; i < RxLen; i++)
	{

		sprintf(&ls_eid[i * 2], "%.2X", RxData[i]);
	}

	_MessageBoxEx("���:", ls_eid, MSG_OKCANCEL);
}
void RFIC1356(void)
{

	uint8 keyval = 0;
	uint16 Rrfid = 0;
	uint8 cardid[4] = {0};
	uint8 pwd[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
	uint8 wdata[20] = "0123456789abcdef";
	uint8 rdata[20] = {0};
	char prbuf[50] = {0}, prbuf1[50] = {0};
	_ClearScreen();
	_Printfxy(18, 0, "RFID����", 0);
	_RfidInit();
	while (1)
	{
		keyval = _ReadKey();
		//	keyval = _Get_Key_Ext();
		switch (keyval)
		{
		case KEY_1:
			_Printfxy(0, 20, "                  ", 0);
			_Printfxy(0, 36, "                  ", 0);
			_Printfxy(0, 52, "                  ", 0);
			_Printfxy(8, 20, "���ҿ�...", 0);
			Rrfid = _RfidFind(RFID_ALL_CARD);

			switch (Rrfid)
			{
			case 0x20:
				sprintf(prbuf, "û���ҵ���X");
				sprintf(prbuf1, "����:0x%X", Rrfid);
				break;
			case 0x0200:
				sprintf(prbuf, "�ҵ���:S70");
				sprintf(prbuf1, "����:0x%X", Rrfid);
				break;
			case 0x0400:
				sprintf(prbuf, "�ҵ���:S50");
				sprintf(prbuf1, "����:0x%X", Rrfid);
				break;
			case 0x0800:
				sprintf(prbuf, "�ҵ���:Pro");
				sprintf(prbuf1, "����:0x%X", Rrfid);
				break;
			case 0x4400:
				sprintf(prbuf, "�ҵ���:Ult");
				sprintf(prbuf1, "����:0x%X", Rrfid);
				break;
			case 0x4403:
				sprintf(prbuf, "�ҵ���:DES");
				sprintf(prbuf1, "����:0x%X", Rrfid);
				break;
			default:
				sprintf(prbuf, "����");
				sprintf(prbuf1, "����:0x%X", Rrfid);
				break;
			}
			_Printfxy(8, 36, prbuf, 0);
			_Printfxy(8, 52, prbuf1, 0);

			break;
		case KEY_2:
			_Printfxy(0, 20, "                  ", 0);
			_Printfxy(0, 36, "                  ", 0);
			_Printfxy(0, 52, "                  ", 0);
			_Printfxy(8, 20, "������...", 0);
			Rrfid = _RfidReadCardId(cardid);
			if (Rrfid == RFID_OK)
			{
				sprintf(prbuf, "����:%X%X%X%X", cardid[0], cardid[1], cardid[2], cardid[3]);
			}
			else
			{
				sprintf(prbuf, "����");
			}
			_Printfxy(8, 36, prbuf, 0);
			break;
		case KEY_3:
			_Printfxy(0, 20, "                  ", 0);
			_Printfxy(0, 36, "                  ", 0);
			_Printfxy(0, 52, "                  ", 0);
			_Printfxy(8, 20, "ѡ��...", 0);
			Rrfid = _RfidSelectCard(cardid);
			if (Rrfid == 0x00)
			{
				sprintf(prbuf, "ѡ��:%X%X%X%X", cardid[0], cardid[1], cardid[2], cardid[3]);
			}
			else
			{
				sprintf(prbuf, "����");
			}
			sprintf(prbuf1, "����:0x%X", Rrfid);
			_Printfxy(8, 36, prbuf, 0);
			_Printfxy(8, 52, prbuf1, 0);
			break;
		case KEY_4:
			_Printfxy(0, 20, "                  ", 0);
			_Printfxy(0, 36, "                  ", 0);
			_Printfxy(0, 52, "                  ", 0);
			_Printfxy(8, 20, "��֤��...", 0);
			Rrfid = _RfidCheckPwd(pwd, cardid, 1, 0x60);
			if (Rrfid == 0x00)
			{
				sprintf(prbuf, "ͨ��:%X%X%X%X", cardid[0], cardid[1], cardid[2], cardid[3]);
			}
			else
			{
				sprintf(prbuf, "����");
			}
			sprintf(prbuf1, "����:0x%X", Rrfid);
			_Printfxy(8, 36, prbuf, 0);
			_Printfxy(8, 52, prbuf1, 0);
			break;
		case KEY_5:
			_Printfxy(0, 20, "                  ", 0);
			_Printfxy(0, 36, "                  ", 0);
			_Printfxy(0, 52, "                  ", 0);
			_Printfxy(8, 20, "д��...", 0);
			Rrfid = _RfidWriteData(wdata, 1);
			if (Rrfid == 0x00)
			{
				sprintf(prbuf, "�ɹ�");
			}
			else
			{
				sprintf(prbuf, "����");
			}
			sprintf(prbuf1, "����:0x%X", Rrfid);
			_Printfxy(8, 36, prbuf, 0);
			_Printfxy(8, 52, prbuf1, 0);
			break;
		case KEY_6:
			_Printfxy(0, 20, "                  ", 0);
			_Printfxy(0, 36, "                  ", 0);
			_Printfxy(0, 52, "                  ", 0);
			_Printfxy(8, 20, "����...", 0);
			Rrfid = _RfidReadData(rdata, 1);
			if (Rrfid == RFID_OK)
			{
				sprintf(prbuf, "�ɹ�");
			}
			else
			{
				sprintf(prbuf, "����");
			}
			_Printfxy(8, 36, prbuf, 0);
			_Printfxy(0, 54, (char *)rdata, 0);
			break;
		case KEY_7:
			_Printfxy(0, 20, "                  ", 0);
			_Printfxy(0, 36, "                  ", 0);
			_Printfxy(0, 52, "                  ", 0);
			_Printfxy(8, 20, "���߿�...", 0);
			Rrfid = _RfidDormancy();
			if (Rrfid == 0x00)
			{
				sprintf(prbuf, "���߳ɹ�");
			}
			else
			{
				sprintf(prbuf, "����");
			}
			sprintf(prbuf1, "����:0x%X", Rrfid);
			_Printfxy(8, 36, prbuf, 0);
			_Printfxy(8, 52, prbuf1, 0);
			break;
		case KEY_8:
			_Printfxy(0, 20, "                  ", 0);
			_Printfxy(0, 36, "                  ", 0);
			_Printfxy(0, 52, "                  ", 0);
			_Printfxy(8, 20, "��λ...", 0);
			Rrfid = _RfidReset();
			if (Rrfid == 0x00)
			{
				sprintf(prbuf, "�ɹ�");
			}
			else
			{
				sprintf(prbuf, "����");
			}
			sprintf(prbuf1, "����:0x%X", Rrfid);
			_Printfxy(8, 36, prbuf, 0);
			_Printfxy(8, 52, prbuf1, 0);
			break;
		case KEY_9:
			_Printfxy(0, 20, "                  ", 0);
			_Printfxy(0, 36, "                  ", 0);
			_Printfxy(0, 52, "                  ", 0);
			_Printfxy(8, 20, "������...", 0);
			Rrfid = _RfidOpenAntenna();
			if (Rrfid == 0x00)
			{
				sprintf(prbuf, "�ɹ�");
			}
			else
			{
				sprintf(prbuf, "����");
			}
			sprintf(prbuf1, "����:0x%X", Rrfid);
			_Printfxy(8, 36, prbuf, 0);
			_Printfxy(8, 52, prbuf1, 0);
			break;
		case KEY_0:
			_Printfxy(0, 20, "                  ", 0);
			_Printfxy(0, 36, "                  ", 0);
			_Printfxy(0, 52, "                  ", 0);
			_Printfxy(8, 20, "������...", 0);
			Rrfid = _RfidCloseAntenna();
			if (Rrfid == 0x00)
			{
				sprintf(prbuf, "�ɹ�");
			}
			else
			{
				sprintf(prbuf, "����");
			}
			sprintf(prbuf1, "����:0x%X", Rrfid);
			_Printfxy(8, 36, prbuf, 0);
			_Printfxy(8, 52, prbuf1, 0);
			break;
		case KEY_ENTER:
			_ClearScreen();
			_Printfxy(18, 0, "RFID����", 0);
			break;
		case KEY_CANCEL:
			return;
			break;
		}
	}
}

void RFIC(void)
{

	_GuiMenuStru MainMenu;
	MainMenu.left = 0;
	MainMenu.top = 0;
	MainMenu.no = 2;
	MainMenu.title = "    ��ѡ��";
	MainMenu.str[0] = " 125K  ";
	MainMenu.str[1] = "13.56M";
	MainMenu.key[0] = "1";
	MainMenu.key[1] = "2";
	MainMenu.Function[0] = Rfic125k;
	MainMenu.Function[1] = RFIC1356;
	MainMenu.FunctionEx = 0;

	_Menu(&MainMenu);
}

/******

extern uint8  _Menu(_GuiMenuStru *);
*******/
int main(void)
{

	_GuiMenuStru MainMenu;
	MainMenu.left = 0;
	MainMenu.top = 0;
	MainMenu.no = 8;
	MainMenu.title = 0;
	MainMenu.str[0] = " �ļ����� ";
	MainMenu.str[1] = " ���ݿ⺯�� ";
	MainMenu.str[2] = " ʱ���� ";
	MainMenu.str[3] = " Ӳ���� ";
	MainMenu.str[4] = " ϵͳ�� ";
	MainMenu.str[5] = " �˿��� ";
	MainMenu.str[6] = " Һ���� ";
	MainMenu.str[7] = " RFIC ";
	MainMenu.key[0] = "1";
	MainMenu.key[1] = "2";
	MainMenu.key[2] = "3";
	MainMenu.key[3] = "4";
	MainMenu.key[4] = "5";
	MainMenu.key[5] = "6";
	MainMenu.key[6] = "7";
	MainMenu.key[7] = "8";
	MainMenu.Function[0] = File;
	MainMenu.Function[1] = dbf;
	MainMenu.Function[2] = time;
	MainMenu.Function[3] = Hardware;
	MainMenu.Function[4] = GetSys;
	MainMenu.Function[5] = COMTEST;
	MainMenu.Function[6] = dispbase;
	MainMenu.Function[7] = RFIC;
	MainMenu.FunctionEx = 0;

	_Menu(&MainMenu);
}
