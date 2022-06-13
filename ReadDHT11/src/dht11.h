#ifndef _H_DHT11
#define _H_DHT11


/**
 * Interface define
*/

//#include "mcs51/8051.h"
#include "delay.h"
#include "stdint.h"
#include <8052.h>//51头文件

#define DATA_WRITE(val) P1_1 = (val)
#define DATA_READ() P1_1

#define _Delay(ms) Delay(ms)

void _Delay10us() //@16MHz
{
    unsigned char i;
     i = 37;
    __asm nop __endasm;
   __asm nop __endasm;
   
    while (--i)
        ;
}

//---------------------

// 返回值含义
#define DHT11_DONE 0
#define DHT11_CONNECT_ERR 1
#define DHT11_VERIFY_ERR 2

typedef struct
{
    uint8_t humidity;
    float temperature;
} DHT11_Data;

#define DHT11_Init() DATA_WRITE(1)

uint8_t DHT11_Measure(DHT11_Data *dat)
{
    int8_t buf[5];
    uint8_t i, j, errCode = DHT11_DONE;

    DATA_WRITE(0);
    _Delay(20); // 开始信号 20 ms
    DATA_WRITE(1);

    // 60 us
    _Delay10us();
    _Delay10us();
    _Delay10us();
    _Delay10us();
    _Delay10us();
    _Delay10us();

    if (DATA_READ() == 0)
    {
        while (DATA_READ() == 0) //等待 DHT11 拉高
            ;

        while (DATA_READ() == 1)
            ;

        i = 0;
        while (i < 5)
        {
            j = 0;
            while (j < 8)
            {
                while (DATA_READ() == 0)
                    ;

                _Delay10us();
                _Delay10us();
                _Delay10us();

                buf[i] <<= 1;
                buf[i] |= DATA_READ();
                while (DATA_READ() == 1)
                    ;
                j++;
            }
            i++;
        }

        _Delay10us();
        _Delay10us();
        _Delay10us();
        _Delay10us();
        _Delay10us();
        _Delay10us();

        if (buf[4] == buf[0] + buf[1] + buf[2] + buf[3])
        {
            dat->temperature = buf[2];
            dat->humidity = buf[0];
        }
        else
        {
            errCode = DHT11_VERIFY_ERR;
        }
    }
    else
    {
        errCode = DHT11_CONNECT_ERR;
    }

    DATA_WRITE(1);
    _Delay(1);

    return errCode;
}

#endif

