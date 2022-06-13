#include "soft_uart.h"

// void BitTime(void)
// {
//   volatile  uint16_t i;
//     i = (16 * 80) / 13- 2; //根据主时钟来计算位时间16
//     while (--i)
//         ;
// }

void	BitTime(void)
{
	 uint16_t  i;
     i = (16 * 80) / 13 - 2; //根据主时钟来计算位时间16
	// i = ((MAIN_Fosc / 100) * 104) / 140000 - 1;  //根据主时钟来计算位时间
	while(--i);
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

