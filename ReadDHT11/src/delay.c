#include <delay.h>

void Delay(unsigned int ms)
{
    unsigned int i;
    do
    {
        i = MAIN_Fosc / 13000;
        while (--i)
            ;
    } while (--ms);
}
void Delay1us()		//@16MHz
{
	unsigned char i;
        i = 1;
    __asm nop__endasm;
   __asm nop __endasm;
	//_nop_();
	
	while (--i);
}
