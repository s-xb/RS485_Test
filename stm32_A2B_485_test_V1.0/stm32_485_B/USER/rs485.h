#ifndef _RS485_H_
#define _RS485_H_
#include "sys.h"


#define RS485_TX_EN PAout(0) 
void rs485Init(int bound);
void RS485_Send_Byte(u8 buf);

#endif
