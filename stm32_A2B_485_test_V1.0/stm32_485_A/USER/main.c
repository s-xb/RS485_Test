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
	delay_init();	    	 //延时函数初始化	  
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(9600);	 	//串口初始化为9600
	LED_Init();			     //LED端口初始化

	RS485_Init(9600);	//初始化RS485

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





























































//ALIENTEK战舰STM32开发板实验24
//485 实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司
