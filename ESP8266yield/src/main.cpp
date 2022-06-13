#include <Arduino.h>


void setup()
{
    Serial.begin(115200);
   Serial.setDebugOutput(true);  //将串口设为调试输出模式
  // Serial.setDebugOutput(false);
}
void loop()
{
   // Serial.println("Hello");
   printf("Hello\n");
   //Serial.printf("Hello\n");
    for(int i=0; i<5000; i++)
    {
        delayMicroseconds(1000);
        yield();
    }
}