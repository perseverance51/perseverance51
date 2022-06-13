#ifndef _STC15_UART_H_
#define _STC15_UART_H_
typedef unsigned char   uint8;	//  8 bits 
typedef unsigned int  	uint16;	// 16 bits 
typedef unsigned long   uint32;	// 32 bits 


typedef signed char     int8;	//  8 bits 
typedef signed int      int16;	// 16 bits 

typedef signed long     int32;	// 32 bits 


typedef volatile int8   vint8;	//  8 bits 
typedef volatile int16  vint16;	// 16 bits 
typedef volatile int32  vint32;	// 32 bits 


typedef volatile uint8  vuint8;	//  8 bits 
typedef volatile uint16 vuint16;	// 16 bits 
typedef volatile uint32 vuint32;	// 32 bits 
typedef unsigned char BYTE;
typedef unsigned int WORD;
typedef unsigned long DWORD;
typedef unsigned char uint8_t;
typedef unsigned long UINT32;

//#include <STC15X.h>
#include <STC12.h>
#ifndef SYS_CLK 
#define SYS_CLK 11059200L
#endif

#define	UART1_CLEAR_RX_FLAG (SCON  &= ~0x01)
#define	UART2_CLEAR_RX_FLAG (S2CON &= ~0x01)
#define	UART3_CLEAR_RX_FLAG (S3CON &= ~0x01)
#define	UART4_CLEAR_RX_FLAG (S4CON &= ~0x01)

#define	UART1_CLEAR_TX_FLAG (SCON  &= ~0x02)
#define	UART2_CLEAR_TX_FLAG (S2CON &= ~0x02)
#define	UART3_CLEAR_TX_FLAG (S3CON &= ~0x02)
#define	UART4_CLEAR_TX_FLAG (S4CON &= ~0x02)

#define UART1_GET_RX_FLAG   (SCON  & 0x01)
#define UART2_GET_RX_FLAG   (S2CON & 0x01)
#define UART3_GET_RX_FLAG   (S3CON & 0x01)
#define UART4_GET_RX_FLAG   (S4CON & 0x01)

#define UART1_GET_TX_FLAG   (SCON  & 0x02)
#define UART2_GET_TX_FLAG   (S2CON & 0x02)
#define UART3_GET_TX_FLAG   (S3CON & 0x02)
#define UART4_GET_TX_FLAG   (S4CON & 0x02)

//此枚举定义不允许用户修改
typedef enum    // 枚举ADC通道
{
	TIM_0,
	TIM_1,
	TIM_2,
	TIM_3,
	TIM_4,
}UART_TIMN;
//此枚举定义不允许用户修改
typedef enum //枚举串口号
{
	UART_1,
	UART_2,
	UART_3,
	UART_4,
}UART_Name;


//此枚举定义不允许用户修改
typedef enum //枚举串口引脚
{
	UART1_RX_P30, UART1_TX_P31,		//只能使用同一行的RX和TX引脚号。不允许混用
	UART1_RX_P36, UART1_TX_P37,		//例如:UART1_RX_P30,UART1_TX_P37。这样不行。
	UART1_RX_P16, UART1_TX_P17,

	UART2_RX_P10, UART2_TX_P11,
	UART2_RX_P46, UART2_TX_P47,

	UART3_RX_P00, UART3_TX_P01,
	UART3_RX_P50, UART3_TX_P51,

	UART4_RX_P02, UART4_TX_P03,
	UART4_RX_P52, UART4_TX_P53,
}UART_PIN;
uint8 busy[5];

//-------------------------------------------------------------------------------------------------------------------
//  @brief      串口初始化
//  @param      uart_n          串口模块号(USART_1,USART_2,USART_3,USART_4)
//  @param      uart_rx_pin     串口波特率
//  @param      uart_tx_pin     串口接收发送引脚
//  @param      baud      		串口接收发送引脚
//  @param      tim_n      		使用tim_n作为串口波特率发生器(TIM1-TIM4)
//  @return     NULL          	
//  Sample usage:               uart_init(USART_1,115200,UART1_RX_P30_TX_P31);       // 初始化串口1 波特率115200 发送引脚使用P31 接收引脚使用P30
//  @note                       串口1使用 定时器1或者定时器2 作为波特率发生器。
//								串口2使用 定时器2 			 作为波特率发生器。
//								串口3使用 定时器3或者定时器2 作为波特率发生器。
//								串口4使用 定时器4或者定时器2 作为波特率发生器。
//                              STC8H仅有 定时器0-定时器4，这5个定时器。
//								编码器采集数据也需要定时器作为外部计数。
//-------------------------------------------------------------------------------------------------------------------
void uart_init(UART_Name uart_n, UART_PIN uart_rx_pin, UART_PIN uart_tx_pin, uint32 baud, UART_TIMN tim_n)
{
	uint16 brt;

	brt = 65536 - SYS_CLK / 4 / baud;

	//brt=64911;

	switch (uart_n)
	{
		case UART_1:
		{
			if (TIM_1 == tim_n)
			{
				SCON |= 0x50;
				TMOD |= 0x00;
				//TMOD=0x00;
				TL1 = brt;
				TH1 = brt >> 8;
				AUXR &= ~0x01;//定时器1切换为串口1
				AUXR |= 0x40;
				TR1 = 1;
				busy[1] = 0;
			}
			else if (TIM_2 == tim_n)
			{
				SCON |= 0x50;
				T2L = brt;
				T2H = brt >> 8;
				AUXR |= 0x15;
			}
			P_SW1 &= ~(0x03 << 6);
			if ((UART1_RX_P30 == uart_rx_pin) && (UART1_TX_P31 == uart_tx_pin))
			{
				P3M1&=~0x01;P3M0&=~0x01;//P30双向IO口
  				P3M1&=~0x02;P3M0&=~0x02;//P31双向IO口
				P_SW1 |= 0x00;
			}
			else if ((UART1_RX_P36 == uart_rx_pin) && (UART1_TX_P37 == uart_tx_pin))
			{
				P3M1&=~0x40;P3M0&=~0x40;//P36双向IO口
  				P3M1&=~0x80;P3M0&=~0x80;//P37双向IO口
				P_SW1 |= 0x40;
			}
			else if ((UART1_RX_P16 == uart_rx_pin) && (UART1_TX_P17 == uart_tx_pin))
			{
				P1M1&=~0x40;P1M0&=~0x40;//P16双向IO口
  				P1M1&=~0x80;P1M0&=~0x80;//P17双向IO口
				P_SW1 |= 0x80;
			}
	
			busy[1] = 0;
			// ES = 1;
			break;
		}

		case UART_2:
		{
			if (TIM_2 == tim_n)
			{
				S2CON |= 0x10;
				T2L = brt;
				T2H = brt >> 8;
				AUXR |= 0x14;
			}

			P_SW2 &= ~(0x01 << 0);
			if ((UART2_RX_P10 == uart_rx_pin) && (UART2_TX_P11 == uart_tx_pin))
			{
				P1M1&=~0x01;P1M0&=~0x01;//双向IO口
  				P1M1&=~0x02;P1M0&=~0x02;//双向IO口
				P_SW2 |= 0x00;
			}
			else if ((UART2_RX_P46 == uart_rx_pin) && (UART2_TX_P47 == uart_tx_pin))
			{
				P4M1&=~0x40;P4M0&=~0x40;//双向IO口
  				P4M1&=~0x80;P4M0&=~0x80;//双向IO口
				P_SW2 |= 0x01;
			}

			// IE2 |= 0x01 << 0;	//允许串行口2中断
			busy[2] = 0;
			break;
		}

		case UART_3:
		{

			if (TIM_2 == tim_n)
			{
				S2CON |= 0x10;
				T2L = brt;
				T2H = brt >> 8;
				AUXR |= 0x14;
			}
			else if (TIM_3 == tim_n)
			{
				S3CON |= 0x50;
				T3L = brt;
				T3H = brt >> 8;
				T4T3M |= 0x0a;
			}

			P_SW2 &= ~(0x01 << 1);
			if ((UART3_RX_P00 == uart_rx_pin) && (UART3_TX_P01 == uart_tx_pin))
			{
				P0M1&=~0x01;P0M0&=~0x01;//P00双向IO口
  				P0M1&=~0x02;P0M0&=~0x02;//P01双向IO口
				P_SW2 |= 0x00;
			}
			else if ((UART3_RX_P50 == uart_rx_pin) && (UART3_TX_P51 == uart_tx_pin))
			{
				P5M1&=~0x01;P5M0&=~0x01;//P50双向IO口
  				P5M1&=~0x02;P5M0&=~0x02;//P51双向IO口
				P_SW2 |= 0x02;
			}

			// IE2 |= 0x01 << 3;	//允许串行口3中断
			busy[3] = 0;
			break;
		}

		case UART_4:
		{
			if (TIM_2 == tim_n)
			{
				S4CON |= 0x10;
				T2L = brt;
				T2H = brt >> 8;
				AUXR |= 0x14;
			}
			else if (TIM_4 == tim_n)
			{
				S4CON |= 0x50;
				T4H = brt >> 8;
				T4L = brt;
				T4T3M |= 0xa0;
			}

			P_SW2 &= ~(0x01 << 2);
			if ((UART4_RX_P02 == uart_rx_pin) && (UART4_TX_P03 == uart_tx_pin))
			{
				P0M1&=~0x04;P0M0&=~0x04;//P02双向IO口
  				P0M1&=~0x08;P0M0&=~0x08;//P03双向IO口
				P_SW2 |= 0x00;
			}
			else if ((UART4_RX_P52 == uart_rx_pin) && (UART4_TX_P53 == uart_tx_pin))
			{
				P5M1&=~0x04;P5M0&=~0x04;//P52双向IO口
				P5M1&=~0x08;P5M0&=~0x08;//P53双向IO口
				P_SW2 |= 0x04;
			}

			// IE2 |= 0x01 << 4;	//允许串行口4中断

			busy[4] = 0;
			break;
		}
	}
	// EA = 1;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      串口字节输出
//  @param      uart_n          串口模块号(USART_1,USART_2,USART_3,USART_4)
//  @param      dat             需要发送的字节
//  @return     void        
//  Sample usage:               uart_putchar(UART_1,0xA5);       // 串口1发送0xA5
//-------------------------------------------------------------------------------------------------------------------
void uart_putchar(UART_Name uart_n, uint8 dat)
{
	switch (uart_n)
	{
		case UART_1:
			/*while (busy[1]);
			busy[1] = 1;*/
			SBUF = dat;
			while (UART1_GET_TX_FLAG == 0);
			UART1_CLEAR_TX_FLAG;
			break;
		case UART_2:
			/*while (busy[2]);
			busy[2] = 1;*/
			S2BUF = dat;
			while (UART2_GET_TX_FLAG == 0);
			UART2_CLEAR_TX_FLAG;
			break;
		case UART_3:
			/*while (busy[3]);
			busy[3] = 1;*/
			S3BUF = dat;
			while (UART3_GET_TX_FLAG == 0);
			UART3_CLEAR_TX_FLAG;
			break;
		case UART_4:
			/*while (busy[4]);
			busy[4] = 1;*/
			S4BUF = dat;
			while (UART4_GET_TX_FLAG == 0);
			UART4_CLEAR_TX_FLAG;
			break;
	}
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      串口发送数组
//  @param      uart_n          串口模块号(USART_1,USART_2,USART_3,USART_4)
//  @param      *buff           要发送的数组地址
//  @param      len             发送长度
//  @return     void
//  Sample usage:               uart_putbuff(UART_1,&a[0],5);
//-------------------------------------------------------------------------------------------------------------------
void uart_putbuff(UART_Name uart_n, uint8* p, uint16 len)
{
	while (len--)
		uart_putchar(uart_n, *p++);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      串口发送字符串
//  @param      uart_n          串口模块号(USART_1,USART_2,USART_3,USART_4)
//  @param      *str            要发送的字符串地址
//  @return     void
//  Sample usage:               uart_putstr(UART_1,"i lvoe you"); 
//-------------------------------------------------------------------------------------------------------------------
void uart_putstr(UART_Name uart_n, uint8* str)
{
	while (*str)
	{
		uart_putchar(uart_n, *str++);
	}
}

#endif /* _STC12_H_ */

