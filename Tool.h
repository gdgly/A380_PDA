#ifndef Tool_H
#define Tool_H

#include "HJLIB.H"
#include "stdarg.h"
#include "Common.h"

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
* ��  ������ӡ��־���ļ��򴮿�No.2 , ͨ�Ŵ������շ�ʱ���ɴ�ӡ������
* ��  ����fileName	 - �ļ���
		  format	- �ַ�����ʽ
		  ... 		- �ɱ����
* ����ֵ��void
*/
void LogPrint(const char * fileName, const char * format, ...)
{
#if Log_On
	//static uint8 buf[512] = {0};
	int fp;
	uint32 len = 0; 
	va_list ap;
	uint8 *buf;

	buf = (uint8 *) _malloc(1024);

	va_start(ap, format);
	len += vsprintf(&buf[0], format, ap);
	buf[len++] = '\n';
	buf[len++] = '\0';
	va_end(ap);
	
	if(_Access("debug.txt", 0) < 0){
		fp = _Fopen("debug.txt", "W");
	}else{
		fp = _Fopen("debug.txt", "RW");
	}

#if LogScom_On
	_CloseCom();
	_ComSetTran(Trans_IR_Quick);
	_ComSet((uint8 *)"115200,E,8,1", 2);
	_SendComStr(buf, len);
	_CloseCom();
#else
	_Lseek(fp, 0, 2);
	_Fwrite(buf, len, fp);
	_Fclose(fp);
#endif

	_free(buf);

#endif
}

/*
* ��  ������ӡ��־���ļ��򴮿�No.2 , ͨ�Ŵ������շ�ʱ���ɴ�ӡ������
* ��  ����title	 - ����
		  buf	- �ֽ�������ʼ��ַ
		  size	- ��ӡ���ֽ����� ���1024
* ����ֵ��void
*/
void LogPrintBytes(const char *title, uint8 *buf, uint16 size)
{
#if Log_On
	const char decHex[16] = {'0', '1', '2', '3','4', '5', '6', '7','8', '9', 'A', 'B','C', 'D', 'E', 'F'};
	uint16 i = 0;
	uint8 *tmp;

	tmp = (uint8 *) _malloc(1024);
	while(size--){
		tmp[i++] = decHex[*buf >> 4];
		tmp[i++] = decHex[*buf & 0x0F];
		tmp[i++] = ' ';
		buf++;
	}
	tmp[i++] = '\0';

	LogPrint(LogName, "%s%s", title, tmp);
	_free(tmp);

#endif
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
* ��  ����uiItem - Ui����ṹָ��
* ����ֵ��uint8  - �˳�Ui���ʱ���صİ��� �� ��/�¼���ȷ�ϼ���ȡ����
*/
static uint8 GetInputNumStr(UI_Item *uiItem)
{
	static uint8 keyBuf[TXTBUF_LEN] = {0};
	uint8 key, cleared = false;
	char keyStr[2] = {0};
	int idx;
	int x = uiItem->x1;
	int y = uiItem->y1;

	memcpy(keyBuf, uiItem->text, TXTBUF_LEN);
	_Printfxy(x, y, keyBuf, Color_White);
	_toxy(x, y + uiItem->height);
	_ShowCur();
	idx = 0;

	while(1){
		key = _ReadKey();

		if((key >= KEY_0 && key <= KEY_9) 
			||(key == KEY_DOT && idx > 0 && idx < uiItem->txtbox.dataLen -1)){

			if(uiItem->txtbox.isClear && cleared == false && idx == 0){
				memset(keyBuf, 0x00, TXTBUF_LEN);
				_GUIRectangleFill(uiItem->x1, uiItem->y1, 
					(uiItem->x1 + uiItem->width), 
					(uiItem->y1 + uiItem->height), Color_White);
				cleared = true;
			}

			keyBuf[idx] = key;
			keyStr[0] = key;
			_Printfxy(x, y, keyStr, Color_White);
			if(idx != uiItem->txtbox.dataLen -1){
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
		else if((key == KEY_RIGHT && keyBuf[idx] >= '0' && keyBuf[idx] <= '9')
			|| (key == KEY_DOT)){
			if(idx != uiItem->txtbox.dataLen - 1){
				idx++;
				x += 8;
			}
		}
		else if(key == KEY_DELETE){
			if(false == uiItem->txtbox.isClear){
				keyBuf[idx] = '0';
				_Printfxy(x, y, "0", Color_White);
				idx--;
				x -= 8;
			}
			else if(idx == uiItem->txtbox.dataLen -1 && keyBuf[idx] != 0x00){
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
				_GUIRectangleFill(uiItem->x1, uiItem->y1, 
					(uiItem->x1 + uiItem->width), 
					(uiItem->y1 + uiItem->height), Color_White);
			}

			if(idx < 0) idx = 0;
			if(x < uiItem->x1) x = uiItem->x1;
		}
		else if(key == KEY_UP || key == KEY_DOWN
			|| key == KEY_ENTER || key == KEY_CANCEL){
			break;
		}
		else{
			// do nothing
		}

		_toxy(x, y + uiItem->height);
	}

	if(key != KEY_CANCEL){
		memcpy(uiItem->text, keyBuf, TXTBUF_LEN);
	}

	_HideCur();

	return key;
}

/*
* ��  ������ȡѡ���б��ǰѡ��
* ��  ����uiItem - Ui����ṹָ��
* ����ֵ��uint8  - �˳�Ui���ʱ���صİ��� �� ��/�¼���ȷ�ϼ���ȡ����
*/
static uint8 CombBoxGetCurrIndex(UI_Item *uiItem)
{
	uint8 key, isShowIcon = 1, cnt = 0;
	uint8 x, y, idx, lastIdx = 0xFF;

	idx = *uiItem->combox.currIdx;
	
	while(1){

		key = _GetKeyExt();

		if(idx != lastIdx){
			uiItem->text = uiItem->combox.strs[idx];
			x = uiItem->x1 + (uiItem->width - strlen(uiItem->text)*8)/2;
			y = uiItem->y1;
			_GUIRectangleFill(uiItem->x1 + 16, y, uiItem->x1 + uiItem->width - 16, y + 16, Color_White);
			_Printfxy(x, y, uiItem->text, Color_White);
			lastIdx = idx;
		}

		if(isShowIcon && cnt == 3){
			_Printfxy(uiItem->x1, uiItem->y1, "<<", Color_White);
			_Printfxy(uiItem->x1 + uiItem->width - 16, uiItem->y1, ">>", Color_White);
			isShowIcon = 0;
			cnt = 0;
		}else if(!isShowIcon && cnt == 3){
			_Printfxy(uiItem->x1, uiItem->y1, "  ", Color_White);
			_Printfxy(uiItem->x1 + uiItem->width - 16, uiItem->y1, "  ", Color_White);
			isShowIcon = 1;
			cnt = 0;
		}
		_Sleep(100);
		cnt++;

		if(key == KEY_LEFT){
			if(idx > 0){
				idx--;
			}
		}
		if(key == KEY_RIGHT){
			if(idx < uiItem->combox.cnt -1){
				idx++;
			}
		}
		else if(key == KEY_UP || key == KEY_DOWN
			|| key == KEY_ENTER || key == KEY_CANCEL){
			break;
		}
		else{
			// do nothing
		}
	}

	_Printfxy(uiItem->x1, uiItem->y1, "<<", Color_White);
	_Printfxy(uiItem->x1 + uiItem->width - 16, uiItem->y1, ">>", Color_White);
	*uiItem->combox.currIdx = idx;

	return key;
}

/*
* ��  �������������
* ��  ����x, y		- UI���ǰ��ı�ǩ����ʼ����
*		 item		- UI�������
*		 title		- UI���ǰ��ı�ǩ
*		 text		- �ַ���������
*		 maxLen		- ����ַ�������
*		 width		- ����򳤶�
*		 isClear	- ����ʱ�Ƿ����
* ����ֵ��void
*/
void TextBoxCreate(UI_Item *item, uint8 x, uint8 y, const char *title, char *text, uint8 maxLen, uint8 width, bool isClear)
{
	item->x = x;
	item->y = y;
	item->title = title;
	item->x1 = x + strlen(title) * 8;
	item->y1 = y;
	item->text = text;
	item->width = width;
	item->height = 16;
	item->type = UI_TxtBox;
	item->txtbox.dataLen = maxLen;
	item->txtbox.isClear = isClear;
}

/*
* ��  ��������ѡ��� �����Ҽ��л�ѡ��
* ��  ����x, y		- UI���ǰ��ı�ǩ����ʼ����
*		 item		- UI�������
*		 title		- UI���ǰ��ı�ǩ
*		 currIdx	- ��ǰѡ���ַ������� 0~9
*		 maxCnt		- ���ѡ���ַ������� 1~10
*		 ...		- N��ѡ���ַ���
* ����ֵ��void
*/
void CombBoxCreate(UI_Item *item, uint8 x, uint8 y, const char *title, uint8 *currIdx, uint32 maxCnt, ...)
{
	va_list ap;
	char i, *ptr;

	va_start(ap, maxCnt);
	for(i = 0; i < maxCnt && i < 10; i++){
		ptr = va_arg(ap, char *);
		if(ptr == NULL){
			break;
		}
		item->combox.strs[i] = ptr;
	}

	if(*currIdx > i -1){
		*currIdx = 0;
	}

	item->x = x;
	item->y = y;
	item->title = title;
	item->x1 = x + strlen(title) * 8;
	item->y1 = y;
	item->text = NULL;
	item->width = 160 - item->x1;
	item->height = 16;
	item->type = UI_CombBox;
	item->combox.cnt = (uint8)i;
	item->combox.currIdx = currIdx;
}

/*
* ��  ������ʾUI���� 
* ��  ����uiList	- UI����б�ṹ
*		 *itemNo	- ��ʼ��ʱ��λ���ĸ�������
* ����ֵ��uint8  - �����˳�ʱ�İ���ֵ��ȷ�ϼ���ȡ����
*/
uint8 ShowUI(UI_ItemList uiList, uint8 *itemNo)
{
	const char * ZeroAddr = "000000000000";
	uint8 key, x, y;
	uint8 i;
	UI_Item *ptr;

	for(i = 0; i < uiList.cnt; i++){
		ptr = &uiList.items[i];
		_Printfxy(ptr->x, ptr->y, ptr->title, Color_White);
		_Printfxy(ptr->x1, ptr->y1, ptr->text, Color_White);

		if(ptr->type == UI_CombBox){
			ptr->text = ptr->combox.strs[*ptr->combox.currIdx];
			x = ptr->x1 + (ptr->width - strlen(ptr->text)*8)/2;
			y = ptr->y1;
			_Printfxy(x, y, ptr->text, Color_White);
			_Printfxy(ptr->x1, ptr->y1, "<<", Color_White);
			_Printfxy(ptr->x1 + ptr->width - 16, ptr->y1, ">>", Color_White);
		}
	}

	(*itemNo) = ((*itemNo) > uiList.cnt -1 ? 0 : (*itemNo));

	while(1){

		ptr = &uiList.items[(*itemNo)];

		if(ptr->type == UI_TxtBox){
			do{
				//��������
				key = GetInputNumStr(ptr);

				if( ptr->text[0] >= '0' && ptr->text[0] <= '9'){
					if(ptr->txtbox.dataLen == 12 && (key == KEY_ENTER || key == KEY_UP || key == KEY_DOWN)){
						_leftspace(ptr->text, ptr->txtbox.dataLen, '0');
						if(strncmp(ZeroAddr, ptr->text, ptr->txtbox.dataLen) == 0){
							sprintf(ptr->text, " ����Ϊ0 ");
							key = 0xFF;
						}
					}
					_Printfxy(ptr->x1, ptr->y1, ptr->text, Color_White);
				}
			}while(key == 0xFF);
		}
		else if(ptr->type == UI_CombBox){
			key = CombBoxGetCurrIndex(ptr);
		}

		if(key == KEY_CANCEL || key == KEY_ENTER){
			break;
		}

		if(key == KEY_DOWN){
			if((*itemNo) < uiList.cnt -1){
				(*itemNo)++;
			}else{
				(*itemNo) = 0;
			}
			continue;
		}
		else if(key == KEY_UP){
			if((*itemNo) > 0){
				(*itemNo)--;
			}else{
				(*itemNo) = uiList.cnt -1;
			}
			continue;
		}
		else{
			continue;
		}

	}

	// set the not-number str to null when back
	if(ptr->text[0] > '9' || ptr->text[0] < '0'){
		ptr->text[0] = 0x00;
	}

	return key;
}

/*
* ��  ������ȡ����Ļx���꿪ʼ��ʾ�����ַ���������
* ��  ����x			- ��Ļ����ʾ��x����
		  buf		- �ַ�����ʼ��ַ
		  lines		- ÿ�е���ʼ��ַ
* ����ֵ��uint8	 �ַ���������
*/
uint8 GetPrintLines(uint8 x, const char * buf, char * lines[])
{
	uint8 lineCnt = 0, col = 0; 
	uint8 * pr = buf;

	// first line
	lines[lineCnt] = pr;
	lineCnt++;

	// next lines
	while(*pr != 0x00){
		if(*pr == '\n' || (x + col * 8) > 160){
			
			col = 0;
			x = 0;
			pr = (*pr == '\n' ? pr + 1 : pr);

			lines[lineCnt] = pr;
			lineCnt++;

			if(*pr == 0x00){
				lineCnt--;
				break;
			}
		}
		pr++;
		col++;
	}

	return lineCnt;
}

/*
* ��  ��������Ļ x,y ������ʾ�����ַ��������Զ�����
* ��  ����x, y		- ��Ļ������
		  buf		- �ַ�����ʼ��ַ
		  maxLines	- �������ʾ������
* ����ֵ��void
*/
void PrintfXyMultiLine(uint8 x, uint8 y, const char * buf, uint8 maxLines)
{
	static uint8 dispLine[21] = {0};
	uint8 row = 0, col = 0; 
	uint8 * pcol, *prow;

	pcol = buf;

	for(row = 0; row < maxLines; row++){

		prow = pcol;

		for(col = x/8; col < 20; col++){
			if(col == 19 && *(pcol + 1) == '\n'){
				pcol++;
			}
			else if(*pcol == '\n'){
				pcol++;
				break;
			}
			else if(*pcol == 0x00){
				break;
			}
			pcol++;
		}
		memcpy(dispLine, prow, col);
		dispLine[col] = 0x00;
		_Printfxy(x, y, dispLine, Color_White);

		if(*pcol == 0x00){
			break;
		}

		col = 0;
		x = 0;
		y += 16;
	}
}

/*
* ��  ��������Ļ x,y ������ʾ�����ַ��������Զ�����
* ��  ����x, y		- ��Ļ������
		  format	- �ַ�����ʽ
		  ... 		- �ɱ����
* ����ֵ��void
*/
void PrintfXyMultiLine_VaList(uint8 x, uint8 y, const char * format, ...)
{
	static uint8 buf[512] = {0};
	int len;
	va_list ap;

	va_start(ap, format);
	len = vsprintf(buf, format, ap);
	if(len < 0 || len > 512){
		len = 0;
	}
	buf[len] = '\0';
	va_end(ap);

	PrintfXyMultiLine(x, y, buf, 10);	// �����ʾ10��
}

/*
* ��  ��������Ļ x,y ������ʾʵ��������
* ��  ����x, y		- ��Ļ������
		  direction	- �����η���0 - ���� �� �� 1 - ���� ��
* ����ֵ��void
*/
void PrintXyTriangle(uint8 x, uint8 y, uint8 direction)
{
	uint8 x1, y1;

	if(direction == 0){
		x1 = 7/2;
		for(y1 = 1; y1 <= 7/2 + 1; y1++){
			_GUIHLine(x + x1, y + y1, x + 7 - x1, Color_Black);
			x1--;
		}
	}else{
		x1 = 0;
		for(y1 = 0; y1 < 7/2 + 1; y1++){
			_GUIHLine(x + x1, y + y1, x + 7 - x1, Color_Black);
			x1++;
		}
	}
}

/*
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
* ��  ����16������ת���ɶ�Ӧ���ַ�
*/
char HexToChar(uint8 b)
{
	const char decHex[16] = {'0', '1', '2', '3','4', '5', '6', '7','8', '9', 'A', 'B','C', 'D', 'E', 'F'};
	
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