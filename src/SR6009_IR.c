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


// --------------------------------  ˮ��ģ��ͨ��  -----------------------------------------

// 1	��������
void WaterCmdFunc_CommonCmd(void)
{
	uint8 key, menuItemNo, tryCnt = 0, i;
	ListBox menuList;
	UI_Item * pUi = &UiList.items[0];
	uint8 * pUiCnt = &UiList.cnt;
	uint8 * pByte;
	uint8 currUi = 0, uiRowIdx, isUiFinish;
	uint16 ackLen = 0, timeout, u16Tmp;
	uint32 u32Tmp;
	char strDstAddrTmp[20];

	_ClearScreen();

	// �˵�
	ListBoxCreate(&menuList, 0, 0, 20, 7, 6, NULL,
		"<<��������",
		6,
		"1. ���ۼ�����",
		"2. ����������",
		"3. ������������ϵ��",
		"4. ����",
		"5. �ط�",
		"6. ���쳣"
	);

	while(1){

		_Printfxy(0, 9*16, "����            ȷ��", Color_White);
		key = ShowListBox(&menuList);
		//------------------------------------------------------------
		if (key == KEY_CANCEL){	// ����
			break;
		}
		menuItemNo = menuList.strIdx + 1;

		memset(StrBuf, 0, TXTBUF_LEN * TXTBUF_MAX);
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
				if(menuItemNo == 1){ // ������ʱ
					sprintf(strDstAddrTmp, "D4D4D4D4D4D4D4D4");	// ��ʼֵΪ�㲥��ַ
				}
				else{
					sprintf(strDstAddrTmp, StrDstAddr);	
				}
				#if (AddrLen == 6)
				TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�� ��:", strDstAddrTmp, 12, 13*8, true);
				#else
				LableCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�� ��:");
				TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "   ", strDstAddrTmp, 16, 17*8, true);	
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
				CurrCmd = WaterCmd_SetBaseValPulseRatio;	// �����������ϵ��
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

				if( strcmp(strDstAddrTmp, "D4D4D4D4D4D4D4D4") != 0 
					&& (strDstAddrTmp[0] < '0' || strDstAddrTmp[0] > '9'))
				{
					sprintf(strDstAddrTmp, " ������");
					currUi = 0;
					continue;
				}

				isUiFinish = true;
				continue;	// go back to get ui args
			}

			memcpy(StrDstAddr, strDstAddrTmp, 20);

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
	ListBox menuList;
	UI_Item * pUi = &UiList.items[0];
	uint8 * pUiCnt = &UiList.cnt;
	uint8 * pByte;
	uint8 currUi = 0, uiRowIdx, isUiFinish;
	uint16 ackLen = 0, timeout, u16Tmp;
	uint32 u32Tmp;

	_ClearScreen();

	// �˵�
	ListBoxCreate(&menuList, 0, 0, 20, 7, 6, NULL,
		"<<��������",
		6,
		"1. ��debug��Ϣ",
		"2. ��debug��Ϣ",
		"3. ��������",
		"4. �����¶�",
		"5. ������ѹ",
		"6. ���÷��ز���"
	);


	while(1){

		_Printfxy(0, 9*16, "����            ȷ��", Color_White);
		key = ShowListBox(&menuList);
		//------------------------------------------------------------
		if (key == KEY_CANCEL){	// ����
			break;
		}
		menuItemNo = menuList.strIdx + 1;

		memset(StrBuf, 0, TXTBUF_LEN * TXTBUF_MAX);
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
				CurrCmd = WaterCmd_RebootDevice;			// "��������"
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
				CurrCmd = WaterCmd_ReadVoltage;				// " ������ѹ "
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
			
			case WaterCmd_SetMeterNumber:		// " ���ñ��� ";
				/*---------------------------------------------*/
				if(false == isUiFinish){
					#if (AddrLen == 6)
					TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�±���:", StrBuf[0], 12, 13*8, true);
					#else
					LableCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�±���:");
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

				// ����ȡ���汾�ţ������ñ���
				if(CmdResult_Ok == Protol6009Tranceiver(WaterCmd_ReadMeterCfgInfo, &Addrs, &Args, ackLen, timeout, tryCnt)){
					i = 0;
					Args.buf[i++] = 0x07;		// ������	07
					ackLen = 1;					// Ӧ�𳤶� 1	
					// ������
					Args.buf[i++] = 0x10;		// ����ѡ�� 10	
					memcpy(&Args.buf[i], &VerInfo[0], 40);	
					i += 40;					// �����汾��
					GetBytesFromStringHex(&Args.buf[i], 0, 6, StrBuf[0], 0, false);
					i += 6;						// �µ�ַ
					Args.lastItemLen = i - 1;

					if(CmdResult_Ok == Protol6009Tranceiver(CurrCmd, &Addrs, &Args, ackLen, timeout, tryCnt)){
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
	ListBox menuList;
	UI_Item * pUi = &UiList.items[0];
	uint8 * pUiCnt = &UiList.cnt;
	uint8 * pByte;
	uint8 currUi = 0, uiRowIdx, isUiFinish;
	uint16 ackLen = 0, timeout;

	_ClearScreen();

	// �˵�
	(*pUiCnt) = 0;
	uiRowIdx = 2;
	_Printfxy(0, 0, "<<��������", Color_White);
	_GUIHLine(0, 1*16 + 4, 160, Color_Black);	
	/*---------------------------------------------*/
	ButtonCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "1. ѡ�������ļ�");	
	LableCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "2. �������");	
	TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "  > ", StrBuf[0], 1, 2*8, true);
	ButtonCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "3. ��ʼ����");	
	ButtonCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "4. ��ѯ����״̬");				
	//----------------------------------------------
	_GUIHLine(0, 9*16 - 4, 160, Color_Black);
	_Printfxy(0, 9*16, "����            ȷ��", Color_White);

	while(1){

		_Printfxy(0, 0, "<<��������", Color_White);
		_Printfxy(0, 9*16, "����            ȷ��", Color_White);
		key = ShowUI(UiList, &currUi);
		//------------------------------------------------------------
		if (key == KEY_CANCEL){	// ����
			break;
		}

		memset(StrBuf, 0, TXTBUF_LEN * TXTBUF_MAX);
		isUiFinish = false;

		switch (currUi)
		{
		case 0:	// ѡ�������ļ�
			{


			}
			break;

		case 3:	// ��ʼ����
			{

			}
			break;

		case 4:	// ��ѯ����״̬
			{

			}
			break;
		
		case 2:	// �������
		default:
			if(StrDstAddr[0] < '0' || StrDstAddr[0] > '9' ){
				sprintf(StrDstAddr, " ������");
				currUi = 2;
				continue;
			}

			break;
		}

		while(1){
			
			_ClearScreen();

			_Printfxy(0, 0, "<<��ʼ����", Color_White);
			_GUIHLine(0, 1*16 + 4, 160, Color_Black);	
			/*---------------------------------------------*/
			PrintfXyMultiLine_VaList(0, 2*16, "�汾��%s  CRC��%s");
			PrintfXyMultiLine_VaList(0, 3*16, "���ţ�\r\n  %s");
			PrintfXyMultiLine_VaList(0, 5*16, "%s");	// cmd name
			PrintfXyMultiLine_VaList(0, 6*16, "%s");	// cmd reponse
			//----------------------------------------------
			_GUIHLine(0, 9*16 - 4, 160, Color_Black);
			_Printfxy(0, 9*16, "����   <������>   ", Color_White);

			
			// �����������
			i = 0;	
			Args.itemCnt = 2;
			Args.items[0] = &Args.buf[0];   // ������
			Args.items[1] = &Args.buf[1];	// ������
			CurrCmd = (0x30 + menuItemNo);

			switch(CurrCmd){
			case WaterCmd_NoticeUpgrade_OnApp:		// ֪ͨϵͳ����_��app
				/*---------------------------------------------*/
                Args.buf[i++] = 0x70;		// ������	70
				ackLen = 41;				// Ӧ�𳤶� 41	
				// ������
				Args.buf[i++] = 0x00;		// ˮ����ѹ
				Args.buf[i++] = 0x00;		// ������ѹ
				Args.buf[i++] = 0x00;		// RSSI����
				Args.buf[i++] = 0x00;		// SNR����
				Args.buf[i++] = 0x00;		// ����汾 40 byte
				Args.buf[i++] = 0x00;		// �汾CRC16
				Args.buf[i++] = 0x00;		
				Args.buf[i++] = 0x00;		// �ܰ���
				Args.buf[i++] = 0x00;
				Args.buf[i++] = 0x00;		// ǰ26K CRC16
				Args.buf[i++] = 0x00;	
				Args.buf[i++] = 0x00;		// ��26K CRC16
				Args.buf[i++] = 0x00;	
				Args.buf[i++] = 0x00;		// ��52K CRC16
				Args.buf[i++] = 0x00;	
				Args.buf[i++] = 0x00;		// RXD�ŵ� 0/1
				Args.lastItemLen = i - 1;
				break;

			case WaterCmd_NoticeUpgrade_OnBoot:		// ֪ͨϵͳ����_��boot
				/*---------------------------------------------*/
				Args.buf[i++] = 0x71;		// ������	71
				ackLen = 41;				// Ӧ�𳤶� 41	
				// ������
				Args.buf[i++] = 0x00;	
				Args.lastItemLen = i - 1;
				break;

			case WaterCmd_SendUpgradePacket:			// ������������
				/*---------------------------------------------*/
				Args.buf[i++] = 0x72;		// ������	72
				ackLen = 0;					// Ӧ�𳤶� 0	
				// ������
				Args.buf[i++] = 0x00;	
				Args.lastItemLen = i - 1;
				break;
			
			case WaterCmd_QueryUpgradeStatus_OnBoot:	// ��ѯ����״̬_��boot
				/*---------------------------------------------*/
				Args.buf[i++] = 0x73;		// ������	73
				ackLen = 93;				// Ӧ�𳤶� 93	
				// ������
				Args.buf[i++] = 0x00;	
				Args.lastItemLen = i - 1;
				break;

            case WaterCmd_QueryUpgradeStatus_OnApp:		// ��ѯ����״̬_��app
				/*---------------------------------------------*/
				Args.buf[i++] = 0x74;		// ������	74
				ackLen = 41;				// Ӧ�𳤶� 41	
				// ������
				Args.buf[i++] = 0x00;	
				Args.lastItemLen = i - 1;
				break;

			default: 
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

}

// 4	Ԥ������
void WaterCmdFunc_PrepaiedVal(void)
{
	uint8 key, menuItemNo, tryCnt = 0, i;
	ListBox menuList;
	UI_Item * pUi = &UiList.items[0];
	uint8 * pUiCnt = &UiList.cnt;
	uint8 * pByte;
	uint8 currUi = 0, uiRowIdx, isUiFinish;
	uint16 ackLen = 0, timeout, u16Tmp;
	uint32 u32Tmp;

	_ClearScreen();

	// �˵�`
	ListBoxCreate(&menuList, 0, 0, 20, 7, 6, NULL,
		"<<Ԥ������",
		6,
		"1. ��Ԥ�ɲο�����",
		"2. ��Ԥ�ɲο�����",
		"3. �������ط���ֵ",
		"4. �豨����ֵ",
		"5. ��ط���ֵ",
		"6. �豨���ط���ֵ"
	);

	while(1){

		_Printfxy(0, 9*16, "����            ȷ��", Color_White);
		key = ShowListBox(&menuList);
		//------------------------------------------------------------
		if (key == KEY_CANCEL){	// ����
			break;
		}
		menuItemNo = menuList.strIdx + 1;

		memset(StrBuf, 0, TXTBUF_LEN * TXTBUF_MAX);
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
}


// 5	��������
void WaterCmdFunc_WorkingParams(void)
{
	uint8 key, menuItemNo, tryCnt = 0, i;
	ListBox menuList;
	UI_Item * pUi = &UiList.items[0];
	uint8 * pUiCnt = &UiList.cnt;
	uint8 * pByte, *time = &TmpBuf[200], *timeBytes = &TmpBuf[300];
	uint8 currUi = 0, uiRowIdx, isUiFinish;
	uint16 ackLen = 0, timeout, enableStatus, u16Tmp;
	uint32 port, u32Tmp, u32Args[20];
	uint8 ip[4], u8Tmp;

	_ClearScreen();

	ListBoxCreate(&menuList, 0, 0, 20, 7, 17, NULL,		
		"<<��������",
		17,
		"1. ����IP+�˿�+ģʽ",
		"2. ��ȡIP+�˿�+ģʽ",
		"3. ��ȡ��Ӫ�̱��",
		"4. ��ȡIMEI+ICCID",
		"5. �����ת��������",
		"6. ��ȡ����ʹ��״̬",
		"7. ���ö�ʱ�ϴ�",
		"8. ���ö����ϴ�",
		"9. ���ö�ʱ�����ϴ�",
		"10.������ʱ��",
		"11.У����ʱ��",
		"12.���շ����ŷ�����",
		"13.��ȡģ�����в���",		
		"14.����ģ�����в���",
		"15.��ȡNB������Ϣ",
		"16.��ȡ����ˮ������",		
		"17.���ñ���ˮ������"	
	);

	while(1){

		_Printfxy(0, 9*16, "����            ȷ��", Color_White);
		key = ShowListBox(&menuList);
		//------------------------------------------------------------
		if (key == KEY_CANCEL){	// ����
			break;
		}
		menuItemNo = menuList.strIdx + 1;

		memset(StrBuf, 0, TXTBUF_LEN * TXTBUF_MAX);
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
					if(StrBuf[0][0] > 1){
						StrBuf[0][0] = 0;
					}
                    CombBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "����ģʽ:", &StrBuf[0][0], 2, 
						"Coap", "Udp");
					TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx)*16, "  IP:", StrBuf[1], 3, 3*8, true);
					TextBoxCreate(&pUi[(*pUiCnt)++], 8*8, (uiRowIdx)*16, ".", StrBuf[2], 3, 3*8, true);
					TextBoxCreate(&pUi[(*pUiCnt)++], 12*8, (uiRowIdx)*16, ".", StrBuf[3], 3, 3*8, true);
					TextBoxCreate(&pUi[(*pUiCnt)++], 16*8, (uiRowIdx)*16, ".", StrBuf[4], 3, 3*8, true);
					uiRowIdx++;
					TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "Port:", StrBuf[5], 5, 6*8, true);
					break;
				}
				// ip
				if((i = IpStrsToIpBytes(ip, StrBuf[1], StrBuf[2], StrBuf[3], StrBuf[4])) > 0){
					currUi = 2 + i;
					isUiFinish = false;
					continue;
				}
				// port
				u32Tmp = (uint32)_atof(StrBuf[5]);
				if(StrBuf[5][0] < '0' || StrBuf[5][0] > '9' || u32Tmp > 65535){
					currUi = 7;
					isUiFinish = false;
					continue;
				}

				i = 0;
				Args.buf[i++] = 0x0D;		// ������	0D
				ackLen = 2;					// Ӧ�𳤶� 2	
				// ������
				Args.buf[i++] = 0x01;		// ����ѡ�0-��ȡ�� 1-����
				Args.buf[i++] = (0xA0 + (uint8)StrBuf[0][0]);	// ģʽ��A0-coap, A1-udp
				Args.buf[i++] = (uint8)(ip[0] & 0xFF);	// IP
				Args.buf[i++] = (uint8)(ip[1] & 0xFF);	
				Args.buf[i++] = (uint8)(ip[2] & 0xFF);		
				Args.buf[i++] = (uint8)(ip[3] & 0xFF);	
				Args.buf[i++] = (uint8)(u32Tmp & 0xFF);	// Port	
				Args.buf[i++] = (uint8)((u32Tmp >> 8) & 0xFF);
				Args.lastItemLen = i - 1;
				break;

			case 2: 
				CurrCmd = WaterCmd_ReadIpPortMode;			// ��IP+�˿�+ģʽ
				/*---------------------------------------------*/
				if(false == isUiFinish){
					break;
				}
				Args.buf[i++] = 0x0D;		// ������	0D
				ackLen = 9;					// Ӧ�𳤶� 9	
				// ������
				Args.buf[i++] = 0x00;		// ����ѡ�0-��ȡ�� 1-����
				Args.lastItemLen = i - 1;
				break;
				
			case 3: 
				CurrCmd = WaterCmd_ReadNbOperaterNumber;		// " ��NB��Ӫ�̱�� ";
				/*---------------------------------------------*/
				if(false == isUiFinish){
					break;
				}
				Args.buf[i++] = 0x0E;		// ������	0E
				ackLen = 7;					// Ӧ�𳤶� 7	
				// ������
				Args.buf[i++] = 0;			// ����ѡ�� 0-��ȡ�� 1-����	
				Args.lastItemLen = i - 1;
				break;

			case 4: 
				CurrCmd = WaterCmd_ReadImeiAndCcid;			// ��IMEI+ICCID
				/*---------------------------------------------*/
				Args.buf[i++] = 0x0F;		// ������	0F
				ackLen = 22;				// Ӧ�𳤶� 12	
				// ������
				Args.buf[i++] = 0;			// ����ϵ��	0-��ȡIMEI+ICCID��1-����IMEI�� 2-����ICCID
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
				CurrCmd = WaterCmd_ReadMeterTime;			// ������ʱ��
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
				CurrCmd = WaterCmd_SetMeterTime;			// У����ʱ��
				/*---------------------------------------------*/
				if(false == isUiFinish){
					_GetDateTime(time, '-',  ':');
					DatetimeToTimeStrs(time, StrBuf[0], StrBuf[1], StrBuf[2], StrBuf[3], StrBuf[4], StrBuf[5]);
					
					LableCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "ʱ ��:");
					TextBoxCreate(&pUi[(*pUiCnt)++], 0*8, (uiRowIdx)*16, " ", StrBuf[0], 4, 4*8, false);	// YYYY
					TextBoxCreate(&pUi[(*pUiCnt)++], 5*8, (uiRowIdx)*16, "-", StrBuf[1], 2, 2*8, false);	// MM
					TextBoxCreate(&pUi[(*pUiCnt)++], 8*8, (uiRowIdx)*16, "-", StrBuf[2], 2, 2*8, false);	// dd
					TextBoxCreate(&pUi[(*pUiCnt)++], 11*8, (uiRowIdx)*16, " ", StrBuf[3], 2, 2*8, false);	// HH
					TextBoxCreate(&pUi[(*pUiCnt)++], 14*8, (uiRowIdx)*16, ":", StrBuf[4], 2, 2*8, false);	// mm
					TextBoxCreate(&pUi[(*pUiCnt)++], 17*8, (uiRowIdx++)*16, ":", StrBuf[5], 2, 2*8, false);	// ss
					break;
				}
				// ʱ����ЧֵУ��
				if( (i = TimeStrsToTimeBytes(timeBytes, StrBuf[0], StrBuf[1], StrBuf[2], StrBuf[3], StrBuf[4], StrBuf[5])) > 0){
					currUi = 3 + (i -1);
					isUiFinish = false;
					continue;
				}

				sprintf(time, "%s-%s-%s %s:%s:%s",
					StrBuf[0], StrBuf[1], StrBuf[2], StrBuf[3], StrBuf[4], StrBuf[5]);
				_SetDateTime(time);

				i = 0;
				Args.buf[i++] = 0x14;		// ������	14
				ackLen = 2;					// Ӧ�𳤶� 2	
				// ������
				Args.buf[i++] = DecToBcd(timeBytes[0]);		// ʱ�� - yyyy/mm/dd HH:mm:ss
				Args.buf[i++] = DecToBcd(timeBytes[1]);		
				Args.buf[i++] = DecToBcd(timeBytes[2]);		
				Args.buf[i++] = DecToBcd(timeBytes[3]);			
				Args.buf[i++] = DecToBcd(timeBytes[4]);			
				Args.buf[i++] = DecToBcd(timeBytes[5]);			
				Args.buf[i++] = DecToBcd(timeBytes[6]);	
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

			case 13: 
				CurrCmd = WaterCmd_ReadModuleRunningParams;		// ��ȡģ�����в���
				/*---------------------------------------------*/
				if(false == isUiFinish){
					break;
				}
				Args.buf[i++] = 0x3A;		// ������	3A
				ackLen = 124;				// Ӧ�𳤶� 124	
				Args.lastItemLen = i - 1; 
				break;

			case 14: 
				CurrCmd = WaterCmd_SetModuleRunningParams;		// ����ģ�����в���
				/*---------------------------------------------*/
				// UI-��1ҳ
				if(false == isUiFinish){
					StrBuf[0][0] = 0;
					StrBuf[0][1] = 0;
					StrBuf[0][2] = 1;
					_Printfxy(0, 9*16, "���� <�ȴ�����> ����", Color_White);
                    CombBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "����:", &StrBuf[0][0], 9, 
						"RF��ˮ��", "GPRS��ˮ��", "NB��ˮ��", 
						"RF��ˮ��", "NB��ˮ��", 
						"RFȼ����", "GPRSȼ����", "NBȼ����", 
						"���");
					CombBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "���:", &StrBuf[0][1], 3, 
						"3.6v", "6v", "4.5v");
					LableCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "����������:");
					CombBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "", &StrBuf[0][2], 13, 
						"���ɻɹ�/����", "˫�ɻɹ�/����", "���ɻɹ�/����", "����4λ���ֱ��", 
						"��Ĥֱ��", "����1λ���ֱ��", "188Э����ֱ��", "188Э���޴�ֱ��",
						"����������", "�����޴�", "ɽ���޴�", "�����޴�",
						"�����޴�");
					break;
				}
				// UI-��2ҳ
				currUi = 0;
				StrBuf[0][3] = 1;
				StrBuf[0][4] = 1;
				sprintf(StrBuf[1], "121");
				sprintf(StrBuf[2], "43");
				sprintf(StrBuf[3], "175");
				sprintf(StrBuf[4], "222");
				sprintf(StrBuf[5], "5683");
				while(1){
					_Printfxy(0, 9*16, "���� <�ȴ�����> ����", Color_White);
					(*pUiCnt) = 0;
					uiRowIdx = 2;
					CombBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "����ϵ��:", &StrBuf[0][3], 4, 
						"1", "10", "100", "1000");
					TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "��ת����:", StrBuf[6], 10, 10*8, true);
					pUi[(*pUiCnt) -1].ui.txtbox.dotEnable = 1;
					CombBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "NB���ӷ�ʽ:", &StrBuf[0][4], 2, 
						"Coap", "Udp");
					TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx)*16, "  IP:", StrBuf[1], 3, 3*8, true);
					TextBoxCreate(&pUi[(*pUiCnt)++], 8*8, (uiRowIdx)*16, ".", StrBuf[2], 3, 3*8, true);
					TextBoxCreate(&pUi[(*pUiCnt)++], 12*8, (uiRowIdx)*16, ".", StrBuf[3], 3, 3*8, true);
					TextBoxCreate(&pUi[(*pUiCnt)++], 16*8, (uiRowIdx)*16, ".", StrBuf[4], 3, 3*8, true);
					uiRowIdx++;
					TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "Port:", StrBuf[5], 5, 6*8, true);

					key = ShowUI(UiList, &currUi);
					
					if (key == KEY_CANCEL){
						break;
					}
					isUiFinish = true;

					// ��ת����
					if(StrBuf[6][0] > '9' || StrBuf[6][0] < '0'){
						currUi = 1;
						isUiFinish = false;
						continue;
					}
					u32Tmp = (uint32) _atof(StrBuf[6]);
					u16Tmp = (uint16)((float)((_atof(StrBuf[6]) - u32Tmp)*1000.0));
					
					// ip
					if((i = IpStrsToIpBytes(ip, StrBuf[1], StrBuf[2], StrBuf[3], StrBuf[4])) > 0){
						currUi = 2 + i;
						isUiFinish = false;
						continue;
					}
					// port
					port = (uint32)_atof(StrBuf[5]);
					if(StrBuf[5][0] < '0' || StrBuf[5][0] > '9' || port > 65535){
						currUi = 7;
						isUiFinish = false;
						continue;
					}

					if(isUiFinish){
						break;
					}
				}
				if (key == KEY_CANCEL){
					break;
				}

				// UI-��3ҳ
				currUi = 0;
				StrBuf[0][5] = 0;
				StrBuf[0][6] = 0;
				StrBuf[0][7] = 1;
				StrBuf[0][8] = 1;
				StrBuf[0][9] = 0;
				StrBuf[0][10] = 0;
				while(1){
					_GUIRectangleFill(0, 2*16, 160, 8*16, Color_White);
					_Printfxy(0, 9*16, "���� <�ȴ�����> ִ��", Color_White);
					(*pUiCnt) = 0;
					uiRowIdx = 2;
					CombBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "���Źط�:", &StrBuf[0][5], 2, 
						"�ر�", "����");
					CombBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "������:", &StrBuf[0][6], 2, 
						"�ر�", "����");
					CombBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�����澯:", &StrBuf[0][7], 2, 
						"�ر�", "����");
					CombBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�����ϱ�:", &StrBuf[0][8], 2, 
						"�ر�", "����");
					CombBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "���ŷ���:", &StrBuf[0][9], 2, 
						"�ر�", "����");
					CombBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "����ط�:", &StrBuf[0][10], 2, 
						"�ر�", "����");
					
					key = ShowUI(UiList, &currUi);
					
					if (key == KEY_CANCEL){
						break;
					}
					isUiFinish = true;

					// ʹ��״̬
					enableStatus = 0;
					enableStatus |= (StrBuf[0][5] == 0 ? 0x0000 : 0x0001);	// ���Źط�
					enableStatus |= (StrBuf[0][6] == 0 ? 0x0000 : 0x0002);	// ������
					enableStatus |= (StrBuf[0][7] == 0 ? 0x0000 : 0x0010);	// �����澯
					enableStatus |= (StrBuf[0][8] == 0 ? 0x0000 : 0x0020);	// �����ϱ�
					enableStatus |= (StrBuf[0][9] == 0 ? 0x0000 : 0x0200);	// ���ŷ���
					enableStatus |= (StrBuf[0][10] == 0 ? 0x0000 : 0x0400);	// ����ط�
					

					if(isUiFinish){
						break;
					}
				}
				if (key == KEY_CANCEL){
					break;
				}
				
				i = 0;
				Args.buf[i++] = 0x3F;		// ������	3F
				ackLen = 1;					// Ӧ�𳤶� 1	
				// ������
				switch ((uint8)StrBuf[0][0]){			// ������
				case 0: u8Tmp = 0x10; break;
				case 1: u8Tmp = 0x11; break;
				case 2: u8Tmp = 0x12; break;
				case 3: u8Tmp = 0x20; break;
				case 4: u8Tmp = 0x21; break;
				case 5: u8Tmp = 0x30; break;
				case 6: u8Tmp = 0x31; break;
				case 7: u8Tmp = 0x32; break;
				case 8: u8Tmp = 0x40; break;
				default: u8Tmp = 0x10; break;
				}

				Args.buf[i++] = u8Tmp;	
				Args.buf[i++] = (uint8)StrBuf[0][3];	// ����ϵ��	
				Args.buf[i++] = 50;			// �Ÿ��ſ���ʱ��
				Args.buf[i++] = 100;		// ����������С�պ�ʱ��
				Args.buf[i++] = (uint8)(12000 & 0xFF);	// ���ط�ʱ�� 12000
				Args.buf[i++] = (uint8)(12000 >> 8);		
				Args.buf[i++] = 150;		// ������ֵ
				Args.buf[i++] = (uint8)StrBuf[0][1];	// ��ص�ѹ����
				Args.buf[i++] = 120;		// ��ʱ�ϴ�ʱ����	
				Args.buf[i++] = 2;			// �����ϴ��������	
				Args.buf[i++] = 3;		// ÿ�춨ʱ��������ϴ�����	
				Args.buf[i++] = (uint8)StrBuf[0][2];	// ��������������	
				Args.buf[i++] = ip[0];		// IP�ĵ�1~4���ֽ�	121.43.175.22 : 5683
				Args.buf[i++] = ip[1];
				Args.buf[i++] = ip[2];
				Args.buf[i++] = ip[3];
				Args.buf[i++] = (uint8)(port & 0xFF);	// ���������Ӷ˿�
				Args.buf[i++] = (uint8)((port >> 8) & 0xFF);
				Args.buf[i++] = 10;			// ˮ�����ⶨʱ���	
				Args.buf[i++] = 0;			// ϵͳ���Լ���	
				Args.buf[i++] = 0x46;			// ��Ӫ�̱��/SIM������ NB-IOTʱ��46001000
				Args.buf[i++] = 0x00;	
				Args.buf[i++] = 0x10;	
				Args.buf[i++] = 0x00;		
				Args.buf[i++] = DecToBcd((uint8)(_GetYear()/100));	// ����/RF�趨��ϵͳʱ�� - yyyy/mm/dd HH:mm:ss
				Args.buf[i++] = DecToBcd((uint8)(_GetYear()%100));		
				Args.buf[i++] = DecToBcd(_GetMonth());		
				Args.buf[i++] = DecToBcd(_GetDay());			
				Args.buf[i++] = DecToBcd(_GetHour());			
				Args.buf[i++] = DecToBcd(_GetMin());			
				Args.buf[i++] = DecToBcd(_GetSec());
				Args.buf[i++] = 8;			// ͨ��Ƶ��	
				Args.buf[i++] = ((uint8)StrBuf[0][4] + 1);	// NB���ӷ�ʽ	1 - coap ,  2- udp
				Args.buf[i++] = 0;			// Ԥ��	
				Args.buf[i++] = 0;			// Ԥ��
				Args.buf[i++] = 0;			// ʣ������������ֵ	
				Args.buf[i++] = 0;			// ʣ�������ط���ֵ	2 byte
				Args.buf[i++] = 0;	
				Args.buf[i++] = 0;			// Ԥ�ɷ����� 6 byte
				Args.buf[i++] = 0;
				Args.buf[i++] = 0;
				Args.buf[i++] = 0;	
				Args.buf[i++] = 0;
				Args.buf[i++] = 0;		
				Args.buf[i++] = 0;			// �ο���ʼ���� 6 byte
				Args.buf[i++] = 0;
				Args.buf[i++] = 0;
				Args.buf[i++] = 0;	
				Args.buf[i++] = 0;
				Args.buf[i++] = 0;	
				Args.buf[i++] = 0;			// ģ�����״̬	2 byte
				Args.buf[i++] = 0;	
				Args.buf[i++] = 0;			// Ԥ��	
				Args.buf[i++] = 0;			// Ԥ��
				Args.buf[i++] = (uint8)(u32Tmp & 0xFF);		// ��ת����	
				Args.buf[i++] = (uint8)((u32Tmp >> 8) & 0xFF);
				Args.buf[i++] = (uint8)((u32Tmp >> 16) & 0xFF);
				Args.buf[i++] = (uint8)((u32Tmp >> 24) & 0xFF);
				Args.buf[i++] = (uint8)(u16Tmp & 0xFF);		
				Args.buf[i++] = (uint8)((u16Tmp >> 8) & 0xFF);
				Args.buf[i++] = 0;			// ��ת���� 6 byte
				Args.buf[i++] = 0;
				Args.buf[i++] = 0;
				Args.buf[i++] = 0;	
				Args.buf[i++] = 0;
				Args.buf[i++] = 0;
				Args.buf[i++] = 0x00;		// ��״̬
				Args.buf[i++] = (uint8)(enableStatus & 0xFF);	// ����ʹ��״̬
				Args.buf[i++] = (uint8)(enableStatus >> 8);	
				Args.buf[i++] = 0x00;		// ����״̬��־λ
				Args.buf[i++] = 0x00;	
				Args.buf[i++] = 0;			// ���հ�������
				Args.buf[i++] = 0;			// RF������ʼʱ��	
				Args.buf[i++] = 24;			// RF��������ʱ��	
				Args.buf[i++] = 0;			// Ԥ��	10 byte
				for(u8Tmp = 0; u8Tmp < 9; u8Tmp++){
					Args.buf[i++] = 0;
				}
				// ��ȡʱ���أ����� + ģ�������汾�� 40 byte	

				Args.lastItemLen = i - 1;
				break;

			case 15: 
				CurrCmd = WaterCmd_ReadNbJoinNetworkInfo;		// ��ȡNB������Ϣ
				/*---------------------------------------------*/
				if(false == isUiFinish){
					break;
				}
				Args.buf[i++] = 0x10;		// ������	10
				ackLen = 34;				// Ӧ�𳤶� 34	
				// ������
				Args.lastItemLen = i - 1;
				break;

			case 16: 
				CurrCmd = WaterCmd_ReadBeiJingWaterMeterParams;		// ��ȡ����ˮ������
				/*---------------------------------------------*/
				if(false == isUiFinish){
					break;
				}
				Args.buf[i++] = 0x26;		// ������	26
				ackLen = 69;				// Ӧ�𳤶� 69
				// ������
				Args.buf[i++] = 0x00;		// ����ѡ�� 00 - ��ȡ�� 01 - ����		
				Args.lastItemLen = i - 1; 
				break;

			case 17: 
				CurrCmd = WaterCmd_SetBeiJingWaterMeterParams;		// ���ñ���ˮ������
				/*---------------------------------------------*/
				// UI-��1ҳ
				if(false == isUiFinish){
					StrBuf[0][0] = 0;
					StrBuf[1][0] = 0;
					_Printfxy(0, 9*16, "���� <�ȴ�����> ����", Color_White);
					LableCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "ˮ������:");
                    CombBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "  ", &StrBuf[0][0], 5, 
						"����NB�޴�", "����NB�޴�", "ɽ��NB�޴�", 
						"����NB�޴�", "��ԴNB�޴�");
					LableCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "��������:");
					TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "   ", StrBuf[1], 14, 15*8, true);
					break;
				}
				// ˮ������
				u32Args[0] = StrBuf[0][0];
				// ��������
				if(StrBuf[1][0] > '9' || StrBuf[1][0] < '0'){
					currUi = 5;
					isUiFinish = false;
					continue;
				}
				memcpy(&TmpBuf[0], &StrBuf[1][0], 14);
				TmpBuf[14] = 0x00;

				// UI-��2ҳ
				currUi = 0;
				sprintf(StrBuf[1], "40");
				sprintf(StrBuf[2], "1");
				sprintf(StrBuf[3], "40");
				sprintf(StrBuf[4], "1");
				sprintf(StrBuf[5], "10");
				sprintf(StrBuf[6], "14");
				while(1){
					_Printfxy(0, 9*16, "���� <�ȴ�����> ����", Color_White);
					(*pUiCnt) = 0;
					uiRowIdx = 2;
					TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�����澯��ֵ:", StrBuf[1], 6, 6*8, true);
					TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�����澯ʱ��:", StrBuf[2], 3, 3*8, true);
					TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�����澯��ֵ:", StrBuf[3], 6, 6*8, true);
					TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�����澯ʱ��:", StrBuf[4], 3, 3*8, true);
					TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "��ѹ�澯��ֵ:", StrBuf[5], 5, 5*8, true);
					TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx)*16, "�ܼ��ϱ�ʱ���:", StrBuf[6], 2, 2*8, true);
					LableCreate(&pUi[(*pUiCnt)++], 9*16, (uiRowIdx++)*16, "��");

					key = ShowUI(UiList, &currUi);
					
					if (key == KEY_CANCEL){
						break;
					}
					isUiFinish = true;

					// ��Чֵ�ж�
					for(i = 1; i <= 6; i++){
						u32Args[i] = (uint32) _atof(StrBuf[i]);
						if(StrBuf[i][0] < '0' || StrBuf[i][0] > '9'
							|| (i == 2 && u32Args[i] > 255)
							|| (i == 4 && u32Args[i] > 255)
							|| (i == 5 && u32Args[i] > 65535)
							|| (i == 6 && u32Args[i] > 23)
						){
							currUi = i - 1;
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

				// UI-��3ҳ
				currUi = 0;
				StrBuf[0][0] = 1;
				StrBuf[0][1] = 2;
				sprintf(StrBuf[1], "121");
				sprintf(StrBuf[2], "43");
				sprintf(StrBuf[3], "175");
				sprintf(StrBuf[4], "222");
				sprintf(StrBuf[5], "5683");
				memset(StrBuf[6], 0x00, 10);
				sprintf(StrBuf[6], "CMIOT");
				sprintf(StrBuf[7], "1");
				while(1){
					_GUIRectangleFill(0, 2*16, 160, 8*16, Color_White);
					_Printfxy(0, 9*16, "���� <�ȴ�����> ����", Color_White);
					(*pUiCnt) = 0;
					uiRowIdx = 2;
					CombBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�ն�����:", &StrBuf[0][0], 2, 
						"ͣ��", "����");
					TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx)*16, "  IP:", StrBuf[1], 3, 3*8, true);
					TextBoxCreate(&pUi[(*pUiCnt)++], 8*8, (uiRowIdx)*16, ".", StrBuf[2], 3, 3*8, true);
					TextBoxCreate(&pUi[(*pUiCnt)++], 12*8, (uiRowIdx)*16, ".", StrBuf[3], 3, 3*8, true);
					TextBoxCreate(&pUi[(*pUiCnt)++], 16*8, (uiRowIdx)*16, ".", StrBuf[4], 3, 3*8, true);
					uiRowIdx++;
					TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "Port:", StrBuf[5], 5, 6*8, true);
					TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, " APN:", StrBuf[6], 6, 7*8, true);
					pUi[(*pUiCnt) -1].ui.txtbox.isInputAny = 1;
					CombBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�ϱ���������:", &StrBuf[0][1], 5, 
						"0", "1", "2", "3", "4");
					TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx)*16, "���ڲ������:", StrBuf[7], 2, 2*8, true);
					LableCreate(&pUi[(*pUiCnt)++], 8*16, (uiRowIdx++)*16, "x30'");

					key = ShowUI(UiList, &currUi);
					
					if (key == KEY_CANCEL){
						break;
					}
					isUiFinish = true;

					// �ն�����
					u32Args[7] = StrBuf[0][0];
					// ip
					if((i = IpStrsToIpBytes(ip, StrBuf[1], StrBuf[2], StrBuf[3], StrBuf[4])) > 0){
						currUi = 2 + i;
						isUiFinish = false;
						continue;
					}
					// port
					port = (uint32)_atof(StrBuf[5]);
					if(StrBuf[5][0] < '0' || StrBuf[5][0] > '9' || port > 65535){
						currUi = 7;
						isUiFinish = false;
						continue;
					}
					// APN
					memcpy(&TmpBuf[20], &StrBuf[6][0], 6);
					// �ϱ���������
					u32Args[8] = StrBuf[0][1];

					// ���ڲ������
					u32Args[9] = (uint32) _atof(StrBuf[7]);
					if(StrBuf[7][0] > '9' || StrBuf[7][0] < '0' || u32Args[9] < 1 || u32Args[9] > 48){
						currUi = 8;
						isUiFinish = false;
						continue;
					}

					if(isUiFinish){
						break;
					}
				}
				if (key == KEY_CANCEL){
					break;
				}

				// UI-��4ҳ
				currUi = 0;
				for(i = 0; i < 14; i++){
					StrBuf[i][0] = 0x00;
				}
				sprintf(StrBuf[13], "24");
				while(1){
					_GUIRectangleFill(0, 2*16, 160, 8*16, Color_White);
					_Printfxy(0, 9*16, "���� <�ȴ�����> ִ��", Color_White);
					(*pUiCnt) = 0;
					uiRowIdx = 2;

					_GetDateTime(time, '-',  ':');
					DatetimeToTimeStrs(time, StrBuf[1], StrBuf[2], StrBuf[3], StrBuf[4], StrBuf[5], StrBuf[6]);
					LableCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�����ϱ���ʼʱ��:");
					TextBoxCreate(&pUi[(*pUiCnt)++], 0*8, (uiRowIdx)*16, " ", StrBuf[1], 4, 4*8, false);	// YYYY
					TextBoxCreate(&pUi[(*pUiCnt)++], 5*8, (uiRowIdx)*16, "-", StrBuf[2], 2, 2*8, false);	// MM
					TextBoxCreate(&pUi[(*pUiCnt)++], 8*8, (uiRowIdx)*16, "-", StrBuf[3], 2, 2*8, false);	// dd
					TextBoxCreate(&pUi[(*pUiCnt)++], 11*8, (uiRowIdx)*16, " ", StrBuf[4], 2, 2*8, false);	// HH
					TextBoxCreate(&pUi[(*pUiCnt)++], 14*8, (uiRowIdx)*16, ":", StrBuf[5], 2, 2*8, false);	// mm
					TextBoxCreate(&pUi[(*pUiCnt)++], 17*8, (uiRowIdx++)*16, ":", StrBuf[6], 2, 2*8, false);	// ss

					_GetDateTime(time, '-',  ':');
					DatetimeToTimeStrs(time, StrBuf[7], StrBuf[8], StrBuf[9], StrBuf[10], StrBuf[11], StrBuf[12]);
					LableCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�����ϱ�����ʱ��:");
					TextBoxCreate(&pUi[(*pUiCnt)++], 0*8, (uiRowIdx)*16, " ", StrBuf[7], 4, 4*8, false);	// YYYY
					TextBoxCreate(&pUi[(*pUiCnt)++], 5*8, (uiRowIdx)*16, "-", StrBuf[8], 2, 2*8, false);	// MM
					TextBoxCreate(&pUi[(*pUiCnt)++], 8*8, (uiRowIdx)*16, "-", StrBuf[9], 2, 2*8, false);	// dd
					TextBoxCreate(&pUi[(*pUiCnt)++], 11*8, (uiRowIdx)*16, " ", StrBuf[10], 2, 2*8, false);	// HH
					TextBoxCreate(&pUi[(*pUiCnt)++], 14*8, (uiRowIdx)*16, ":", StrBuf[11], 2, 2*8, false);	// mm
					TextBoxCreate(&pUi[(*pUiCnt)++], 17*8, (uiRowIdx++)*16, ":", StrBuf[12], 2, 2*8, false);	// ss

					CombBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�����ϱ�����:", &StrBuf[0][0], 6, 
						"10s", "20s", "30s", "40s", "50s", "60s");
					TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx)*16, "�����ϱ�Ƶ��:", StrBuf[13], 2, 2*8, true);
					LableCreate(&pUi[(*pUiCnt)++], 8*16, (uiRowIdx++)*16, "Сʱ");
					key = ShowUI(UiList, &currUi);
					
					if (key == KEY_CANCEL){
						break;
					}
					isUiFinish = true;

					// �����ϱ���ʼʱ��
					if( (i = TimeStrsToTimeBytes(timeBytes, StrBuf[1], StrBuf[2], StrBuf[3], StrBuf[4], StrBuf[5], StrBuf[6])) > 0){
						currUi = 1 + (i -1);
						isUiFinish = false;
						continue;
					}
					// �����ϱ�����ʱ��
					if( (i = TimeStrsToTimeBytes(&timeBytes[7], StrBuf[7], StrBuf[8], StrBuf[9], StrBuf[10], StrBuf[11], StrBuf[12])) > 0){
						currUi = 8 + (i -1);
						isUiFinish = false;
						continue;
					}
					// �����ϱ�����
					u32Args[10] = StrBuf[0][0] + 1;
					// �����ϱ�Ƶ��
					u32Args[11] = (uint32)_atof(StrBuf[13]);
					if(StrBuf[13][0] < '0' || StrBuf[13][0] > '9' || u32Args[11] < 1 || u32Args[11] > 24){
						currUi = 15;
						isUiFinish = false;
						continue;
					}
					
					if(isUiFinish){
						break;
					}
				}
				if (key == KEY_CANCEL){
					break;
				}
				
				i = 0;
				Args.buf[i++] = 0x26;		// ������	26
				ackLen = 1;					// Ӧ�𳤶� 1	
				// ������
				Args.buf[i++] = 0x01;		// ����ѡ�� 00 - ��ȡ�� 01 - ����	
				switch ((uint8)u32Args[0]){			// �豸����
				case 0: u8Tmp = 0x41; break;
				case 1: u8Tmp = 0x44; break;
				case 2: u8Tmp = 0x35; break;
				case 3: u8Tmp = 0x2A; break;
				case 4: u8Tmp = 0x2B; break;
				default: u8Tmp = 0x41; break;
				}
				Args.buf[i++] = u8Tmp;	
				GetBytesFromStringHex(Args.buf, i, 7, &TmpBuf[0], 0, false);	// ��������	
				i += 7;
				Args.buf[i++] = (uint8)(u32Args[1] & 0xFF);	// �����澯��ֵ 4 byte
				Args.buf[i++] = (uint8)((u32Args[1] >> 8) & 0xFF);
				Args.buf[i++] = (uint8)((u32Args[1] >> 16) & 0xFF);	
				Args.buf[i++] = (uint8)((u32Args[1] >> 24) & 0xFF);	
				Args.buf[i++] = (uint8)(u32Args[2] & 0xFF);	// �����澯ʱ�� 1 byte

				Args.buf[i++] = (uint8)(u32Args[3] & 0xFF);	// �����澯��ֵ 4 byte
				Args.buf[i++] = (uint8)((u32Args[3] >> 8) & 0xFF);
				Args.buf[i++] = (uint8)((u32Args[3] >> 16) & 0xFF);	
				Args.buf[i++] = (uint8)((u32Args[3] >> 24) & 0xFF);	
				Args.buf[i++] = (uint8)(u32Args[4] & 0xFF);	// �����澯ʱ�� 1 byte

				Args.buf[i++] = (uint8)(u32Args[5] & 0xFF);	// ��ѹ�澯��ֵ 2 byte
				Args.buf[i++] = (uint8)((u32Args[5] >> 8) & 0xFF);		
				Args.buf[i++] = ip[0];					// IP	121.43.175.22 : 5683
				Args.buf[i++] = ip[1];
				Args.buf[i++] = ip[2];
				Args.buf[i++] = ip[3];
				Args.buf[i++] = (uint8)(port & 0xFF);	// port 2 byte
				Args.buf[i++] = (uint8)((port >> 8) & 0xFF);
				memcpy(&Args.buf[i], &TmpBuf[20], 6);	// APN	6 byte
				i += 6;
				Args.buf[i++] = (uint8)(u32Args[8] & 0xFF);	// �ϱ��������� 2 byte
				Args.buf[i++] = DecToBcd(timeBytes[1]);	// �����ϱ���ʼʱ�� YY MM dd HH mm ss		
				Args.buf[i++] = DecToBcd(timeBytes[2]);		
				Args.buf[i++] = DecToBcd(timeBytes[3]);			
				Args.buf[i++] = DecToBcd(timeBytes[4]);			
				Args.buf[i++] = DecToBcd(timeBytes[5]);			
				Args.buf[i++] = DecToBcd(timeBytes[6]);
				Args.buf[i++] = DecToBcd(timeBytes[8]);	// �����ϱ�����ʱ�� YY MM dd HH mm ss		
				Args.buf[i++] = DecToBcd(timeBytes[9]);		
				Args.buf[i++] = DecToBcd(timeBytes[10]);			
				Args.buf[i++] = DecToBcd(timeBytes[11]);			
				Args.buf[i++] = DecToBcd(timeBytes[12]);			
				Args.buf[i++] = DecToBcd(timeBytes[13]);
				Args.buf[i++] = (uint8)(u32Args[10] & 0xFF);	// �����ϱ�����ʱ��
				Args.buf[i++] = (uint8)(u32Args[7] & 0xFF);		// �ն���ͣ����	
				Args.buf[i++] = (uint8)(u32Args[11] & 0xFF);	// �����ϱ�Ƶ��
				Args.buf[i++] = (uint8)(u32Args[6] & 0xFF);		// �ܼ��ϱ���ʼʱ���	
				Args.buf[i++] = (uint8)(u32Args[9] & 0xFF);		// ���ڲ��������n x30����	
				Args.buf[i++] = 20;						// �ϱ������ȴ�ʱ�䣺�̶� 20 ����
				Args.buf[i++] = 5;						// ���ڲ���������̶� 5 ����	
				for(u8Tmp = 0; u8Tmp < 16; u8Tmp++){	// KEY	16 byte, �̶�ֵ�����裬��0
					Args.buf[i++] = 0;
				}

				Args.lastItemLen = i - 1;
				break;

			default: 
				break;
			}

			if (key == KEY_CANCEL){
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
}

// 6	��������
void WaterCmdFunc_Other(void)
{
	uint8 key, menuItemNo, tryCnt = 0, i;
	ListBox menuList;
	UI_Item * pUi = &UiList.items[0];
	uint8 * pUiCnt = &UiList.cnt;
	uint8 * pByte;
	uint8 currUi = 0, uiRowIdx, isUiFinish, u8Tmp;
	uint16 ackLen = 0, timeout, u16Tmp;

	_ClearScreen();

	// �˵�
	ListBoxCreate(&menuList, 0, 0, 20, 7, 5, NULL,
		"<<��������",
		5,
		"1. ���շ����ŷ�����",
		"2. ��ȡRXD��TXD�ŵ�",
		"3. ����RXD��TXD�ŵ�",
		"4. ������Ӫ�̱��",
		"5. ·���·�"
	);

	while(1){

		_Printfxy(0, 9*16, "����            ȷ��", Color_White);
		key = ShowListBox(&menuList);
		//------------------------------------------------------------
		if (key == KEY_CANCEL){	// ����
			break;
		}
		menuItemNo = menuList.strIdx + 1;

		memset(StrBuf, 0, TXTBUF_LEN * TXTBUF_MAX);
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
}


void WaterCmdFunc(void)
{
	_GuiMenuStru menu;

	/* v2.2 �˵�
	// menu.title= "<<���̵��� ";		
	// menu.str[0]=" �������� ";
	// menu.str[1]=" �������� ";
	// menu.str[2]=" �������� ";
	// menu.str[3]=" Ԥ������ ";
	// menu.str[4]=" �������� ";
	// menu.str[5]=" �������� ";
	// menu.str[6]=" �汾��Ϣ ";
	*/
	
	menu.left=0;
	menu.top=0;
	menu.no=5;
	menu.title= "<<���̵��� ";		// ���̵��� --> ��ԭ���� ���˲���
	menu.str[0]=" �������� ";
	menu.str[1]=" �������� ";
	menu.str[2]=" �������� ";
	menu.str[3]=" �������� ";
	menu.str[4]=" �汾��Ϣ ";
	menu.key[0]="1";
	menu.key[1]="2";
	menu.key[2]="3";
	menu.key[3]="4";
	menu.key[4]="5";
	menu.Function[0]=WaterCmdFunc_CommonCmd;
	menu.Function[1]=WaterCmdFunc_TestCmd;
	menu.Function[2]=WaterCmdFunc_WorkingParams;
	menu.Function[3]=WaterCmdFunc_Upgrade;
	menu.Function[4]=VersionInfoFunc;
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
	char strDstAddrTmp[20];

	memset(StrBuf, 0, TXTBUF_LEN * TXTBUF_MAX);
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
			sprintf(strDstAddrTmp, "D4D4D4D4D4D4D4D4");	// ��ʼֵΪ�㲥��ַ
			#if (AddrLen == 6)
			TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�� ��:", strDstAddrTmp, 12, 13*8, true);
			#else
			LableCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�� ��:");
			TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "   ", strDstAddrTmp, 16, 17*8, true);	
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

			if( strcmp(strDstAddrTmp, "D4D4D4D4D4D4D4D4") != 0 
				&& (strDstAddrTmp[0] < '0' || strDstAddrTmp[0] > '9'))
			{
				sprintf(strDstAddrTmp, " ������");
				currUi = 0;
				continue;
			}

			isUiFinish = true;
			continue;	// go back to get ui args
		}

		memcpy(StrDstAddr, strDstAddrTmp, 20);

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

// ��ȡ��������
void MainFuncReadFrozenData(void)
{
	uint8 key, tryCnt = 0, i;
	UI_Item * pUi = &UiList.items[0];
	uint8 * pUiCnt = &UiList.cnt;
	uint8 currUi = 0, uiRowIdx, isUiFinish;
	uint16 ackLen = 0, timeout;

	memset(StrBuf, 0, TXTBUF_LEN * TXTBUF_MAX);
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
}

// ��ȡ����ʱ��
void MainFuncReadMeterTime(void)
{
	uint8 key, tryCnt = 0, i;
	UI_Item * pUi = &UiList.items[0];
	uint8 * pUiCnt = &UiList.cnt;
	uint8 currUi = 0, uiRowIdx, isUiFinish;
	uint16 ackLen = 0, timeout;

	memset(StrBuf, 0, TXTBUF_LEN * TXTBUF_MAX);
	isUiFinish = false;

	while(1){
		
		_ClearScreen();

		// �������� :  ������ʾ
		_Printfxy(0, 0, "<<��ȡ����ʱ��", Color_White);
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
		CurrCmd = WaterCmd_ReadMeterTime;	// ��ȡ����ʱ��

		switch(CurrCmd){
		case WaterCmd_ReadMeterTime:		// "��ȡ����ʱ��"
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
}

// ���ñ���ʱ��
void MainFuncSetMeterTime(void)
{
	uint8 key, tryCnt = 0, i;
	UI_Item * pUi = &UiList.items[0];
	uint8 * pUiCnt = &UiList.cnt;
	uint8 currUi = 0, uiRowIdx, isUiFinish;
	uint16 ackLen = 0, timeout;
	uint8 *time = &TmpBuf[200], *timeBytes = &TmpBuf[300];

	memset(StrBuf, 0, TXTBUF_LEN * TXTBUF_MAX);
	isUiFinish = false;

	while(1){
		
		_ClearScreen();

		// �������� :  ������ʾ
		_Printfxy(0, 0, "<<���ñ���ʱ��", Color_White);
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
		CurrCmd = WaterCmd_SetMeterTime;	// ���ñ���ʱ��

		switch(CurrCmd){
		case WaterCmd_SetMeterTime:		// "���ñ���ʱ��"
			/*---------------------------------------------*/
			if(false == isUiFinish){
				_GetDateTime(time, '-',  ':');
				DatetimeToTimeStrs(time, StrBuf[0], StrBuf[1], StrBuf[2], StrBuf[3], StrBuf[4], StrBuf[5]);
				
				LableCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "ʱ ��:");
				TextBoxCreate(&pUi[(*pUiCnt)++], 0*8, (uiRowIdx)*16, " ", StrBuf[0], 4, 4*8, false);	// YYYY
				TextBoxCreate(&pUi[(*pUiCnt)++], 5*8, (uiRowIdx)*16, "-", StrBuf[1], 2, 2*8, false);	// MM
				TextBoxCreate(&pUi[(*pUiCnt)++], 8*8, (uiRowIdx)*16, "-", StrBuf[2], 2, 2*8, false);	// dd
				TextBoxCreate(&pUi[(*pUiCnt)++], 11*8, (uiRowIdx)*16, " ", StrBuf[3], 2, 2*8, false);	// HH
				TextBoxCreate(&pUi[(*pUiCnt)++], 14*8, (uiRowIdx)*16, ":", StrBuf[4], 2, 2*8, false);	// mm
				TextBoxCreate(&pUi[(*pUiCnt)++], 17*8, (uiRowIdx++)*16, ":", StrBuf[5], 2, 2*8, false);	// ss
				break;
			}
			// ʱ����ЧֵУ��
			if( (i = TimeStrsToTimeBytes(timeBytes, StrBuf[0], StrBuf[1], StrBuf[2], StrBuf[3], StrBuf[4], StrBuf[5])) > 0){
				currUi = 3 + (i -1);
				isUiFinish = false;
				continue;
			}

			sprintf(time, "%s-%s-%s %s:%s:%s",
				StrBuf[0], StrBuf[1], StrBuf[2], StrBuf[3], StrBuf[4], StrBuf[5]);
			_SetDateTime(time);

			i = 0;
			Args.buf[i++] = 0x14;		// ������	14
			ackLen = 2;					// Ӧ�𳤶� 2	
			// ������
			Args.buf[i++] = DecToBcd(timeBytes[0]);		// ʱ�� - yyyy/mm/dd HH:mm:ss
			Args.buf[i++] = DecToBcd(timeBytes[1]);		
			Args.buf[i++] = DecToBcd(timeBytes[2]);		
			Args.buf[i++] = DecToBcd(timeBytes[3]);			
			Args.buf[i++] = DecToBcd(timeBytes[4]);			
			Args.buf[i++] = DecToBcd(timeBytes[5]);			
			Args.buf[i++] = DecToBcd(timeBytes[6]);	
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

// ���쳣����
void MainFuncClearException(void)
{
	uint8 key, tryCnt = 0, i;
	UI_Item * pUi = &UiList.items[0];
	uint8 * pUiCnt = &UiList.cnt;
	uint8 currUi = 0, uiRowIdx, isUiFinish;
	uint16 ackLen = 0, timeout;

	memset(StrBuf, 0, TXTBUF_LEN * TXTBUF_MAX);
	isUiFinish = false;

	while(1){
		
		_ClearScreen();

		// �������� :  ������ʾ
		_Printfxy(0, 0, "<<���쳣", Color_White);
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
}

// ����
void MainFuncOpenValve(void)
{
	uint8 key, tryCnt = 0, i;
	UI_Item * pUi = &UiList.items[0];
	uint8 * pUiCnt = &UiList.cnt;
	uint8 currUi = 0, uiRowIdx, isUiFinish;
	uint16 ackLen = 0, timeout;

	memset(StrBuf, 0, TXTBUF_LEN * TXTBUF_MAX);
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
}

// �ط�
void MainFuncCloseValve(void)
{
	uint8 key, tryCnt = 0, i;
	UI_Item * pUi = &UiList.items[0];
	uint8 * pUiCnt = &UiList.cnt;
	uint8 currUi = 0, uiRowIdx, isUiFinish;
	uint16 ackLen = 0, timeout;

	memset(StrBuf, 0, TXTBUF_LEN * TXTBUF_MAX);
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
}

// ��ȡNB������Ϣ
void MainFuncReadNbJoinNetworkInfo(void)
{
	uint8 key, tryCnt = 0, i;
	UI_Item * pUi = &UiList.items[0];
	uint8 * pUiCnt = &UiList.cnt;
	uint8 currUi = 0, uiRowIdx, isUiFinish;
	uint16 ackLen = 0, timeout;

	memset(StrBuf, 0, TXTBUF_LEN * TXTBUF_MAX);
	isUiFinish = false;

	while(1){
		
		_ClearScreen();

		// �������� :  ������ʾ
		_Printfxy(0, 0, "<<��ȡNB������Ϣ", Color_White);
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
		CurrCmd = WaterCmd_ReadNbJoinNetworkInfo;
		
		switch(CurrCmd){
		case WaterCmd_ReadNbJoinNetworkInfo:		// ��ȡNB������Ϣ
				/*---------------------------------------------*/
				if(false == isUiFinish){
					break;
				}
				Args.buf[i++] = 0x10;		// ������	10
				ackLen = 34;				// Ӧ�𳤶� 34	
				// ������
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

// --------------------------------   ������   -----------------------------------------------
int main(void)
{
	_GuiMenuStru MainMenu;
	int fp;

	fp = _Fopen("system.cfg", "R");
	#ifdef Project_6009_RF
		_Lseek(fp, 0, 0);	// byte [0 ~ 19] 12λ���� 
	#else
		_Lseek(fp, 40, 0);	// byte [40 ~ 59] 16λ���� 
	#endif
	_Fread(StrDstAddr, TXTBUF_LEN, fp);
	_Fclose(fp);
	
	MainMenu.left=0;
	MainMenu.top=0;
	MainMenu.no=7;
	MainMenu.title =  "     ɣ���ֳֻ�    ";
	MainMenu.str[0] = " ��ȡ�û����� ";
	MainMenu.str[1] = " ��ȡ�������� ";
	MainMenu.str[2] = " ���� ";
	MainMenu.str[3] = " �ط� ";
    MainMenu.str[4] = " ���쳣 ";
	MainMenu.str[5] = " ��ȡNB������Ϣ ";
	MainMenu.str[6] = " ���̵��� ";
	MainMenu.key[0] = "1";
	MainMenu.key[1] = "2";
	MainMenu.key[2] = "3";
	MainMenu.key[3] = "4";
	MainMenu.key[4] = "5";
	MainMenu.key[5] = "6";
	MainMenu.key[6] = "7";
	MainMenu.Function[0] = MainFuncReadRealTimeData;
	MainMenu.Function[1] = MainFuncReadFrozenData;
	MainMenu.Function[2] = MainFuncOpenValve;
	MainMenu.Function[3] = MainFuncCloseValve;
    MainMenu.Function[4] = MainFuncClearException;
	MainMenu.Function[5] = MainFuncReadNbJoinNetworkInfo;
	MainMenu.Function[6] = WaterCmdFunc;	// ���̵��� --> ��ԭ���� ���˲���
	MainMenu.FunctionEx=0;
	_OpenLcdBackLight();
	_Menu(&MainMenu);	
}
