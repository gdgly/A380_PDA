#include "HJLIB.H"
#include "string.h"
#include "stdio.h"

#include "Upgrade.h"
#include "Common.h"
#include "WaterMeter.h"

#include "List.h"

#include "MeterDocDBF.h"
//extern MeterListSt Meters;
#ifndef Meter_Max
#define Meter_Max	300	// һ��¥-������
#endif

//-------------------------		ȫ�ֱ���	-----------------
AppFileInfo AppInfo;
PacketInfo PktInfo;
UpgradeDocs UpgrdDocs;
TList DocList;

//-----------------------		�ڲ�����	------------------

// �Ӱ汾��Ϣ�н�ȡ�汾��
static uint8 * GetVersionNo(uint8 *buf, uint8 verSize)
{
	uint8 i, *ptr;

	for(i = 0; i < 40; i++){
		if(buf[i] == (uint8)'.'){
			ptr = &buf[i - 1];
		}
		if(buf[i] == (uint8)' '){
			buf[i] = 0x00;
			break;
		}
	}

	return ptr;
}

bool IsMtrNoEqual(void *node, void *mtrNo)
{
    uint8 *src = ((DocInfo *)node)->mtrNo;
    uint8 *dst = (uint8 *)mtrNo;
    uint16 i;

    if(node == NULL || mtrNo == NULL) return false;

    for(i = 0; i < 20 && *src != 0x00; i++)
    {
        if(*src != *dst) break;

		src++;
		dst++;
    }
    
    return (*src == *dst);
}

/*
* ��������ʾ���������б�, ���100��
* ������docs - ������Ϣ�б�
* ���أ�uint8 - �����˳�ʱ�İ���ֵ�� KEY_CANCEL - ���ؼ� �� KEY_ENTER - ȷ�ϼ�
*/
static uint8 ShowDocList(TList *docs)
{
	uint8 key;
	//uint8 **strs = &DispBuf;
	uint8 **strs = Meters.strs;
	uint8 strSize = 20, *ptr, *pList;
	ListBox docList;
	DocInfo *docItem;

	if(docs->cnt > Meter_Max) return KEY_CANCEL;

	pList = (uint8 *)strs;
	docItem = (DocInfo *)docs->head;
	
	while(docItem != NULL){

		switch (docItem->state){
		case UpgrdState_NotStart: ptr = "not"; break;
		case UpgrdState_PktWait: ptr = "wait"; break;
		case UpgrdState_Finish: ptr = "ok"; break;
		default: break;
		}

		sprintf(pList, "%s  %s", docItem->mtrNo, ptr);
		pList += strSize;

		docItem = (DocInfo *)docItem->next;
	}

	ListBoxCreateEx(&docList, 0, 0, 20, 7, docs->cnt, NULL,
			"<<�����б�", strs, strSize, docs->cnt);
	_Printfxy(0, 9*16, "����            ȷ��", Color_White);
	key = ShowListBox(&docList);
	return key;
}

/*
* ������ɾ������
* ������docs - ������Ϣ�б�
* ���أ�uint8 - �����˳�ʱ�İ���ֵ�� KEY_CANCEL - ���ؼ� �� KEY_ENTER - ȷ�ϼ�
*/
static uint8 ShowDocList_ForDelete(TList *docs)
{
	uint8 key;
	//uint8 **strs = &DispBuf;
	uint8 **strs = Meters.strs;
	uint8 strSize = 20, *ptr, *pList;
	uint16 currIdx = 0;
	ListBox docList;
	DocInfo *docItem;

	if(docs->cnt > Meter_Max) return KEY_CANCEL;

	while(true){

		pList = (uint8 *)strs;
		docItem = (DocInfo *)docs->head;
		
		while(docItem != NULL){
			switch (docItem->state){
			case UpgrdState_NotStart: ptr = "not"; break;
			case UpgrdState_PktWait: ptr = "wait"; break;
			case UpgrdState_Finish: ptr = "ok"; break;
			default: break;
			}
			sprintf(pList, "%s  %s", docItem->mtrNo, ptr);
			pList += strSize;
			docItem = (DocInfo *)docItem->next;
		}

		ListBoxCreateEx(&docList, 0, 0, 20, 7, docs->cnt, NULL,
				"<<�����б�", strs, strSize, docs->cnt);
		_Printfxy(0, 9*16, "����            ɾ��", Color_White);
		
		docList.currIdx = currIdx;
		key = ShowListBox(&docList);
		currIdx = docList.currIdx;

		if(key == KEY_ENTER){
			docs->removeAt(docs, currIdx);
			LogPrint("del currIdx : %d \n", currIdx);
		}
		else{
			break;
		}
	}

	return key;
}

// ������������
static void UpgradeFunc_UpgradeDocMgmt(void)
{
	uint8 key, menuItemNo;
	UI_Item uiTxtbox;
	ListBox menuList;
	DocInfo *pDocInfo;

	if(DocList.cnt == 0){
		List_Init(&DocList);
	}

	ListBoxCreate(&menuList, 0, 0, 20, 7, 4, NULL,
		"<<������������", 
		4,
		"1. ��ӵ���",
		"2. �鿴����",
		"3. ɾ������",
		"4. ��յ���"
	);

	while(1){
		_ClearScreen();

		key = ShowListBox(&menuList);
		if (key == KEY_CANCEL){	// ����
			return;
		}
		menuItemNo = menuList.strIdx + 1;

		switch (menuItemNo)
		{
		case 1:	// ��ӵ���
			{
				_ClearScreen();
				TextBoxCreate(&uiTxtbox, 0, 2*16 + 8, "  > ", StrDstAddr, (AddrLen * 2), (AddrLen * 8 + 8), true);

				while(2){
					//_GUIRectangleFill(0, 0, );

					_Printfxy(0, 0, "<<��ӵ���", Color_White);
					_GUIHLine(0, 1*16 + 4, 160, Color_Black);	
					/*---------------------------------------------*/
					_Printfxy(0, 1*16 + 8, "��ţ�", Color_White);
					_Printfxy(uiTxtbox.x, uiTxtbox.y, uiTxtbox.title, Color_White);
					//----------------------------------------------
					_GUIHLine(0, 9*16 - 4, 160, Color_Black);
					_Printfxy(0, 9*16, "����            ȷ��", Color_White);
					key = GetInputNumStr(&uiTxtbox);
					//------------------------------------------------------------
					if (key == KEY_CANCEL){	// ����
						break;
					}

					_leftspace(StrDstAddr, AddrLen * 2, '0');

					if(StrDstAddr[0] >= '0' && StrDstAddr[0] <= '9' ){
						if(NULL != DocList.find(&DocList, IsMtrNoEqual, StrDstAddr)){
							_Printfxy(0, 5*16 + 8, "    ����ظ���  ", Color_White);
							_Sleep(500);
							continue;
						}
						pDocInfo = (DocInfo *)DocList.add(&DocList, NULL, sizeof(DocInfo));
						memcpy(pDocInfo->mtrNo, StrDstAddr, 20);
						pDocInfo->state = UpgrdState_NotStart;
					}

					_Printfxy(0, 5*16 + 8, "    ��ӳɹ���  ", Color_White);
					PrintfXyMultiLine_VaList(0, 6*16 + 8, "��ǰ����������%d", DocList.cnt);
					_Sleep(500);
					_Printfxy(0, 5*16 + 8, "                    ", Color_White);
				}
			}
			break;
		
		case 2:	// �鿴�����б�
			{
				key = ShowDocList(&DocList);
			}
			break;

		case 3:	// ɾ������
			{
				key = ShowDocList_ForDelete(&DocList);
			}
			break;

		case 4:	// ��յ���
			{
				ShowMsg(16, 4 * 16, "  ���������... ", 50);
				DocList.clear(&DocList);
				ShowMsg(16, 4 * 16, "  ���������ɣ� ", 1000);
			}
			break;

		default:
			break;
		}
	}
}


// ��ʼ����
static UpgradeState UpgradeFunc_UpgradeStart(uint8 upgradeMode)
{
	UpgradeState state = UpgrdState_NotStart;
	AppFileInfo *app = &AppInfo;
	PacketInfo *pkt = &PktInfo;
	UpgradeDocs *docs = &UpgrdDocs;
	uint16 docIdx, sendIdx, pktIdx, crc16, i, u16Tmp;
	uint8 *mtrNo, *cmdName = &DispBuf[1024], *cmdMsg = &DispBuf[1060];
	uint8 cmdState = Cmd_Send;
	uint16 ackLen, timeout, dispIdx, index;
	uint8 tryCnt, lcdCtrl, key;
	CmdResult cmdResult = CmdResult_Ok;
	//char strTmp[20];
	uint8 *pData, *ptr;

	_ClearScreen();

	_Printfxy(0, 0, "<<��ʼ����", Color_White);
	_GUIHLine(0, 1*16 + 4, 160, Color_Black);	
	/*---------------------------------------------*/
	PrintfXyMultiLine_VaList(0, 2*16, "�汾��%s  CRC��%4X", pkt->version, pkt->verCrc16);
	// to show meter no
	// to show cmd name
	// to show cmd msg
	//----------------------------------------------
	_GUIHLine(0, 9*16 - 4, 160, Color_Black);
	_Printfxy(0, 9*16, "����   <������>   ", Color_White);


	// �м����
	for(i = 0; i < RELAY_MAX; i++){				
		if(StrRelayAddr[i][0] > '9' || StrRelayAddr[i][0] < '0'){
			StrRelayAddr[i][0] = 0x00;
		}
	}

	CurrCmd = WaterCmd_NoticeUpgrade_OnApp;
	docIdx = 0;

	while(1){
	
		// LCD�����ƿ���
		if(lcdCtrl == 0){
			_OpenLcdBackLight();
			lcdCtrl++;
			LcdOpened = true;
		}
		else if(lcdCtrl < 4){
			lcdCtrl++;
		}
		else if(lcdCtrl == 4){
			_CloseLcdBackLight();
			lcdCtrl++;
			LcdOpened = false;
		}

		if(cmdState == Cmd_Send){
			// ��ǰ���
			mtrNo = &UpgrdDocs.mtrNos[docIdx][0];
			strcpy(StrDstAddr, mtrNo);
		
			// �����������
			i = 0;	
			Args.itemCnt = 2;
			Args.items[0] = &Args.buf[0];   // ������
			Args.items[1] = &Args.buf[1];	// ������
		}

		switch(CurrCmd){
		case WaterCmd_NoticeUpgrade_OnApp:		// ֪ͨϵͳ����_��app
		case WaterCmd_NoticeUpgrade_OnBoot:		// ֪ͨϵͳ����_��boot
			/*---------------------------------------------*/
			if(cmdState == Cmd_Finish){
				if(CurrCmd == WaterCmd_NoticeUpgrade_OnBoot){
					GetMissPktList(pkt);
					sendIdx = 0;
				}
				CurrCmd++;
				cmdState = Cmd_Send;
				docIdx = 0;
				break;
			}
			if(cmdState == Cmd_RecvOk){
				dispIdx = 0;
				index = 0;
				ptr = Water6009_GetStrDisUpgradeReason(pData[index]);
				dispIdx += sprintf(&cmdMsg[dispIdx], "��������: %s\n", ptr);
				index += 1;
				ptr = GetVersionNo(&pData[index], 40);
				dispIdx += sprintf(&cmdMsg[dispIdx], "��ǰ�汾: %s\n", ptr);

				if(docIdx == docs->cnt){
					cmdState = Cmd_Finish;
				}
				break;
			}
			docIdx++;
			
			if(CurrCmd == WaterCmd_NoticeUpgrade_OnApp){
				sprintf(cmdName, "֪ͨ��ʼ����-A");
				sprintf(cmdMsg, "");
				Args.buf[i++] = 0x70;		// ������	70
			}
			else{
				sprintf(cmdName, "֪ͨ��ʼ����-B");
				sprintf(cmdMsg, "");
				Args.buf[i++] = 0x71;		// ������	71
			}
			
			ackLen = 41;				// Ӧ�𳤶� 41	
			// ������
			Args.buf[i++] = app->waterMeterVbat;		// ˮ���ѹ
			Args.buf[i++] = app->gasMeterVbat;		// �����ѹ
			Args.buf[i++] = app->rssi;				// RSSI����
			Args.buf[i++] = app->snr;				// SNR����
			memcpy(&Args.buf[i], app->appVer, 40);	// ����汾 40 byte	
			i += 40;	
			Args.buf[i++] = app->crc16_appVer[0];	// �汾CRC16
			Args.buf[i++] = app->crc16_appVer[1];	
			Args.buf[i++] = app->packetCnt[0];		// �ܰ���
			Args.buf[i++] = app->packetCnt[1];
			Args.buf[i++] = app->crc16_1st26K[0];	// ǰ26K CRC16
			Args.buf[i++] = app->crc16_1st26K[1];	
			Args.buf[i++] = app->crc16_2nd26K[0];	// ��26K CRC16
			Args.buf[i++] = app->crc16_2nd26K[1];	
			Args.buf[i++] = app->crc16_all52K[0];	// ��52K CRC16
			Args.buf[i++] = app->crc16_all52K[1];	
			Args.buf[i++] = 0x00;						// RXD�ŵ� 0/1
			memset(&Args.buf[i], 0x00, 7);				// Ԥ�� 7 byte	
			i += 7;
			Args.lastItemLen = i - 1;
			break;

		case WaterCmd_SendUpgradePacket:			// ������������
			/*---------------------------------------------*/
			if(cmdState == Cmd_Finish){
				ClearMissPktFlags(pkt);
				IsNoAckCmd = false;
				CurrCmd++;
				cmdState = Cmd_Send;
				docIdx = 0;
				break;
			}
			if(cmdState == Cmd_RecvOk){
				if(sendIdx == pkt->missPktsCnt){
					cmdState = Cmd_Finish;
				}
				break;
			}

			IsNoAckCmd = true;
			pktIdx = pkt->missPkts[sendIdx];
			sprintf(cmdName, "������������");
			sprintf(cmdMsg, "��ǰ������%d/%d", sendIdx + 1, pkt->missPktsCnt);
			sendIdx++;

			Args.buf[i++] = 0x72;		// ������	72
			ackLen = 0;					// Ӧ�𳤶� 0	
			// ������
			Args.buf[i++] = (uint8)(pktIdx & 0xFF);	// �����
			Args.buf[i++] = (uint8)(pktIdx >> 8);
			Args.buf[i++] = app->crc16_appVer[0];	// �汾CRC16
			Args.buf[i++] = app->crc16_appVer[1];	
			u16Tmp = CopyPktToBuf(pkt, pktIdx, &Args.buf[i]); 	// ������
			crc16 = GetCrc16(&Args.buf[i], u16Tmp, 0x8408);
			i += u16Tmp;
			Args.buf[i++] = (uint8)(crc16 & 0xFF);	// ������CRC16
			Args.buf[i++] = (uint8)(crc16 >> 8);
			Args.lastItemLen = i - 1;
			break;
		
		case WaterCmd_QueryUpgradeStatus_OnBoot:	// ��ѯ����״̬_��boot
			/*---------------------------------------------*/
			if(cmdState == Cmd_Finish){
				GetMissPktList(pkt);
				if(pkt->missPktsCnt > 0){
					CurrCmd = WaterCmd_SendUpgradePacket;
					sendIdx = 0;
				}
				CurrCmd++;
				cmdState = Cmd_Send;
				docIdx = 0;
				break;
			}
			if(cmdState == Cmd_RecvOk){
				dispIdx = 0;
				index = 0;
				ptr = Water6009_GetStrUpgradeStatus(pData[index]);
				dispIdx += sprintf(&cmdMsg[dispIdx], "����״̬: %s\n", ptr);
				index += 1;
				index += 40;	// skip ver 40 byte
				AddMissPktFlags(pkt, &pData[index], 52);
				GetMissPktList_CurrPkt(&pData[index], 52, pkt->packetCnt, NULL, &u16Tmp);
				dispIdx += sprintf(&cmdMsg[dispIdx], "��ǰȱ��: %d\n", u16Tmp);

				if(docIdx == docs->cnt){
					cmdState = Cmd_Finish;
				}
				break;
			}
			if(cmdState == Cmd_RecvNg){
				if(docIdx == docs->cnt){
					cmdState = Cmd_Finish;
				}
				break;
			}

			docIdx++;
			sprintf(cmdName, "��ѯ����״̬-B");
			sprintf(cmdMsg, "");
			Args.buf[i++] = 0x73;		// ������	73
			ackLen = 93;				// Ӧ�𳤶� 93	

			// ������
			Args.buf[i++] = app->packetCnt[0];		// �ܰ���
			Args.buf[i++] = app->packetCnt[1];	
			Args.buf[i++] = app->crc16_appVer[0];	// �汾CRC16
			Args.buf[i++] = app->crc16_appVer[1];	
			Args.lastItemLen = i - 1;
			break;

		case WaterCmd_QueryUpgradeStatus_OnApp:		// ��ѯ����״̬_��app
			/*---------------------------------------------*/
			if(cmdState == Cmd_Finish){
				cmdState = Cmd_Exit;
				break;
			}
			if(cmdState == Cmd_RecvOk){
				dispIdx = 0;
				index = 0;
				ptr = Water6009_GetStrUpgradeStatus(pData[index]);
				dispIdx += sprintf(&cmdMsg[dispIdx], "����״̬: %s\n", ptr);
				index += 1;
				ptr = GetVersionNo(&pData[index], 40);
				dispIdx += sprintf(&cmdMsg[dispIdx], "��ǰ�汾: %s\n", ptr);

				if(docIdx == docs->cnt){
					cmdState = Cmd_Finish;
				}
				break;
			}
			if(cmdState == Cmd_RecvNg){
				if(docIdx == docs->cnt){
					cmdState = Cmd_Finish;
				}
				break;
			}
			
			docIdx++;
			sprintf(cmdName, "��ѯ����״̬-A");
			sprintf(cmdMsg, "");
			Args.buf[i++] = 0x74;		// ������	74
			ackLen = 41;				// Ӧ�𳤶� 41	

			// ������
			Args.buf[i++] = app->packetCnt[0];		// �ܰ���
			Args.buf[i++] = app->packetCnt[1];	
			Args.buf[i++] = app->crc16_appVer[0];	// �汾CRC16
			Args.buf[i++] = app->crc16_appVer[1];	
			Args.lastItemLen = i - 1;
			break;

		default: 
			break;
		}

		_GUIRectangleFill(0, 3*16, 160, 7*16, Color_White);
		PrintfXyMultiLine_VaList(0, 3*16, "��ţ�\r\n  %s", mtrNo);
		PrintfXyMultiLine_VaList(0, 5*16, "--> %s", cmdName);	
		PrintfXyMultiLine_VaList(0, 6*16, "%s", cmdMsg);		

		if(cmdState == Cmd_Exit){
			break;
		}
		if(cmdState == Cmd_RecvOk || cmdState == Cmd_RecvNg){
			_Sleep(500);
			cmdState = Cmd_Send;
			continue;
		}
		if(cmdState == Cmd_Finish){
			_Sleep(500);
			continue;
		}

		// ��ַ���
		Water6009_PackAddrs(&Addrs, StrDstAddr, StrRelayAddr);

		// Ӧ�𳤶ȡ���ʱʱ�䡢�ط�����
		ackLen += 14 + Addrs.itemCnt * AddrLen;
		timeout = 8000 + (Addrs.itemCnt - 2) * 6000 * 2;
		tryCnt = 3;

		if(IsNoAckCmd){
			ackLen = 0;
			timeout = 0;
			tryCnt = 1;
		}

		// ���͡����ա������ʾ
		cmdResult = CommandTranceiver(CurrCmd, &Addrs, &Args, ackLen, timeout, tryCnt);

		if(LcdOpened && lcdCtrl > 4){	
			lcdCtrl = 0;
		}

		if(cmdResult == CmdResult_Cancel){	// ȡ��
			break;
		}
		else if(cmdResult == CmdResult_Ok){
			cmdState = Cmd_RecvOk;
			u16Tmp = DispBuf[0] + DispBuf[1] * 256;
			pData = &RxBuf[u16Tmp];
		}
		else if(cmdResult == CmdResult_CrcError){
			cmdState = Cmd_RecvNg;
			sprintf(cmdMsg, "����ʧ��: CRC����");
		}
		else{
			cmdState = Cmd_RecvNg;
			sprintf(cmdMsg, "����ʧ��: �ȴ���ʱ");
		}
	}

	if(cmdResult == CmdResult_Cancel){
		_Printfxy(0, 9*16, "����  <��ȡ��>  ȷ��", Color_White);
		#if RxBeep_On
		_SoundOn();
		_Sleep(100);
		_SoundOff();
		#endif
	}
	else{
		_Printfxy(0, 9*16, "����  <�����>  ȷ��", Color_White);
		#if RxBeep_On
		_SoundOn();
		_Sleep(200);
		_SoundOff();
		#endif
	}
	

	//sprintf(strTmp, "�ɹ�:%d", meters->readOkCnt);
	//_Printfxy(6*16, 7*16 + 8 + 3, strTmp, Color_White);
	
	while(1){
		key = _ReadKey();
		if(key == KEY_CANCEL || KEY_ENTER){
			break;
		}
		_Sleep(100);
	}

	return state;
}

// ��ѯ����״̬
static void UpgradeFunc_QueryUpgradeState(uint8 upgradeMode)
{

}

//------------------------		����		-----------------
void UpgradeFunc(void)
{
	uint8 key, menuItemNo, u8Tmp;
	UI_Item * pUi = &UiList.items[0];
	uint8 * pUiCnt = &UiList.cnt;
	uint8 * fileName, *ptr;
	uint8 version[20];
	uint8 currUi = 0, uiRowIdx;
	uint8 upgradeMode = 1;	// ����ģʽ�� 1 - ���� �� 2 - ����
	DocInfo *pDocInfo;

#ifdef Project_6009_RF
	ListBox menuList;
	_ClearScreen();

	ListBoxCreate(&menuList, 16, 3*16, 16, 2, 2, NULL,
		"������ʽ", 
		2,
		"1. ��������",
		"2. ��������"
	);
	key = ShowListBox(&menuList);

	if (key == KEY_CANCEL){	// ����
		return;
	}
	upgradeMode = menuList.strIdx + 1;
#endif

	version[0] = 0x00;

	// �˵�
	(*pUiCnt) = 0;
	uiRowIdx = 1;
	if(upgradeMode == 1){
		_Printfxy(0, 0, "<<��������", Color_White);
	}
	else{
		_Printfxy(0, 0, "<<��������", Color_White);
	}
	_GUIHLine(0, 1*16 + 4, 160, Color_Black);	
	/*---------------------------------------------*/
	ButtonCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16 + 8, "1. ѡ�������ļ�");	
	if(upgradeMode == 1){
		LableCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16 + 8, "2. ������");	
		TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16 + 8, "  > ", StrDstAddr, (AddrLen * 2), (AddrLen * 8 + 8), true);
	}
	else{
		ButtonCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16 + 8, "2. ������������");	
	}
	ButtonCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16 + 8, "3. ��ʼ����");	
	ButtonCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16 + 8, "4. ��ѯ����״̬");				
	//----------------------------------------------
	_GUIHLine(0, 9*16 - 4, 160, Color_Black);
	_Printfxy(0, 9*16, "����            ȷ��", Color_White);

	while(1){
		_ClearScreen();

		_Printfxy(0, 0, "<<��������", Color_White);
		_GUIHLine(0, 1*16 + 4, 160, Color_Black);	
		/*---------------------------------------------*/
		// ui ctrl
		_Printfxy(0, 7*16 + 8, version, Color_White);
		//----------------------------------------------
		_GUIHLine(0, 9*16 - 4, 160, Color_Black);
		_Printfxy(0, 9*16, "����            ȷ��", Color_White);
		key = ShowUI(UiList, &currUi);
		//------------------------------------------------------------
		if (key == KEY_CANCEL){	// ����
			break;
		}
		menuItemNo = (upgradeMode == 1 && currUi > 0) ? currUi : currUi + 1;

		if(upgradeMode == 1){	// �������� ��� 
			if(StrDstAddr[0] >= '0' && StrDstAddr[0] <= '9' ){
				DocList.clear(&DocList);
				pDocInfo = (DocInfo *)DocList.add(&DocList, NULL, sizeof(DocInfo));
				memcpy(pDocInfo->mtrNo, StrDstAddr, 20);
				pDocInfo->state = UpgrdState_NotStart;
			}
		}

		if(menuItemNo > 2){
			if(fileName == NULL){
				ShowMsg(16, 4*16, "����ѡ�������ļ���", 2000);
				continue;
			}
			if(UpgrdDocs.cnt == 0){
				ShowMsg(16, 4*16, "������Ų���Ϊ�գ�", 2000);
				continue;
			}
		}

		switch (menuItemNo)
		{
		case 1:	// ѡ�������ļ�
			{
				_ClearScreen();
				fileName = _GetFileList("ѡ|\n��|\n��|\n��|\n��|\n��|\n  |\n  |\n  |\n  |\n", "BIN", "");
				if(fileName == NULL){
					continue;
				}
				_ClearScreen();
				_Printfxy(0, 5*16, "  �ļ�У����... ", Color_White);
				u8Tmp = InitPktInfo(&PktInfo, fileName, 128, 128, &AppInfo);
				if(u8Tmp != 0){
					if(u8Tmp == 2){
						ShowMsg(16, 4*16, "��ѡ����ȷ�������ļ���", 2000);
					}
					else if(u8Tmp == 3){
						ShowMsg(16, 4*16, "��������CRC16����", 2000);
					}
					else if(u8Tmp == 4){
						ShowMsg(16, 4*16, "�汾��CRC16����", 2000);
					}
					fileName = NULL;
					version[0] = 0x00;
				}
				else{
					ptr = GetVersionNo(PktInfo.version, 40);
					sprintf(version, "v%s  CRC:%4X", ptr, PktInfo.verCrc16);
				}
			}
			break;

		case 2:	// ������������
			{
				if(upgradeMode == 2){
					UpgradeFunc_UpgradeDocMgmt();
				}
				currUi++;
			}
			break;

		case 3:	// ��ʼ����
			{
				UpgradeFunc_UpgradeStart(upgradeMode);
			}
			break;

		case 4:	// ��ѯ����״̬
			{
				UpgradeFunc_QueryUpgradeState(upgradeMode);
			}
			break;

		default:
			break;
		}

	}
}

//------------------------- 	�ӿں���		-----------------------

/*
* ��  ������ʼ�����ݰ��ṹ
* ��  ����
*		pktInfo	- ���ݰ���Ϣ�ṹ ��set pktInfo.all��
*		fileName - �ļ���
*		pktSize - �ְ���С
*		pktStartIdx - ���ݰ����ļ��е���ʼλ��
*		header - ͷ����Ϣ������ 0 -- pktStartIdx
* ����ֵ��int  - ���ݰ���ʼ����� 0 - �ɹ��� 1 - �ļ������ڣ� 
*				2 - app�ļ����� 3 - ����CRC16���� 4 - �汾CRC16����
*/
extern int InitPktInfo(PacketInfo *pktInfo, char *fileName, uint16 pktSize, uint32 pktStartIdx, void *header)
{
	int fp;
	uint32 fileLen, readLen;
	uint16 crc16, crc16Keep = 0xFFFF;
	uint8 *tmpBuf = &DispBuf[0], cnt = 0;
	AppFileInfo *app;
	volatile uint32 timeTick;

	if(_Access(fileName, 0) < 0){
		return 1;
	}

	// test -->
	timeTick = _GetTickCount();
	// -->end

	fp = _Fopen(fileName, "R");
	fileLen = _Filelenth(fp);
	if(header != NULL){
		_Fread(header, pktStartIdx, fp);
	}
	_Lseek(fp, pktStartIdx, 0);
	while(0 == _Feof(fp) && cnt < 26){
		readLen = _Fread(tmpBuf, 2048, fp);
		crc16 = GetCrc16_Continue(tmpBuf, readLen, 0x8408, &crc16Keep);
		cnt++;
	}
	_Fclose(fp);

	app = (AppFileInfo *)header;

	// test -->
	timeTick = _GetTickCount() - timeTick;
	_DoubleToStr(&DispBuf[200], (double)timeTick / 32768, 3);
	sprintf(DispBuf, "Tick: %d, time: %s, app-crc16: %4X,  crc16: %4X", 
		timeTick, &DispBuf[200], (app->crc16_all52K[0] + app->crc16_all52K[1] * 256),  crc16);
	ShowMsg(16, 32, DispBuf, 10000);
	// -->end

	if(fileLen <= pktStartIdx 
		|| app->appVer[0] != 'S' || app->appVer[1] != 'R' || app->appVer[2] != 'W' || app->appVer[3] != 'F'){
		return 2;
	}

	if((app->crc16_all52K[0] + app->crc16_all52K[1] * 256) != crc16){
		//return 3;
	}

	crc16 = GetCrc16(app->appVer, 40, 0x8408);
	if((app->crc16_appVer[0] + app->crc16_appVer[1] * 256) != crc16){
		//return 4;
	}

	pktInfo->fileCrc16 = (app->crc16_all52K[0] + app->crc16_all52K[1] * 256);
	pktInfo->verCrc16 = (app->crc16_appVer[0] + app->crc16_appVer[1] * 256);
	pktInfo->version = app->appVer;
	app->crc16_1st26K[0] = 0x00;	// appver str end

	pktInfo->fileName = fileName;
	pktInfo->packetSize = pktSize;
	pktInfo->fileSize = fileLen - pktStartIdx;
	pktInfo->fileKbSize = (uint16)((pktInfo->fileSize + 1023) / 1024);
	pktInfo->packetCnt = (pktInfo->fileSize + pktInfo->packetSize - 1) / pktInfo->packetSize;
	pktInfo->pktStartIdx = pktStartIdx;
	if(pktInfo->fileSize % pktInfo->packetSize != 0){
		pktInfo->lastPktSize = pktInfo->fileSize % pktInfo->packetSize;
	}else{
		pktInfo->lastPktSize = pktInfo->packetSize;
	}
	pktInfo->bitFlagsCnt = (pktInfo->packetCnt + 7) / 8;

	ClearMissPktFlags(pktInfo);

	return 0;
}

/*
* ��  �����������ݰ�������
* ��  ����
*		pktInfo	- ���ݰ���Ϣ�ṹ ��get��
*		pktIdx - Ҫ���������ݰ����
*		buf - Ŀ�Ļ���
* ����ֵ��	int �������ֽ���
*/
extern int  CopyPktToBuf(PacketInfo *pktInfo, uint16 pktIdx, uint8 *buf)
{
	int fp;
	int len = (pktInfo->packetCnt - 1 == pktIdx ? pktInfo->lastPktSize : pktInfo->packetSize);

	fp = _Fopen(pktInfo->fileName, "R");
	_Lseek(fp, pktInfo->pktStartIdx + pktInfo->packetSize * pktIdx, 0);
	_Fread(buf, len, fp);
	_Fclose(fp);

	return len;
}

/*
* ��  �������ȱ����ǣ�ȱ������Ϊ�ܰ�����ȫ����ʼ��Ϊ0xFF�����ۼ�,�磺0xFF & 0x23 & 0x7F��
* ��  ����
*		pktInfo	- ���ݰ���Ϣ�ṹ ��set pktInfo.bitFlags��
* ����ֵ��void
*/
extern void ClearMissPktFlags(PacketInfo *pktInfo)
{
	memset(pktInfo->bitFlags, 0xFF, pktInfo->bitFlagsCnt);
	pktInfo->missPktsCnt = pktInfo->packetCnt;
}

/*
* ��  �������ȱ�����
* ��  ����
*		pktInfo	- ���ݰ���Ϣ�ṹ ��set pktInfo.bitFlags��
*		bitflags - ��ǰȱ��������ݻ��棬���/�ϲ����ܱ����
*		byteCnt - ��ǰȱ������ֽ���
* ����ֵ��void
*/
extern void AddMissPktFlags(PacketInfo *pktInfo, uint8 *bitflags, uint16 byteCnt)
{
	uint8 * p8 = &pktInfo->bitFlags[0];

	while(byteCnt > 0){
		*p8++ &= *bitflags++;
		byteCnt--;
	}

	pktInfo->missPktsCnt = 0xFFFF;	// set to unknown
}

/*
* ��  ������ȡȱ���б� - �ۼ����а���ȱ����
* ��  ����
*		pktInfo	- ���ݰ���Ϣ�ṹ ��set pktInfo.missPktsCnt��set pktInfo.missPkts��
* ����ֵ��void
*/
extern void GetMissPktList(PacketInfo *pktInfo)
{
	uint16 i, k, pktIdx = 0;
	uint8 b;
	pktInfo->missPktsCnt = 0;

	for(i = 0; i < pktInfo->bitFlagsCnt; i++){
		b = pktInfo->bitFlags[i];
		if (b == 0xFF){
			pktIdx += 8;
			continue;
		}
		for (k = 0; k < 8; k++){
			if (pktIdx >= pktInfo->packetCnt){
				break;
			}
			if ((b & 0x01) == 0){
				pktInfo->missPkts[pktInfo->missPktsCnt++] = pktIdx;
			}
			b = (uint8)(b >> 1);
			pktIdx++;
		}
	}
}

/*
* ��  ������ȡȱ���б�	- ��ǰ����ȱ����
* ��  ����
*		bitfalgs	- ȱ����ǻ���
*		byteCnt		- ȱ����ǻ����ֽ���
*		pktCnt		- �ܰ���
*		missPkts	- ȱ���б���, ��ΪNULL����ֻ����ȱ����
*		missCnt		- ȱ����
* ����ֵ��void
*/
extern void GetMissPktList_CurrPkt(uint8 *bitfalgs, uint16 byteCnt, uint16 pktCnt, uint16 *missPkts, uint16 *missCnt)
{
	uint16 i, k, pktIdx = 0;
	uint8 b;
	*missCnt = 0;

	for(i = 0; i < byteCnt; i++){
		b = bitfalgs[i];
		if (b == 0xFF){
			pktIdx += 8;
			continue;
		}
		for (k = 0; k < 8; k++){
			if (pktIdx >= pktCnt){
				break;
			}
			if ((b & 0x01) == 0){
				if(missPkts != NULL){
					missPkts[*missCnt] = pktIdx;
				}
				(*missCnt)++;
			}
			b = (uint8)(b >> 1);
			pktIdx++;
		}
	}
}

