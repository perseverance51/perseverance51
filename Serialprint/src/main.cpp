#include <Arduino.h>
unsigned int jishu = 0;
char str[5] = "abcd";
char *p = str;

void setup()
{
  Serial.begin(115200);
}
void loop()
{
  while (Serial.available() > 0)
  {
    *p++ = (char)Serial.read();
    delay(2);
    // jishu++;
  }
  if (*p > 0)
  {
    Serial.print("串口接收到的数据:");
    Serial.println(strcpy(str, p));
    memset(str, 0, sizeof(str)); //清空数组

    // jishu = 0;                   //清空计数值
  }
}
