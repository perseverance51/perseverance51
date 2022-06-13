#include<lint.h>//包含SDCC_mcs51定义
#include "stdio.h"
#include <stc15.h>//51头文件
#include "soft_uart.h"
#include "delay.h"
#define led P1_0 
#define MAIN_Fosc 16000000uL //晶振频率
/*重映射串口打印函数printf   */
int putchar(int c)
{
    TxSend((uint8_t)c);
    return c;
}

int main(void)
{       
   // EA = 1;
   AUXR |= 0xc0;
    led = 0;
    while (1)
    {
      printf("AUXR=%u \r\n",AUXR);  
      printf("hello world! \n");    
      Delay(1000);
      led = !led;
      printf("perseverance51! \n"); 
    }
}