#ifndef __CAN_H
#define __CAN_H	 
#include "sys.h"	    


void CAN_Configuration(void);
void CAN_SendMessage(uint8_t data);
void CAN_ReceiveMessage(void);

#endif

















