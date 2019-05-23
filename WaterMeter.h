#ifndef WaterMeter_H
#define WaterMeter_H

#include "stdio.h"
#include "Common.h"


//----------------------------------------  �������  ------------------------
/*
��˲�����	
1	��������
2	��������
3	��������
4	Ԥ������
5	��������
6	��������
*/
typedef enum{

	WaterCmd_ReadMeterCfgInfo	 	= 0x04,	// ��ȡ��˲���������Ϣ
	
	/*
	�������	
	1	��ȡ�û�����
	2	��ȡ������ת����
	3	����
	4	ǿ�ƿ���
	5	�ط�
	6	ǿ�ƹط�
	7	���쳣����
	*/
	WaterCmd_ReadRealTimeData		= 0x11,
	WaterCmd_ReadFrozenData,
	WaterCmd_OpenValve,	
	WaterCmd_OpenValveForce,
	WaterCmd_CloseValve,
	WaterCmd_CloseValveForce,	
	WaterCmd_ClearException,

	/*
	�������	
	1	�������
	2	�����¶�
	3	�����ѹ
	4	��Ԥ�ɲο���
	5	���ù�����ʱ
	6	����Ӫ�̱��
	7	���ϱ�·��
	8	���ñ��
	*/
	WaterCmd_RebootDevice				= 0x21,
	WaterCmd_ReadTemperature,
	WaterCmd_ReadVoltage, 
	WaterCmd_ClearPrepaidRefVal,
	WaterCmd_SetOverCurrentTimeout,
	WaterCmd_ReadOperatorNumber,	
	WaterCmd_ReadReportRoute,
	WaterCmd_SetMeterNumber,

	/*
	����������	
	1	��������
	2	��ѯ����
	3	�㲥����
	4	��ӵ���
	5	ɾ������
	6	��ѯ����
	7	����ͳ��
	*/
	WaterCmd_SingleUpgrade			= 0x31,
	WaterCmd_QueryUpgrade,
	WaterCmd_BroadcastUpgrade, 
	WaterCmd_DocAdd,
	WaterCmd_DocDelete,
	WaterCmd_DocQuery,	
	WaterCmd_UpgradeStatistics,

	/*
	Ԥ��������	
	1	��Ԥ�ɲο�����
	2	��Ԥ�ɲο�����
	3	��������ֵ͸֧
	4	�豨����ֵ
	5	��ط���ֵ
	6	�豨���ط���ֵ
	*/
	WaterCmd_ReadPrepaidRefVal		= 0x41,
	WaterCmd_SetPrepaidRefVal,
	WaterCmd_ReadAlarmLimitOverdraft, 
	WaterCmd_SetAlarmLimit,
	WaterCmd_SetCloseValveLimit,
	WaterCmd_SetAlarmAndCloseValveLimit,	

	/*
	����������	
	1	����������ϵ��
	2	�����ת��������
	3	��ȡ����ʹ��״̬
	4	���ö�ʱ�ϴ�
	5	���ö����ϴ�
	6	���ö�ʱ�����ϴ�
	7	�����ʱ��
	8	У���ʱ��
	*/
	WaterCmd_SetBaseValPulseRatio	= 0x51,
	WaterCmd_ClearReverseMeasureData,
	WaterCmd_ReadFuncEnableState, 
	WaterCmd_SetTimedUpload,
	WaterCmd_SetFixedValUpload,
	WaterCmd_SetTimedAndFixedValUpload,	
	WaterCmd_ReadMeterTime,
	WaterCmd_SetMeterTime,

	/*
	����������	
	1	���շ����ŷ�����
	2	��ȡRXD��TXD�ŵ�
	3	����RXD��TXD�ŵ�
	4	������Ӫ�̱��
	5	·���·�
	*/
	WaterCmd_ReadRxTxMgnDistbCnt		= 0x61,
	WaterCmd_ReadRxdAndTxdChanel,
	WaterCmd_SetRxdAndTxdChanel, 
	WaterCmd_SetOperatorNumber,
	WaterCmd_SetDefinedRoute
}WaterCmdDef;


//----------------------------------------  ����������  ------------------------
/*
������������	
1	��������
2	��������
3	·������
4	����ת��
*/
typedef enum{
	/*
	���ò�����	
	1	����������
	2	���������汾
	3	��������ʱ��
	4	�輯����ʱ��
	5	��GPRS����
	6	��GPRS����
	7	��GPRS�ź�ǿ��
	8	��������ʼ��
	9	������������ģʽ
	*/
	CenterCmd_ReadCenterNo		= 0x1011,
	CenterCmd_ReadCenterVer,
	CenterCmd_ReadCenterTime,
	CenterCmd_SetCenterTime,
	CenterCmd_ReadGprsParam,
	CenterCmd_SetGprsParam,
	CenterCmd_ReadGprsSignal,
	CenterCmd_InitCenter,
	CenterCmd_ReadCenterWorkMode,

	/*
	����������	
	1	����������
	2	��������Ϣ
	3	��ӵ�����Ϣ
	4	ɾ��������Ϣ
	5	�޸ĵ�����Ϣ
	*/
	CenterCmd_ReadDocCount		= 0x1021,
	CenterCmd_ReadDocInfo,
	CenterCmd_AddDocInfo,
	CenterCmd_DeleteDocInfo,
	CenterCmd_ModifyDocInfo,

	/*
	·�����ã�	
	1	���Զ���·��
	2	���Զ���·��
	*/
	CenterCmd_ReadDefinedRoute	= 0x1031,
	CenterCmd_SetDefinedRoute,

	/*
	����ת����	
	1	��ʵʱ����
	2	����ʱ��������
	3	����������
	4	����
	5	�ط�
	6	��ʹ��
	7	���쳣
	*/
	CenterCmd_ReadRealTimeData	= 0x1041,
	CenterCmd_ReadFixedTimeData,
	CenterCmd_ReadFrozenData,
	CenterCmd_OpenValve,
	CenterCmd_CloseValve,
	CenterCmd_ReadEnableState,
	CenterCmd_ClearException

}CenterCmdDef;


typedef enum{
	RxResult_Ok,
	RxResult_Failed,
	RxResult_CrcError,
	RxResult_Timeout
}CmdRxResult;


//---------------------------------------		6009 ��������	-------------------------------------

/*
* ��  �������ַ�����ַ�����6009ˮ���ַ��
* ��  ����addrs			- ��ַ��ṹ
*		  strDstAddr	- Ŀ�ĵ�ַ�ַ���
* 		  strRelayAddrs - �м̵�ַ�ַ�������
* ����ֵ��void
*/
void Water6009_PackAddrs(ParamsBuf *addrs, const char strDstAddr[], const char strRelayAddrs[][20])
{
#ifdef Project_6009_RF
	uint8 i;
#endif

	// Դ��ַ
	addrs->itemCnt = 0;
	addrs->items[addrs->itemCnt] = &addrs->buf[0];
	memcpy(addrs->items[addrs->itemCnt], LocalAddr, 6);
	addrs->itemCnt++;

#ifdef Project_6009_RF
	// �м̵�ַ
	for(i = 0; i < RELAY_MAX; i++){
		if(strRelayAddrs[i][0] >= '0' && strRelayAddrs[i][0] <= '9'){
			addrs->items[addrs->itemCnt] = &addrs->buf[6 + i*6];
			GetBytesFromStringHex(addrs->items[addrs->itemCnt], 0, 6, strRelayAddrs[i], 0, false);
			addrs->itemCnt++;
		}
	}
#endif

	// Ŀ�ĵ�ַ
	GetBytesFromStringHex(DstAddr, 0, 6, strDstAddr, 0, false);
	addrs->items[addrs->itemCnt] = &addrs->buf[addrs->itemCnt*6];
	memcpy(addrs->items[addrs->itemCnt], DstAddr, 6);
	addrs->itemCnt++;
}

/*
* ��  ������ȡ6009ˮ��-�豸������
* ��  ����typeId	- ����ID
* ����ֵ��char *	- ��������ַ���
*/
char * Water6009_GetStrDeviceType(uint8 typeId)
{
	char * str = NULL;
	
	switch(typeId){
	case 0x10:	str = "RF��ˮ��";	break;
	case 0x11:	str = "GPRS��ˮ��";	break;
	case 0x12:	str = "NB-IoT��ˮ��";	break;
	case 0x20:	str = "RF��ˮ��";	break;
	case 0x21:	str = "NB-IoT��ˮ��";	break;
	case 0x30:	str = "RF����";	break;
	case 0x31:	str = "GPRS����";	break;
	case 0x32:	str = "NB-IoT����";	break;
	case 0x40:	str = "���";	break;

	case 0x50:	str = "͸��ģ��";	break;

	case 0xF9:	str = "USB";	break;
	case 0xFA:	str = "��λ����";	break;
	case 0xFB:	str = "UART����";	break;
	case 0xFC:	str = "������";	break;
	case 0xFD:	str = "�м���";	break;
	case 0xFE:	str = "�ֳֻ�";	break;
	default:
		str = "δ֪";
		break;
	}

	return str;
}

/*
* ��  ������ȡ6009ˮ�����������
* ��  ����typeId	- ����ID
* ����ֵ��char *	- ��������ַ���
*/
char * Water6009_GetStrValueType(uint8 typeId)
{
	char * str = NULL;
	
	switch(typeId){
	case 0x00:	str = "ʵʱ";	break;
	case 0x01:	str = "�����ϴ�";	break;
	case 0x02:	str = "��ʱ�ϴ�";	break;
	case 0x03:	str = "�����ϴ�";	break;
	case 0x04:	str = "����";	break;
	default:
		str = "δ֪";
		break;
	}

	return str;
}

/*
* ��  ��������6009ˮ��-�澯״̬��1
* ��  ����status	- ״̬��
* ����ֵ��char *	- ��������ַ���
*/
char * Water6009_GetStrAlarmStatus1(uint8 status)
{
	char * str = NULL;
	uint8 mask = 1, i;

	for(i = 0; i < 8; i++){

		mask = (1 << i);
		
		switch(status & mask){
		case 0x01:	str = "�ɻɹܹ���";	break;
		case 0x02:	str = "����λ����";	break;
		case 0x04:	str = "�������߶Ͽ�";	break;
		case 0x08:	str = "���Ƿѹ";	break;
		case 0x10:	str = "����һ���ܻ�";	break;
		case 0x20:	str = "�Ÿ��ű�־";	break;
		case 0x40:	str = "���������ܻ�";	break;
		case 0x80:	str = "������ǿ�����";	break;
		default:
			break;
		}

		if(str != NULL){
			break;
		}
	}

	if(str == NULL){
		str = " ";
	}

	return str;
}

/*
* ��  ��������6009ˮ��-�澯״̬��2
* ��  ����status	- ״̬��
* ����ֵ��char *	- ��������ַ���
*/
char * Water6009_GetStrAlarmStatus2(uint8 status)
{
	char * str = NULL;
	uint8 mask = 1, i;

	for(i = 0; i < 6; i++){

		mask = (1 << i);
		
		switch(status & mask){
		case 0x01:	str = "ˮ��ת";	break;
		case 0x02:	str = "ˮ����ж";	break;
		case 0x04:	str = "ˮ����ֱ��װ";	break;
		case 0x08:	str = "EEPROM�쳣";	break;
		case 0x10:	str = "ú��й©";	break;
		case 0x20:	str = "Ƿ�ѱ�־";	break;
		default:	
			break;
		}

		if(str != NULL){
			break;
		}
	}

	if(str == NULL){
		str = " ";
	}
	
	return str;
}

/*
* ��  ������ȡ6009ˮ�� ����״̬
* ��  ����status	- ״̬
* ����ֵ��char *	- ��������ַ���
*/
char * Water6009_GetStrValveStatus(uint8 status)
{
	char * str = NULL;
	
	switch(status){
	case 0:	str = "����";	break;
	case 1:	str = "��";	break;
	case 2:	str = "��";	break;
	default:
		str = "δ֪";
		break;
	}

	return str;
}
/*
* ��  ������ȡ6009ˮ�� ������������
* ��  ����status	- ״̬
* ����ֵ��char *	- ��������ַ���
*/
char * Water6009_GetStrErrorMsg(uint8 errorCode)
{
	char * str = NULL;

	switch(errorCode){
	case 0xAA:
		str = "�����ɹ�";
		break;
	case 0xAB:
	    str = "����ʧ��";
		break;
	case 0xAC:
	    str = "ͨѶʧ��";
		break;
	case 0xAD:
	    str = "�����´�ɹ�";
		break;
	case 0xAE:
	    str = "��ʽ����";
		break;
	case 0xAF:
	    str = "Ԥ��";
		break;
	case 0xBA:
	    str = "���󲻴���";
		break;
	case 0xBB:
	    str = "�����ظ�";
		break;
	case 0xBC:
		str = "��������";
		break;
	case 0xCC:
	    str = "��ʱ����";
		break;
	case 0xCD:
	    str = "�������г�ʱ";
		break;
	case 0xCE:
	    str = "����ִ��";
		break;
	case 0xCF:
	    str = "�����Ѵ���";
		break;
	case 0xD0:
	    str = "��Ӧ��";
		break;
	case 0xD1:
	    str = "�������";
		break;
	case 0xD2:
	    str = "�޴˹���";
		break;
	default:
		str = "δ֪����";
		break;
	}

	return str;
}

/*
* ��  ������ȡ6009ˮ�� ����ʧ��ԭ��
* ��  ����errorCode	- ������
* ����ֵ��char *	- ��������ַ���
*/
char * Water6009_GetStrValveCtrlFailed(uint16 errorCode)
{
	char * str = NULL;
	uint16 mask = 1, i;

	for(i = 0; i < 12; i++){

		mask = (mask << i);
		
		switch(errorCode & mask){
		case 0x01:	str = "���Ƿѹ";	break;
		case 0x02:	str = "�Ÿ�����";	break;
		case 0x04:	str = "Adc���ڹ���";	break;
		case 0x08:	str = "������������";	break;
		case 0x10:	str = "���Ź���";	break;
		case 0x20:	str = "RF���ڹ���";	break;
		case 0x40:	str = "��������ʧ��";	break;
		case 0x80:	str = "�ȴ���������";	break;
		case 0x100:	str = "�����Ѿ���λ";	break;
		case 0x200:	str = "�豸���ʹ���";	break;
		case 0x400:	str = "time����ʧ��";	break;
		case 0x800:	str = "ϵͳǷ��";	break;
		default:	
			break;
		}

		if(str != NULL){
			break;
		}
	}

	if(str == NULL){
		str = " ";
	}

	return str;
}

/*
* ��  ������ȡ6009ˮ�� ����ʹ��״̬
* ��  ����stateCode	- ʹ��״̬��
* 		  buf		- ʹ��״̬���ַ������������
* ����ֵ��uint16	- ��������ַ����ܳ���
*/
uint16 Water6009_GetStrMeterFuncEnableState(uint16 stateCode, char * buf)
{
	uint16 len = 0;

	len += sprintf(&buf[len], "�Ÿ��Źط�����  :%s\n", ((stateCode & 0x0001) > 0 ? "��" : " ��"));
	len += sprintf(&buf[len], "�ϱ����ݼ���    :%s\n", ((stateCode & 0x0002) > 0 ? "��" : " ��"));
	len += sprintf(&buf[len], "����ж��⹦��  :%s\n", ((stateCode & 0x0004) > 0 ? "��" : " ��"));
	len += sprintf(&buf[len], "��ֱ��װ���    :%s\n", ((stateCode & 0x0008) > 0 ? "��" : " ��"));
	len += sprintf(&buf[len], "�����澯        :%s\n", ((stateCode & 0x0010) > 0 ? "��" : " ��"));
	len += sprintf(&buf[len], "�����ϴ���������:%s\n", ((stateCode & 0x0020) > 0 ? "��" : " ��"));
	len += sprintf(&buf[len], "͸֧�ط�����    :%s\n", ((stateCode & 0x0040) > 0 ? "��" : " ��"));
	len += sprintf(&buf[len], "Ԥ���ѹ���      :%s\n", ((stateCode & 0x0080) > 0 ? "��" : " ��"));
	len += sprintf(&buf[len], "�Զ��ŵ�����    :%s\n", ((stateCode & 0x0100) > 0 ? "��" : " ��"));
	len += sprintf(&buf[len], "���⹦��        :%s\n", ((stateCode & 0x0200) > 0 ? "��" : " ��"));
	len += sprintf(&buf[len], "����ط�����    :%s\n", ((stateCode & 0x0400) > 0 ? "��" : " ��"));
	len += sprintf(&buf[len], "RF���߲���      :%s\n", ((stateCode & 0x0800) > 0 ? "��" : " ��"));
	len += sprintf(&buf[len], "�����Զ��ط�    :%s\n", ((stateCode & 0x1000) > 0 ? "��" : " ��"));
	len += sprintf(&buf[len], "ú��й©���    :%s\n", ((stateCode & 0x2000) > 0 ? "��" : " ��"));

	len += sprintf(&buf[len], "���ٿ��ƹ���    :%s\n", ((stateCode & 0x8000) > 0 ? "��" : " ��"));

	return len;
}


//-----------------------------------		6009ˮ��Э�� ��� / ���	-----------------------------


/*
* ��������PackWater6009RequestFrame
* ��  �������6009ˮ����������֡
* ��  ����buf	- ���ݻ�����ʼ��ַ
		  addrs - ��ַ��Դ��ַ���м̵�ַ��Ŀ�ĵ�ַ
		  cmdId - ������
		  args	- ���������
		  retryCnt - �ط�������0 - ��1�η��ͣ����� - ��n���ط�
* ����ֵ��uint8 ֡�ܳ���
*/
uint8 PackWater6009RequestFrame(uint8 * buf, ParamsBuf *addrs, uint16 cmdId, ParamsBuf *args, uint8 retryCnt)
{
	static uint8 Fsn = 0;
	static uint16 index = 0;
	uint8 i, cmd , crc8;

	if(retryCnt > 0 && index > 0){
		return index;
	}

	index = 0;
	buf[index++] = 0xD3;		// ֡ͷͬ���룺 �̶�Ϊ D3 91
	buf[index++] = 0x91;
	buf[index++] = 0x00;		// ���ȣ� ���ı�ʶ --> ������16
	buf[index++] = 0x00;	
	buf[index++] = 0x10;		// ���ı�־ bit7 0/1 - ����/���У� bit6 0/1 - ����/Ӧ�� bit4 �̶�Ϊ1
	buf[index++] = Fsn++;		// ����ţ� mac fsn �������ۼ�
	cmd = *args->items[0];
	buf[index++] = cmd;			// ������
	buf[index++] = 0xFE;		// �豸����: FE - �ֳֻ��� 10 - ��ˮ�� 11 - GPRSˮ��
	buf[index++] = 0x0F;		// ��������
	buf[index++] = addrs->itemCnt & 0x0F;	// ·����Ϣ:  ��ǰλ��|·������
	// ��ַ��
	for(i = 0; i < (addrs->itemCnt & 0x0F); i++){
		memcpy(&buf[index], addrs->items[i], 6);
		index += 6;
	}

	// ������
	memcpy(&buf[index], args->items[args->itemCnt -1], args->lastItemLen);
	index += args->lastItemLen;

	buf[index++] = 0x00;		// ���г�ǿ
	buf[index++] = 0x00;		// ���г�ǿ
    // ���ȼ���
	buf[2] = (uint8)(index & 0xFF);	
	buf[3] = (uint8)(index >> 8);	
    
	crc8 = GetCrc8(&buf[2], index - 2);
	buf[index++] = crc8;		// crc8 У��
	buf[index++] = 0x16;		// ������

		

	if(cmd < 0x40 || cmd == 0x70 || cmd == 0x74){
		buf[index++] = 0x1E;	// ���Գ��ȱ�ʶ
		buf[index++] = 0x03;	// ���APPʱ �����ŵ�
		buf[index++] = 0x19;	// ���APPʱ �����ŵ�
	}else if(cmd > 0x70 && cmd < 0x74){
		buf[index++] = 0x1E;	// ���Գ��ȱ�ʶ
		buf[index++] = 0x03;	// ���Bootʱ �����ŵ�
		buf[index++] = 0x19;	// ���Bootʱ �����ŵ�
	}else{
		buf[index++] = 0x00;	// ���Գ��ȱ�ʶ
		buf[index++] = 0x19;	// ������ �����ŵ�
		buf[index++] = 0x03;	// ������ �����ŵ�
	}
	
	return index;
}

/*
* ��������ExplainWater6009ResponseFrame
* ��  ��������ˮ��������Ӧ֡
* ��  ����buf		- ���ջ�����ʼ��ַ
*		  rxlen		- ���յĳ���
*		  dstAddr	- Ŀ�ĵ�ַ���жϽ��յ�Ŀ�ĵ�ַ�Ƿ����Լ�
*		  cmdId		- ������
*		  ackLen	- Ӧ�𳤶�
*		  dispBuf 	- ��������ʾ����
* ����ֵ��uint8 ���������0 - �ɹ� , 1 - ʧ�� �� 2 - CRC���� 3 - ��ʱ��Ӧ��
*/
uint8 ExplainWater6009ResponseFrame(uint8 * buf, uint16 rxlen, const uint8 * dstAddr, uint16 cmdId, uint16 ackLen, char *dispBuf)
{
	bool ret = RxResult_Failed;
	uint8 crc8, addrsCnt, cmd, i, u8Tmp;
	uint16 index = 0, dispIdx, length, startIdx, payloadIdx, u16Tmp;
	uint32 u32Tmp;
	char *ptr;

	dispIdx = 0;

	// ��ʾ��� �� ��������
	if(cmdId < 0x1010){
		dispIdx += sprintf(&dispBuf[dispIdx], "���: %s\n", StrDstAddr);
	}else{
		dispIdx += sprintf(&dispBuf[dispIdx], "��������: \n      %s\n", StrDstAddr);
	}

	// ����������в���
	while(1){

		if(rxlen < index + 27){
			sprintf(&dispBuf[dispIdx], "���: ��ʱ,��Ӧ��");
			return RxResult_Timeout;
		}

		// start check
		if(buf[index] == 0xD3 && buf[index + 1] == 0x91){
			index += 2;
		}else{
			index++;
			continue;
		}
		
		// length check
		length = (uint16)((buf[index + 1] << 8) + buf[index]);
		if((index + length) > rxlen){
			index += length;
			continue;
		}	

		// dstaddr check
		addrsCnt = buf[index + 7];
		if(memcmp(&buf[index + 8 + (addrsCnt - 1) * 6], dstAddr, 6) != 0){
			index += length;
			continue;
		}

		// crc8 check
		crc8 = GetCrc8(&buf[index], length - 2);
		if(crc8 !=  buf[index + length - 2]){
			sprintf(&dispBuf[dispIdx], "���: ��Ӧ��,CRC����");
			return RxResult_CrcError;
		}

		// pass
		break;
	}

	startIdx = index;

	// ������
	cmd = buf[index + 4];

	// ���� ���� --> ·����Ϣ
	index += 8;
	// ���� ��ַ��
	index += addrsCnt * 6;

	// ������ת��������ͷ��
	if(cmd == 0x4D){
		// ������
		cmd = buf[index];
		index += 1;
		// ���
		GetStringHexFromBytes(&TmpBuf[0], &buf[index], 0, 6, 0, false);
		TmpBuf[6] = 0x00;
		dispIdx += sprintf(&dispBuf[dispIdx], "���: %s\n", &TmpBuf[0]);
		index += 6;
		// ת�����
		ptr = Water6009_GetStrErrorMsg(buf[index]);
		dispIdx += sprintf(&dispBuf[dispIdx], "���: %s\n", ptr);
		index += 1;
	}

	// ���������
	payloadIdx = index;
	switch(cmdId){
	
	//-------------------------------------------------------------------------------------------------
	//--------------------------------------	������� 0x01 ~ 0x25 , 70 ~ 74		--------------------
	//-------------------------------------------------------------------------------------------------
	//----------------------------------------		��ȡ�û�����		-------------
	case WaterCmd_ReadRealTimeData:	// ��ȡ�û�����
	case CenterCmd_ReadRealTimeData:
		if(rxlen < index + 21 && cmd != 0x01){
			break;
		}
		ret = RxResult_Ok;
		
		// ����
		ptr = Water6009_GetStrValueType((buf[index] >> 4));
		dispIdx += sprintf(&dispBuf[dispIdx], "����: %s\n", ptr);
		index += 1;
		// ��ת����
		u32Tmp = ((buf[index + 3] << 24) + (buf[index + 2] << 16) + (buf[index + 1] << 8) + buf[index]);
		index += 4;
		u16Tmp = ((buf[index + 1] << 8) + buf[index]);
		index += 2;
		dispIdx += sprintf(&dispBuf[dispIdx], "��ת: %d.%03d\n", u32Tmp, u16Tmp);
		// ��ת����
		u32Tmp = ((buf[index + 3] << 24) + (buf[index + 2] << 16) + (buf[index + 1] << 8) + buf[index]);
		index += 4;
		u16Tmp = ((buf[index + 1] << 8) + buf[index]);
		index += 2;
		dispIdx += sprintf(&dispBuf[dispIdx], "��ת: %d.%03d\n", u32Tmp, u16Tmp);
		//�澯״̬��1
		ptr = Water6009_GetStrAlarmStatus1(buf[index]);
		dispIdx += sprintf(&dispBuf[dispIdx], "�澯: %s ", ptr);
		index += 1;
		//�澯״̬��2
		ptr = Water6009_GetStrAlarmStatus2(buf[index]);
		dispIdx += sprintf(&dispBuf[dispIdx], "%s\n", ptr);
		index += 1;
		//����״̬ 
		ptr = Water6009_GetStrValveStatus(buf[index]);
		dispIdx += sprintf(&dispBuf[dispIdx], "����: %s  ", ptr);
		index += 1;
		//��ص�ѹ
		dispIdx += sprintf(&dispBuf[dispIdx], "���: %c.%c\n", (buf[index] / 10) + '0', (buf[index] % 10) + '0');
		index += 1;
		//�����¶�
		ptr = ((buf[index] & 0x80) > 0 ? "-" : "");
		dispIdx += sprintf(&dispBuf[dispIdx], "�¶�: %s%d  ", ptr, (buf[index] & 0x7F));
        index += 1;
		//SNR ������
		dispIdx += sprintf(&dispBuf[dispIdx], "SNR : %d\n", buf[index]);
		index += 1;
		//tx|rx�ŵ�
		dispIdx += sprintf(&dispBuf[dispIdx], "�ŵ�: Tx-%d, Rx-%d\n", (buf[index] & 0x0F), (buf[index] >> 4));
		index += 1;
		//Э��汾
		dispIdx += sprintf(&dispBuf[dispIdx], "Э��汾: %d\n", buf[index]);
		index += 1;
		break;

	//----------------------------------------		��ȡ��������	---------------------
	case WaterCmd_ReadFrozenData:	// ��ȡ��������
		if(rxlen < index + 88 && cmd != 0x02){
			break;
		}
		ret = RxResult_Ok;
		// ������������
		dispIdx += sprintf(&dispBuf[dispIdx], "����: %s\n", (buf[index] == 0x01 ? "����" : "��ת"));
		index += 1;

		if(rxlen < index + 104){	// �������ݸ�ʽ-�ɰ汾 1 + 78 byte
			// ����������ʼ���
			u8Tmp = buf[index] * 10;
			dispIdx += sprintf(&dispBuf[dispIdx], "��Χ: �� %d~%d ��\n", u8Tmp, u8Tmp + 9);
			index += 1;
			// ����������ʼʱ��
			dispIdx += sprintf(&dispBuf[dispIdx], "ʱ��: %X%x%x%x %x:00:00\n"
				, buf[payloadIdx + 2], buf[payloadIdx + 3], buf[payloadIdx + 4], buf[payloadIdx + 5], buf[payloadIdx + 6]);
			index += 5;
			// �������ݷ�ʽ ��0-����, 1-����
			// ������������ ���������24�����������30��
			dispIdx += sprintf(&dispBuf[dispIdx], "��ʽ: ÿ%s����%d��\n", (buf[index] == 0x01 ? "��" : "��"), buf[index + 1]);
			index += 2;	
			// ��������ʱ����
			if(buf[index] == 0){
				dispIdx += sprintf(&dispBuf[dispIdx], "���: ÿ%s����1��\n", (buf[index - 2] == 0x01 ? "��" : "��"));
			}
			else{
				dispIdx += sprintf(&dispBuf[dispIdx], "���: %d%s����1��\n", buf[index], (buf[index - 2] == 0x01 ? "Сʱ" : "��"));
			}
			index += 1;
			// ������������ݣ�7*N �ֽ� ��6 byte ���� + 1 byte date.day��
			dispIdx += sprintf(&dispBuf[dispIdx], "��ȡ��10����������: \n");
			for(i = 0; i < 10; i++){
				u32Tmp = ((buf[index + 3] << 24) + (buf[index + 2] << 16) + (buf[index + 1] << 8) + buf[index]);
				index += 4;
				u16Tmp = ((buf[index + 1] << 8) + buf[index]);
				index += 2;
				dispIdx += sprintf(&dispBuf[dispIdx], "%d, %x/%x: %d.%03d\n", i, buf[payloadIdx + 4], buf[index], u32Tmp, u16Tmp);
				index +=1;
			}
		}
		else{		// �������ݸ�ʽ-�°汾	1 + 104 byte
			// ����������ʼ���
			dispIdx += sprintf(&dispBuf[dispIdx], "��Χ: ������%d������\n", buf[index] + 1);
			index += 1;
			// ʱ����Ϣ
			dispIdx += sprintf(&dispBuf[dispIdx], "ʱ��: %02X-%02X %02X:%02X\n",
				buf[index], buf[index + 1], buf[index + 2], buf[index + 3]);
			index += 4;
			// �ۼ�����
			u32Tmp = ((buf[index + 3] << 24) + (buf[index + 2] << 16) + (buf[index + 1] << 8) + buf[index]);
			index += 4;
			u16Tmp = ((buf[index + 1] << 8) + buf[index]);
			index += 2;
			dispIdx += sprintf(&dispBuf[dispIdx], "�ۼ�����: %d.%03d\n", u32Tmp, u16Tmp);
			// 0:00 ~ 23:30 ����
			u8Tmp = 0;
			u16Tmp = 0x00;
			for(i = 0; i < 47; i++){
				dispIdx += sprintf(&dispBuf[dispIdx], "%d:%02X~", u8Tmp, u16Tmp);
				u16Tmp += 0x30;
				if(u16Tmp == 0x60){
					u16Tmp = 0x00;
					u8Tmp += 1;
				}
				dispIdx += sprintf(&dispBuf[dispIdx], "%d:%02X����:%d\n", u8Tmp, u16Tmp, (buf[index] + buf[index + 1]*256));
				index += 2;
			}
		}

		//�澯״̬��1
		ptr = Water6009_GetStrAlarmStatus1(buf[index]);
		dispIdx += sprintf(&dispBuf[dispIdx], "�澯: %s ", ptr);
		index += 1;
		//�澯״̬��2
		ptr = Water6009_GetStrAlarmStatus2(buf[index]);
		dispIdx += sprintf(&dispBuf[dispIdx], "%s\n", ptr);
		index += 1;
		//����״̬ 
		ptr = Water6009_GetStrValveStatus(buf[index]);
		dispIdx += sprintf(&dispBuf[dispIdx], "����: %s  ", ptr);
		index += 1;
		//��ص�ѹ
		dispIdx += sprintf(&dispBuf[dispIdx], "���: %c.%c\n", (buf[index] / 10) + '0', (buf[index] % 10) + '0');
		index += 1;
		//�����¶�
		ptr = ((buf[index] & 0x80) > 0 ? "-" : "");
		dispIdx += sprintf(&dispBuf[dispIdx], "�¶�: %s%d  ", ptr, (buf[index] & 0x7F));
        index += 1;
		//SNR ������
		dispIdx += sprintf(&dispBuf[dispIdx], "SNR : %d\n", buf[index]);
		index += 1;
		//tx|rx�ŵ�
		dispIdx += sprintf(&dispBuf[dispIdx], "�ŵ�: Tx-%d, Rx-%d\n", (buf[index] & 0x0F), (buf[index] >> 4));
		index += 1;
		//Э��汾
		dispIdx += sprintf(&dispBuf[dispIdx], "Э��汾: %d\n", buf[index]);
		index += 1;
		break;

	//---------------------------------------		���ط���	---------------------
	case WaterCmd_OpenValve:		// ����
	case WaterCmd_OpenValveForce:	// ǿ�ƿ���
	case WaterCmd_CloseValve:		// �ط�
	case WaterCmd_CloseValveForce:	// ǿ�ƹط�
	case CenterCmd_OpenValve:
	case CenterCmd_CloseValve:
		if(rxlen < index + 3 && cmd != 0x03){
			break;
		}
		ret = RxResult_Ok;
		// ����״̬
		ptr = Water6009_GetStrErrorMsg(buf[index]);
		dispIdx += sprintf(&dispBuf[dispIdx], "���: %s\n", ptr);
		if(buf[index] == 0xAB){
			ptr = Water6009_GetStrValveCtrlFailed(((buf[index + 1] << 8) + buf[index + 2]));
			dispIdx += sprintf(&dispBuf[dispIdx], "ԭ��: %s\n", ptr);
		}
		index += 3;
		break;

	//----------------------------------------		��ȡ���������Ϣ		-------------
	case WaterCmd_ReadMeterCfgInfo:	// ��ȡ���������Ϣ

		if(rxlen < index + 124 && cmd != 0x04){
			break;
		}
		ret = RxResult_Ok;
		index += 84;
		memcpy(VerInfo, &buf[index], 40);
		memcpy(&TmpBuf[1020], &buf[index], 40);
		TmpBuf[1060] = 0x00;
		dispIdx += sprintf(&dispBuf[dispIdx], "�汾: %s\n", &TmpBuf[1020]);
		index += 40;
		break;

	//---------------------------------------		���쳣����		---------------------
	case WaterCmd_ClearException:	// ���쳣���� 
	case CenterCmd_ClearException:
		if(rxlen < index + 1 && cmd != 0x05){
			break;
		}
		ret = RxResult_Ok;
		// ����״̬
		ptr = Water6009_GetStrErrorMsg(buf[index]);
		dispIdx += sprintf(&dispBuf[dispIdx], "���: %s\n", ptr);
		index += 1;
		break;

	//---------------------------------------		��������	---------------------
	case WaterCmd_RebootDevice:	// �������
		if(rxlen < index + 2 && cmd != 0x07){
			break;
		}
		ret = RxResult_Ok;
		index += 1;
		// ����״̬
		ptr = Water6009_GetStrErrorMsg(buf[index]);
		dispIdx += sprintf(&dispBuf[dispIdx], "���: %s\n", ptr);
		index += 1;
		break;

	case WaterCmd_ReadTemperature:	// �����¶�
		if(rxlen < index + 1 && cmd != 0x07){
			break;
		}
		ret = RxResult_Ok;
		index += 1;
		//�����¶�
		ptr = ((buf[index] & 0x80) > 0 ? "-" : "");
		dispIdx += sprintf(&dispBuf[dispIdx], "�¶�: %s%d\n", ptr, (buf[index] & 0x7F));
        index += 1;
		break;

	case WaterCmd_ReadVoltage:	// �����ѹ
		if(rxlen < index + 1 && cmd != 0x07){
			break;
		}
		ret = RxResult_Ok;
		index += 1;
		//��ص�ѹ
		dispIdx += sprintf(&dispBuf[dispIdx], "��ص�ѹ: %c.%c\n", (buf[index] / 10) + '0', (buf[index] % 10) + '0');
		index += 1;
		break;

	case WaterCmd_ClearPrepaidRefVal:	// ��Ԥ�ɲο���
		if(rxlen < index + 1 && cmd != 0x07){
			break;
		}
		ret = RxResult_Ok;
		index += 1;
		// ����״̬
		ptr = Water6009_GetStrErrorMsg(buf[index]);
		dispIdx += sprintf(&dispBuf[dispIdx], "���: %s\n", ptr);
		index += 1;
		break;

	case WaterCmd_SetOverCurrentTimeout:	// ���ù�����ʱ
		if(rxlen < index + 3 && cmd != 0x07){
			break;
		}
		ret = RxResult_Ok;
		index += 1;
		// ����״̬
		if(buf[index] != Args.buf[2] || buf[index + 1] != Args.buf[3] || buf[index + 2] != Args.buf[4]){
			ptr = Water6009_GetStrErrorMsg(buf[index]);
			dispIdx += sprintf(&dispBuf[dispIdx], "���: %s\n", ptr);
			index += 1;
			ret = RxResult_Failed;
		}
		else{
			dispIdx += sprintf(&dispBuf[dispIdx], "���: �����ɹ�\n");
			// ��������
			dispIdx += sprintf(&dispBuf[dispIdx], "��������: %d mA\n", buf[index]);
			index += 1;
			// ��ʱʱ��
			dispIdx += sprintf(&dispBuf[dispIdx], "��ʱʱ��: %d ms\n", (buf[index] + buf[index + 1] * 256));
			index += 2;
		}
		break;

	case WaterCmd_ReadOperatorNumber:	// ����Ӫ�̱��
		if(rxlen < index + 4 && cmd != 0x07){
			break;
		}
		ret = RxResult_Ok;
		index += 1;
		// ��Ӫ�̱��
		dispIdx += sprintf(&dispBuf[dispIdx], "��Ӫ�̱��: %2X%2X%2X%2X\n", 
			buf[index], buf[index + 1], buf[index + 2], buf[index + 3]);
		index += 4;
		break;

	case WaterCmd_ReadReportRoute:	// ���ϱ�·��
		if(rxlen < index + 63 && cmd != 0x07){
			break;
		}
		ret = RxResult_Ok;
		index += 1;

		// ·��1���� 6*n
		u8Tmp = buf[index] / 6;
		dispIdx += sprintf(&dispBuf[dispIdx], "·��1����: %d\n", u8Tmp);
		index += 1;
		// ·��1��ַ�б�
		for(i = 0; i < u8Tmp; i++){
			GetStringHexFromBytes((char *)&TmpBuf[0], buf, index, 6, 0, false);
			TmpBuf[12] = 0x00;
			dispIdx += sprintf(&dispBuf[dispIdx], "  %d: %s\n", i + 1, &TmpBuf[0]);
			index += 6;
		}
		index += (30 - u8Tmp * 6);

		// ·��2���� 6*n
		u8Tmp = buf[index] / 6;
		dispIdx += sprintf(&dispBuf[dispIdx], "  \n·��2����: %d\n", u8Tmp);
		index += 1;
		// ·��2��ַ�б�
		for(i = 0; i < u8Tmp; i++){
			GetStringHexFromBytes((char *)&TmpBuf[0], buf, index, 6, 0, false);
			TmpBuf[12] = 0x00;
			dispIdx += sprintf(&dispBuf[dispIdx], "  %d: %s\n", i + 1, &TmpBuf[0]);
			index += 6;
		}
		index += (30 - u8Tmp * 6);

		break;

	case WaterCmd_SetMeterNumber:	// ���ñ��
		if(rxlen < index + 1 && cmd != 0x07){
			break;
		}
		ret = RxResult_Ok;
		// ����״̬
		ptr = Water6009_GetStrErrorMsg(buf[index]);
		dispIdx += sprintf(&dispBuf[dispIdx], "���: %s\n", ptr);
		index += 1;
		break;

	//--------------------------------------		Ԥ���������ο�����-��ȡ/����	---------------------
	case WaterCmd_ReadPrepaidRefVal:	// ��Ԥ�ɲο�����
		if(rxlen < index + 12 && cmd != 0x15){
			break;
		}
		ret = RxResult_Ok;
		// Ԥ������
		u32Tmp = ((buf[index + 3] << 24) + (buf[index + 2] << 16) + (buf[index + 1] << 8) + buf[index]);
		index += 4;
		u16Tmp = ((buf[index + 1] << 8) + buf[index]);
		index += 2;
		dispIdx += sprintf(&dispBuf[dispIdx], "Ԥ������: %d.%03d\n", u32Tmp, u16Tmp);
		// �ο�����
		u32Tmp = ((buf[index + 3] << 24) + (buf[index + 2] << 16) + (buf[index + 1] << 8) + buf[index]);
		index += 4;
		u16Tmp = ((buf[index + 1] << 8) + buf[index]);
		index += 2;
		dispIdx += sprintf(&dispBuf[dispIdx], "�ο�����: %d.%03d\n", u32Tmp, u16Tmp);
		break;

	case WaterCmd_SetPrepaidRefVal:		// ��Ԥ�ɲο�����
		if(rxlen < index + 2 && cmd != 0x16){
			break;
		}
		ret = RxResult_Ok;
		// ����״̬
		ptr = (buf[index] == 0xAA ? "�����ɹ�" : "���ݷǷ�");
		ret = (buf[index] == 0xAA ? RxResult_Ok : RxResult_Failed);
		dispIdx += sprintf(&dispBuf[dispIdx], "���: %s\n", ptr);
		index += 1;
		// ���ݷǷ�ԭ��
		if(buf[index - 1] == 0xAE){
			if(buf[index] & 0x01 > 0){
				dispIdx += sprintf(&dispBuf[dispIdx], "-->�ο���ʼ�������Ϸ�\n");
			}
			if(buf[index] & 0x02 > 0){
				dispIdx += sprintf(&dispBuf[dispIdx], "-->���õ�Ԥ������δ�ﵽ��������\n");
			}
			index += 1;
		}
		break;

	case WaterCmd_ReadAlarmLimitOverdraft:		// �������ط���ֵ
		if(rxlen < index + 1 && cmd != 0x17){
			break;
		}
		ret = RxResult_Ok;
		// ����״̬
		ptr = (buf[index] == 0xAB ? "����ʧ��" : "�����ɹ�");
		ret = (buf[index] == 0xAA ? RxResult_Ok : RxResult_Failed);
		dispIdx += sprintf(&dispBuf[dispIdx], "���: %s\n", ptr);
		index += 1;
		// �����ɹ�ʱ���
		if(buf[index - 1] == 0xAA){
			dispIdx += sprintf(&dispBuf[dispIdx], "������ֵ: %d\n", buf[index]);
			index += 1;
			u16Tmp = ((uint16)(buf[index + 1] << 8) + buf[index]);
			dispIdx += sprintf(&dispBuf[dispIdx], "�ط���ֵ: %s%d\n", 
				((u16Tmp & 0x8000) > 0 ? "-" : ""), (u16Tmp & 0x7FFF));
			index += 2;
		}
		break;

	case WaterCmd_SetAlarmLimit:				// �豨����ֵ
	case WaterCmd_SetCloseValveLimit:			// ��ط���ֵ
	case WaterCmd_SetAlarmAndCloseValveLimit:	// �豨���ط���ֵ
		if(rxlen < index + 2 && cmd != 0x18){
			break;
		}
		ret = RxResult_Ok;
		// ����״̬
		ptr = (buf[index] == 0xAA ? "�����ɹ�" : "����ʧ��");
		ret = (buf[index] == 0xAA ? RxResult_Ok : RxResult_Failed);
		dispIdx += sprintf(&dispBuf[dispIdx], "���: %s\n", ptr);
		index += 1;
		// �°����+ 2 byte
		break;

	//--------------------------------------		��������	---------------------
	case WaterCmd_SetBaseValPulseRatio:	// ����������ϵ��
		if(rxlen < index + 7 && cmd != 0x06){
			break;
		}
		ret = RxResult_Ok;
		// �û�����
		u32Tmp = ((buf[index + 3] << 24) + (buf[index + 2] << 16) + (buf[index + 1] << 8) + buf[index]);
		index += 4;
		u16Tmp = ((buf[index + 1] << 8) + buf[index]);
		index += 2;
		dispIdx += sprintf(&dispBuf[dispIdx], "�û�����:%d.%03d\n", u32Tmp, u16Tmp);
		// ����ϵ��
		switch (buf[index]){
		case 0x00:	u16Tmp = 1;	break;
		case 0x01:	u16Tmp = 10;	break;
		case 0x02:	u16Tmp = 100;	break;
		case 0x03:	u16Tmp = 1000;	break;
		default:  u16Tmp = buf[index];	break;
		}
		dispIdx += sprintf(&dispBuf[dispIdx], "����ϵ��:%d����/��\n", u16Tmp);
		index += 1;
		break;

	case WaterCmd_ClearReverseMeasureData:	// �����ת��������
		if(rxlen < index + 6 && cmd != 0x0A){
			break;
		}
		ret = RxResult_Ok;
		// ��ת����
		u32Tmp = ((buf[index + 3] << 24) + (buf[index + 2] << 16) + (buf[index + 1] << 8) + buf[index]);
		index += 4;
		u16Tmp = ((buf[index + 1] << 8) + buf[index]);
		index += 2;
		dispIdx += sprintf(&dispBuf[dispIdx], "��ת����:%d.%03d\n", u32Tmp, u16Tmp);
		break;

	case WaterCmd_ReadFuncEnableState:	// ��ȡ����ʹ��״̬
	case CenterCmd_ReadEnableState:
		if(rxlen < index + 2 && cmd != 0x0B){
			break;
		}
		ret = RxResult_Ok;
		u16Tmp = (buf[index] + buf[index + 1] * 256);
		dispIdx += Water6009_GetStrMeterFuncEnableState(u16Tmp, &dispBuf[dispIdx]);
		index += 2;
		break;

	case WaterCmd_SetTimedUpload:		// ���ö�ʱ�ϴ�
	case WaterCmd_SetFixedValUpload:	// ���ö����ϴ�
	case WaterCmd_SetTimedAndFixedValUpload:	// ���ö�ʱ�����ϴ�
		if(rxlen < index + 2 && cmd != 0x0C){
			break;
		}
		ret = RxResult_Ok;
		if(buf[index] == 0xAA){
			dispIdx += sprintf(&dispBuf[dispIdx], "���: �����ɹ�\n");
			index += 1;
		}
		if(rxlen >= index + 2 + 4){		// ��Э������
			dispIdx += sprintf(&dispBuf[dispIdx], "��ʱ�ϴ����:%d Сʱ\n", buf[index]);
			index += 1;
			dispIdx += sprintf(&dispBuf[dispIdx], "�����ϴ����:%d m3\n", buf[index]);
			index += 1;
		}
		break;

	case WaterCmd_ReadMeterTime:	// �����ʱ��
		if(rxlen < index + 7 && cmd != 0x13){
			break;
		}
		ret = RxResult_Ok;
		dispIdx += sprintf(&dispBuf[dispIdx], "���ʱ��: \n %02X%02X-%02X-%02X %02X:%02X:%02X\n", 
			buf[index], buf[index + 1], buf[index + 2], buf[index + 3]
			, buf[index + 4], buf[index + 5], buf[index + 6]);
		index += 7;
		break;

	case WaterCmd_SetMeterTime:		// У���ʱ��
		if(rxlen < index + 1 && cmd != 0x14){
			break;
		}
		// ����״̬
		ptr = (buf[index] == 0xAA ? "�����ɹ�" : "����ʧ��");
		ret = (buf[index] == 0xAA ? RxResult_Ok : RxResult_Failed);
		dispIdx += sprintf(&dispBuf[dispIdx], "���: %s\n", ptr);
		index += 1;
		break;

	//--------------------------------------		��������		---------------------
	case WaterCmd_ReadRxTxMgnDistbCnt:		// ���շ����ŷ�����
		if(rxlen < index + 7 && cmd != 0x09){
			break;
		}
		ret = RxResult_Ok;
		dispIdx += sprintf(&dispBuf[dispIdx], "�������: %d\n", (buf[index] + buf[index + 1] * 256));
		index += 2;
		dispIdx += sprintf(&dispBuf[dispIdx], "���մ���: %d\n", (buf[index] + buf[index + 1] * 256));
		index += 2;
		dispIdx += sprintf(&dispBuf[dispIdx], "���ط�����: %d\n", (buf[index] + buf[index + 1] * 256));
		index += 2;
		dispIdx += sprintf(&dispBuf[dispIdx], "�Ÿ��Ŵ���: %d\n", (buf[index]));
		index += 1;
		break;

	case WaterCmd_ReadRxdAndTxdChanel:	// ��ȡRXD��TXD�ŵ�
		if(rxlen < index + 2 && cmd != 0x1B){
			break;
		}
		ret = RxResult_Ok;
		if(buf[index] == 0xAB){
			ret = RxResult_Failed;
			dispIdx += sprintf(&dispBuf[dispIdx], "���: ����ʧ��\n");
			index += 1;
			if(rxlen >= index + 1 + 4){
				index += 1;		// ʧ��ԭ��
			}
		}
		else{
			dispIdx += sprintf(&dispBuf[dispIdx], "�����ŵ�: %d\n", (buf[index] & 0x7F));
			index += 1;
			dispIdx += sprintf(&dispBuf[dispIdx], "�ϱ��ŵ�: %d\n", (buf[index] & 0x7F));
			index += 1;
		}
		break;

	case WaterCmd_SetRxdAndTxdChanel:	// ����RXD��TXD�ŵ�
		if(rxlen < index + 2 && cmd != 0x1B){
			break;
		}
		ret = RxResult_Ok;
		if(buf[index] == 0xAB){
			ret = RxResult_Failed;
			dispIdx += sprintf(&dispBuf[dispIdx], "���: ����ʧ��\n");
			index += 1;
			if(rxlen >= index + 1 + 4){
				index += 1;		// ʧ��ԭ��
			}
		}
		else{
			dispIdx += sprintf(&dispBuf[dispIdx], "�����ŵ�: %d\n", (buf[index]));
			index += 1;
			dispIdx += sprintf(&dispBuf[dispIdx], "�ϱ��ŵ�: %d\n", (buf[index]));
			index += 1;
		}
		break;

	case WaterCmd_SetOperatorNumber:		// ������Ӫ�̱��
		if(rxlen < index + 1 && cmd != 0x21){
			break;
		}
		// ����״̬
		ptr = (buf[index] == 0xAA ? "�����ɹ�" : "����ʧ��");
		ret = (buf[index] == 0xAA ? RxResult_Ok : RxResult_Failed);
		dispIdx += sprintf(&dispBuf[dispIdx], "���: %s\n", ptr);
		index += 1;
		if(rxlen >= index + 1 + 4){
			index += 1;		// ʧ��ԭ��
		}
		break;

	case WaterCmd_SetDefinedRoute:	// ·���·�
		if(rxlen < index + 1 && cmd != 0x22){
			break;
		}
		// ����״̬
		ptr = (buf[index] == 0xAA ? "�����ɹ�" : "����ʧ��");
		ret = (buf[index] == 0xAA ? RxResult_Ok : RxResult_Failed);
		dispIdx += sprintf(&dispBuf[dispIdx], "���: %s\n", ptr);
		index += 1;
		if(rxlen >= index + 1 + 4){
			index += 1;		// ʧ��ԭ��
		}
		break;


	//--------------------------------------		��������		---------------------
	case WaterCmd_SingleUpgrade:		// ��������
		if(rxlen < index + 12 && cmd != 0x15){
			break;
		}
		ret = RxResult_Ok;

		break;

	case WaterCmd_QueryUpgrade:			// ��ѯ����
		if(rxlen < index + 12 && cmd != 0x15){
			break;
		}
		ret = RxResult_Ok;
		
		break;

	case WaterCmd_BroadcastUpgrade:		// �㲥����
		if(rxlen < index + 12 && cmd != 0x15){
			break;
		}
		ret = RxResult_Ok;
		
		break;

	case WaterCmd_DocAdd:			// ��ӵ���
		if(rxlen < index + 12 && cmd != 0x15){
			break;
		}
		ret = RxResult_Ok;
		
		break;

	case WaterCmd_DocDelete:		// ɾ������
		if(rxlen < index + 12 && cmd != 0x15){
			break;
		}
		ret = RxResult_Ok;
		
		break;

	case WaterCmd_DocQuery:			// ��ѯ����
		if(rxlen < index + 12 && cmd != 0x15){
			break;
		}
		ret = RxResult_Ok;
		
		break;

	case WaterCmd_UpgradeStatistics:	// ����ͳ��
		if(rxlen < index + 12 && cmd != 0x15){
			break;
		}
		ret = RxResult_Ok;
		
		break;


	//-------------------------------------------------------------------------------------------------
	//--------------------------------------	���������� 0x40 ~ 0x65 , F1 ~ F3	--------------------
	//-------------------------------------------------------------------------------------------------
	//--------------------------------------		���ò���		-------------------------------------
	case CenterCmd_ReadCenterNo:	// ����������
		if(rxlen < index + 6 && cmd != 0x41){
			break;
		}
		ret = RxResult_Ok;
		// ��������
		dispIdx += sprintf(&dispBuf[dispIdx], "��������: \n    %02X%02X%02X%02X%02X%02X\n", 
			buf[index], buf[index + 1], buf[index + 2], buf[index + 3], buf[index + 4], buf[index + 5]);
		index += 6;
		break;

	case CenterCmd_ReadCenterVer:		// ���������汾
		if(rxlen < index + 6 && cmd != 0x40){
			break;
		}
		ret = RxResult_Ok;
		// ����汾
		dispIdx += sprintf(&dispBuf[dispIdx], "����汾: %02X%02X \n", buf[index], buf[index + 1]);
		index += 2;
		// Ӳ���汾
		dispIdx += sprintf(&dispBuf[dispIdx], "Ӳ���汾: %02X%02X \n", buf[index], buf[index + 1]);
		index += 2;
		// Э��汾
		dispIdx += sprintf(&dispBuf[dispIdx], "Э��汾: %02X%02X \n", buf[index], buf[index + 1]);
		index += 2;
		break;

	case CenterCmd_ReadCenterTime:		// ��������ʱ��
		if(rxlen < index + 7 && cmd != 0x43){
			break;
		}
		ret = RxResult_Ok;
		// ������ʱ��
		dispIdx += sprintf(&dispBuf[dispIdx], "������ʱ��: \n %02X%02X-%02X-%02X %02X:%02X:%02X\n", 
			buf[index], buf[index + 1], buf[index + 2], buf[index + 3]
			, buf[index + 4], buf[index + 5], buf[index + 6]);
		index += 7;
		break;

	case CenterCmd_SetCenterTime:		// �輯����ʱ��
		if(rxlen < index + 1 && cmd != 0x44){
			break;
		}
		ret = RxResult_Ok;
		// ����״̬
		ptr = Water6009_GetStrErrorMsg(buf[index]);
		dispIdx += sprintf(&dispBuf[dispIdx], "���: %s\n", ptr);
		index += 1;
		break;

	case CenterCmd_ReadGprsParam:		// ��GPRS����
		if(rxlen < index + 16 && cmd != 0x45){
			break;
		}
		ret = RxResult_Ok;
		// ����IP + �˿�
		dispIdx += sprintf(&dispBuf[dispIdx], "����IP: %d.%d.%d.%d\n", 
			buf[index], buf[index + 1], buf[index + 2], buf[index + 3]);
		index += 4;
		dispIdx += sprintf(&dispBuf[dispIdx], "  �˿�: %d\n", (buf[index] + buf[index + 1] * 256));
		index += 2;
		// ����IP + �˿�
		dispIdx += sprintf(&dispBuf[dispIdx], "����IP: %d.%d.%d.%d\n", 
			buf[index], buf[index + 1], buf[index + 2], buf[index + 3]);
		index += 4;
		dispIdx += sprintf(&dispBuf[dispIdx], "  �˿�: %d\n", (buf[index] + buf[index + 1] * 256));
		index += 2;
		// ���������
		dispIdx += sprintf(&dispBuf[dispIdx], "���������: %d s\n", (buf[index] * 10));
		// A P N
		u8Tmp = buf[index];
		memcpy(&TmpBuf[0], &buf[index + 1], u8Tmp);
		TmpBuf[u8Tmp] = 0x00;
		dispIdx += sprintf(&dispBuf[dispIdx], "A P N: %s\n", &TmpBuf[0]);
		index += (u8Tmp + 1);
		// �û���
		u8Tmp = buf[index];
		memcpy(&TmpBuf[0], &buf[index + 1], u8Tmp);
		TmpBuf[u8Tmp] = 0x00;
		dispIdx += sprintf(&dispBuf[dispIdx], "�û���: %s\n", &TmpBuf[0]);
		index += (u8Tmp + 1);
		// ��  ��
		u8Tmp = buf[index];
		memcpy(&TmpBuf[0], &buf[index + 1], u8Tmp);
		TmpBuf[u8Tmp] = 0x00;
		dispIdx += sprintf(&dispBuf[dispIdx], "��  ��: %s\n", &TmpBuf[0]);
		index += (u8Tmp + 1);
		break;

	case CenterCmd_SetGprsParam:		// ��GPRS����
		if(rxlen < index + 1 && cmd != 0x46){
			break;
		}
		ret = RxResult_Ok;
		// ����״̬
		ptr = Water6009_GetStrErrorMsg(buf[index]);
		dispIdx += sprintf(&dispBuf[dispIdx], "���: %s\n", ptr);
		index += 1;
		break;

	case CenterCmd_ReadGprsSignal:		// ��GPRS�ź�ǿ��
		if(rxlen < index + 4 && cmd != 0x47){
			break;
		}
		ret = RxResult_Ok;
		// �ź�ǿ��
		if(buf[index] == 0){
			ptr = "<= -113dBm";
		}
		else if(buf[index] < 31){
			sprintf(&TmpBuf[0], "-%ddBm", 113 - 2 * buf[index]);
			ptr = &TmpBuf[0];
		}
		else if(buf[index] == 31){
			ptr = ">= -53dBm";
		}
		else if(buf[index] == 99){
			ptr = "δ֪";
		}
		dispIdx += sprintf(&dispBuf[dispIdx], "�ź�ǿ��: %s\n", ptr);
		index += 1;
		// ����״̬
		ptr = (buf[index] == 0 ? "����" : "����");
		dispIdx += sprintf(&dispBuf[dispIdx], "����״̬: %s\n", ptr);
		index += 1;
		// IMSI
		u8Tmp = buf[index];
		memcpy(&TmpBuf[0], &buf[index + 1], u8Tmp);
		TmpBuf[u8Tmp] = 0x00;
		dispIdx += sprintf(&dispBuf[dispIdx], "IMSI: %s\n", &TmpBuf[0]);
		index += (u8Tmp + 1);
		// GMM
		u8Tmp = buf[index];
		memcpy(&TmpBuf[0], &buf[index + 1], u8Tmp);
		TmpBuf[u8Tmp] = 0x00;
		dispIdx += sprintf(&dispBuf[dispIdx], "GMM: %s\n", &TmpBuf[0]);
		index += (u8Tmp + 1);
		break;

	case CenterCmd_InitCenter:		// ��������ʼ��
		if(rxlen < index + 2 && cmd != 0x48){
			break;
		}
		ret = RxResult_Ok;
		// �����ϴ�����
		ptr = (buf[index] == 0 ? "��յ�����·��" : "�����������");
		dispIdx += sprintf(&dispBuf[dispIdx], "����: %s\n", ptr);
		index += 1;
		// ����״̬
		ptr = Water6009_GetStrErrorMsg(buf[index]);
		dispIdx += sprintf(&dispBuf[dispIdx], "���: %s\n", ptr);
		index += 1;
		break;

	case CenterCmd_ReadCenterWorkMode:		// ������������ģʽ
		if(rxlen < index + 7 && cmd != 0x49){
			break;
		}
		ret = RxResult_Ok;
		// ��������
		ptr = (buf[index] == 0 ? "����ʱ����" : (buf[index] == 1 ? "������" : "��Ч"));
		dispIdx += sprintf(&dispBuf[dispIdx], "��������: %s\n", ptr);
		index += 1;
		// ���ݲ�������
		ptr = ((buf[index] & 0x80) > 0 ? "��" : "��");
		dispIdx += sprintf(&dispBuf[dispIdx], "���ݲ�������: %s\n", ptr);
		// �����ϴ�����
		ptr = ((buf[index] & 0x40) > 0 ? "��" : "��");
		dispIdx += sprintf(&dispBuf[dispIdx], "�����ϴ�����: %s\n", ptr);
		index += 1;
		dispIdx += sprintf(&dispBuf[dispIdx], "�����ϴ�ʱ��: %2X��\n", buf[index]);
		index += 1;
		dispIdx += sprintf(&dispBuf[dispIdx], "���ݲ�������-λ���: \n");
		dispIdx += sprintf(&dispBuf[dispIdx], " 1- 7�� �������: %02X\n", (buf[index] & 0x7F));
		index += 1;
		dispIdx += sprintf(&dispBuf[dispIdx], " 8-15�� �������: %02X\n", (buf[index]));
		index += 1;
		dispIdx += sprintf(&dispBuf[dispIdx], "16-23�� �������: %02X\n", (buf[index]));
		index += 1;
		dispIdx += sprintf(&dispBuf[dispIdx], "24-31�� �������: %02X\n", (buf[index]));
		index += 1;
		break;


	//--------------------------------------		����������		-------------------------------------
	case CenterCmd_ReadDocCount:		// ����������
		if(rxlen < index + 3 && cmd != 0x50){
			break;
		}
		ret = RxResult_Ok;
		// ����״̬
		ptr = Water6009_GetStrDeviceType(buf[index]);
		dispIdx += sprintf(&dispBuf[dispIdx], "�豸����: %s\n", ptr);
		index += 1;
		dispIdx += sprintf(&dispBuf[dispIdx], "��������: %d\n", (buf[index] + buf[index + 1] * 256));
		index += 2;
		break;

	case CenterCmd_ReadDocInfo:			// ��������Ϣ
		if(rxlen < index + 3 && cmd != 0x51){
			break;
		}
		ret = RxResult_Ok;
		// �ڵ�������
		dispIdx += sprintf(&dispBuf[dispIdx], "�ڵ�������  : %d\n", (buf[index] + buf[index + 1] * 256));
		index += 2;
		// �ڵ�������
		dispIdx += sprintf(&dispBuf[dispIdx], "����Ӧ������: %d\n", (buf[index]));
		index += 1;
		// N���ڵ���Ϣ
		u8Tmp = buf[index - 1];
		for(i = 0; i < u8Tmp; i++){
			GetStringHexFromBytes(&TmpBuf[0], &buf[index], 0, 6, 0, false);
			TmpBuf[6] = 0x00;
			dispIdx += sprintf(&dispBuf[dispIdx], "�ڵ� %2d:%s\n", i + 1, &TmpBuf[0]);
			index += 6;
			ptr = Water6009_GetStrDeviceType(buf[index]);
			dispIdx += sprintf(&dispBuf[dispIdx], "   ����:%s\n", ptr);
			index += 1;
			ptr = (buf[index] == 0 ? "ʧ��" : (buf[index] == 1 ? "�ɹ�" : "δ֪"));
			dispIdx += sprintf(&dispBuf[dispIdx], "   ����:%s\n", ptr);
			index += 1;
		}
		break;

	case CenterCmd_AddDocInfo:			// ��ӵ�����Ϣ
		if(rxlen < index + 1 && cmd != 0x52){
			break;
		}
		ret = RxResult_Ok;
		// ���õĽڵ�����
		dispIdx += sprintf(&dispBuf[dispIdx], "���õĽڵ�����: %d\n", (buf[index]));
		index += 1;
		// N���ڵ����ý��
		u8Tmp = buf[index - 1];
		for(i = 0; i < u8Tmp; i++){
			GetStringHexFromBytes(&TmpBuf[0], &buf[index], 0, 6, 0, false);
			TmpBuf[6] = 0x00;
			dispIdx += sprintf(&dispBuf[dispIdx], "�ڵ� %2d:%s\n", i + 1, &TmpBuf[0]);
			index += 6;
			ptr = Water6009_GetStrErrorMsg(buf[index]);
			dispIdx += sprintf(&dispBuf[dispIdx], "   ���:%s\n", ptr);
			index += 1;
		}
		break;

	case CenterCmd_DeleteDocInfo:			// ɾ��������Ϣ
		if(rxlen < index + 7 && cmd != 0x53){
			break;
		}
		ret = RxResult_Ok;
		// N���ڵ����ý��
		u8Tmp = (rxlen - index - 4) / 7;
		for(i = 0; i < u8Tmp; i++){
			GetStringHexFromBytes(&TmpBuf[0], &buf[index], 0, 6, 0, false);
			TmpBuf[6] = 0x00;
			dispIdx += sprintf(&dispBuf[dispIdx], "�ڵ� %2d:%s\n", i + 1, &TmpBuf[0]);
			index += 6;
			ptr = Water6009_GetStrErrorMsg(buf[index]);
			dispIdx += sprintf(&dispBuf[dispIdx], "   ���:%s\n", ptr);
			index += 1;
		}
		break;

	case CenterCmd_ModifyDocInfo:			// �޸ĵ�����Ϣ
		if(rxlen < index + 1 && cmd != 0x54){
			break;
		}
		ret = RxResult_Ok;
		// ����״̬
		ptr = Water6009_GetStrErrorMsg(buf[index]);
		dispIdx += sprintf(&dispBuf[dispIdx], "���: %s\n", ptr);
		index += 1;
		break;

	//--------------------------------------		·�����ã�		-------------------------------------
	case CenterCmd_ReadDefinedRoute:		// ���Զ���·��
		if(rxlen < index + 10 && cmd != 0x55){
			break;
		}
		ret = RxResult_Ok;
		// ����״̬
		ptr = Water6009_GetStrErrorMsg(buf[index]);
		dispIdx += sprintf(&dispBuf[dispIdx], "���: %s\n", ptr);
		index += 1;
		// ���
		GetStringHexFromBytes(&TmpBuf[0], &buf[index], 0, 6, 0, false);
		TmpBuf[6] = 0x00;
		dispIdx += sprintf(&dispBuf[dispIdx], "���: %s\n", &TmpBuf[0]);
		index += 6;
		// �豸����
		ptr = Water6009_GetStrDeviceType(buf[index]);
		dispIdx += sprintf(&dispBuf[dispIdx], "����: %s\n", ptr);
		index += 1;
		// ·��1����
		u8Tmp = buf[index];
		dispIdx += sprintf(&dispBuf[dispIdx], "·��1����: %d\n", u8Tmp);
		index += 1;
		// ·��1��ַ�б�
		for(i = 0; i < u8Tmp; i++){
			GetStringHexFromBytes((char *)&TmpBuf[0], buf, index, 6, 0, false);
			TmpBuf[12] = 0x00;
			dispIdx += sprintf(&dispBuf[dispIdx], "  %d: %s\n", i + 1, &TmpBuf[0]);
			index += 6;
		}
		// ·��2����
		u8Tmp = buf[index];
		dispIdx += sprintf(&dispBuf[dispIdx], "·��2����: %d\n", u8Tmp);
		index += 1;
		// ·��2��ַ�б�
		for(i = 0; i < u8Tmp; i++){
			GetStringHexFromBytes((char *)&TmpBuf[0], buf, index, 6, 0, false);
			TmpBuf[12] = 0x00;
			dispIdx += sprintf(&dispBuf[dispIdx], "  %d: %s\n", i + 1, &TmpBuf[0]);
			index += 6;
		}
		break;

	case CenterCmd_SetDefinedRoute:			// ���Զ���·��
		if(rxlen < index + 7 && cmd != 0x56){
			break;
		}
		ret = RxResult_Ok;
		// ���
		GetStringHexFromBytes(&TmpBuf[0], &buf[index], 0, 6, 0, false);
		TmpBuf[6] = 0x00;
		dispIdx += sprintf(&dispBuf[dispIdx], "���: %s\n", &TmpBuf[0]);
		index += 6;
		// ����״̬
		ptr = Water6009_GetStrErrorMsg(buf[index]);
		dispIdx += sprintf(&dispBuf[dispIdx], "���: %s\n", ptr);
		index += 1;
		break;

	//--------------------------------------		����ת����		-------------------------------------
	case CenterCmd_ReadFixedTimeData:			// ����ʱ��������
		if(rxlen < index + 28 && cmd != 0x63){
			break;
		}
		ret = RxResult_Ok;
		// ���
		GetStringHexFromBytes(&TmpBuf[0], &buf[index], 0, 6, 0, false);
		TmpBuf[6] = 0x00;
		dispIdx += sprintf(&dispBuf[dispIdx], "���: %s\n", &TmpBuf[0]);
		index += 6;
		// ����״̬
		ptr = Water6009_GetStrErrorMsg(buf[index]);
		dispIdx += sprintf(&dispBuf[dispIdx], "���: %s\n", ptr);
		index += 1;
		// ʱ��
		dispIdx += sprintf(&dispBuf[dispIdx], "ʱ��: \n %02X%02X-%02X-%02X %02X:%02X:%02X\n", 
			buf[index], buf[index + 1], buf[index + 2], buf[index + 3]
			, buf[index + 4], buf[index + 5], buf[index + 6]);
		index += 7;
		// ����
		ptr = Water6009_GetStrValueType((buf[index] >> 4));
		dispIdx += sprintf(&dispBuf[dispIdx], "����: %s\n", ptr);
		index += 1;
		// ��ת����
		u32Tmp = ((buf[index + 3] << 24) + (buf[index + 2] << 16) + (buf[index + 1] << 8) + buf[index]);
		index += 4;
		u16Tmp = ((buf[index + 1] << 8) + buf[index]);
		index += 2;
		dispIdx += sprintf(&dispBuf[dispIdx], "��ת: %d.%03d\n", u32Tmp, u16Tmp);
		// ��ת����
		u32Tmp = ((buf[index + 3] << 24) + (buf[index + 2] << 16) + (buf[index + 1] << 8) + buf[index]);
		index += 4;
		u16Tmp = ((buf[index + 1] << 8) + buf[index]);
		index += 2;
		dispIdx += sprintf(&dispBuf[dispIdx], "��ת: %d.%03d\n", u32Tmp, u16Tmp);
		//�澯״̬��1
		ptr = Water6009_GetStrAlarmStatus1(buf[index]);
		dispIdx += sprintf(&dispBuf[dispIdx], "�澯: %s ", ptr);
		index += 1;
		//�澯״̬��2
		ptr = Water6009_GetStrAlarmStatus2(buf[index]);
		dispIdx += sprintf(&dispBuf[dispIdx], "%s\n", ptr);
		index += 1;
		//����״̬ 
		ptr = Water6009_GetStrValveStatus(buf[index]);
		dispIdx += sprintf(&dispBuf[dispIdx], "����: %s  ", ptr);
		index += 1;
		//��ص�ѹ
		dispIdx += sprintf(&dispBuf[dispIdx], "���: %c.%c\n", (buf[index] / 10) + '0', (buf[index] % 10) + '0');
		index += 1;
		//�����¶�
		ptr = ((buf[index] & 0x80) > 0 ? "-" : "");
		dispIdx += sprintf(&dispBuf[dispIdx], "�¶�: %s%d  ", ptr, (buf[index] & 0x7F));
        index += 1;
		//SNR ������
		dispIdx += sprintf(&dispBuf[dispIdx], "SNR : %d\n", buf[index]);
		index += 1;
		//tx|rx�ŵ�
		dispIdx += sprintf(&dispBuf[dispIdx], "�ŵ�: Tx-%d, Rx-%d\n", (buf[index] & 0x0F), (buf[index] >> 4));
		index += 1;
		//Э��汾
		dispIdx += sprintf(&dispBuf[dispIdx], "Э��汾: %d\n", buf[index]);
		index += 1;
		break;

	case CenterCmd_ReadFrozenData:			// ����������
		if(rxlen < index + 7 && cmd != 0x64){
			break;
		}
		ret = RxResult_Ok;
		// ���
		GetStringHexFromBytes(&TmpBuf[0], &buf[index], 0, 6, 0, false);
		TmpBuf[6] = 0x00;
		dispIdx += sprintf(&dispBuf[dispIdx], "���: %s\n", &TmpBuf[0]);
		index += 6;
		// ����״̬
		ptr = Water6009_GetStrErrorMsg(buf[index]);
		dispIdx += sprintf(&dispBuf[dispIdx], "���: %s\n", ptr);
		index += 1;
		// ������������
		dispIdx += sprintf(&dispBuf[dispIdx], "����: %s\n", (buf[index] == 0x01 ? "����" : "��ת"));
		index += 1;

		if(rxlen < index + 104){	// �������ݸ�ʽ-�ɰ汾 1 + 78 byte
			// ����������ʼ���
			u8Tmp = buf[index] * 10;
			dispIdx += sprintf(&dispBuf[dispIdx], "��Χ: �� %d~%d ��\n", u8Tmp, u8Tmp + 9);
			index += 1;
			// ����������ʼʱ��
			dispIdx += sprintf(&dispBuf[dispIdx], "ʱ��: %X%x%x%x %x:00:00\n"
				, buf[payloadIdx + 2], buf[payloadIdx + 3], buf[payloadIdx + 4], buf[payloadIdx + 5], buf[payloadIdx + 6]);
			index += 5;
			// �������ݷ�ʽ ��0-����, 1-����
			// ������������ ���������24�����������30��
			dispIdx += sprintf(&dispBuf[dispIdx], "��ʽ: ÿ%s����%d��\n", (buf[index] == 0x01 ? "��" : "��"), buf[index + 1]);
			index += 2;	
			// ��������ʱ����
			if(buf[index] == 0){
				dispIdx += sprintf(&dispBuf[dispIdx], "���: ÿ%s����1��\n", (buf[index - 2] == 0x01 ? "��" : "��"));
			}
			else{
				dispIdx += sprintf(&dispBuf[dispIdx], "���: %d%s����1��\n", buf[index], (buf[index - 2] == 0x01 ? "Сʱ" : "��"));
			}
			index += 1;
			// ������������ݣ�7*N �ֽ� ��6 byte ���� + 1 byte date.day��
			dispIdx += sprintf(&dispBuf[dispIdx], "��ȡ��10����������: \n");
			for(i = 0; i < 10; i++){
				u32Tmp = ((buf[index + 3] << 24) + (buf[index + 2] << 16) + (buf[index + 1] << 8) + buf[index]);
				index += 4;
				u16Tmp = ((buf[index + 1] << 8) + buf[index]);
				index += 2;
				dispIdx += sprintf(&dispBuf[dispIdx], "%d, %x/%x: %d.%03d\n", i, buf[payloadIdx + 4], buf[index], u32Tmp, u16Tmp);
				index +=1;
			}
		}
		else{		// �������ݸ�ʽ-�°汾	1 + 104 byte
			// ����������ʼ���
			dispIdx += sprintf(&dispBuf[dispIdx], "��Χ: ������%d������\n", buf[index] + 1);
			index += 1;
			// ʱ����Ϣ
			dispIdx += sprintf(&dispBuf[dispIdx], "ʱ��: %02X-%02X %02X:%02X\n",
				buf[index], buf[index + 1], buf[index + 2], buf[index + 3]);
			index += 4;
			// �ۼ�����
			u32Tmp = ((buf[index + 3] << 24) + (buf[index + 2] << 16) + (buf[index + 1] << 8) + buf[index]);
			index += 4;
			u16Tmp = ((buf[index + 1] << 8) + buf[index]);
			index += 2;
			dispIdx += sprintf(&dispBuf[dispIdx], "�ۼ�����: %d.%03d\n", u32Tmp, u16Tmp);
			// 0:00 ~ 23:30 ����
			u8Tmp = 0;
			u16Tmp = 0x00;
			for(i = 0; i < 47; i++){
				dispIdx += sprintf(&dispBuf[dispIdx], "%d:%02X~", u8Tmp, u16Tmp);
				u16Tmp += 0x30;
				if(u16Tmp == 0x60){
					u16Tmp = 0x00;
					u8Tmp += 1;
				}
				dispIdx += sprintf(&dispBuf[dispIdx], "%d:%02X����:%d\n", u8Tmp, u16Tmp, (buf[index] + buf[index + 1]*256));
				index += 2;
			}
		}

		//�澯״̬��1
		ptr = Water6009_GetStrAlarmStatus1(buf[index]);
		dispIdx += sprintf(&dispBuf[dispIdx], "�澯: %s ", ptr);
		index += 1;
		//�澯״̬��2
		ptr = Water6009_GetStrAlarmStatus2(buf[index]);
		dispIdx += sprintf(&dispBuf[dispIdx], "%s\n", ptr);
		index += 1;
		//����״̬ 
		ptr = Water6009_GetStrValveStatus(buf[index]);
		dispIdx += sprintf(&dispBuf[dispIdx], "����: %s  ", ptr);
		index += 1;
		//��ص�ѹ
		dispIdx += sprintf(&dispBuf[dispIdx], "���: %c.%c\n", (buf[index] / 10) + '0', (buf[index] % 10) + '0');
		index += 1;
		//�����¶�
		ptr = ((buf[index] & 0x80) > 0 ? "-" : "");
		dispIdx += sprintf(&dispBuf[dispIdx], "�¶�: %s%d  ", ptr, (buf[index] & 0x7F));
        index += 1;
		//SNR ������
		dispIdx += sprintf(&dispBuf[dispIdx], "SNR : %d\n", buf[index]);
		index += 1;
		//tx|rx�ŵ�
		dispIdx += sprintf(&dispBuf[dispIdx], "�ŵ�: Tx-%d, Rx-%d\n", (buf[index] & 0x0F), (buf[index] >> 4));
		index += 1;
		//Э��汾
		dispIdx += sprintf(&dispBuf[dispIdx], "Э��汾: %d\n", buf[index]);
		index += 1;
		break;


	default:
		ret = RxResult_Ok;
		dispIdx += sprintf(&dispBuf[dispIdx], "������[%02X]��δ����\n", cmd);
		break;
	}


	if(index == startIdx + length - 4)
	{
		//����/���� �ź�ǿ��
		dispIdx += sprintf(&dispBuf[dispIdx], "                    \n");
		dispIdx += sprintf(&dispBuf[dispIdx], "����: %d  ����: %d\n", buf[index], buf[index + 1]);
		index += 2;
	}

	dispBuf[dispIdx] = '\0';
	
	
	return ret;
}

#endif
