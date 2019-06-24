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
* ��  ������ӡ��־���ļ��򴮿�No.3 , ͨ�Ŵ������շ�ʱ���ɴ�ӡ������
*		  	Ĭ����־�ļ�������Ϊ #define LogName "debug.txt" �� 
*			�����޸����ڵ���ǰ���¶��� LogName 
* ��  ����format	- �ַ�����ʽ
		  ... 		- �ɱ����
* ����ֵ��void
*/
void LogPrint(const char * format, ...)
{
#if LOG_ON
	
#if !(LogScom_On)
	int fp;
#endif
	uint32 len = 0; 
	va_list ap;
	uint8 *buf;

	buf = (uint8 *) _malloc(1024);

	va_start(ap, format);
	len += vsprintf(&buf[0], format, ap);
	buf[len++] = '\n';
	buf[len++] = '\0';
	va_end(ap);
	

#if LogScom_On
	_CloseCom();
	_ComSetTran(LogPort);
	_ComSet((uint8 *)"115200,E,8,1", 2);
	_SendComStr(buf, len);
	_CloseCom();
#else

	#ifndef LogName
		#define LogName "debug.txt"
	#endif
	if(_Access(LogName, 0) < 0){
		fp = _Fopen(LogName, "W");
	}else{
		fp = _Fopen(LogName, "RW");
	}
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
#if LOG_ON
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

	LogPrint("%s%s", title, tmp);
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
	_GUIRectangleFill(0, y, width, y + 16, Color_Black);
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
	int idx = 0;
	int x = uiItem->x1;
	int y = uiItem->y1;

	memcpy(keyBuf, uiItem->text, TXTBUF_LEN);
	_Printfxy(x, y, keyBuf, Color_White);
	
	if((uiItem->ui.txtbox.dataLen == 12 || uiItem->ui.txtbox.dataLen == 16)
		&&(uiItem->text[0] >= '0' && uiItem->text[0] <= '9')){
		idx = strlen(uiItem->text);
		idx = (idx > 0 ? idx - 1 : idx);
	}
	x = x + idx * 8;
	_toxy(x, y + uiItem->height);
	_ShowCur();
	

	while(1){
		key = _ReadKey();

		if((key >= KEY_0 && key <= KEY_9) 
			||(key == KEY_DOT && uiItem->ui.txtbox.dotEnable && idx > 0 && idx < uiItem->ui.txtbox.dataLen -1)){

			if(uiItem->ui.txtbox.isClear && cleared == false && idx == 0){
				memset(keyBuf, 0x00, TXTBUF_LEN);
				_GUIRectangleFill(uiItem->x1, uiItem->y1, 
					(uiItem->x1 + uiItem->width), 
					(uiItem->y1 + uiItem->height), Color_White);
				cleared = true;
			}

			keyBuf[idx] = key;
			keyStr[0] = key;
			_Printfxy(x, y, keyStr, Color_White);
			if(idx != uiItem->ui.txtbox.dataLen -1){
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
		else if(key == KEY_RIGHT 
			&& ((keyBuf[idx] >= '0' && keyBuf[idx] <= '9') || (keyBuf[idx] == '.'))){
			if(idx != uiItem->ui.txtbox.dataLen - 1){
				idx++;
				x += 8;
			}
		}
		else if(key == KEY_DELETE){
			if(false == uiItem->ui.txtbox.isClear){
				keyBuf[idx] = '0';
				_Printfxy(x, y, "0", Color_White);
				idx--;
				x -= 8;
			}
			else if(idx == uiItem->ui.txtbox.dataLen -1 && keyBuf[idx] != 0x00){
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

	idx = *uiItem->ui.combox.currIdx;
	
	while(1){

		key = _GetKeyExt();

		if(idx != lastIdx){
			uiItem->text = uiItem->ui.combox.strs[idx];
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
			if(idx < uiItem->ui.combox.cnt -1){
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
	*uiItem->ui.combox.currIdx = idx;

	return key;
}

/*
* ��  ���������б���ͼ
* ��  ����lbx		- �б���ͼ�ṹָ��
*		  totalCnt	- ��¼����
*		  maxRow	- һҳ��ʾ��¼����
*		  fillStrsFunc	- ����б�Ļص�����
*		  title		- �б����
*		  strCnt	- �ַ�������
*		  ...		- �ַ����б�
* ����ֵ��void
*/
void ListBoxCreate(ListBox *lbx, uint8 x, uint8 y, uint8 maxCol, uint8 maxRow, uint16 totalCnt, FillListFunc fillStrsFunc, const char *title, uint32 strCnt, ...)
{
	uint16 i;
	va_list ap;
	char *ptr;

	lbx->x = x;		// Ĭ�� 0
	lbx->y = y;		// Ĭ�� 0
	lbx->maxCol = maxCol;	// Ĭ�� 20
	lbx->maxRow = maxRow;	// Ĭ�� 7
	lbx->totalCnt = totalCnt;
	lbx->fillStrsFunc = fillStrsFunc;
	lbx->title = title;
	lbx->currIdx = 0;

	// init value
	va_start(ap, strCnt);
	lbx->strCnt = strCnt;
	for(i = 0; i < lbx->strCnt; i++){
		ptr = va_arg(ap, char *);
		if(ptr == NULL){
			break;
		}
		lbx->str[i] = ptr;
	}
	
}

/*
* ��  ���������б���ͼ-��չ
* ��  ����lbx		- �б���ͼ�ṹָ��
* 		  x			- x ����
*		  y			- y ����
*		  maxCol	- һ����ʾ�ַ�����
*		  maxRow	- һҳ��ʾ��¼����
*		  totalCnt	- ��¼����
*		  fillStrsFunc	- ����б�Ļص�����
*		  title		- �б����
*		  strs		- �ַ����б�����
*		  strCnt	- �ַ�������
* ����ֵ��void
*/
void ListBoxCreateEx(ListBox *lbx, uint8 x, uint8 y, uint8 maxCol, uint8 maxRow, uint16 totalCnt, FillListFunc fillStrsFunc, const char *title, char **strs, uint8 strLen, uint8 strCnt)
{
	uint16 i;
	char *str = (char *)strs;

	lbx->x = x;		// Ĭ�� 0
	lbx->y = y;		// Ĭ�� 0
	lbx->maxCol = maxCol;	// Ĭ�� 20
	lbx->maxRow = maxRow;	// Ĭ�� 7
	lbx->totalCnt = totalCnt;
	lbx->fillStrsFunc = fillStrsFunc;
	lbx->title = title;
	lbx->strCnt = strCnt;
	lbx->currIdx = 0;

	for(i = 0; i < lbx->strCnt; i++){
		lbx->str[i] = str;
		str += strLen;
	}
}

/*
* ��  ������ȡ�б���ͼ��ǰѡ��
* ��  ����lbx	- �б���ͼ�ṹָ��
* ����ֵ��uint8  - �����˳�ʱ�İ���ֵ��ȷ�ϼ���ȡ����
*/
uint8 ShowListBox(ListBox *lbx)
{
	uint16 dstIndex, srcIndex, currIndex;
	uint8 key, i;
	int16 x1, y1, recX, recY, recX1, recY1, fillX, fillY, fillX1, fillY1;
	uint8 **lines = lbx->str;
	uint16 fillMax = (lbx->strCnt >= lbx->totalCnt ? lbx->totalCnt : (lbx->strCnt - lbx->strCnt % lbx->maxRow));
	uint16 fillCnt = 0;
	char title[21], temp[15];

	lbx->currIdx = (lbx->currIdx > 0 ? lbx->currIdx : 0);
	lbx->strCnt = (lbx->totalCnt < fillMax ? lbx->totalCnt : fillMax);
	lbx->strIdx = (lbx->currIdx % fillMax);
	lbx->dispStartIdx = lbx->strIdx - (lbx->strIdx % lbx->maxRow);

	
	recX = lbx->x - 4;
	recY = lbx->y - 4;
	x1 = lbx->x + lbx->maxCol * 8;
	y1 = lbx->y + (lbx->maxRow + 2) * 16;
	recX1 = x1 + 4;
	recY1 = y1 - 4;
	fillX = (recX <= 0 ? 0 : recX);
	fillY = (recY <= 0 ? 0 : recY);
	fillX1 = (recX1 >= 160 ? 160 : recX1);
	fillY1 = recY1 + 4;
	
	// ��/�¹�����ʾ   �� ��  �� ��
	while(1){

		_GUIRectangleFill(fillX, fillY, fillX1, fillY1, Color_White);	// �������
		_GUIRectangle(recX, recY, recX1, recY1, Color_Black);			// ���Ʒ���

		// ��ʾ���⣺lbx->title  n/m
		//-------------------------------------------------
		currIndex = (lbx->totalCnt == 0 ? 0 : lbx->currIdx + 1);
		sprintf(temp, "%d/%d", currIndex, lbx->totalCnt);
		StringPadLeft(temp, (lbx->maxCol - strlen(lbx->title) - 1), ' ');
		sprintf(title, "%s%s", lbx->title, temp);	
		_Printfxy(lbx->x, lbx->y, title, Color_White);
		_GUIHLine(lbx->x, lbx->y + 16 + 4, x1, Color_Black);	
		//--------------------------------------------��---
		for(i = 0; i < lbx->maxRow && (lbx->dispStartIdx + i) < lbx->strCnt; i++){
			_Printfxy(lbx->x, i * 16 + lbx->y + 16 + 8, lines[lbx->dispStartIdx + i], Color_White);
		}
		_Printfxy(lbx->x, (lbx->strIdx - lbx->dispStartIdx) * 16 + lbx->y + 16 + 8, lines[lbx->strIdx], Color_Black);
		//--------------------------------------------��---
		_GUIHLine(lbx->x, recY1, x1, Color_Black);	

		key = _ReadKey();

		if(key == KEY_CANCEL || key == KEY_ENTER){
			break;
		}

		if(key == KEY_UP){
			lbx->strIdx--;
			lbx->currIdx--;
		}
		else if(key == KEY_DOWN){
			lbx->strIdx++;
			lbx->currIdx++;
		}
		else if(key == KEY_LEFT){
			lbx->strIdx -= lbx->maxRow;
			lbx->currIdx -= lbx->maxRow;
		}
		else if(key == KEY_RIGHT){
			lbx->strIdx += lbx->maxRow;
			lbx->currIdx += lbx->maxRow;
		}
		else{
			continue;
		}

		// record index check
		lbx->isCircle = 1;
		if(lbx->currIdx < 0){
			lbx->currIdx = (lbx->isCircle ? lbx->totalCnt - 1 : 0);
		}
		else if(lbx->currIdx > lbx->totalCnt - 1){
			lbx->currIdx = (lbx->isCircle ? 0 : lbx->totalCnt - 1);
		}

		// disp index check 	and 	re-fill strs
		if(lbx->strIdx < 0){

			if(lbx->totalCnt > fillMax){
				fillCnt = (lbx->currIdx % fillMax) + 1;
				srcIndex = lbx->currIdx + 1 - fillCnt;
				dstIndex = 0;
				lbx->strCnt = fillCnt;
			}
			lbx->strIdx = lbx->strCnt - 1;
			
			if(lbx->fillStrsFunc != NULL && fillCnt > 0){
				(*lbx->fillStrsFunc)(lbx->str, dstIndex, srcIndex, fillCnt);
				fillCnt = 0;
			}
		}
		else if(lbx->strIdx > lbx->strCnt - 1){

			if(lbx->totalCnt > fillMax){
				fillCnt = (lbx->totalCnt - lbx->currIdx >= fillMax ? fillMax : (lbx->totalCnt - lbx->currIdx));
				srcIndex = lbx->currIdx;
				dstIndex = 0;
				lbx->strCnt = fillCnt;
			}
			lbx->strIdx = 0;
			
			if(lbx->fillStrsFunc != NULL && fillCnt > 0){
				(*lbx->fillStrsFunc)(lbx->str, dstIndex, srcIndex, fillCnt);
				fillCnt = 0;
			}
		}

		lbx->dispStartIdx = lbx->strIdx - (lbx->strIdx % lbx->maxRow);

	}

	return key;
}

/*
* ��  ����������ǩ
* ��  ����x, y		- UI���ǰ��ı�ǩ����ʼ����
*		 item		- UI�������
*		 title		- UI���ǰ��ı�ǩ
* ����ֵ��void
*/
void LableCreate(UI_Item *item, uint8 x, uint8 y, const char *title)
{
	item->x = x;
	item->y = y;
	item->title = title;
	item->x1 = x + strlen(title) * 8;
	item->y1 = y;
	item->text = NULL;
	item->type = UI_Lable;
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
	item->ui.txtbox.dataLen = maxLen;
	item->ui.txtbox.isClear = isClear;
	item->ui.txtbox.dotEnable = 0;		// Ĭ�ϲ���������'.' , ������������textbox���������øñ�־Ϊ1
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
		item->ui.combox.strs[i] = ptr;
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
	item->ui.combox.cnt = (uint8)i;
	item->ui.combox.currIdx = currIdx;
}

/*
* ��  ������ʾUI���� 
* ��  ����uiList	- UI����б�ṹ
*		 *itemNo	- ��ʼ��ʱ��λ���ĸ�������
* ����ֵ��uint8  - �����˳�ʱ�İ���ֵ��ȷ�ϼ���ȡ����
*/
uint8 ShowUI(UI_ItemList uiList, uint8 *itemNo)
{
	const char * ZeroAddr = "0000000000000000";
	uint8 key, x, y;
	uint8 i;
	UI_Item *ptr;

	for(i = 0; i < uiList.cnt; i++){
		ptr = &uiList.items[i];
		_Printfxy(ptr->x, ptr->y, ptr->title, Color_White);
		_Printfxy(ptr->x1, ptr->y1, ptr->text, Color_White);

		if(ptr->type == UI_CombBox){
			ptr->text = ptr->ui.combox.strs[*ptr->ui.combox.currIdx];
			x = ptr->x1 + (ptr->width - strlen(ptr->text)*8)/2;
			y = ptr->y1;
			_Printfxy(x, y, ptr->text, Color_White);
			_Printfxy(ptr->x1, ptr->y1, "<<", Color_White);
			_Printfxy(ptr->x1 + ptr->width - 16, ptr->y1, ">>", Color_White);
		}
	}

	(*itemNo) = ((*itemNo) > uiList.cnt -1 ? 0 : (*itemNo));

	if(uiList.items[(*itemNo)].type == UI_Lable){
		key = KEY_DOWN;
	}

	while(1){

		ptr = &uiList.items[(*itemNo)];

		if(ptr->type == UI_TxtBox){
			do{
				//��������
				key = GetInputNumStr(ptr);

				if( ptr->text[0] >= '0' && ptr->text[0] <= '9'){
					if((ptr->ui.txtbox.dataLen == 12 || ptr->ui.txtbox.dataLen == 16)
						&& (key == KEY_ENTER || key == KEY_UP || key == KEY_DOWN)){
						_leftspace(ptr->text, ptr->ui.txtbox.dataLen, '0');
						if(strncmp(ZeroAddr, ptr->text, ptr->ui.txtbox.dataLen) == 0){
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
		else{	// UI_Lable
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
	uint8 *pcurrLine, *pnextLine, *pr;

	pcurrLine = buf;
	pr = buf;

	while(*pr != 0x00){
		if((x/8 + col) >= 20){
			if(col == 21){
				pr -= 2;
			}
			else{
				pr = (*pr == '\n' ? pr + 1 : pr);
			}
			pnextLine = pr;
			
			lines[lineCnt++] = pcurrLine;
			pcurrLine = pnextLine;

			col = 0;
			x = 0;
		}
		else if(*pr == '\n'){
			pr++;
			pnextLine = pr;

			lines[lineCnt++] = pcurrLine;
			pcurrLine = pnextLine;

			col = 0;
			x = 0;
		}

		if(*pr == 0x00){
			break;
		}
		else if(*(pr) > 0x80){
			pr += 2;
			col += 2;
		}
		else{
			pr++;
			col++;
		}
		
	}

	// last line 
	if(pcurrLine != 0x00){
		lines[lineCnt++] = pcurrLine;
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
	uint8 lineCnt = 0, col = 0; 
	uint8 *pcurrLine, *pnextLine, *pr;

	pcurrLine = buf;
	pr = buf;

	while(*pr != 0x00 && lineCnt < maxLines){
		if((x/8 + col) >= 20){
			if(col == 21){
				pr -= 2;
				col -= 2;
			}
			else{	// col == 20
				pr = (*pr == '\n' ? pr + 1 : pr);
			}
			pnextLine = pr;
			
			memcpy(dispLine, pcurrLine, col);
			dispLine[col] = 0x00;
			_Printfxy(x, y, dispLine, Color_White);
			lineCnt++;
			pcurrLine = pnextLine;

			col = 0;
			x = 0;
			y += 16;
		}
		else if(*pr == '\n'){
			pr++;
			pnextLine = pr;

			memcpy(dispLine, pcurrLine, col);
			dispLine[col] = 0x00;
			_Printfxy(x, y, dispLine, Color_White);
			lineCnt++;
			pcurrLine = pnextLine;

			col = 0;
			x = 0;
			y += 16;
		}

		if(*pr == 0x00){
			break;
		}
		else if(*pr > 0x80){
			pr += 2;
			col += 2;
		}
		else{
			pr++;
			col++;
		}
	}

	// last line 
	if(pcurrLine != 0x00 && lineCnt < maxLines){
		memcpy(dispLine, pcurrLine, col);
		dispLine[col] = 0x00;
		_Printfxy(x, y, dispLine, Color_White);
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
* ��  ������ʾ�ɹ������ַ���	- ���Զ����У����� \n �� ������Ļ�߽�ʱ
* ��  ����strBuf	- �ַ���������
*		 lineStep	- �����¼�ʱ���������������7��
* ����ֵ��uint8  - �����˳�ʱ�İ���ֵ��ȷ�ϼ���ȡ����
*/
uint8 ShowScrollStr(char *strBuf, uint8 lineStep)
{
	const uint8 lineMax = 7;
	int8 lineCnt = 0, currLine = 0;
	uint8 *lines[150], key;

	// lineStep check
	if(lineStep > lineMax){
		lineStep = lineMax;
	}

	lineCnt = GetPrintLines(0, strBuf, lines);

	_GUIHLine(0, 1*16 + 4, 160, Color_Black);	
	/*---------------------------------------------*/
	PrintfXyMultiLine(0, 1*16 + 8, lines[0], lineMax);	
	//----------------------------------------------
	_GUIHLine(0, 9*16 - 4, 160, Color_Black);
	
	// ��/�¹�����ʾ   �� ��  �� ��
	while(1){

		if(lineCnt > lineMax){
			if(currLine < lineCnt - lineMax){
				PrintXyTriangle(9*16 + 8, 8*16 + 8, 1);		// ��
			}else{
				_GUIRectangleFill(9*16 + 8, 8*16 + 8, 160, 8*16 + 12, Color_White);
			}

			if(currLine > 0){
				PrintXyTriangle(9*16 + 8, 1*16 + 4, 0);		// ��
			}else{
				_GUIRectangleFill(9*16 + 8, 1*16 + 5, 160, 1*16 + 8, Color_White);
			}
		}

		key = _ReadKey();

		if(key == KEY_CANCEL || key == KEY_ENTER){
			break;
		}
		else if(key == KEY_UP && lineCnt > lineMax){
			currLine -= lineStep;
			if(currLine < 0){
				currLine = 0;
			}
		}
		else if(key == KEY_DOWN && lineCnt > lineMax){
			currLine += lineStep;
			if(currLine > lineCnt - lineMax){
				currLine = lineCnt - lineMax;
			}
		}
		else{
			continue;
		}

		_GUIRectangleFill(0, 1*16 + 8, 160, 8*16 + 8, Color_White);
		PrintfXyMultiLine(0, 1*16 + 8, lines[currLine], lineMax);
	}

	return key;
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
	int srcStrLen, i = 0;
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
* ��  �����ַ���β���ü�
* ��  ����srcStr - �ַ�����ʼ��ַ
		  trimChar - �ü����ַ�
* ����ֵ��int �ü�����ַ�������
*/
int StringTrimEnd(const char * srcStr, char trimChar)
{
	int srcStrLen, i = 0;
	char *pr = srcStr;

	srcStrLen = strlen(srcStr);
	if(srcStrLen == 0 || trimChar == 0x00){
		return 0;
	}
	
	for(i = srcStrLen - 1; i >= 0; i--){
		if(pr[i] != trimChar){
			break;
		}
	}
	pr[i + 1] = 0x00;

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
* ��  ����BCD�� --ת��Ϊ--> 10������
*/
uint8 BcdToDec(uint8 bcd)
{
	return (uint8)(bcd - (bcd >> 4) * 6);
}

/*
* ��  ����10������ --ת��Ϊ--> BCD��
*/
uint8 DecToBcd(uint8 dec)
{
	return (uint8)(dec + (dec / 10) * 6);
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