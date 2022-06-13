
#ifndef _STC15_ADC_H_
#define _STC15_ADC_H_
#include <8052.h>
//#include <STC15X.h>
#include "STC15_delay.h"

#define ADC_POWER   0x80            //ADC电源控制位
#define ADC_FLAG    0x10            //ADC完成标志
#define ADC_START   0x08            //ADC起始控制位

uint8  setbit = 0;
uint8  setspeed = 0;

typedef enum
{
	ADC_P10 = 0,
	ADC_P11,
	ADC_P12, //仅做占位
	ADC_P13,
	ADC_P14,
	ADC_P15,
	ADC_P16,
	ADC_P17
} ADC_Name;

//此枚举定义不允许用户修改
typedef enum
{
	ADC_540T = 0,
	ADC_360T,
	ADC_180T,
	ADC_90T
} ADC_CLK;

//此枚举定义不允许用户修改
typedef enum    // 枚举ADC通道
{
	ADC_10BIT= 0,		//10位分辨率
	ADC_9BIT,    	//9位分辨率
	ADC_8BIT,     	//8位分辨率
}ADC_bit;

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADC初始化
//  @param      adcn            选择ADC通道
//  @param      speed      		ADC速度
//  @return     void
//  Sample usage:               adc_init(ADC_P10,ADC_SYSclk_DIV_2);//初始化P1.0为ADC功能,ADC时钟频率：SYSclk/2
//-------------------------------------------------------------------------------------------------------------------
void adc_init(ADC_Name adcn, ADC_CLK speed, ADC_bit _sbit)
{
	setbit = _sbit;
	setspeed = speed;

	P1ASF |= 1 << adcn;  //2. 设置为模拟口功能

	ADC_RES = 0;         //3. 清除结果寄存器

	ADC_CONTR = ADC_POWER | (speed<<5);

	CLK_DIV |= 1 << 5;//转换结果右对齐。 ADC_RES 保存结果的高 2 位， ADC_RESL 保存结果的低 8 位。

    delay(2);                       //ADC上电并延时
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADC转换一次
//  @param      adcn            选择ADC通道
//  @return     void
//  Sample usage:               adc_read(ADC_P10, ADC_10BIT);
//-------------------------------------------------------------------------------------------------------------------
uint16 adc_read(ADC_Name adcn)
{
	uint16 adc_value;

	// ADC_CONTR &= (0xF0);	//清除ADC_CHS[3:0] ： ADC 模拟通道选择位
	// ADC_CONTR |= adcn;

	ADC_CONTR = ADC_POWER | (setspeed<<5) | adcn | ADC_START;
    _nop_();
	_nop_();
	_nop_();
	_nop_();
	//ADC_CONTR |= 0x40;  // 启动 AD 转换
	while (!(ADC_CONTR & ADC_FLAG));  // 查询 ADC 完成标志
	ADC_CONTR &= ~ADC_FLAG;         //Close ADC

	adc_value = ADC_RES;  //存储 ADC 的 10 位结果的高 2 位
	adc_value <<= 8;
	adc_value |= ADC_RESL;  //存储 ADC 的 10 位结果的低 8 位

	ADC_RES = 0;
	ADC_RESL = 0;

	adc_value >>= setbit;//取多少位

	return adc_value;
}

#endif