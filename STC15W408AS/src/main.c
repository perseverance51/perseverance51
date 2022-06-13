#include<lint.h>//包含SDCC_mcs51定义
#include <8052.h>//51头文件
//#include "delay.h"
#define MAIN_Fosc 16000000uL  //时钟频率

#define Led10 P1_0
typedef unsigned int u16;

unsigned char atime = 128;

// 仅作为延时, pms取值区间为 0 - 128
void delay(u16 pms) {
  u16 x, y;
  for (x=pms; x>0; x--) {
    for (y=11; y>0; y--);
  }
}

// 这里控制占空比, i取值区间为 0 - 128, 
// i越大脉冲宽度越低, 因为输出是低位点亮, 所以i越大LED越亮
void ledfade(u16 i) {
  Led10 = 0;
  delay(i);
  Led10 = 1;
  delay(atime-i);
}

int main(void) {
  u16 a, b;
  // 每个循环, 小灯
  while(1) {
    // a增大, 脉冲宽度降, 亮度增
    for (a=0; a<atime; a++) {
      for (b=0; b < (atime - a)/4; b++) {
        ledfade(a);
      }
    }
    // a减小, 脉冲宽度增, 亮度降
    for (a=atime; a>0; a--) {
      for (b=0; b < (atime - a)/4; b++) {
        ledfade(a);
      }
    }
  }
}
