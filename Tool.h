#ifndef Tool_H
#define Tool_H

#include <stdarg.h>
#include "Common.h"

// ���ñ��ļ�ǰ ������ <HJLIB.H> �� "string.h"
extern void _GUIRectangleFill(uint32 x, uint32 y, uint32 x1, uint32 y1, uint8 color);
extern size_t strlen(const char * /*s*/);

//---------------------------------  ͨ�÷���  -------------------------------------
//
//	IndexOf				�������в��������飬��ָ�����ҵ���ʼλ�úͷ�Χ
//	ShowProgressBar		��ʾ������
//	StringPadLeft		�ַ���������
//	StringTrimStart		�ַ���ͷ���ü�
//	HexToChar			16������ת���ɶ�Ӧ���ַ�
//	CharToHex			�ַ�ת���ɶ�Ӧ��16������
//	GetStringHexFromBytes	���ֽ�����ת����16�����ַ���
//	GetBytesFromStringHex	��16�����ַ���ת�����ֽ�����
//	GetCrc16 			����CRC16
//	GetSum8				����8λ�ۼӺ�
//
//-----------------------------------------------------------------------------------

/*
* ��������IndexOf
* ��  �����������в��������飬��ָ�����ҵ���ʼλ�úͷ�Χ
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
* ��  ������ȡ������ַ���
* ��  ����inputSt - �����ṹָ��
* ����ֵ��uint8  - ����󷵻صİ��� �� ��/�¼���ȷ�ϼ���ȡ����
*/
uint8 GetInputNumStr(_GuiInputBoxStru * inputSt)
{
	static uint8 keyBuf[TXTBUF_LEN] = {0};
	uint8 key, idx, cleared = false;
	char keyStr[2] = {0};
	uint8 x = inputSt->left;
	uint8 y = inputSt->top;

	memcpy(keyBuf, inputSt->context, TXTBUF_LEN);
	_Printfxy(x, y, keyBuf, Color_White);
	_toxy(x, y + inputSt->hight);
	_ShowCur();
	idx = 0;

	while(1){
		key = _ReadKey();

		if(key >= KEY_0 && key <= KEY_9){

			if(inputSt->IsClear && cleared == false){
				memset(keyBuf, 0x00, TXTBUF_LEN);
				_GUIRectangleFill(inputSt->left, inputSt->top, 
					(inputSt->left + inputSt->width), 
					(inputSt->top + inputSt->hight), Color_White);
				cleared = true;
			}

			keyBuf[idx] = key;
			keyStr[0] = key;
			_Printfxy(x, y, keyStr, Color_White);
			if(idx != inputSt->datelen -1){
				idx++;
				x += 8;
			}
		}
		else if(key == KEY_LEFT){
			if(idx != 0){
				idx--;
				x -= 8;
			}
		}
		else if(key == KEY_RIGHT && keyBuf[idx] >= '0' && keyBuf[idx] <= '9'){
			if(idx != inputSt->datelen - 1){
				idx++;
				x += 8;
			}
		}
		else if(key == KEY_DELETE){
			if(idx == inputSt->datelen -1 && keyBuf[idx] != 0x00){
				keyBuf[idx] = 0x00;
				_Printfxy(x, y, " ", Color_White);
			}
			else if(idx > 0 && keyBuf[idx] == 0x00){
				keyBuf[idx -1] = 0x00;
				_Printfxy(x - 8, y, " ", Color_White);
				idx--;
				x -= 8;
			}
			else if(idx == 0 && keyBuf[idx] != 0x00){
				memset(keyBuf, 0x00, TXTBUF_LEN);
				_GUIRectangleFill(inputSt->left, inputSt->top, 
					(inputSt->left + inputSt->width), 
					(inputSt->top + inputSt->hight), Color_White);
			}
		}
		else if(key == KEY_UP || key == KEY_DOWN
			|| key == KEY_ENTER || key == KEY_CANCEL){
			break;
		}
		else{
			// do nothing
		}

		_toxy(x, y + inputSt->hight);
	}

	if(key != KEY_CANCEL && keyBuf[0] != 0x00){
		memcpy(inputSt->context, keyBuf, TXTBUF_LEN);
	}

	_HideCur();

	return key;
}

/*
* ��  ������ʾ�����
* ��  ����x, y		- �����ǰ��ı�ǩ����ʼ����
*		 item		- ���������
*		 title		- �����ǰ��ı�ǩ
*		 text		- �ַ���������
*		 maxLen		- ����ַ�������
*		 width		- ����򳤶�
* ����ֵ��void
*/
void InputBoxShow(InputItem *item, uint8 x, uint8 y, const char * title, char * text, uint8 maxLen, uint8 width)
{
	uint8 x1 = x + strlen(title) * 8;

	item->x = x;
	item->y = y;
	item->title = title;
	item->text = text;
	item->dataLen = maxLen;
	item->width = width;

	_Printfxy(x, y, title, Color_White);
	_Printfxy(x1 + 4, y, text, Color_White);
}

/*
* ��  ������ȡ������е��ַ���
* ��  ����x, y		- �����ǰ��ı�ǩ����ʼ����
*		 title		- �����ǰ��ı�ǩ
*		 text		- �ַ���������
*		 maxLen		- ����ַ�������
*		 length		- ����򳤶�
* ����ֵ��uint8  - ����󷵻صİ��� �� ��/�¼���ȷ�ϼ���ȡ����
*/
uint8 InputBoxGetStr(uint8 x, uint8 y, const char * title, char * text, uint8 maxLen)
{
	const char * ZeroAddr = "000000000000";
	static _GuiInputBoxStru NewInput;
	uint8 retKey = 0;

	// ���������
	NewInput.top = y;
	NewInput.left = x + strlen(title) * 8 + 4;
	NewInput.width = 10 * 16;	// ����ʾ�������Ϊ���
	NewInput.hight = 16;
	NewInput.caption = "";
	NewInput.context = text;
	NewInput.type = 1;			// ����
	NewInput.datelen = maxLen;	// ��󳤶�
	NewInput.keyUpDown = 1; 
	NewInput.IsClear = 1;
	_SetInputMode(1); 			//�������뷽ʽ 
	_DisInputMode(1);			//���뷨�Ƿ������л�

	do{
		//��������
		retKey = GetInputNumStr(&NewInput);

		if( NewInput.context[0] >= '0' && NewInput.context[0] <= '9'){

			if(retKey == KEY_ENTER 
				|| retKey == KEY_UP
				|| retKey == KEY_DOWN )
			{
				_leftspace(NewInput.context, maxLen, '0');
			}

			if(strncmp(ZeroAddr, NewInput.context, maxLen) == 0){
				sprintf(NewInput.context, "��ַ����Ϊ0");
				retKey = 0xFF;
			}

			_Printfxy(NewInput.left, NewInput.top, NewInput.context, Color_White);
		}
		else if(NewInput.context[0] != 0x00){
			NewInput.context[0] = 0x00;
		}
	}while(retKey == 0xFF);
	
	return retKey;
}

/*
* ��  ������ʾ����������� 
* ��  ����inputList	- �������뻺��ṹ
*		 *itemNo	- ��ʼ��ʱ��λ���ĸ�������
* ����ֵ��uint8  - �����˳�ʱ�İ���ֵ��ȷ�ϼ���ȡ����
*/
uint8 ShowInputUI(InputItemBuf inputList, uint8 *itemNo)
{
	uint8 key;
	InputItem *ptr;

	(*itemNo) = ((*itemNo) > inputList.cnt -1 ? 0 : (*itemNo));

	while(1){

		ptr = &inputList.items[(*itemNo)];

		key = InputBoxGetStr(ptr->x, ptr->y, ptr->title, ptr->text, ptr->dataLen);

		if(key == KEY_CANCEL || key == KEY_ENTER){
			break;
		}

		if(key == KEY_DOWN){
			if((*itemNo) < inputList.cnt -1){
				(*itemNo)++;
			}
			continue;
		}
		else if(key == KEY_UP){
			if((*itemNo) > 0){
				(*itemNo)--;
			}
			continue;
		}
		else{
			continue;
		}

	}

	return key;
}

/*
* ��������PrintfXyMultiLine_VaList
* ��  ��������Ļ x,y ������ʾ�����ַ��������Զ�����
* ��  ����x, y		- ��Ļ������
		  format	- �ַ�����ʽ
* ����ֵ��void
*/
void PrintfXyMultiLine(uint8 x, uint8 y, const char * buf)
{
	uint16 len; 
	uint8 * pr = buf;

	// first line
	_Printfxy(x, y, pr, 0);
	
	// next lines
	len = 0;
	while(*pr != '\0'){
		if(*pr == '\n' || (x + len) == 20){
			pr = (*pr == '\n' ? pr + 1 : pr);
			len = 0;
			x = 0;
			y += 16;
			_Printfxy(x, y, pr, 0);
		}
		pr++;
		len++;
	}
}

/*
* ��������PrintfXyMultiLine_VaList
* ��  ��������Ļ x,y ������ʾ�����ַ��������Զ�����
* ��  ����x, y		- ��Ļ������
		  format	- �ַ�����ʽ
		  ... 		- �ɱ����
* ����ֵ��void
*/
void PrintfXyMultiLine_VaList(uint8 x, uint8 y, const char * format, ...)
{
	static uint8 buf[512] = {0};
	uint16 len; 
	va_list ap;

	va_start(ap, format);
	len = vsprintf(buf, format, ap);
	buf[len] = '\0';
	va_end(ap);

	PrintfXyMultiLine(x, y, buf);
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
		*pw = 0x00;
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
		  iLength - �����пɱ�����󳤶�
		  strHex - Դ�ַ�����������ַ
		  separate - �ַ�����Hex�ֽ�֮��ļ������0 - �޼������ �����ַ� - ��ո�򶺺�
		  reverse - �Ƿ���Ҫ����false - ������ true - ����
* ����ֵ��int - ת������ֽ�����0 - ת��ʧ��
*/
int GetBytesFromStringHex(uint8 bytes[], int iStart, int iLength, const char * strHex, char separate, bool reverse)
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

	while (iLoop < strHexLen - 1 && index < iLength)
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
* ��������GetCrc16
* ��  ��������CRC8
* ��  ����Buf - ���ݻ�����ʼ��ַ
		  Len - ������ܳ���
		  Seed - �����/ˮ���̶�ʹ�� 0x8408
* ����ֵ��uint8 CRC16ֵ
*/
uint8 GetCrc8(uint8 *Buf, int len)
{
    uint8 i;
    uint8 crc = 0x0;

    while (len--)
    {
        crc ^= *Buf++;
        for (i = 0; i < 8; i++)
        {
            if (crc & 0x01)
                crc = (crc >> 1) ^ 0x8C;
            else
                crc >>= 1;
        }
    }
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

#endif