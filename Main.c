
#include <HJLIB.H>
#include "string.h"
#include "dbf.h"
#include "stdio.h"

#include "Common.h"
#include "Tool.h"
#include "PowerMeter.h"
#include "WaterMeter.h"


// --------------------------------  ȫ�ֱ���  -----------------------------------------
char Screenbuff[160*(160/3+1)*2]; 
uint8 TmpBuf[1080];
uint8 TxBuf[1080];
uint8 RxBuf[1080];
uint32 RxLen, TxLen;
uint8 LocalAddr[7] = { 0x20, 0x19, 0x00, 0x00, 0x20, 0x19, 0x00};	// ��ַ 201900002019��12�ַ�
uint8 DstAddr[7];
uint8 CurrCmd;
ParamsBuf Addrs;		
ParamsBuf Args;
ParamsBuf Disps;
uint8 StrDstAddr[TXTBUF_LEN] = {0};
uint8 StrRelayAddr[RELAY_MAX][TXTBUF_LEN] = {0};
UI_ItemList UiList;

//--------------------------------------	6009ˮ������ ���͡����ա������ʾ	----------------------------

bool Protol6009Tranceiver(uint8 cmdid, ParamsBuf *addrs, ParamsBuf *args, uint16 ackLen, uint16 timeout, uint8 tryCnt)
{
	bool ret;
	uint8 sendCnt = 0;
	
	do{
		// ���� 
		TxLen = PackWater6009RequestFrame(TxBuf, addrs, cmdid, args, tryCnt);
		_GetComStr(TmpBuf, 1024, 100/10);	// clear , 100ms timeout
		_SendComStr(TxBuf, TxLen);
		sendCnt++;
		if(sendCnt == 1){
			_Printfxy(0, 9*16, "״̬: <�����>    ", Color_White);
		}
		else{
			PrintfXyMultiLine_VaList(0, 9*16, "״̬: <�����ط� %d>  ", sendCnt);
		}

		// ����
		RxLen = 0;
		PrintfXyMultiLine_VaList(0, 7*16, "���ս��� %d/%d", RxLen, ackLen);
		RxLen = _GetComStr(RxBuf, ackLen, timeout/10);
		PrintfXyMultiLine_VaList(0, 7*16, "���ս��� %d/%d", RxLen, ackLen);
		_SoundOn();
		_Sleep(50);
		_SoundOff();

		if(false == ExplainWater6009ResponseFrame(RxBuf, RxLen, LocalAddr, CurrCmd, &Disps)){
			PrintfXyMultiLine_VaList(0, 9*16, "״̬: <ʧ�ܣ�%s>    ", Disps.items[0]);
			ret = false;
			_Sleep(50);
			_SoundOn();
			_Sleep(50);
			_SoundOff();
		}
		else{
			
			_Printfxy(0, 9*16, "״̬: <����ɹ�>    ", Color_White);
				
			// ��ʾ���
			_GUIRectangleFill(0, 0, 10*16, 8*16, Color_White);
			PrintfXyMultiLine_VaList(0, 0*16, "���: %s", StrDstAddr);
			PrintfXyMultiLine(0, 1 * 16, Disps.items[0]);
			ret = true;
		}
	}while(sendCnt < tryCnt && ret == false);

	return ret;
}


// --------------------------------  ������ģ��ͨ��  -----------------------------------------
// 1	��������
void CenterCmdFunc_CommonCmd(void)
{
	uint8 key, menuItemNo, tryCnt = 0, i;
	_GuiLisStruEx menuList;
	UI_Item * pUiItems = &UiList.items[0];
	uint8 * pUiCnt = &UiList.cnt;
	uint8 * pChar;
	uint8 currUiItem = 0;
	uint16 ackLen, timeout;

	_ClearScreen();

	// �˵�
	menuList.title = "<< ��������";
	menuList.no = 9;
	menuList.MaxNum = 9;
	menuList.isRt = 0;
	menuList.x = 0;
	menuList.y = 0;
	menuList.with = 10 * 16;
	menuList.str[0] = "  1. ����������";
	menuList.str[1] = "  2. ���������汾";
	menuList.str[2] = "  3. ��������ʱ��";
	menuList.str[3] = "  4. �輯����ʱ��";
	menuList.str[4] = "  5. ��GPRS����";
	menuList.str[5] = "  6. ��GPRS����";
	menuList.str[6] = "  7. ��GPRS�ź�ǿ��";
	menuList.str[7] = "  8. ��������ʼ��";
	menuList.str[8] = "  9. ������������ģʽ";
	menuList.defbar = 1;

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

			// �������� :  ������ʾ
			pChar = menuList.str[menuItemNo - 1];
			sprintf(TmpBuf, "<< %s",&pChar[5]);
			_Printfxy(0, 0, TmpBuf, Color_White);
			_GUIHLine(0, 1*16 + 4, 160, Color_Black);	
			/*---------------------------------------------*/

			for(i = 0; i < RELAY_MAX; i++){
				StrRelayAddr[i][0] = 0x00;
			}
			sprintf(StrRelayAddr[0], "  �о����� ");
			(*pUiCnt) = 0;
			TextBoxCreate(&pUiItems[(*pUiCnt)++], 0, 2*16, "�� ��:", StrDstAddr, 12, 13*8);
			TextBoxCreate(&pUiItems[(*pUiCnt)++], 0, 3*16, "�м�1:", StrRelayAddr[0], 12, 13*8);
			TextBoxCreate(&pUiItems[(*pUiCnt)++], 0, 4*16, "�м�2:", StrRelayAddr[1], 12, 13*8);
			TextBoxCreate(&pUiItems[(*pUiCnt)++], 0, 5*16, "�м�3:", StrRelayAddr[2], 12, 13*8);

			// �����������
			CurrCmd = (0x10 + menuItemNo);
			switch(CurrCmd){
			case WaterCmd_ReadRealTimeData:		// "��ȡ�û�����"
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			case WaterCmd_ReadFrozenData:		// "  "
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			case WaterCmd_OpenValve:			// "  "
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;
			
			case WaterCmd_OpenValveForce:		// "  ";
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

            case WaterCmd_CloseValve:		// "  ";
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			case WaterCmd_CloseValveForce:		// "  ";
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			case WaterCmd_ClearException:		// "  ";
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			default: 
				break;
			}


			if (key == KEY_CANCEL){
				break;
			}

			if(StrDstAddr[0] == 0x00 ){
				sprintf(StrDstAddr, "����������");
				continue;
			}

			// 6009 Э���ַ��д���÷���
			GetBytesFromStringHex(DstAddr, 0, 6, StrDstAddr, 0, false);
			PrintfXyMultiLine_VaList(0, 2*16, "�� ��: %s", StrDstAddr);

			// ����ַ
			Addrs.itemCnt = 0;
			Addrs.items[Addrs.itemCnt] = &Addrs.buf[0];
			memcpy(Addrs.items[Addrs.itemCnt], LocalAddr, 6);
			Addrs.itemCnt++;
			for(i = 0; i < RELAY_MAX; i++){
				if(StrRelayAddr[i][0] != 0x00){
					Addrs.items[Addrs.itemCnt] = &Addrs.buf[6 + i*6];
					GetBytesFromStringHex(DstAddr, 0, 6, StrRelayAddr[i], 0, false);
					Addrs.itemCnt++;
				}
			}
			Addrs.items[Addrs.itemCnt] = &Addrs.buf[6 + i*6];
			memcpy(Addrs.items[Addrs.itemCnt], DstAddr, 6);
			Addrs.itemCnt++;

			timeout = 3500 + (Addrs.itemCnt - 2) * 3500 * 2;
			
			// ���͡����ա������ʾ
			Protol6009Tranceiver(CurrCmd, &Addrs, &Args, ackLen, timeout, tryCnt);
			
			// ���� / ����
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
void CenterCmdFunc_DocumentOperation(void)
{
	uint8 key, menuItemNo, tryCnt = 0, i;
	_GuiLisStruEx menuList;
	UI_Item * pUiItems = &UiList.items[0];
	uint8 * pUiCnt = &UiList.cnt;
	uint8 * pChar;
	uint8 currUiItem = 0;
	uint16 ackLen, timeout;

	_ClearScreen();

	// �˵�
	menuList.title = "<< ��������";
	menuList.no = 5;
	menuList.MaxNum = 5;
	menuList.isRt = 0;
	menuList.x = 0;
	menuList.y = 0;
	menuList.with = 10 * 16;
	menuList.str[0] = "  1. ����������";
	menuList.str[1] = "  2. ��������Ϣ";
	menuList.str[2] = "  3. ��ӵ�����Ϣ";
	menuList.str[3] = "  4. ɾ��������Ϣ";
	menuList.str[4] = "  5. �޸ĵ�����Ϣ";
	menuList.defbar = 1;

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

			// �������� :  ������ʾ
			pChar = menuList.str[menuItemNo - 1];
			sprintf(TmpBuf, "<< %s",&pChar[5]);
			_Printfxy(0, 0, TmpBuf, Color_White);
			_GUIHLine(0, 1*16 + 4, 160, Color_Black);	
			/*---------------------------------------------*/

			for(i = 0; i < RELAY_MAX; i++){
				StrRelayAddr[i][0] = 0x00;
				sprintf(StrRelayAddr[i], " <��ѡ> ");
			}

			(*pUiCnt) = 0;
			TextBoxCreate(&pUiItems[(*pUiCnt)++], 0, 2*16, "�� ��:", StrDstAddr, 12, 13*8);
			TextBoxCreate(&pUiItems[(*pUiCnt)++], 0, 3*16, "�м�1:", StrRelayAddr[0], 12, 13*8);
			TextBoxCreate(&pUiItems[(*pUiCnt)++], 0, 4*16, "�м�2:", StrRelayAddr[1], 12, 13*8);
			TextBoxCreate(&pUiItems[(*pUiCnt)++], 0, 5*16, "�м�3:", StrRelayAddr[2], 12, 13*8);

			// �����������
			CurrCmd = (0x10 + menuItemNo);
			switch(CurrCmd){
			case WaterCmd_ReadRealTimeData:		// "��ȡ�û�����"
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			case WaterCmd_ReadFrozenData:		// "  "
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			case WaterCmd_OpenValve:			// "  "
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;
			
			case WaterCmd_OpenValveForce:		// "  ";
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

            case WaterCmd_CloseValve:		// "  ";
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			case WaterCmd_CloseValveForce:		// "  ";
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			case WaterCmd_ClearException:		// "  ";
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			default: 
				break;
			}


			if (key == KEY_CANCEL){
				break;
			}

			if(StrDstAddr[0] == 0x00 ){
				sprintf(StrDstAddr, "����������");
				continue;
			}

			// 6009 Э���ַ��д���÷���
			GetBytesFromStringHex(DstAddr, 0, 6, StrDstAddr, 0, false);
			PrintfXyMultiLine_VaList(0, 2*16, "�� ��: %s", StrDstAddr);

			// ����ַ
			Addrs.itemCnt = 0;
			Addrs.items[Addrs.itemCnt] = &Addrs.buf[0];
			memcpy(Addrs.items[Addrs.itemCnt], LocalAddr, 6);
			Addrs.itemCnt++;
			for(i = 0; i < RELAY_MAX; i++){
				if(StrRelayAddr[i][0] != 0x00){
					Addrs.items[Addrs.itemCnt] = &Addrs.buf[6 + i*6];
					GetBytesFromStringHex(DstAddr, 0, 6, StrRelayAddr[i], 0, false);
					Addrs.itemCnt++;
				}
			}
			Addrs.items[Addrs.itemCnt] = &Addrs.buf[6 + i*6];
			memcpy(Addrs.items[Addrs.itemCnt], DstAddr, 6);
			Addrs.itemCnt++;

			timeout = 3500 + (Addrs.itemCnt - 2) * 3500 * 2;
			
			// ���͡����ա������ʾ
			Protol6009Tranceiver(CurrCmd, &Addrs, &Args, ackLen, timeout, tryCnt);
			
			// ���� / ����
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

// 3	·������
void CenterCmdFunc_RouteSetting(void)
{
	uint8 key, menuItemNo, tryCnt = 0, i;
	_GuiLisStruEx menuList;
	UI_Item * pUiItems = &UiList.items[0];
	uint8 * pUiCnt = &UiList.cnt;
	uint8 * pChar;
	uint8 currUiItem = 0;
	uint16 ackLen, timeout;

	_ClearScreen();

	// �˵�
	menuList.title = "<<·������";
	menuList.no = 2;
	menuList.MaxNum = 2;
	menuList.isRt = 0;
	menuList.x = 0;
	menuList.y = 0;
	menuList.with = 10 * 16;
	menuList.str[0] = "  1. ���Զ���·��";
	menuList.str[1] = "  2. ���Զ���·��";
	menuList.defbar = 1;

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

			// �������� :  ������ʾ
			pChar = menuList.str[menuItemNo - 1];
			sprintf(TmpBuf, "<< %s",&pChar[5]);
			_Printfxy(0, 0, TmpBuf, Color_White);
			_GUIHLine(0, 1*16 + 4, 160, Color_Black);	
			/*---------------------------------------------*/

			for(i = 0; i < RELAY_MAX; i++){
				StrRelayAddr[i][0] = 0x00;
			}
			sprintf(StrRelayAddr[0], "  �о����� ");
			(*pUiCnt) = 0;
			TextBoxCreate(&pUiItems[(*pUiCnt)++], 0, 2*16, "�� ��:", StrDstAddr, 12, 13*8);
			TextBoxCreate(&pUiItems[(*pUiCnt)++], 0, 3*16, "�м�1:", StrRelayAddr[0], 12, 13*8);
			TextBoxCreate(&pUiItems[(*pUiCnt)++], 0, 4*16, "�м�2:", StrRelayAddr[1], 12, 13*8);
			TextBoxCreate(&pUiItems[(*pUiCnt)++], 0, 5*16, "�м�3:", StrRelayAddr[2], 12, 13*8);

			// �����������
			CurrCmd = (0x10 + menuItemNo);
			switch(CurrCmd){
			case WaterCmd_ReadRealTimeData:		// "��ȡ�û�����"
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			case WaterCmd_ReadFrozenData:		// "  "
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			case WaterCmd_OpenValve:			// "  "
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;
			
			case WaterCmd_OpenValveForce:		// "  ";
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

            case WaterCmd_CloseValve:		// "  ";
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			case WaterCmd_CloseValveForce:		// "  ";
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			case WaterCmd_ClearException:		// "  ";
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			default: 
				break;
			}


			if (key == KEY_CANCEL){
				break;
			}

			if(StrDstAddr[0] == 0x00 ){
				sprintf(StrDstAddr, "����������");
				continue;
			}

			// 6009 Э���ַ��д���÷���
			GetBytesFromStringHex(DstAddr, 0, 6, StrDstAddr, 0, false);
			PrintfXyMultiLine_VaList(0, 2*16, "�� ��: %s", StrDstAddr);

			// ����ַ
			Addrs.itemCnt = 0;
			Addrs.items[Addrs.itemCnt] = &Addrs.buf[0];
			memcpy(Addrs.items[Addrs.itemCnt], LocalAddr, 6);
			Addrs.itemCnt++;
			for(i = 0; i < RELAY_MAX; i++){
				if(StrRelayAddr[i][0] != 0x00){
					Addrs.items[Addrs.itemCnt] = &Addrs.buf[6 + i*6];
					GetBytesFromStringHex(DstAddr, 0, 6, StrRelayAddr[i], 0, false);
					Addrs.itemCnt++;
				}
			}
			Addrs.items[Addrs.itemCnt] = &Addrs.buf[6 + i*6];
			memcpy(Addrs.items[Addrs.itemCnt], DstAddr, 6);
			Addrs.itemCnt++;

			timeout = 3500 + (Addrs.itemCnt - 2) * 3500 * 2;
			
			// ���͡����ա������ʾ
			Protol6009Tranceiver(CurrCmd, &Addrs, &Args, ackLen, timeout, tryCnt);
			
			// ���� / ����
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

// 4	����ת��
void CenterCmdFunc_CommandTransfer(void)
{
	uint8 key, menuItemNo, tryCnt = 0, i;
	_GuiLisStruEx menuList;
	UI_Item * pUiItems = &UiList.items[0];
	uint8 * pUiCnt = &UiList.cnt;
	uint8 * pChar;
	uint8 currUiItem = 0;
	uint16 ackLen, timeout;

	_ClearScreen();

	// �˵�
	menuList.title = "<< ����ת��";
	menuList.no = 7;
	menuList.MaxNum = 7;
	menuList.isRt = 0;
	menuList.x = 0;
	menuList.y = 0;
	menuList.with = 10 * 16;
	menuList.str[0] = "  1. ��ʵʱ����";
	menuList.str[1] = "  2. ����ʱ��������";
	menuList.str[2] = "  3. ����������";
	menuList.str[3] = "  4. ����";
	menuList.str[4] = "  5. �ط�";
	menuList.str[5] = "  6. ��ʹ��";
	menuList.str[6] = "  7. ���쳣";
	menuList.defbar = 1;

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

			// �������� :  ������ʾ
			pChar = menuList.str[menuItemNo - 1];
			sprintf(TmpBuf, "<< %s",&pChar[5]);
			_Printfxy(0, 0, TmpBuf, Color_White);
			_GUIHLine(0, 1*16 + 4, 160, Color_Black);	
			/*---------------------------------------------*/

			for(i = 0; i < RELAY_MAX; i++){
				StrRelayAddr[i][0] = 0x00;
			}
			sprintf(StrRelayAddr[0], "  �о����� ");
			(*pUiCnt) = 0;
			TextBoxCreate(&pUiItems[(*pUiCnt)++], 0, 2*16, "�� ��:", StrDstAddr, 12, 13*8);
			TextBoxCreate(&pUiItems[(*pUiCnt)++], 0, 3*16, "�м�1:", StrRelayAddr[0], 12, 13*8);
			TextBoxCreate(&pUiItems[(*pUiCnt)++], 0, 4*16, "�м�2:", StrRelayAddr[1], 12, 13*8);
			TextBoxCreate(&pUiItems[(*pUiCnt)++], 0, 5*16, "�м�3:", StrRelayAddr[2], 12, 13*8);

			// �����������
			CurrCmd = (0x10 + menuItemNo);
			switch(CurrCmd){
			case WaterCmd_ReadRealTimeData:		// "��ȡ�û�����"
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			case WaterCmd_ReadFrozenData:		// "  "
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			case WaterCmd_OpenValve:			// "  "
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;
			
			case WaterCmd_OpenValveForce:		// "  ";
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

            case WaterCmd_CloseValve:		// "  ";
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			case WaterCmd_CloseValveForce:		// "  ";
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			case WaterCmd_ClearException:		// "  ";
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			default: 
				break;
			}


			if (key == KEY_CANCEL){
				break;
			}

			if(StrDstAddr[0] == 0x00 ){
				sprintf(StrDstAddr, "����������");
				continue;
			}

			// 6009 Э���ַ��д���÷���
			GetBytesFromStringHex(DstAddr, 0, 6, StrDstAddr, 0, false);
			PrintfXyMultiLine_VaList(0, 2*16, "�� ��: %s", StrDstAddr);

			// ����ַ
			Addrs.itemCnt = 0;
			Addrs.items[Addrs.itemCnt] = &Addrs.buf[0];
			memcpy(Addrs.items[Addrs.itemCnt], LocalAddr, 6);
			Addrs.itemCnt++;
			for(i = 0; i < RELAY_MAX; i++){
				if(StrRelayAddr[i][0] != 0x00){
					Addrs.items[Addrs.itemCnt] = &Addrs.buf[6 + i*6];
					GetBytesFromStringHex(DstAddr, 0, 6, StrRelayAddr[i], 0, false);
					Addrs.itemCnt++;
				}
			}
			Addrs.items[Addrs.itemCnt] = &Addrs.buf[6 + i*6];
			memcpy(Addrs.items[Addrs.itemCnt], DstAddr, 6);
			Addrs.itemCnt++;

			timeout = 3500 + (Addrs.itemCnt - 2) * 3500 * 2;
			
			// ���͡����ա������ʾ
			Protol6009Tranceiver(CurrCmd, &Addrs, &Args, ackLen, timeout, tryCnt);
			
			// ���� / ����
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

void CenterCmdFunc(void)
{
	_GuiMenuStru menu;
	
	menu.left=0;
	menu.top=0;
	menu.no=4;
	menu.title= "<< ������ͨ�� ";
	menu.str[0]=" �������� ";
	menu.str[1]=" �������� ";
	menu.str[2]=" ·������ ";
	menu.str[3]=" ����ת�� ";
	menu.key[0]="1";
	menu.key[1]="2";
	menu.key[2]="3";
	menu.key[3]="4";
	menu.Function[0]=CenterCmdFunc_CommonCmd;
	menu.Function[1]=CenterCmdFunc_DocumentOperation;
	menu.Function[2]=CenterCmdFunc_RouteSetting;
	menu.Function[3]=CenterCmdFunc_CommandTransfer;
	menu.FunctionEx=0;
	_Menu(&menu);	
	
}


// --------------------------------  ���ģ��ͨ��  -----------------------------------------

void PowerCmdFunc(void)
{
	uint8 key, menuItemNo, tryCnt = 0, i;
	_GuiLisStruEx menuList;
	UI_Item * pUiItems = &UiList.items[0];
	uint8 * pUiCnt = &UiList.cnt;
	uint8 * pChar;
	uint8 currUiItem = 0;
	uint16 ackLen, timeout;

	_ClearScreen();

	return;
	
	#if 1

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
			pChar = menuList.str[menuItemNo - 1];
			sprintf(TmpBuf, "<< %s",&pChar[5]);
			_Printfxy(0, 0, TmpBuf, 0);
			_GUIHLine(0, 1*16 + 4, 160, 1);	
			/*---------------------------------------------*/
			
			for(i = 0; i < RELAY_MAX; i++){
				StrRelayAddr[i][0] = 0x00;
			}
			sprintf(StrRelayAddr[0], "  �о����� ");
			(*pUiCnt) = 0;
			TextBoxCreate(&pUiItems[(*pUiCnt)++], 0, 2*16, "�� ��:", StrDstAddr, 12, 13*8);
			TextBoxCreate(&pUiItems[(*pUiCnt)++], 0, 3*16, "�м�1:", StrRelayAddr[0], 12, 13*8);
			TextBoxCreate(&pUiItems[(*pUiCnt)++], 0, 4*16, "�м�2:", StrRelayAddr[1], 12, 13*8);
			TextBoxCreate(&pUiItems[(*pUiCnt)++], 0, 5*16, "�м�3:", StrRelayAddr[2], 12, 13*8);

			GetBytesFromStringHex(DstAddr, 0, 6, StrDstAddr, 0, false);
			PrintfXyMultiLine_VaList(0, 2*16, "�� ��: %s", StrDstAddr);

			// �����������
			switch(menuItemNo){
			case 1:		// " ��ȡ����汾 ";
				CurrCmd = PowerCmd_ReadVerInfo;
				/*---------------------------------------------*/
				Args.itemCnt = 1;
				memcpy(Args.buf, DstAddr, 6);
				Args.items[0] = &Args.buf[0];
				break;

			case 2:		// " ��ȡ�ڵ����� "
				CurrCmd = PowerCmd_ReadNodeInfo;
				/*---------------------------------------------*/
				Args.itemCnt = 1;
				memcpy(Args.buf, DstAddr, 6);
				Args.items[0] = &Args.buf[0];
				break;

			case 3:		// " ��ȡ���书��" "
				CurrCmd = PowerCmd_ReadSendPower;
				/*---------------------------------------------*/
				Args.itemCnt = 1;
				memcpy(Args.buf, DstAddr, 6);
				Args.items[0] = &Args.buf[0];
				break;
			
			case 4:		// " 645-07���� ";
				CurrCmd = PowerCmd_ReadMeter_645_07;
				/*---------------------------------------------*/
				Args.itemCnt = 2;
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

			// ����ַ
			Addrs.itemCnt = 0;
			Addrs.items[Addrs.itemCnt] = &Addrs.buf[0];
			memcpy(Addrs.items[Addrs.itemCnt], LocalAddr, 6);
			Addrs.itemCnt++;
			for(i = 0; i < RELAY_MAX; i++){
				if(StrRelayAddr[i][0] != 0x00){
					Addrs.items[Addrs.itemCnt] = &Addrs.buf[6 + i*6];
					GetBytesFromStringHex(DstAddr, 0, 6, StrRelayAddr[i], 0, false);
					Addrs.itemCnt++;
				}
			}
			Addrs.items[Addrs.itemCnt] = &Addrs.buf[6 + i*6];
			memcpy(Addrs.items[Addrs.itemCnt], DstAddr, 6);
			Addrs.itemCnt++;

			timeout = (Addrs.itemCnt -1) * 300 * 2;		// timeout - ms
			
			// ���� 
			TxLen = PackElectricRequestFrame(TxBuf, DstAddr, CurrCmd, Args.items, 0);
			_GetComStr(RxBuf, 1024, 10);	// clear , 100ms timeout
			_SendComStr(TxBuf, TxLen);
			_Printfxy(0, 9*16, "    �����...   ", 0);

			// ����
			RxLen = _GetComStr(RxBuf, 100, timeout / 10);	// recv , timeout - ms
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
	UI_Item * pUiItems = &UiList.items[0];
	uint8 * pUiCnt = &UiList.cnt;
	uint8 * pChar;
	uint8 currUiItem = 0;
	uint16 ackLen, timeout;

	_ClearScreen();

	// �˵�
	menuList.title = "<<��������";
	menuList.no = 7;
	menuList.MaxNum = 7;
	menuList.isRt = 0;
	menuList.x = 0;
	menuList.y = 0;
	menuList.with = 10 * 16;
	menuList.str[0] = "  1. ��ȡ�û�����";
	menuList.str[1] = "  2. ��ȡ������ת����";
	menuList.str[2] = "  3. ����";
	menuList.str[3] = "  4. ǿ�ƿ���";
	menuList.str[4] = "  5. �ط�";
	menuList.str[5] = "  6. ǿ�ƹط�";
	menuList.str[6] = "  7. ���쳣����";
	menuList.defbar = 1;

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

			// �������� :  ������ʾ
			pChar = menuList.str[menuItemNo - 1];
			sprintf(TmpBuf, "<< %s",&pChar[5]);
			_Printfxy(0, 0, TmpBuf, Color_White);
			_GUIHLine(0, 1*16 + 4, 160, Color_Black);	
			/*---------------------------------------------*/

			for(i = 0; i < RELAY_MAX; i++){
				if(StrRelayAddr[i][0] > '9' || StrRelayAddr[i][0] < '0'){
					StrRelayAddr[i][0] = 0x00;
					sprintf(StrRelayAddr[i], " <��ѡ> ");
				}
			}

			(*pUiCnt) = 0;
			TextBoxCreate(&pUiItems[(*pUiCnt)++], 0, 2*16, "�� ��:", StrDstAddr, 12, 13*8);
			TextBoxCreate(&pUiItems[(*pUiCnt)++], 0, 3*16, "�м�1:", StrRelayAddr[0], 12, 13*8);
			TextBoxCreate(&pUiItems[(*pUiCnt)++], 0, 4*16, "�м�2:", StrRelayAddr[1], 12, 13*8);
			TextBoxCreate(&pUiItems[(*pUiCnt)++], 0, 5*16, "�м�3:", StrRelayAddr[2], 12, 13*8);

			// �����������
			CurrCmd = (0x10 + menuItemNo);
			switch(CurrCmd){
			case WaterCmd_ReadRealTimeData:		// "��ȡ�û�����"
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   // ������	01
				Args.items[1] = &Args.buf[1];	// ������
                *Args.items[0] = 0x01;
				ackLen = 21;					// Ӧ�𳤶� 21	
				i = 0;		
				*Args.items[i++] = 0x00;		// ���ݸ�ʽ 00	
				Args.lastItemLen = i;
				break;

			case WaterCmd_ReadFrozenData:		// "  "
				/*---------------------------------------------*/
				TextBoxCreate(&pUiItems[(*pUiCnt)++], 0, 6*16, "�����������0-9:", &TmpBuf[0], 1, 2*8);
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 3;
				Args.items[0] = &Args.buf[0];   // ������	02
				Args.items[1] = &Args.buf[1];	// ���ݸ�ʽ 02
				Args.items[2] = &Args.buf[2];	// ʱ��
				Args.items[3] = &Args.buf[9];	// �����������
                *Args.items[0] = 0x02;			
				*Args.items[1] = 0x02;	

				Args.lastItemLen = 1;
				break;

			case WaterCmd_OpenValve:			// "  "
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;
			
			case WaterCmd_OpenValveForce:		// "  ";
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

            case WaterCmd_CloseValve:		// "  ";
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			case WaterCmd_CloseValveForce:		// "  ";
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			case WaterCmd_ClearException:		// "  ";
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			default: 
				break;
			}


			if (key == KEY_CANCEL){
				break;
			}

			if(StrDstAddr[0] == 0x00 ){
				sprintf(StrDstAddr, "����������");
				continue;
			}

			// 6009 Э���ַ��д���÷���
			GetBytesFromStringHex(DstAddr, 0, 6, StrDstAddr, 0, false);
			PrintfXyMultiLine_VaList(0, 2*16, "�� ��: %s", StrDstAddr);

			// ����ַ
			Addrs.itemCnt = 0;
			Addrs.items[Addrs.itemCnt] = &Addrs.buf[0];
			memcpy(Addrs.items[Addrs.itemCnt], LocalAddr, 6);
			Addrs.itemCnt++;
			for(i = 0; i < RELAY_MAX; i++){
				if(StrRelayAddr[i][0] >= '0' && StrRelayAddr[i][0] <= '9'){
					Addrs.items[Addrs.itemCnt] = &Addrs.buf[6 + i*6];
					GetBytesFromStringHex(Addrs.items[Addrs.itemCnt], 0, 6, StrRelayAddr[i], 0, false);
					Addrs.itemCnt++;
				}
			}
			Addrs.items[Addrs.itemCnt] = &Addrs.buf[6 + i*6];
			memcpy(Addrs.items[Addrs.itemCnt], DstAddr, 6);
			Addrs.itemCnt++;

			ackLen += 14 + Addrs.itemCnt * 6;
			timeout = 6500 + (Addrs.itemCnt - 2) * 6000 * 2;

			// ���͡����ա������ʾ
			Protol6009Tranceiver(CurrCmd, &Addrs, &Args, ackLen, timeout, tryCnt);
			
			// ������ - ���ֽ������
			do{
				key = _ReadKey();
			}
			while(key != KEY_CANCEL && key != KEY_ENTER);
			
			// ���� / ����
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
	uint8 key, menuItemNo, tryCnt = 0, i;
	_GuiLisStruEx menuList;
	UI_Item * pUiItems = &UiList.items[0];
	uint8 * pUiCnt = &UiList.cnt;
	uint8 * pChar;
	uint8 currUiItem = 0;
	uint16 ackLen, timeout;

	_ClearScreen();

	// �˵�
	menuList.title = "<<��������";
	menuList.no = 8;
	menuList.MaxNum = 8;
	menuList.isRt = 0;
	menuList.x = 0;
	menuList.y = 0;
	menuList.with = 10 * 16;
	menuList.str[0] = "  1. �������";
	menuList.str[1] = "  2. �����¶�";
	menuList.str[2] = "  3. �����ѹ";
	menuList.str[3] = "  4. ��Ԥ�ɲο���";
	menuList.str[4] = "  5. ���ù�����ʱ";
	menuList.str[5] = "  6. ����Ӫ�̱��";
	menuList.str[6] = "  7. ���ϱ�·��";
	menuList.str[7] = "  8. ���ñ��";
	menuList.defbar = 1;

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

			// �������� :  ������ʾ
			pChar = menuList.str[menuItemNo - 1];
			sprintf(TmpBuf, "<< %s",&pChar[5]);
			_Printfxy(0, 0, TmpBuf, Color_White);
			_GUIHLine(0, 1*16 + 4, 160, Color_Black);	
			/*---------------------------------------------*/

			for(i = 0; i < RELAY_MAX; i++){
				StrRelayAddr[i][0] = 0x00;
			}
			sprintf(StrRelayAddr[0], "  �о����� ");
			(*pUiCnt) = 0;
			TextBoxCreate(&pUiItems[(*pUiCnt)++], 0, 2*16, "�� ��:", StrDstAddr, 12, 13*8);
			TextBoxCreate(&pUiItems[(*pUiCnt)++], 0, 3*16, "�м�1:", StrRelayAddr[0], 12, 13*8);
			TextBoxCreate(&pUiItems[(*pUiCnt)++], 0, 4*16, "�м�2:", StrRelayAddr[1], 12, 13*8);
			TextBoxCreate(&pUiItems[(*pUiCnt)++], 0, 5*16, "�м�3:", StrRelayAddr[2], 12, 13*8);

			// �����������
			CurrCmd = (0x10 + menuItemNo);
			switch(CurrCmd){
			case WaterCmd_ReadRealTimeData:		// "��ȡ�û�����"
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			case WaterCmd_ReadFrozenData:		// "  "
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			case WaterCmd_OpenValve:			// "  "
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;
			
			case WaterCmd_OpenValveForce:		// "  ";
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

            case WaterCmd_CloseValve:		// "  ";
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			case WaterCmd_CloseValveForce:		// "  ";
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			case WaterCmd_ClearException:		// "  ";
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			default: 
				break;
			}


			if (key == KEY_CANCEL){
				break;
			}

			if(StrDstAddr[0] == 0x00 ){
				sprintf(StrDstAddr, "����������");
				continue;
			}

			// 6009 Э���ַ��д���÷���
			GetBytesFromStringHex(DstAddr, 0, 6, StrDstAddr, 0, false);
			PrintfXyMultiLine_VaList(0, 2*16, "�� ��: %s", StrDstAddr);

			// ����ַ
			Addrs.itemCnt = 0;
			Addrs.items[Addrs.itemCnt] = &Addrs.buf[0];
			memcpy(Addrs.items[Addrs.itemCnt], LocalAddr, 6);
			Addrs.itemCnt++;
			for(i = 0; i < RELAY_MAX; i++){
				if(StrRelayAddr[i][0] != 0x00){
					Addrs.items[Addrs.itemCnt] = &Addrs.buf[6 + i*6];
					GetBytesFromStringHex(DstAddr, 0, 6, StrRelayAddr[i], 0, false);
					Addrs.itemCnt++;
				}
			}
			Addrs.items[Addrs.itemCnt] = &Addrs.buf[6 + i*6];
			memcpy(Addrs.items[Addrs.itemCnt], DstAddr, 6);
			Addrs.itemCnt++;

			timeout = 3500 + (Addrs.itemCnt - 2) * 3500 * 2;
			
			// ���͡����ա������ʾ
			Protol6009Tranceiver(CurrCmd, &Addrs, &Args, ackLen, timeout, tryCnt);
			
			// ���� / ����
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

// 3	��������
void WaterCmdFunc_Upgrade(void)
{
	uint8 key, menuItemNo, tryCnt = 0, i;
	_GuiLisStruEx menuList;
	UI_Item * pUiItems = &UiList.items[0];
	uint8 * pUiCnt = &UiList.cnt;
	uint8 * pChar;
	uint8 currUiItem = 0;
	uint16 ackLen, timeout;

	_ClearScreen();

	// �˵�
	menuList.title = "<<��������";
	menuList.no = 7;
	menuList.MaxNum = 7;
	menuList.isRt = 0;
	menuList.x = 0;
	menuList.y = 0;
	menuList.with = 10 * 16;
	menuList.str[0] = "  1. ��������";
	menuList.str[1] = "  2. ��ѯ����";
	menuList.str[2] = "  3. �㲥����";
	menuList.str[3] = "  4. ��ӵ���";
	menuList.str[4] = "  5. ɾ������";
	menuList.str[5] = "  6. ��ѯ����";
	menuList.str[6] = "  7. ����ͳ��";
	menuList.defbar = 1;

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

			// �������� :  ������ʾ
			pChar = menuList.str[menuItemNo - 1];
			sprintf(TmpBuf, "<< %s",&pChar[5]);
			_Printfxy(0, 0, TmpBuf, Color_White);
			_GUIHLine(0, 1*16 + 4, 160, Color_Black);	
			/*---------------------------------------------*/

			for(i = 0; i < RELAY_MAX; i++){
				StrRelayAddr[i][0] = 0x00;
			}
			sprintf(StrRelayAddr[0], "  �о����� ");
			(*pUiCnt) = 0;
			TextBoxCreate(&pUiItems[(*pUiCnt)++], 0, 2*16, "�� ��:", StrDstAddr, 12, 13*8);
			TextBoxCreate(&pUiItems[(*pUiCnt)++], 0, 3*16, "�м�1:", StrRelayAddr[0], 12, 13*8);
			TextBoxCreate(&pUiItems[(*pUiCnt)++], 0, 4*16, "�м�2:", StrRelayAddr[1], 12, 13*8);
			TextBoxCreate(&pUiItems[(*pUiCnt)++], 0, 5*16, "�м�3:", StrRelayAddr[2], 12, 13*8);

			// �����������
			CurrCmd = (0x10 + menuItemNo);
			switch(CurrCmd){
			case WaterCmd_ReadRealTimeData:		// "��ȡ�û�����"
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			case WaterCmd_ReadFrozenData:		// "  "
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			case WaterCmd_OpenValve:			// "  "
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;
			
			case WaterCmd_OpenValveForce:		// "  ";
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

            case WaterCmd_CloseValve:		// "  ";
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			case WaterCmd_CloseValveForce:		// "  ";
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			case WaterCmd_ClearException:		// "  ";
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			default: 
				break;
			}


			if (key == KEY_CANCEL){
				break;
			}

			if(StrDstAddr[0] == 0x00 ){
				sprintf(StrDstAddr, "����������");
				continue;
			}

			// 6009 Э���ַ��д���÷���
			GetBytesFromStringHex(DstAddr, 0, 6, StrDstAddr, 0, false);
			PrintfXyMultiLine_VaList(0, 2*16, "�� ��: %s", StrDstAddr);

			// ����ַ
			Addrs.itemCnt = 0;
			Addrs.items[Addrs.itemCnt] = &Addrs.buf[0];
			memcpy(Addrs.items[Addrs.itemCnt], LocalAddr, 6);
			Addrs.itemCnt++;
			for(i = 0; i < RELAY_MAX; i++){
				if(StrRelayAddr[i][0] != 0x00){
					Addrs.items[Addrs.itemCnt] = &Addrs.buf[6 + i*6];
					GetBytesFromStringHex(DstAddr, 0, 6, StrRelayAddr[i], 0, false);
					Addrs.itemCnt++;
				}
			}
			Addrs.items[Addrs.itemCnt] = &Addrs.buf[6 + i*6];
			memcpy(Addrs.items[Addrs.itemCnt], DstAddr, 6);
			Addrs.itemCnt++;

			timeout = 3500 + (Addrs.itemCnt - 2) * 3500 * 2;
			
			// ���͡����ա������ʾ
			Protol6009Tranceiver(CurrCmd, &Addrs, &Args, ackLen, timeout, tryCnt);
			
			// ���� / ����
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

// 4	Ԥ������
void WaterCmdFunc_PrepaiedVal(void)
{
	uint8 key, menuItemNo, tryCnt = 0, i;
	_GuiLisStruEx menuList;
	UI_Item * pUiItems = &UiList.items[0];
	uint8 * pUiCnt = &UiList.cnt;
	uint8 * pChar;
	uint8 currUiItem = 0;
	uint16 ackLen, timeout;

	_ClearScreen();

	// �˵�
	menuList.title = "<<Ԥ������";
	menuList.no = 6;
	menuList.MaxNum = 6;
	menuList.isRt = 0;
	menuList.x = 0;
	menuList.y = 0;
	menuList.with = 10 * 16;
	menuList.str[0] = "  1. ��Ԥ�ɲο�����";
	menuList.str[1] = "  2. ��Ԥ�ɲο�����";
	menuList.str[2] = "  3. ��������ֵ͸֧";
	menuList.str[3] = "  4. �豨����ֵ";
	menuList.str[4] = "  5. ��ط���ֵ";
	menuList.str[5] = "  6. �豨���ط���ֵ";
	menuList.defbar = 1;

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

			// �������� :  ������ʾ
			pChar = menuList.str[menuItemNo - 1];
			sprintf(TmpBuf, "<< %s",&pChar[5]);
			_Printfxy(0, 0, TmpBuf, Color_White);
			_GUIHLine(0, 1*16 + 4, 160, Color_Black);	
			/*---------------------------------------------*/

			for(i = 0; i < RELAY_MAX; i++){
				StrRelayAddr[i][0] = 0x00;
			}
			sprintf(StrRelayAddr[0], "  �о����� ");
			(*pUiCnt) = 0;
			TextBoxCreate(&pUiItems[(*pUiCnt)++], 0, 2*16, "�� ��:", StrDstAddr, 12, 13*8);
			TextBoxCreate(&pUiItems[(*pUiCnt)++], 0, 3*16, "�м�1:", StrRelayAddr[0], 12, 13*8);
			TextBoxCreate(&pUiItems[(*pUiCnt)++], 0, 4*16, "�м�2:", StrRelayAddr[1], 12, 13*8);
			TextBoxCreate(&pUiItems[(*pUiCnt)++], 0, 5*16, "�м�3:", StrRelayAddr[2], 12, 13*8);

			// �����������
			CurrCmd = (0x10 + menuItemNo);
			switch(CurrCmd){
			case WaterCmd_ReadRealTimeData:		// "��ȡ�û�����"
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			case WaterCmd_ReadFrozenData:		// "  "
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			case WaterCmd_OpenValve:			// "  "
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;
			
			case WaterCmd_OpenValveForce:		// "  ";
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

            case WaterCmd_CloseValve:		// "  ";
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			case WaterCmd_CloseValveForce:		// "  ";
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			case WaterCmd_ClearException:		// "  ";
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			default: 
				break;
			}


			if (key == KEY_CANCEL){
				break;
			}

			if(StrDstAddr[0] == 0x00 ){
				sprintf(StrDstAddr, "����������");
				continue;
			}

			// 6009 Э���ַ��д���÷���
			GetBytesFromStringHex(DstAddr, 0, 6, StrDstAddr, 0, false);
			PrintfXyMultiLine_VaList(0, 2*16, "�� ��: %s", StrDstAddr);

			// ����ַ
			Addrs.itemCnt = 0;
			Addrs.items[Addrs.itemCnt] = &Addrs.buf[0];
			memcpy(Addrs.items[Addrs.itemCnt], LocalAddr, 6);
			Addrs.itemCnt++;
			for(i = 0; i < RELAY_MAX; i++){
				if(StrRelayAddr[i][0] != 0x00){
					Addrs.items[Addrs.itemCnt] = &Addrs.buf[6 + i*6];
					GetBytesFromStringHex(DstAddr, 0, 6, StrRelayAddr[i], 0, false);
					Addrs.itemCnt++;
				}
			}
			Addrs.items[Addrs.itemCnt] = &Addrs.buf[6 + i*6];
			memcpy(Addrs.items[Addrs.itemCnt], DstAddr, 6);
			Addrs.itemCnt++;

			timeout = 3500 + (Addrs.itemCnt - 2) * 3500 * 2;
			
			// ���͡����ա������ʾ
			Protol6009Tranceiver(CurrCmd, &Addrs, &Args, ackLen, timeout, tryCnt);
			
			// ���� / ����
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

// 5	��������
void WaterCmdFunc_WorkingParams(void)
{
	uint8 key, menuItemNo, tryCnt = 0, i;
	_GuiLisStruEx menuList;
	UI_Item * pUiItems = &UiList.items[0];
	uint8 * pUiCnt = &UiList.cnt;
	uint8 * pChar;
	uint8 currUiItem = 0;
	uint16 ackLen, timeout;

	_ClearScreen();

	// �˵�
	menuList.title = "<<��������";
	menuList.no = 8;
	menuList.MaxNum = 8;
	menuList.isRt = 0;
	menuList.x = 0;
	menuList.y = 0;
	menuList.with = 10 * 16;
	menuList.str[0] = "1. ����������ϵ��";
	menuList.str[1] = "2. �����ת��������";
	menuList.str[2] = "3. ��ȡ����ʹ��״̬";
	menuList.str[3] = "4. ���ö�ʱ�ϴ�";
	menuList.str[4] = "5. ���ö����ϴ�";
	menuList.str[5] = "6. ���ö�ʱ�����ϴ�";
	menuList.str[6] = "7. �����ʱ��";
	menuList.str[7] = "8. У���ʱ��";
	menuList.defbar = 1;

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

			// �������� :  ������ʾ
			pChar = menuList.str[menuItemNo - 1];
			sprintf(TmpBuf, "<< %s",&pChar[5]);
			_Printfxy(0, 0, TmpBuf, Color_White);
			_GUIHLine(0, 1*16 + 4, 160, Color_Black);	
			/*---------------------------------------------*/

			for(i = 0; i < RELAY_MAX; i++){
				StrRelayAddr[i][0] = 0x00;
			}
			sprintf(StrRelayAddr[0], "  �о����� ");
			(*pUiCnt) = 0;
			TextBoxCreate(&pUiItems[(*pUiCnt)++], 0, 2*16, "�� ��:", StrDstAddr, 12, 13*8);
			TextBoxCreate(&pUiItems[(*pUiCnt)++], 0, 3*16, "�м�1:", StrRelayAddr[0], 12, 13*8);
			TextBoxCreate(&pUiItems[(*pUiCnt)++], 0, 4*16, "�м�2:", StrRelayAddr[1], 12, 13*8);
			TextBoxCreate(&pUiItems[(*pUiCnt)++], 0, 5*16, "�м�3:", StrRelayAddr[2], 12, 13*8);

			// �����������
			CurrCmd = (0x10 + menuItemNo);
			switch(CurrCmd){
			case WaterCmd_ReadRealTimeData:		// "��ȡ�û�����"
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			case WaterCmd_ReadFrozenData:		// "  "
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			case WaterCmd_OpenValve:			// "  "
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;
			
			case WaterCmd_OpenValveForce:		// "  ";
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

            case WaterCmd_CloseValve:		// "  ";
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			case WaterCmd_CloseValveForce:		// "  ";
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			case WaterCmd_ClearException:		// "  ";
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			default: 
				break;
			}


			if (key == KEY_CANCEL){
				break;
			}

			if(StrDstAddr[0] == 0x00 ){
				sprintf(StrDstAddr, "����������");
				continue;
			}

			// 6009 Э���ַ��д���÷���
			GetBytesFromStringHex(DstAddr, 0, 6, StrDstAddr, 0, false);
			PrintfXyMultiLine_VaList(0, 2*16, "�� ��: %s", StrDstAddr);

			// ����ַ
			Addrs.itemCnt = 0;
			Addrs.items[Addrs.itemCnt] = &Addrs.buf[0];
			memcpy(Addrs.items[Addrs.itemCnt], LocalAddr, 6);
			Addrs.itemCnt++;
			for(i = 0; i < RELAY_MAX; i++){
				if(StrRelayAddr[i][0] != 0x00){
					Addrs.items[Addrs.itemCnt] = &Addrs.buf[6 + i*6];
					GetBytesFromStringHex(DstAddr, 0, 6, StrRelayAddr[i], 0, false);
					Addrs.itemCnt++;
				}
			}
			Addrs.items[Addrs.itemCnt] = &Addrs.buf[6 + i*6];
			memcpy(Addrs.items[Addrs.itemCnt], DstAddr, 6);
			Addrs.itemCnt++;

			timeout = 3500 + (Addrs.itemCnt - 2) * 3500 * 2;
			
			// ���͡����ա������ʾ
			Protol6009Tranceiver(CurrCmd, &Addrs, &Args, ackLen, timeout, tryCnt);
			
			// ���� / ����
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

// 6	��������
void WaterCmdFunc_Other(void)
{
	uint8 key, menuItemNo, tryCnt = 0, i;
	_GuiLisStruEx menuList;
	UI_Item * pUiItems = &UiList.items[0];
	uint8 * pUiCnt = &UiList.cnt;
	uint8 * pChar;
	uint8 currUiItem = 0;
	uint16 ackLen, timeout;

	_ClearScreen();

	// �˵�
	menuList.title = "<<��������";
	menuList.no = 5;
	menuList.MaxNum = 5;
	menuList.isRt = 0;
	menuList.x = 0;
	menuList.y = 0;
	menuList.with = 10 * 16;
	menuList.str[0] = "1. ����/��/���Ŵ���";
	menuList.str[1] = "2. ��ȡRXD��TXD�ŵ�";
	menuList.str[2] = "3. ����RXD��TXD�ŵ�";
	menuList.str[3] = "4. ������Ӫ�̱��";
	menuList.str[4] = "5. ·���·�";
	menuList.defbar = 1;

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

			// �������� :  ������ʾ
			pChar = menuList.str[menuItemNo - 1];
			sprintf(TmpBuf, "<< %s",&pChar[5]);
			_Printfxy(0, 0, TmpBuf, Color_White);
			_GUIHLine(0, 1*16 + 4, 160, Color_Black);	
			/*---------------------------------------------*/

			for(i = 0; i < RELAY_MAX; i++){
				StrRelayAddr[i][0] = 0x00;
			}
			sprintf(StrRelayAddr[0], "  �о����� ");
			(*pUiCnt) = 0;
			TextBoxCreate(&pUiItems[(*pUiCnt)++], 0, 2*16, "�� ��:", StrDstAddr, 12, 13*8);
			TextBoxCreate(&pUiItems[(*pUiCnt)++], 0, 3*16, "�м�1:", StrRelayAddr[0], 12, 13*8);
			TextBoxCreate(&pUiItems[(*pUiCnt)++], 0, 4*16, "�м�2:", StrRelayAddr[1], 12, 13*8);
			TextBoxCreate(&pUiItems[(*pUiCnt)++], 0, 5*16, "�м�3:", StrRelayAddr[2], 12, 13*8);

			// �����������
			CurrCmd = (0x10 + menuItemNo);
			switch(CurrCmd){
			case WaterCmd_ReadRealTimeData:		// "��ȡ�û�����"
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			case WaterCmd_ReadFrozenData:		// "  "
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			case WaterCmd_OpenValve:			// "  "
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;
			
			case WaterCmd_OpenValveForce:		// "  ";
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

            case WaterCmd_CloseValve:		// "  ";
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			case WaterCmd_CloseValveForce:		// "  ";
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			case WaterCmd_ClearException:		// "  ";
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUiItem))){
					break;
				}
				Args.itemCnt = 2;
				Args.items[0] = &Args.buf[0];   //������
				Args.items[1] = &Args.buf[1];
                *Args.items[0] = 0x01;
				*Args.items[1] = 0x00;
				Args.lastItemLen = 1;
				break;

			default: 
				break;
			}


			if (key == KEY_CANCEL){
				break;
			}

			if(StrDstAddr[0] == 0x00 ){
				sprintf(StrDstAddr, "����������");
				continue;
			}

			// 6009 Э���ַ��д���÷���
			GetBytesFromStringHex(DstAddr, 0, 6, StrDstAddr, 0, false);
			PrintfXyMultiLine_VaList(0, 2*16, "�� ��: %s", StrDstAddr);

			// ����ַ
			Addrs.itemCnt = 0;
			Addrs.items[Addrs.itemCnt] = &Addrs.buf[0];
			memcpy(Addrs.items[Addrs.itemCnt], LocalAddr, 6);
			Addrs.itemCnt++;
			for(i = 0; i < RELAY_MAX; i++){
				if(StrRelayAddr[i][0] != 0x00){
					Addrs.items[Addrs.itemCnt] = &Addrs.buf[6 + i*6];
					GetBytesFromStringHex(DstAddr, 0, 6, StrRelayAddr[i], 0, false);
					Addrs.itemCnt++;
				}
			}
			Addrs.items[Addrs.itemCnt] = &Addrs.buf[6 + i*6];
			memcpy(Addrs.items[Addrs.itemCnt], DstAddr, 6);
			Addrs.itemCnt++;

			timeout = 3500 + (Addrs.itemCnt - 2) * 3500 * 2;
			
			// ���͡����ա������ʾ
			Protol6009Tranceiver(CurrCmd, &Addrs, &Args, ackLen, timeout, tryCnt);
			
			// ���� / ����
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
	MainMenu.no=2;
	MainMenu.title= "   ɣ���ֳֻ�  ";
	MainMenu.str[0]=" ������ͨ�� ";
	MainMenu.str[1]=" ˮ��ͨ�� ";
	//MainMenu.str[2]=" ���ͨ�� ";
	//MainMenu.str[3]=" ͸��ģ������ ";
	MainMenu.key[0]="1";
	MainMenu.key[1]="2";
	MainMenu.key[2]="3";
	MainMenu.key[3]="4";
	MainMenu.Function[0]=CenterCmdFunc;
	MainMenu.Function[1]=WaterCmdFunc;
	//MainMenu.Function[2]=PowerCmdFunc;
	//MainMenu.Function[3]=TransParentModuleFunc;
	MainMenu.FunctionEx=0;
	_OpenLcdBackLight();
	_Menu(&MainMenu);	
}

