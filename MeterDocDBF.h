#ifndef MeterDocDBF_H
#define MeterDocDBF_H

#include "HJLIB.H"
#include "string.h"
#include "dbf.h"
#include "stdio.h"

#include "Common.h"

//----------------------	�궨��		------------------------
#define	MeterDocDB 	"jk.dbf"			// �������ݿ��ļ���

#define Invalid_Idx	(0xFFFFFFFF)		// ��Ч����

#define District_Max				20	// ���С����
#define Building_Max				50	// һ��С��-���¥����
#define Meter_Max					200	// һ��¥-������

#define	Size_ListStr	20				// �б��ַ�������

#define	Size_DistrictNum			12	// С����� ����	10
#define	Size_DistrictName			50	// С������ ����	50
#define	Size_BuildingNum			12	// ¥����� ����	10
#define	Size_BuildingName			50	// ¥������ ����	50

#define	Size_MeterNum				16	//��ų��� 		12
#define	Size_UserNum               	16	//���ų���		32
#define Size_RoomNum              	16	//���ƺų���	16
#define Size_MeterReadStatus        2	//����״̬����	1   : 0 - δ��  1 - �ѳ� 
#define	Size_UserName               40	//��������		50
#define Size_MobileNum           	12	//�ֻ��ų���	50
#define	Size_UserAddr               80	//��ַ���� 		100 
#define Size_MeterReadType			2	//����ʽ����	1	
#define Size_MeterReadTime          20	//����ʱ�䳤��	20
#define Size_MeterValue             12	//�����		12
#define Size_MeterStatusHex         8	//��״̬Hex����	8
#define Size_MeterStatusStr         40	//��״̬Str����	50
#define Size_BatteryVoltage         6   //��ص�ѹ����	5
#define Size_SignalValue           	10 	//�ź�ǿ�ȳ���	10

//----------------------	���ݿ���Ϣ	- ���ֶ�
// �ֶ�����
typedef enum{
	Idx_Id	= 0,		// "ID",		// ID
	Idx_UserNum,		// "HH",		// ����
	Idx_UserName,		// "HM",		// ����
	Idx_UserAddrs,		// "DZ",		// ��ַ
	Idx_UserRoomNum,	// "MPH",		// ���ƺ�
	Idx_UserTelNum,		// "TEL",		// �绰
	Idx_UserMobileNum,	// "MOBILE",	// �ֻ�
	Idx_AreaNum,			// "CBQY",		// ��������
	Idx_CollectorNum,		// "ZDBH",		// �ɼ��� ���
	Idx_CollectorName,		// "ZDMC",		// �ɼ��� ����
	Idx_CollectorSite,		// "ZDDZ",		// �ɼ��� λ��
	Idx_DistrictNum,		// "CBYJFZBH",		// ����С�� ���
	Idx_DistrictName,		// "CBYJFZMC",		// ����С�� ����
	Idx_BuildingNum,		// "CBEJFZBH",		// ����¥�� ���
	Idx_BuildingName,		// "CBEJFZMC",		// ����¥�� ����
	Idx_MeterNum,			// "BH",		// ���
	Idx_FLXS,				// "FLXS",		// 
	Idx_MeterValue,			// "FLA",		// ����� FL[A-J] 10�� ����
	Idx_FLB,				// "FLB",		// 
	Idx_FLC,				// "FLC",		// 
	Idx_FLD,				// "FLD",		// 
	Idx_FLE,				// "FLE",		// 
	Idx_FLF,				// "FLF",		// 
	Idx_FLG,				// "FLG",		// 
	Idx_FLH,				// "FLH",		// 
	Idx_FLI,				// "FLI",		// 
	Idx_FLJ,				// "FLJ",		// ����� FL[A-J] 10�� ����
	Idx_LYXS,				// "LYXS",		// 
	Idx_LYA,				// "LYA",		// LY[A-J] 10�� >>
	Idx_LYB,				// "LYB",		// 
	Idx_LYC,				// "LYC",		// 
	Idx_LYD,				// "LYD",		// 
	Idx_LYE,				// "LYE",		// 
	Idx_LYF,				// "LYF",		// 
	Idx_LYG,				// "LYG",		// 
	Idx_LYH,				// "LYH",		// 
	Idx_LYI,				// "LYI",		// 
	Idx_LYJ,				// "LYJ",		// LY[A-J] 10�� <<
	Idx_BLXHEX,				// "BLXHEX",	// ������ HEX
	Idx_BLXSTR,				// "BLXSTR",	// ������ STR
	Idx_MeterStatusHex,		// "BZTHEX",	// ��״̬ HEX
	Idx_MeterStatusStr,		// "BZTSTR",	// ��״̬ STR
	Idx_BatteryVoltage,		// "DCDY",		// ��ص�ѹ
	Idx_MCCS,				// "MCCS",		// ���峣��
	Idx_MeterReadType,		// "SGCQ",		// ����ʽ 0 - �ƻ����� �� 1 - ����������
	Idx_QF,					// "QF",		// 
	Idx_SignalValue,		// "XHQD",		// �ź�ǿ��
	Idx_MeterReadTime,		// "CBSJ",		// ����ʱ��
	Idx_MeterReadStatus,	// "CBZT",		// ����״̬
	Idx_BLZDA,				// "BLZDA",		// BLZD[A-J] 10�� ����
	Idx_BLZDB,				// "BLZDB",		// 
	Idx_BLZDC,				// "BLZDC",		// ������ ���
	Idx_BLZDD,				// "BLZDD",		// ������ ����
	Idx_BLZDE,				// "BLZDE",		// 
	Idx_BLZDF,				// "BLZDF",		// 
	Idx_BLZDG,				// "BLZDG",		// 
	Idx_BLZDH,				// "BLZDH",		// 
	Idx_BLZDI,				// "BLZDI",		// 
	Idx_BLZDJ,				// "BLZDJ"		// BLZD[A-J] 10�� ����
	Idx_Invalid	= 0xFF	// ��Ч�ֶ�	
}DB_Field_Index;

// �ֶ���
const char *Fields[] = {
	"ID",		// ID
	"HH",		// ����
	"HM",		// ����
	"DZ",		// ��ַ
	"MPH",		// ���ƺ�
	"TEL",		// �绰
	"MOBILE",	// �ֻ�
	"CBQY",		// ��������
	"ZDBH",		// �ɼ��� ���
	"ZDMC",		// �ɼ��� ����
	"ZDDZ",		// �ɼ��� λ��
	"CBYJFZBH",		// ����С�� ���
	"CBYJFZMC",		// ����С�� ����
	"CBEJFZBH",		// ����¥�� ���
	"CBEJFZMC",		// ����¥�� ����
	"BH",		// ���
	"FLXS",		// 
	"FLA",		// ����� FL[A-J] 10�� ����
	"FLB",		// 
	"FLC",		// 
	"FLD",		// 
	"FLE",		// 
	"FLF",		// 
	"FLG",		// 
	"FLH",		// 
	"FLI",		// 
	"FLJ",		// ����� FL[A-J] 10�� ����
	"LYXS",		// 
	"LYA",		// LY[A-J] 10�� >>
	"LYB",		// 
	"LYC",		// 
	"LYD",		// 
	"LYE",		// 
	"LYF",		// 
	"LYG",		// 
	"LYH",		// 
	"LYI",		// 
	"LYJ",		// LY[A-J] 10�� <<
	"BLXHEX",	// 
	"BLXSTR",	// 
	"BZTHEX",	// ��״̬ HEX
	"BZTSTR",	// ��״̬ STR
	"DCDY",		// ��ص�ѹ
	"MCCS",		// 
	"SGCQ",		// ����ʽ ��
	"QF",		// 
	"XHQD",		// �ź�ǿ��
	"CBSJ",		// ����ʱ��
	"CBZT",		// ����״̬
	"BLZDA",		// BLZD[A-J] 10�� ����
	"BLZDB",		// 
	"BLZDC",		// ������ ���
	"BLZDD",		// ������ ����
	"BLZDE",		// 
	"BLZDF",		// 
	"BLZDG",		// 
	"BLZDH",		// 
	"BLZDI",		// 
	"BLZDJ"			// BLZD[A-J] 10�� ����
};

//---------------	dbf ��ѯ�ṹ
typedef struct {
	// uint8 queryType;	// ��ѯ���ͣ�0-С���б� 1-¥���б� 2-��������б� , 3 - ������Ϣ
	// char *districNum;	// С�����
	// char *buildingNum;	// ¥�����
	// char *meterReadStatus;	// ����״̬
	// char *meterNum;		// ���
	// char *userNum;		// ����
	// char *roomNum;		// ���ƺ�
	// uint32 dbSelectIdx;	// ѡ������ݿ��¼����

	uint32	dbCurrIdx;	// ���ݿ⵱ǰλ��
	uint32	reqMaxCnt;	// ���������
	uint32	resultCnt;	// ��ѯ�Ľ����¼��
	uint8	errorCode;	// 0 - ok,  ���� - ����
}DbQuerySt;

//---------------	С���б�
typedef struct{
	char 	nums[District_Max][Size_ListStr];	// �б����ַ�����¥�����
	char 	names[District_Max][Size_ListStr];	// �б����ַ�����¥������
	uint8 	idx;			// �б�������
	uint8	cnt;			// �б�������
}DistrictListSt;

//--------------	xxС�� - ¥���б�
typedef struct{
	char 	nums[Building_Max][Size_ListStr];		// �б����ַ�����¥�����
	char 	names[Building_Max][Size_ListStr];		// �б����ַ�����¥������
	uint8 	idx;			// �б�������
	uint8	cnt;			// �б�������
	char 	*qryDistricNum;	// С�����
}BuildingListSt;

//---------------	xxС�� - xx¥�� - �����б����/����/���ƺ�/������ 
typedef struct{
	uint32 	dbIdx[Meter_Max];				// �б����Ӧ�����ݿ�����
	char 	strs[Meter_Max][Size_ListStr];	// �б����ַ��������/����/���ƺ�/����/��ַ
	uint8 	idx;			// �б�������
	uint8	cnt;			// �б�������
	uint8 	selectField;	// Ҫ��ʾ���ֶΣ����/����/���ƺ�/����/��ַ
	char 	*qryDistricNum;			// С����ţ���ֵ��ʾ����
	char 	*qryBuildingNum;		// ¥����ţ���ֵ��ʾ����
	char 	*qryMeterReadStatus;	// ����״̬
	char 	districName[Size_DistrictName];		// С����
	char 	buildingName[Size_BuildingName];	// ¥����
	uint16 	meterCnt;		// ��ǰ������
	uint16 	readOkCnt;		// �ѳ�����
	uint16 	readNgCnt;		// δ������
}MeterListSt;


//---------------	������Ϣ
typedef struct 
{
	uint32	dbIdx;		// ��¼�����ݿ�������
	char	*qryMeterNum;		// ���
	char 	*qryUserNum;		// ����
	char 	*qryRoomNum;		// ���ƺ�
	char	meterNum[Size_MeterNum];
	char	userNum[Size_UserNum];
	char	roomNum[Size_RoomNum];
	char	meterReadStatus[Size_MeterReadStatus];
	char	userName[Size_UserName];
	char	mobileNum[Size_MobileNum];
	char	userAddr[Size_UserAddr];
	char	meterReadType[Size_MeterReadType];
	char	meterReadTime[Size_MeterReadTime];
	char	meterValue[Size_MeterValue];
	char	meterStatusHex[Size_MeterStatusHex];
	char	meterStatusStr[Size_MeterStatusStr];
	char	batteryVoltage[Size_BatteryVoltage];
	char	signalValue[Size_SignalValue];

}MeterInfoSt;

//------------------------		�ⲿ�ӿ�����	-------------------------------------
extern MeterInfoSt MeterInfo;
extern DistrictListSt Districts;
extern BuildingListSt Buildings;
extern MeterListSt Meters;
extern DbQuerySt DbQuery;

extern void QueryDistrictList(DistrictListSt *districts, DbQuerySt *query);
extern void QueryBuildingList(BuildingListSt *buildings, DbQuerySt *query);
extern void QueryMeterList(MeterListSt *meters, DbQuerySt *query);
extern uint8 ShowMeterReadCountInfo(MeterListSt *meters);
extern void SaveMeterReadResult(MeterInfoSt *meterInfo);
extern void QueryMeterInfo(MeterInfoSt *meterInfo, DbQuerySt *query);
extern uint8 ShowMeterInfo(MeterInfoSt *meterInfo);

//-----------------------	ȫ�ֱ�������
MeterInfoSt MeterInfo;
DistrictListSt Districts;
BuildingListSt Buildings;
MeterListSt Meters;
DbQuerySt DbQuery;

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
	char strTmp1[50];
	char strTmp2[50];

	_Select(1);
	_Use(MeterDocDB);	// �����ݿ�
	recCnt = _Reccount();
	_Go(0);
	districts->cnt = 0;
	districts->idx = 0;
	query->reqMaxCnt = (query->reqMaxCnt == 0 ? District_Max : query->reqMaxCnt);
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
	char strTmp[50];
	char strTmp1[50];
	char strTmp2[50];

	_Select(1);
	_Use(MeterDocDB);	// �����ݿ�
	recCnt = _Reccount();
	_Go(0);
	buildings->cnt = 0;
	buildings->idx = 0;
	query->reqMaxCnt = (query->reqMaxCnt == 0 ? Building_Max : query->reqMaxCnt);
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
	char strTmp[50];

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
	//query->reqMaxCnt = (query->reqMaxCnt == 0 ? Meter_Max : query->reqMaxCnt);
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

		meters->meterCnt++;		// ��ǰ������
		
		if(meters->qryMeterReadStatus != NULL){
			_ReadField(Idx_MeterReadStatus, strTmp);		// ����״̬ ����  NULL - ���У� ��0�� - δ���� ��1�� - �ѳ�
			strTmp[Size_MeterReadStatus - 1] = '\0';
			if(strcmp(strTmp, "1") != 0){
				meters->readNgCnt++;		// δ������
			}else{
				meters->readOkCnt++;		// �ѳ�����
			}
			if(strcmp(meters->qryMeterReadStatus, strTmp) != 0){
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
		if(meters->selectField == Idx_Invalid){	// δѡ���ֶΣ��򲻹����б�
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
	uint8 key, i, cnt = 0, isCancel = false;
	uint8 tryCnt;
	uint16 ackLen = 0, timeout;
	char strTmp[50];

	// �м����
	for(i = 0; i < RELAY_MAX; i++){				
		if(StrRelayAddr[i][0] > '9' || StrRelayAddr[i][0] < '0'){
			StrRelayAddr[i][0] = 0x00;
		}
	}
	while(cnt < meters->cnt){

		_ClearScreen();
		_Printfxy(0, 0, "<<�Զ�����", Color_White);
		/*---------------------------------------------*/
		// ���ñ��
		_Select(1);
		_Use(MeterDocDB);	// �����ݿ�
		_Go(meters->dbIdx[cnt]);

		_Use("");			// �ر����ݿ�
		strcpy(StrDstAddr, strTmp);

		// �����������
		i = 0;	
		Args.itemCnt = 2;
		Args.items[0] = &Args.buf[0];   // ������
		Args.items[1] = &Args.buf[1];	// ������
		Args.buf[i++] = 0x01;		// ������	01
		ackLen = 21;				// Ӧ�𳤶� 21	
		Args.buf[i++] = 0x00;		// ���ݸ�ʽ 00	
		Args.lastItemLen = i - 1;

		// ��ַ���
		Water6009_PackAddrs(&Addrs, StrDstAddr, StrRelayAddr);

		// Ӧ�𳤶ȡ���ʱʱ�䡢�ط�����
		ackLen += 14 + Addrs.itemCnt * AddrLen;
		timeout = 8000 + (Addrs.itemCnt - 2) * 6000 * 2;
		tryCnt = 3;

		// ���͡����ա������ʾ
		if(false == Protol6009Tranceiver(CurrCmd, &Addrs, &Args, ackLen, timeout, tryCnt)){
			if(strncmp(DispBuf, "���", 4) != 0){	// ������ȡ��	
				DispBuf[0] = NULL;
				isCancel = true;
				break;
			}
		}

		//----------------------------------------------
		_Printfxy(0, 9*16, "����  < �ֳ��� >     ", Color_White);
	
	}

	if(isCancel){
		
		_Printfxy(0, 9*16, "����  < ��ȡ�� >    ", Color_White);
	}
	else{
		_Printfxy(0, 9*16, "����  < ����� >    ", Color_White);
	}
	
	while(1){
		
	}
	
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

	_Printfxy(0, 0, "<<������Ϣ", Color_White);
	/*---------------------------------------------*/
	dispIdx += sprintf(&dispBuf[dispIdx], "С��: %s\n", meters->districName);
	dispIdx += sprintf(&dispBuf[dispIdx], "¥��: %s\n", meters->buildingName);
	dispIdx += sprintf(&dispBuf[dispIdx], "����: %d\n", meters->meterCnt);
	dispIdx += sprintf(&dispBuf[dispIdx], "�ѳ�: %d\n", meters->readOkCnt);
	dispIdx += sprintf(&dispBuf[dispIdx], "δ��: %d\n", meters->readNgCnt);
	//----------------------------------------------
	_Printfxy(0, 9*16, "����            ȷ��", Color_White);

	key = ShowScrollStr(&DispBuf,  7);
	
	return key;
}

/*
* �� ������ʾxxС��-xx¥��-�ѳ�/δ���б�
* �� ����meters		- ��������б�: ����ǰ�����ó���״̬ qryMeterReadStatus
* �� �أ�uint8 	- �����˳�ʱ�İ���ֵ�� KEY_CANCEL - ���ؼ� �� KEY_ENTER - ȷ�ϼ�
*/
uint8 ShowMeterList(MeterListSt *meters)
{
	uint8 key;
	ListBox showTpList, meterList;		// ��ʾ��ʽ/����Ϣ-�б�
	char *title = NULL;

	// �б���ʾ��ʽ-����
	title = (meters->qryMeterReadStatus[0] == '1' ? "<<�ѳ��б�" : "<<δ���б�");
	ListBoxCreate(&showTpList, 0, 0, 20, 7, 4, NULL,
		title, 
		4,
		"1. �������ʾ",
		"2. ��������ʾ",
		"3. ��������ʾ",
		"4. �����ƺ���ʾ");
	//------------------------------------------------------------
	_Printfxy(0, 9*16, "����            ȷ��", Color_White);
	while(1){

		_ClearScreen();
		
		key = ShowListBox(&showTpList);
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
		//------------------------------------------------------------
		_Printfxy(0, 9*16, "����        ������Ϣ", Color_White);
		while(2){
			#if Log_On
			LogPrint("meters->idx = %d,  meters->cnt = %d,  meters->meterCnt = %d,  meters->selectField = %d \n \
				meterList.strIdx = %d, meterList.strCnt = %d, meterList.currIdx = %d, meterList.totalCnt = %d \n", \
				meters->idx, meters->cnt, meters->meterCnt, meters->selectField, \
				meterList.strIdx, meterList.strCnt, meterList.currIdx, meterList.totalCnt);
			#endif
			
			_ClearScreen();
		
			key = ShowListBox(&meterList);
			if(key == KEY_CANCEL){	// ����
				break;
			}
			if(key == KEY_ENTER && meters->cnt == 0){	
				_Printfxy(0, 5*16, "��ǰ�б�����Ŀ��", Color_White);
				continue;
			}
			
			meters->idx = (uint8)(meterList.strIdx & 0xFF);
			
			#if Log_On
			LogPrint("meters->idx = %d,  meters->cnt = %d, meters->meterCnt = %d,  meters->selectField = %d \n " , \
				meters->idx, meters->cnt, meters->meterCnt, meters->selectField);
			#endif
			
			// ������Ϣ-����
			//------------------------------------------------------
			MeterInfo.dbIdx = meters->dbIdx[meters->idx];
			QueryMeterInfo(&MeterInfo, &DbQuery);	// ������Ϣ��ѯ
			
			#if Log_On
			LogPrint("meters->idx = %d,  meters->cnt = %d, meters->meterCnt = %d,  meters->selectField = %d \n " , \
				meters->idx, meters->cnt, meters->meterCnt, meters->selectField);
			#endif
			//------------------------------------------------------
			key = ShowMeterInfo(&MeterInfo);	// ���ص��б�

			#if Log_On
			LogPrint("meters->idx = %d,  meters->cnt = %d, meters->meterCnt = %d,  meters->selectField = %d \n " , \
				meters->idx, meters->cnt, meters->meterCnt, meters->selectField);
			#endif
			
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

	// ���³�����
	_GetDateTime(time, '-', ':');
	_Replace(Idx_MeterReadStatus, "1");	// ����״̬ 0 - δ���� 1 - �ѳ�
	_Replace(Idx_MeterReadType, "0");	// ����ʽ 0 - �ƻ����� �� 1 - ����������
	_Replace(Idx_MeterReadTime, time);	// ����ʱ��
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
	char strTmp[100];

	_Select(1);
	_Use(MeterDocDB);	// �����ݿ�
	recCnt = _Reccount();
	_Go(0);
	query->resultCnt = 0;
	query->errorCode = 0;

	if(meterInfo->dbIdx != Invalid_Idx){	// ���ݿ��¼���� �Ƿ���Ч��
		if(meterInfo->dbIdx > recCnt -1){
			meterInfo->dbIdx = Invalid_Idx;
		}
	}

	if(meterInfo->dbIdx == Invalid_Idx){	// ���ݿ��¼������Чʱ ִ�в�ѯ

		#if 0
		for(i = 0; i < recCnt; i++){
			if(meterInfo->qryMeterNum != NULL){
				_ReadField(Idx_MeterNum, strTmp);		// ����Ų�ѯ
				strTmp[Size_MeterNum - 1] = '\0';
				if(strcmp(meterInfo->qryMeterNum, strTmp) == 0){
					meterInfo->dbIdx = i;
					break;
				}
			}
			else if(meterInfo->qryUserNum != NULL){
				_ReadField(Idx_UserNum, strTmp);		// �����Ų�ѯ
				strTmp[Size_UserNum - 1] = '\0';
				if(strcmp(meterInfo->qryUserNum, strTmp) == 0){
					meterInfo->dbIdx = i;
					break;
				}
			}
			else if(meterInfo->qryRoomNum != NULL){
				_ReadField(Idx_UserRoomNum, strTmp);	// �����ƺŲ�ѯ
				strTmp[Size_RoomNum - 1] = '\0';
				if(strcmp(meterInfo->qryRoomNum, strTmp) == 0){
					meterInfo->dbIdx = i;
					break;
				}
			}
		}
		#endif

		if(meterInfo->qryMeterNum != NULL
			&& _LocateEx(Idx_MeterNum, '=', meterInfo->qryMeterNum, 1, recCnt, 0) > 0){ 
			// ����Ų�ѯ
			meterInfo->dbIdx = _Recno();
		}
		else if(meterInfo->qryMeterNum != NULL
			&& _LocateEx(Idx_UserNum, '=', meterInfo->qryMeterNum, 1, recCnt, 0) > 0){ 
			// ����Ų�ѯ
			meterInfo->dbIdx = _Recno();
		}
		else if(meterInfo->qryRoomNum != NULL
			&& _LocateEx(Idx_UserRoomNum, '=', meterInfo->qryRoomNum, 1, recCnt, 0) > 0){ 
			// ����Ų�ѯ
			meterInfo->dbIdx = _Recno();
		}

		if(i >= recCnt){
			meterInfo->dbIdx = Invalid_Idx;
		}
	}

	if(meterInfo->dbIdx != Invalid_Idx){	// ���ݿ��¼������Чʱ ��ȡ��¼

		_Go(meterInfo->dbIdx);
		query->resultCnt = 1;

		_ReadField(Idx_MeterNum, strTmp);					// ���
		strTmp[Size_MeterNum - 1] = '\0';
		strncpy(meterInfo->meterNum, strTmp, Size_MeterNum);	

		_ReadField(Idx_UserNum, strTmp);					// ����
		strTmp[Size_UserNum - 1] = '\0';
		strncpy(meterInfo->userNum, strTmp, Size_UserNum);	

		_ReadField(Idx_UserRoomNum, strTmp);				// ���ƺ�
		strTmp[Size_RoomNum - 1] = '\0';
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
		strTmp[Size_UserAddr - 1] = '\0';
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

	query->dbCurrIdx = i;
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
	uint8 * pByte, tryCnt;
	uint16 ackLen = 0, timeout;
	char *dispBuf;

	// ������Ϣ-����
	while(1){

		_ClearScreen();

		_Printfxy(0, 0, "<<������Ϣ", Color_White);
		/*---------------------------------------------*/
		dispBuf = &DispBuf;
		dispIdx = 0;
		dispIdx += sprintf(&dispBuf[dispIdx], "���: %s\n", meterInfo->meterNum);
		dispIdx += sprintf(&dispBuf[dispIdx], "����: %s\n", meterInfo->userNum);
		dispIdx += sprintf(&dispBuf[dispIdx], "���ƺ�: %s\n", meterInfo->roomNum);
		dispIdx += sprintf(&dispBuf[dispIdx], "����״̬: %s\n", meterInfo->meterReadStatus);
		dispIdx += sprintf(&dispBuf[dispIdx], "����: %s\n", meterInfo->userName);
		dispIdx += sprintf(&dispBuf[dispIdx], "�ֻ�: %s\n", meterInfo->mobileNum);
		dispIdx += sprintf(&dispBuf[dispIdx], "��ַ: %s\n", meterInfo->userAddr);
		dispIdx += sprintf(&dispBuf[dispIdx], "����ʽ: %s\n", meterInfo->meterReadType);
		dispIdx += sprintf(&dispBuf[dispIdx], "����ʱ��: %s\n", meterInfo->meterReadTime);
		dispIdx += sprintf(&dispBuf[dispIdx], "�����: %s\n", meterInfo->meterValue);
		dispIdx += sprintf(&dispBuf[dispIdx], "��״̬: %s\n", meterInfo->meterStatusStr);
		dispIdx += sprintf(&dispBuf[dispIdx], "��ص�ѹ: %s\n", meterInfo->batteryVoltage);
		dispIdx += sprintf(&dispBuf[dispIdx], "�ź�ǿ��: %s\n", meterInfo->signalValue);
		//----------------------------------------------
		_Printfxy(0, 9*16, "����        ��������", Color_White);

		key = ShowScrollStr(dispBuf,  7);
		if(key == KEY_CANCEL){	// ����
			break;
		}

		
		strcpy(StrDstAddr, meterInfo->meterNum);	// �������
		for(i = 0; i < RELAY_MAX; i++){				// �м����
			if(StrRelayAddr[i][0] > '9' || StrRelayAddr[i][0] < '0'){
				StrRelayAddr[i][0] = 0x00;
			}
		}

		// ��������-����
		ListBoxCreate(&menuList, 3*16, 3*16, 14, 4, 4, NULL, 
			"��������", 
			4, 
			"1. ����",
			"2. ����",
			"3. �ط�",
			"4. ���쳣");
		//---------------------
		key = ShowListBox(&menuList);
		if(key == KEY_CANCEL){	// ȡ��ִ�����������ʾ������Ϣ
			continue;
		}

		// ��ʾ����
		pByte = menuList.str[menuList.strIdx];
		sprintf(TmpBuf, "<<%s",&pByte[3]);
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
			Args.buf[i++] = 0x01;		// ������	01
			ackLen = 21;				// Ӧ�𳤶� 21	
			// ������
			Args.buf[i++] = 0x00;				// ���ݸ�ʽ 00	
			Args.lastItemLen = i - 1;
			break;

		case 2:
			CurrCmd = WaterCmd_OpenValve;			// " ���� "
			/*---------------------------------------------*/
			Args.buf[i++] = 0x03;		// ������	03
			ackLen = 3;					// Ӧ�𳤶� 3	
			// ������
			Args.buf[i++] = 0x00;		// ǿ�Ʊ�ʶ 	0 - ��ǿ�ƣ� 1 - ǿ��
			Args.buf[i++] = 0x01;		// ���ط���ʶ	0 - �ط��� 1 - ����
			Args.lastItemLen = i - 1;
			break;

		case 3:
			CurrCmd = WaterCmd_CloseValve;		// " �ط� ";
			/*---------------------------------------------*/
			Args.buf[i++] = 0x03;		// ������	03
			ackLen = 3;					// Ӧ�𳤶� 3	
			// ������
			Args.buf[i++] = 0x00;		// ǿ�Ʊ�ʶ 	0 - ��ǿ�ƣ� 1 - ǿ��
			Args.buf[i++] = 0x00;		// ���ط���ʶ	0 - �ط��� 1 - ����
			Args.lastItemLen = i - 1;
			break;

		case 4:
			CurrCmd = WaterCmd_ClearException;		// " ���쳣���� ";
			/*---------------------------------------------*/
			Args.buf[i++] = 0x05;		// ������	05
			ackLen = 1;					// Ӧ�𳤶� 1	
			// ������
			Args.buf[i++] = 0x00;		// ����ѡ�� 00	
			Args.lastItemLen = i - 1;
			break;

		default: 
			break;
		}

		// ��ַ���
		Water6009_PackAddrs(&Addrs, StrDstAddr, StrRelayAddr);

		// Ӧ�𳤶ȡ���ʱʱ�䡢�ط�����
		ackLen += 14 + Addrs.itemCnt * AddrLen;
		timeout = 8000 + (Addrs.itemCnt - 2) * 6000 * 2;
		tryCnt = 3;

		// ���͡����ա������ʾ
		if(false == Protol6009Tranceiver(CurrCmd, &Addrs, &Args, ackLen, timeout, tryCnt)){
			if(strncmp(DispBuf, "���", 4) != 0){	// ������ȡ��	
				DispBuf[0] = NULL;
			}
		}
		else{
			// �����������ݿ�
			SaveMeterReadResult(meterInfo);
		}
		_Printfxy(0, 9*16, "����            ȷ��", Color_White);
		
		while(3){
			key = ShowScrollStr(&DispBuf, 7);
		
			// ���� / ȷ�� 
			if (key == KEY_CANCEL || key == KEY_ENTER){
				break;
			}else{
				continue;
			}
		}

		if(key == KEY_CANCEL){	// ����
			break;
		}
	}// while 1  ������Ϣ
	

	return key;
}

#endif