#ifndef __RS485_H
#define __RS485_H			 
#include "sys.h"	 								  

extern unsigned char rbuf;


//模式控制
#define RS485_TX_EN		PGout(9)	//485模式控制.0,接收;1,发送.

void RS485_Init(u32 bound);
void RS485_Send_Byte(u8 buf);

#endif	   






































































//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//RS485驱动 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/9
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
