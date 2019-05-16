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
	1	����/��/���Ŵ���
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



//---------------------------------------		6009 ��������	-------------------------------------

/*
* ��  �������ַ�����ַ�����6009ˮ���ַ��
* ��  ����addrs			- ��ַ��ṹ
*		  strDstAddr	- Ŀ�ĵ�ַ�ַ���
* 		  strRelayAddrs - �м̵�ַ�ַ�������
* ����ֵ��void
*/
void Water6009_PackAddrs(ParamsBuf *addrs, const uint8 strDstAddr[], const uint8 strRelayAddrs[][20])
{
	uint8 i;

	// Դ��ַ
	addrs->itemCnt = 0;
	addrs->items[addrs->itemCnt] = &addrs->buf[0];
	memcpy(addrs->items[addrs->itemCnt], LocalAddr, 6);
	addrs->itemCnt++;
	// �м̵�ַ
	for(i = 0; i < RELAY_MAX; i++){
		if(strRelayAddrs[i][0] >= '0' && strRelayAddrs[i][0] <= '9'){
			addrs->items[addrs->itemCnt] = &addrs->buf[6 + i*6];
			GetBytesFromStringHex(addrs->items[addrs->itemCnt], 0, 6, strRelayAddrs[i], 0, false);
			addrs->itemCnt++;
		}
	}
	// Ŀ�ĵ�ַ
	GetBytesFromStringHex(DstAddr, 0, 6, strDstAddr, 0, false);
	addrs->items[addrs->itemCnt] = &addrs->buf[6 + i*6];
	memcpy(addrs->items[addrs->itemCnt], DstAddr, 6);
	addrs->itemCnt++;
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
uint8 PackWater6009RequestFrame(uint8 * buf, ParamsBuf *addrs, uint8 cmdId, ParamsBuf *args, uint8 retryCnt)
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
*		  disp 		- ��������ʾ����
* ����ֵ��bool ���������fasle - ʧ�� �� true - �ɹ�
*/
bool ExplainWater6009ResponseFrame(uint8 * buf, uint16 rxlen, const uint8 * dstAddr, uint8 cmdId, uint16 ackLen, ParamsBuf * disps)
{
	bool ret = false;
	uint8 crc8, addrsCnt, cmd, i, u8Tmp;
	uint16 index = 0, dispIdx, length, startIdx, payloadIdx, u16Tmp;
	uint32 u32Tmp;
	uint8 *ptr;

	// ��ʾ���
	dispIdx = 0;
	disps->items[0] = &disps->buf[0];
	disps->itemCnt = 1;
	dispIdx += sprintf(&disps->buf[dispIdx], "���: %s\n", StrDstAddr);

	// ����������в���
	while(1){

		if(rxlen < index + 27){
			sprintf(&disps->buf[dispIdx], "���: ��ʱ,��Ӧ��");
			return false;
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
			sprintf(&disps->buf[dispIdx], "���: ��Ӧ��,CRC����");
			return false;
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


	// ���������
	payloadIdx = index;
	switch(cmdId){
	
	//----------------------------------------		��ȡ�û�����		-------------
	case WaterCmd_ReadRealTimeData:	// ��ȡ�û�����

		if(rxlen < index + 21 && cmd != 0x01){
			break;
		}
		ret = true;
		
		// ����
		ptr = Water6009_GetStrValueType((buf[index] >> 4));
		dispIdx += sprintf(&disps->buf[dispIdx], "����: %s\n", ptr);
		index += 1;
		// ��ת����
		u32Tmp = ((buf[index + 3] << 24) + (buf[index + 2] << 16) + (buf[index + 1] << 8) + buf[index]);
		index += 4;
		u16Tmp = ((buf[index + 1] << 8) + buf[index]);
		index += 2;
		dispIdx += sprintf(&disps->buf[dispIdx], "��ת: %d.%03d\n", u32Tmp, u16Tmp);
		// ��ת����
		u32Tmp = ((buf[index + 3] << 24) + (buf[index + 2] << 16) + (buf[index + 1] << 8) + buf[index]);
		index += 4;
		u16Tmp = ((buf[index + 1] << 8) + buf[index]);
		index += 2;
		dispIdx += sprintf(&disps->buf[dispIdx], "��ת: %d.%03d\n", u32Tmp, u16Tmp);
		//�澯״̬��1
		ptr = Water6009_GetStrAlarmStatus1(buf[index]);
		dispIdx += sprintf(&disps->buf[dispIdx], "�澯: %s ", ptr);
		index += 1;
		//�澯״̬��2
		ptr = Water6009_GetStrAlarmStatus2(buf[index]);
		dispIdx += sprintf(&disps->buf[dispIdx], "%s\n", ptr);
		index += 1;
		//����״̬ 
		ptr = Water6009_GetStrValveStatus(buf[index]);
		dispIdx += sprintf(&disps->buf[dispIdx], "����: %s  ", ptr);
		index += 1;
		//��ص�ѹ
		dispIdx += sprintf(&disps->buf[dispIdx], "���: %c.%c\n", (buf[index] / 10) + '0', (buf[index] % 10) + '0');
		index += 1;
		//�����¶�
		ptr = ((buf[index] & 0x80) > 0 ? "-" : "");
		dispIdx += sprintf(&disps->buf[dispIdx], "�¶�: %s%d  ", ptr, (buf[index] & 0x7F));
        index += 1;
		//SNR ������
		dispIdx += sprintf(&disps->buf[dispIdx], "SNR : %d\n", buf[index]);
		index += 1;
		//tx|rx�ŵ���Э��汾 ����
		index += 2;
		break;

	//----------------------------------------		��ȡ��������	---------------------
	case WaterCmd_ReadFrozenData:	// ��ȡ��������
		if(rxlen < index + 88 && cmd != 0x02){
			break;
		}
		ret = true;
		// ������������
		dispIdx += sprintf(&disps->buf[dispIdx], "����: %s\n", (buf[index] == 0x01 ? "����" : "��ת"));
		index += 1;

		if(rxlen < index + 104){	// �������ݸ�ʽ-�ɰ汾 1 + 78 byte
			// ����������ʼ���
			u8Tmp = buf[index] * 10;
			dispIdx += sprintf(&disps->buf[dispIdx], "��Χ: �� %d~%d ��\n", u8Tmp, u8Tmp + 9);
			index += 1;
			// ����������ʼʱ��
			dispIdx += sprintf(&disps->buf[dispIdx], "ʱ��: %X%x%x%x %x:00:00\n"
				, buf[payloadIdx + 2], buf[payloadIdx + 3], buf[payloadIdx + 4], buf[payloadIdx + 5], buf[payloadIdx + 6]);
			index += 5;
			// �������ݷ�ʽ ��0-����, 1-����
			// ������������ ���������24�����������30��
			dispIdx += sprintf(&disps->buf[dispIdx], "��ʽ: ÿ%s����%d��\n", (buf[index] == 0x01 ? "��" : "��"), buf[index + 1]);
			index += 2;	
			// ��������ʱ����
			if(buf[index] == 0){
				dispIdx += sprintf(&disps->buf[dispIdx], "���: ÿ%s����1��\n", (buf[index - 2] == 0x01 ? "��" : "��"));
			}
			else{
				dispIdx += sprintf(&disps->buf[dispIdx], "���: %d%s����1��\n", buf[index], (buf[index - 2] == 0x01 ? "Сʱ" : "��"));
			}
			index += 1;
			// ������������ݣ�7*N �ֽ� ��6 byte ���� + 1 byte date.day��
			dispIdx += sprintf(&disps->buf[dispIdx], "��ȡ��10����������: \n");
			for(i = 0; i < 10; i++){
				u32Tmp = ((buf[index + 3] << 24) + (buf[index + 2] << 16) + (buf[index + 1] << 8) + buf[index]);
				index += 4;
				u16Tmp = ((buf[index + 1] << 8) + buf[index]);
				index += 2;
				dispIdx += sprintf(&disps->buf[dispIdx], "%d, %x/%x: %d.%03d\n", i, buf[payloadIdx + 4], buf[index], u32Tmp, u16Tmp);
				index +=1;
			}
		}
		else{		// �������ݸ�ʽ-�°汾	1 + 104 byte
			// ����������ʼ���
			dispIdx += sprintf(&disps->buf[dispIdx], "��Χ: ������%d������\n", buf[index] + 1);
			index += 1;
			// ʱ����Ϣ
			dispIdx += sprintf(&disps->buf[dispIdx], "ʱ��: %02X-%02X %02X:%02X\n",
				buf[index], buf[index + 1], buf[index + 2], buf[index + 3]);
			index += 4;
			// �ۼ�����
			u32Tmp = ((buf[index + 3] << 24) + (buf[index + 2] << 16) + (buf[index + 1] << 8) + buf[index]);
			index += 4;
			u16Tmp = ((buf[index + 1] << 8) + buf[index]);
			index += 2;
			dispIdx += sprintf(&disps->buf[dispIdx], "�ۼ�����: %d.%03d\n", u32Tmp, u16Tmp);
			// 0:00 ~ 23:30 ����
			u8Tmp = 0;
			u16Tmp = 0x00;
			for(i = 0; i < 47; i++){
				dispIdx += sprintf(&disps->buf[dispIdx], "%d:%02X~", u8Tmp, u16Tmp);
				u16Tmp += 0x30;
				if(u16Tmp == 0x60){
					u16Tmp = 0x00;
					u8Tmp += 1;
				}
				dispIdx += sprintf(&disps->buf[dispIdx], "%d:%02X����:%d\n", u8Tmp, u16Tmp, (buf[index] + buf[index + 1]*256));
				index += 2;
			}
		}

		//�澯״̬��1
		ptr = Water6009_GetStrAlarmStatus1(buf[index]);
		dispIdx += sprintf(&disps->buf[dispIdx], "�澯: %s ", ptr);
		index += 1;
		//�澯״̬��2
		ptr = Water6009_GetStrAlarmStatus2(buf[index]);
		dispIdx += sprintf(&disps->buf[dispIdx], "%s\n", ptr);
		index += 1;
		//����״̬ 
		ptr = Water6009_GetStrValveStatus(buf[index]);
		dispIdx += sprintf(&disps->buf[dispIdx], "����: %s  ", ptr);
		index += 1;
		//��ص�ѹ
		dispIdx += sprintf(&disps->buf[dispIdx], "���: %c.%c\n", (buf[index] / 10) + '0', (buf[index] % 10) + '0');
		index += 1;
		//�����¶�
		ptr = ((buf[index] & 0x80) > 0 ? "-" : "");
		dispIdx += sprintf(&disps->buf[dispIdx], "�¶�: %s%d  ", ptr, (buf[index] & 0x7F));
        index += 1;
		//SNR ������
		dispIdx += sprintf(&disps->buf[dispIdx], "SNR : %d\n", buf[index]);
		index += 1;
		//tx|rx�ŵ�
		dispIdx += sprintf(&disps->buf[dispIdx], "�ŵ�: Tx-%d, Rx-%d\n", (buf[index] & 0x0F), (buf[index] >> 4));
		index += 1;
		//Э��汾
		dispIdx += sprintf(&disps->buf[dispIdx], "�汾: %d\n", buf[index]);
		index += 1;
		break;

	//---------------------------------------		���ط���	---------------------
	case WaterCmd_OpenValve:		// ����
	case WaterCmd_OpenValveForce:	// ǿ�ƿ���
	case WaterCmd_CloseValve:		// �ط�
	case WaterCmd_CloseValveForce:	// ǿ�ƹط�
		if(rxlen < index + 3 && cmd != 0x03){
			break;
		}
		ret = true;
		// ����״̬
		ptr = Water6009_GetStrErrorMsg(buf[index]);
		dispIdx += sprintf(&disps->buf[dispIdx], "���: %s\n", ptr);
		if(buf[index] == 0xAB){
			ptr = Water6009_GetStrValveCtrlFailed(((buf[index + 1] << 8) + buf[index + 2]));
			dispIdx += sprintf(&disps->buf[dispIdx], "ԭ��: %s\n", ptr);
		}
		index += 3;
		break;

	//----------------------------------------		��ȡ���������Ϣ		-------------
	case WaterCmd_ReadMeterCfgInfo:	// ��ȡ���������Ϣ

		if(rxlen < index + 124 && cmd != 0x04){
			break;
		}
		ret = true;
		index += 84;
		memcpy(VerInfo, &buf[index], 40);
		memcpy(&TmpBuf[1020], &buf[index], 40);
		TmpBuf[1060] = 0x00;
		dispIdx += sprintf(&disps->buf[dispIdx], "�汾: %s\n", &TmpBuf[1020]);
		index += 40;
		break;

	//---------------------------------------		���쳣����		---------------------
	case WaterCmd_ClearException:	// ���쳣���� 
		if(rxlen < index + 1 && cmd != 0x05){
			break;
		}
		ret = true;
		// ����״̬
		ptr = Water6009_GetStrErrorMsg(buf[index]);
		dispIdx += sprintf(&disps->buf[dispIdx], "���: %s\n", ptr);
		index += 1;
		break;

	//---------------------------------------		��������	---------------------
	case WaterCmd_RebootDevice:	// �������
		if(rxlen < index + 2 && cmd != 0x07){
			break;
		}
		ret = true;
		index += 1;
		// ����״̬
		ptr = Water6009_GetStrErrorMsg(buf[index]);
		dispIdx += sprintf(&disps->buf[dispIdx], "���: %s\n", ptr);
		index += 1;
		break;

	case WaterCmd_ReadTemperature:	// �����¶�
		if(rxlen < index + 1 && cmd != 0x07){
			break;
		}
		ret = true;
		index += 1;
		//�����¶�
		ptr = ((buf[index] & 0x80) > 0 ? "-" : "");
		dispIdx += sprintf(&disps->buf[dispIdx], "�¶�: %s%d\n", ptr, (buf[index] & 0x7F));
        index += 1;
		break;

	case WaterCmd_ReadVoltage:	// �����ѹ
		if(rxlen < index + 1 && cmd != 0x07){
			break;
		}
		ret = true;
		index += 1;
		//��ص�ѹ
		dispIdx += sprintf(&disps->buf[dispIdx], "��ص�ѹ: %c.%c\n", (buf[index] / 10) + '0', (buf[index] % 10) + '0');
		index += 1;
		break;

	case WaterCmd_ClearPrepaidRefVal:	// ��Ԥ�ɲο���
		if(rxlen < index + 1 && cmd != 0x07){
			break;
		}
		ret = true;
		index += 1;
		// ����״̬
		ptr = Water6009_GetStrErrorMsg(buf[index]);
		dispIdx += sprintf(&disps->buf[dispIdx], "���: %s\n", ptr);
		index += 1;
		break;

	case WaterCmd_SetOverCurrentTimeout:	// ���ù�����ʱ
		if(rxlen < index + 3 && cmd != 0x07){
			break;
		}
		ret = true;
		index += 1;
		// ����״̬
		if(buf[index] != Args.buf[2] || buf[index + 1] != Args.buf[3] || buf[index + 2] != Args.buf[4]){
			ptr = Water6009_GetStrErrorMsg(buf[index]);
			dispIdx += sprintf(&disps->buf[dispIdx], "���: %s\n", ptr);
			index += 1;
		}
		else{
			dispIdx += sprintf(&disps->buf[dispIdx], "���: �����ɹ�\n");
			// ��������
			dispIdx += sprintf(&disps->buf[dispIdx], "��������: %d mA\n", buf[index]);
			index += 1;
			// ��ʱʱ��
			dispIdx += sprintf(&disps->buf[dispIdx], "��ʱʱ��: %d ms\n", (buf[index] + buf[index + 1] * 256));
			index += 2;
		}
		break;

	case WaterCmd_ReadOperatorNumber:	// ����Ӫ�̱��
		if(rxlen < index + 4 && cmd != 0x07){
			break;
		}
		ret = true;
		index += 1;
		// ��Ӫ�̱��
		dispIdx += sprintf(&disps->buf[dispIdx], "��Ӫ�̱��: %2X%2X%2X%2X\n", 
			buf[index], buf[index + 1], buf[index + 2], buf[index + 3]);
		index += 4;
		break;

	case WaterCmd_ReadReportRoute:	// ���ϱ�·��
		if(rxlen < index + 63 && cmd != 0x07){
			break;
		}
		ret = true;
		index += 1;

		// ·��1���� 6*n
		u8Tmp = buf[index] / 6;
		dispIdx += sprintf(&disps->buf[dispIdx], "·��1����: %d\n", u8Tmp);
		index += 1;
		// ·��1��ַ�б�
		for(i = 0; i < u8Tmp; i++){
			GetStringHexFromBytes(&TmpBuf[0], buf, index, 6, 0, false);
			TmpBuf[12] = 0x00;
			dispIdx += sprintf(&disps->buf[dispIdx], "%d : %s\n", i + 1, &TmpBuf[0]);
			index += 6;
		}
		index += (30 - u8Tmp * 6);

		// ·��2���� 6*n
		u8Tmp = buf[index] / 6;
		dispIdx += sprintf(&disps->buf[dispIdx], "  \n·��2����: %d\n", u8Tmp);
		index += 1;
		// ·��2��ַ�б�
		for(i = 0; i < u8Tmp; i++){
			GetStringHexFromBytes(&TmpBuf[0], buf, index, 6, 0, false);
			TmpBuf[12] = 0x00;
			dispIdx += sprintf(&disps->buf[dispIdx], "%d : %s\n", i + 1, &TmpBuf[0]);
			index += 6;
		}
		index += (30 - u8Tmp * 6);

		break;

	case WaterCmd_SetMeterNumber:	// ���ñ��
		if(rxlen < index + 1 && cmd != 0x07){
			break;
		}
		ret = true;
		// ����״̬
		ptr = Water6009_GetStrErrorMsg(buf[index]);
		dispIdx += sprintf(&disps->buf[dispIdx], "���: %s\n", ptr);
		index += 1;
		break;

	//--------------------------------------		Ԥ���������ο�����-��ȡ/����	---------------------
	case WaterCmd_ReadPrepaidRefVal:	// ��Ԥ�ɲο�����
		if(rxlen < index + 12 && cmd != 0x15){
			break;
		}
		ret = true;
		// Ԥ������
		u32Tmp = ((buf[index + 3] << 24) + (buf[index + 2] << 16) + (buf[index + 1] << 8) + buf[index]);
		index += 4;
		u16Tmp = ((buf[index + 1] << 8) + buf[index]);
		index += 2;
		dispIdx += sprintf(&disps->buf[dispIdx], "Ԥ������: %d.%03d\n", u32Tmp, u16Tmp);
		// �ο�����
		u32Tmp = ((buf[index + 3] << 24) + (buf[index + 2] << 16) + (buf[index + 1] << 8) + buf[index]);
		index += 4;
		u16Tmp = ((buf[index + 1] << 8) + buf[index]);
		index += 2;
		dispIdx += sprintf(&disps->buf[dispIdx], "�ο�����: %d.%03d\n", u32Tmp, u16Tmp);
		break;

	case WaterCmd_SetPrepaidRefVal:		// ��Ԥ�ɲο�����
		if(rxlen < index + 2 && cmd != 0x16){
			break;
		}
		ret = true;
		// ����״̬
		ptr = (buf[index] == 0xAA ? "�����ɹ�" : "���ݷǷ�");
		dispIdx += sprintf(&disps->buf[dispIdx], "���: %s\n", ptr);
		index += 1;
		// ���ݷǷ�ԭ��
		if(buf[index - 1] == 0xAE){
			if(buf[index] & 0x01 > 0){
				dispIdx += sprintf(&disps->buf[dispIdx], "-->�ο���ʼ�������Ϸ�\n");
			}
			if(buf[index] & 0x02 > 0){
				dispIdx += sprintf(&disps->buf[dispIdx], "-->���õ�Ԥ������δ�ﵽ��������\n");
			}
			index += 1;
		}
		break;

	case WaterCmd_ReadAlarmLimitOverdraft:		// �������ط���ֵ
		if(rxlen < index + 1 && cmd != 0x17){
			break;
		}
		ret = true;
		// ����״̬
		ptr = (buf[index] == 0xAB ? "����ʧ��" : "�����ɹ�");
		dispIdx += sprintf(&disps->buf[dispIdx], "���: %s\n", ptr);
		index += 1;
		// �����ɹ�ʱ���
		if(buf[index - 1] == 0xAA){
			dispIdx += sprintf(&disps->buf[dispIdx], "������ֵ: %d\n", buf[index]);
			index += 1;
			u16Tmp = ((uint16)(buf[index + 1] << 8) + buf[index]);
			dispIdx += sprintf(&disps->buf[dispIdx], "�ط���ֵ: %s%d\n", 
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
		ret = true;
		// ����״̬
		ptr = (buf[index] == 0xAA ? "�����ɹ�" : "����ʧ��");
		dispIdx += sprintf(&disps->buf[dispIdx], "���: %s\n", ptr);
		index += 1;
		// �°����+ 2 byte
		break;

	//--------------------------------------		��������	---------------------
	case WaterCmd_SetBaseValPulseRatio:	// ����������ϵ��
		if(rxlen < index + 12 && cmd != 0x15){
			break;
		}
		ret = true;

		break;

	case WaterCmd_ClearReverseMeasureData:	// �����ת��������
		if(rxlen < index + 12 && cmd != 0x15){
			break;
		}
		ret = true;
		
		break;

	case WaterCmd_ReadFuncEnableState:	// ��ȡ����ʹ��״̬
		if(rxlen < index + 12 && cmd != 0x15){
			break;
		}
		ret = true;
		
		break;

	case WaterCmd_SetTimedUpload:		// ���ö�ʱ�ϴ�
		if(rxlen < index + 12 && cmd != 0x15){
			break;
		}
		ret = true;
		
		break;

	case WaterCmd_SetFixedValUpload:	// ���ö����ϴ�
		if(rxlen < index + 12 && cmd != 0x15){
			break;
		}
		ret = true;
		
		break;

	case WaterCmd_SetTimedAndFixedValUpload:	// ���ö�ʱ�����ϴ�
		if(rxlen < index + 12 && cmd != 0x15){
			break;
		}
		ret = true;
		
		break;

	case WaterCmd_ReadMeterTime:	// �����ʱ��
		if(rxlen < index + 7 && cmd != 0x13){
			break;
		}
		ret = true;
		dispIdx += sprintf(&disps->buf[dispIdx], "���ʱ��: \n %02X%02X-%02X-%02X %02X:%02X:%02X\n", 
			buf[index], buf[index + 1], buf[index + 2], buf[index + 3]
			, buf[index + 4], buf[index + 5], buf[index + 6]);
		index += 7;
		break;

	case WaterCmd_SetMeterTime:		// У���ʱ��
		if(rxlen < index + 1 && cmd != 0x14){
			break;
		}
		ret = true;
		// ����״̬
		ptr = (buf[index] == 0xAA ? "�����ɹ�" : "����ʧ��");
		dispIdx += sprintf(&disps->buf[dispIdx], "���: %s\n", ptr);
		index += 1;
		break;

	//--------------------------------------		��������		---------------------
	case WaterCmd_ReadRxTxMgnDistbCnt:		// ����/��/���Ŵ���
		if(rxlen < index + 12 && cmd != 0x15){
			break;
		}
		ret = true;

		break;

	case WaterCmd_ReadRxdAndTxdChanel:	// ��ȡRXD��TXD�ŵ�
		if(rxlen < index + 12 && cmd != 0x15){
			break;
		}
		ret = true;
		
		break;

	case WaterCmd_SetRxdAndTxdChanel:	// ����RXD��TXD�ŵ�
		if(rxlen < index + 12 && cmd != 0x15){
			break;
		}
		ret = true;
		
		break;

	case WaterCmd_SetOperatorNumber:		// ������Ӫ�̱��
		if(rxlen < index + 12 && cmd != 0x15){
			break;
		}
		ret = true;
		
		break;

	case WaterCmd_SetDefinedRoute:	// ·���·�
		if(rxlen < index + 12 && cmd != 0x15){
			break;
		}
		ret = true;
		
		break;

	//--------------------------------------		��������		---------------------
	case WaterCmd_SingleUpgrade:		// ��������
		if(rxlen < index + 12 && cmd != 0x15){
			break;
		}
		ret = true;

		break;

	case WaterCmd_QueryUpgrade:			// ��ѯ����
		if(rxlen < index + 12 && cmd != 0x15){
			break;
		}
		ret = true;
		
		break;

	case WaterCmd_BroadcastUpgrade:		// �㲥����
		if(rxlen < index + 12 && cmd != 0x15){
			break;
		}
		ret = true;
		
		break;

	case WaterCmd_DocAdd:			// ��ӵ���
		if(rxlen < index + 12 && cmd != 0x15){
			break;
		}
		ret = true;
		
		break;

	case WaterCmd_DocDelete:		// ɾ������
		if(rxlen < index + 12 && cmd != 0x15){
			break;
		}
		ret = true;
		
		break;

	case WaterCmd_DocQuery:			// ��ѯ����
		if(rxlen < index + 12 && cmd != 0x15){
			break;
		}
		ret = true;
		
		break;

	case WaterCmd_UpgradeStatistics:	// ����ͳ��
		if(rxlen < index + 12 && cmd != 0x15){
			break;
		}
		ret = true;
		
		break;
		
	default:
		ret = true;
		dispIdx += sprintf(&disps->buf[dispIdx], "������[%02X]��δ����\n", cmd);
		break;
	}


	if(index == startIdx + length - 4)
	{
		//����/���� �ź�ǿ��
		dispIdx += sprintf(&disps->buf[dispIdx], "                    \n");
		dispIdx += sprintf(&disps->buf[dispIdx], "����: %d  ����: %d\n", buf[index], buf[index + 1]);
		index += 2;
	}

	disps->buf[dispIdx] = '\0';
	
	
	return ret;
}

#endif
