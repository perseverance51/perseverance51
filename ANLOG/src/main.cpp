#include <Arduino.h>

int led = 13;
void setup() {                  
  pinMode(led, OUTPUT);
  DDRC=0xff;//端口C设置为输出
  PORTC=0x00;//端口C初始值设置为0  
  Serial.begin(9600);  
}

void loop() {
digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
PORTC=0x35;//控制A0、A2、A4、A5亮,A1和A3熄灭
  Serial.println(PORTC,HEX);
  delay(1000);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  PORTC =~0x35;////控制A0、A2、A4、A5熄灭,A1和A3亮起
  Serial.println(PORTC,HEX);
  delay(1000);               // wait for a second
}