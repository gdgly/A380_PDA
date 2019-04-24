#ifndef MAIN_H
#define MAIN_H

// --------------------------------  ����  -----------------------------------------

#define CRC16_Seed		0x8408		// ����/���� CRC16�㷨 seed �̶�Ϊ 0x8408


// --------------------------------  ���Ͷ���  -----------------------------------------
typedef unsigned char bool;
#ifndef true
#define true    1
#endif
#ifndef false
#define false   0
#endif

typedef struct{
	uint8 buf[200];
	uint8 *items[10];
	uint8 cnt;
}ParamsBuf;


typedef enum{
	ModType_MainNode,
	ModType_PowerSub,
	ModType_WaterSub
}ModuleTypeDef;

typedef enum{
	// ����
	PowerCmd_ReadMeter_645_07,	
	PowerCmd_ReadMeter_645_97,	
	PowerCmd_ReadMeter_698,

	// �ڵ����
	PowerCmd_SetSerialCom,
	PowerCmd_SetChanelGrp,
	PowerCmd_SetRssiThreshold,
	PowerCmd_SetSendPower,
	PowerCmd_DeviceReboot,
	
	PowerCmd_ParamsInit,			// ���Լ�����
	PowerCmd_StartNwkBuild,		// ���Լ�����
	PowerCmd_StartNwkMaintain,	// ���Լ�����
	PowerCmd_BroadClearNeighbor,	// ���Լ����� Aps���� 97
	PowerCmd_BroadSetSendPower,	// ���Լ�����
	
	PowerCmd_ClearNeighbor,		// ���Ե�� Aps���� 97 00
	PowerCmd_ChangeCollect2Addr,	// ���Ե�� Aps���� 96
	PowerCmd_ForceJoinNwkRequest,// ���Ե�� Aps���� 99
	
	// ������ȡ
	PowerCmd_ReadNodeInfo,
	PowerCmd_ReadSendPower,
	PowerCmd_ReadNwkStatus,
	PowerCmd_ReadVerInfo,
	PowerCmd_ReadSubNodeRoute,	// ���Լ�����
	PowerCmd_ReadAllMeterDoc,	// ���Լ�����
	PowerCmd_ReadNeighbor,		// ���Ե�� Aps���� 97 00

	// ����ˮ��
	PowerCmd_ReadReportData,		
	PowerCmd_ClearReportData,
	PowerCmd_QueryBindedWaterMeter,
	PowerCmd_AddBindedWaterMeter,
	PowerCmd_DelBindedWaterMeter
}PowerCmdDef;

typedef enum{
	WaterCmd_ReadMeterDirect,
	WaterCmd_ReadMeterByPwrMeter,
	WaterCmd_ReadNeighbor,	
	WaterCmd_ClearNeighbor		// aps���� 97 02
}WaterCmdDef;

#endif
