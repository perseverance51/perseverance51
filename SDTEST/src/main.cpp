#include <Arduino.h>

void setup()
{
  Serial.begin(115200);
  uint64_t SDsize = 12345678;
  String str = "SDsize:";
  char buff[10];
  char buff2[10];
  sprintf_P(buff, PSTR(" %llu"), SDsize);
  sprintf(buff2," %llu", SDsize);
  str += buff;
  Serial.println(str);
}

void loop()
{
}