
#include <delay.h>

void delay_ms(unsigned int ms)
{
    unsigned int i;
    do
    {
        i = MAIN_Fosc / 13000;
        while (--i)
            ;
    } while (--ms);
}

