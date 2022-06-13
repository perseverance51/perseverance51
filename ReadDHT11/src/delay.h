#ifndef	__DELAY_H
#define	__DELAY_H
#include "stdint.h"//包含数据类型名缩写
#include "stdio.h"
#define _nop_()   __asm NOP__endasm
#define _nop()_  __asm nop __endasm

#define MAIN_Fosc 16000000uL //晶振频率
void Delay(unsigned int ms);
void Delay1us()	;	//@16MHz


#endif
