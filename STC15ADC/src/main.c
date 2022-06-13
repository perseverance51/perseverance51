#define SYS_CLK 24000000L//设置定时器、串口频率参数
#include<lint.h>//包含SDCC_mcs51定义
#include <STC12.h>
//#include <STC15X.h>
#include "STC15_UART.h"
#include "STC15_ADC.h"
#include "STC15_delay.h"

void setup()
{
  /*****本案例程序说明**************************/
  //本案例可以通过软件的串口监视器查看AD值，因为串口以字节流发送数据，所以把AD设置为8位模式，方便查看
  /*********************************************/
  uart_init(UART_1, UART1_RX_P30, UART1_TX_P31, 9600, TIM_0);//初始化串口
  adc_init(ADC_P10, ADC_540T, ADC_10BIT);//ADC初始化，三个参数ADC引脚，时钟分频双数2-32，输出值位数12BIT最大分率-12位的ADC输出12位，10位的输出10位
}

void loop()
{
  uart_putchar(UART_1, (adc_read(ADC_P10)));//串口单个字符输出
  delay(1000);
}

void main(void)
{
  setup();
  while(1){
    loop();
  }
}
