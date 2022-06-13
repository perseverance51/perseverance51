#ifndef _H_SOFT_UART
#define _H_SOFT_UART

/**1 个起始位，8 个数据位，0 个校验位，1 个停止位，
 * ：8，N，1；8 个数据位，一个停止位，无校验位。
 * 波特率：9600，位宽度：8，停止位：1
 * 传输一个字节(8 位)的数据，在总线上产生 10 个电平变换，也就是串行总线上，需要 10 位，才能发送 1 个字节数据
 * 1 秒可以发送 9600 位，那么用 9600/10 ，就是1秒可以发送 960 个字节数据，
 * 则每发送一个字节需要的时间就是：1/960 ~= 0.00104166…s ~= 1.0416667 ms。
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

