
#include <Arduino.h>
#include "LittleFS.h"

void setup()
{
  Serial.begin(9600);
  struct FSInfo x;
  if (!LittleFS.begin())
  {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }
  LittleFS.info(x);
  Serial.println("全部的大小");
  Serial.println(x.totalBytes);
  Serial.println("已经使用的");
  Serial.println(x.usedBytes);
}

void loop()
{
}
