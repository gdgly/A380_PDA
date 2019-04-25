#ifndef WaterMeter_H
#define WaterMeter_H


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
	WaterCmd_SetDefinedRoute,
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
	CenterCmd_ClearException,

}CenterCmdDef;


//----------------------------------------------------------------
/*
* ��������PackWaterRequestFrame
* ��  �������ˮ����������֡
* ��  ����buf - ���ݻ�����ʼ��ַ
		  dstAddr - Ŀ�ĵ�ַ
		  cmdId - ������
		  retryCnt - �ط�������0 - ��1�η��ͣ����� - ��n���ط�
* ����ֵ��uint8 ֡�ܳ���
*/
uint8 PackWaterRequestFrame(uint8 * buf, const uint8 * dstAddr, uint8 cmdId, uint8 *args[], uint8 retryCnt)
{
	static uint8 macFsn = 0xFF, nwkFsn = 0xFF, apsFsn = 0xFF, index = 0;
	uint8 macCmdStart, nwkCmdStart, apsCmdStart, dltStart, i, relayCnt;
	uint16 crc16;
	uint8 crc8;

	if(retryCnt > 0 && index > 0){
		return index;
	}

	// mac layer
	index = 0;
	buf[index++] = 0x00;	// length		- skip
	buf[index++] = 0x00;	// channel
	buf[index++] = 0x01;	// ver
	buf[index++] = 0x00;	// xor(0~2) check	- skip
	buf[index++] = 0x41;	// mac ctrl 
	buf[index++] = 0xCD;
	macFsn++;
	buf[index++] = macFsn;	// mac fsn
	buf[index++] = 0xFF;	// panid
	buf[index++] = 0xFF;
	memcpy(&buf[index], dstAddr, 6);
	index += 6; 			// mac dst addr - skip
	memcpy(&buf[index], LocalAddr, 6);
	index += 6; 			// mac src addr
	macCmdStart = index;

	// nwk layer
	buf[index++] = 0x3C;	// nwk ctrl
	memcpy(&buf[index], dstAddr, 6);
	index += 6; 			// nwk dst addr - skip
	memcpy(&buf[index], LocalAddr, 6);
	index += 6; 			// nwk src addr
	nwkFsn++;
	buf[index++] = (nwkFsn << 4) | 0x01;	// nwk fsn|radius - fixed
	
	// ����·�ɣ�����м̵�ַ
	if(cmdId == PowerCmd_ReadMeter_645_97
		|| cmdId == PowerCmd_ReadMeter_645_07
		|| cmdId == PowerCmd_ReadMeter_698){

		// args[0] - ����ַ
		// args[1] - �м�����
		// args[2-n] - ��ַ�б�
		relayCnt = *args[1];
		if(relayCnt > 0){	
			// �޸�mac��Ŀ�ĵ�ַ
			memcpy(&buf[9], args[2], 6);
			// �޸�����뾶
			buf[index - 1] = (nwkFsn << 4) | ((relayCnt & 0x07) + 1);
			// �м�����bit4-0 , �м����� bit9-5 , �м̵�ַģʽ bit23-10, 2λ * 7 ��10 - �̵�ַ�� 11 - ����ַ
			buf[index++] = ((relayCnt & 0x07) << 5) + (relayCnt & 0x1F);
			buf[index++] = (relayCnt >> 3)+ 0xFC;
			buf[index++] = 0xFF;
			// �м��б�
			for(i = 0; i < relayCnt; i++){
				memcpy(&buf[index], args[2 + i], 6);
				index += 6;
			}
		}
	}
	nwkCmdStart = index;

	// aps layer
	buf[index++] = 0x09;	// aps ctrl 
	buf[index++] = apsFsn;	// aps Fsn
	buf[index++] = 0x05;	// expand : "SR19"
	buf[index++] = 0x53;
	buf[index++] = 0x53;
	buf[index++] = 0x52;
	buf[index++] = 0x31;
	buf[index++] = 0x39;
	apsCmdStart = index;
	
	// cmd case
	switch(cmdId){

	//-------------------------------------------  ����		-------------
	
	//-------------------------------------------  ������ȡ	 -------------
	
	case PowerCmd_ReadNodeInfo:		/*	������/��� ����  */
		apsFsn++;
		buf[index++] = 0x04;
		break;
	case PowerCmd_ReadNwkStatus:
		apsFsn++;
		buf[index++] = 0x93;
		break;
	case PowerCmd_ReadSendPower:
		apsFsn++;
		buf[index++] = 0x94;
		break;
	case PowerCmd_ReadVerInfo:
		apsFsn++;
		buf[index++] = 0x95;
		break;
	case PowerCmd_ReadNeighbor:			/*	��� ����  */
		index = nwkCmdStart;
		buf[macCmdStart] = 0x3D;	// nwk ctrl
		buf[index++] = 0x10;
		break;
	case PowerCmd_ReadSubNodeRoute:		/*	������ ����  */
		apsFsn++;
		buf[index++] = 0x92;
		memcpy(&buf[index], args[0], 6);
		index += 6;
		break;


	//-------------------------------------------  �ڵ����		-------------
	
	case PowerCmd_SetSerialCom:			/*	������/��� ����  */
		apsFsn++;
		buf[index++] = 0x00;
		buf[index++] = *args[0];
		buf[index++] = *args[1];
		break;


	case PowerCmd_ParamsInit:			/*	������ ����  */
		apsFsn++;
		buf[index++] = 0x90;
		break;
	
	default:
		break;
	}

	// calc length / crc16
	buf[0] = index - 1;
	buf[3] = buf[0] ^ buf[1] ^ buf[2];
	crc16 = GetCrc16(buf, index, CRC16_Seed);
	buf[index++] = (uint8)(crc16 & 0xFF);
	buf[index++] = (uint8)(crc16 >> 8);
	
	return index;
}

/*
* ��������ExplainWaterResponseFrame
* ��  ��������ˮ��������Ӧ֡
* ��  ����buf - ���ջ�����ʼ��ַ
		  srcAddr - Դ��ַ
		  cmdId - ������
		  disp - ��������ʾ����
* ����ֵ��bool ���������fasle - ʧ�� �� true - �ɹ�
*/
bool ExplainWaterResponseFrame(uint8 * buf, uint16 rxlen, const uint8 * srcAddr, uint8 cmdId, ParamsBuf * disps)
{
	uint8 index = 0, len = 0, radius;
	uint8 nwkCtrl, apsCtrl;
	bool ret = false;
	uint16 crc16;

	// ����������в���
	while(1){

		if(rxlen < index + 35){
			disps->cnt = 1;
			sprintf(&disps->buf[0], "δӦ��");
			disps->items[0] = &disps->buf[0];
			return false;
		}

		if(buf[index] == 0x55 && buf[index + 1] == 0xAA){
			index += 3;
		}else{
			index++;
			continue;
		}
		
		// length check
		if(index + buf[index] + 3 > rxlen){
			index += buf[index] + 3;
			continue;
		}	

		// dstaddr check
		if(strncmp(&buf[index + 9], LocalAddr, 6) != 0){
			index += buf[index] + 3;
			continue;
		}

		// crc16 check
		len = buf[index];
		crc16 = GetCrc16(&buf[index], len + 1, CRC16_Seed);
		if(crc16 !=  (uint16)((buf[index + len + 2] << 8) + buf[index + len + 1])){
			disps->cnt = 1;
			sprintf(&disps->buf[0], "CRC����");
			disps->items[0] = &disps->buf[0];
			return false;
		}

		break;
	}

	// mac layer
	index += 21;
	// nwk layer
	nwkCtrl = buf[index++];			// nwk ctrl
	index += 12;
	radius = buf[index++] & 0x0F;	// nwk radius
	index += (radius > 1 ? (radius - 1) * 6 + 3 : 0);

	// aps layer
	if(nwkCtrl == 0x3C || nwkCtrl == 0xBC){
		apsCtrl = buf[index++];	// aps ctrl
		index++;				// aps Fsn
		if((apsCtrl & 0x08) > 0){
			index += buf[index] + 1;
		}
	}
	
	// cmd case
	switch(cmdId){

	//-------------------------------------------  ����		-------------
	case PowerCmd_ReadMeter_698:
		index++;
		if(buf[index] == 0x68 
			&& buf[index + 1] == 0x34 && buf[index + 2] == 0x00	// length
			&& buf[index + 3] == 0xC3 		// ctrl
			&& buf[index + 4] == 0x05
			){
			// �����ɹ����ݲ�����
			ret = true;

			disps->cnt = 1;
			sprintf(&disps->buf[0], "�ݲ�����");
			disps->items[0] = &disps->buf[0];
		}
		break;
	//-------------------------------------------  ������ȡ	 -------------

	default:
		break;
	}
	
	return ret;
}






#endif
