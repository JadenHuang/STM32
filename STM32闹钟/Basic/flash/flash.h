#ifndef __FLASH_H
#define __FLASH_H 			   
#include "sys.h"

#define FLASH_START_ADDR  0x0801f000	  //д�����ʼ��ַ
extern	u8 alarn_hour[8];
extern	u8 alarn_min[8]; 

void FLASH_W(u32 add,u16 dat);
void Alarn_FLASH_W(void);
u16 FLASH_R(u32 add);



#endif





























