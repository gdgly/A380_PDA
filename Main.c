#include <HJLIB.H>
#include "string.h"
//#include "dbf.h"
#include "stdio.h"

// --------------------------------  ȫ�ֱ���  -----------------------------------------
char Screenbuff[160*(160/3+1)*2]; 


// --------------------------------  �������ڵ�ͨ��  -----------------------------------------

void ElectricMainNodeFunc(void)
{
	
}


// --------------------------------  �����ӽڵ�ͨ��  -----------------------------------------

void ElectricSubNodeFunc(void)
{
	
}

// --------------------------------  ˮ���ӽڵ�ͨ��  -----------------------------------------

void WaterMainNodeFunc(void)
{
	
}

// --------------------------------  ͸��ģ������  -----------------------------------------
void TransParentModuleFunc(void)
{
	uint8 key, menuItemNo;
	_ProGressBar progBar;
	_GuiLisStruEx menuList;
	char *fileName;
	char tmp[70];

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

	// ������
	progBar.caption = "��������";
	progBar.left = 0;
	progBar.top = 7*16;
	progBar.width = 10*16;
	progBar.hight = 16;

	while(1){

		menuItemNo = _ListEx(&menuList);

		if (menuItemNo == 0) 
			return;

		switch(menuItemNo){
		case 1:	// " �鿴��ǰ�汾 ";
			_GUIRectangleFill(0, 5 * 16, 160, 9 * 16, 0);
			sprintf(tmp, "��ǰ�汾: %s\0", "SRWF-4E88M-SG-BXD17-20190401-Vsp0.33");
			_Printfxy(0, 5*16, &tmp[0], 0);
			_Printfxy(0, 6*16, &tmp[20], 0);
			_Printfxy(0, 7*16, &tmp[40], 0);
			break;

		case 2:	// " �������ļ� "
			// ѡ�������ļ�
			_GUIRectangleFill(0, 5 * 16, 160, 9 * 16, 0);
			sprintf(tmp, "�ļ�: %s\0", "SRWF-4E88-APP-20190401-Vsp0.33.bin");
			_Printfxy(0, 5*16, &tmp[0], 0);
			_Printfxy(0, 6*16, &tmp[20], 0);
			sprintf(tmp, "��С:%dK,�ܰ���:%d\0", 30, 300);
			_Printfxy(0, 7*16, &tmp[0], 0);
			// ����·��
			//fileName = 

			break;
			
		case 3:	// " ��ʼ���� ";
		
			// ��ʼ��
			//progBar.step = 1;	
			progBar.min = 0;
			progBar.max = 300; 	// �ܰ���
			_GUIRectangleFill(0, 5 * 16, 160, 9 * 16, 0);
			sprintf(tmp, "�ܰ�����%d\0", 300);
			_Printfxy(0, 5*16, &tmp[0], 0);
			sprintf(tmp, "�ѷ��ͣ�%d\0",233);
			_Printfxy(0, 6*16, &tmp[0], 0);

			_Printfxy(0, 9*16, "״̬: ������...", 0);

			_GUIRectangle(0, 7*16, 160, 8*16, 1);

			// ���½���
			//_CreateProgressBar(&progBar);
			_ReadKey();

			progBar.value = 30;
			//_IncrementProgressBar(&progBar);
			_GUIRectangleFill(0, 7 * 16, 30, 8 * 16, 1);
			_ReadKey();

			progBar.value = 150;
			//_IncrementProgressBar(&progBar);
			_GUIRectangleFill(0, 7 * 16, 80, 8 * 16, 1);
			_ReadKey();

			progBar.value = 250;
			//_IncrementProgressBar(&progBar);
			_GUIRectangleFill(0, 7 * 16, 130, 8 * 16, 1);
			_ReadKey();

			progBar.value = 300 - 10;
			//_IncrementProgressBar(&progBar);
			_GUIRectangleFill(0, 7 * 16, 160, 8 * 16, 1);

			// �������
			_Printfxy(0, 9*16, "״̬: �������  ", 0);
			break;
		}
		
		key = _ReadKey();
		
		switch(key){
			
		case KEY_CANCEL:
			return;
			
		case KEY_1:
		case KEY_2:
		case KEY_3:
			menuList.defbar = (key - '0');
			break;

		case KEY_LEFT:
		case KEY_UP:
			menuList.defbar = (menuItemNo <= 1 ? 3 : menuItemNo - 1);
			break;

		case KEY_RIGHT:
		case KEY_DOWN:
		default:
			menuList.defbar = (menuItemNo >= 3 ? 1 : menuItemNo + 1);
			break;
		}

	}
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

	_Menu(&MainMenu);
}

