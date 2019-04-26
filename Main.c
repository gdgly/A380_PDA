#include <HJLIB.H>
#include "string.h"
//#include "dbf.h"
#include "stdio.h"

#include "Common.h"
#include "Tool.h"
//#include "PowerMeter.h"
#include "WaterMeter.h"


// --------------------------------  ȫ�ֱ���  -----------------------------------------
char Screenbuff[160*(160/3+1)*2]; 
uint8 TmpBuf[1080];
uint8 TxBuf[1080];
uint8 RxBuf[1080];
uint32 RxLen, TxLen;
const char * ZeroAddr = "00000000000000";	// 0 ��ַ��14�ַ�
const uint8 LocalAddr[7] = {0x19, 0x20, 0x00, 0x00, 0x19, 0x20, 0x00};	// ��ַ 00201900002019��14�ַ�
uint8 DstAddr[7];
uint8 CurrCmd;
ParamsBuf Addrs;		
ParamsBuf Args;
ParamsBuf Disps;
uint8 InputBuff_1[20] = {0};
uint8 InputBuff_2[20] = {0};
uint8 InputBuff_Tmp1[20] = {0};
uint8 InputBuff_Tmp2[20] = {0};


// --------------------------------  ��������/��ȡ  -----------------------------------------



// --------------------------------  �ֳ�����  -----------------------------------------



// --------------------------------  ������ģ��ͨ��  -----------------------------------------

void CenterCmdFunc(void)
{
	
}


// --------------------------------  ���ģ��ͨ��  -----------------------------------------

void PowerCmdFunc(void)
{
	uint8 key, menuItemNo, tryCnt = 0, i;
	_GuiLisStruEx menuList;
	_GuiInputBoxStru inputBox;
	int inputLen;
	uint8 * ptr;

	_ClearScreen();

	return;
	
	#if 0

	// �˵�
	menuList.title = "<< �����ӽڵ�ͨ�� ";
	menuList.no = 4;
	menuList.MaxNum = 4;
	menuList.isRt = 0;
	menuList.x = 0;
	menuList.y = 0;
	menuList.with = 10 * 16;
	menuList.str[0] = "  1. ��ȡ����汾";
	menuList.str[1] = "  2. ��ȡ�ڵ�����";
	menuList.str[2] = "  3. ��ȡ���书��";
	menuList.str[3] = "  4. 645-07����";
	menuList.defbar = 1;
	//_GUIHLine(0, 4*16 + 8, 160, 1);

	// �����
	inputBox.top = 2 * 16;
	inputBox.left = 3 * 16;
	inputBox.width = 7 * 16;
	inputBox.hight = 16;
	inputBox.caption = "";
	inputBox.context = InputBuff_Tmp1;
	inputBox.type = 1;		// ����
	inputBox.datelen = 12;	// ��󳤶�
	inputBox.keyUpDown = 1; 
	inputBox.IsClear = 1;
	_SetInputMode(1); 		//�������뷽ʽ 
	_DisInputMode(0);		//���뷨�Ƿ������л�

	_CloseCom();
	_ComSetTran(3);
	_ComSet((uint8 *)"19200,E,8,1", 2);

	while(1){

		_ClearScreen();
		menuItemNo = _ListEx(&menuList);

		if (menuItemNo == 0){
			break;
		}
		menuList.defbar = menuItemNo;

		while(1){
			
			_ClearScreen();

			// �������� :  ������ʾ/���봦��
			ptr = menuList.str[menuItemNo - 1];
			sprintf(TmpBuf, "<< %s",&ptr[5]);
			_Printfxy(0, 0, TmpBuf, 0);
			/*---------------------------------------------*/
			_GUIHLine(0, 1*16 + 4, 160, 1);	
			_Printfxy(0, 2*16, "��ַ:", 0);
			_Printfxy(0, 8*16, "����            ȷ��", 0);

			if(InputBuff_1[0] != '\0'){
				memcpy(inputBox.context, InputBuff_1, 20);
			}
			key = _InputBox(&inputBox);
			if (key == KEY_CANCEL)
				break;

			inputLen = strlen(inputBox.context);

			if(inputLen == 0 && InputBuff_1[0] != '\0'){
				memcpy(inputBox.context, InputBuff_1, 20);
				inputLen = 12;
			}
			if(inputLen == 0 || strncmp(ZeroAddr, inputBox.context, inputLen) == 0){
				_Printfxy(0, 4*16, "��������Ч��ַ", 0);
				_ReadKey();
				continue;
			}
			StringPadLeft(inputBox.context, 12, '0');
			memcpy(InputBuff_1, inputBox.context, 20);
			GetBytesFromStringHex(DstAddr, 0, inputBox.context, 0, true);
			PrintfXyMultiLine_VaList(0, 2*16, "��ַ: %s", InputBuff_1);

			// �����������
			switch(menuItemNo){
			case 1:		// " ��ȡ����汾 ";
				CurrCmd = PowerCmd_ReadVerInfo;
				/*---------------------------------------------*/
				Args.cnt = 1;
				memcpy(Args.buf, DstAddr, 6);
				Args.items[0] = &Args.buf[0];
				break;

			case 2:		// " ��ȡ�ڵ����� "
				CurrCmd = PowerCmd_ReadNodeInfo;
				/*---------------------------------------------*/
				Args.cnt = 1;
				memcpy(Args.buf, DstAddr, 6);
				Args.items[0] = &Args.buf[0];
				break;

			case 3:		// " ��ȡ���书��" "
				CurrCmd = PowerCmd_ReadSendPower;
				/*---------------------------------------------*/
				Args.cnt = 1;
				memcpy(Args.buf, DstAddr, 6);
				Args.items[0] = &Args.buf[0];
				break;
			
			case 4:		// " 645-07���� ";
				CurrCmd = PowerCmd_ReadMeter_645_07;
				/*---------------------------------------------*/
				Args.cnt = 2;
				memcpy(Args.buf, DstAddr, 6);
				Args.items[0] = &Args.buf[0];
				Args.buf[6] = 0;
				Args.items[1] = &Args.buf[6];
				break;

				default: 
					break;
			}

			if(key != KEY_ENTER){
			 	if (key == KEY_CANCEL){
					break;
				}else{
					continue;
				}
			}
			
			// ���� 
			TxLen = PackElectricRequestFrame(TxBuf, DstAddr, CurrCmd, Args.items, 0);
			_GetComStr(RxBuf, 1024, 10);	// clear , 100ms timeout
			_SendComStr(TxBuf, TxLen);
			_Printfxy(0, 9*16, "    �����...   ", 0);

			// ����
			RxLen = _GetComStr(RxBuf, 100, 100);	// recv , 500ms timeout
			if(false == ExplainElectricResponseFrame(RxBuf, RxLen, LocalAddr, CurrCmd, &Disps)){
				PrintfXyMultiLine_VaList(0, 9*16, "    ʧ��:%s", Disps.items[0]);
				_ReadKey();
				continue;
			}
			else{
				_Printfxy(0, 9*16, "      ����ɹ�     ", 0);
			}
			
			// ��ʾ���
			PrintfXyMultiLine(0, 3 * 16, Disps.items[0]);

			key = _ReadKey();

			if (key == KEY_CANCEL){
				break;
			}else{
				continue;
			}
		}
		
	}

	_CloseCom();

	#endif
}

// --------------------------------  ˮ��ģ��ͨ��  -----------------------------------------

// 1	��������
void WaterCmdFunc_CommonCmd(void)
{
	uint8 key, menuItemNo, tryCnt = 0, i;
	_GuiLisStruEx menuList;
	_GuiInputBoxStru inputBox;
	int inputLen;
	uint8 * ptr;

	_ClearScreen();

	// �˵�
	menuList.title = "<< ˮ��������";
	menuList.no = 4;
	menuList.MaxNum = 4;
	menuList.isRt = 0;
	menuList.x = 0;
	menuList.y = 0;
	menuList.with = 10 * 16;
	menuList.str[0] = "  1. ��ȡ�û�����";
	menuList.str[1] = "  2. ��ȡ������ת����";
	menuList.str[2] = "  3. ����";
	menuList.str[3] = "  4. ǿ�ƿ���";
	menuList.defbar = 1;
	//_GUIHLine(0, 4*16 + 8, 160, 1);

	// �����
	inputBox.top = 2 * 16;
	inputBox.left = 3 * 16;
	inputBox.width = 7 * 16;
	inputBox.hight = 16;
	inputBox.caption = "";
	inputBox.context = InputBuff_Tmp1;
	inputBox.type = 1;		// ����
	inputBox.datelen = 12;	// ��󳤶�
	inputBox.keyUpDown = 1; 
	inputBox.IsClear = 1;
	_SetInputMode(1); 		//�������뷽ʽ 
	_DisInputMode(0);		//���뷨�Ƿ������л�

	_CloseCom();
	_ComSetTran(3);
	_ComSet((uint8 *)"9600,E,8,1", 2);

	while(1){

		_ClearScreen();
		menuItemNo = _ListEx(&menuList);

		if (menuItemNo == 0){
			break;
		}
		menuList.defbar = menuItemNo;

		while(1){
			
			_ClearScreen();

			// �������� :  ������ʾ/���봦��
			ptr = menuList.str[menuItemNo - 1];
			sprintf(TmpBuf, "<< %s",&ptr[5]);
			_Printfxy(0, 0, TmpBuf, 0);
			/*---------------------------------------------*/
			_GUIHLine(0, 1*16 + 4, 160, 1);	
			_Printfxy(0, 2*16, "��ַ:", 0);
			_Printfxy(0, 8*16, "����            ȷ��", 0);

			if(InputBuff_1[0] != '\0'){
				memcpy(inputBox.context, InputBuff_1, 20);
			}
			key = _InputBox(&inputBox);
			if (key == KEY_CANCEL)
				break;

			inputLen = strlen(inputBox.context);

			if(inputLen == 0 && InputBuff_1[0] != '\0'){
				memcpy(inputBox.context, InputBuff_1, 20);
				inputLen = 12;
			}
			if(inputLen == 0 || strncmp(ZeroAddr, inputBox.context, inputLen) == 0){
				_Printfxy(0, 4*16, "��������Ч��ַ", 0);
				_ReadKey();
				continue;
			}
			StringPadLeft(inputBox.context, 12, '0');
			memcpy(InputBuff_1, inputBox.context, 20);
			GetBytesFromStringHex(DstAddr, 0, inputBox.context, 0, true);
			PrintfXyMultiLine_VaList(0, 2*16, "��ַ: %s", InputBuff_1);

			// �����������
			CurrCmd = (0x10 + menuItemNo);
			switch(CurrCmd){
			case WaterCmd_ReadRealTimeData:		// "��ȡ�û�����"
				/*---------------------------------------------*/
				// ��ַ
				Addrs.cnt = 1;
				memcpy(&Addrs.buf[0], LocalAddr, 6);
				memcpy(&Addrs.buf[6], DstAddr, 6);
				Addrs.items[0] = &Args.buf[0];
				// �����֡�������
				Args.cnt = 2;
				Args.buf[0] = 0x01;
				Args.buf[1] = 0x00;
				Args.items[0] = &Args.buf[0];
				Args.items[1] = &Args.buf[1];
				Args.len = 1;
				break;

			case WaterCmd_ReadFrozenData:		// "  "
				/*---------------------------------------------*/
				Args.cnt = 1;
				memcpy(Args.buf, DstAddr, 6);
				Args.items[0] = &Args.buf[0];
				break;

			case WaterCmd_OpenValve:		// "  "
				/*---------------------------------------------*/
				Args.cnt = 1;
				memcpy(Args.buf, DstAddr, 6);
				Args.items[0] = &Args.buf[0];
				break;
			
			case WaterCmd_OpenValveForce:		// "  ";
				/*---------------------------------------------*/
				Args.cnt = 2;
				memcpy(Args.buf, DstAddr, 6);
				Args.items[0] = &Args.buf[0];
				Args.buf[6] = 0;
				Args.items[1] = &Args.buf[6];
				break;

				default: 
					break;
			}

			if(key != KEY_ENTER){
			 	if (key == KEY_CANCEL){
					break;
				}else{
					continue;
				}
			}
			
			// ���� 
			TxLen = PackWater6009RequestFrame(TxBuf, &Addrs, CurrCmd, &Args, 0);
			_GetComStr(RxBuf, 1024, 10);	// clear , 100ms timeout
			_SendComStr(TxBuf, TxLen);
			_Printfxy(0, 9*16, "    �����...   ", 0);

			// ����
			RxLen = _GetComStr(RxBuf, 100, 300);	// recv , 300 X 10ms timeout
			if(false == ExplainWater6009ResponseFrame(RxBuf, RxLen, LocalAddr, CurrCmd, &Disps)){
				PrintfXyMultiLine_VaList(0, 9*16, "    ʧ��:%s", Disps.items[0]);
				_ReadKey();
				continue;
			}
			else{
				_Printfxy(0, 9*16, "      ����ɹ�     ", 0);
			}
			
			// ��ʾ���
			PrintfXyMultiLine(0, 3 * 16, Disps.items[0]);

			key = _ReadKey();

			if (key == KEY_CANCEL){
				break;
			}else{
				continue;
			}
		}
		
	}

	_CloseCom();
}

// 2	��������
void WaterCmdFunc_TestCmd(void)
{

}

// 3	��������
void WaterCmdFunc_Upgrade(void)
{

}

// 4	Ԥ������
void WaterCmdFunc_PrepaiedVal(void)
{

}

// 5	��������
void WaterCmdFunc_WorkingParams(void)
{

}

// 6	��������
void WaterCmdFunc_Other(void)
{

}

void WaterCmdFunc(void)
{
	_GuiMenuStru menu;
	
	menu.left=0;
	menu.top=0;
	menu.no=6;
	menu.title= "<< ˮ��ͨ�� ";
	menu.str[0]=" �������� ";
	menu.str[1]=" �������� ";
	menu.str[2]=" �������� ";
	menu.str[3]=" Ԥ������ ";
	menu.str[4]=" �������� ";
	menu.str[5]=" �������� ";
	menu.key[0]="1";
	menu.key[1]="2";
	menu.key[2]="3";
	menu.key[3]="4";
	menu.key[4]="5";
	menu.key[5]="6";
	menu.Function[0]=WaterCmdFunc_CommonCmd;
	menu.Function[1]=WaterCmdFunc_TestCmd;
	menu.Function[2]=WaterCmdFunc_Upgrade;
	menu.Function[3]=WaterCmdFunc_PrepaiedVal;
	menu.Function[4]=WaterCmdFunc_WorkingParams;
	menu.Function[5]=WaterCmdFunc_Other;
	menu.FunctionEx=0;
	_Menu(&menu);	
}

// --------------------------------  ͸��ģ������  -----------------------------------------
void TransParentModuleFunc(void)
{
	uint8 key, menuItemNo, tryCnt = 0;
	_GuiLisStruEx menuList;
	char *fileName = NULL;
	char tmp[70];
	int fileHdl, fileLen, totalCnt, sendCnt;
	int index;
	
	_ClearScreen();

	// �˵�
	menuList.title = "<< ͸��ģ������ ";
	menuList.no = 3;
	menuList.MaxNum = 3;
	menuList.isRt = 0;
	menuList.x = 0;
	menuList.y = 0;
	menuList.with = 10 * 16;
	menuList.str[0] = "  1. �鿴��ǰ�汾";
	menuList.str[1] = "  2. �������ļ�";
	menuList.str[2] = "  3. ��ʼ����";
	menuList.defbar = 1;
	_GUIHLine(0, 4*16 + 8, 160, 1);

	_CloseCom();
	_ComSetTran(3);
	_ComSet((uint8 *)"19200,E,8,1", 2);

	while(1){

		menuItemNo = _ListEx(&menuList);

		if (menuItemNo == 0){
			break;
		}

		menuList.defbar = menuItemNo;

		switch(menuItemNo){
		case 1:	// " �鿴��ǰ�汾 ";
			_GUIRectangleFill(0, 5 * 16, 160, 9 * 16, 0);
			TxLen = 0;
			TxBuf[TxLen++] = 0xAA;
			TxBuf[TxLen++] = 0xBB;
			TxBuf[TxLen++] = 0x01;
			TxBuf[TxLen++] = 0x07;
			TxBuf[TxLen++] = 0xCC;
			_GetComStr(RxBuf, 1024, 10);	// clear , 100ms timeout
			_SendComStr(TxBuf, TxLen);
			_Printfxy(0, 5*16, "��ѯ��...", 0);

			sprintf(TxBuf, "��ǰ�汾:");
			RxLen = _GetComStr(&TxBuf[9], 50, 50);	// recv , 500ms timeout
			if(RxLen < 30 || strncmp(&TxBuf[9], "SRWF-", 5) != 0)
			{
				_Printfxy(0, 5*16, "���ճ�ʱ", 0);
				break;
			}
			_Printfxy(0, 5*16, &TxBuf[0], 0);
			_Printfxy(0, 6*16, &TxBuf[20], 0);
			_Printfxy(0, 7*16, &TxBuf[40], 0);
			break;

		case 2:	// " �������ļ� "
			_GUIRectangleFill(0, 5 * 16, 160, 9 * 16, 0);

			_SaveScreenToBuff(Screenbuff);
			_ClearScreen();
			fileName = _GetFileList("ѡ|\n��|\n��|\n��|\n��|\n��|\n  |\n  |\n", "", "");
			_ClearScreen();
			_RestoreBuffToScreen(Screenbuff);

			if (fileName == NULL){
				break;
			}
			
			sprintf(tmp, "�ļ�: %s\0", fileName);
			_Printfxy(0, 5*16, &tmp[0], 0);
			_Printfxy(0, 6*16, &tmp[20], 0);

			fileHdl = _Fopen(fileName, "R");
			fileLen = _Filelenth(fileHdl);
			totalCnt = (fileLen + 1023)/1024;
			sendCnt = 0;
			_Fread(TxBuf, 1024, fileHdl);
			_Fclose(fileHdl);
				
			index = IndexOf(TxBuf, 1024, "SRWF-", 5, 512, 512);
			if(index < 0){
				_Printfxy(0, 7*16, "����4E88-APP�ļ�", 0);
				fileName = NULL;
			}
			else{
				sprintf(tmp, "��С:%dK,�ܰ���:%d\0", fileLen/1024, totalCnt);
				_Printfxy(0, 7*16, &tmp[0], 0);
			}
			break;
			
		case 3:	// " ��ʼ���� ";
			_GUIRectangleFill(0, 5 * 16, 160, 9 * 16, 0);
			totalCnt = 200;
			
			// ��ʼ��
			if (fileName == NULL){
				_Printfxy(0, 5*16, "����ѡ�������ļ�", 0);
				break;
			}
			fileHdl = _Fopen(fileName, "R");
			sendCnt = 0;

			sprintf(tmp, "�ܰ���: %d\0", totalCnt);
			_Printfxy(0, 5*16, &tmp[0], 0);
			sprintf(tmp, "������: %d   \0",sendCnt);
			_Printfxy(0, 6*16, &tmp[0], 0);
			_Printfxy(0, 9*16, "״̬: ������...", 0);

			ShowProgressBar(7*16+8, totalCnt, sendCnt);

			// ��������
			while(1){

				if(tryCnt > 3 || sendCnt >= totalCnt){
					break;
				}
				
				TxLen = _Fread(TxBuf, 1024, fileHdl);
				_GetComStr(RxBuf, 1024, 1);		// clear , 100ms timeout
				_SendComStr(TxBuf, TxLen);

				sprintf(tmp, "������: %d   \0",sendCnt + 1);
				_Printfxy(0, 6*16, &tmp[0], 0);
				if(tryCnt > 0){
					sprintf(tmp, "����%d \0",tryCnt);
					_Printfxy(6*16, 6*16, &tmp[0], 0);
				}
				tryCnt++;

				RxLen = _GetComStr(&TxBuf[9], 50, 1);	// recv , 500ms timeout
				if(RxLen < 10){
				//	continue;
				}

				sendCnt++;
				tryCnt = 0;
				ShowProgressBar(7*16+8, totalCnt, sendCnt);

			}
			_Fclose(fileHdl);

			// �������
			if(tryCnt > 3){
				_Printfxy(0, 9*16, "״̬: ����ʧ��  ", 0);
			}else{
				_Printfxy(0, 9*16, "״̬: �������  ", 0);
			}
			_SoundOn();
			_Sleep(500);
			_SoundOff();
			_Sleep(300);
			_SoundOn();
			_Sleep(500);
			_SoundOff();
			break;

			default: 
				break;
		}
	}

	_CloseCom();
}


// --------------------------------   ������   -----------------------------------------------
int main(void)
{
	_GuiMenuStru MainMenu;
	
	MainMenu.left=0;
	MainMenu.top=0;
	MainMenu.no=4;
	MainMenu.title= "   ɣ���ֳֻ�  ";
	MainMenu.str[0]=" ������ͨ�� ";
	MainMenu.str[1]=" ˮ��ͨ�� ";
	MainMenu.str[2]=" ���ͨ�� ";
	MainMenu.str[3]=" ͸��ģ������ ";
	MainMenu.key[0]="1";
	MainMenu.key[1]="2";
	MainMenu.key[2]="3";
	MainMenu.key[3]="4";
	MainMenu.Function[0]=CenterCmdFunc;
	MainMenu.Function[1]=WaterCmdFunc;
	MainMenu.Function[2]=PowerCmdFunc;
	MainMenu.Function[3]=TransParentModuleFunc;
	MainMenu.FunctionEx=0;
	_OpenLcdBackLight();
	_Menu(&MainMenu);	
}

