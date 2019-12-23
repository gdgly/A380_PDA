
#include "HJLIB.H"
#include "string.h"
#include "dbf.h"
#include "stdio.h"

#include "MeterDocDBF.h"
#include "Common.h"
#ifdef Project_6009_RF
#include "WaterMeter.h"
#else
#include "WaterMeter8009.h"
#endif

//-----------------------	ȫ�ֱ�������
MeterInfoSt MeterInfo;
DistrictListSt Districts;
BuildingListSt Buildings;
MeterListSt Meters;
DbQuerySt DbQuery;
uint8 **MetersStrs = Meters.strs;

//----------------------	���ݿ���Ϣ-��������		-------------------------------------

/*
* �� �����ַ����б��в����ַ���
* �� ����strs		- �ַ����б�
*		strLen		- ÿ���ַ�������
*		strCnt		- �ַ�������
*		dstStr		- ���ҵ��ַ���
*		cmpMaxLen	- �Ƚϵ���󳤶�
* �� �أ�int	- �ҵ����ַ������б��е������� -1 - δ�ҵ��� 0~n - �ҵ�	
*/
int FindStrInList(char ** strs, uint8 strLen, uint16 strCnt, const char *dstStr, uint16 cmpMaxLen)
{
	int i = -1;
	char *str = (char *)strs;

	for(i = 0; i < strCnt; i++){

		if(strncmp(str, dstStr, cmpMaxLen) == 0){
			break;
		}
		str += strLen;
	}
	if(i >= strCnt){
		i = -1;
	}

	return i;
}

/*
* �� ������ѯС���б�
* �� ����districts	- С���б�
*		 query		- ���ݿ��ѯ�ṹ
* �� �أ�void
*/
void QueryDistrictList(DistrictListSt *districts, DbQuerySt *query)
{
	uint32 i, recCnt;
	char strTmp1[Size_DbMaxStr];
	char strTmp2[Size_DbMaxStr];

	_Select(1);
	_Use(MeterDocDB);	// �����ݿ�
	recCnt = _Reccount();
	_Go(0);
	districts->cnt = 0;
	districts->idx = 0;
	query->reqMaxCnt = District_Max;
	query->resultCnt = 0;
	query->errorCode = 0;
	for(i = 0; i < recCnt; i++){
		_ReadField(Idx_DistrictNum, strTmp1);		// С�����
		strTmp1[Size_ListStr - 1] = '\0';

		// �ų��ظ��󣬼����б���
		if(-1 == FindStrInList(districts->nums, Size_ListStr, districts->cnt, strTmp1, Size_ListStr)){
			
			query->resultCnt++;
			if(query->resultCnt > query->reqMaxCnt){
				query->errorCode = 1;
				break;
			}

			_ReadField(Idx_DistrictName, strTmp2);	// С������
			strTmp2[Size_ListStr - 1] = '\0';

			strncpy(districts->nums[districts->cnt], strTmp1, Size_ListStr);
			strncpy(districts->names[districts->cnt], strTmp2, Size_ListStr);
			districts->cnt++;
		}
		_Skip(1);	// ��һ�����ݿ��¼
	}
	_Use("");		// �ر����ݿ�

	query->dbCurrIdx = i;
}

/*
* �� ������ѯxxС��-¥���б�
* �� ����buildings	- ¥���б�
*		 query		- ���ݿ��ѯ�ṹ
* �� �أ�void
*/
void QueryBuildingList(BuildingListSt *buildings, DbQuerySt *query)
{
	uint32 i, recCnt;
	char strTmp[Size_DbMaxStr];
	char strTmp1[Size_DbMaxStr];
	char strTmp2[Size_DbMaxStr];

	_Select(1);
	_Use(MeterDocDB);	// �����ݿ�
	recCnt = _Reccount();
	_Go(0);
	buildings->cnt = 0;
	buildings->idx = 0;
	query->reqMaxCnt = Building_Max;
	query->resultCnt = 0;
	query->errorCode = 0;
	for(i = 0; i < recCnt; i++){

		_ReadField(Idx_DistrictNum, strTmp);		// С����� ����
		strTmp[Size_ListStr - 1] = '\0';
		if(strcmp(buildings->qryDistricNum, strTmp) != 0){
			_Skip(1);	// ��һ�����ݿ��¼
			continue;
		}

		_ReadField(Idx_BuildingNum, strTmp1);		// ¥�����
		strTmp1[Size_ListStr - 1] = '\0';

		// �ų��ظ��󣬼����б���
		if(-1 == FindStrInList(buildings->nums, Size_ListStr, buildings->cnt, strTmp1, Size_ListStr)){
			
			query->resultCnt++;
			if(query->resultCnt > query->reqMaxCnt){
				query->errorCode = 1;
				break;
			}

			_ReadField(Idx_BuildingName, strTmp2);	// ¥������
			strTmp2[Size_ListStr -1] = '\0';

			strncpy(buildings->nums[buildings->cnt], strTmp1, Size_ListStr);
			strncpy(buildings->names[buildings->cnt], strTmp2, Size_ListStr);
			buildings->cnt++;
		}
		_Skip(1);	// ��һ�����ݿ��¼
	}
	_Use("");		// �ر����ݿ�

	query->dbCurrIdx = i;
}

/*
* �� ������ѯxxС��-xx¥��-����ͳ�����
* �� ����meters		- �����б���ѯǰ������ meters.(С�����/¥�����/����״̬/��ʾ�ֶ�)
*		 query		- ���ݿ��ѯ�ṹ
* �� �أ�void
*/
void QueryMeterList(MeterListSt *meters, DbQuerySt *query)
{
	uint32 i, recCnt;
	char strTmp[Size_DbMaxStr];

	_Select(1);
	_Use(MeterDocDB);	// �����ݿ�
	recCnt = _Reccount();
	_Go(0);
	meters->cnt = 0;
	meters->idx = 0;
	meters->districName[0] = 0x00;
	meters->buildingName[0] = 0x00;
	meters->meterCnt = 0;
	meters->readOkCnt = 0;
	meters->readNgCnt = 0;
	query->reqMaxCnt = Meter_Max;
	query->resultCnt = 0;
	query->errorCode = 0;
	for(i = 0; i < recCnt; i++){

		if(meters->qryDistricNum != NULL){
			_ReadField(Idx_DistrictNum, strTmp);		// С����� ����
			strTmp[Size_ListStr - 1] = '\0';
			if(strcmp(meters->qryDistricNum, strTmp) != 0){
				_Skip(1);	// ��һ�����ݿ��¼
				continue;
			}
			else{
				// ����С����
				if(meters->districName[0] == 0x00){
					_ReadField(Idx_DistrictName, strTmp);	
					strTmp[Size_DistrictName - 1] = '\0';
					strcpy(meters->districName, strTmp);
				}
			}
		}
		if(meters->qryBuildingNum != NULL){
			_ReadField(Idx_BuildingNum, strTmp);		// ¥����� ����
			strTmp[Size_ListStr - 1] = '\0';
			if(strcmp(meters->qryBuildingNum, strTmp) != 0){
				_Skip(1);	// ��һ�����ݿ��¼
				continue;
			}
			else{
				// ����¥����
				if(meters->buildingName[0] == 0x00){
					_ReadField(Idx_BuildingName, strTmp);	
					strTmp[Size_BuildingName - 1] = '\0';
					strcpy(meters->buildingName, strTmp);
				}
			}
		}

		meters->meterCnt++;				// ��ǰ������
		_ReadField(Idx_MeterReadStatus, strTmp);		
		strTmp[Size_MeterReadStatus - 1] = '\0';
		if(strcmp(strTmp, "1") == 0){
			meters->readOkCnt++;		// �ɹ�����
		}
		else if(strcmp(strTmp, "2") == 0){
			meters->readNgCnt++;		// ʧ������
		}else{
			// 0 - δ������
		}
		
		if(meters->qryMeterReadStatus != NULL){			// ����״̬ ����  ��0�� - δ��/ʧ�ܣ� ��1�� - �ѳ�
			if((meters->qryMeterReadStatus[0] == '1' && strTmp[0] != '1')
				|| (meters->qryMeterReadStatus[0] == '0' && strTmp[0] == '1')){
				_Skip(1);	// ��һ�����ݿ��¼
				continue;
			}
		}

		switch (meters->selectField)		// �б����ͣ�Ĭ��Ϊ����б�
		{
		case Idx_MeterNum:
		case Idx_UserNum:
		case Idx_UserRoomNum:
		case Idx_UserName:
		case Idx_UserAddrs:
			break;
		default: 
			meters->selectField = Idx_Invalid;
			break;
		}
		if(meters->selectField == Idx_Invalid){		// δѡ���ֶΣ��򲻹����б�
			_Skip(1);	// ��һ�����ݿ��¼
			continue;
		}

		_ReadField(meters->selectField, strTmp);	// ��ȡ�ֶΣ����/����/���ƺ�/����/��ַ
		strTmp[Size_ListStr - 1] = '\0';			// �����ʾһ��

		query->resultCnt++;
		if(query->resultCnt > query->reqMaxCnt){
			query->errorCode = 1;
			break;
		}

		// ��ѡ����ֶ���Ϣ �� ���ݿ����� �����б�
		strncpy(meters->strs[meters->cnt], strTmp, Size_ListStr);	
		meters->dbIdx[meters->cnt] = (i + 1);	// ���ݿ������� 1 ��ʼ���
		meters->cnt++;

		_Skip(1);	// ��һ�����ݿ��¼
	}
	_Use("");		// �ر����ݿ�

	query->dbCurrIdx = i;
}


/*
* �� ������ʾ xxС��-xx¥��- �Զ�����
* �� ����meters		- ��������б�
* �� �أ�uint8 	- �����˳�ʱ�İ���ֵ�� KEY_CANCEL - ���ؼ� �� KEY_ENTER - ȷ�ϼ�
*/
uint8 ShowAutoMeterReading(MeterListSt *meters)
{
	uint8 key, i, cnt;
	uint16 ackLen, timeout, dispIdx;
	uint8 tryCnt, lcdCtrl;
	CmdResult cmdResult = CmdResult_Ok;
	char *dispBuf = &DispBuf;
	MeterInfoSt *meterInfo = &MeterInfo;
	char strTmp[20];
	uint32 shutdownTime;

	if(meters->cnt == 0){
		return KEY_CANCEL;
	}

	// �м����
	for(i = 0; i < RELAY_MAX; i++){				
		if(StrRelayAddr[i][0] > '9' || StrRelayAddr[i][0] < '0'){
			StrRelayAddr[i][0] = 0x00;
		}
	}

	// ��ʼ��
	cnt = 0;	
	meters->readOkCnt = 0;
	meters->readNgCnt = 0;
	lcdCtrl = 0;

	// ��ֹ�Զ�����ʱ�ػ��������Զ��ػ�ʱ��
	shutdownTime = _GetShutDonwTime();
	_SetShutDonwTime(0);		// 20 - 999 ��Ч��0 - �ر��Զ��ػ�

	// �Զ�����
	while(cnt < meters->cnt){

		#if LOG_ON
			CurrCmdName = strTmp;
			sprintf(CurrCmdName, "�������� %d/%d", (cnt + 1), meters->cnt); 
		#endif

		// LCD�����ƿ���
		LcdLightCycleCtrl(&lcdCtrl, 4);
		
		// ��ȡ��ǰ������Ϣ
		meterInfo->dbIdx = meters->dbIdx[cnt];
		QueryMeterInfo(meterInfo, &DbQuery);

		// �Զ�����-������ʾ
		_ClearScreen();
		//PrintfXyMultiLine_VaList(0, 0, "<<�Զ�����  %3d/%-3d ", (cnt + 1), meters->cnt);
		_Printfxy(0, 0, "<<�Զ�����", Color_White);
		_GUIHLine(0, 1*16 + 4, 160, Color_Black);	
		/*---------------------------------------------*/
		//dispBuf = &DispBuf;
		dispIdx = 0;
		dispIdx += sprintf(&dispBuf[dispIdx], "���: %s\n", meterInfo->meterNum);
		dispIdx += sprintf(&dispBuf[dispIdx], "����: %s\n", meterInfo->userNum);
		dispIdx += sprintf(&dispBuf[dispIdx], "��ַ: %s\n", meterInfo->userAddr);
		PrintfXyMultiLine(0, 1*16 + 8, dispBuf, 7);	

		sprintf(strTmp, "��ǰ����: %d/%d", (cnt + 1), meters->cnt);
		_Printfxy(0, 6*16 + 8, strTmp, Color_White);
		_GUIHLine(0, 7*16 + 8 + 1, 160, Color_Black);
		ShowProgressBar(7*16 + 8 + 3, meters->cnt, cnt + 1);	// ������
		//----------------------------------------------
		_GUIHLine(0, 9*16 - 4, 160, Color_Black);
		_Printfxy(0, 9*16, "  <  �ֳ���...   >  ", Color_White);
		
		// �������
		strcpy(StrDstAddr, meterInfo->meterNum);
		
		// �����������
		i = 0;	
		CurrCmd = WaterCmd_ReadRealTimeData;	// ��ʵʱ����
		Args.itemCnt = 2;
		Args.items[0] = &Args.buf[0];   // ������
		Args.items[1] = &Args.buf[1];	// ������
		#ifdef Project_6009_RF
			Args.buf[i++] = 0x01;		// ������	01
			ackLen = 21;				// Ӧ�𳤶� 21	
			Args.buf[i++] = 0x00;		// ���ݸ�ʽ 00	
		#else // Project_8009_RF
			Args.buf[i++] = 0x01;		// ������	01
			ackLen = 9;					// Ӧ�𳤶� 9	
		#endif
		
		Args.lastItemLen = i - 1;

		// ��ַ���
		#ifdef Project_6009_RF
			Water6009_PackAddrs(&Addrs, StrDstAddr, StrRelayAddr);
		#else
			Water8009_PackAddrs(&Addrs, StrDstAddr, StrRelayAddr);
		#endif

		// Ӧ�𳤶ȡ���ʱʱ�䡢�ط�����
		#ifdef Project_6009_RF
			ackLen += 15 + Addrs.itemCnt * AddrLen;
			timeout = 8000 + (Addrs.itemCnt - 2) * 6000 * 2;
			tryCnt = 2;
		#else // Project_8009_RF
			ackLen += 10 + Addrs.itemCnt * AddrLen;
			timeout = 2000 + (Addrs.itemCnt - 1) * 2000;
			tryCnt = 2;
		#endif

		// ���͡����ա������ʾ
		cmdResult = CommandTranceiver(CurrCmd, &Addrs, &Args, ackLen, timeout, tryCnt);

		if(cmdResult == CmdResult_Cancel){	// ȡ���ֳ�
			break;
		}
		else if(cmdResult == CmdResult_Ok){
			// �ɹ��������������ݿ�
			
			_Printfxy(0, 9*16, " < ��ǰ����: �ɹ� > ", Color_White);
			meters->readOkCnt++;
		}
		else{
			// ʧ�ܣ���������
			_Printfxy(0, 9*16, " < ��ǰ����: ʧ�� > ", Color_White);
			meters->readNgCnt++;
		}
		meterInfo->meterReadType[0] = '0';		// ����ʽ�� �ƻ�����
		SaveMeterReadResult(meterInfo);
		_Sleep(1000);

		// ��ʾ����
		_Printfxy(0, 9*16, "                   ", Color_White);
		_DispTimeSys();	
		_Sleep(1000);

		cnt++;
	}

	_OpenLcdBackLight();

	// �ֳ������� ȡ���շ�ʱ�ص�����
	//TranceiverCycleHook = NULL;

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
	

	sprintf(strTmp, "�ɹ�:%d", meters->readOkCnt);
	_Printfxy(6*16, 7*16 + 8 + 3, strTmp, Color_White);
	
	while(1){
		key = _ReadKey();
		if(key == KEY_CANCEL || KEY_ENTER){
			break;
		}
		_Sleep(100);
	}

	_SetShutDonwTime(shutdownTime);	
	
	return key;
}

/*
* �� ������ʾxxС��-xx¥��-����ͳ�����
* �� ����meters		- ��������б�
* �� �أ�uint8 	- �����˳�ʱ�İ���ֵ�� KEY_CANCEL - ���ؼ� �� KEY_ENTER - ȷ�ϼ�
*/
uint8 ShowMeterReadCountInfo(MeterListSt *meters)
{
	uint8 key;
	uint16 dispIdx = 0;
	char *dispBuf = &DispBuf;

	_ClearScreen();

	_Printfxy(0, 0, "<<����ͳ��", Color_White);
	/*---------------------------------------------*/
	dispIdx += sprintf(&dispBuf[dispIdx], "С��: %s\n", meters->districName);
	dispIdx += sprintf(&dispBuf[dispIdx], "¥��: %s\n", meters->buildingName);
	dispIdx += sprintf(&dispBuf[dispIdx], "����: %d\n", meters->meterCnt);
	dispIdx += sprintf(&dispBuf[dispIdx], "�ѳ�: %d\n", meters->readOkCnt);
	dispIdx += sprintf(&dispBuf[dispIdx], "δ��: %d\n", meters->readNgCnt);
	//----------------------------------------------
	_Printfxy(0, 9*16, "����  < ��� >  ȷ��", Color_White);

	key = ShowScrollStr(&DispBuf,  7);
	
	return key;
}

/*
* �� ������ʾxxС��-xx¥��-�ѳ�/δ���б�
* �� ����meters		- ��������б�: ����ǰ�����ó���״̬ qryMeterReadStatus
* �� �أ�uint8 	- �����˳�ʱ�İ���ֵ�� KEY_CANCEL - ���ؼ� �� KEY_ENTER - ȷ�ϼ�
*/
uint8 ShowMeterList(MeterListSt *meterReadList)
{
	uint8 key;
	ListBox showTpList, meterList;		// ��ʾ��ʽ/����Ϣ-�б�
	char *title = NULL;
	MeterListSt *meters = meterReadList;

	// �б���ʾ��ʽ-����
	title = (meters->qryMeterReadStatus[0] == '1' ? "<<�ѳ��б�" : "<<δ���б�");
	ListBoxCreate(&showTpList, 0, 0, 20, 7, 4, NULL,
		title, 
		4,
		"1. �������ʾ",
		"2. ��������ʾ",
		"3. ��������ʾ",
		"4. �����ƺ���ʾ");
	while(1){

		_Printfxy(0, 9*16, "����            ȷ��", Color_White);
		key = ShowListBox(&showTpList);
		//------------------------------------------------------------
		if(key == KEY_CANCEL){	// ����
			break;
		}

		switch (showTpList.strIdx + 1){
		case 1:	meters->selectField = Idx_MeterNum;	
			break;
		case 2:	meters->selectField = Idx_UserNum;	
			break;
		case 3:	meters->selectField = Idx_UserName;	
			break;
		case 4:	meters->selectField = Idx_UserRoomNum;	
			break;
		default:
			break;
		}
		// �ѳ�/δ���б�-����
		//------------------------------------------------------------
		_Printfxy(0, 9*16, "    <  ��ѯ��  >    ", Color_White);
		QueryMeterList(meters, &DbQuery);	// �ѳ�/δ���б� ��ѯ
		ListBoxCreateEx(&meterList, 0, 0, 20, 7, meters->cnt, NULL,
				title, meters->strs, Size_ListStr, meters->cnt);
		while(2){

			_Printfxy(0, 9*16, "����        ������Ϣ", Color_White);
			key = ShowListBox(&meterList);
			//------------------------------------------------------------
			if(key == KEY_CANCEL){	// ����
				break;
			}
			if(key == KEY_ENTER && meters->cnt == 0){	
				continue;
			}

			while(3){
				// ������Ϣ-����
				//------------------------------------------------------
				meters->idx = meterList.strIdx;
				MeterInfo.dbIdx = meters->dbIdx[meters->idx];
				MeterInfo.strIdx = meters->idx;
				MeterInfo.strCnt = meters->cnt;
				QueryMeterInfo(&MeterInfo, &DbQuery);	// ������Ϣ��ѯ
				key = ShowMeterInfo(&MeterInfo);	
				//------------------------------------------------------
				if(key == KEY_LEFT){
					if(meterList.strIdx == 0){
						meterList.strIdx = meterList.strCnt - 1;
					}
					else{
						meterList.strIdx--;
					}
				}
				else if(key == KEY_RIGHT){
					if(meterList.strIdx == meterList.strCnt - 1){
						meterList.strIdx = 0;
					}
					else{
						meterList.strIdx++;
					}
				}
				else{	// KEY_CANCEL
					meterList.currIdx = meterList.strIdx;
					break;
				}
			} // while 3 ������Ϣ

		}// while 2 �ѳ�/δ���б�

	}// while 1 ��ʾ��ʽ
	
	return key;
}

/*
* �� �������泭����
* �� ����meterInfo	- ������Ϣ
* �� �أ�void
*/
void SaveMeterReadResult(MeterInfoSt *meterInfo)
{
	char time[20];

	_Select(1);
	_Use(MeterDocDB);	// �����ݿ�
	_Go(meterInfo->dbIdx);

	meterInfo->meterReadStatus[1] = '\0';
	meterInfo->meterReadType[1] = '\0';

	// ���³�����
	_GetDateTime(time, '-', ':');
	_Replace(Idx_MeterReadStatus, meterInfo->meterReadStatus);	// ����״̬ 0 - δ���� 1 - �ɹ��� 2 - ʧ��
	_Replace(Idx_MeterReadType, meterInfo->meterReadType);		// ����ʽ 0 - �ƻ����� �� 1 - �ֹ�¼��
	_Replace(Idx_MeterReadTime, time);							// ����ʱ��
	_Replace(Idx_MeterValue, meterInfo->meterValue);
	_Replace(Idx_MeterStatusHex, meterInfo->meterStatusHex);
	_Replace(Idx_MeterStatusStr, meterInfo->meterStatusStr);
	_Replace(Idx_BatteryVoltage, meterInfo->batteryVoltage);
	_Replace(Idx_SignalValue, meterInfo->signalValue);

	_Use("");			// �ر����ݿ�
}

/*
* �� ������ѯ������Ϣ
* �� ����meterInfo	- ������Ϣ
*		 query		- ���ݿ��ѯ�ṹ
* �� �أ�void
*/
void QueryMeterInfo(MeterInfoSt *meterInfo, DbQuerySt *query)
{
	uint32 i, recCnt;
	char *strTmp = &TmpBuf[0];

	_Select(1);
	_Use(MeterDocDB);	// �����ݿ�
	recCnt = _Reccount();
	_Go(0);
	query->resultCnt = 0;
	query->errorCode = 0;

	if(meterInfo->dbIdx != Invalid_dbIdx){	// ���ݿ��¼���� �Ƿ���Ч��
		if(meterInfo->dbIdx > recCnt -1){
			meterInfo->dbIdx = Invalid_dbIdx;
		}
	}

	if(meterInfo->dbIdx == Invalid_dbIdx){	// ���ݿ��¼������Чʱ ִ�в�ѯ

		#if 1
		if(meterInfo->qryMeterNum != NULL
			&& _LocateEx(Idx_MeterNum, '=', meterInfo->qryMeterNum, 1, recCnt, 0) > 0){ 
			// ����Ų�ѯ
			meterInfo->dbIdx = _Recno();
		}
		else if(meterInfo->qryUserNum != NULL
			&& _LocateEx(Idx_UserNum, '=', meterInfo->qryUserNum, 1, recCnt, 0) > 0){ 
			// �����Ų�ѯ
			meterInfo->dbIdx = _Recno();
		}
		else if(meterInfo->qryRoomNum != NULL
			&& _LocateEx(Idx_UserRoomNum, '=', meterInfo->qryRoomNum, 1, recCnt, 0) > 0){ 
			// �����ƺŲ�ѯ
			meterInfo->dbIdx = _Recno();
		}

		#else

		for(i = 0; i < recCnt; i++){

			if(meterInfo->qryMeterNum != NULL){
				_ReadField(Idx_MeterNum, strTmp);		// ����Ų�ѯ
				strTmp[Size_MeterNum - 1] = '\0';
				if(strcmp(meterInfo->qryMeterNum, strTmp) == 0){
					meterInfo->dbIdx = i + 1;
					break;
				}
			}
			else if(meterInfo->qryUserNum != NULL){
				_ReadField(Idx_UserNum, strTmp);		// �����Ų�ѯ
				strTmp[Size_UserNum - 1] = '\0';
				if(strcmp(meterInfo->qryUserNum, strTmp) == 0){
					meterInfo->dbIdx = i + 1;
					break;
				}
			}
			else if(meterInfo->qryRoomNum != NULL){
				_ReadField(Idx_UserRoomNum, strTmp);	// �����ƺŲ�ѯ
				strTmp[Size_RoomNum - 1] = '\0';
				if(strcmp(meterInfo->qryRoomNum, strTmp) == 0){
					meterInfo->dbIdx = i + 1;
					break;
				}
			}

			_Skip(1);	// ��һ�����ݿ��¼
		}
		#endif
	}

	if(meterInfo->dbIdx != Invalid_dbIdx){	// ���ݿ��¼������Чʱ ��ȡ��¼

		_Go(meterInfo->dbIdx);
		query->resultCnt = 1;

		_ReadField(Idx_MeterNum, strTmp);					// ���
		strncpy(meterInfo->meterNum, strTmp, Size_MeterNum);
		meterInfo->meterNum[AddrLen * 2] = 0x00;	
		StringPadLeft(meterInfo->meterNum, AddrLen * 2, '0');

		_ReadField(Idx_UserNum, strTmp);					// ����
		strncpy(meterInfo->userNum, strTmp, Size_UserNum);	

		_ReadField(Idx_UserRoomNum, strTmp);				// ���ƺ�
		StringFixGbkStrEndError(strTmp);
		strncpy(meterInfo->roomNum, strTmp, Size_RoomNum);	

		_ReadField(Idx_MeterReadStatus, strTmp);			// ����״̬
		strTmp[Size_MeterReadStatus - 1] = '\0';
		strncpy(meterInfo->meterReadStatus, strTmp, Size_MeterReadStatus);	

		_ReadField(Idx_UserName, strTmp);					// ����
		strTmp[Size_UserName - 1] = '\0';
		strncpy(meterInfo->userName, strTmp, Size_UserName);	

		_ReadField(Idx_UserMobileNum, strTmp);			// �ֻ���
		strTmp[Size_MobileNum - 1] = '\0';
		strncpy(meterInfo->mobileNum, strTmp, Size_MobileNum);	

		_ReadField(Idx_UserAddrs, strTmp);				// ��ַ
		StringFixGbkStrEndError(strTmp);
		strncpy(meterInfo->userAddr, strTmp, Size_UserAddr);	

		_ReadField(Idx_MeterReadType, strTmp);			// ����ʽ
		strTmp[Size_MeterReadType - 1] = '\0';
		strncpy(meterInfo->meterReadType, strTmp, Size_MeterReadType);	

		_ReadField(Idx_MeterReadTime, strTmp);			// ����ʱ��
		strTmp[Size_MeterReadTime - 1] = '\0';
		strncpy(meterInfo->meterReadTime, strTmp, Size_MeterReadTime);	

		_ReadField(Idx_MeterValue, strTmp);				// �����
		strTmp[Size_MeterValue - 1] = '\0';
		strncpy(meterInfo->meterValue, strTmp, Size_MeterValue);	

		_ReadField(Idx_MeterStatusStr, strTmp);			// ��״̬
		strTmp[Size_MeterStatusStr - 1] = '\0';
		strncpy(meterInfo->meterStatusStr, strTmp, Size_MeterStatusStr);	

		_ReadField(Idx_BatteryVoltage, strTmp);			// ��ص�ѹ
		strTmp[Size_BatteryVoltage - 1] = '\0';
		strncpy(meterInfo->batteryVoltage, strTmp, Size_BatteryVoltage);	

		_ReadField(Idx_SignalValue, strTmp);			// �ź�ǿ��
		strTmp[Size_SignalValue - 1] = '\0';
		strncpy(meterInfo->signalValue, strTmp, Size_SignalValue);	
	}

	_Use("");		// �ر����ݿ�

	query->dbCurrIdx = meterInfo->dbIdx;
}

/*
* �� ������ʾ������Ϣ
* �� ����meterInfo	- ������Ϣ
* �� �أ�uint8 	- �����˳�ʱ�İ���ֵ�� KEY_CANCEL - ���ؼ� �� KEY_ENTER - ȷ�ϼ�
*/
uint8 ShowMeterInfo(MeterInfoSt *meterInfo)
{
	uint8 key, i;
	uint16 dispIdx = 0;
	ListBox menuList;
	UI_Item * pUi = &UiList.items[0];
	uint8 * pUiCnt = &UiList.cnt;
	uint8 currUi = 0, uiRowIdx, isUiFinish, u8Tmp;
	uint8 tryCnt;
	uint16 ackLen = 0, timeout, u16Tmp;
	uint32 u32Tmp;
	char *dispBuf;

	while(1){
		_ClearScreen();

		// ������Ϣ-����
		//-----------------------------------------------------
		PrintfXyMultiLine_VaList(0, 0, "<<������Ϣ  %3d/%-3d ", meterInfo->strIdx + 1, meterInfo->strCnt);
		dispBuf = &DispBuf;
		dispIdx = 0;
		dispIdx += sprintf(&dispBuf[dispIdx], "���: %s\n", meterInfo->meterNum);
		dispIdx += sprintf(&dispBuf[dispIdx], "����: %s\n", meterInfo->userNum);
		dispIdx += sprintf(&dispBuf[dispIdx], "���ƺ�: %s\n", meterInfo->roomNum);
		dispIdx += sprintf(&dispBuf[dispIdx], "����״̬: %s\n", (meterInfo->meterReadStatus[0] == '1' ? "�ѳ�" : "δ��"));
		dispIdx += sprintf(&dispBuf[dispIdx], "����: %s\n", meterInfo->userName);
		dispIdx += sprintf(&dispBuf[dispIdx], "�ֻ�: %s\n", meterInfo->mobileNum);
		dispIdx += sprintf(&dispBuf[dispIdx], "��ַ: %s\n", meterInfo->userAddr);
		dispIdx += sprintf(&dispBuf[dispIdx], "����ʽ: %s\n", 
			(meterInfo->meterReadType[0] == '0' ? "�ֳֻ�����" : (meterInfo->meterReadType[0] == '1' ? "����������" : " ")));
		dispIdx += sprintf(&dispBuf[dispIdx], "����ʱ��: \n %s\n", meterInfo->meterReadTime);
		dispIdx += sprintf(&dispBuf[dispIdx], "�����: %s\n", meterInfo->meterValue);
		dispIdx += sprintf(&dispBuf[dispIdx], "��״̬: %s\n", meterInfo->meterStatusStr);
		dispIdx += sprintf(&dispBuf[dispIdx], "��ص�ѹ: %s\n", meterInfo->batteryVoltage);
		dispIdx += sprintf(&dispBuf[dispIdx], "�ź�ǿ��: %s", meterInfo->signalValue);
		//----------------------------------------------
		_Printfxy(0, 9*16, "����        ��������", Color_White);
		key = ShowScrollStrEx(dispBuf,  7);
		//----------------------------------------------
		if(key == KEY_CANCEL || key == KEY_LEFT || key == KEY_RIGHT){	// ����
			break;
		}

		
		strcpy(StrDstAddr, meterInfo->meterNum);	// �������
		for(i = 0; i < RELAY_MAX; i++){				// �м����
			if(StrRelayAddr[i][0] > '9' || StrRelayAddr[i][0] < '0'){
				StrRelayAddr[i][0] = 0x00;
			}
		}

		// ��������-����
		ListBoxCreate(&menuList, 3*16, 2*16, 14, 5, 5, NULL, 
			"��������", 
			5, 
			"1. ����",
			"2. ����",
			"3. �ط�",
			"4. ���쳣",
			"5. �ֹ�¼��");
		//---------------------
		key = ShowListBox(&menuList);
		if(key == KEY_CANCEL){	// ȡ��ִ�����������ʾ������Ϣ
			continue;
		}

		// ��������-����
		_ClearScreen();
		CurrCmdName = menuList.str[menuList.strIdx];
		_GUIHLine(0, 1*16 + 4, 160, Color_Black);	
		//--------------------------------------
		sprintf(TmpBuf, "<<%s",&CurrCmdName[3]);
		_Printfxy(0, 0, TmpBuf, Color_White);
		//------------------------------------

		// �����������
		i = 0;	
		Args.itemCnt = 2;
		Args.items[0] = &Args.buf[0];   // ������
		Args.items[1] = &Args.buf[1];	// ������

		switch(menuList.strIdx + 1){

		case 1:
			CurrCmd = WaterCmd_ReadRealTimeData;		// "��ȡ�û�����"
			/*---------------------------------------------*/
			#ifdef Project_6009_RF
				Args.buf[i++] = 0x01;		// ������	01
				ackLen = 21;				// Ӧ�𳤶� 21	
				Args.buf[i++] = 0x00;		// ���ݸ�ʽ 00	
			#else // Project_8009_RF
				Args.buf[i++] = 0x01;		// ������	01
				ackLen = 9;					// Ӧ�𳤶� 9	
			#endif
			Args.lastItemLen = i - 1;
			break;

		case 2:
			CurrCmd = WaterCmd_OpenValve;			// " ���� "
			/*---------------------------------------------*/
			#ifdef Project_6009_RF
				Args.buf[i++] = 0x03;		// ������	03
				ackLen = 3;					// Ӧ�𳤶� 3	
				Args.buf[i++] = 0x00;		// ǿ�Ʊ�ʶ 	0 - ��ǿ�ƣ� 1 - ǿ��
				Args.buf[i++] = 0x01;		// ���ط���ʶ	0 - �ط��� 1 - ����
			#else // Project_8009_RF
				Args.buf[i++] = 0x05;		// ������	05
				ackLen = 0;					// Ӧ�𳤶� 0	
			#endif
			Args.lastItemLen = i - 1;
			break;

		case 3:
			CurrCmd = WaterCmd_CloseValve;		// " �ط� ";
			/*---------------------------------------------*/
			#ifdef Project_6009_RF
				Args.buf[i++] = 0x03;		// ������	03
				ackLen = 3;					// Ӧ�𳤶� 3	
				Args.buf[i++] = 0x00;		// ǿ�Ʊ�ʶ 	0 - ��ǿ�ƣ� 1 - ǿ��
				Args.buf[i++] = 0x00;		// ���ط���ʶ	0 - �ط��� 1 - ����
			#else // Project_8009_RF
				Args.buf[i++] = 0x06;		// ������	06
				ackLen = 0;					// Ӧ�𳤶� 0	
			#endif
			Args.lastItemLen = i - 1;
			break;

		case 4:
			CurrCmd = WaterCmd_ClearException;		// " ���쳣���� ";
			/*---------------------------------------------*/
			#ifdef Project_6009_RF
				Args.buf[i++] = 0x05;		// ������	05
				ackLen = 1;					// Ӧ�𳤶� 1	
				Args.buf[i++] = 0x00;		// ����ѡ�� 00	
			#else // Project_8009_RF
				Args.buf[i++] = 0x03;		// ������	03
				ackLen = 0;					// Ӧ�𳤶� 0	
			#endif
			Args.lastItemLen = i - 1;
			break;

		case 5:
			while(2){
				pUiCnt = 0;
				uiRowIdx = 3;
				TextBoxCreate(&pUi[(*pUiCnt)++], 0, (uiRowIdx++)*16, "�������:", StrBuf[0], 9, 11*8, true);
						pUi[(*pUiCnt) -1].ui.txtbox.dotEnable = 1;
				key = ShowUI(UiList, &currUi);

				if (key == KEY_CANCEL){
					break;
				}

				if(false == StringToDecimal(StrBuf[0], 2, &u8Tmp, &u32Tmp, &u16Tmp)){
					sprintf(StrBuf[0], " ");
					continue;
				}

				meterInfo->meterReadStatus[0] = '1';	// ����״̬�� �ɹ�
				meterInfo->meterReadType[0] = '1';		// ����ʽ�� �ֹ�¼��
				sprintf(MeterInfo.meterValue, "%d.%02d", u32Tmp, (u16Tmp & 0xFF));
				SaveMeterReadResult(meterInfo);
				ShowMsg(16, 3*16, "����� ¼��ɹ�!", 1000);
				break;
			}
			break;

		default: 
			break;
		}

		// ��ַ���
		#ifdef Project_6009_RF
			Water6009_PackAddrs(&Addrs, StrDstAddr, StrRelayAddr);
		#else
			Water8009_PackAddrs(&Addrs, StrDstAddr, StrRelayAddr);
		#endif

		// Ӧ�𳤶ȡ���ʱʱ�䡢�ط�����
		#ifdef Project_6009_RF
			ackLen += 15 + Addrs.itemCnt * AddrLen;
			timeout = 8000 + (Addrs.itemCnt - 2) * 6000 * 2;
			tryCnt = 2;
		#else // Project_8009_RF
			ackLen += 10 + Addrs.itemCnt * AddrLen;
			timeout = 2000 + (Addrs.itemCnt - 1) * 2000;
			tryCnt = 2;
		#endif

		// ���͡����ա������ʾ
		if(false == ProtolCommandTranceiver(CurrCmd, &Addrs, &Args, ackLen, timeout, tryCnt)){
			if(strncmp(DispBuf, "���", 4) != 0){	// ������ȡ��	
				DispBuf[0] = NULL;
			}else{
				meterInfo->meterReadStatus[0] = '2';	// ����״̬�� ʧ��
			}
		}
		else{
			meterInfo->meterReadStatus[0] = '1';	// ����״̬�� �ɹ�
		}
		meterInfo->meterReadType[0] = '0';		// ����ʽ�� �ƻ�����
		SaveMeterReadResult(meterInfo);
		//------------------------------------------------------
		_Printfxy(0, 9*16, "����            ȷ��", Color_White);
		
		key = ShowScrollStr(&DispBuf, 7);

	}// while 1  ������Ϣ
	

	return key;
}
