#ifndef WaterMeter_H
#define WaterMeter_H

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
	CenterCmd_ReadCenterNo		= 0x11,
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
	CenterCmd_ReadDocCount		= 0x21,
	CenterCmd_ReadDocInfo,
	CenterCmd_AddDocInfo,
	CenterCmd_DeleteDocInfo,
	CenterCmd_ModifyDocInfo,

	/*
	·�����ã�	
	1	���Զ���·��
	2	���Զ���·��
	*/
	CenterCmd_ReadDefinedRoute	= 0x31,
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
	CenterCmd_ReadRealTimeData	= 0x41,
	CenterCmd_ReadFixedTimeData,
	CenterCmd_ReadFrozenData,
	CenterCmd_OpenValve,
	CenterCmd_CloseValve,
	CenterCmd_ReadEnableState,
	CenterCmd_ClearException

}CenterCmdDef;



//---------------------------------------		6009 ��������	-------------------------------------

/*
* ��  ������ȡ6009ˮ�����������
* ��  ����typeId	- ����ID
* ����ֵ��char *	- ��������ַ���
*/
char * Water6009_GetStrValueType(uint8 typeId)
{
	char * str = NULL;
	
	switch(typeId){
	case 0:   
		str = "ʵʱ";
		break;
	case 1:
		str = "�����ϴ�";
		break;
	case 2:
		str = "��ʱ�ϴ�";
		break;
	case 3:
		str = "�����ϴ�";
		break;
	case 4:
		str = "����";
		break;

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
		case 0x01:   
			str = "�ɻɹܹ���";
			break;
		case 0x02:
			str = "����λ����";
			break;
		case 0x04:
			str = "�������߶Ͽ�";
			break;
		case 0x08:
			str = "���Ƿѹ";
			break;
		case 0x10:
			str = "����һ���ܻ�";
			break;
		case 0x20:
			str = "�Ÿ��ű�־";
			break;
		case 0x40:
			str = "���������ܻ�";
			break;
		case 0x80:
			str = "������ǿ�����";
			break;
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
		case 0x01:   
			str = "ˮ��ת";
			break;
		case 0x02:
			str = "ˮ����ж";
			break;
		case 0x04:
			str = "ˮ����ֱ��װ";
			break;
		case 0x08:
			str = "EEPROM�쳣";
			break;
		case 0x10:
			str = "ú��й©";
			break;
		case 0x20:
			str = "Ƿ�ѱ�־";
			break;
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
	case 0:   
		str = "����";
		break;
	case 1:
		str = "��";
		break;
	case 2:
		str = "��";
		break;
		
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
	uint8 i, crc8;

	if(retryCnt > 0 && index > 0){
		return index;
	}

	index = 0;
	buf[index++] = 0xD3;		// ֡ͷͬ���룺 �̶�Ϊ D3 91
	buf[index++] = 0x91;
	buf[index++] = 0x00;		// ���ȣ� ���ı�ʶ --> ������16
	buf[index++] = 0x00;	
	buf[index++] = 0x10;		// ���ı�־ bit7 0/1 - ����/���У� bit6 0/1 - ����/Ӧ�� bit4 �̶�Ϊ1
	buf[index++] = Fsn++;	// ����ţ� mac fsn �������ۼ�
	buf[index++] = *args->items[0];		// ������
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

		

	if(cmdId < 0x40 || cmdId == 0x70 || cmdId == 0x74){
		buf[index++] = 0x1E;	// ���Գ��ȱ�ʶ
		buf[index++] = 0x03;	// ���APPʱ �����ŵ�
		buf[index++] = 0x19;	// ���APPʱ �����ŵ�
	}else if(cmdId > 0x70 && cmdId < 0x74){
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
	uint8 crc8, addrsCnt, cmd;
	uint16 index = 0, startIdx, length, u16Tmp;
	uint8 *ptr, dispIdx;
	uint32 u32Tmp;

	// ����������в���
	while(1){

		if(rxlen < index + ackLen){
			disps->itemCnt = 1;
			disps->items[0] = &disps->buf[0];
            sprintf(disps->items[0], "δӦ��");
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
			disps->itemCnt = 1;
			disps->items[0] = &disps->buf[0];
            sprintf(disps->items[0], "CRC����");
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

	// ��ʾ���
	dispIdx = 0;
	dispIdx += sprintf(&disps->buf[dispIdx], "���: %s", StrDstAddr);

	// ���������
	switch(cmdId){

	//-------------------------------------------  ����		-------------
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
		if((buf[index] & 0x80) > 0){
			dispIdx += sprintf(&disps->buf[dispIdx], "�¶�: -%d  ", (buf[index] & 0x7F));
		}else{
			dispIdx += sprintf(&disps->buf[dispIdx], "�¶�: %d  ", (buf[index] & 0x7F));
		}
        index += 1;
		//SNR ������
		dispIdx += sprintf(&disps->buf[dispIdx], "SNR: %d\n", buf[index]);
		index += 1;
		//tx|rx�ŵ���Э��汾 ����
		index += 2;
		break;

	case WaterCmd_ReadFrozenData:	// ��ȡ��������
		if(rxlen < index + 21 && cmd != 0x02){
			break;
		}
		ret = true;

		break;

	case WaterCmd_OpenValve:		// ����
	case WaterCmd_OpenValveForce:	// ǿ�ƿ���
	case WaterCmd_CloseValve:		// �ط�
	case WaterCmd_CloseValveForce:	// ǿ�ƹط�
		if(rxlen < index + 3 && cmd != 0x03){
			break;
		}
		ret = true;
		// ����״̬
		if(buf[index] == 0xAD){
			dispIdx += sprintf(&disps->buf[dispIdx], "ִ�гɹ�\n");
		}
		else{
			dispIdx += sprintf(&disps->buf[dispIdx], "ִ��ʧ��\n");
			ptr = Water6009_GetStrErrorMsg(buf[index]);
			dispIdx += sprintf(&disps->buf[dispIdx], "ԭ��: %s \n", ptr);
		}
		index += 1;
		break;

	case WaterCmd_ClearException:	// ���쳣���� 
		if(rxlen < index + 1 && cmd != 0x05){
			break;
		}
		ret = true;
		// ����״̬
		if(buf[index] == 0xAA){
			dispIdx += sprintf(&disps->buf[dispIdx], "ִ�гɹ�\n");
		}
		else{
			dispIdx += sprintf(&disps->buf[dispIdx], "ִ��ʧ��\n");
			ptr = Water6009_GetStrErrorMsg(buf[index]);
			dispIdx += sprintf(&disps->buf[dispIdx], "ԭ��: %s \n", ptr);
		}
		index += 1;
		break;

	//-------------------------------------------------		��������	---------------------
	case WaterCmd_RebootDevice:	// �������
		if(rxlen < index + 21 && cmd != 0x07){
			break;
		}
		ret = true;
		// ����״̬
		if(buf[index] == 0xAA){
			dispIdx += sprintf(&disps->buf[dispIdx], "ִ�гɹ�\n");
		}
		else{
			dispIdx += sprintf(&disps->buf[dispIdx], "ִ��ʧ��\n");
			ptr = Water6009_GetStrErrorMsg(buf[index]);
			dispIdx += sprintf(&disps->buf[dispIdx], "ԭ��: %s \n", ptr);
		}
		index += 1;
		break;

	case WaterCmd_ReadTemperature:	// �����¶�
		if(rxlen < index + 21 && cmd != 0x07){
			break;
		}
		ret = true;
		// ����״̬
		if(buf[index] == 0xAA){
			dispIdx += sprintf(&disps->buf[dispIdx], "ִ�гɹ�\n");
		}
		else{
			dispIdx += sprintf(&disps->buf[dispIdx], "ִ��ʧ��\n");
			ptr = Water6009_GetStrErrorMsg(buf[index]);
			dispIdx += sprintf(&disps->buf[dispIdx], "ԭ��: %s \n", ptr);
		}
		index += 1;
		break;
		
	default:
		break;
	}


	if(index == startIdx + length - 4)
	{
		//����/���� �ź�ǿ��
		dispIdx += sprintf(&disps->buf[dispIdx], "����: %d  ����: %d\n", buf[index], buf[index + 1]);
		index += 2;
		disps->items[0] = &disps->buf[0];
		disps->itemCnt = 1;
	}
	
	return ret;
}

#endif
