#include <HJLIB.H>
#include "string.h"
//#include "dbf.h"
#include "stdio.h"

// --------------------------------  ȫ�ֱ���  -----------------------------------------
char Screenbuff[160*(160/3+1)*2]; 
uint8 ComBuf[1080];
uint8 TmpBuf[1080];
uint32 rxLen, txLen;
typedef enum{
	ModType_MainNode,
	ModType_PowerSub,
	ModType_WaterSub
}ModuleType;
ModuleType currModType;


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
* ��������showProgressBar
* ��  ������ʾ������
* ��  ����y - ������y���꣬����ʾ��(0,y)λ�ã��̶����Ϊ160���̶��߶�Ϊ16,��ɫ���
		  maxValue - ���������ֵ
		  currValue - ��������ǰֵ
* ����ֵ��int ����-1: δ�ҵ��� ����/����0 : Ŀ��������Դ�����е���ʼ����
*/
void showProgressBar(uint8 y, uint32 maxValue, uint32 currValue)
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
* ����ֵ��char * ���������ַ���
*/
char * StringPadLeft(const char * srcStr, int totalLen, char padChar)
{
	uint32 srcStrLen, i = 0;
	char * newStr = NULL;

	srcStrLen = strlen(srcStr);
	if(srcStrLen >= totalLen){
		return srcStr;
	}
	if(padChar == 0x00){
		return NULL;
	}
	
	newStr = (char *)malloc(totalLen + 1);
	for(i = 0; i < (totalLen - srcStrLen); i++){
		newStr[i] = padChar;
	}
	while(i < totalLen){
		newStr[i] = srcStr[i];
		i++;
	}
	newStr[i] = 0x00;

	return newStr;
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

char * GetStringHexFromBytes(uint8 bytes[], int iStart, int iLength, char strSeparate, uint8 Reverse)
{
	char * strHex = "";
	uint8 aByte;
	int iLoop;
   
	if(bytes == NULL || iStart + iLength > sizeof(bytes) || iStart < 0){
		return strHex;
	}

	if(strSeparate != 0){
		strHex = (char *)malloc(iLength * 3);
	}else{
		strHex = (char *)malloc(iLength * 2);
	}
	
	for (iLoop = 0; iLoop < iLength; iLoop++)
	{
		if (Reverse){
			aByte = bytes[iStart + iLength - 1 - iLoop];
		}
		else{
			aByte = bytes[iStart + iLoop];
		}
		strHex[iLoop] = HexToChar(aByte >> 4);
		strHex[iLoop + 1] = HexToChar(aByte & 0x0F);
		if(strSeparate != 0){
			strHex[iLoop + 2] = strSeparate;
		}
	}

	return strHex;
}

int GetBytesFromStringHex(uint8 bytes[], int iStart, const char * strHex, char strSeparate, uint8 Reverse)
{
	int index = 0, bytesLen, strHexLen;
	uint8 aByte;
	int iLoop;
  
	strHexLen = strlen(strHex);
	if(strSeparate != 0){
		bytesLen = (strHexLen + 1) / 3;
	}else{
		bytesLen = (strHexLen + 1) / 2;
	}

	if(bytes == NULL || iStart + bytesLen > sizeof(bytes) || iStart < 0){
		return 0;
	}

	for (iLoop = 0; iLoop < strHexLen; iLoop++)
	{
		aByte = (CharToHex(strHex[iLoop]) << 4) + (CharToHex(strHex[iLoop + 1]) & 0x0F);
		iLoop += 2;

		if (Reverse){
			bytes[iStart + bytesLen - 1 - index] = aByte;
		}
		else{
			bytes[iStart + index] = aByte;
		}
	
		if(strSeparate != 0){
			iLoop++;
		}
		index++;
	}

	return index;
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
	uint8 inputLen;
	int index;
	char * zeroAddr = "00000000000000";	// 0 ��ַ��14�ַ�
	
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
	inputBox.top = 1 * 16;
	inputBox.left = 5 * 16;
	inputBox.width = 5 * 16;
	inputBox.hight = 16;
	inputBox.type = 1;	// ����
	inputBox.keyUpDown = 1;
	inputBox.IsClear = 0;

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
			_Printfxy(0, 0, "���>>��ȡ����汾", 0);
			_Printfxy(0, 1*16, "����ַ:", 0);
			_Printfxy(0, 9*16, "����            ȷ��", 0);

			while(1)
			{
				key = _InputBox(&inputBox);
				if (key == KEY_CANCEL)
					break;

				inputLen = strlen(inputBox.context);
				if(inputBox.datelen == 0 || strncmp(zeroAddr, inputBox.context, inputLen) == 0){
					_Printfxy(0, 3*16, "����������Ч��ַ", 0);
				}else{
					_Printfxy(0, 3*16, "                ", 0);
				}
				inputBox.context = StringPadLeft(inputBox.context, 12, '0');

				GetBytesFromStringHex(TmpBuf, 0, inputBox.context, 0, 1);
				_Printfxy(0, 3*16, &TmpBuf[0], 0);
				
				txLen = 0;
				ComBuf[txLen++] = 0xAA;
				ComBuf[txLen++] = 0xBB;
				ComBuf[txLen++] = 0x01;
				ComBuf[txLen++] = 0x07;
				ComBuf[txLen++] = 0xCC;
				_GetComStr(TmpBuf, 1024, 10);	// clear , 100ms timeout
				_SendComStr(ComBuf, txLen);
				_Printfxy(0, 2*16, "�ѷ���...        ", 0);

				sprintf(ComBuf, "�汾��Ϣ:");
				rxLen = _GetComStr(&ComBuf[9], 50, 50);	// recv , 500ms timeout
				if(rxLen < 30 || strncmp(&ComBuf[9], "SRWF-", 5) != 0)
				{
					_Printfxy(0, 2*16, "���ճ�ʱ       ", 0);
					break;
				}
				_Printfxy(0, 5*16, &ComBuf[0], 0);
				_Printfxy(0, 6*16, &ComBuf[20], 0);
				_Printfxy(0, 7*16, &ComBuf[40], 0);

				_ReadKey();
			}
			break;

		case 2:		// " ��ȡ�ڵ����� "
			_GUIRectangleFill(0, 5 * 16, 160, 9 * 16, 0);

			//sprintf(TmpBuf, "�ļ�: %s\0", fileName);
			_Printfxy(0, 5*16, &TmpBuf[0], 0);
			_Printfxy(0, 6*16, &TmpBuf[20], 0);

			break;
			
		case 3:		// " 645-07���� ";
			_GUIRectangleFill(0, 5 * 16, 160, 9 * 16, 0);
			

			//sprintf(tmp, "�ܰ���: %d\0", totalCnt);
			//_Printfxy(0, 5*16, &tmp[0], 0);

			
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
			txLen = 0;
			ComBuf[txLen++] = 0xAA;
			ComBuf[txLen++] = 0xBB;
			ComBuf[txLen++] = 0x01;
			ComBuf[txLen++] = 0x07;
			ComBuf[txLen++] = 0xCC;
			_GetComStr(TmpBuf, 1024, 10);	// clear , 100ms timeout
			_SendComStr(ComBuf, txLen);
			_Printfxy(0, 5*16, "��ѯ��...", 0);

			sprintf(ComBuf, "��ǰ�汾:");
			rxLen = _GetComStr(&ComBuf[9], 50, 50);	// recv , 500ms timeout
			if(rxLen < 30 || strncmp(&ComBuf[9], "SRWF-", 5) != 0)
			{
				_Printfxy(0, 5*16, "���ճ�ʱ", 0);
				break;
			}
			_Printfxy(0, 5*16, &ComBuf[0], 0);
			_Printfxy(0, 6*16, &ComBuf[20], 0);
			_Printfxy(0, 7*16, &ComBuf[40], 0);
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
			_Fread(ComBuf, 1024, fileHdl);
			_Fclose(fileHdl);
				
			index = IndexOf(ComBuf, 1024, "SRWF-", 5, 512, 512);
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

			showProgressBar(7*16+8, totalCnt, sendCnt);

			// ��������
			while(1){

				if(tryCnt > 3 || sendCnt >= totalCnt){
					break;
				}
				
				txLen = _Fread(ComBuf, 1024, fileHdl);
				_GetComStr(TmpBuf, 1024, 1);		// clear , 100ms timeout
				_SendComStr(ComBuf, txLen);

				sprintf(tmp, "������: %d   \0",sendCnt + 1);
				_Printfxy(0, 6*16, &tmp[0], 0);
				if(tryCnt > 0){
					sprintf(tmp, "����%d \0",tryCnt);
					_Printfxy(6*16, 6*16, &tmp[0], 0);
				}
				tryCnt++;

				rxLen = _GetComStr(&ComBuf[9], 50, 1);	// recv , 500ms timeout
				if(rxLen < 10){
				//	continue;
				}

				sendCnt++;
				tryCnt = 0;
				showProgressBar(7*16+8, totalCnt, sendCnt);

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

