#include "soft_uart.h"

// void BitTime(void)
// {
//   volatile  uint16_t i;
//     i = (16 * 80) / 13- 2; //根据主时钟来计算位时间16
//     while (--i)
//         ;
// }

void BitTime(void)
{
	 uint16_t  i;
     i = (16 * 160) / 13 -3; //根据主时钟来计算位时间16
    // i = (16 * 7) / 12 -3; //根据主时钟来计算位时间16

      
	while(--i);
   // __asm NOP __endasm;//内嵌汇编指令：NOP，相当于_nop_()
}

void TxSend(uint8_t dat)
{
    uint8_t i;
    EA = 0;
    P_TXD = 0;
    BitTime();
    for (i = 0; i < 8; i++)
    {
        if (dat & 1)
            P_TXD = 1;
        else
            P_TXD = 0;
        dat >>= 1;
        BitTime();
    }
    P_TXD = 1;
    EA = 1;
    BitTime();
    BitTime();
}

