#ifndef _H_SOFT_UART
#define _H_SOFT_UART

/**
 * 波特率：9600，位宽度：8，停止位：1
*/

#include "stdint.h"
#include<lint.h>//包含SDCC_mcs51定义
//#include <8052.h>//51头文件
#include <stc15.h>//51头文件
#define P_TXD P2_0 //定义模拟串口发送端,可以是任意IO

#define CLOCK		16	//定义主时钟16MHz 
#define MAIN_Fosc 16000000uL //晶振频率
void TxSend(uint8_t dat);

#endif

