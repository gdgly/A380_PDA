/*
*
*	ɣ��6009ˮ���ֳֻ� - �����
*
*/
#include "SR6009_IR.h"

#include "HJLIB.H"
#include "string.h"
#include "dbf.h"
#include "stdio.h"

#include "Common.h"
#include "Tool.h"
#include "WaterMeter.h"


// --------------------------------  ȫ�ֱ���  -----------------------------------------
char Screenbuff[160*(160/3+1)*2]; 
uint8 TmpBuf[1080];
uint8 TxBuf[1080];
uint8 RxBuf[1080];
uint8 DispBuf[2048];
uint32 RxLen, TxLen;
const uint8 LocalAddr[9] = { 0x20, 0x19, 0x00, 0x00, 0x20, 0x19, 0x00, 0x00, 0x00};	// ��ַ 2019000020190000��12/16�ַ�
uint8 DstAddr[9];
uint8 VerInfo[41];
uint16 CurrCmd;
ParamsBuf Addrs;		
ParamsBuf Args;
char StrBuf[10][TXTBUF_LEN];    // extend input buffer
char StrDstAddr[TXTBUF_LEN];
char StrRelayAddr[RELAY_MAX][TXTBUF_LEN];
UI_ItemList UiList;


//----------------------------------	�汾��Ϣ		--------------------------
void VersionInfoFunc(void)
{
	_ClearScreen();

	PrintfXyMultiLine_VaList(0, 0*16, "  %s ", VerInfo_Name);
	PrintfXyMultiLine_VaList(0, 1*16, "�� �� �ţ�%s", VerInfo_RevNo);
	PrintfXyMultiLine_VaList(0, 2*16, "�汾���ڣ�%s", VerInfo_RevDate);
	PrintfXyMultiLine_VaList(0, 3*16, "ͨ�ŷ�ʽ��%s", TransType);

	_ReadKey();		// ���������
}

//--------------------------------------	6009ˮ������ ���͡����ա������ʾ	----------------------------
/*
* ������ �����/���ս���		- ִ����ɺ󣬷��ؽ��
* ������ cmdid	- ��ǰ�����ʶ
*		addrs	- ��ַ��		
*		args	- ���������args->items[0] - ����ID, args->items[1] - ������
*		ackLen	- Ӧ�𳤶� (byte)
*		timeout	- ��ʱʱ�� (ms)  Ĭ��Ϊ 8s + �м��� x 2 x 6s
*		tryCnt	- ���Դ��� Ĭ��3��
* ���أ� bool  - ����ִ�н���� true - �ɹ��� false - ʧ��		
*/
bool Protol6009Tranceiver(uint8 cmdid, ParamsBuf *addrs, ParamsBuf *args, uint16 ackLen, uint16 timeout, uint8 tryCnt)
{
	uint8 sendCnt = 0, cmdResult, ret;
	uint16 waitTime = 0, lastRxLen;
	int fp;

	if(_Access("system.cfg", 0) < 0){
		fp = _Fopen("system.cfg", "W");
	}else{
		fp = _Fopen("system.cfg", "RW");
	}
	if((args->buf[0] >= 0x40 && args->buf[0] <= 0x66) 
		|| (args->buf[0] >= 0xF1 && args->buf[0] <= 0xF3)){
		_Lseek(fp, 20, 0);	// ��������
	}else{
		#ifdef Project_6009_RF
		_Lseek(fp, 0, 0);	// byte [0 ~ 19] 12λ��� 
		#else
		_Lseek(fp, 40, 0);	// byte [40 ~ 59] 16λ��� 
		#endif
	}
	_Fwrite(StrDstAddr, TXTBUF_LEN, fp);
	_Fclose(fp);
	
	_GUIRectangleFill(0, 1*16 + 8, 160, 8*16 + 8, Color_White);
#if (AddrLen == 6)
	PrintfXyMultiLine_VaList(0, 1*16 + 8, "���: %s ", StrDstAddr);
#else
	PrintfXyMultiLine_VaList(0, 1*16 + 8, "���:\n   %s ", StrDstAddr);
#endif

	do{
		_CloseCom();
		_ComSetTran(CurrPort);
		_ComSet(CurrBaud, 2);

		// ���� 
		TxLen = PackWater6009RequestFrame(TxBuf, addrs, cmdid, args, sendCnt);
		_GetComStr(TmpBuf, 1000, 100/10);	// clear , 100ms timeout
		_SendComStr(TxBuf, TxLen);
		sendCnt++;
		if(sendCnt == 1){
			//------------------------------------------------------
			_GUIHLine(0, 9*16 - 4, 160, Color_Black);
			//_Printfxy(0, 9*16, "״̬: �����      ", Color_White);
			_Printfxy(0, 9*16, " <  �����...  >  ", Color_White);
		}
		else{
			//------------------------------------------------------
			_GUIHLine(0, 9*16 - 4, 160, Color_Black);
			//PrintfXyMultiLine_VaList(0, 9*16, "״̬: �����ط� %d    ", sendCnt);
			PrintfXyMultiLine_VaList(0, 9*16, " <  �����ط�...%d  > ", sendCnt);
		}

		// ����
		_GetComStr(TmpBuf, 1000, 100/10);	// clear , 100ms timeout
		RxLen = 0;
		waitTime = 0;
		lastRxLen = 0;
		_DoubleToStr(TmpBuf, (double)(timeout / 1000), 1);
		PrintfXyMultiLine_VaList(0, 5*16, "�ȴ�Ӧ�� %d/%d \n���ȴ� %s s  ", RxLen, ackLen, TmpBuf);

		do{

			RxLen += _GetComStr(&RxBuf[lastRxLen], 100, 8);	// N x10 ms ������, ʱ��У׼Ϊ N x90% x10
			if(KEY_CANCEL == _GetKeyExt()){
				//------------------------------------------------------
				_GUIHLine(0, 9*16 - 4, 160, Color_Black);
				_Printfxy(0, 6*16, "������ȡ��         ", Color_White);
				_Printfxy(0, 9*16, "����  <��ȡ��>  ����", Color_White);
				DispBuf[0] = 0x00;
				return false;
			}
			waitTime += 200;

			if(waitTime % 1000 == 0){
				_DoubleToStr(TmpBuf, (double)((timeout - waitTime) / 1000), 1);
				PrintfXyMultiLine_VaList(0, 5*16, "�ȴ�Ӧ�� %d/%d \n���ȴ� %s s  ", RxLen, ackLen, TmpBuf);
			}

			if(lastRxLen > 0){
				if(lastRxLen != RxLen){
					lastRxLen = RxLen;
					continue;
				}else{
					break;
				}
			}else{
				lastRxLen = RxLen;
			}
		}while(waitTime < timeout);

		PrintfXyMultiLine_VaList(0, 5*16, "��ǰӦ�� %d/%d \n", RxLen, ackLen);

#if Log_On
		LogPrintBytes("Tx: ", TxBuf, TxLen);
		LogPrintBytes("Rx: ", RxBuf, RxLen);
#endif

		cmdResult = ExplainWater6009ResponseFrame(RxBuf, RxLen, LocalAddr, cmdid, ackLen, DispBuf);

	}while(sendCnt < tryCnt && (cmdResult == RxResult_Timeout || cmdResult == RxResult_CrcError));

	// ��ʾ���
#if RxBeep_On
	_SoundOn();
	_Sleep(50);
	_SoundOff();
#endif
	if(cmdResult == RxResult_Ok){
		_GUIRectangleFill(0, 1*16 + 8, 160, 8*16 + 8, Color_White);
		//------------------------------------------------------
		_GUIHLine(0, 9*16 - 4, 160, Color_Black);
		_Printfxy(0, 9*16, "����  < �ɹ� >  ����", Color_White);
		ret = true;
	}
	else{
#if RxBeep_On
		_Sleep(30);
		_SoundOn();
		_Sleep(30);
		_SoundOff();
#endif
		if(cmdResult == RxResult_Failed){
		//	_GUIRectangleFill(0, 1*16 + 8, 160, 8*16 + 8, Color_White);
		}
		//-----------------------------------------------------
		_GUIHLine(0, 9*16 - 4, 160, Color_Black);
		_Printfxy(0, 9*16, "����  < ʧ�� >  ����", Color_White);
		ret = false;
	}

	_CloseCom();

	return ret;
}

/*
* ������ �����/���ս���	- ִ����ɺ󣬵ȴ���������/�¼� - ������ʾ�� ȷ��/ȡ���� - ����
* ������ cmdid	- ��ǰ�����ʶ
*		addrs	- ��ַ��		
*		args	- ���������args->items[0] - ����ID, args->items[1] - ������
*		ackLen	- Ӧ�𳤶� (byte)
*		timeout	- ��ʱʱ�� (ms)  Ĭ��Ϊ 8s + �м��� x 2 x 6s
*		tryCnt	- ���Դ��� Ĭ��3��
* ���أ� uint8	- �����˳�ʱ�İ���ֵ��ȷ�ϼ���ȡ����	
*/
uint8 Protol6009TranceiverWaitUI(uint8 cmdid, ParamsBuf *addrs, ParamsBuf *args, uint16 ackLen, uint16 timeout, uint8 tryCnt)
{
	const uint8 lineStep = 7, lineMax = 7;
	int8 lineCnt = 0, currLine = 0;
	uint8 *lines[100], key;

	// Ӧ�𳤶ȡ���ʱʱ�䡢�ط�����
#ifdef Project_6009_IR
	timeout = 3000;
	tryCnt = 3;
#else
	timeout = 10000 + (Addrs.itemCnt - 2) * 6000 * 2;
	tryCnt = 3;
#endif

	if(false == Protol6009Tranceiver(cmdid, addrs, args, ackLen, timeout, tryCnt)){
		if(strncmp(DispBuf, "���", 4) != 0){	// ������ȡ��	
			DispBuf[0] = NULL;
			lines[0] = NULL;
		}
	}

	lineCnt = GetPrintLines(0, DispBuf, lines);
	PrintfXyMultiLine(0, 1*16 + 8, lines[0], lineMax);	

	// ��/�¹�����ʾ   �� ��  �� ��
	while(1){

		if(lineCnt > lineMax){
			if(currLine < lineCnt - lineMax){
				PrintXyTriangle(9*16 + 8, 8*16 + 8, 1);		// ��
			}else{
				_GUIRectangleFill(9*16 + 8, 8*16 + 8, 160, 8*16 + 12, Color_White);
			}

			if(currLine > 0){
				PrintXyTriangle(9*16 + 8, 1*16 + 4, 0);		// ��
			}else{
				_GUIRectangleFill(9*16 + 8, 1*16 + 5, 160, 1*16 + 8, Color_White);
			}
		}

		key = _ReadKey();

		if(key == KEY_CANCEL || key == KEY_ENTER){
			break;
		}
		else if(key == KEY_UP && lineCnt > lineMax){
			currLine -= lineStep;
			if(currLine < 0){
				currLine = 0;
			}
		}
		else if(key == KEY_DOWN && lineCnt > lineMax){
			currLine += lineStep;
			if(currLine > lineCnt - lineMax){
				currLine = lineCnt - lineMax;
			}
		}
		else{
			continue;
		}

		_GUIRectangleFill(0, 1*16 + 8, 160, 8*16 + 8, Color_White);
		PrintfXyMultiLine(0, 1*16 + 8, lines[currLine], lineMax);
	}

	return key;
}


// --------------------------------  ˮ��ģ��ͨ��  -----------------------------------------

// 1	��������
void WaterCmdFunc_CommonCmd(void)
{
	uint8 key, menuItemNo, tryCnt = 0, i;
	_GuiLisStruEx menuList;
	UI_Item * pUi = &UiList.items[0];
	uint8 * pUiCnt = &UiList.cnt;
	uint8 * pByte;
	uint8 currUi = 0, uiRowIdx, isUiFinish;
	uint16 ackLen = 0, timeout, u16Tmp;
	uint32 u32Tmp;

	_ClearScreen();

	// �˵�
	menuList.title = "<<��������";
	menuList.no = 6;
	menuList.MaxNum = 6;
	menuList.isRt = 0;
	menuList.x = 0;
	menuList.y = 0;
	menuList.with = 10 * 16;
	menuList.str[0] = "1. ���ۼ�����";
	menuList.str[1] = "2. ����������";
	menuList.str[2] = "3. ������������ϵ��";
	menuList.str[3] = "4. ����";
	menuList.str[4] = "5. �ط�";
	menuList.str[5] = "6. ���쳣����";
	menuList.defbar = 1;

	while(1){

		_ClearScreen();
		menuItemNo = _ListEx(&menuList);

		if (menuItemNo == 0){
			break;
		}
		menuList.defbar = menuItemNo;
		memset(StrBuf, 0, TXTBUF_LEN * 10);
		isUiFinish = false;

		while(1){
			
			_ClearScreen();

			// �������� :  ������ʾ
			pByte = menuList.str[menuItemNo - 1];
			sprintf(TmpBuf, "<<%s",&pByte[3]);
			_Printfxy(0, 0, TmpBuf, Color_White);
			_GUIHLine(0, 1*16 + 4, 160, Color_Black);	
			/*---------------------------------------------*/
		 	//----------------------------------------------
			_GUIHLine(0, 9*16 - 4, 160, Color_Black);
			_Printfxy(0, 9*16, "���� <�ȴ�����> ִ��", Color_White);

			if(false == isUiFinish){
				(*pUiCnt) = 0;
				uiRowIdx = 2;
				#if (AddrLen == 6)
				TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�� ��:", StrDstAddr, 12, 13*8, true);
				#else
				LableCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�� ��:");
				TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "   ", StrDstAddr, 16, 17*8, true);	
                #endif
			}

			// �����������
			i = 0;	
			Args.itemCnt = 2;
			Args.items[0] = &Args.buf[0];   // ������
			Args.items[1] = &Args.buf[1];	// ������

			switch(menuItemNo){
			case 1: 
				CurrCmd = WaterCmd_ReadRealTimeData;		// "��ȡ�û�����"
				/*---------------------------------------------*/
				if(false == isUiFinish){
					break;
				}
				Args.buf[i++] = 0x01;		// ������	01
				ackLen = 21;				// Ӧ�𳤶� 21	
				// ������
				Args.buf[i++] = 0x00;				// ���ݸ�ʽ 00	
				Args.lastItemLen = i - 1;
				break;

			case 2: 
				CurrCmd = WaterCmd_ReadFrozenData;		// "��ȡ������ת����"
				/*---------------------------------------------*/
				if(false == isUiFinish){
					sprintf(StrBuf[0], "0 (0-9��Ч)");
					TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�� ��:", StrBuf[0], 1, 2*8, true);
					break;
				}
				
				if(StrBuf[0][0] > '9' || StrBuf[0][0] < '0'){
					currUi = uiRowIdx - 2 - 1;
					isUiFinish = false;
					continue;
				}
				Args.buf[i++] = 0x02;		// ������	02
				ackLen = 114;				// Ӧ�𳤶� 88/114	
				// ������
				Args.buf[i++] = 0x01;				// ���ݸ�ʽ 01/02
				Args.buf[i++] = _GetYear()/100;		// ʱ�� - yyyy/mm/dd HH:mm:ss
				Args.buf[i++] = _GetYear()%100;		
				Args.buf[i++] = _GetMonth();		
				Args.buf[i++] = _GetDay();			
				Args.buf[i++] = _GetHour();			
				Args.buf[i++] = _GetMin();			
				Args.buf[i++] = _GetSec();			
				Args.buf[i++] = StrBuf[0][0] - '0';	// �����������	
				Args.lastItemLen = i - 1;
				break;

			case 3: 
				CurrCmd = WaterCmd_SetBaseValPulseRatio;	// ����������ϵ��
				/*---------------------------------------------*/
				if(false == isUiFinish){
					if(StrBuf[1][0] == 0x00){
						StrBuf[1][0] = 0x01;
					}
					TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�û�����:", StrBuf[0], 10, 11*8, true);
					pUi[(*pUiCnt) -1].ui.txtbox.dotEnable = 1;
                    CombBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "����ϵ��:", &StrBuf[1][0], 4, 
						"1", "10", "100", "1000");
					break;
				}
				if(StrBuf[0][0] > '9' || StrBuf[0][0] < '0'){
					sprintf(StrBuf[0], " ������");
					currUi = uiRowIdx - 2 - 2;
					isUiFinish = false;
					continue;
				}

				Args.buf[i++] = 0x06;		// ������	06
				ackLen = 7;					// Ӧ�𳤶� 7	
				// ������
				u32Tmp = (uint32) _atof(StrBuf[0]);
				u16Tmp = (uint16)((float)((_atof(StrBuf[0]) - u32Tmp)*1000.0));
				Args.buf[i++] = (uint8)(u32Tmp & 0xFF);		// �û�����	
				Args.buf[i++] = (uint8)((u32Tmp >> 8) & 0xFF);
				Args.buf[i++] = (uint8)((u32Tmp >> 16) & 0xFF);
				Args.buf[i++] = (uint8)((u32Tmp >> 24) & 0xFF);
				Args.buf[i++] = (uint8)(u16Tmp & 0xFF);		
				Args.buf[i++] = (uint8)((u16Tmp >> 8) & 0xFF);
				Args.buf[i++] = (uint8)StrBuf[1][0];		// ����ϵ��	
				Args.lastItemLen = i - 1;
				break;

			case 4: 
				CurrCmd = WaterCmd_OpenValve;			// " ���� "
				/*---------------------------------------------*/
				if(false == isUiFinish){
					break;
				}
				Args.buf[i++] = 0x03;		// ������	03
				ackLen = 3;					// Ӧ�𳤶� 3	
				// ������
				Args.buf[i++] = 0x00;		// ǿ�Ʊ�ʶ 	0 - ��ǿ�ƣ� 1 - ǿ��
				Args.buf[i++] = 0x01;		// ���ط���ʶ	0 - �ط��� 1 - ����
				Args.lastItemLen = i - 1;
				break;

            case 5: 
				CurrCmd = WaterCmd_CloseValve;			// " �ط� ";
				/*---------------------------------------------*/
				if(false == isUiFinish){
					break;
				}
				Args.buf[i++] = 0x03;		// ������	03
				ackLen = 3;					// Ӧ�𳤶� 3	
				// ������
				Args.buf[i++] = 0x00;		// ǿ�Ʊ�ʶ 	0 - ��ǿ�ƣ� 1 - ǿ��
				Args.buf[i++] = 0x00;		// ���ط���ʶ	0 - �ط��� 1 - ����
				Args.lastItemLen = i - 1;
				break;

			case 6: 
				CurrCmd = WaterCmd_ClearException;		// " ���쳣���� ";
				/*---------------------------------------------*/
				if(false == isUiFinish){
					break;
				}
				Args.buf[i++] = 0x05;		// ������	05
				ackLen = 1;					// Ӧ�𳤶� 1	
				// ������
				Args.buf[i++] = 0x00;		// ����ѡ�� 00	
				Args.lastItemLen = i - 1;
				break;

			default: 
				break;
			}

			// ���� ���м̵�ַ����� �� ��ʾUI
			if(false == isUiFinish){
			
				key = ShowUI(UiList, &currUi);

				if (key == KEY_CANCEL){
					break;
				}

				if(StrDstAddr[0] < '0' || StrDstAddr[0] > '9' ){
					sprintf(StrDstAddr, " ������");
					currUi = 0;
					continue;
				}

				isUiFinish = true;
				continue;	// go back to get ui args
			}

			// ��ַ���
			Water6009_PackAddrs(&Addrs, StrDstAddr, StrRelayAddr);
			#if (AddrLen == 6)
			PrintfXyMultiLine_VaList(0, 2*16, "�� ��: %s", StrDstAddr);
			#else
			PrintfXyMultiLine_VaList(0, 2*16, "�� ��:\n   %s", StrDstAddr);
			#endif

			// Ӧ�𳤶ȡ���ʱʱ�䡢�ط�����
			ackLen += 14 + Addrs.itemCnt * AddrLen;
			timeout = 8000 + (Addrs.itemCnt - 2) * 6000 * 2;
			tryCnt = 3;

			// ���͡����ա������ʾ
			key = Protol6009TranceiverWaitUI(CurrCmd, &Addrs, &Args, ackLen, timeout, tryCnt);
			
			
			// ���� / ����
			if (key == KEY_CANCEL){
				break;
			}else{
				isUiFinish = false;
				continue;
			}
		}
		
	}
}

// 2	��������
void WaterCmdFunc_TestCmd(void)
{
	uint8 key, menuItemNo, tryCnt = 0, i;
	_GuiLisStruEx menuList;
	UI_Item * pUi = &UiList.items[0];
	uint8 * pUiCnt = &UiList.cnt;
	uint8 * pByte;
	uint8 currUi = 0, uiRowIdx, isUiFinish;
	uint16 ackLen = 0, timeout, u16Tmp;
	uint32 u32Tmp;

	_ClearScreen();

	// �˵�
	menuList.title = "<<��������";
	menuList.no = 6;
	menuList.MaxNum = 6;
	menuList.isRt = 0;
	menuList.x = 0;
	menuList.y = 0;
	menuList.with = 10 * 16;
	menuList.str[0] = "  1. ��debug��Ϣ";
	menuList.str[1] = "  2. ��debug��Ϣ";
	menuList.str[2] = "  3. �������";
	menuList.str[3] = "  4. �����¶�";
	menuList.str[4] = "  5. �����ѹ";
	menuList.str[5] = "  6. ���÷��ز���";
	menuList.defbar = 1;

	while(1){

		_ClearScreen();
		menuItemNo = _ListEx(&menuList);

		if (menuItemNo == 0){
			break;
		}
		menuList.defbar = menuItemNo;
		memset(StrBuf, 0, TXTBUF_LEN * 10);
		isUiFinish = false;

		while(1){
			
			_ClearScreen();

			// �������� :  ������ʾ
			pByte = menuList.str[menuItemNo - 1];
			sprintf(TmpBuf, "<<%s",&pByte[5]);
			_Printfxy(0, 0, TmpBuf, Color_White);
			_GUIHLine(0, 1*16 + 4, 160, Color_Black);	
			/*---------------------------------------------*/
			//----------------------------------------------
			_GUIHLine(0, 9*16 - 4, 160, Color_Black);
			_Printfxy(0, 9*16, "���� <�ȴ�����> ִ��", Color_White);

			if(false == isUiFinish){
				(*pUiCnt) = 0;
				uiRowIdx = 2;
				#if (AddrLen == 6)
				TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�� ��:", StrDstAddr, 12, 13*8, true);
				#else
				LableCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�� ��:");
				TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "   ", StrDstAddr, 16, 17*8, true);	
				#endif
			}
			
			// �����������
			i = 0;	
			Args.itemCnt = 2;
			Args.items[0] = &Args.buf[0];   // ������
			Args.items[1] = &Args.buf[1];	// ������

			switch(menuItemNo){
			case 1: 
				CurrCmd = WaterCmd_ReadDebugInfo;		// ��debug��Ϣ
				/*---------------------------------------------*/
				if(false == isUiFinish){
					break;
				}
				Args.buf[i++] = 0x07;		// ������	07
				ackLen = 58;				// Ӧ�𳤶� 58	
				// ������
				Args.buf[i++] = 0x01;		// ����ѡ�� 01	
				Args.lastItemLen = i - 1;
				break;

			case 2: 
				CurrCmd = WaterCmd_ClearDebugInfo;		// ��debug��Ϣ
				/*---------------------------------------------*/
				if(false == isUiFinish){
					break;
				}
				Args.buf[i++] = 0x07;		// ������	07
				ackLen = 2;					// Ӧ�𳤶� 2	
				// ������
				Args.buf[i++] = 0x02;		// ����ѡ�� 05	
				Args.lastItemLen = i - 1;
				break;

			case 3: 
				CurrCmd = WaterCmd_RebootDevice;			// "�������"
				/*---------------------------------------------*/
				if(false == isUiFinish){
					break;
				}
				Args.buf[i++] = 0x07;		// ������	07
				ackLen = 2;					// Ӧ�𳤶� 2	
				// ������
				Args.buf[i++] = 0x04;		// ����ѡ�� 04	
				Args.lastItemLen = i - 1;
				break;

			case 4: 
				CurrCmd = WaterCmd_ReadTemperature;			// " �����¶� "
				/*---------------------------------------------*/
				if(false == isUiFinish){
					break;
				}
				Args.buf[i++] = 0x07;		// ������	07
				ackLen = 2;					// Ӧ�𳤶� 2	
				// ������
				Args.buf[i++] = 0x05;		// ����ѡ�� 05	
				Args.lastItemLen = i - 1;
				break;

			case 5: 
				CurrCmd = WaterCmd_ReadVoltage;				// " �����ѹ "
				/*---------------------------------------------*/
				if(false == isUiFinish){
					break;
				}
				Args.buf[i++] = 0x07;		// ������	07
				ackLen = 2;					// Ӧ�𳤶� 2	
				// ������
				Args.buf[i++] = 0x07;		// ����ѡ�� 07	
				Args.lastItemLen = i - 1;
				break;
			
            case 6: 
				CurrCmd = WaterCmd_SetOverCurrentTimeout;		// " ���÷��ز��� ";
				/*---------------------------------------------*/
				if(false == isUiFinish){
                    _Printfxy(7*16, (uiRowIdx)*16, "mA", Color_White);
					TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "��������:", StrBuf[0], 3, 5*8, true);
					_Printfxy(7*16, (uiRowIdx)*16, "ms", Color_White);
                    TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "��ʱʱ��:", StrBuf[1], 5, 5*8, true);
					break;
				}

				if(StrBuf[0][0] > '9' || StrBuf[0][0] < '0'){
					sprintf(StrBuf[0], "     ");
					currUi = uiRowIdx - 2 - 2;
					isUiFinish = false;
					continue;
				}
				if(StrBuf[1][0] > '9' || StrBuf[1][0] < '0'){
					sprintf(StrBuf[1], "     ");
					currUi = uiRowIdx - 2 - 1;
					isUiFinish = false;
					continue;
				}

				u16Tmp = (uint16)_atof(StrBuf[0]);
				u32Tmp = (uint32)_atof(StrBuf[1]);
				if(u16Tmp > 255){
					sprintf(StrBuf[0], " 0-255");
					currUi = uiRowIdx - 2 - 2;
					isUiFinish = false;
					continue;
				}
				if(u32Tmp > 65535){
					sprintf(StrBuf[1], " 0-65535");
					currUi = uiRowIdx - 2 - 1;
					isUiFinish = false;
					continue;
				}
				
				Args.buf[i++] = 0x07;		// ������	07
				ackLen = 4;					// Ӧ�𳤶� 4	
				// ������
				Args.buf[i++] = 0x09;		// ����ѡ�� 09
				Args.buf[i++] = (uint8)u16Tmp;			// �������� 0~255	
				Args.buf[i++] = (uint8)(u32Tmp & 0xFF);	// ��ʱʱ�� 0~65535	
				Args.buf[i++] = (uint8)(u32Tmp >> 8);
				Args.lastItemLen = i - 1;
				break;

			case WaterCmd_ReadReportRoute:		// " ���ϱ�·�� ";
				/*---------------------------------------------*/
				if(false == isUiFinish){
					break;
				}
				Args.buf[i++] = 0x07;		// ������	07
				ackLen = 63;				// Ӧ�𳤶� 63	
				// ������
				Args.buf[i++] = 0x0E;		// ����ѡ�� 0E	
				Args.lastItemLen = i - 1;
				break;
			
			case WaterCmd_SetMeterNumber:		// " ���ñ�� ";
				/*---------------------------------------------*/
				if(false == isUiFinish){
					#if (AddrLen == 6)
					TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�±��:", StrBuf[0], 12, 13*8, true);
					#else
					LableCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�±��:");
					TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "   ", StrBuf[0], 16, 17*8, true);	
					#endif
					break;
				}

				// �ȶ�ȡ����������Ϣ - ��ȡ�汾��
				if(StrDstAddr[0] < '0' || StrDstAddr[0] > '9'  ){
					sprintf(StrDstAddr, " ������");
					currUi = 0;
					isUiFinish = false;
					continue;
				}
				if(StrBuf[0][0] > '9' || StrBuf[0][0] < '0'){
					sprintf(StrBuf[0], " ������");
					currUi = 1;
					isUiFinish = false;
					continue;
				}
				
				Water6009_PackAddrs(&Addrs, StrDstAddr, StrRelayAddr);
				i = 0;
				Args.buf[i++] = 0x04;		// ������	04
				ackLen = 128;				// Ӧ�𳤶� 128	
				Args.lastItemLen = i - 1;

				// ����ȡ���汾�ţ������ñ��
				if(true == Protol6009Tranceiver(WaterCmd_ReadMeterCfgInfo, &Addrs, &Args, ackLen, timeout, tryCnt)){
					i = 0;
					Args.buf[i++] = 0x07;		// ������	07
					ackLen = 1;					// Ӧ�𳤶� 1	
					// ������
					Args.buf[i++] = 0x10;		// ����ѡ�� 10	
					memcpy(&Args.buf[i], &VerInfo[0], 40);	
					i += 40;					// ����汾��
					GetBytesFromStringHex(&Args.buf[i], 0, 6, StrBuf[0], 0, false);
					i += 6;						// �µ�ַ
					Args.lastItemLen = i - 1;

					if(true == Protol6009Tranceiver(CurrCmd, &Addrs, &Args, ackLen, timeout, tryCnt)){
						memcpy(StrDstAddr, StrBuf[0], TXTBUF_LEN);
					}
				}

				key = _ReadKey();

				// ���� / ����
				if (key == KEY_CANCEL){
					break;
				}else{
					isUiFinish = false;
					continue;
				}
				break;

			default: 
				break;
			}

			if (key == KEY_CANCEL){
				key = KEY_NOHOOK;
				break;
			}


			// ���� ���м̵�ַ����� �� ��ʾUI
			if(false == isUiFinish){
			
				key = ShowUI(UiList, &currUi);

				if (key == KEY_CANCEL){
					key = KEY_NOHOOK;
					break;
				}

				if(StrDstAddr[0] < '0' || StrDstAddr[0] > '9'  ){
					sprintf(StrDstAddr, " ������");
					currUi = 0;
					continue;
				}

				isUiFinish = true;
				continue;	// go back to get ui args
			}

			// ��ַ���
			Water6009_PackAddrs(&Addrs, StrDstAddr, StrRelayAddr);
			#if (AddrLen == 6)
			PrintfXyMultiLine_VaList(0, 2*16, "�� ��: %s", StrDstAddr);
			#else
			PrintfXyMultiLine_VaList(0, 2*16, "�� ��:\n   %s", StrDstAddr);
			#endif

			// Ӧ�𳤶ȡ���ʱʱ�䡢�ط�����
			ackLen += 14 + Addrs.itemCnt * AddrLen;
			timeout = 8000 + (Addrs.itemCnt - 2) * 6000 * 2;
			tryCnt = 3;

			// ���͡����ա������ʾ
			key = Protol6009TranceiverWaitUI(CurrCmd, &Addrs, &Args, ackLen, timeout, tryCnt);
			
			
			// ���� / ����
			if (key == KEY_CANCEL){
				key = KEY_NOHOOK;
				break;
			}else{
				isUiFinish = false;
				continue;
			}
		}
		
	}
}

// 3	��������
void WaterCmdFunc_Upgrade(void)
{
	uint8 key, menuItemNo, tryCnt = 0, i;
	_GuiLisStruEx menuList;
	UI_Item * pUi = &UiList.items[0];
	uint8 * pUiCnt = &UiList.cnt;
	uint8 * pByte;
	uint8 currUi = 0, uiRowIdx, isUiFinish;
	uint16 ackLen = 0, timeout;

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
	_ComSetTran(CurrPort);
	_ComSet(CurrBaud, 2);

	while(1){

		_ClearScreen();
		menuItemNo = _ListEx(&menuList);

		if (menuItemNo == 0){
			break;
		}
		menuList.defbar = menuItemNo;
		memset(StrBuf, 0, TXTBUF_LEN * 10);
		isUiFinish = false;

		while(1){
			
			_ClearScreen();

			// �������� :  ������ʾ
			pByte = menuList.str[menuItemNo - 1];
			sprintf(TmpBuf, "<<%s",&pByte[5]);
			_Printfxy(0, 0, TmpBuf, Color_White);
			_GUIHLine(0, 1*16 + 4, 160, Color_Black);	
			/*---------------------------------------------*/
			//----------------------------------------------
			_GUIHLine(0, 9*16 - 4, 160, Color_Black);
			_Printfxy(0, 9*16, "���� <�ȴ�����> ִ��", Color_White);

			if(false == isUiFinish){
				(*pUiCnt) = 0;
				uiRowIdx = 2;
				#if (AddrLen == 6)
				TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�� ��:", StrDstAddr, 12, 13*8, true);
				#else
				LableCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�� ��:");
				TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "   ", StrDstAddr, 16, 17*8, true);	
				#endif
			}
			
			// �����������
			i = 0;	
			Args.itemCnt = 2;
			Args.items[0] = &Args.buf[0];   // ������
			Args.items[1] = &Args.buf[1];	// ������
			CurrCmd = (0x30 + menuItemNo);

			switch(CurrCmd){
			case WaterCmd_ReadRealTimeData:		// "��ȡ�û�����"
				/*---------------------------------------------*/
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUi))){
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
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUi))){
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
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUi))){
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
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUi))){
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
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUi))){
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
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUi))){
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
				if(KEY_CANCEL == (key = ShowUI(UiList, &currUi))){
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


			// ���� ���м̵�ַ����� �� ��ʾUI
			if(false == isUiFinish){
		
				key = ShowUI(UiList, &currUi);

				if (key == KEY_CANCEL){
					break;
				}

				if(StrDstAddr[0] < '0' || StrDstAddr[0] > '9'  ){
					sprintf(StrDstAddr, " ������");
					currUi = 0;
					continue;
				}

				isUiFinish = true;
				continue;	// go back to get ui args
			}

			// ��ַ���
			Water6009_PackAddrs(&Addrs, StrDstAddr, StrRelayAddr);
			#if (AddrLen == 6)
			PrintfXyMultiLine_VaList(0, 2*16, "�� ��: %s", StrDstAddr);
			#else
			PrintfXyMultiLine_VaList(0, 2*16, "�� ��:\n   %s", StrDstAddr);
			#endif

			// Ӧ�𳤶ȡ���ʱʱ�䡢�ط�����
			ackLen += 14 + Addrs.itemCnt * AddrLen;
			timeout = 8000 + (Addrs.itemCnt - 2) * 6000 * 2;
			tryCnt = 3;

			// ���͡����ա������ʾ
			key = Protol6009TranceiverWaitUI(CurrCmd, &Addrs, &Args, ackLen, timeout, tryCnt);
			
			
			// ���� / ����
			if (key == KEY_CANCEL){
				break;
			}else{
				isUiFinish = false;
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
	UI_Item * pUi = &UiList.items[0];
	uint8 * pUiCnt = &UiList.cnt;
	uint8 * pByte;
	uint8 currUi = 0, uiRowIdx, isUiFinish;
	uint16 ackLen = 0, timeout, u16Tmp;
	uint32 u32Tmp;

	_ClearScreen();

	// �˵�`
	menuList.title = "<<Ԥ������";
	menuList.no = 6;
	menuList.MaxNum = 6;
	menuList.isRt = 0;
	menuList.x = 0;
	menuList.y = 0;
	menuList.with = 10 * 16;
	menuList.str[0] = "  1. ��Ԥ�ɲο�����";
	menuList.str[1] = "  2. ��Ԥ�ɲο�����";
	menuList.str[2] = "  3. �������ط���ֵ";
	menuList.str[3] = "  4. �豨����ֵ";
	menuList.str[4] = "  5. ��ط���ֵ";
	menuList.str[5] = "  6. �豨���ط���ֵ";
	menuList.defbar = 1;

	_CloseCom();
	_ComSetTran(CurrPort);
	_ComSet(CurrBaud, 2);

	while(1){

		_ClearScreen();
		menuItemNo = _ListEx(&menuList);

		if (menuItemNo == 0){
			break;
		}
		menuList.defbar = menuItemNo;
		memset(StrBuf, 0, TXTBUF_LEN * 10);	
		isUiFinish = false;

		while(1){
			
			_ClearScreen();

			// �������� :  ������ʾ
			pByte = menuList.str[menuItemNo - 1];
			sprintf(TmpBuf, "<<%s",&pByte[5]);
			_Printfxy(0, 0, TmpBuf, Color_White);
			_GUIHLine(0, 1*16 + 4, 160, Color_Black);	
			//----------------------------------------------
			_GUIHLine(0, 9*16 - 4, 160, Color_Black);
			_Printfxy(0, 9*16, "���� <�ȴ�����> ִ��", Color_White);

			if(false == isUiFinish){
				(*pUiCnt) = 0;
				uiRowIdx = 2;
				#if (AddrLen == 6)
				TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�� ��:", StrDstAddr, 12, 13*8, true);
				#else
				LableCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�� ��:");
				TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "   ", StrDstAddr, 16, 17*8, true);	
				#endif
			}
			
			// �����������
			i = 0;	
			Args.itemCnt = 2;
			Args.items[0] = &Args.buf[0];   // ������
			Args.items[1] = &Args.buf[1];	// ������
			CurrCmd = (0x40 + menuItemNo);

			switch(CurrCmd){
			case WaterCmd_ReadPrepaidRefVal:		// "��Ԥ�ɲο�����"
				/*---------------------------------------------*/
				if(false == isUiFinish){
					break;
				}
				Args.buf[i++] = 0x15;		// ������	15
				ackLen = 12;				// Ӧ�𳤶� 12	
				// ������
				Args.lastItemLen = i - 1;
				break;

			case WaterCmd_SetPrepaidRefVal:		// " ��Ԥ�ɲο����� "
				/*---------------------------------------------*/
				if(false == isUiFinish){
					TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "Ԥ������:", StrBuf[0], 10, 11*8, true);
					pUi[(*pUiCnt) -1].ui.txtbox.dotEnable = 1;
                    TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�ο�����:", StrBuf[1], 10, 11*8, true);
					pUi[(*pUiCnt) -1].ui.txtbox.dotEnable = 1;
					break;
				}
				if(StrBuf[0][0] > '9' || StrBuf[0][0] < '0'){
					sprintf(StrBuf[0], " ������");
					currUi = uiRowIdx - 2 - 2;
					isUiFinish = false;
					continue;
				}
				if(StrBuf[1][0] > '9' || StrBuf[1][0] < '0'){
					sprintf(StrBuf[1], " ������");
					currUi = uiRowIdx - 2 - 1;
					isUiFinish = false;
					continue;
				}

				Args.buf[i++] = 0x16;		// ������	16
				ackLen = 2;					// Ӧ�𳤶� 2	
				// ������
				u32Tmp = (uint32)_atof(StrBuf[0]);
				u16Tmp = (uint16)((float)((_atof(StrBuf[0]) - u32Tmp)*1000.0));
				Args.buf[i++] = (uint8)(u32Tmp & 0xFF);		// Ԥ������	
				Args.buf[i++] = (uint8)((u32Tmp >> 8) & 0xFF);
				Args.buf[i++] = (uint8)((u32Tmp >> 16) & 0xFF);
				Args.buf[i++] = (uint8)((u32Tmp >> 24) & 0xFF);
				Args.buf[i++] = (uint8)(u16Tmp & 0xFF);		
				Args.buf[i++] = (uint8)((u16Tmp >> 8) & 0xFF);
				u32Tmp = (uint32)_atof(StrBuf[1]);
				u16Tmp = (uint16)((float)((_atof(StrBuf[1]) - u32Tmp)*1000.0));
				Args.buf[i++] = (uint8)(u32Tmp & 0xFF);		// �ο���ʼ����	
				Args.buf[i++] = (uint8)((u32Tmp >> 8) & 0xFF);
				Args.buf[i++] = (uint8)((u32Tmp >> 16) & 0xFF);
				Args.buf[i++] = (uint8)((u32Tmp >> 24) & 0xFF);
				Args.buf[i++] = (uint8)(u16Tmp & 0xFF);		
				Args.buf[i++] = (uint8)((u16Tmp >> 8) & 0xFF);
				Args.lastItemLen = i - 1;
				break;

			case WaterCmd_ReadAlarmLimitOverdraft:	// " ��������ֵ͸֧ "
				/*---------------------------------------------*/
				if(false == isUiFinish){
					break;
				}
				Args.buf[i++] = 0x17;		// ������	17
				ackLen = 4;					// Ӧ�𳤶� 4	
				break;
			
			case WaterCmd_SetAlarmLimit:		// " �豨����ֵ ";
				/*---------------------------------------------*/
				if(false == isUiFinish){
					TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "������ֵ:", StrBuf[0], 3, 11*8, true);
					break;
				}
				if(StrBuf[0][0] > '9' || StrBuf[0][0] < '0'){
					sprintf(StrBuf[0], " ������");
					currUi = uiRowIdx - 2 - 1;
					isUiFinish = false;
					continue;
				}

				u16Tmp = (uint16)_atof(StrBuf[0]);
				if(u16Tmp > 255){
					sprintf(StrBuf[0], " 0-255");
					currUi = uiRowIdx - 2 - 1;
                    isUiFinish = false;
					continue;
				}
				Args.buf[i++] = 0x18;		// ������	18
				ackLen = 4;					// Ӧ�𳤶� 4	
				// ������
				Args.buf[i++] = 0x00;		// ����ѡ�0- ������ֵ 1-�ط���ֵ  2-������ֵ+�ط���ֵ	
				Args.buf[i++] = (uint8)u16Tmp;		// ������ֵ
				Args.lastItemLen = i - 1;
				break;

            case WaterCmd_SetCloseValveLimit:		// " ��ط���ֵ ";
				/*---------------------------------------------*/
				if(false == isUiFinish){
					TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�ط���ֵ:", StrBuf[0], 5, 11*8, true);
					break;
				}
				if(StrBuf[0][0] == 0x00){
					sprintf(StrBuf[0], " ������");
					currUi = uiRowIdx - 2 - 1;
					isUiFinish = false;
					continue;
				}
				u32Tmp = (uint32)_atof(StrBuf[0]);
				if(u32Tmp > 65535){
					sprintf(StrBuf[0], " 0-65535");
					currUi = uiRowIdx - 2 - 1;
					isUiFinish = false;
					continue;
				}
				Args.buf[i++] = 0x18;		// ������	18
				ackLen = 4;					// Ӧ�𳤶� 4	
				// ������
				Args.buf[i++] = 0x01;		// ����ѡ�0- ������ֵ 1-�ط���ֵ  2-������ֵ+�ط���ֵ	
				Args.buf[i++] = (uint8)(u32Tmp & 0xFF);	// �ط���ֵ
				Args.buf[i++] = (uint8)(u32Tmp >> 8);
				Args.lastItemLen = i - 1;
				break;

			case WaterCmd_SetAlarmAndCloseValveLimit:		// " �豨���ط���ֵ ";
				/*---------------------------------------------*/
				if(false == isUiFinish){
					TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "������ֵ:", StrBuf[0], 3, 11*8, true);
					TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�ط���ֵ:", StrBuf[1], 5, 11*8, true);
					break;
				}
				if(StrBuf[0][0] > '9' || StrBuf[0][0] < '0'){
					sprintf(StrBuf[0], " ������");
					currUi = uiRowIdx - 2 - 2;
					isUiFinish = false;
					continue;
				}
				if(StrBuf[1][0] > '9' || StrBuf[1][0] < '0'){
					sprintf(StrBuf[1], " ������");
					currUi = uiRowIdx - 2 - 1;
					isUiFinish = false;
					continue;
				}
				u16Tmp = (uint16) _atof(StrBuf[0]);
				if(u16Tmp > 255){
					sprintf(StrBuf[0], " 0-255");
					currUi = uiRowIdx - 2 - 2;
					isUiFinish = false;
					continue;
				}
				u32Tmp = (uint32) _atof(StrBuf[1]);
				if(u32Tmp > 65535){
					sprintf(StrBuf[1], " 0-65535");
					currUi = uiRowIdx - 2 - 1;
					isUiFinish = false;
					continue;
				}
				Args.buf[i++] = 0x18;		// ������	18
				ackLen = 4;					// Ӧ�𳤶� 4	
				// ������
				Args.buf[i++] = 0x02;		// ����ѡ�0- ������ֵ 1-�ط���ֵ  2-������ֵ+�ط���ֵ	
				Args.buf[i++] = (uint8)u16Tmp;				// ������ֵ
				Args.buf[i++] = (uint8)(u32Tmp & 0xFF);	// �ط���ֵ
				Args.buf[i++] = (uint8)(u32Tmp >> 8);
				Args.lastItemLen = i - 1;
				break;

			default: 
				break;
			}


			// ���� ���м̵�ַ����� �� ��ʾUI
			if(false == isUiFinish){
			
				key = ShowUI(UiList, &currUi);

				if (key == KEY_CANCEL){
					break;
				}

				if(StrDstAddr[0] < '0' || StrDstAddr[0] > '9'  ){
					sprintf(StrDstAddr, " ������");
					currUi = 0;
					continue;
				}

				isUiFinish = true;
				continue;	// go back to get ui args
			}

			// ��ַ���
			Water6009_PackAddrs(&Addrs, StrDstAddr, StrRelayAddr);
			#if (AddrLen == 6)
			PrintfXyMultiLine_VaList(0, 2*16, "�� ��: %s", StrDstAddr);
			#else
			PrintfXyMultiLine_VaList(0, 2*16, "�� ��:\n   %s", StrDstAddr);
			#endif

			// Ӧ�𳤶ȡ���ʱʱ�䡢�ط�����
			ackLen += 14 + Addrs.itemCnt * AddrLen;
			timeout = 8000 + (Addrs.itemCnt - 2) * 6000 * 2;
			tryCnt = 3;

			// ���͡����ա������ʾ
			key = Protol6009TranceiverWaitUI(CurrCmd, &Addrs, &Args, ackLen, timeout, tryCnt);
			
			
			// ���� / ����
			if (key == KEY_CANCEL){
				break;
			}else{
				isUiFinish = false;
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
	UI_Item * pUi = &UiList.items[0];
	uint8 * pUiCnt = &UiList.cnt;
	uint8 * pByte;
	uint8 currUi = 0, uiRowIdx, isUiFinish;
	uint16 ackLen = 0, timeout, u16Tmp;
	uint32 u32Tmp;

	_ClearScreen();

	// �˵�
	menuList.title = "<<��������";
	menuList.no = 12;
	menuList.MaxNum = 8;
	menuList.isRt = 0;
	menuList.x = 0;
	menuList.y = 0;
	menuList.with = 10 * 16;
	menuList.str[0] = "1. ��IP+�˿�+ģʽ";
	menuList.str[1] = "2. ��IP+�˿�+ģʽ";
	menuList.str[2] = "3. ����Ӫ�̱��";
	menuList.str[3] = "4. ��IMEI+CCID";
	menuList.str[4] = "5. �����ת��������";
	menuList.str[5] = "6. ��ȡ����ʹ��״̬";
	menuList.str[6] = "7. ���ö�ʱ�ϴ�";
	menuList.str[7] = "8. ���ö����ϴ�";
	menuList.str[8] = "9. ���ö�ʱ�����ϴ�";
	menuList.str[9] = "10.�����ʱ��";
	menuList.str[10] = "11.У���ʱ��";
	menuList.str[11] = "12.���շ����ŷ�����";
	menuList.defbar = 1;

	_CloseCom();
	_ComSetTran(CurrPort);
	_ComSet(CurrBaud, 2);

	while(1){

		_ClearScreen();
		menuItemNo = _ListEx(&menuList);

		if (menuItemNo == 0){
			break;
		}
		menuList.defbar = menuItemNo;
		memset(StrBuf, 0, TXTBUF_LEN * 10);
		isUiFinish = false;

		while(1){
			
			_ClearScreen();

			// �������� :  ������ʾ
			pByte = menuList.str[menuItemNo - 1];
			sprintf(TmpBuf, "<<%s",&pByte[3]);
			_Printfxy(0, 0, TmpBuf, Color_White);
			_GUIHLine(0, 1*16 + 4, 160, Color_Black);	
			//----------------------------------------------
			_GUIHLine(0, 9*16 - 4, 160, Color_Black);
			_Printfxy(0, 9*16, "���� <�ȴ�����> ִ��", Color_White);

			if(false == isUiFinish){
				(*pUiCnt) = 0;
				uiRowIdx = 2;
				#if (AddrLen == 6)
				TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�� ��:", StrDstAddr, 12, 13*8, true);
				#else
				LableCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�� ��:");
				TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "   ", StrDstAddr, 16, 17*8, true);	
				#endif
			}
			
			// �����������
			i = 0;	
			Args.itemCnt = 2;
			Args.items[0] = &Args.buf[0];   // ������
			Args.items[1] = &Args.buf[1];	// ������

			switch(menuItemNo){
			case 1: 
				CurrCmd = WaterCmd_SetIpPortMode;			// ��IP+�˿�+ģʽ
				/*---------------------------------------------*/
				if(false == isUiFinish){
					if(StrBuf[1][0] == 0x00){
						StrBuf[1][0] = 0x01;
					}
					TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�û�����:", StrBuf[0], 10, 11*8, true);
					pUi[(*pUiCnt) -1].ui.txtbox.dotEnable = 1;
                    CombBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "����ϵ��:", &StrBuf[1][0], 4, 
						"1", "10", "100", "1000");
					break;
				}
				if(StrBuf[0][0] > '9' || StrBuf[0][0] < '0'){
					sprintf(StrBuf[0], " ������");
					currUi = uiRowIdx - 2 - 2;
					isUiFinish = false;
					continue;
				}

				Args.buf[i++] = 0x06;		// ������	06
				ackLen = 7;					// Ӧ�𳤶� 7	
				// ������
				u32Tmp = (uint32) _atof(StrBuf[0]);
				u16Tmp = (uint16)((float)((_atof(StrBuf[0]) - u32Tmp)*1000.0));
				Args.buf[i++] = (uint8)(u32Tmp & 0xFF);		// �û�����	
				Args.buf[i++] = (uint8)((u32Tmp >> 8) & 0xFF);
				Args.buf[i++] = (uint8)((u32Tmp >> 16) & 0xFF);
				Args.buf[i++] = (uint8)((u32Tmp >> 24) & 0xFF);
				Args.buf[i++] = (uint8)(u16Tmp & 0xFF);		
				Args.buf[i++] = (uint8)((u16Tmp >> 8) & 0xFF);
				Args.buf[i++] = (uint8)StrBuf[1][0];		// ����ϵ��	
				Args.lastItemLen = i - 1;
				break;

			case 2: 
				CurrCmd = WaterCmd_ReadIpPortMode;			// ��IP+�˿�+ģʽ
				/*---------------------------------------------*/
				if(false == isUiFinish){
					break;
				}
				Args.buf[i++] = 0x07;		// ������	07
				ackLen = 4;					// Ӧ�𳤶� 5	
				// ������
				Args.buf[i++] = 0x0A;		// ����ѡ�� 0A	
				Args.lastItemLen = i - 1;
				break;
				
			case 3: 
				CurrCmd = WaterCmd_ReadOperatorNumber;		// " ����Ӫ�̱�� ";
				/*---------------------------------------------*/
				if(false == isUiFinish){
					break;
				}
				Args.buf[i++] = 0x07;		// ������	07
				ackLen = 4;					// Ӧ�𳤶� 5	
				// ������
				Args.buf[i++] = 0x0A;		// ����ѡ�� 0A	
				Args.lastItemLen = i - 1;
				break;

			case 4: 
				CurrCmd = WaterCmd_ReadImeiAndCcid;			// ��IMEI+CCID
				/*---------------------------------------------*/
				if(false == isUiFinish){
					if(StrBuf[1][0] == 0x00){
						StrBuf[1][0] = 0x01;
					}
					TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�û�����:", StrBuf[0], 10, 11*8, true);
					pUi[(*pUiCnt) -1].ui.txtbox.dotEnable = 1;
                    CombBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "����ϵ��:", &StrBuf[1][0], 4, 
						"1", "10", "100", "1000");
					break;
				}
				if(StrBuf[0][0] > '9' || StrBuf[0][0] < '0'){
					sprintf(StrBuf[0], " ������");
					currUi = uiRowIdx - 2 - 2;
					isUiFinish = false;
					continue;
				}

				Args.buf[i++] = 0x06;		// ������	06
				ackLen = 7;					// Ӧ�𳤶� 7	
				// ������
				u32Tmp = (uint32) _atof(StrBuf[0]);
				u16Tmp = (uint16)((float)((_atof(StrBuf[0]) - u32Tmp)*1000.0));
				Args.buf[i++] = (uint8)(u32Tmp & 0xFF);		// �û�����	
				Args.buf[i++] = (uint8)((u32Tmp >> 8) & 0xFF);
				Args.buf[i++] = (uint8)((u32Tmp >> 16) & 0xFF);
				Args.buf[i++] = (uint8)((u32Tmp >> 24) & 0xFF);
				Args.buf[i++] = (uint8)(u16Tmp & 0xFF);		
				Args.buf[i++] = (uint8)((u16Tmp >> 8) & 0xFF);
				Args.buf[i++] = (uint8)StrBuf[1][0];		// ����ϵ��	
				Args.lastItemLen = i - 1;
				break;

			case 5: 
				CurrCmd = WaterCmd_ClearReverseMeasureData;	// �����ת��������
				/*---------------------------------------------*/
				if(false == isUiFinish){
					break;
				}
				Args.buf[i++] = 0x0A;		// ������	0A
				ackLen = 6;					// Ӧ�𳤶� 6	
				// ������
				Args.lastItemLen = i - 1;
				break;

			case 6: 
				CurrCmd = WaterCmd_ReadFuncEnableState;		// ��ȡ����ʹ��״̬
				/*---------------------------------------------*/
				if(false == isUiFinish){
					break;
				}
				Args.buf[i++] = 0x0B;		// ������	0B
				ackLen = 2;					// Ӧ�𳤶� 2	
				// ������
				Args.buf[i++] = 0x00;
				Args.lastItemLen = i - 1;
				break;
			
			case 7: 
				CurrCmd = WaterCmd_SetTimedUpload;		// ���ö�ʱ�ϴ�
				/*---------------------------------------------*/
				if(false == isUiFinish){
					_Printfxy(7*16, (uiRowIdx)*16, "Сʱ", Color_White);
					TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "��ʱ���:", StrBuf[0], 2, 3*8, true);
					break;
				}
				if(StrBuf[0][0] > '9' || StrBuf[0][0] < '0'){
					sprintf(StrBuf[0], "   ");
					currUi = uiRowIdx - 2 - 1;
					isUiFinish = false;
					continue;
				}
				Args.buf[i++] = 0x0C;		// ������	0C
				ackLen = 2;					// Ӧ�𳤶� 2	
				// ������
				Args.buf[i++] = 0x00;		// ��𣺶�ʱ���
				Args.buf[i++] = (uint8) _atof(StrBuf[0]);	// ��ʱ���ʱ��
				Args.lastItemLen = i - 1;
				break;

            case 8: 
				CurrCmd = WaterCmd_SetFixedValUpload;		// ���ö����ϴ�
				/*---------------------------------------------*/
				if(false == isUiFinish){
					_Printfxy(7*16, (uiRowIdx)*16, "������", Color_White);
					TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�������:", StrBuf[0], 3, 4*8, true);
					break;
				}
				if((StrBuf[0][0] > '9' || StrBuf[0][0] < '0') 
					|| _atof(StrBuf[0]) > 255){
					sprintf(StrBuf[0], "   ");
					currUi = uiRowIdx - 2 - 1;
					isUiFinish = false;
					continue;
				}
				Args.buf[i++] = 0x0C;		// ������	0C
				ackLen = 2;					// Ӧ�𳤶� 2	
				// ������
				Args.buf[i++] = 0x01;		// ��𣺶������
				Args.buf[i++] = (uint8) _atof(StrBuf[0]);	// �����������
				Args.lastItemLen = i - 1;
				break;

			case 9: 
				CurrCmd = WaterCmd_SetTimedAndFixedValUpload;	// ���ö�ʱ�����ϴ�
				/*---------------------------------------------*/
				if(false == isUiFinish){
					_Printfxy(7*16, (uiRowIdx)*16, "Сʱ", Color_White);
					TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "��ʱ���:", StrBuf[0], 2, 3*8, true);
					_Printfxy(7*16, (uiRowIdx)*16, "������", Color_White);
					TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�������:", StrBuf[1], 3, 4*8, true);
					break;
				}
				if(StrBuf[0][0] > '9' || StrBuf[0][0] < '0'){
					sprintf(StrBuf[0], "   ");
					currUi = uiRowIdx - 2 - 2;
					isUiFinish = false;
					continue;
				}
				if((StrBuf[1][0] > '9' || StrBuf[1][0] < '0') 
					|| _atof(StrBuf[1]) > 255){
					sprintf(StrBuf[1], "   ");
					currUi = uiRowIdx - 2 - 1;
					isUiFinish = false;
					continue;
				}
				Args.buf[i++] = 0x0C;		// ������	0C
				ackLen = 2;					// Ӧ�𳤶� 2	
				// ������
				Args.buf[i++] = 0x02;		// ��𣺶�ʱ���
				Args.buf[i++] = (uint8) _atof(StrBuf[0]);	// ��ʱ���ʱ��
				Args.buf[i++] = (uint8) _atof(StrBuf[1]);	// �����������
				Args.lastItemLen = i - 1;
				break;

			case 10: 
				CurrCmd = WaterCmd_ReadMeterTime;			// �����ʱ��
				/*---------------------------------------------*/
				if(false == isUiFinish){
					break;
				}
				Args.buf[i++] = 0x13;		// ������	13
				ackLen = 7;					// Ӧ�𳤶� 7	
				// ������
				Args.buf[i++] = 0x00;		// ����ѡ�� 00	
				Args.lastItemLen = i - 1;
				break;

			case 11: 
				CurrCmd = WaterCmd_SetMeterTime;			// У���ʱ��
				/*---------------------------------------------*/
				if(false == isUiFinish){
					_GetDate(&TmpBuf[200], '-');
					_GetTime(&TmpBuf[220], ':');
					StrBuf[0][0] = TmpBuf[200];		// year
					StrBuf[0][1] = TmpBuf[201];
					StrBuf[0][2] = TmpBuf[202];
					StrBuf[0][3] = TmpBuf[203];
					StrBuf[0][4] = 0x00;
					StrBuf[1][0] = TmpBuf[205];		// month
					StrBuf[1][1] = TmpBuf[206];
					StrBuf[1][2] = 0x00;
					StrBuf[2][0] = TmpBuf[208];	// day
					StrBuf[2][1] = TmpBuf[209];
					StrBuf[2][2] = 0x00;
					StrBuf[3][0] = TmpBuf[220];	// hour
					StrBuf[3][1] = TmpBuf[221];
					StrBuf[3][2] = 0x00;
					StrBuf[4][0] = TmpBuf[223];	// minute
					StrBuf[4][1] = TmpBuf[224];
					StrBuf[4][2] = 0x00;
					StrBuf[5][0] = TmpBuf[226];	// second
					StrBuf[5][1] = TmpBuf[227];
					StrBuf[5][2] = 0x00;
					LableCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "ʱ ��:");
					TextBoxCreate(&pUi[(*pUiCnt)++], 0*8, (uiRowIdx)*16, " ", StrBuf[0], 4, 4*8, false);	// YYYY
					TextBoxCreate(&pUi[(*pUiCnt)++], 5*8, (uiRowIdx)*16, "-", StrBuf[1], 2, 2*8, false);	// MM
					TextBoxCreate(&pUi[(*pUiCnt)++], 8*8, (uiRowIdx)*16, "-", StrBuf[2], 2, 2*8, false);	// dd
					TextBoxCreate(&pUi[(*pUiCnt)++], 11*8, (uiRowIdx)*16, " ", StrBuf[3], 2, 2*8, false);	// HH
					TextBoxCreate(&pUi[(*pUiCnt)++], 14*8, (uiRowIdx)*16, ":", StrBuf[4], 2, 2*8, false);	// mm
					TextBoxCreate(&pUi[(*pUiCnt)++], 17*8, (uiRowIdx++)*16, ":", StrBuf[5], 2, 2*8, false);	// ss
					break;
				}
				
				for(i = 0; i < 6; i ++){
					if((StrBuf[i][0] > '9' && StrBuf[i][0] < '0') || StrBuf[0][0] == '0' ){
						// ��������Чֵ 
						currUi = uiRowIdx - 2 - 6;
						isUiFinish = false;
						break;
					}
				}
				if(false == isUiFinish){
					continue;
				}

				sprintf(&TmpBuf[200], "%s-%s-%s %s:%s:%s",
					StrBuf[0], StrBuf[1], StrBuf[2], StrBuf[3], StrBuf[4], StrBuf[5]);
				_SetDateTime(&TmpBuf[200]);

				i = 0;
				Args.buf[i++] = 0x14;		// ������	14
				ackLen = 2;					// Ӧ�𳤶� 2	
				// ������
				Args.buf[i++] = DecToBcd((uint8)(_GetYear()/100));		// ʱ�� - yyyy/mm/dd HH:mm:ss
				Args.buf[i++] = DecToBcd((uint8)(_GetYear()%100));		
				Args.buf[i++] = DecToBcd(_GetMonth());		
				Args.buf[i++] = DecToBcd(_GetDay());			
				Args.buf[i++] = DecToBcd(_GetHour());			
				Args.buf[i++] = DecToBcd(_GetMin());			
				Args.buf[i++] = DecToBcd(_GetSec());	
				Args.lastItemLen = i - 1;
				break;

            case 12: 
				CurrCmd = WaterCmd_ReadRxTxMgnDistbCnt;		// ����/��/���Ŵ���
				/*---------------------------------------------*/
				if(false == isUiFinish){
					break;
				}
				Args.buf[i++] = 0x09;		// ������	09
				ackLen = 7;					// Ӧ�𳤶� 7	
				// ������
				Args.buf[i++] = 0x00;				// ���ݸ�ʽ 00	
				Args.lastItemLen = i - 1;
				break;

			default: 
				break;
			}


			// ���� ���м̵�ַ����� �� ��ʾUI
			if(false == isUiFinish){
				
				key = ShowUI(UiList, &currUi);

				if (key == KEY_CANCEL){
					break;
				}

				if(StrDstAddr[0] < '0' || StrDstAddr[0] > '9'  ){
					sprintf(StrDstAddr, " ������");
					currUi = 0;
					continue;
				}

				isUiFinish = true;
				continue;	// go back to get ui args
			}

			// ��ַ���
			Water6009_PackAddrs(&Addrs, StrDstAddr, StrRelayAddr);
			#if (AddrLen == 6)
			PrintfXyMultiLine_VaList(0, 2*16, "�� ��: %s", StrDstAddr);
			#else
			PrintfXyMultiLine_VaList(0, 2*16, "�� ��:\n   %s", StrDstAddr);
			#endif

			// Ӧ�𳤶ȡ���ʱʱ�䡢�ط�����
			ackLen += 14 + Addrs.itemCnt * AddrLen;
			timeout = 8000 + (Addrs.itemCnt - 2) * 6000 * 2;
			tryCnt = 3;

			// ���͡����ա������ʾ
			key = Protol6009TranceiverWaitUI(CurrCmd, &Addrs, &Args, ackLen, timeout, tryCnt);
			
			
			// ���� / ����
			if (key == KEY_CANCEL){
				break;
			}else{
				isUiFinish = false;
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
	UI_Item * pUi = &UiList.items[0];
	uint8 * pUiCnt = &UiList.cnt;
	uint8 * pByte;
	uint8 currUi = 0, uiRowIdx, isUiFinish, u8Tmp;
	uint16 ackLen = 0, timeout, u16Tmp;

	_ClearScreen();

	// �˵�
	menuList.title = "<<��������";
	menuList.no = 5;
	menuList.MaxNum = 5;
	menuList.isRt = 0;
	menuList.x = 0;
	menuList.y = 0;
	menuList.with = 10 * 16;
	menuList.str[0] = "1. ���շ����ŷ�����";
	menuList.str[1] = "2. ��ȡRXD��TXD�ŵ�";
	menuList.str[2] = "3. ����RXD��TXD�ŵ�";
	menuList.str[3] = "4. ������Ӫ�̱��";
	menuList.str[4] = "5. ·���·�";
	menuList.defbar = 1;

	_CloseCom();
	_ComSetTran(CurrPort);
	_ComSet(CurrBaud, 2);

	while(1){

		_ClearScreen();
		menuItemNo = _ListEx(&menuList);

		if (menuItemNo == 0){
			break;
		}
		menuList.defbar = menuItemNo;
		memset(StrBuf, 0, TXTBUF_LEN * 10);
		isUiFinish = false;

		while(1){
			
			_ClearScreen();

			// �������� :  ������ʾ
			pByte = menuList.str[menuItemNo - 1];
			sprintf(TmpBuf, "<<%s",&pByte[3]);
			_Printfxy(0, 0, TmpBuf, Color_White);
			_GUIHLine(0, 1*16 + 4, 160, Color_Black);	
			//----------------------------------------------
			_GUIHLine(0, 9*16 - 4, 160, Color_Black);
			_Printfxy(0, 9*16, "���� <�ȴ�����> ִ��", Color_White);

			if(false == isUiFinish){
				(*pUiCnt) = 0;
				uiRowIdx = 2;
				#if (AddrLen == 6)
				TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�� ��:", StrDstAddr, 12, 13*8, true);
				#else
				LableCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�� ��:");
				TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "   ", StrDstAddr, 16, 17*8, true);	
				#endif
			}
			
			// �����������
			i = 0;	
			Args.itemCnt = 2;
			Args.items[0] = &Args.buf[0];   // ������
			Args.items[1] = &Args.buf[1];	// ������
			CurrCmd = (0x60 + menuItemNo);

			switch(CurrCmd){
			case WaterCmd_ReadRxTxMgnDistbCnt:		// ����/��/���Ŵ���
				/*---------------------------------------------*/
				if(false == isUiFinish){
					break;
				}
				Args.buf[i++] = 0x09;		// ������	09
				ackLen = 7;					// Ӧ�𳤶� 7	
				// ������
				Args.buf[i++] = 0x00;				// ���ݸ�ʽ 00	
				Args.lastItemLen = i - 1;
				break;

			case WaterCmd_ReadRxdAndTxdChanel:	// ��ȡRXD��TXD�ŵ�
				/*---------------------------------------------*/
				if(false == isUiFinish){
					break;
				}
				Args.buf[i++] = 0x1B;		// ������	1B
				ackLen = 3;					// Ӧ�𳤶� 3	
				// ������
				Args.buf[i++] = 0x00;		// ��ȡ��� 0 - �����ŵ� �� �ϴ��ŵ�
				Args.buf[i++] = 0;			// �����ŵ� bit7 0-��ȡ ��1-����
				Args.buf[i++] = 0;			// �ϴ��ŵ� bit7 0-��ȡ ��1-����
				Args.lastItemLen = i - 1;
				break;

			case WaterCmd_SetRxdAndTxdChanel:	// ����RXD��TXD�ŵ�
				/*---------------------------------------------*/
				if(false == isUiFinish){
					TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�����ŵ�:", StrBuf[0], 2, 11*8, true);
					TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�ϴ��ŵ�:", StrBuf[1], 2, 11*8, true);
					break;
				}
				if(StrBuf[0][0] > '9' || StrBuf[0][0] < '0'){
					sprintf(StrBuf[0], " ������");
					currUi = uiRowIdx - 2 - 2;
					isUiFinish = false;
					continue;
				}
				if(StrBuf[1][0] > '9' || StrBuf[1][0] < '0'){
					sprintf(StrBuf[1], " ������");
					currUi = uiRowIdx - 2 - 1;
					isUiFinish = false;
					continue;
				}
				u8Tmp = (uint8) _atof(StrBuf[0]);
				u16Tmp = (uint16) _atof(StrBuf[1]);

				Args.buf[i++] = 0x1B;		// ������	1B
				ackLen = 2;					// Ӧ�𳤶� 2	
				// ������
				Args.buf[i++] = 0x00;		// ������� 0 - �����ŵ� �� �ϴ��ŵ�
				Args.buf[i++] = (0x80 | u8Tmp);			// �����ŵ� bit7 0-��ȡ ��1-����
				Args.buf[i++] = (0x80 | (uint8)u16Tmp);	// �ϴ��ŵ� bit7 0-��ȡ ��1-����
				Args.lastItemLen = i - 1;
				break;
			
			case WaterCmd_SetOperatorNumber:		// ������Ӫ�̱��
				/*---------------------------------------------*/
				if(false == isUiFinish){
					TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "��Ӫ�̱��:", StrBuf[0], 8, 9*8, true);
					break;
				}
				if(StrBuf[0][0] > '9' || StrBuf[0][0] < '0'){
					sprintf(StrBuf[0], " ������");
					currUi = uiRowIdx - 2 - 1;
					isUiFinish = false;
					continue;
				}
				Args.buf[i++] = 0x21;		// ������	21
				ackLen = 2;					// Ӧ�𳤶� 2	
				// ������
				GetBytesFromStringHex(&Args.buf[i], 0, 4, StrBuf[0], 0, false);
				i += 4;
				Args.lastItemLen = i - 1;
				break;

            case WaterCmd_SetDefinedRoute:	// ·���·�
				/*---------------------------------------------*/
				_Printfxy(0, 9*16, "���� <�ȴ�����> ����", Color_White);
				if(false == isUiFinish){
					if(StrBuf[1][0] == 0x00){
						StrBuf[1][0] = '0';
					}
					CombBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "·������: ", &StrBuf[0][0], 4, 
						"2", "3", "4", "5");
					TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "��ǰλ��: ", StrBuf[1], 1, 10*8, true);
					break;
				}
				if(StrBuf[1][0] > '9' || StrBuf[1][0] < '0'){
					sprintf(StrBuf[1], " ������");
					currUi = uiRowIdx - 2 - 1;
					isUiFinish = false;
					continue;
				}
				u8Tmp = (uint8)StrBuf[0][0] + 2;	// ·������
				u16Tmp = (uint16) _atof(StrBuf[1]);	// ��ǰλ��
				if(u16Tmp > u8Tmp - 2){
					sprintf(StrBuf[1], " (��ʱ<=%d)", (uint8)StrBuf[0][0]);
					currUi = uiRowIdx - 2 - 1;
					isUiFinish = false;
					continue;
				}
				while(1){
					_Printfxy(0, 9*16, "���� <�ȴ�����> ִ��", Color_White);
					_Printfxy(0, 2*16, "·���ĸ��ڵ��ַ:", Color_White);
					(*pUiCnt) = 0;
					uiRowIdx = 3;
					StrBuf[0][0] = '\0';
					StrBuf[1][0] = '\0';
					for(i = 0; i < u8Tmp; i++){
						PrintfXyMultiLine_VaList(0, (uiRowIdx)*16, "%d: ", i);
						if(i == u16Tmp){	// ��ǰλ��
							memcpy(StrBuf[i], StrDstAddr, TXTBUF_LEN);
							_Printfxy(4*8, (uiRowIdx)*16, StrBuf[i], Color_White);
						}
						else{
							TextBoxCreate(&pUi[(*pUiCnt)++], 3*8, (uiRowIdx)*16, " ", StrBuf[i], 12, 13*8, true);
						}
						uiRowIdx++;
					}
					currUi = 0;
					key = ShowUI(UiList, &currUi);
					isUiFinish = true;

					if (key == KEY_CANCEL){
						break;
					}
					currUi = 0;
					for(i = 0; i < u8Tmp; i++){
						if(i != u16Tmp){
							currUi++;
						}
						if(StrBuf[i][0] > '9' || StrBuf[i][0] < '0'){
							sprintf(StrBuf[i], " ������");
							isUiFinish = false;
							break;
						}
					}

					if(isUiFinish){
						break;
					}
				}
				if (key == KEY_CANCEL){
					break;
				}
				i = 0;
				Args.buf[i++] = 0x22;		// ������	22
				ackLen = 2;					// Ӧ�𳤶� 2	
				// ������
				Args.buf[i++] = (uint8)u16Tmp;	// ��ǰλ��
				Args.buf[i++] = u8Tmp;			// ·������
				for(u16Tmp = 0; u16Tmp < u8Tmp; u16Tmp++){
					GetBytesFromStringHex(&Args.buf[i], 0, 6, StrBuf[u16Tmp], 0, false);
					i += 6;
				}
				Args.lastItemLen = i - 1;
				break;

			default: 
				break;
			}

			// ���� ���м̵�ַ����� �� ��ʾUI
			if(false == isUiFinish){

				key = ShowUI(UiList, &currUi);

				if (key == KEY_CANCEL){
					break;
				}

				if(StrDstAddr[0] < '0' || StrDstAddr[0] > '9'  ){
					sprintf(StrDstAddr, " ������");
					currUi = 0;
					continue;
				}

				isUiFinish = true;
				continue;	// go back to get ui args
			}

			if (key == KEY_CANCEL){
				break;
			}

			// ��ַ���
			Water6009_PackAddrs(&Addrs, StrDstAddr, StrRelayAddr);
			#if (AddrLen == 6)
			PrintfXyMultiLine_VaList(0, 2*16, "�� ��: %s", StrDstAddr);
			#else
			PrintfXyMultiLine_VaList(0, 2*16, "�� ��:\n   %s", StrDstAddr);
			#endif

			// Ӧ�𳤶ȡ���ʱʱ�䡢�ط�����
			ackLen += 14 + Addrs.itemCnt * AddrLen;
			timeout = 8000 + (Addrs.itemCnt - 2) * 6000 * 2;
			tryCnt = 3;

			// ���͡����ա������ʾ
			key = Protol6009TranceiverWaitUI(CurrCmd, &Addrs, &Args, ackLen, timeout, tryCnt);
			
			
			// ���� / ����
			if (key == KEY_CANCEL){
				break;
			}else{
				isUiFinish = false;
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
	menu.no=7;
	menu.title= "<<���̵��� ";		// ���̵��� --> ��ԭ���� ��˲���
	menu.str[0]=" �������� ";
	menu.str[1]=" �������� ";
	menu.str[2]=" �������� ";
	menu.str[3]=" Ԥ������ ";
	menu.str[4]=" �������� ";
	menu.str[5]=" �������� ";
	menu.str[6]=" �汾��Ϣ ";
	menu.key[0]="1";
	menu.key[1]="2";
	menu.key[2]="3";
	menu.key[3]="4";
	menu.key[4]="5";
	menu.key[5]="6";
	menu.key[6]="7";
	menu.Function[0]=WaterCmdFunc_CommonCmd;
	menu.Function[1]=WaterCmdFunc_TestCmd;
	menu.Function[2]=WaterCmdFunc_Upgrade;
	menu.Function[3]=WaterCmdFunc_PrepaiedVal;
	menu.Function[4]=WaterCmdFunc_WorkingParams;
	menu.Function[5]=WaterCmdFunc_Other;
	menu.Function[6]=VersionInfoFunc;
	menu.FunctionEx=0;
	_Menu(&menu);	
}

//-----------------------------------	���˵�	---------------------------
// ��ȡ�û�����
void MainFuncReadRealTimeData(void)
{
	uint8 key, tryCnt = 0, i;
	UI_Item * pUi = &UiList.items[0];
	uint8 * pUiCnt = &UiList.cnt;
	uint8 currUi = 0, uiRowIdx, isUiFinish;
	uint16 ackLen = 0, timeout;

	_CloseCom();
	_ComSetTran(CurrPort);
	_ComSet(CurrBaud, 2);

	memset(StrBuf, 0, TXTBUF_LEN * 10);
	isUiFinish = false;

	while(1){
		
		_ClearScreen();

		// �������� :  ������ʾ
		_Printfxy(0, 0, "<<��ȡ�û�����", Color_White);
		_GUIHLine(0, 1*16 + 4, 160, Color_Black);	
		/*---------------------------------------------*/
		//----------------------------------------------
		_GUIHLine(0, 9*16 - 4, 160, Color_Black);
		_Printfxy(0, 9*16, "���� <�ȴ�����> ִ��", Color_White);

		if(false == isUiFinish){
			(*pUiCnt) = 0;
			uiRowIdx = 2;
			#if (AddrLen == 6)
			TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�� ��:", StrDstAddr, 12, 13*8, true);
			#else
			LableCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�� ��:");
			TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "   ", StrDstAddr, 16, 17*8, true);	
			#endif
		}
			
		// �����������
		i = 0;	
		Args.itemCnt = 2;
		Args.items[0] = &Args.buf[0];   // ������
		Args.items[1] = &Args.buf[1];	// ������
		CurrCmd = WaterCmd_ReadRealTimeData;	// "��ȡ�û�����"

		
		switch(CurrCmd){
		case WaterCmd_ReadRealTimeData:		// "��ȡ�û�����"
			/*---------------------------------------------*/
			if(false == isUiFinish){
				break;
			}
			Args.buf[i++] = 0x01;		// ������	01
			ackLen = 21;				// Ӧ�𳤶� 21	
			// ������
			Args.buf[i++] = 0x00;				// ���ݸ�ʽ 00	
			Args.lastItemLen = i - 1;
			break;

		default: 
			break;
		}

		// ���� ���м̵�ַ����� �� ��ʾUI
		if(false == isUiFinish){
		
			key = ShowUI(UiList, &currUi);

			if (key == KEY_CANCEL){
				break;
			}

			if(StrDstAddr[0] < '0' || StrDstAddr[0] > '9'  ){
				sprintf(StrDstAddr, " ������");
				currUi = 0;
				continue;
			}

			isUiFinish = true;
			continue;	// go back to get ui args
		}

		// ��ַ���
		Water6009_PackAddrs(&Addrs, StrDstAddr, StrRelayAddr);
		#if (AddrLen == 6)
		PrintfXyMultiLine_VaList(0, 2*16, "�� ��: %s", StrDstAddr);
		#else
		PrintfXyMultiLine_VaList(0, 2*16, "�� ��:\n   %s", StrDstAddr);
		#endif

		// Ӧ�𳤶ȡ���ʱʱ�䡢�ط�����
		ackLen += 14 + Addrs.itemCnt * AddrLen;
		timeout = 8000 + (Addrs.itemCnt - 2) * 6000 * 2;
		tryCnt = 3;

		// ���͡����ա������ʾ
		key = Protol6009TranceiverWaitUI(CurrCmd, &Addrs, &Args, ackLen, timeout, tryCnt);
		
		
		// ���� / ����
		if (key == KEY_CANCEL){
			break;
		}else{
			isUiFinish = false;
			continue;
		}
	}

	_CloseCom();
}

// ��ȡ��������
void MainFuncReadFrozenData(void)
{
	uint8 key, tryCnt = 0, i;
	UI_Item * pUi = &UiList.items[0];
	uint8 * pUiCnt = &UiList.cnt;
	uint8 currUi = 0, uiRowIdx, isUiFinish;
	uint16 ackLen = 0, timeout;

	_CloseCom();
	_ComSetTran(CurrPort);
	_ComSet(CurrBaud, 2);

	memset(StrBuf, 0, TXTBUF_LEN * 10);
	isUiFinish = false;

	while(1){
		
		_ClearScreen();

		// �������� :  ������ʾ
		_Printfxy(0, 0, "<<��ȡ��������", Color_White);
		_GUIHLine(0, 1*16 + 4, 160, Color_Black);	
		/*---------------------------------------------*/
		//----------------------------------------------
		_GUIHLine(0, 9*16 - 4, 160, Color_Black);
		_Printfxy(0, 9*16, "���� <�ȴ�����> ִ��", Color_White);

		if(false == isUiFinish){
			(*pUiCnt) = 0;
			uiRowIdx = 2;
			#if (AddrLen == 6)
			TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�� ��:", StrDstAddr, 12, 13*8, true);
			#else
			LableCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�� ��:");
			TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "   ", StrDstAddr, 16, 17*8, true);	
			#endif
		}
			
		// �����������
		i = 0;	
		Args.itemCnt = 2;
		Args.items[0] = &Args.buf[0];   // ������
		Args.items[1] = &Args.buf[1];	// ������
		CurrCmd = WaterCmd_ReadFrozenData;		// "��ȡ������ת����"
		
		switch(CurrCmd){
		case WaterCmd_ReadFrozenData:		// "��ȡ������ת����"
			/*---------------------------------------------*/
			if(false == isUiFinish){
				sprintf(StrBuf[0], "0 (0-9��Ч)");
				TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�� ��:", StrBuf[0], 1, 2*8, true);
				break;
			}
			
			if(StrBuf[0][0] > '9' || StrBuf[0][0] < '0'){
				currUi = 1;
				isUiFinish = false;
				continue;
			}
			Args.buf[i++] = 0x02;		// ������	02
			ackLen = 114;				// Ӧ�𳤶� 88/114	
			// ������
			Args.buf[i++] = 0x01;				// ���ݸ�ʽ 01/02
			Args.buf[i++] = _GetYear()/100;		// ʱ�� - yyyy/mm/dd HH:mm:ss
			Args.buf[i++] = _GetYear()%100;		
			Args.buf[i++] = _GetMonth();		
			Args.buf[i++] = _GetDay();			
			Args.buf[i++] = _GetHour();			
			Args.buf[i++] = _GetMin();			
			Args.buf[i++] = _GetSec();			
			Args.buf[i++] = StrBuf[0][0] - '0';	// �����������	
			Args.lastItemLen = i - 1;
			break;

		default: 
			break;
		}
				
		// ���� ���м̵�ַ����� �� ��ʾUI
		if(false == isUiFinish){
	
			key = ShowUI(UiList, &currUi);

			if (key == KEY_CANCEL){
				break;
			}

			if(StrDstAddr[0] < '0' || StrDstAddr[0] > '9'  ){
				sprintf(StrDstAddr, " ������");
				currUi = 0;
				continue;
			}

			isUiFinish = true;
			continue;	// go back to get ui args
		}

		// ��ַ���
		Water6009_PackAddrs(&Addrs, StrDstAddr, StrRelayAddr);
		#if (AddrLen == 6)
		PrintfXyMultiLine_VaList(0, 2*16, "�� ��: %s", StrDstAddr);
		#else
		PrintfXyMultiLine_VaList(0, 2*16, "�� ��:\n   %s", StrDstAddr);
		#endif

		// Ӧ�𳤶ȡ���ʱʱ�䡢�ط�����
		ackLen += 14 + Addrs.itemCnt * AddrLen;
		timeout = 8000 + (Addrs.itemCnt - 2) * 6000 * 2;
		tryCnt = 3;

		// ���͡����ա������ʾ
		key = Protol6009TranceiverWaitUI(CurrCmd, &Addrs, &Args, ackLen, timeout, tryCnt);
		
		
		// ���� / ����
		if (key == KEY_CANCEL){
			break;
		}else{
			isUiFinish = false;
			continue;
		}
	}

	_CloseCom();
}

// ��ȡ���ʱ��
void MainFuncReadMeterTime(void)
{
	uint8 key, tryCnt = 0, i;
	UI_Item * pUi = &UiList.items[0];
	uint8 * pUiCnt = &UiList.cnt;
	uint8 currUi = 0, uiRowIdx, isUiFinish;
	uint16 ackLen = 0, timeout;

	_CloseCom();
	_ComSetTran(CurrPort);
	_ComSet(CurrBaud, 2);

	memset(StrBuf, 0, TXTBUF_LEN * 10);
	isUiFinish = false;

	while(1){
		
		_ClearScreen();

		// �������� :  ������ʾ
		_Printfxy(0, 0, "<<��ȡ���ʱ��", Color_White);
		_GUIHLine(0, 1*16 + 4, 160, Color_Black);	
		/*---------------------------------------------*/
		//----------------------------------------------
		_GUIHLine(0, 9*16 - 4, 160, Color_Black);
		_Printfxy(0, 9*16, "���� <�ȴ�����> ִ��", Color_White);

		if(false == isUiFinish){
			(*pUiCnt) = 0;
			uiRowIdx = 2;
			#if (AddrLen == 6)
			TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�� ��:", StrDstAddr, 12, 13*8, true);
			#else
			LableCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�� ��:");
			TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "   ", StrDstAddr, 16, 17*8, true);	
			#endif
		}
			
		// �����������
		i = 0;	
		Args.itemCnt = 2;
		Args.items[0] = &Args.buf[0];   // ������
		Args.items[1] = &Args.buf[1];	// ������
		CurrCmd = WaterCmd_ReadMeterTime;	// ��ȡ���ʱ��

		switch(CurrCmd){
		case WaterCmd_ReadMeterTime:		// "��ȡ���ʱ��"
			/*---------------------------------------------*/
			if(false == isUiFinish){
				break;
			}
			Args.buf[i++] = 0x13;		// ������	13
			ackLen = 7;					// Ӧ�𳤶� 7	
			Args.lastItemLen = i - 1;
			break;
		default:
			break;
		}
		// ���� ���м̵�ַ����� �� ��ʾUI
		if(false == isUiFinish){
		
			key = ShowUI(UiList, &currUi);

			if (key == KEY_CANCEL){
				break;
			}

			if(StrDstAddr[0] < '0' || StrDstAddr[0] > '9'  ){
				sprintf(StrDstAddr, " ������");
				currUi = 0;
				continue;
			}

			isUiFinish = true;
			continue;	// go back to get ui args
		}

		// ��ַ���
		Water6009_PackAddrs(&Addrs, StrDstAddr, StrRelayAddr);
		#if (AddrLen == 6)
		PrintfXyMultiLine_VaList(0, 2*16, "�� ��: %s", StrDstAddr);
		#else
		PrintfXyMultiLine_VaList(0, 2*16, "�� ��:\n   %s", StrDstAddr);
		#endif

		// Ӧ�𳤶ȡ���ʱʱ�䡢�ط�����
		ackLen += 14 + Addrs.itemCnt * AddrLen;
		timeout = 8000 + (Addrs.itemCnt - 2) * 6000 * 2;
		tryCnt = 3;

		// ���͡����ա������ʾ
		key = Protol6009TranceiverWaitUI(CurrCmd, &Addrs, &Args, ackLen, timeout, tryCnt);
		
		
		// ���� / ����
		if (key == KEY_CANCEL){
			break;
		}else{
			isUiFinish = false;
			continue;
		}
	}

	_CloseCom();
}

// ���ñ��ʱ��
void MainFuncSetMeterTime(void)
{
	uint8 key, tryCnt = 0, i;
	UI_Item * pUi = &UiList.items[0];
	uint8 * pUiCnt = &UiList.cnt;
	uint8 currUi = 0, uiRowIdx, isUiFinish;
	uint16 ackLen = 0, timeout;

	_CloseCom();
	_ComSetTran(CurrPort);
	_ComSet(CurrBaud, 2);

	memset(StrBuf, 0, TXTBUF_LEN * 10);
	isUiFinish = false;

	while(1){
		
		_ClearScreen();

		// �������� :  ������ʾ
		_Printfxy(0, 0, "<<���ñ��ʱ��", Color_White);
		_GUIHLine(0, 1*16 + 4, 160, Color_Black);	
		/*---------------------------------------------*/
		//----------------------------------------------
		_GUIHLine(0, 9*16 - 4, 160, Color_Black);
		_Printfxy(0, 9*16, "���� <�ȴ�����> ִ��", Color_White);

		if(false == isUiFinish){
			(*pUiCnt) = 0;
			uiRowIdx = 2;
			#if (AddrLen == 6)
			TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�� ��:", StrDstAddr, 12, 13*8, true);
			#else
			LableCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�� ��:");
			TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "   ", StrDstAddr, 16, 17*8, true);	
			#endif
		}
			
		// �����������
		i = 0;	
		Args.itemCnt = 2;
		Args.items[0] = &Args.buf[0];   // ������
		Args.items[1] = &Args.buf[1];	// ������
		CurrCmd = WaterCmd_SetMeterTime;	// ���ñ��ʱ��

		switch(CurrCmd){
		case WaterCmd_SetMeterTime:		// "���ñ��ʱ��"
			/*---------------------------------------------*/
			if(false == isUiFinish){
				_GetDate(&TmpBuf[200], '-');
				_GetTime(&TmpBuf[220], ':');
				StrBuf[0][0] = TmpBuf[200];		// year
				StrBuf[0][1] = TmpBuf[201];
				StrBuf[0][2] = TmpBuf[202];
				StrBuf[0][3] = TmpBuf[203];
				StrBuf[0][4] = 0x00;
				StrBuf[1][0] = TmpBuf[205];		// month
				StrBuf[1][1] = TmpBuf[206];
				StrBuf[1][2] = 0x00;
				StrBuf[2][0] = TmpBuf[208];	// day
				StrBuf[2][1] = TmpBuf[209];
				StrBuf[2][2] = 0x00;
				StrBuf[3][0] = TmpBuf[220];	// hour
				StrBuf[3][1] = TmpBuf[221];
				StrBuf[3][2] = 0x00;
				StrBuf[4][0] = TmpBuf[223];	// minute
				StrBuf[4][1] = TmpBuf[224];
				StrBuf[4][2] = 0x00;
				StrBuf[5][0] = TmpBuf[226];	// second
				StrBuf[5][1] = TmpBuf[227];
				StrBuf[5][2] = 0x00;
				LableCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "ʱ ��:");
				TextBoxCreate(&pUi[(*pUiCnt)++], 0*8, (uiRowIdx)*16, " ", StrBuf[0], 4, 4*8, false);	// YYYY
				TextBoxCreate(&pUi[(*pUiCnt)++], 5*8, (uiRowIdx)*16, "-", StrBuf[1], 2, 2*8, false);	// MM
				TextBoxCreate(&pUi[(*pUiCnt)++], 8*8, (uiRowIdx)*16, "-", StrBuf[2], 2, 2*8, false);	// dd
				TextBoxCreate(&pUi[(*pUiCnt)++], 11*8, (uiRowIdx)*16, " ", StrBuf[3], 2, 2*8, false);	// HH
				TextBoxCreate(&pUi[(*pUiCnt)++], 14*8, (uiRowIdx)*16, ":", StrBuf[4], 2, 2*8, false);	// mm
				TextBoxCreate(&pUi[(*pUiCnt)++], 17*8, (uiRowIdx++)*16, ":", StrBuf[5], 2, 2*8, false);	// ss
				break;
			}
			for(i = 0; i < 6; i ++){
				if((StrBuf[i][0] > '9' && StrBuf[i][0] < '0') || StrBuf[0][0] == '0' ){
					// ��������Чֵ 
					currUi = 1;
					isUiFinish = false;
					break;
				}
			}
			if(false == isUiFinish){
				continue;
			}

			sprintf(&TmpBuf[200], "%s-%s-%s %s:%s:%s",
				StrBuf[0], StrBuf[1], StrBuf[2], StrBuf[3], StrBuf[4], StrBuf[5]);
			_SetDateTime(&TmpBuf[200]);

			i = 0;
			Args.buf[i++] = 0x14;		// ������	14
			ackLen = 2;					// Ӧ�𳤶� 2	
			// ������
			Args.buf[i++] = DecToBcd((uint8)(_GetYear()/100));		// ʱ�� - yyyy/mm/dd HH:mm:ss
			Args.buf[i++] = DecToBcd((uint8)(_GetYear()%100));		
			Args.buf[i++] = DecToBcd(_GetMonth());		
			Args.buf[i++] = DecToBcd(_GetDay());			
			Args.buf[i++] = DecToBcd(_GetHour());			
			Args.buf[i++] = DecToBcd(_GetMin());			
			Args.buf[i++] = DecToBcd(_GetSec());	
			Args.lastItemLen = i - 1;
			break;

		default:
			break;
		}

		// ���� ���м̵�ַ����� �� ��ʾUI
		if(false == isUiFinish){
		
			key = ShowUI(UiList, &currUi);

			if (key == KEY_CANCEL){
				break;
			}

			if(StrDstAddr[0] < '0' || StrDstAddr[0] > '9'  ){
				sprintf(StrDstAddr, " ������");
				currUi = 0;
				continue;
			}

			isUiFinish = true;
			continue;	// go back to get ui args
		}

		// ��ַ���
		Water6009_PackAddrs(&Addrs, StrDstAddr, StrRelayAddr);
		#if (AddrLen == 6)
		PrintfXyMultiLine_VaList(0, 2*16, "�� ��: %s", StrDstAddr);
		#else
		PrintfXyMultiLine_VaList(0, 2*16, "�� ��:\n   %s", StrDstAddr);
		#endif

		// Ӧ�𳤶ȡ���ʱʱ�䡢�ط�����
		ackLen += 14 + Addrs.itemCnt * AddrLen;
		timeout = 8000 + (Addrs.itemCnt - 2) * 6000 * 2;
		tryCnt = 3;

		// ���͡����ա������ʾ
		key = Protol6009TranceiverWaitUI(CurrCmd, &Addrs, &Args, ackLen, timeout, tryCnt);
		
		
		// ���� / ����
		if (key == KEY_CANCEL){
			break;
		}else{
			isUiFinish = false;
			continue;
		}
	}

	_CloseCom();
}

// ���쳣����
void MainFuncClearException(void)
{
	uint8 key, tryCnt = 0, i;
	UI_Item * pUi = &UiList.items[0];
	uint8 * pUiCnt = &UiList.cnt;
	uint8 currUi = 0, uiRowIdx, isUiFinish;
	uint16 ackLen = 0, timeout;

	_CloseCom();
	_ComSetTran(CurrPort);
	_ComSet(CurrBaud, 2);

	memset(StrBuf, 0, TXTBUF_LEN * 10);
	isUiFinish = false;

	while(1){
		
		_ClearScreen();

		// �������� :  ������ʾ
		_Printfxy(0, 0, "<<���쳣����", Color_White);
		_GUIHLine(0, 1*16 + 4, 160, Color_Black);	
		/*---------------------------------------------*/
		//----------------------------------------------
		_GUIHLine(0, 9*16 - 4, 160, Color_Black);
		_Printfxy(0, 9*16, "���� <�ȴ�����> ִ��", Color_White);

		if(false == isUiFinish){
			(*pUiCnt) = 0;
			uiRowIdx = 2;
			#if (AddrLen == 6)
			TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�� ��:", StrDstAddr, 12, 13*8, true);
			#else
			LableCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�� ��:");
			TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "   ", StrDstAddr, 16, 17*8, true);	
			#endif
		}
			
		// �����������
		i = 0;	
		Args.itemCnt = 2;
		Args.items[0] = &Args.buf[0];   // ������
		Args.items[1] = &Args.buf[1];	// ������
		CurrCmd = WaterCmd_ClearException;		// " ���쳣���� ";
		
		switch(CurrCmd){
		case WaterCmd_ClearException:		// " ���쳣���� ";
			/*---------------------------------------------*/
			if(false == isUiFinish){
				break;
			}
			Args.buf[i++] = 0x05;		// ������	05
			ackLen = 1;					// Ӧ�𳤶� 1	
			// ������
			Args.buf[i++] = 0x00;		// ����ѡ�� 00	
			Args.lastItemLen = i - 1;
			break;

		default: 
			break;
		}

		// ���� ���м̵�ַ����� �� ��ʾUI
		if(false == isUiFinish){
	
			key = ShowUI(UiList, &currUi);

			if (key == KEY_CANCEL){
				break;
			}

			if(StrDstAddr[0] < '0' || StrDstAddr[0] > '9'  ){
				sprintf(StrDstAddr, " ������");
				currUi = 0;
				continue;
			}

			isUiFinish = true;
			continue;	// go back to get ui args
		}

		// ��ַ���
		Water6009_PackAddrs(&Addrs, StrDstAddr, StrRelayAddr);
		#if (AddrLen == 6)
		PrintfXyMultiLine_VaList(0, 2*16, "�� ��: %s", StrDstAddr);
		#else
		PrintfXyMultiLine_VaList(0, 2*16, "�� ��:\n   %s", StrDstAddr);
		#endif

		// Ӧ�𳤶ȡ���ʱʱ�䡢�ط�����
		ackLen += 14 + Addrs.itemCnt * AddrLen;
		timeout = 8000 + (Addrs.itemCnt - 2) * 6000 * 2;
		tryCnt = 3;

		// ���͡����ա������ʾ
		key = Protol6009TranceiverWaitUI(CurrCmd, &Addrs, &Args, ackLen, timeout, tryCnt);
		
		
		// ���� / ����
		if (key == KEY_CANCEL){
			break;
		}else{
			isUiFinish = false;
			continue;
		}
	}

	_CloseCom();
}

// ����
void MainFuncOpenValve(void)
{
	uint8 key, tryCnt = 0, i;
	UI_Item * pUi = &UiList.items[0];
	uint8 * pUiCnt = &UiList.cnt;
	uint8 currUi = 0, uiRowIdx, isUiFinish;
	uint16 ackLen = 0, timeout;

	_CloseCom();
	_ComSetTran(CurrPort);
	_ComSet(CurrBaud, 2);

	memset(StrBuf, 0, TXTBUF_LEN * 10);
	isUiFinish = false;

	while(1){
		
		_ClearScreen();

		// �������� :  ������ʾ
		_Printfxy(0, 0, "<<����", Color_White);
		_GUIHLine(0, 1*16 + 4, 160, Color_Black);	
		/*---------------------------------------------*/
		//----------------------------------------------
		_GUIHLine(0, 9*16 - 4, 160, Color_Black);
		_Printfxy(0, 9*16, "���� <�ȴ�����> ִ��", Color_White);

		if(false == isUiFinish){
			(*pUiCnt) = 0;
			uiRowIdx = 2;
			#if (AddrLen == 6)
			TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�� ��:", StrDstAddr, 12, 13*8, true);
			#else
			LableCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�� ��:");
			TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "   ", StrDstAddr, 16, 17*8, true);	
			#endif
		}
			
		// �����������
		i = 0;	
		Args.itemCnt = 2;
		Args.items[0] = &Args.buf[0];   // ������
		Args.items[1] = &Args.buf[1];	// ������
		CurrCmd = WaterCmd_OpenValve;		// " ���� "
		
		switch(CurrCmd){
		case WaterCmd_OpenValve:			// " ���� "
			/*---------------------------------------------*/
			if(false == isUiFinish){
				break;
			}
			Args.buf[i++] = 0x03;		// ������	03
			ackLen = 3;					// Ӧ�𳤶� 3	
			// ������
			Args.buf[i++] = 0x00;		// ǿ�Ʊ�ʶ 	0 - ��ǿ�ƣ� 1 - ǿ��
			Args.buf[i++] = 0x01;		// ���ط���ʶ	0 - �ط��� 1 - ����
			Args.lastItemLen = i - 1;
			break;

		default: 
			break;
		}

		// ���� ���м̵�ַ����� �� ��ʾUI
		if(false == isUiFinish){
		
			key = ShowUI(UiList, &currUi);

			if (key == KEY_CANCEL){
				break;
			}

			if(StrDstAddr[0] < '0' || StrDstAddr[0] > '9'  ){
				sprintf(StrDstAddr, " ������");
				currUi = 0;
				continue;
			}

			isUiFinish = true;
			continue;	// go back to get ui args
		}

		// ��ַ���
		Water6009_PackAddrs(&Addrs, StrDstAddr, StrRelayAddr);
		#if (AddrLen == 6)
		PrintfXyMultiLine_VaList(0, 2*16, "�� ��: %s", StrDstAddr);
		#else
		PrintfXyMultiLine_VaList(0, 2*16, "�� ��:\n   %s", StrDstAddr);
		#endif

		// Ӧ�𳤶ȡ���ʱʱ�䡢�ط�����
		ackLen += 14 + Addrs.itemCnt * AddrLen;
		timeout = 8000 + (Addrs.itemCnt - 2) * 6000 * 2;
		tryCnt = 3;

		// ���͡����ա������ʾ
		key = Protol6009TranceiverWaitUI(CurrCmd, &Addrs, &Args, ackLen, timeout, tryCnt);
		
		
		// ���� / ����
		if (key == KEY_CANCEL){
			break;
		}else{
			isUiFinish = false;
			continue;
		}
	}

	_CloseCom();
}

// �ط�
void MainFuncCloseValve(void)
{
	uint8 key, tryCnt = 0, i;
	UI_Item * pUi = &UiList.items[0];
	uint8 * pUiCnt = &UiList.cnt;
	uint8 currUi = 0, uiRowIdx, isUiFinish;
	uint16 ackLen = 0, timeout;

	_CloseCom();
	_ComSetTran(CurrPort);
	_ComSet(CurrBaud, 2);

	memset(StrBuf, 0, TXTBUF_LEN * 10);
	isUiFinish = false;

	while(1){
		
		_ClearScreen();

		// �������� :  ������ʾ
		_Printfxy(0, 0, "<<�ط�", Color_White);
		_GUIHLine(0, 1*16 + 4, 160, Color_Black);	
		/*---------------------------------------------*/
		//----------------------------------------------
		_GUIHLine(0, 9*16 - 4, 160, Color_Black);
		_Printfxy(0, 9*16, "���� <�ȴ�����> ִ��", Color_White);

		if(false == isUiFinish){
			(*pUiCnt) = 0;
			uiRowIdx = 2;
			#if (AddrLen == 6)
			TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�� ��:", StrDstAddr, 12, 13*8, true);
			#else
			LableCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�� ��:");
			TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "   ", StrDstAddr, 16, 17*8, true);	
			#endif
		}
		
		// �����������
		i = 0;	
		Args.itemCnt = 2;
		Args.items[0] = &Args.buf[0];   // ������
		Args.items[1] = &Args.buf[1];	// ������
		CurrCmd = WaterCmd_CloseValve;		// " �ط� ";
		
		switch(CurrCmd){
		case WaterCmd_CloseValve:		// " �ط� ";
			/*---------------------------------------------*/
			if(false == isUiFinish){
				break;
			}
			Args.buf[i++] = 0x03;		// ������	03
			ackLen = 3;					// Ӧ�𳤶� 3	
			// ������
			Args.buf[i++] = 0x00;		// ǿ�Ʊ�ʶ 	0 - ��ǿ�ƣ� 1 - ǿ��
			Args.buf[i++] = 0x00;		// ���ط���ʶ	0 - �ط��� 1 - ����
			Args.lastItemLen = i - 1;
			break;

		default: 
			break;
		}

		// ���� ���м̵�ַ����� �� ��ʾUI
		if(false == isUiFinish){

			key = ShowUI(UiList, &currUi);

			if (key == KEY_CANCEL){
				break;
			}

			if(StrDstAddr[0] < '0' || StrDstAddr[0] > '9'  ){
				sprintf(StrDstAddr, " ������");
				currUi = 0;
				continue;
			}

			isUiFinish = true;
			continue;	// go back to get ui args
		}

		// ��ַ���
		Water6009_PackAddrs(&Addrs, StrDstAddr, StrRelayAddr);
		#if (AddrLen == 6)
		PrintfXyMultiLine_VaList(0, 2*16, "�� ��: %s", StrDstAddr);
		#else
		PrintfXyMultiLine_VaList(0, 2*16, "�� ��:\n   %s", StrDstAddr);
		#endif

		// Ӧ�𳤶ȡ���ʱʱ�䡢�ط�����
		ackLen += 14 + Addrs.itemCnt * AddrLen;
		timeout = 8000 + (Addrs.itemCnt - 2) * 6000 * 2;
		tryCnt = 3;

		// ���͡����ա������ʾ
		key = Protol6009TranceiverWaitUI(CurrCmd, &Addrs, &Args, ackLen, timeout, tryCnt);
		
		
		// ���� / ����
		if (key == KEY_CANCEL){
			break;
		}else{
			isUiFinish = false;
			continue;
		}
	}

	_CloseCom();
}

// --------------------------------   ������   -----------------------------------------------
int main(void)
{
	_GuiMenuStru MainMenu;
	int fp;

	fp = _Fopen("system.cfg", "R");
#ifdef Project_6009_RF
	_Lseek(fp, 0, 0);	// byte [0 ~ 19] 12λ��� 
#else
	_Lseek(fp, 40, 0);	// byte [40 ~ 59] 16λ��� 
#endif
	_Fread(StrDstAddr, TXTBUF_LEN, fp);
	_Fclose(fp);
	
	MainMenu.left=0;
	MainMenu.top=0;
	MainMenu.no=8;
	MainMenu.title =  "     ɣ���ֳֻ�    ";
	MainMenu.str[0] = " ��ȡ�û����� ";
	MainMenu.str[1] = " ��ȡ�������� ";
	MainMenu.str[2] = " ��ȡ���ʱ�� ";
	MainMenu.str[3] = " ���ñ��ʱ�� ";
	MainMenu.str[4] = " ���� ";
	MainMenu.str[5] = " �ط� ";
    MainMenu.str[6] = " ���쳣 ";
	MainMenu.str[7] = " ���̵��� ";
	MainMenu.key[0] = "1";
	MainMenu.key[1] = "2";
	MainMenu.key[2] = "3";
	MainMenu.key[3] = "4";
	MainMenu.key[4] = "5";
	MainMenu.key[5] = "6";
	MainMenu.key[6] = "7";
	MainMenu.key[7] = "8";
	MainMenu.Function[0] = MainFuncReadRealTimeData;
	MainMenu.Function[1] = MainFuncReadFrozenData;
	MainMenu.Function[2] = MainFuncReadMeterTime;
	MainMenu.Function[3] = MainFuncSetMeterTime;
	MainMenu.Function[4] = MainFuncOpenValve;
	MainMenu.Function[5] = MainFuncCloseValve;
    MainMenu.Function[6] = MainFuncClearException;
	MainMenu.Function[7] = WaterCmdFunc;	// ���̵��� --> ��ԭ���� ��˲���
	MainMenu.FunctionEx=0;
	_OpenLcdBackLight();
	_Menu(&MainMenu);	
}

