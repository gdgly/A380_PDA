#ifndef MeterDocDBF_8009_PY_H
#define MeterDocDBF_8009_PY_H

#include "common.h"

//----------------------	�궨��		------------------------
#define	MeterDocDB			"MRDATA.DBF"	// ���������ݿ��ļ���

#define Invalid_dbIdx		0				// ��Ч����, ���ݿ��¼��Ŵ�1��ʼ

#define Sect_Max			50				// ��󳭱�������
#define Meter_Max			ListItemEx_Max	// ����������-������
#define	Size_ListStr		21				// �б��ַ�������
#define	Size_DbMaxStr		255				// ���ݿ��ֶ���󳤶�


//----------------------	���ݿ���Ϣ
// MRDATA.DBF ���ֶ�����
typedef enum _db_field_index{
	Idx_MtrReadId = 0,	// �������ݱ�ʶ	"MR_ID"		char(16) 
	Idx_AppNum,			// app������	"APP_NO"	char(16) 
	Idx_SectNum,		// ��������	"SECT_NO"	char(16) 
	Idx_SectName,		// ����������	"SECT_NAME"	char(254) 
	Idx_MtrReadSn,		// ����˳���	"MR_SN"		number(5) 
	Idx_ReaderNum,		// ����Ա����	"READER_NO"	char(16)
	Idx_ReaderName,		// ����Ա����	"READER"	char(16)
	Idx_OrgNum,			// ������λ		"ORG_NO"	char(16)
	Idx_UserNum,		// �û����		"CONS_NO"	char(16)
	Idx_UserName,		// �û�����		"CONS_NAME"	char(254)
	Idx_UserAddr,		// �û���ַ		"CONS_ADDR"	char(254)
	Idx_PlanMrDate,		// �ƻ���������	"PL_MR_DATE"	char(8)
	Idx_MeterId,		// ȼ������ʶ	"METER_ID"		char(16)
	Idx_MeterType,		// ȼ��������	"APPLY_TYPE"	char(8)
	Idx_MeterNum,		// �������		"FAC_NUMBER"	char(32)
	Idx_PriceCode,		// ������		"PRC_CODE"		char(16)
	Idx_TempFactor,		// �¶Ȳ���ϵ��	"TP_FACTOR"		number(15��6)
	Idx_LastMrDate,		// �ϴγ�������	"LAST_DATE"		char(8)
	Idx_LastReadVal,	// �ϴζ���		"LAST_READ"		number(15,6)
	Idx_LastGasVol,		// �ϴ�����		"LAST_MR_GQ"	number(15,6)
	Idx_AvgGasVol,		// ƽ������		"AVG_MR_GQ"		number(15,6)
	Idx_MtrReadStatus,		// ����״̬		"MR_STATUS"		char(8)			// set
	Idx_MtrReadType,		// ��������		"MR_TYPE"		char(8)			// set
	Idx_MtrReadDate,		// ʵ�ʳ�������	"THIS_YMD"		char(8)			// set
	Idx_CurrReadVal,		// ���ζ���		"THIS_READ"		number(15,6)	// set
	Idx_CurrGasVol,			// ��������		"THIS_GQ"		number(15,6)	// set
	Idx_MrExcepType,		// �����쳣����	"EXCP_TYPE"		char(8)			// set
	Idx_LastElecReadVal,	// �ϴε��Ӷ���	"LEM_READ"		number(15,6)
	Idx_CurrElecReadVal,	// ���ε��Ӷ���	"EM_READ"		number(15,6)	// set	
	Idx_LastRemainGasVol,	// �ϴ�ʣ������	"LR_GAS"		number(15,6)
	Idx_CurrRemainGasVol,	// ����ʣ������	"REMAIN_GAS"	number(15,6)	// set
	Idx_MeterDigit,		// ��λ��		"MR_DIGIT"	number(12,2)
	Idx_UserBalance,	// �û����		"BLA"		number(15,6)
	Idx_RemarkA,		// ��չ�ֶ�A	"REMARK_A"		char(32)
	Idx_RemarkA_V,		// ��չ�ֶ�A_V	"REMARK_A_V"	char(128)
	Idx_RemarkB,		// ��չ�ֶ�B	"REMARK_B"		char(32)
	Idx_RemarkB_V,		// ��չ�ֶ�B_V	"REMARK_B_V"	char(128)
	Idx_RemarkC,		// ��չ�ֶ�C	"REMARK_C"		char(32)
	Idx_RemarkC_V,		// ��չ�ֶ�C_V	"REMARK_C_V"	char(128)
	Idx_RemarkD,		// ��չ�ֶ�D	"REMARK_D"		char(32)
	Idx_RemarkD_V,		// ��չ�ֶ�D_V	"REMARK_D_V"	char(128)
	Idx_RemarkE,		// ��չ�ֶ�E	"REMARK_E"		char(32)
	Idx_RemarkE_V,		// ��չ�ֶ�E_V	"REMARK_E_V"	char(128)
	Idx_Invalid	= 0xFF	// ��Ч�ֶ�	
}DB_Field_Index;

// MRDATA.DBF ���ֶδ�С
#define Size_MtrReadId		17		// �������ݱ�ʶ	"MR_ID"		char(16) 
#define Size_AppNum			17		// app������	"APP_NO"	char(16) 
#define Size_SectNum		17		// ��������	"SECT_NO"	char(16) 
#define Size_SectName		255		// ����������	"SECT_NAME"	char(254) 
#define Size_MtrReadSn		6		// ����˳���	"MR_SN"		number(5) 
#define Size_ReaderNum		17		// ����Ա����	"READER_NO"	char(16)
#define Size_ReaderName		17		// ����Ա����	"READER"	char(16)
#define Size_OrgNum			17		// ������λ		"ORG_NO"	char(16)
#define Size_UserNum		17		// �û����		"CONS_NO"	char(16)
#define Size_UserName		255		// �û�����		"CONS_NAME"	char(254)
#define Size_UserAddr		255		// �û���ַ		"CONS_ADDR"	char(254)
#define Size_PlanMrDate		9		// �ƻ���������	"PL_MR_DATE"	char(8)
#define Size_MeterId		17		// ȼ������ʶ	"METER_ID"		char(16)
#define Size_MeterType		9		// ȼ��������	"APPLY_TYPE"	char(8)
#define Size_MeterNum		33		// �������		"FAC_NUMBER"	char(32)
#define Size_PriceCode		17		// ������		"PRC_CODE"		char(16)
#define Size_TempFactor		23		// �¶Ȳ���ϵ��	"TP_FACTOR"		number(15,6)
#define Size_LastMrDate		9		// �ϴγ�������	"LAST_DATE"		char(8)
#define Size_LastReadVal	23		// �ϴζ���		"LAST_READ"		number(15,6)
#define Size_LastGasVol		23		// �ϴ�����		"LAST_MR_GQ"	number(15,6)
#define Size_AvgGasVol		23		// ƽ������		"AVG_MR_GQ"		number(15,6)
#define Size_MtrReadStatus		9	// ����״̬		"MR_STATUS"		char(8)			// set
#define Size_MtrReadType		9	// ��������		"MR_TYPE"		char(8)			// set
#define Size_MtrReadDate		9	// ʵ�ʳ�������	"THIS_YMD"		char(8)			// set
#define Size_CurrReadVal		23	// ���ζ���		"THIS_READ"		number(15,6)	// set
#define Size_CurrGasVol			23	// ��������		"THIS_GQ"		number(15,6)	// set
#define Size_MrExcepType		9	// �����쳣����	"EXCP_TYPE"		char(8)			// set
#define Size_LastElecReadVal	23	// �ϴε��Ӷ���	"LEM_READ"		number(15,6)
#define Size_CurrElecReadVal	23	// ���ε��Ӷ���	"EM_READ"		number(15,6)	// set	
#define Size_LastRemainGasVol	23	// �ϴ�ʣ������	"LR_GAS"		number(15,6)
#define Size_CurrRemainGasVol	23	// ����ʣ������	"REMAIN_GAS"	number(15,6)	// set
#define Size_MeterDigit		16		// ��λ��		"MR_DIGIT"	number(12,2)
#define Size_UserBalance	23		// �û����		"BLA"		number(15,6)
#define Size_RemarkA		33		// ��չ�ֶ�A	"REMARK_A"		char(32)
#define Size_RemarkA_V		129		// ��չ�ֶ�A_V	"REMARK_A_V"	char(128)
#define Size_RemarkB		33		// ��չ�ֶ�B	"REMARK_B"		char(32)
#define Size_RemarkB_V		129		// ��չ�ֶ�B_V	"REMARK_B_V"	char(128)
#define Size_RemarkC		33		// ��չ�ֶ�C	"REMARK_C"		char(32)
#define Size_RemarkC_V		129		// ��չ�ֶ�C_V	"REMARK_C_V"	char(128)
#define Size_RemarkD		33		// ��չ�ֶ�D	"REMARK_D"		char(32)
#define Size_RemarkD_V		129		// ��չ�ֶ�D_V	"REMARK_D_V"	char(128)
#define Size_RemarkE		33		// ��չ�ֶ�E	"REMARK_E"		char(32)
#define Size_RemarkE_V		129		// ��չ�ֶ�E_V	"REMARK_E_V"	char(128)


//---------------	dbf ��ѯ�ṹ
typedef struct {
	uint32	dbCurrIdx;	// ���ݿ⵱ǰλ��
	uint32	reqMaxCnt;	// ���������
	uint32	resultCnt;	// ��ѯ�Ľ����¼��
	uint16	errorCode;	// 0 - ok,  ���� - ����
}DbQuerySt;

//---------------	�������б�
typedef struct{
	char 	nums[Sect_Max][Size_ListStr];	// �б����ַ�������������
	//char 	names[Sect_Max][Size_ListStr];	// �б����ַ���������������
	uint8 	idx;			// �б�������
	uint8	cnt;			// �б�������
}SectListSt;

//---------------	xx������ - �����б�������/����/����/��ַ�� 
typedef struct{
	uint32 	dbIdx[Meter_Max];				// �б����Ӧ�����ݿ�����
	char 	strs[Meter_Max][Size_ListStr];	// �б����ַ���������/����/����/��ַ
	uint16 	idx;			// �б�������
	uint16	cnt;			// �б�������
	uint16 	selectField;	// Ҫ��ʾ���ֶΣ�����/����/��ַ/����/��ַ
	char 	*qrySectNum;			// �������ţ���ֵ��ʾ����
	char 	*qryMeterReadStatus;	// ����״̬
	char 	sectNum[Size_SectNum];			// ��������
	//char 	sectName[Size_SectName];		// ����������
	uint16 	meterCnt;		// ��ǰ��������δ��+�ɹ�+ʧ��
	uint16 	readOkCnt;		// �ɹ�����
	uint16 	readNgCnt;		// ʧ������
}MeterListSt;


//---------------	������Ϣ
typedef struct 
{
	uint32	dbIdx;		// ��ǰ��¼�����ݿ�������
	uint8 	strIdx;		// ��ǰ��¼���б��������
	uint8	strCnt;		// ��ǰ��¼�����б�������	
	char	*qryMeterNum;		// ����
	char	*qryUserNum;		// ����
	char 	*qryUserName;		// ����
	char 	*qryUserAddr;		// ��ַ
	
	char	userNum[Size_UserNum];		// �û����
	char	userName[Size_UserName];	// �û�����
	char	userAddr[Size_UserAddr];	// �û���ַ
	char	meterNum[Size_MeterNum];	// ����
	char	meterReadStatus[Size_MtrReadStatus];	// ����״̬
	char	meterReadTime[Size_MtrReadDate];		// ��������
	char	meterReadType[Size_MtrReadType];		// ��������
	char	meterExcpType[Size_MrExcepType];		// �����쳣����
	char	currGasVol[Size_CurrGasVol];				// ��������
	char	currRemainGasVol[Size_CurrRemainGasVol];	// ����ʣ������
	char	currReadVal[Size_CurrReadVal];				// ���ζ���
	char	currElecReadVal[Size_CurrElecReadVal];		// ���ε��Ӷ���

	char	lastGasVol[Size_LastGasVol];				// �ϴ�����
	char	lastRemainGasVol[Size_LastRemainGasVol];	// �ϴ�ʣ������
	char	lastReadVal[Size_LastReadVal];				// �ϴζ���
	char	lastElecReadVal[Size_LastElecReadVal];		// �ϴε��Ӷ���

	char	lastMtrReadTime[Size_LastMrDate];			// �ϴγ�������
	char	userBalance[Size_UserBalance];				// �û����
	char	avgGasVol[Size_AvgGasVol];					// ƽ������

	char	sectNum[Size_SectNum];			// ��������
	char	sectName[Size_SectName];		// ����������
	char	readerNum[Size_ReaderNum];		// ����Ա���
	char	readerName[Size_ReaderName];	// ����Ա����
	char	orgNum[Size_OrgNum];			// ������λ���

}MeterInfoSt;

// �����쳣����
typedef enum{
	MrExcp_Normal		= 1,	// ����
	MrExcp_Flip,				// ��ת
	MrExcp_Reverse,				// ��ת
	MrExcp_FlipReverse,			// ��ת�ҵ�ת
	MrExcp_DataExcp		= 9		// �����쳣
}MtrReadException;

// ��������
typedef enum{
	MrType_BySee		= 1,	// ����
	MrType_ByEstimate,			// ����
	MrType_ByDoor,				// �ų�
	MrType_UserReport,			// �û��Ա�
	MrType_EmptyHourse,			// �շ�	
	MrType_ZeroGasVol			// ������
}MtrReadType;


//------------------------		�ⲿ�ӿ�����	-------------------------------------

extern SectListSt Sects;
extern MeterListSt Meters;
extern MeterInfoSt MeterInfo;
extern DbQuerySt DbQuery;
extern uint8 **MetersStrs;

extern void QuerySectList(SectListSt *districts, DbQuerySt *query);
extern void QueryMeterList(MeterListSt *meters, DbQuerySt *query);
extern uint8 ShowMeterReadCountInfo(MeterListSt *meters);
extern uint8 ShowMeterList(MeterListSt *meters);
extern void ShowSettingRoutes(void);
extern uint8 ShowAutoMeterReading(MeterListSt *meters);
extern void SaveMeterReadResult(MeterInfoSt *meterInfo, uint8 readType, uint8 readStatus);
extern void QueryMeterInfo(MeterInfoSt *meterInfo, DbQuerySt *query);
extern uint8 ShowMeterInfo(MeterInfoSt *meterInfo);
extern void FixDbfRecCnt(void);

#endif