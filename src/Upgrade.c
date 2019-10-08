#include "HJLIB.H"
#include "string.h"
#include "stdio.h"

#include "Upgrade.h"
#include "Common.h"
#include "WaterMeter.h"

void Func_Upgrade(void)
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
	LableCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "2. ������");	
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

		_MessageBoxEx("", "unavailable now", MSG_OK);
		_Sleep(1000);
		return;

		memset(StrBuf, 0, TXTBUF_LEN * TXTBUF_MAX);
		isUiFinish = false;

		switch (currUi)
		{
		case 0:	// ѡ�������ļ�
			{


			}
			break;

		case 2:	// ������
		default:
			if(StrDstAddr[0] < '0' || StrDstAddr[0] > '9' ){
				sprintf(StrDstAddr, " ������");
				currUi = 2;
				continue;
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
		}

		while(1){
			
			_ClearScreen();

			_Printfxy(0, 0, "<<��ʼ����", Color_White);
			_GUIHLine(0, 1*16 + 4, 160, Color_Black);	
			/*---------------------------------------------*/
			PrintfXyMultiLine_VaList(0, 2*16, "�汾��%s  CRC��%s");
			PrintfXyMultiLine_VaList(0, 3*16, "��ţ�\r\n  %s");
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
				Args.buf[i++] = 0x00;		// ˮ���ѹ
				Args.buf[i++] = 0x00;		// �����ѹ
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

