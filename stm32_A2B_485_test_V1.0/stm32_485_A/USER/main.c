#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"	 
#include "rs485.h"


extern char rflag;
extern unsigned char rbuf;

int main(void)
{	 
	char sdata=65;
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(9600);	 	//���ڳ�ʼ��Ϊ9600
	LED_Init();			     //LED�˿ڳ�ʼ��

	RS485_Init(9600);	//��ʼ��RS485

	while(1)
	{
		
		sdata++;
		if(sdata>90)
			sdata=65;
		RS485_Send_Byte(sdata);
		delay_ms(900);
		delay_ms(900);
		LED0=~LED0;
		
	}
}





























































//ALIENTEKս��STM32������ʵ��24
//485 ʵ��  
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾
