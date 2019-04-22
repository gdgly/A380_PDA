#include <HJLIB.H>
#include "string.h"
//#include "dbf.h"
#include "stdio.h"
#include "main.h"

// --------------------------------  ȫ�ֱ���  -----------------------------------------
char Screenbuff[160*(160/3+1)*2]; 
uint8 TmpBuf[1080];
uint8 TxBuf[1080];
uint8 RxBuf[1080];
uint32 RxLen, TxLen;
const char * ZeroAddr = "00000000000000";	// 0 ��ַ��14�ַ�
const uint8 LocalAddr[7] = {0x19, 0x20, 0x00, 0x00, 0x19, 0x20, 0x00};	// ��ַ 00201900002019��14�ַ�
uint8 DstAddr[7];
uint8 CurrCmd;
ParamsBuf Args;
ParamsBuf Disps;

//---------------------------------  ͨ�÷���  -------------------------------------
/*
* ��������IndexOf
* ��  �����������в��ң���ָ�����ҵ���ʼλ�úͷ�Χ
* ��  ����srcArray - Դ�����ַ�� srcLen - Դ���鳤��
		  dstBytes - Ŀ�������ַ�� dstLen - Ŀ�����鳤�� 
		  startIndex - Դ������ҵ���ʼλ�ã�offset - ���ҵķ�Χ
* ����ֵ��int ����-1: δ�ҵ��� ����/����0 : Ŀ��������Դ�����е���ʼ����
*/
int IndexOf(const uint8 * srcArray, int srcLen, const uint8 * dstBytes, int dstLen, int startIndex, int offset)
{
    int index = -1, i, j;
	
    if (dstBytes == NULL || dstLen == 0) return index;

    if(offset > (srcLen - startIndex))
    {
        offset = (srcLen - startIndex);
    }

    for (i = startIndex; i <= (startIndex + offset - dstLen); i++)
    {
        if (srcArray[i] == dstBytes[0])
        {
            for (j = 0; j < dstLen; j++)
            {
                if (srcArray[i + j] != dstBytes[j])
                {
                    break;
                }
            }

            if (j == dstLen)
            {
                index = i;
                break;
            }
        }
    }

    return index;
}

/*
* ��������ShowProgressBar
* ��  ������ʾ������
* ��  ����y - ������y���꣬����ʾ��(0,y)λ�ã��̶����Ϊ160���̶��߶�Ϊ16,��ɫ���
		  maxValue - ���������ֵ
		  currValue - ��������ǰֵ
* ����ֵ��int ����-1: δ�ҵ��� ����/����0 : Ŀ��������Դ�����е���ʼ����
*/
void ShowProgressBar(uint8 y, uint32 maxValue, uint32 currValue)
{
	uint32 width = (currValue >= maxValue? 160 : 160*currValue/maxValue);
	_GUIRectangleFill(0, y, width, y + 16, 1);
}

/*
* ��������StringPadLeft
* ��  �����ַ���������
* ��  ����srcStr - ԭ�ַ���
		  totalLen - ���ַ����ȣ�ԭ�ַ�+��������ַ�����ԭ�ַ�����>=�ܳ��ȣ���������䣩
		  padChar - �����ַ�
* ����ֵ��void
*/
void StringPadLeft(const char * srcStr, int totalLen, char padChar)
{
	uint32 srcStrLen, i = 0;
	char *pr, *pw;

	srcStrLen = strlen(srcStr);
	if(srcStrLen >= totalLen || padChar == 0x00){
		return;
	}

	pr = srcStr + srcStrLen - 1;
	pw = srcStr + totalLen - 1;
	*(pw + 1) = 0x00;
	
	for(i = 0; i < srcStrLen; i++){
		*pw = *pr;
		pr--;
		pw--;
	}

	while(i < totalLen){
		*pw = padChar;
		pw--;
		i++;
	}

}

/*
* ��������StringTrimStart
* ��  �����ַ���ͷ���ü�
* ��  ����srcStr - �ַ�����ʼ��ַ
		  trimChar - �ü����ַ�
* ����ֵ��int �ü�����ַ�������
*/
int StringTrimStart(const char * srcStr, char trimChar)
{
	uint32 srcStrLen, i = 0;
	char *pr, *pw;

	srcStrLen = strlen(srcStr);
	if(srcStrLen == 0 || trimChar == 0x00){
		return 0;
	}

	pr = srcStr;
	pw = srcStr;
	
	for(i = 0; i < srcStrLen; i++){
		if(*pr != trimChar){
			break;
		}
		pr++;
	}
	if(pr != srcStr){
		while(pr < srcStr + srcStrLen){
			*pw = *pr;
			pr++;
			pw++;
		}
		*(pw + 1) = 0x00;
	}

	return (srcStrLen - i);
}


/*
* ��������HexToChar
* ��  ����16������ת���ɶ�Ӧ���ַ�
*/
char HexToChar(uint8 b)
{
	char decHex[16] = {'0', '1', '2', '3','4', '5', '6', '7','8', '9', 'A', 'B','C', 'D', 'E', 'F'};
	
	if(b < 16){
		return decHex[b];
	}else{
		return '\0';
	}
}
/*
* ��������CharToHex
* ��  �����ַ�ת���ɶ�Ӧ��16������
*/
uint8 CharToHex(char c)
{
	uint8 hex;
	
	if(c >= '0' && c <= '9'){
		hex = c - '0';
	}else if(c >= 'A' && c <= 'F'){
		hex = c - 'A' + 10;
	}else if(c >= 'a' && c <= 'f'){
		hex = c - 'a' + 10;
	}else{
		hex = 0xFF;
	}

	return hex;
}

/*
* ��������GetStringHexFromBytes
* ��  �������ֽ�����ת����16�����ַ���
* ��  ����strHex - Ŀ���ַ�����������ַ
		  bytes - Դ�ֽ�����
		  iStart - ��������Ҫת������ʼ����
		  iLength - ��Ҫת���ĳ���
		  separate - �ַ�����Hex�ֽ�֮��ļ������0 - �޼������ �����ַ� - ��ո�򶺺�
		  reverse - �Ƿ���Ҫ����false - ������ true - ����
* ����ֵ��int - ת������ַ�����0 - ת��ʧ��
*/
int GetStringHexFromBytes(char * strHex, uint8 bytes[], int iStart, int iLength, char separate, bool reverse)
{
	uint8 aByte;
	int iLoop, index = 0;
   
	if(iLength == 0 || iStart < 0){
		strHex[index] = 0;
		return 0;
	}
	
	for (iLoop = 0; iLoop < iLength; iLoop++)
	{
		if (reverse){
			aByte = bytes[iStart + iLength - 1 - iLoop];
		}
		else{
			aByte = bytes[iStart + iLoop];
		}
		strHex[index++] = HexToChar(aByte >> 4);
		strHex[index++] = HexToChar(aByte & 0x0F);
		if(separate != 0){
			strHex[index++] = separate;
		}
	}
	strHex[index++] = 0;

	return index;
}
/*
* ��������GetBytesFromStringHex
* ��  ������16�����ַ���ת�����ֽ�����
* ��  ����bytes - Ŀ���ֽ�����
		  iStart - �����б������ʼ����
		  strHex - Դ�ַ�����������ַ
		  separate - �ַ�����Hex�ֽ�֮��ļ������0 - �޼������ �����ַ� - ��ո�򶺺�
		  reverse - �Ƿ���Ҫ����false - ������ true - ����
* ����ֵ��int - ת������ֽ�����0 - ת��ʧ��
*/
int GetBytesFromStringHex(uint8 bytes[], int iStart, const char * strHex, char separate, bool reverse)
{
	int iLoop = 0, index = 0;
	int bytesLen, strHexLen;
	uint8 aByte;
  
	strHexLen = strlen(strHex);
	if(separate != 0){
		bytesLen = (strHexLen + 1) / 3;
	}else{
		bytesLen = (strHexLen + 1) / 2;
	}

	if(bytesLen == 0 || iStart < 0){
		return 0;
	}

	while (iLoop < strHexLen - 1)
	{
		aByte = (CharToHex(strHex[iLoop]) << 4) | (CharToHex(strHex[iLoop + 1]) & 0x0F);
		iLoop += 2;

		if (reverse){
			bytes[iStart + bytesLen - 1 - index] = aByte;
		}
		else{
			bytes[iStart + index] = aByte;
		}
	
		if(separate != 0){
			iLoop++;
		}
		index++;
	}

	return index;
}

/*
* ��������GetCrc16
* ��  ��������CRC16
* ��  ����Buf - ���ݻ�����ʼ��ַ
		  Len - ������ܳ���
		  Seed - �����/ˮ���̶�ʹ�� 0x8408
* ����ֵ��uint16 CRC16ֵ
*/
uint16 GetCrc16(uint8 *Buf, uint16 Len, uint16 Seed)
{
    uint16 crc = 0xFFFF;
    uint8 i;

	while (Len--){
        crc ^= * Buf++;
        for(i = 0; i < 8; i++){
            if (crc & 0x0001){
                crc >>= 1;
                crc ^= Seed;
            }
            else{
                crc >>= 1;
            }
        }
    }
    crc ^= 0xFFFF;

    return crc;
}

/*
* ��������GetSum8
* ��  ��������8λ�ۼӺ�
* ��  ����Buf - ���ݻ�����ʼ��ַ
		  Len - ������ܳ���
* ����ֵ��uint8 �ۼӺ�
*/
uint8 GetSum8(uint8 *buf, uint16 len)
{
    uint8 sum = 0;

	while(len-- > 0){
		sum += *buf++;
	}
	
    return sum;
}


//---------------------------------  �����	-----------------------------------------

/*
* ��������PackElectricRequestFrame
* ��  �������������������֡
* ��  ����buf - ���ݻ�����ʼ��ַ
		  dstAddr - Ŀ�ĵ�ַ
		  cmdId - ������
		  retryCnt - �ط�������0 - ��1�η��ͣ����� - ��n���ط�
* ����ֵ��uint8 ֡�ܳ���
*/
uint8 PackElectricRequestFrame(uint8 * buf, const uint8 * dstAddr, uint8 cmdId, uint8 *args[], uint8 retryCnt)
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
	buf[index++] = 0x06;	// expand : "SR2019"
	buf[index++] = 0x53;
	buf[index++] = 0x52;
	buf[index++] = 0x32;
	buf[index++] = 0x30;
	buf[index++] = 0x31;
	buf[index++] = 0x39;
	apsCmdStart = index;
	
	// cmd case
	switch(cmdId){

	//-------------------------------------------  ����		-------------
	// args[0] - ����ַ
	// args[1] - �м�����
	// args[2-n] - ��ַ�б�
	case PowerCmd_ReadMeter_645_97:
		apsFsn++;
		buf[nwkCmdStart] = 0x0A;		// aps ctrl
		buf[index++] = 0x00;
		buf[index++] = 0xFF;
		buf[index++] = 0xFF;
		buf[index++] = 0xFF;
		buf[index++] = 0xFF;
		dltStart = index;
		buf[index++] = 0x68;
		memcpy(&buf[index], args[0], 6);
		index += 6;
		buf[index++] = 0x68;
		buf[index++] = 0x01;
		buf[index++] = 0x02;
		buf[index++] = 0x43;
		buf[index++] = 0xC3;
		crc8 = GetSum8(&buf[dltStart], index - dltStart);
		buf[index++] = crc8;
		buf[index++] = 0x16;
		break;
	case PowerCmd_ReadMeter_645_07:
		apsFsn++;
		buf[nwkCmdStart] = 0x0A;	// aps ctrl
		buf[index++] = 0x00;
		buf[index++] = 0xFF;
		buf[index++] = 0xFF;
		buf[index++] = 0xFF;
		buf[index++] = 0xFF;
		dltStart = index;
		buf[index++] = 0x68;
		memcpy(&buf[index], args[0], 6);
		index += 6;
		buf[index++] = 0x68;
		buf[index++] = 0x11;
		buf[index++] = 0x04;
		buf[index++] = 0x33;
		buf[index++] = 0x33;
		buf[index++] = 0x34;
		buf[index++] = 0x33;
		crc8 = GetSum8(&buf[dltStart], index - dltStart);
		buf[index++] = crc8;
		buf[index++] = 0x16;
		break;
	case PowerCmd_ReadMeter_698:
		apsFsn++;
		buf[nwkCmdStart] = 0x0A;	// aps ctrl
		buf[index++] = 0x00;
		buf[index++] = 0xFF;
		buf[index++] = 0xFF;
		buf[index++] = 0xFF;
		buf[index++] = 0xFF;
		dltStart = index;
		buf[index++] = 0x68;	// start
		buf[index++] = 0x17;	// length :  length --> Fcs 
		buf[index++] = 0x00;
		buf[index++] = 0x43;	// ctrl
		buf[index++] = 0x05;	// addr  05 + addr(6 byte) + 00
		memcpy(&buf[index], args[0], 6);
		index += 6;
		buf[index++] = 0x00;
		crc16 = GetCrc16(&buf[dltStart + 1], index - dltStart, CRC16_Seed);
		buf[index++] = (uint8)(crc16 & 0xFF);	// header crc16 : length --> addr
		buf[index++] = (uint8)(crc16 >> 4);
		buf[index++] = 0x05;	// data filed
		buf[index++] = 0x01;
		buf[index++] = 0x01;
		buf[index++] = 0x00;
		buf[index++] = 0x10;
		buf[index++] = 0x02;
		buf[index++] = 0x00;
		buf[index++] = 0x00;
		crc16 = GetCrc16(&buf[dltStart + 1], index - dltStart, CRC16_Seed);
		buf[index++] = (uint8)(crc16 & 0xFF);	// frame crc16 : length --> data filed
		buf[index++] = (uint8)(crc16 >> 4);
		buf[index++] = 0x16;	// end
		break;
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
	case PowerCmd_ReadAllMeterDoc:
		apsFsn++;
		buf[index++] = 0x88;
		buf[index++] = *args[0];	// start index
		buf[index++] = 20;			// read cnt
		break;

	//-------------------------------------------  �ڵ����		-------------
	
	case PowerCmd_SetSerialCom:			/*	������/��� ����  */
		apsFsn++;
		buf[index++] = 0x00;
		buf[index++] = *args[0];
		buf[index++] = *args[1];
		break;
	case PowerCmd_SetChanelGrp:
		apsFsn++;
		buf[index++] = 0x01;
		buf[index++] = *args[0];
		break;
	case PowerCmd_SetRssiThreshold:
		apsFsn++;
		buf[index++] = 0x02;
		buf[index++] = *args[0];
		break;
	case PowerCmd_SetSendPower:
		apsFsn++;
		buf[index++] = 0x03;
		buf[index++] = *args[0];
		break;
	case PowerCmd_DeviceReboot:
		apsFsn++;
		buf[index++] = 0x05;
		break;
	case PowerCmd_ParamsInit:			/*	������ ����  */
		apsFsn++;
		buf[index++] = 0x90;
		break;
	case PowerCmd_StartNwkBuild:
		apsFsn++;
		buf[index++] = 0x91;
		break;
	case PowerCmd_StartNwkMaintain:
		apsFsn++;
		buf[index++] = 0x9A;
		break;
	case PowerCmd_BroadClearNeighbor:
		apsFsn++;
		buf[index++] = 0x97;
		break;
	case PowerCmd_BroadSetSendPower:
		apsFsn++;
		buf[index++] = 0x98;
		buf[index++] = *args[0];
		break;
	case PowerCmd_ClearNeighbor:		/*	��� ����  */
		apsFsn++;
		buf[index++] = 0x97;
		buf[index++] = 0x00;
		break;
	case PowerCmd_ChangeCollect2Addr:
		apsFsn++;
		buf[index++] = 0x96;
		memcpy(&buf[index], args[0], 6);
		index += 6;
		break;
	case PowerCmd_ForceJoinNwkRequest:
		apsFsn++;
		buf[index++] = 0x99;
		buf[index++] = *args[0];
		break;

	case PowerCmd_ReadReportData:		/*	����ˮ�� ����  */
		index = nwkCmdStart;
		buf[macCmdStart] = 0x3D;	// nwk ctrl
		buf[index++] = 0x0A;
		buf[index++] = 0x00;
		break;
	case PowerCmd_ClearReportData:
		apsFsn++;
		buf[index++] = 0x97;
		buf[index++] = 0x01;
		break;
	case PowerCmd_QueryBindedWaterMeter:
		index = nwkCmdStart;
		buf[macCmdStart] = 0x3D;	// nwk ctrl
		buf[index++] = 0xFA;
		buf[index++] = 0x00;
		buf[index++] = 0x00;
		break;
	case PowerCmd_AddBindedWaterMeter:
		index = nwkCmdStart;
		buf[macCmdStart] = 0x3D;	// nwk ctrl
		buf[index++] = 0xFA;
		buf[index++] = 0x01;
		memcpy(&buf[index], args[0], 7);
		index += 7;
		break;
	case PowerCmd_DelBindedWaterMeter:
		index = nwkCmdStart;
		buf[macCmdStart] = 0x3D;	// nwk ctrl
		buf[index++] = 0xFA;
		buf[index++] = 0x02;
		memcpy(&buf[index], args[0], 7);
		index += 7;
		break;
	
	default:
		break;
	}

	// calc length / crc16
	buf[0] = index - 1;
	buf[3] = buf[0] ^ buf[1] ^ buf[2];
	crc16 = GetCrc16(buf, index, CRC16_Seed);
	buf[index++] = (uint8)(crc16 & 0xFF);
	buf[index++] = (uint8)(crc16 >> 4);
	
	return index;
}

/*
* ��������ExplainElectricResponseFrame
* ��  ������������������Ӧ֡
* ��  ����buf - ���ջ�����ʼ��ַ
		  srcAddr - Դ��ַ
		  cmdId - ������
		  disp - ��������ʾ����
* ����ֵ��bool ���������fasle - ʧ�� �� true - �ɹ�
*/
bool ExplainElectricResponseFrame(uint8 * buf, uint16 rxlen, const uint8 * srcAddr, uint8 cmdId, ParamsBuf * disps)
{
	uint8 index = 0, len = 0, radius;
	uint8 nwkCtrl, apsCtrl, dltStart, i;
	bool ret = false;
	uint16 crc16;

	// ����������в���
	while(1){

		index += 3;

		// length check
		if(rxlen < index + 35){
			return false;
		}
		if(index + buf[index] + 3 > rxlen){
			return false;
		}	

		// dstaddr check
		if(strncmp(&buf[index + 9], LocalAddr, 6) != 0){
			index += buf[index] + 3;
			continue;
		}

		// crc16 check
		crc16 = buf[buf[index] + 1] + buf[buf[index] + 2] * 256;
		if(GetCrc16(&buf[index], buf[index] + 1, CRC16_Seed) != crc16){
			return false;
		}

		break;
	}


	// mac layer
	index += 21;
	// nwk layer
	nwkCtrl = buf[index];
	index += 13;
	radius = buf[index++] & 0x0F;
	index += (radius > 1 ? (radius - 1) * 6 + 3 : 0);

	// aps layer
	if(nwkCtrl == 0x3C || nwkCtrl == 0xBC){
		apsCtrl = buf[index++];	// aps ctrl
		index++;				// aps Fsn
		if(apsCtrl == 0x09){
			index += buf[index] + 1;
		}
	}
	
	// cmd case
	switch(cmdId){

	//-------------------------------------------  ����		-------------
	case PowerCmd_ReadMeter_645_97:
		index++;					// skip 0x00
		if(rxlen > index + 17 
			&& buf[index] == 0x68 && buf[index + 7] == 0x68
			&& buf[index + 8] == 0x91)
		{
			ret = true;
			
			buf[index + 14] -= 0x33;
			buf[index + 15] -= 0x33;
			buf[index + 16] -= 0x33;
			buf[index + 17] -= 0x33;
			GetStringHexFromBytes(&disps->buf[0], buf, index + 15, 3, 0, true);
			len = StringTrimStart(&disps->buf[0], '0');
			if(disps->buf[0] == 0x00){
				disps->buf[len++] = '0';
			}
			disps->buf[len++] = '.';
			GetStringHexFromBytes(&disps->buf[len], buf, index + 14, 1, 0, true);
			len++;
			disps->buf[len++] = 'k';
			disps->buf[len++] = 'W';
			disps->buf[len++] = 'h';                                   
			disps->cnt = 1;
			disps->items[0] = &disps->buf[0];
		}
		break;
	case PowerCmd_ReadMeter_645_07:
		index++;					// skip 0x00
		if(rxlen > index + 17 
			&& buf[index] == 0x68 && buf[index + 7] == 0x68
			&& buf[index + 8] == 0x91 && buf[index + 9] == 0x08)
		{
			ret = true;
			
			buf[index + 14] -= 0x33;
			buf[index + 15] -= 0x33;
			buf[index + 16] -= 0x33;
			buf[index + 17] -= 0x33;
			GetStringHexFromBytes(&disps->buf[0], buf, index + 15, 3, 0, true);
			len = StringTrimStart(&disps->buf[0], '0');
			if(disps->buf[0] == 0x00){
				disps->buf[len++] = '0';
			}
			disps->buf[len++] = '.';
			GetStringHexFromBytes(&disps->buf[len], buf, index + 14, 1, 0, true);
			len++;
			disps->buf[len++] = 'k';
			disps->buf[len++] = 'W';
			disps->buf[len++] = 'h';
			disps->items[0] = &disps->buf[0];
			disps->cnt = 1;
		}
		break;
	case PowerCmd_ReadMeter_698:
		index++;
		dltStart = index;
		if(buf[index] == 0x68 
			&& buf[index + 1] == 0x34 && buf[index + 2] == 0x00	// length
			&& buf[index + 3] == 0xC3 		// ctrl
			&& buf[index + 4] == 0x05
			){
			// �����ɹ����ݲ�����
			ret = true;
		}
		break;
	//-------------------------------------------  ������ȡ	 -------------
	
	case PowerCmd_ReadNodeInfo:		/*	������/��� ����  */
		buf[index++] = 0x04;
		break;
	case PowerCmd_ReadNwkStatus:
		buf[index++] = 0x93;
		break;
	case PowerCmd_ReadSendPower:
		buf[index++] = 0x94;
		break;
	case PowerCmd_ReadVerInfo:
		buf[index++] = 0x95;
		break;
	case PowerCmd_ReadNeighbor:			/*	��� ����  */
		buf[index++] = 0x10;
		break;
	case PowerCmd_ReadSubNodeRoute:		/*	������ ����  */
		buf[index++] = 0x92;
		break;
	case PowerCmd_ReadAllMeterDoc:
		buf[index++] = 0x88;
		break;

	//-------------------------------------------  �ڵ����		-------------
	
	case PowerCmd_SetSerialCom:			/*	������/��� ����  */
		buf[index++] = 0x00;
		break;
	case PowerCmd_SetChanelGrp:
		buf[index++] = 0x01;
		break;
	case PowerCmd_SetRssiThreshold:
		buf[index++] = 0x02;
		break;
	case PowerCmd_SetSendPower:
		buf[index++] = 0x03;
		break;
	case PowerCmd_DeviceReboot:
		buf[index++] = 0x05;
		break;
	case PowerCmd_ParamsInit:			/*	������ ����  */
		buf[index++] = 0x90;
		break;
	case PowerCmd_StartNwkBuild:
		buf[index++] = 0x91;
		break;
	case PowerCmd_StartNwkMaintain:
		buf[index++] = 0x9A;
		break;
	case PowerCmd_BroadClearNeighbor:
		buf[index++] = 0x97;
		break;
	case PowerCmd_BroadSetSendPower:
		buf[index++] = 0x98;
		break;
	case PowerCmd_ClearNeighbor:		/*	��� ����  */
		buf[index++] = 0x97;
		buf[index++] = 0x00;
		break;
	case PowerCmd_ChangeCollect2Addr:
		buf[index++] = 0x96;
		break;
	case PowerCmd_ForceJoinNwkRequest:
		buf[index++] = 0x99;
		break;

	case PowerCmd_ReadReportData:		/*	����ˮ�� ����  */
		buf[index++] = 0x0A;
		buf[index++] = 0x00;
		break;
	case PowerCmd_ClearReportData:
		buf[index++] = 0x97;
		buf[index++] = 0x01;
		break;
	case PowerCmd_QueryBindedWaterMeter:
		buf[index++] = 0xFA;
		buf[index++] = 0x00;
		buf[index++] = 0x00;
		break;
	case PowerCmd_AddBindedWaterMeter:
		buf[index++] = 0xFA;
		buf[index++] = 0x01;
		break;
	case PowerCmd_DelBindedWaterMeter:
		buf[index++] = 0xFA;
		buf[index++] = 0x02;
		break;
	
	default:
		break;
	}
	
	return ret;
}


// --------------------------------  ��������/��ȡ  -----------------------------------------



// --------------------------------  �ֳ�����  -----------------------------------------



// --------------------------------  �������ڵ�ͨ��  -----------------------------------------

void ElectricMainNodeFunc(void)
{
	
}


// --------------------------------  �����ӽڵ�ͨ��  -----------------------------------------

void ElectricSubNodeFunc(void)
{
	uint8 key, menuItemNo, tryCnt = 0;
	_GuiLisStruEx menuList;
	_GuiInputBoxStru inputBox;
	uint8 inputBuff[20] = {0};
	uint8 *buf;
	int inputLen;
	int index;

	_ClearScreen();

	// �˵�
	menuList.title = ">> �����ӽڵ�ͨ�� ";
	menuList.no = 3;
	menuList.MaxNum = 3;
	menuList.isRt = 0;
	menuList.x = 0;
	menuList.y = 0;
	menuList.with = 10 * 16;
	menuList.str[0] = "  1. ��ȡ����汾";
	menuList.str[1] = "  2. ��ȡ�ڵ�����";
	menuList.str[2] = "  3. 645-07����";
	menuList.defbar = 1;
	//_GUIHLine(0, 4*16 + 8, 160, 1);

	// �����
	inputBox.top = 2 * 16;
	inputBox.left = 3 * 16;
	inputBox.width = 7 * 16;
	inputBox.hight = 16;
	inputBox.caption = "";
	inputBox.context = inputBuff;
	inputBox.type = 1;		// ����
	inputBox.datelen = 12;	// ��󳤶�
	inputBox.keyUpDown = 1; 
	inputBox.IsClear = 1;
	_SetInputMode(1); 		//�������뷽ʽ 
	_DisInputMode(0);		//���뷨�Ƿ������л�

	_CloseCom();
	_ComSetTran(3);
	_ComSet((uint8 *)"19200,E,8,1", 2);

	while(1){

		_ClearScreen();
		menuItemNo = _ListEx(&menuList);

		if (menuItemNo == 0){
			break;
		}
		menuList.defbar = menuItemNo;
		_ClearScreen();

		switch(menuItemNo){
		case 1:		// " ��ȡ����汾 ";
			_Printfxy(0, 0, ">> ��ȡ����汾", 0);
			/*---------------------------------------------*/
			_GUIHLine(0, 1*16 + 4, 160, 1);	
			_Printfxy(0, 2*16, "��ַ:", 0);
			_Printfxy(0, 8*16, "  ����        ȷ��  ", 0);

			while(1)
			{
				key = _InputBox(&inputBox);
				if (key == KEY_CANCEL)
					break;

				inputLen = strlen(inputBox.context);
				if(inputLen == 0 || strncmp(ZeroAddr, inputBox.context, inputLen) == 0){
					_Printfxy(0, 4*16, "��������Ч��ַ", 0);
				}else{
					_Printfxy(0, 4*16, "                ", 0);
				}
				StringPadLeft(inputBox.context, 12, '0');


				index = GetBytesFromStringHex(TxBuf, 5, inputBox.context, 0, 1);
				TxLen += index;
				
				_GetComStr(RxBuf, 1024, 10);	// clear , 100ms timeout
				_SendComStr(TxBuf, TxLen);
				_Printfxy(0, 3*16, "���ݷ���...        ", 0);

				
				RxLen = _GetComStr(RxBuf, 50, 50);	// recv , 500ms timeout
				if(RxLen < 30 || strncmp(&RxBuf[9], "SRWF-", 5) != 0)
				{
					_Printfxy(0, 3*16, "���ճ�ʱ��       ", 0);
					continue;
				}
				
				sprintf(&TxBuf[20], "�汾��Ϣ:");
				_Printfxy(0, 3*16, &TxBuf[0], 0);
				_Printfxy(0, 4*16, &TxBuf[20], 0);

				_ReadKey();
				continue;
			}
			break;

		case 2:		// " ��ȡ�ڵ����� "
			_GUIRectangleFill(0, 5 * 16, 160, 9 * 16, 0);

			//sprintf(RxBuf, "�ļ�: %s\0", fileName);
			_Printfxy(0, 5*16, &RxBuf[0], 0);
			_Printfxy(0, 6*16, &RxBuf[20], 0);

			break;
			
		case 3:		// " 645-07���� ";
			CurrCmd = PowerCmd_ReadMeter_645_07;
			_Printfxy(0, 0, ">> 645-07����", 0);
			/*---------------------------------------------*/
			_GUIHLine(0, 1*16 + 4, 160, 1);	
			_Printfxy(0, 2*16, "��ַ:", 0);
			_Printfxy(0, 8*16, "  ����        ȷ��  ", 0);

			while(1)
			{
				key = _InputBox(&inputBox);
				if (key == KEY_CANCEL)
					break;

				inputLen = strlen(inputBox.context);
				if(inputLen == 0 || strncmp(ZeroAddr, inputBox.context, inputLen) == 0){
					_Printfxy(0, 4*16, "��������Ч��ַ", 0);
				}else{
					_Printfxy(0, 4*16, "                ", 0);
				}
				StringPadLeft(inputBox.context, 12, '0');
				GetBytesFromStringHex(DstAddr, 0, inputBox.context, 0, true);
				
				index = 0;
				memcpy(Args.buf, DstAddr, 6);
				Args.items[0] = &Args.buf[index];
				index += 6;
				Args.buf[index] = 0;
				Args.items[1] = &Args.buf[index];
				index += 1;
				Args.cnt = 2;
				
				TxLen = PackElectricRequestFrame(TxBuf, DstAddr, CurrCmd, Args.items, 0);
				_GetComStr(RxBuf, 1024, 10);	// clear , 100ms timeout
				_SendComStr(TxBuf, TxLen);
				_Printfxy(0, 3*16, "�����...        ", 0);

				RxLen = _GetComStr(RxBuf, 50, 50);	// recv , 500ms timeout
				if(false == ExplainElectricResponseFrame(RxBuf, RxLen, DstAddr, CurrCmd, &Disps))
				{
					_Printfxy(0, 3*16, "���ճ�ʱ��       ", 0);
					continue;
				}
				
				_Printfxy(0, 3*16, "����:", 0);
				_Printfxy(3 * 16, 3*16, Disps.items[0], 0);

				_ReadKey();
				continue;
			}

			
			break;

			default: 
				break;
		}
	}

	_CloseCom();
}

// --------------------------------  ˮ���ӽڵ�ͨ��  -----------------------------------------

void WaterMainNodeFunc(void)
{
	
}

// --------------------------------  ͸��ģ������  -----------------------------------------
void TransParentModuleFunc(void)
{
	uint8 key, menuItemNo, tryCnt = 0;
	_GuiLisStruEx menuList;
	char *fileName = NULL;
	char tmp[70];
	int fileHdl, fileLen, totalCnt, sendCnt;
	int index;
	
	_ClearScreen();

	// �˵�
	menuList.title = ">> ͸��ģ������ ";
	menuList.no = 3;
	menuList.MaxNum = 3;
	menuList.isRt = 0;
	menuList.x = 0;
	menuList.y = 0;
	menuList.with = 10 * 16;
	menuList.str[0] = "  1. �鿴��ǰ�汾";
	menuList.str[1] = "  2. �������ļ�";
	menuList.str[2] = "  3. ��ʼ����";
	menuList.defbar = 1;
	_GUIHLine(0, 4*16 + 8, 160, 1);

	_CloseCom();
	_ComSetTran(3);
	_ComSet((uint8 *)"19200,E,8,1", 2);

	while(1){

		menuItemNo = _ListEx(&menuList);

		if (menuItemNo == 0){
			break;
		}

		menuList.defbar = menuItemNo;

		switch(menuItemNo){
		case 1:	// " �鿴��ǰ�汾 ";
			_GUIRectangleFill(0, 5 * 16, 160, 9 * 16, 0);
			TxLen = 0;
			TxBuf[TxLen++] = 0xAA;
			TxBuf[TxLen++] = 0xBB;
			TxBuf[TxLen++] = 0x01;
			TxBuf[TxLen++] = 0x07;
			TxBuf[TxLen++] = 0xCC;
			_GetComStr(RxBuf, 1024, 10);	// clear , 100ms timeout
			_SendComStr(TxBuf, TxLen);
			_Printfxy(0, 5*16, "��ѯ��...", 0);

			sprintf(TxBuf, "��ǰ�汾:");
			RxLen = _GetComStr(&TxBuf[9], 50, 50);	// recv , 500ms timeout
			if(RxLen < 30 || strncmp(&TxBuf[9], "SRWF-", 5) != 0)
			{
				_Printfxy(0, 5*16, "���ճ�ʱ", 0);
				break;
			}
			_Printfxy(0, 5*16, &TxBuf[0], 0);
			_Printfxy(0, 6*16, &TxBuf[20], 0);
			_Printfxy(0, 7*16, &TxBuf[40], 0);
			break;

		case 2:	// " �������ļ� "
			_GUIRectangleFill(0, 5 * 16, 160, 9 * 16, 0);

			_SaveScreenToBuff(Screenbuff);
			_ClearScreen();
			fileName = _GetFileList("ѡ|\n��|\n��|\n��|\n��|\n��|\n  |\n  |\n", "", "");
			_ClearScreen();
			_RestoreBuffToScreen(Screenbuff);

			if (fileName == NULL){
				break;
			}
			
			sprintf(tmp, "�ļ�: %s\0", fileName);
			_Printfxy(0, 5*16, &tmp[0], 0);
			_Printfxy(0, 6*16, &tmp[20], 0);

			fileHdl = _Fopen(fileName, "R");
			fileLen = _Filelenth(fileHdl);
			totalCnt = (fileLen + 1023)/1024;
			sendCnt = 0;
			_Fread(TxBuf, 1024, fileHdl);
			_Fclose(fileHdl);
				
			index = IndexOf(TxBuf, 1024, "SRWF-", 5, 512, 512);
			if(index < 0){
				_Printfxy(0, 7*16, "����4E88-APP�ļ�", 0);
				fileName = NULL;
			}
			else{
				sprintf(tmp, "��С:%dK,�ܰ���:%d\0", fileLen/1024, totalCnt);
				_Printfxy(0, 7*16, &tmp[0], 0);
			}
			break;
			
		case 3:	// " ��ʼ���� ";
			_GUIRectangleFill(0, 5 * 16, 160, 9 * 16, 0);
			totalCnt = 200;
			
			// ��ʼ��
			if (fileName == NULL){
				_Printfxy(0, 5*16, "����ѡ�������ļ�", 0);
				break;
			}
			fileHdl = _Fopen(fileName, "R");
			sendCnt = 0;

			sprintf(tmp, "�ܰ���: %d\0", totalCnt);
			_Printfxy(0, 5*16, &tmp[0], 0);
			sprintf(tmp, "������: %d   \0",sendCnt);
			_Printfxy(0, 6*16, &tmp[0], 0);
			_Printfxy(0, 9*16, "״̬: ������...", 0);

			ShowProgressBar(7*16+8, totalCnt, sendCnt);

			// ��������
			while(1){

				if(tryCnt > 3 || sendCnt >= totalCnt){
					break;
				}
				
				TxLen = _Fread(TxBuf, 1024, fileHdl);
				_GetComStr(RxBuf, 1024, 1);		// clear , 100ms timeout
				_SendComStr(TxBuf, TxLen);

				sprintf(tmp, "������: %d   \0",sendCnt + 1);
				_Printfxy(0, 6*16, &tmp[0], 0);
				if(tryCnt > 0){
					sprintf(tmp, "����%d \0",tryCnt);
					_Printfxy(6*16, 6*16, &tmp[0], 0);
				}
				tryCnt++;

				RxLen = _GetComStr(&TxBuf[9], 50, 1);	// recv , 500ms timeout
				if(RxLen < 10){
				//	continue;
				}

				sendCnt++;
				tryCnt = 0;
				ShowProgressBar(7*16+8, totalCnt, sendCnt);

			}
			_Fclose(fileHdl);

			// �������
			if(tryCnt > 3){
				_Printfxy(0, 9*16, "״̬: ����ʧ��  ", 0);
			}else{
				_Printfxy(0, 9*16, "״̬: �������  ", 0);
			}
			_SoundOn();
			_Sleep(500);
			_SoundOff();
			_Sleep(300);
			_SoundOn();
			_Sleep(500);
			_SoundOff();
			break;

			default: 
				break;
		}
	}

	_CloseCom();
}


// --------------------------------   ������   -----------------------------------------------
int main(void)
{
	_GuiMenuStru MainMenu;
	
	MainMenu.left=0;
	MainMenu.top=0;
	MainMenu.no=4;
	MainMenu.title= "  ɣ���ֳֻ�  ";
	MainMenu.str[0]=" �������ڵ�ͨ�� ";
	MainMenu.str[1]=" �����ӽڵ�ͨ�� ";
	MainMenu.str[2]=" ˮ���ӽڵ�ͨ�� ";
	MainMenu.str[3]=" ͸��ģ������ ";
	MainMenu.key[0]="1";
	MainMenu.key[1]="2";
	MainMenu.key[2]="3";
	MainMenu.key[3]="4";
	MainMenu.Function[0]=ElectricMainNodeFunc;
	MainMenu.Function[1]=ElectricSubNodeFunc;
	MainMenu.Function[2]=WaterMainNodeFunc;
	MainMenu.Function[3]=TransParentModuleFunc;
	MainMenu.FunctionEx=0;
	_OpenLcdBackLight();
	_Menu(&MainMenu);	
}

