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
    __asm NOP __endasm;//内嵌汇编指令：NOP，相当于_nop_()
   __asm NOP __endasm;
	//_nop_();
	
	while (--i);
}
