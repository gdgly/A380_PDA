#include <HJLIB.H>
#include "string.h"
//#include "dbf.h"
#include "stdio.h"

// --------------------------------  全局变量  -----------------------------------------
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


//---------------------------------  通用方法  -------------------------------------
/*
* 函数名：IndexOf
* 描  述：在数组中查找，可指定查找的起始位置和范围
* 参  数：srcArray - 源数组地址， srcLen - 源数组长度
		  dstBytes - 目的数组地址， dstLen - 目的数组长度 
		  startIndex - 源数组查找的起始位置，offset - 查找的范围
* 返回值：int 等于-1: 未找到， 大于/等于0 : 目的数组在源数组中的起始索引
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
* 函数名：showProgressBar
* 描  述：显示进度条
* 参  数：y - 进度条y坐标，将显示在(0,y)位置，固定宽度为160，固定高度为16,黑色填充
		  maxValue - 进度条最大值
		  currValue - 进度条当前值
* 返回值：int 等于-1: 未找到， 大于/等于0 : 目的数组在源数组中的起始索引
*/
void showProgressBar(uint8 y, uint32 maxValue, uint32 currValue)
{
	uint32 width = (currValue >= maxValue? 160 : 160*currValue/maxValue);
	_GUIRectangleFill(0, y, width, y + 16, 1);
}

// --------------------------------  参数配置/读取  -----------------------------------------



// --------------------------------  现场调试  -----------------------------------------



// --------------------------------  电力主节点通信  -----------------------------------------

void ElectricMainNodeFunc(void)
{
	
}


// --------------------------------  电力子节点通信  -----------------------------------------

void ElectricSubNodeFunc(void)
{
	uint8 key, menuItemNo, tryCnt = 0;
	_GuiLisStruEx menuList;
	int index;
	
	_ClearScreen();

	// 菜单
	menuList.title = ">> 电力子节点通信 ";
	menuList.no = 3;
	menuList.MaxNum = 3;
	menuList.isRt = 0;
	menuList.x = 0;
	menuList.y = 0;
	menuList.with = 10 * 16;
	menuList.str[0] = "  1. 读取软件版本";
	menuList.str[1] = "  2. 读取节点配置";
	menuList.str[2] = "  3. 645-07抄表";
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
		case 1:		// " 读取软件版本 ";
			_GUIRectangleFill(0, 5 * 16, 160, 9 * 16, 0);
			txLen = 0;
			ComBuf[txLen++] = 0xAA;
			ComBuf[txLen++] = 0xBB;
			ComBuf[txLen++] = 0x01;
			ComBuf[txLen++] = 0x07;
			ComBuf[txLen++] = 0xCC;
			_GetComStr(TmpBuf, 1024, 10);	// clear , 100ms timeout
			_SendComStr(ComBuf, txLen);
			_Printfxy(0, 5*16, "查询中...", 0);

			sprintf(ComBuf, "当前版本:");
			rxLen = _GetComStr(&ComBuf[9], 50, 50);	// recv , 500ms timeout
			if(rxLen < 30 || strncmp(&ComBuf[9], "SRWF-", 5) != 0)
			{
				_Printfxy(0, 5*16, "接收超时", 0);
				break;
			}
			_Printfxy(0, 5*16, &ComBuf[0], 0);
			_Printfxy(0, 6*16, &ComBuf[20], 0);
			_Printfxy(0, 7*16, &ComBuf[40], 0);
			break;

		case 2:		// " 读取节点配置 "
			_GUIRectangleFill(0, 5 * 16, 160, 9 * 16, 0);

			sprintf(TmpBuf, "文件: %s\0", fileName);
			_Printfxy(0, 5*16, &TmpBuf[0], 0);
			_Printfxy(0, 6*16, &TmpBuf[20], 0);

			break;
			
		case 3:		// " 645-07抄表 ";
			_GUIRectangleFill(0, 5 * 16, 160, 9 * 16, 0);
			

			sprintf(tmp, "总包数: %d\0", totalCnt);
			_Printfxy(0, 5*16, &tmp[0], 0);

			
			break;

			default: 
				break;
		}
	}

	_CloseCom();
}

// --------------------------------  水力子节点通信  -----------------------------------------

void WaterMainNodeFunc(void)
{
	
}

// --------------------------------  透传模块设置  -----------------------------------------
void TransParentModuleFunc(void)
{
	uint8 key, menuItemNo, tryCnt = 0;
	_GuiLisStruEx menuList;
	char *fileName = NULL;
	char tmp[70];
	int fileHdl, fileLen, totalCnt, sendCnt;
	int index;
	
	_ClearScreen();

	// 菜单
	menuList.title = ">> 透传模块升级 ";
	menuList.no = 3;
	menuList.MaxNum = 3;
	menuList.isRt = 0;
	menuList.x = 0;
	menuList.y = 0;
	menuList.with = 10 * 16;
	menuList.str[0] = "  1. 查看当前版本";
	menuList.str[1] = "  2. 打开升级文件";
	menuList.str[2] = "  3. 开始升级";
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
		case 1:	// " 查看当前版本 ";
			_GUIRectangleFill(0, 5 * 16, 160, 9 * 16, 0);
			txLen = 0;
			ComBuf[txLen++] = 0xAA;
			ComBuf[txLen++] = 0xBB;
			ComBuf[txLen++] = 0x01;
			ComBuf[txLen++] = 0x07;
			ComBuf[txLen++] = 0xCC;
			_GetComStr(TmpBuf, 1024, 10);	// clear , 100ms timeout
			_SendComStr(ComBuf, txLen);
			_Printfxy(0, 5*16, "查询中...", 0);

			sprintf(ComBuf, "当前版本:");
			rxLen = _GetComStr(&ComBuf[9], 50, 50);	// recv , 500ms timeout
			if(rxLen < 30 || strncmp(&ComBuf[9], "SRWF-", 5) != 0)
			{
				_Printfxy(0, 5*16, "接收超时", 0);
				break;
			}
			_Printfxy(0, 5*16, &ComBuf[0], 0);
			_Printfxy(0, 6*16, &ComBuf[20], 0);
			_Printfxy(0, 7*16, &ComBuf[40], 0);
			break;

		case 2:	// " 打开升级文件 "
			_GUIRectangleFill(0, 5 * 16, 160, 9 * 16, 0);

			_SaveScreenToBuff(Screenbuff);
			_ClearScreen();
			fileName = _GetFileList("选|\n择|\n升|\n级|\n文|\n件|\n  |\n  |\n", "", "");
			_ClearScreen();
			_RestoreBuffToScreen(Screenbuff);

			if (fileName == NULL){
				break;
			}
			
			sprintf(tmp, "文件: %s\0", fileName);
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
				_Printfxy(0, 7*16, "不是4E88-APP文件", 0);
				fileName = NULL;
			}
			else{
				sprintf(tmp, "大小:%dK,总包数:%d\0", fileLen/1024, totalCnt);
				_Printfxy(0, 7*16, &tmp[0], 0);
			}
			break;
			
		case 3:	// " 开始升级 ";
			_GUIRectangleFill(0, 5 * 16, 160, 9 * 16, 0);
			totalCnt = 200;
			
			// 初始化
			if (fileName == NULL){
				_Printfxy(0, 5*16, "请先选择升级文件", 0);
				break;
			}
			fileHdl = _Fopen(fileName, "R");
			sendCnt = 0;

			sprintf(tmp, "总包数: %d\0", totalCnt);
			_Printfxy(0, 5*16, &tmp[0], 0);
			sprintf(tmp, "正发送: %d   \0",sendCnt);
			_Printfxy(0, 6*16, &tmp[0], 0);
			_Printfxy(0, 9*16, "状态: 升级中...", 0);

			showProgressBar(7*16+8, totalCnt, sendCnt);

			// 升级进度
			while(1){

				if(tryCnt > 3 || sendCnt >= totalCnt){
					break;
				}
				
				txLen = _Fread(ComBuf, 1024, fileHdl);
				_GetComStr(TmpBuf, 1024, 1);		// clear , 100ms timeout
				_SendComStr(ComBuf, txLen);

				sprintf(tmp, "正发送: %d   \0",sendCnt + 1);
				_Printfxy(0, 6*16, &tmp[0], 0);
				if(tryCnt > 0){
					sprintf(tmp, "重试%d \0",tryCnt);
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

			// 升级完成
			if(tryCnt > 3){
				_Printfxy(0, 9*16, "状态: 升级失败  ", 0);
			}else{
				_Printfxy(0, 9*16, "状态: 升级完成  ", 0);
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


// --------------------------------   主函数   -----------------------------------------------
int main(void)
{
	_GuiMenuStru MainMenu;
	
	MainMenu.left=0;
	MainMenu.top=0;
	MainMenu.no=4;
	MainMenu.title= "  桑锐手持机  ";
	MainMenu.str[0]=" 电力主节点通信 ";
	MainMenu.str[1]=" 电力子节点通信 ";
	MainMenu.str[2]=" 水力子节点通信 ";
	MainMenu.str[3]=" 透传模块升级 ";
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

