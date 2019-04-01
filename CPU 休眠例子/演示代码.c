#include <HJLIB.H>
#define PCON            (*((volatile unsigned char *) 0xE01FC0C0)) //CPU ���ʿ���


#define T0IR            (*((volatile unsigned long *) 0xE0004000))
#define T0TCR           (*((volatile unsigned long *) 0xE0004004))
#define T0TC            (*((volatile unsigned long *) 0xE0004008))
#define T0PR            (*((volatile unsigned long *) 0xE000400C))
#define T0PC            (*((volatile unsigned long *) 0xE0004010))
#define T0MCR           (*((volatile unsigned long *) 0xE0004014))
#define T0MR0           (*((volatile unsigned long *) 0xE0004018))
#define T0MR1           (*((volatile unsigned long *) 0xE000401C))
#define T0MR2           (*((volatile unsigned long *) 0xE0004020))
#define T0MR3           (*((volatile unsigned long *) 0xE0004024))
#define T0CCR           (*((volatile unsigned long *) 0xE0004028))
#define T0CR0           (*((volatile unsigned long *) 0xE000402C))
#define T0CR1           (*((volatile unsigned long *) 0xE0004030))
#define T0CR2           (*((volatile unsigned long *) 0xE0004034))
#define T0CR3           (*((volatile unsigned long *) 0xE0004038))
#define T0EMR           (*((volatile unsigned long *) 0xE000403C))
#define VICIntEnClr     (*((volatile unsigned long *) 0xFFFFF014))
 
#define VICVectAddr  (*((volatile unsigned long *)   0xFFFFF030))
#define VICIntEnable   (*((volatile unsigned long *)  0xFFFFF010))
#define VICVectAddr4    (*((volatile unsigned long *) 0xFFFFF110))
#define VICVectCntl4    (*((volatile unsigned long *) 0xFFFFF210))
 
 

void __irq Timer0_Exception (void) 
{     
   T0IR =0x01;  
  //�жϳ���
//  _sprintfxy(0,0,0 ,"��ʱ��:%d   ",i );
   VICVectAddr = 0;                       
}

 void Timer0Open(void)
{
	 VICVectCntl4 = (0x20 | 4);
     VICVectAddr4 =(uint32) Timer0_Exception;
     VICIntEnable=VICIntEnable | 1 << 4;
}

void Timer0CLose(void)
{
	  T0TCR = 0x00;
      VICIntEnable=VICIntEnable | 1 << 4;
	  VICIntEnClr=1 << 4;
}

#define Fosc            14745600                     
#define Fcclk           (Fosc * 5)                  
#define Fcco            (Fcclk * 4)                  
#define Fpclk           (Fcclk / 4) * 1              
 
void Timer0Set(uint32 val)
{
	  T0TCR = 0x00;
	  T0TC = 0x00;	 
	  T0PR = 00;                    	  
      T0EMR = 0x000;
	  T0MCR = 0x003;                      
      T0MR0 =val * (Fpclk / 1000-1);              //1ms
	  T0TCR = 0x01;  
}


int main(void)
{
	char buf[5]={0};

	int i=0;   
	int j,k;	
	_ClearScreen();
	_Printfxy( 56,0,"������",1);
	_ReadKey();

	_ComSetTran(1);
	_ComSet((uint8 *)"115200,N,8,1",1);
	_DisableArmIrqInt(); //����֮ǰҪ�ر�ʱ���ж�
	Timer0Open();       //��ʼ����ʱ  
	Timer0Set(5000);    //���ö�ʱ������ʱ�䣬Ϊ5��
	j=_GetSec(); 
	PCON = 0x01;         //��CPUΪ����
	// ����ճ����룬д���պ���������н������ݣ�CPU ��ֱ������
	//
	//
	_GetComStr(buf,1,50);
	_sprintfxy(0,112,0,"�յ���%.2x  %s",buf[0],buf);
	//
	//

	//���ô���κεĽ��գ��ɶ�ʱ�����ѣ�����5�����

	_EnableArmIrqInt();//�������Ҫ��ʱ���жϣ���Ȼ������Ч��
	Timer0CLose();
	k=_GetSec();

	_sprintfxy(0,0,0 ,"��ʱ��:%d  %d %d ",i,j,k);
	_CloseCom();
	_ReadKey();
	return 0;
}