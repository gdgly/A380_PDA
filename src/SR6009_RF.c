/*
*
*	ɣ��6009ˮ���ֳֻ� - Lora��
*
*/
#include "SR6009_RF.h"

#include "HJLIB.H"
#include "string.h"
#include "stdio.h"

#include "Common.h"
#include "Tool.h"
#include "WaterMeter.h"
#include "MeterDocDBF.h"
#include "MeterDocDBF.c"

void FillStrsFunc(char **strs, int16 strsIdx, int16 srcIdx, uint16 cnt)
{
	int i = strsIdx;

	for(i = strsIdx; i < strsIdx + cnt; i++){
		sprintf(strs[i], "�б��� %d", i);
	}
}


#if CenterCmd_Enable
// --------------------------------  ������ģ��ͨ��  -----------------------------------------
// 1	��������
void CenterCmdFunc_CommonCmd(void)
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
	ListBoxCreate(&menuList, 0, 0, 20, 7, 9, NULL,
		"<<��������",
		9,
		"1. ��ȡ��������",
		"2. ��ȡ�������汾",
		"3. ��ȡ������ʱ��",
		"4. ���ü�����ʱ��",
		"5. ��ȡGPRS����",
		"6. ����GPRS����",
		"7. ��GPRS�ź�ǿ��",
		"8. ��������ʼ��",
		"9. ������������ģʽ"
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
			CurrCmd = (0x1010 + menuItemNo);

			switch(CurrCmd){
			case CenterCmd_ReadCenterNo:	// ����������
				/*---------------------------------------------*/
				if(false == isUiFinish){
					break;
				}
				Args.buf[i++] = 0x41;		// ������	41
				ackLen = 6;					// Ӧ�𳤶� 6	
				// ������
				Args.lastItemLen = i - 1;
				break;

			case CenterCmd_ReadCenterVer:	// ���������汾
				/*---------------------------------------------*/
				if(false == isUiFinish){
					break;
				}
				Args.buf[i++] = 0x40;		// ������	40
				ackLen = 6;					// Ӧ�𳤶� 6	
				// ������
				Args.lastItemLen = i - 1;
				break;

			case CenterCmd_ReadCenterTime:		// ��������ʱ��
				/*---------------------------------------------*/
				if(false == isUiFinish){
					break;
				}
				Args.buf[i++] = 0x43;		// ������	43
				ackLen = 7;					// Ӧ�𳤶� 7	
				// ������
				Args.lastItemLen = i - 1;
				break;
			
			case CenterCmd_SetCenterTime:		// �輯����ʱ��
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
					_Printfxy(0, 3*16, "ʱ ��: ", Color_White);
					uiRowIdx = 4;
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

            case CenterCmd_ReadGprsParam:		// ��GPRS����
				/*---------------------------------------------*/
				if(false == isUiFinish){
					break;
				}
				Args.buf[i++] = 0x45;		// ������	45
				ackLen = 40;				// Ӧ�𳤶� Լ 40	
				// ������
				Args.lastItemLen = i - 1;
				break;

			case CenterCmd_SetGprsParam:		// ��GPRS����
				/*---------------------------------------------*/
				if(false == isUiFinish){
					break;
				}
				Args.buf[i++] = 0x46;		// ������	46
				ackLen = 1;					// Ӧ�𳤶� 1	
				// ������
				Args.buf[i++] = 0x01;		// ǿ�Ʊ�ʶ 	0 - ��ǿ�ƣ� 1 - ǿ��
				Args.buf[i++] = 0x00;		// ���ط���ʶ	0 - �ط��� 1 - ����
				Args.lastItemLen = i - 1;
				break;

			case CenterCmd_ReadGprsSignal:		// ��GPRS�ź�ǿ��
				/*---------------------------------------------*/
				if(false == isUiFinish){
					break;
				}
				Args.buf[i++] = 0x47;		// ������	47
				ackLen = 50;				// Ӧ�𳤶� Լ 50	
				// ������
				Args.lastItemLen = i - 1;
				break;

			case CenterCmd_InitCenter:			// ��������ʼ��
				/*---------------------------------------------*/
				if(false == isUiFinish){
					CombBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "����:", &StrBuf[0][0], 2, 
						"��յ�����·��", "�����������");
					break;
				}
				Args.buf[i++] = 0x48;		// ������	48
				ackLen = 2;					// Ӧ�𳤶� 1	
				// ������
				Args.buf[i++] = (uint8)StrBuf[0][0];	// ��������	
				Args.lastItemLen = i - 1;
				break;

			case CenterCmd_ReadCenterWorkMode:	// ������������ģʽ
				/*---------------------------------------------*/
				if(false == isUiFinish){
					break;
				}
				Args.buf[i++] = 0x49;		// ������	49
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
			PrintfXyMultiLine_VaList(0, 2*16, "�� ��: %s", StrDstAddr);

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

// 2	��������
void CenterCmdFunc_DocumentOperation(void)
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
	ListBoxCreate(&menuList, 0, 0, 20, 7, 5, NULL,
		"<<��������",
		5,
		"1. ����������",
		"2. ��������Ϣ",
		"3. ��ӵ�����Ϣ",
		"4. ɾ��������Ϣ",
		"5. �޸ĵ�����Ϣ"
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
			CurrCmd = (0x1020 + menuItemNo);

			switch(CurrCmd){
			case CenterCmd_ReadDocCount:		// ����������
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
				Args.buf[i++] = 0x01;		// ������	01
				ackLen = 21;				// Ӧ�𳤶� 21	
				// ������
				Args.buf[i++] = 0x00;				// ���ݸ�ʽ 00	
				Args.lastItemLen = i - 1;
				break;

			case CenterCmd_ReadDocInfo:			// ��������Ϣ
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
				Args.lastItemLen = i - 1;
				break;

			case CenterCmd_AddDocInfo:			// ��ӵ�����Ϣ
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
			
			case CenterCmd_DeleteDocInfo:		// ɾ��������Ϣ
				/*---------------------------------------------*/
				if(false == isUiFinish){
					break;
				}
				Args.buf[i++] = 0x03;		// ������	03
				ackLen = 3;					// Ӧ�𳤶� 3	
				// ������
				Args.buf[i++] = 0x01;		// ǿ�Ʊ�ʶ 	0 - ��ǿ�ƣ� 1 - ǿ��
				Args.buf[i++] = 0x01;		// ���ط���ʶ	0 - �ط��� 1 - ����
				Args.lastItemLen = i - 1;
				break;

            case CenterCmd_ModifyDocInfo:		// �޸ĵ�����Ϣ
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
			PrintfXyMultiLine_VaList(0, 2*16, "�� ��: %s", StrDstAddr);

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

// 3	·������
void CenterCmdFunc_RouteSetting(void)
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
	ListBoxCreate(&menuList, 0, 0, 20, 7, 2, NULL,
		"<<·������",
		2,
		"1. ��ȡ�Զ���·��",
		"2. �����Զ���·��"
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
			CurrCmd = (0x1030 + menuItemNo);

			switch(CurrCmd){
			case CenterCmd_ReadDefinedRoute:	// ���Զ���·��
				/*---------------------------------------------*/
				if(false == isUiFinish){
					TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�� ��: ", StrBuf[0], 12, 13*8, true);
					break;
				}
				if(StrBuf[0][0] > '9' || StrBuf[0][0] < '0'){
					currUi = 1;
					isUiFinish = false;
					continue;
				}
				Args.buf[i++] = 0x55;		// ������	55
				ackLen = 21;				// Ӧ�𳤶� 21	
				// ������
				Args.buf[i++] = 0x00;				// ���ݸ�ʽ 00	
				Args.lastItemLen = i - 1;
				break;

			case CenterCmd_SetDefinedRoute:		// ���Զ���·��
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
			PrintfXyMultiLine_VaList(0, 2*16, "�� ��: %s", StrDstAddr);

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

// 4	����ת��
void CenterCmdFunc_CommandTransfer(void)
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
	ListBoxCreate(&menuList, 0, 0, 20, 7, 7, NULL,
		"<<����ת��",
		7,
		"1. ��ʵʱ����",
		"2. ����ʱ��������",
		"3. ����������",
		"4. ����",
		"5. �ط�",
		"6. ��ʹ��",
		"7. ���쳣"
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
			CurrCmd = (0x1040 + menuItemNo);

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
			
			case WaterCmd_OpenValveForce:		// " ǿ�ƿ��� ";
				/*---------------------------------------------*/
				if(false == isUiFinish){
					break;
				}
				Args.buf[i++] = 0x03;		// ������	03
				ackLen = 3;					// Ӧ�𳤶� 3	
				// ������
				Args.buf[i++] = 0x01;		// ǿ�Ʊ�ʶ 	0 - ��ǿ�ƣ� 1 - ǿ��
				Args.buf[i++] = 0x01;		// ���ط���ʶ	0 - �ط��� 1 - ����
				Args.lastItemLen = i - 1;
				break;

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

			case WaterCmd_CloseValveForce:		// " ǿ�ƹط� ";
				/*---------------------------------------------*/
				if(false == isUiFinish){
					break;
				}
				Args.buf[i++] = 0x03;		// ������	03
				ackLen = 3;					// Ӧ�𳤶� 3	
				// ������
				Args.buf[i++] = 0x01;		// ǿ�Ʊ�ʶ 	0 - ��ǿ�ƣ� 1 - ǿ��
				Args.buf[i++] = 0x00;		// ���ط���ʶ	0 - �ط��� 1 - ����
				Args.lastItemLen = i - 1;
				break;

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
			PrintfXyMultiLine_VaList(0, 2*16, "�� ��: %s", StrDstAddr);

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

void CenterCmdFunc(void)
{
	_GuiMenuStru menu;
	int fp;

	fp = _Fopen("system.cfg", "R");
	_Lseek(fp, 20, 0);	// byte [20 ~ 29] �������� 
	_Fread(StrDstAddr, TXTBUF_LEN, fp);
	_Fclose(fp);

	menu.left=0;
	menu.top=0;
	menu.no=4;
	menu.title= "<<���������� ";
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

	fp = _Fopen("system.cfg", "R");
	_Lseek(fp, 0, 0);	// byte [0 ~ 19] ��� 
	_Fread(StrDstAddr, TXTBUF_LEN, fp);
	_Fclose(fp);
	
}
#endif

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
	uint16 ackLen = 0, timeout;

	_ClearScreen();

	// �˵�
	ListBoxCreate(&menuList, 0, 0, 20, 7, 7, NULL,
		"<<��������",
		7,
		"1. ��ȡ�û�����",
		"2. ��ȡ��������",
		"3. ����",
		"4. ǿ�ƿ���",
		"5. �ط�",
		"6. ǿ�ƹط�",
		"7. ���쳣����"
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
			CurrCmd = (0x10 + menuItemNo);

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
			
			case WaterCmd_OpenValveForce:		// " ǿ�ƿ��� ";
				/*---------------------------------------------*/
				if(false == isUiFinish){
					break;
				}
				Args.buf[i++] = 0x03;		// ������	03
				ackLen = 3;					// Ӧ�𳤶� 3	
				// ������
				Args.buf[i++] = 0x01;		// ǿ�Ʊ�ʶ 	0 - ��ǿ�ƣ� 1 - ǿ��
				Args.buf[i++] = 0x01;		// ���ط���ʶ	0 - �ط��� 1 - ����
				Args.lastItemLen = i - 1;
				break;

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

			case WaterCmd_CloseValveForce:		// " ǿ�ƹط� ";
				/*---------------------------------------------*/
				if(false == isUiFinish){
					break;
				}
				Args.buf[i++] = 0x03;		// ������	03
				ackLen = 3;					// Ӧ�𳤶� 3	
				// ������
				Args.buf[i++] = 0x01;		// ǿ�Ʊ�ʶ 	0 - ��ǿ�ƣ� 1 - ǿ��
				Args.buf[i++] = 0x00;		// ���ط���ʶ	0 - �ط��� 1 - ����
				Args.lastItemLen = i - 1;
				break;

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
#ifdef Project_6009_RF
				for(i = 0; i < RELAY_MAX; i++){
					if(StrRelayAddr[i][0] > '9' || StrRelayAddr[i][0] < '0'){
						StrRelayAddr[i][0] = 0x00;
						sprintf(StrRelayAddr[i], "    (��ѡ)    ");
					}
				}
				TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�м�1:", StrRelayAddr[0], 12, 13*8, true);
				TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�м�2:", StrRelayAddr[1], 12, 13*8, true);
				TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�м�3:", StrRelayAddr[2], 12, 13*8, true);
#endif
				
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

	_CloseCom();
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
	ListBoxCreate(&menuList, 0, 0, 20, 7, 10, NULL,
		"<<��������",
		10,
		"1. �������",
		"2. �����¶�",
		"3. �����ѹ",
		"4. ��Ԥ�ɲο���",
		"5. ���ù�����ʱ",
		"6. ����Ӫ�̱��",
		"7. ���ϱ�·��",
		"8. ���ñ��",
		"9. ��debug��Ϣ",
		"10.��debug��Ϣ"
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
			CurrCmd = (0x20 + menuItemNo);

			switch(CurrCmd){
			case WaterCmd_ReadDebugInfo:		// ��debug��Ϣ
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

			case WaterCmd_ClearDebugInfo:		// ��debug��Ϣ
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

			case WaterCmd_RebootDevice:			// "�������"
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

			case WaterCmd_ReadTemperature:			// " �����¶� "
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

			case WaterCmd_ReadVoltage:				// " �����ѹ "
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
			
			case WaterCmd_ClearPrepaidRefVal:		// " ��Ԥ�ɲο��� ";
				/*---------------------------------------------*/
				if(false == isUiFinish){
					break;
				}
				Args.buf[i++] = 0x07;		// ������	07
				ackLen = 2;					// Ӧ�𳤶� 2	
				// ������
				Args.buf[i++] = 0x08;		// ����ѡ�� 08	
				Args.lastItemLen = i - 1;
				break;

            case WaterCmd_SetOverCurrentTimeout:		// " ���ù�����ʱ ";
				/*---------------------------------------------*/
				if(false == isUiFinish){
                    _Printfxy(7*16, (uiRowIdx)*16, "mA", Color_White);
					TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "��������:", StrBuf[0], 3, 8*8, true);
					_Printfxy(7*16, (uiRowIdx)*16, "ms", Color_White);
                    TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "��ʱʱ��:", StrBuf[1], 5, 8*8, true);
					break;
				}

				if(StrBuf[0][0] > '9' || StrBuf[0][0] < '0'){
					sprintf(StrBuf[0], " ������");
					currUi = 1;
					isUiFinish = false;
					continue;
				}
				if(StrBuf[1][0] > '9' || StrBuf[1][0] < '0'){
					sprintf(StrBuf[1], " ������");
					currUi = 2;
					isUiFinish = false;
					continue;
				}

				u16Tmp = (uint16)_atof(StrBuf[0]);
				u32Tmp = (uint32)_atof(StrBuf[1]);
				if(u16Tmp > 255){
					sprintf(StrBuf[0], " 0-255");
					currUi = 1;
					isUiFinish = false;
					continue;
				}
				if(u32Tmp > 65535){
					sprintf(StrBuf[1], " 0-65535");
					currUi = 2;
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

			case WaterCmd_ReadOperatorNumber:		// " ����Ӫ�̱�� ";
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
				if(CmdResult_Ok == Protol6009Tranceiver(WaterCmd_ReadMeterCfgInfo, &Addrs, &Args, ackLen, timeout, tryCnt)){
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
#ifdef Project_6009_RF
				for(i = 0; i < RELAY_MAX; i++){
					if(StrRelayAddr[i][0] > '9' || StrRelayAddr[i][0] < '0'){
						StrRelayAddr[i][0] = 0x00;
						sprintf(StrRelayAddr[i], "    (��ѡ)    ");
					}
				}
				TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�м�1:", StrRelayAddr[0], 12, 13*8, true);
				TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�м�2:", StrRelayAddr[1], 12, 13*8, true);
				TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�м�3:", StrRelayAddr[2], 12, 13*8, true);
#endif
				
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

	_CloseCom();
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
	ListBoxCreate(&menuList, 0, 0, 20, 7, 7, NULL,
		"<<��������",
		7,
		"1. ��������",
		"2. ��ѯ����",
		"3. �㲥����",
		"4. ��ӵ���",
		"5. ɾ������",
		"6. ��ѯ����",
		"7. ����ͳ��"
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
#ifdef Project_6009_RF
				for(i = 0; i < RELAY_MAX; i++){
					if(StrRelayAddr[i][0] > '9' || StrRelayAddr[i][0] < '0'){
						StrRelayAddr[i][0] = 0x00;
						sprintf(StrRelayAddr[i], "    (��ѡ)    ");
					}
				}
				TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�м�1:", StrRelayAddr[0], 12, 13*8, true);
				TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�м�2:", StrRelayAddr[1], 12, 13*8, true);
				TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�м�3:", StrRelayAddr[2], 12, 13*8, true);
#endif
				
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
					currUi = 1;
					isUiFinish = false;
					continue;
				}
				if(StrBuf[1][0] > '9' || StrBuf[1][0] < '0'){
					sprintf(StrBuf[1], " ������");
					currUi = 2;
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
					currUi = 1;
					isUiFinish = false;
					continue;
				}

				u16Tmp = (uint16)_atof(StrBuf[0]);
				if(u16Tmp > 255){
					sprintf(StrBuf[0], " 0-255");
					currUi = 4;
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
					currUi = 1;
					isUiFinish = false;
					continue;
				}
				u32Tmp = (uint32)_atof(StrBuf[0]);
				if(u32Tmp > 65535){
					sprintf(StrBuf[0], " 0-65535");
					currUi = 1;
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
					currUi = 1;
					isUiFinish = false;
					continue;
				}
				if(StrBuf[1][0] > '9' || StrBuf[1][0] < '0'){
					sprintf(StrBuf[1], " ������");
					currUi = 2;
					isUiFinish = false;
					continue;
				}
				u16Tmp = (uint16) _atof(StrBuf[0]);
				if(u16Tmp > 255){
					sprintf(StrBuf[0], " 0-255");
					currUi = 1;
					isUiFinish = false;
					continue;
				}
				u32Tmp = (uint32) _atof(StrBuf[1]);
				if(u32Tmp > 65535){
					sprintf(StrBuf[1], " 0-65535");
					currUi = 2;
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
#ifdef Project_6009_RF
				for(i = 0; i < RELAY_MAX; i++){
					if(StrRelayAddr[i][0] > '9' || StrRelayAddr[i][0] < '0'){
						StrRelayAddr[i][0] = 0x00;
						sprintf(StrRelayAddr[i], "    (��ѡ)    ");
					}
				}
				TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�м�1:", StrRelayAddr[0], 12, 13*8, true);
				TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�м�2:", StrRelayAddr[1], 12, 13*8, true);
				TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�м�3:", StrRelayAddr[2], 12, 13*8, true);
#endif
				
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
	ListBox menuList;
	UI_Item * pUi = &UiList.items[0];
	uint8 * pUiCnt = &UiList.cnt;
	uint8 * pByte, *time = &TmpBuf[200], *timeBytes = &TmpBuf[300];
	uint8 currUi = 0, uiRowIdx, isUiFinish;
	uint16 ackLen = 0, timeout, u16Tmp;
	uint32 u32Tmp;

	_ClearScreen();

	// �˵�
	ListBoxCreate(&menuList, 0, 0, 20, 7, 8, NULL,
		"<<��������",
		8,
		"1. ������������ϵ��",
		"2. �����ת��������",
		"3. ��ȡ����ʹ��״̬",
		"4. ���ö�ʱ�ϴ�",
		"5. ���ö����ϴ�",
		"6. ���ö�ʱ�����ϴ�",
		"7. �����ʱ��",
		"8. У���ʱ��"
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
			CurrCmd = (0x50 + menuItemNo);

			switch(CurrCmd){
			case WaterCmd_SetBaseValPulseRatio:	// ����������ϵ��
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
					currUi = 1;
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

			case WaterCmd_ClearReverseMeasureData:	// �����ת��������
				/*---------------------------------------------*/
				if(false == isUiFinish){
					break;
				}
				Args.buf[i++] = 0x0A;		// ������	0A
				ackLen = 6;					// Ӧ�𳤶� 6	
				// ������
				Args.lastItemLen = i - 1;
				break;

			case WaterCmd_ReadFuncEnableState:	// ��ȡ����ʹ��״̬
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
			
			case WaterCmd_SetTimedUpload:		// ���ö�ʱ�ϴ�
				/*---------------------------------------------*/
				if(false == isUiFinish){
					_Printfxy(7*16, (uiRowIdx)*16, "Сʱ", Color_White);
					TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "��ʱ���:", StrBuf[0], 2, 3*8, true);
					break;
				}
				if(StrBuf[0][0] > '9' || StrBuf[0][0] < '0'){
					sprintf(StrBuf[0], "   ");
					currUi = 1;
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

            case WaterCmd_SetFixedValUpload:	// ���ö����ϴ�
				/*---------------------------------------------*/
				if(false == isUiFinish){
					_Printfxy(7*16, (uiRowIdx)*16, "������", Color_White);
					TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�������:", StrBuf[0], 3, 4*8, true);
					break;
				}
				if((StrBuf[0][0] > '9' || StrBuf[0][0] < '0') 
					|| _atof(StrBuf[0]) > 255){
					sprintf(StrBuf[0], "   ");
					currUi = 1;
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

			case WaterCmd_SetTimedAndFixedValUpload:	// ���ö�ʱ�����ϴ�
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
					currUi = 1;
					isUiFinish = false;
					continue;
				}
				if((StrBuf[1][0] > '9' || StrBuf[1][0] < '0') 
					|| _atof(StrBuf[1]) > 255){
					sprintf(StrBuf[1], "   ");
					currUi = 2;
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

			case WaterCmd_ReadMeterTime:	// �����ʱ��
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

			case WaterCmd_SetMeterTime:		// У���ʱ��
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
					currUi = 2 + (i -1);
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
#ifdef Project_6009_RF
				for(i = 0; i < RELAY_MAX; i++){
					if(StrRelayAddr[i][0] > '9' || StrRelayAddr[i][0] < '0'){
						StrRelayAddr[i][0] = 0x00;
						sprintf(StrRelayAddr[i], "    (��ѡ)    ");
					}
				}
				TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�м�1:", StrRelayAddr[0], 12, 13*8, true);
				TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�м�2:", StrRelayAddr[1], 12, 13*8, true);
				TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�м�3:", StrRelayAddr[2], 12, 13*8, true);
#endif
				
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
					currUi = 1;
					isUiFinish = false;
					continue;
				}
				if(StrBuf[1][0] > '9' || StrBuf[1][0] < '0'){
					sprintf(StrBuf[1], " ������");
					currUi = 2;
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
					currUi = 1;
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
					currUi = 2;
					isUiFinish = false;
					continue;
				}
				u8Tmp = (uint8)StrBuf[0][0] + 2;	// ·������
				u16Tmp = (uint16) _atof(StrBuf[1]);	// ��ǰλ��
				if(u16Tmp > u8Tmp - 2){
					sprintf(StrBuf[1], " (��ʱ<=%d)", (uint8)StrBuf[0][0]);
					currUi = 2;
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

            case WaterCmd_ReadModuleRunningParams:		// ��ȡģ�����в���
				/*---------------------------------------------*/
				if(false == isUiFinish){
					break;
				}
				Args.buf[i++] = 0x3A;		// ������	3A
				ackLen = 124;				// Ӧ�𳤶� 124	
				Args.lastItemLen = i - 1;
				break;

			default: 
				break;
			}

			// ���� ���м̵�ַ����� �� ��ʾUI
			if(false == isUiFinish){
#ifdef Project_6009_RF
				for(i = 0; i < RELAY_MAX; i++){
					if(StrRelayAddr[i][0] > '9' || StrRelayAddr[i][0] < '0'){
						StrRelayAddr[i][0] = 0x00;
						sprintf(StrRelayAddr[i], "    (��ѡ)   ");
					}
				}
				TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�м�1:", StrRelayAddr[0], 12, 13*8, true);
				TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�м�2:", StrRelayAddr[1], 12, 13*8, true);
				TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�м�3:", StrRelayAddr[2], 12, 13*8, true);
#endif

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


//-----------------------------------	������	---------------------------
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
#ifdef Project_6009_RF
			for(i = 0; i < RELAY_MAX; i++){
				if(StrRelayAddr[i][0] > '9' || StrRelayAddr[i][0] < '0'){
					StrRelayAddr[i][0] = 0x00;
					sprintf(StrRelayAddr[i], "    (��ѡ)    ");
				}
			}
			TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�м�1:", StrRelayAddr[0], 12, 13*8, true);
			TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�м�2:", StrRelayAddr[1], 12, 13*8, true);
			TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�м�3:", StrRelayAddr[2], 12, 13*8, true);
#endif
			
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
#ifdef Project_6009_RF
			for(i = 0; i < RELAY_MAX; i++){
				if(StrRelayAddr[i][0] > '9' || StrRelayAddr[i][0] < '0'){
					StrRelayAddr[i][0] = 0x00;
					sprintf(StrRelayAddr[i], "    (��ѡ)    ");
				}
			}
			TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�м�1:", StrRelayAddr[0], 12, 13*8, true);
			TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�м�2:", StrRelayAddr[1], 12, 13*8, true);
			TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�м�3:", StrRelayAddr[2], 12, 13*8, true);
#endif
			
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

	memset(StrBuf, 0, TXTBUF_LEN * TXTBUF_MAX);
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
#ifdef Project_6009_RF
			for(i = 0; i < RELAY_MAX; i++){
				if(StrRelayAddr[i][0] > '9' || StrRelayAddr[i][0] < '0'){
					StrRelayAddr[i][0] = 0x00;
					sprintf(StrRelayAddr[i], "    (��ѡ)    ");
				}
			}
			TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�м�1:", StrRelayAddr[0], 12, 13*8, true);
			TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�м�2:", StrRelayAddr[1], 12, 13*8, true);
			TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�м�3:", StrRelayAddr[2], 12, 13*8, true);
#endif
			
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
	uint8 *time = &TmpBuf[200], *timeBytes = &TmpBuf[300];

	_CloseCom();
	_ComSetTran(CurrPort);
	_ComSet(CurrBaud, 2);

	memset(StrBuf, 0, TXTBUF_LEN * TXTBUF_MAX);
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
				currUi = 2 + (i -1);
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
#ifdef Project_6009_RF
			for(i = 0; i < RELAY_MAX; i++){
				if(StrRelayAddr[i][0] > '9' || StrRelayAddr[i][0] < '0'){
					StrRelayAddr[i][0] = 0x00;
					sprintf(StrRelayAddr[i], "    (��ѡ)    ");
				}
			}
			TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�м�1:", StrRelayAddr[0], 12, 13*8, true);
			TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�м�2:", StrRelayAddr[1], 12, 13*8, true);
			TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�м�3:", StrRelayAddr[2], 12, 13*8, true);
#endif
			
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

	memset(StrBuf, 0, TXTBUF_LEN * TXTBUF_MAX);
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
#ifdef Project_6009_RF
			for(i = 0; i < RELAY_MAX; i++){
				if(StrRelayAddr[i][0] > '9' || StrRelayAddr[i][0] < '0'){
					StrRelayAddr[i][0] = 0x00;
					sprintf(StrRelayAddr[i], "    (��ѡ)    ");
				}
			}
			TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�м�1:", StrRelayAddr[0], 12, 13*8, true);
			TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�м�2:", StrRelayAddr[1], 12, 13*8, true);
			TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�м�3:", StrRelayAddr[2], 12, 13*8, true);
#endif
			
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
#ifdef Project_6009_RF
			for(i = 0; i < RELAY_MAX; i++){
				if(StrRelayAddr[i][0] > '9' || StrRelayAddr[i][0] < '0'){
					StrRelayAddr[i][0] = 0x00;
					sprintf(StrRelayAddr[i], "    (��ѡ)    ");
				}
			}
			TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�м�1:", StrRelayAddr[0], 12, 13*8, true);
			TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�м�2:", StrRelayAddr[1], 12, 13*8, true);
			TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�м�3:", StrRelayAddr[2], 12, 13*8, true);
#endif
			
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
#ifdef Project_6009_RF
			for(i = 0; i < RELAY_MAX; i++){
				if(StrRelayAddr[i][0] > '9' || StrRelayAddr[i][0] < '0'){
					StrRelayAddr[i][0] = 0x00;
					sprintf(StrRelayAddr[i], "    (��ѡ)    ");
				}
			}
			TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�м�1:", StrRelayAddr[0], 12, 13*8, true);
			TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�м�2:", StrRelayAddr[1], 12, 13*8, true);
			TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�м�3:", StrRelayAddr[2], 12, 13*8, true);
#endif

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

// ��������
void MainFuncBatchMeterReading(void)
{
	uint8 key;
	ListBox menuList, menuList_2, menuList_3;
	ListBox XqList, LdList;				// С��/¥���б�
	_GuiInputBoxStru inputSt;
	UI_Item * pUi = &UiList.items[0];
	uint8 * pUiCnt = &UiList.cnt;
	uint8 currUi = 0, uiRowIdx, isUiFinish;
	uint8 *pByte;
	uint16 dispIdx, i;
	char *dispBuf = &DispBuf, *strTmp = &TmpBuf[0], *time = &TmpBuf[150];
	char *qryStrXq = &TmpBuf[200], *qryStrLd = &TmpBuf[350];
	uint8 qryTypeXq, qryTypeLd;
	uint16 qryIndexXq, qryIndexLd;
	uint32 recCnt;

	// �˵�
	//------------------------------------------------------------
	ListBoxCreate(&menuList, 0, 0, 20, 7, 5, NULL,
		"<<��������",
		5,
		"1. ��¥������",
		"2. ��ճ�����",
		"3. ���ó���ʱ��",
		"4. �����ѯ",
		"5. ����ͳ��"
	);
	while(1){

		_Printfxy(0, 9*16, "����            ȷ��", Color_White);
		key = ShowListBox(&menuList);
		//------------------------------------------------------------
		if (key == KEY_CANCEL){	// ����
			break;
		}
		memset(StrBuf, 0, TXTBUF_LEN * TXTBUF_MAX);
		isUiFinish = false;

		_Select(1);
		_Use(MeterDocDB);	// �����ݿ�
		recCnt = _Reccount();
		_Use("");			// �ر����ݿ�

		if(recCnt == 0){
			_GUIRectangleFill(0, 3*16 - 8, 160, 6*16 + 8, Color_White);
			PrintfXyMultiLine_VaList(0, 3*16, "  ��ǰ����Ϊ�գ�\n �������س�������\n �ٽ�����������");
			_GUIRectangle(0, 3*16 - 8, 160, 6*16 + 8, Color_Black);
			_Sleep(3000);
			continue;
		}

		switch(menuList.strIdx + 1){
		case 1:		// ��¥������

			// С���б�-����
			//------------------------------------------------------------
			_Printfxy(0, 9*16, "    <  ��ѯ��  >    ", Color_White);
			QueryDistrictList(&Districts, &DbQuery);
			ListBoxCreateEx(&XqList, 0, 0, 20, 7, Districts.cnt, NULL,
				"<<С��ѡ��", Districts.names, Size_ListStr, Districts.cnt);
			while(2){
				
				_Printfxy(0, 9*16, "����            ȷ��", Color_White);
				key = ShowListBox(&XqList);
				//------------------------------------------------------------
				if (key == KEY_CANCEL){	// ����
					break;
				}

				// xxС��-¥���б�-����
				//------------------------------------------------------------
				_Printfxy(0, 9*16, "    <  ��ѯ��  >    ", Color_White);
				Buildings.qryDistricNum = Districts.nums[XqList.strIdx];
				QueryBuildingList(&Buildings, &DbQuery);
				ListBoxCreateEx(&LdList, 0, 0, 20, 7, Buildings.cnt, NULL,
					"<<¥��ѡ��", Buildings.names, Size_ListStr, Buildings.cnt);
				//------------------------------------------------------------
				while(3){
	
					_Printfxy(0, 9*16, "����            ȷ��", Color_White);
					key = ShowListBox(&LdList);
					//------------------------------------------------------------
					if(key == KEY_CANCEL){	// ����
						break;
					}

					// ¥������-����
					//------------------------------------------------------------
					Meters.qryDistricNum = Districts.nums[XqList.strIdx];
					Meters.qryBuildingNum = Buildings.nums[LdList.strIdx];
					ListBoxCreate(&menuList_2, 0, 0, 20, 7, 6, NULL,
						"<<¥������", 
						6,
						"1. �Զ�����",
						"2. �ѳ��б�",
						"3. δ���б�",
						"4. ����ͳ��",
						"5. ��ճ�����",
						"6. ���ó���ʱ��");
					while(4){

						_Printfxy(0, 9*16, "����            ȷ��", Color_White);
						key = ShowListBox(&menuList_2);
						//------------------------------------------------------------
						if(key == KEY_CANCEL){	// ����
							break;
						}

						switch (menuList_2.strIdx + 1){
						case 1:		// �Զ�����
							Meters.selectField = Idx_MeterNum;
							Meters.qryMeterReadStatus = "0";
							QueryMeterList(&Meters, &DbQuery);
							key = ShowAutoMeterReading(&Meters);
							break;

						case 2:		// �ѳ��б�
							Meters.qryMeterReadStatus = "1";
							key = ShowMeterList(&Meters);
							break;
						case 3:		// δ���б�
							Meters.qryMeterReadStatus = "0";
							key = ShowMeterList(&Meters);
							break;

						case 4:		// ����ͳ��
							Meters.selectField = Idx_Invalid;
							_Printfxy(0, 9*16, "    <  ͳ����  >    ", Color_White);
							QueryMeterList(&Meters, &DbQuery);
							key = ShowMeterReadCountInfo(&Meters);
							break;

						case 5:		// ��ճ�����
							//-------------------------------------------------------
							_GUIRectangleFill(0, 4*16 - 8, 160, 6*16 + 8, Color_White);
							_Printfxy(0, 4*16, " ȷ��Ҫ���        ", Color_White);
							_Printfxy(0, 5*16, " ��ǰ¥����������?", Color_White);
							_GUIRectangle(0, 4*16 - 8, 160, 6*16 + 8, Color_Black);
							key = _ReadKey();
							//-------------------------------------------------------
							if(key != KEY_ENTER){
								break;
							}
							_GUIRectangleFill(0, 4*16 - 8, 160, 6*16 + 8, Color_White);
							_Printfxy(0, 4*16, "  ��ǰ¥��         ", Color_White);
							_Printfxy(0, 5*16, "  �����������... ", Color_White);
							_GUIRectangle(0, 4*16 - 8, 160, 6*16 + 8, Color_Black);
							//------------------------------------------------------------
							_Select(1);
							_Use(MeterDocDB);	// �����ݿ�
							_Go(0);
							for(i = 0; i < recCnt; i++){
								_ReadField(Idx_DistrictNum, strTmp);	// С����� ����
								strTmp[Size_DistrictNum - 1] = '\0';
								if(strcmp(Meters.qryDistricNum, strTmp) != 0){
									_Skip(1);	// ��һ�����ݿ��¼
									continue;
								}

								_ReadField(Idx_BuildingNum, strTmp);	// ¥����� ����
								strTmp[Size_BuildingNum - 1] = '\0';
								if(strcmp(Meters.qryBuildingNum, strTmp) != 0){
									_Skip(1);	// ��һ�����ݿ��¼
									continue;
								}

								_ReadField(Idx_MeterReadStatus, strTmp);	// ����״̬ ����
								strTmp[Size_MeterReadStatus - 1] = '\0';
								if(strTmp[0] != '1'){
									_Skip(1);	// ��һ�����ݿ��¼
									continue;
								}

								_Replace(Idx_MeterReadStatus, "0");	
								_Replace(Idx_MeterReadTime, "");
								_Replace(Idx_MeterReadType, "");
								_Replace(Idx_MeterValue, "");
								_Replace(Idx_MeterStatusHex, "");
								_Replace(Idx_MeterStatusStr, "");
								_Replace(Idx_BatteryVoltage, "");
								_Replace(Idx_SignalValue, "");
								_Skip(1);
							}
							_Use("");		// �ر����ݿ�
							//------------------------------------------------------------
							_Printfxy(0, 5*16, "  ������������! ", Color_White);
							_GUIRectangle(0, 4*16 - 8, 160, 6*16 + 8, Color_Black);
							_Sleep(3000);
							break;

						case 6:		// ���ó���ʱ��
							//-------------------------------------------------------
							_GUIRectangleFill(0, 3*16 - 8, 160, 7*16 + 8, Color_White);
							_GUIRectangle(0, 3*16 - 8, 160, 7*16 + 8, Color_Black);
							memset(StrBuf, 0, TXTBUF_LEN * TXTBUF_MAX);
							isUiFinish = false;
							while(true){
								if(false == isUiFinish){
									(*pUiCnt) = 0;
									uiRowIdx = 3;
									_GetDateTime(time, '-',  ':');
									DatetimeToTimeStrs(time, StrBuf[0], StrBuf[1], StrBuf[2], StrBuf[3], StrBuf[4], StrBuf[5]);
									
									LableCreate(&pUi[(*pUiCnt)++], 8, (uiRowIdx++)*16, "ϵͳʱ��:");
									TextBoxCreate(&pUi[(*pUiCnt)++], 0*8, (uiRowIdx)*16, " ", StrBuf[0], 4, 4*8, false);	// YYYY
									TextBoxCreate(&pUi[(*pUiCnt)++], 5*8, (uiRowIdx)*16, "-", StrBuf[1], 2, 2*8, false);	// MM
									TextBoxCreate(&pUi[(*pUiCnt)++], 8*8, (uiRowIdx)*16, "-", StrBuf[2], 2, 2*8, false);	// dd
									TextBoxCreate(&pUi[(*pUiCnt)++], 11*8, (uiRowIdx)*16, " ", StrBuf[3], 2, 2*8, false);	// HH
									TextBoxCreate(&pUi[(*pUiCnt)++], 14*8, (uiRowIdx)*16, ":", StrBuf[4], 2, 2*8, false);	// mm
									TextBoxCreate(&pUi[(*pUiCnt)++], 17*8, (uiRowIdx++)*16, ":", StrBuf[5], 2, 2*8, false);	// ss
									LableCreate(&pUi[(*pUiCnt)++], 8, (uiRowIdx++)*16, "ȷ��������ʱ������");
									LableCreate(&pUi[(*pUiCnt)++], 8, (uiRowIdx++)*16, "Ϊ��ǰϵͳʱ����?");
									
									key = ShowUI(UiList, &currUi);
									if (key == KEY_CANCEL){
										break;
									}
									isUiFinish = true;
								}

								// ʱ����ЧֵУ��
								if( (i = TimeStrsToTimeBytes(&TmpBuf[0], StrBuf[0], StrBuf[1], StrBuf[2], StrBuf[3], StrBuf[4], StrBuf[5])) > 0){
									currUi = 1 + (i -1);
									isUiFinish = false;
									continue;
								}

								sprintf(time, "%s-%s-%s %s:%s:%s",
									StrBuf[0], StrBuf[1], StrBuf[2], StrBuf[3], StrBuf[4], StrBuf[5]);
								_SetDateTime(time);
								break;
							}
							if (key == KEY_CANCEL){
								break;
							}

							_Printfxy(8, 5*16, "��ǰ¥��           ", Color_White);
							_Printfxy(8, 6*16, "����ʱ��������... ", Color_White);
							//------------------------------------------------------------
							_Select(1);
							_Use(MeterDocDB);	// �����ݿ�
							_Go(0);
							for(i = 0; i < recCnt; i++){
								_ReadField(Idx_DistrictNum, strTmp);	// С����� ����
								strTmp[Size_DistrictNum - 1] = '\0';
								if(strcmp(Meters.qryDistricNum, strTmp) != 0){
									_Skip(1);	// ��һ�����ݿ��¼
									continue;
								}

								_ReadField(Idx_BuildingNum, strTmp);	// ¥����� ����
								strTmp[Size_BuildingNum - 1] = '\0';
								if(strcmp(Meters.qryBuildingNum, strTmp) != 0){
									_Skip(1);	// ��һ�����ݿ��¼
									continue;
								}

								_ReadField(Idx_MeterReadStatus, strTmp);	// ����״̬ ����
								strTmp[Size_MeterReadStatus - 1] = '\0';
								if(strTmp[0] != '1'){
									_Skip(1);	// ��һ�����ݿ��¼
									continue;
								}

								_Replace(Idx_MeterReadTime, time);
								_Skip(1);
							}
							_Use("");		// �ر����ݿ�
							//------------------------------------------------------------
							_Printfxy(8, 6*16, "����ʱ��������ɣ� ", Color_White);
							_Sleep(3000);
							break;
						
						default:
							break;
						}

					}// while 4 ¥������
				}// while 3 ¥���б�
			}// while 2 С���б�
			break;

		case 2:		// ��ճ�����
			//-------------------------------------------------------
			_GUIRectangleFill(0, 4*16 - 8, 160, 6*16 + 8, Color_White);
			_Printfxy(0, 4*16, "  ȷ��Ҫ���       ", Color_White);
			_Printfxy(0, 5*16, "  ���г�������?   ", Color_White);
			_GUIRectangle(0, 4*16 - 8, 160, 6*16 + 8, Color_Black);
			key = _ReadKey();
			//-------------------------------------------------------
			if(key != KEY_ENTER){
				break;
			}
			_GUIRectangleFill(0, 4*16 - 8, 160, 5*16 + 8, Color_White);
			_Printfxy(0, 4*16, "  ��ճ�������... ", Color_White);
			_GUIRectangle(0, 4*16 - 8, 160, 5*16 + 8, Color_Black);
			//-------------------------------------------------------
			_Select(1);
			_Use(MeterDocDB);	// �����ݿ�
			_Go(0);
			do{
				_ReadField(Idx_MeterReadStatus, strTmp);	// ����״̬ ����
				strTmp[Size_MeterReadStatus - 1] = '\0';
				if(strTmp[0] != '1'){
					_Skip(1);	// ��һ�����ݿ��¼
					continue;
				}
				_Replace(Idx_MeterReadStatus, "0");
				_Replace(Idx_MeterReadTime, "");
				_Replace(Idx_MeterReadType, "");
				_Replace(Idx_MeterValue, "");
				_Replace(Idx_MeterStatusHex, "");
				_Replace(Idx_MeterStatusStr, "");
				_Replace(Idx_BatteryVoltage, "");
				_Replace(Idx_SignalValue, "");
				_Skip(1);		// ��һ�����ݿ��¼
			}while(_Eof() == false);
			_Use("");			// �ر����ݿ�
			//-------------------------------------------------------
			_Printfxy(0, 4*16, "  ��ճ�������ɣ�", Color_White);
			_GUIRectangle(0, 4*16 - 8, 160, 5*16 + 8, Color_Black);
			_Sleep(3000);
			break;

		case 3:		// ���ó���ʱ��
			//-------------------------------------------------------
			_GUIRectangleFill(0, 3*16 - 8, 160, 7*16 + 8, Color_White);
			_GUIRectangle(0, 3*16 - 8, 160, 7*16 + 8, Color_Black);
			while(true){
				if(false == isUiFinish){
					(*pUiCnt) = 0;
					uiRowIdx = 3;

					_GetDateTime(time, '-',  ':');
					DatetimeToTimeStrs(time, StrBuf[0], StrBuf[1], StrBuf[2], StrBuf[3], StrBuf[4], StrBuf[5]);
					
					LableCreate(&pUi[(*pUiCnt)++], 8, (uiRowIdx++)*16, "ϵͳʱ��:");
					TextBoxCreate(&pUi[(*pUiCnt)++], 0*8, (uiRowIdx)*16, " ", StrBuf[0], 4, 4*8, false);	// YYYY
					TextBoxCreate(&pUi[(*pUiCnt)++], 5*8, (uiRowIdx)*16, "-", StrBuf[1], 2, 2*8, false);	// MM
					TextBoxCreate(&pUi[(*pUiCnt)++], 8*8, (uiRowIdx)*16, "-", StrBuf[2], 2, 2*8, false);	// dd
					TextBoxCreate(&pUi[(*pUiCnt)++], 11*8, (uiRowIdx)*16, " ", StrBuf[3], 2, 2*8, false);	// HH
					TextBoxCreate(&pUi[(*pUiCnt)++], 14*8, (uiRowIdx)*16, ":", StrBuf[4], 2, 2*8, false);	// mm
					TextBoxCreate(&pUi[(*pUiCnt)++], 17*8, (uiRowIdx++)*16, ":", StrBuf[5], 2, 2*8, false);	// ss
					LableCreate(&pUi[(*pUiCnt)++], 8, (uiRowIdx++)*16, "ȷ��������ʱ������");
					LableCreate(&pUi[(*pUiCnt)++], 8, (uiRowIdx++)*16, "Ϊ��ǰϵͳʱ����?");
					
					key = ShowUI(UiList, &currUi);
					if (key == KEY_CANCEL){
						break;
					}
					isUiFinish = true;
				
				}
				// ʱ����ЧֵУ��
				if( (i = TimeStrsToTimeBytes(&TmpBuf[0], StrBuf[0], StrBuf[1], StrBuf[2], StrBuf[3], StrBuf[4], StrBuf[5])) > 0){
					currUi = 1 + (i -1);
					isUiFinish = false;
					continue;
				}

				sprintf(time, "%s-%s-%s %s:%s:%s",
					StrBuf[0], StrBuf[1], StrBuf[2], StrBuf[3], StrBuf[4], StrBuf[5]);
				_SetDateTime(time);
				break;
			}
			//-------------------------------------------------------
			if(key == KEY_CANCEL){
				break;
			}
			_Printfxy(8, 5*16, "���е���          ", Color_White);
			_Printfxy(8, 6*16, "����ʱ��������... ", Color_White);
			//------------------------------------------------------------
			_Select(1);
			_Use(MeterDocDB);	// �����ݿ�
			_Go(0);
			do{
				_ReadField(Idx_MeterReadStatus, strTmp);	// ����״̬ ����
				strTmp[Size_MeterReadStatus - 1] = '\0';
				if(strTmp[0] != '1'){
					_Skip(1);	// ��һ�����ݿ��¼
					continue;
				}
				_Replace(Idx_MeterReadTime, time);
				_Skip(1);
			}while(_Eof() == false);
			_Use("");		// �ر����ݿ�
			//------------------------------------------------------------
			_Printfxy(8, 6*16, "����ʱ��������ɣ�", Color_White);
			_Sleep(3000);
			break;

		case 4:		// �����ѯ
			// �����ѯ-����
			//------------------------------------------------------------
			ListBoxCreate(&menuList_2, 0, 0, 20, 7, 3, NULL,
				"<<�����ѯ", 
				3,
				"1. ����Ų�ѯ",
				"2. �����Ų�ѯ",
				"3. �����ƺŲ�ѯ");
			while(2){

				_Printfxy(0, 9*16, "����            ȷ��", Color_White);
				key = ShowListBox(&menuList_2);
				//------------------------------------------------------------
				if(key == KEY_CANCEL){	// ����
					break;
				}

				// �����ѯ-�������
				//------------------------------------------------------------
				_ClearScreen();
				sprintf(&TmpBuf[0], "<<%s", &(menuList_2.str[menuList_2.strIdx][3]));
				_Printfxy(0, 0*16, &TmpBuf[0], Color_White);
				_GUIHLine(0, 1*16 + 4, 160, Color_Black);	
				MeterInfo.dbIdx = Invalid_dbIdx;
				MeterInfo.qryMeterNum = NULL;
				MeterInfo.qryUserNum = NULL;
				MeterInfo.qryRoomNum = NULL;
				switch (menuList_2.strIdx + 1){
				case 1: pByte = "������: "; MeterInfo.qryMeterNum = StrBuf[0]; break;
				case 2: pByte = "���뻧��: "; MeterInfo.qryUserNum = StrBuf[0]; break;
				case 3: pByte = "�������ƺ�:"; MeterInfo.qryRoomNum = StrBuf[0]; break;
				default: break;
				}
				_Printfxy(0, 2*16, pByte, Color_White);
				StrBuf[0][0] = 0x00;
				inputSt.left = 0;
				inputSt.top = 3*16;
				inputSt.width = 10*16;
				inputSt.hight = 16;
				inputSt.caption = "";
				inputSt.context = StrBuf[0];
				inputSt.datelen = 20;
				inputSt.IsClear = true;
				inputSt.keyUpDown = false;
				inputSt.type = 1;
				_GUIHLine(0, 9*16 - 4, 160, Color_Black);	
				_Printfxy(0, 9*16, "����            ȷ��", Color_White);
				//------------------------------------------------------------
				if(KEY_CANCEL ==  _GetStr(&inputSt)){
					continue;
				}
				StringTrimStart(StrBuf[0],  ' ');
				StringTrimEnd(StrBuf[0],  ' ');

				QueryMeterInfo(&MeterInfo, &DbQuery);
				if(DbQuery.resultCnt > 0){
					MeterInfo.strCnt = 1;
					MeterInfo.strIdx = 0;
					ShowMeterInfo(&MeterInfo);
				}
				else{
					_GUIRectangleFill(0, 4*16 - 8, 160, 5*16 + 8, Color_White);
					_Printfxy(0, 4*16, " ��ѯʧ�ܣ�δ�ҵ� ", Color_White);
					_GUIRectangle(0, 4*16 - 8, 160, 5*16 + 8, Color_Black);
					_Sleep(3000);
				}
				
			} // while 2 �����ѯ
			break;

		case 5:		// ����ͳ��
			// ����ͳ��-����
			//------------------------------------------------------------
			ListBoxCreate(&menuList_2, 0, 0, 20, 3, 3, NULL,
				"<<����ͳ��", 
				3, 
				"1. С��ѡ��>",
				"2. ¥��ѡ��>",
				"3. ͳ��");
			//------------------------------------------------------------
			sprintf(qryStrXq, "ȫ��");
			sprintf(qryStrLd, "ȫ��");
			qryTypeXq = 0;		// 0 - ȫ���� 1 - ѡ��ĳ��
			qryTypeLd = 0;		// 0 - ȫ���� 1 - ѡ��ĳ��
			qryIndexXq = 0;		// ��ǰѡ���С�� ���б�������
			qryIndexLd = 0;		// ��ǰѡ���¥�� ���б�������
			while(2){

				_ClearScreen();
				// ����ͳ��-����
				//---------------------------------------------------------------------
				dispBuf = &DispBuf;
				dispIdx = 0;
				dispIdx += sprintf(&dispBuf[dispIdx], "С��: %s\n", qryStrXq);
				dispIdx += sprintf(&dispBuf[dispIdx], "¥��: %s", qryStrLd);
				PrintfXyMultiLine(0, 5*16, dispBuf, 7);
				//----------------------------------------------------------------------
				_GUIHLine(0, 9*16 - 4, 160, Color_Black);
				_Printfxy(0, 9*16, "����            ȷ��", Color_White);
				key = ShowListBox(&menuList_2);
				//----------------------------------------------------------------------
				if(key == KEY_CANCEL){	// ����
					break;
				}
				
				if(menuList_2.strIdx == 0){		// С��ѡ��
					//----------------------------------------------
					ListBoxCreate(&menuList_3, 2*16, 2*16, 12, 2, 2, NULL, 
					"С��ѡ��", 2, "ȫ��", "ѡ��ĳ��");
					key = ShowListBox(&menuList_3);
					//----------------------------------------------
					if(key == KEY_CANCEL){	// δѡ�񣬷���
						continue;
					}

					if(menuList_3.strIdx == 0){
						qryTypeXq = 0;
						sprintf(qryStrXq, "ȫ��");
					}
					else{
						// С���б�-����
						//------------------------------------------------------------
						_Printfxy(0, 9*16, "    <  ��ѯ��  >    ", Color_White);
						QueryDistrictList(&Districts, &DbQuery);
						ListBoxCreateEx(&XqList, 0, 0, 20, 7, Districts.cnt, NULL,
							"<<С��ѡ��", Districts.names, Size_ListStr, Districts.cnt);
						_Printfxy(0, 9*16, "����            ȷ��", Color_White);
						key = ShowListBox(&XqList);
						//------------------------------------------------------------
						if (key == KEY_CANCEL){		// δѡ���б���
							continue;
						}

						qryTypeXq = 1;
						qryIndexXq = XqList.strIdx;

						_Select(1);
						_Use(MeterDocDB);	// �����ݿ�
						recCnt = _Reccount();
						if(_LocateEx(Idx_DistrictNum, '=', Districts.nums[qryIndexXq], 1, recCnt, 0) > 0){ 
							_ReadField(Idx_DistrictName, qryStrXq);	// ����С����
						}
						_Use("");			// �ر����ݿ�
					}

				}
				else if(menuList_2.strIdx == 1){	// ¥��ѡ��
					//----------------------------------------------
					ListBoxCreate(&menuList_3, 2*16, 3*16, 12, 2, 2, NULL, 
					"¥��ѡ��", 2, "ȫ��", "ѡ��ĳ��");
					key = ShowListBox(&menuList_3);
					//----------------------------------------------
					if(key == KEY_CANCEL){	// δѡ�񣬷���
						continue;
					}

					if(menuList_3.strIdx == 0){
						qryTypeLd = 0;
						sprintf(qryStrLd, "ȫ��");
					}
					else{

						if(qryTypeXq == 0){
							_GUIRectangleFill(0, 4*16 - 8, 160, 6*16 + 8, Color_White);
							_Printfxy(0, 4*16, " ѡ��ĳ��¥��ʱ ", Color_White);
							_Printfxy(0, 5*16, " ����ѡ������С��! ", Color_White);
							_GUIRectangle(0, 4*16 - 8, 160, 6*16 + 8, Color_Black);
							_Sleep(3000);
							continue;
						}

						// xxС��-¥���б�-����
						//------------------------------------------------------------
						_Printfxy(0, 9*16, "    <  ��ѯ��  >    ", Color_White);
						Buildings.qryDistricNum = Districts.nums[qryIndexXq];
						QueryBuildingList(&Buildings, &DbQuery);
						ListBoxCreateEx(&LdList, 0, 0, 20, 7, Buildings.cnt, NULL,
							"<<¥��ѡ��", Buildings.names, Size_ListStr, Buildings.cnt);

						_Printfxy(0, 9*16, "����            ȷ��", Color_White);
						key = ShowListBox(&LdList);
						//------------------------------------------------------------
						if (key == KEY_CANCEL){		// δѡ���б���
							continue;
						}

						qryTypeLd = 1;
						qryIndexLd = LdList.strIdx;

						_Select(1);
						_Use(MeterDocDB);	// �����ݿ�
						recCnt = _Reccount();
						if(_LocateEx(Idx_BuildingNum, '=', Buildings.nums[qryIndexLd], 1, recCnt, 0) > 0){ 
							_ReadField(Idx_BuildingName, qryStrLd);	// ����¥����
						}
						_Use("");			// �ر����ݿ�
					}
					
				}
				else {	// if(menuList_2.strIdx == 3){	// ͳ��
					Meters.qryDistricNum = (qryTypeXq == 0 ? NULL : Districts.nums[qryIndexXq]);
					Meters.qryBuildingNum = (qryTypeLd == 0 ? NULL : Buildings.nums[qryIndexLd]);
					Meters.selectField = Idx_Invalid;
					_Printfxy(0, 9*16, "    <  ͳ����  >    ", Color_White);
					QueryMeterList(&Meters, &DbQuery);
					if(Meters.districName[0] == 0x00){
						strcpy(Meters.districName, "ȫ��");
					}
					if(Meters.buildingName[0] == 0x00){
						strcpy(Meters.buildingName, "ȫ��");
					}
					key = ShowMeterReadCountInfo(&Meters);
				}
			}
			break;

		default: 
			break;
		}

	} // while 1 ��������
}

// ���̵���		------------------------------
void MainFuncEngineerDebuging(void)
{
	#if CenterCmd_Enable
	_GuiMenuStru menu;

	menu.top = 0;
	menu.left = 0;
	menu.no = 3;
	menu.title = "<<���̵���";
	menu.str[0] = "��˲���";
	menu.str[1] = "����������";
	menu.str[2] = "�汾��Ϣ";
	menu.key[0] = "1";
	menu.key[1] = "2";
	menu.key[2] = "3";
	menu.Function[0] = WaterCmdFunc;
	menu.Function[1] = CenterCmdFunc;
	menu.Function[2] = VersionInfoFunc;
	menu.FunctionEx = 0;
	_Menu(&menu);
	#endif
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
	MainMenu.str[6] = " ���̵��� ";
	MainMenu.str[7] = " �������� ";
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
	MainMenu.Function[6] = WaterCmdFunc;	// ���̵��� --> ��ԭ���� ��˲���
	MainMenu.Function[7] = MainFuncBatchMeterReading;
	MainMenu.FunctionEx=0;
	_OpenLcdBackLight();
	_Menu(&MainMenu);	
}

