#ifndef PRINTER_H
#define PRINTER_H
extern void _PrintOpen(void);
extern void _PrintClose(void);
extern uint16 _PrintStr(char *,uint16);
extern uint16 _PrintCmd(char *,uint16);
extern uint16 _PrintMove(int);
extern uint16 _PrintMovePaper(int);
extern void _PrintTestPage(void);

/**********************************
 ���ڱ�
 uint8 s =0	 ��⵽�ڱ�Ž���
 	   >0 ����S���δ��⵽�ڱ��򷵻�-1
 uint8 nNum = 0 ��⵽�ڱ꼴ֹͣ��ֽ
!=0 ��⵽�ڱ꣬Խ���ڱ�� ֹͣ��ֽ

**********************************/

int8 _PrintCheckTag(uint8 s,uint8 nNum);
#endif