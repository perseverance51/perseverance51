#include <Arduino.h>
#include "SPI.h"
#include "TFT_eSPI.h"
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <TJpg_Decoder.h>
//#include <FS.h>

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite clk = TFT_eSprite(&tft);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

const char *ssid = "CMCC-DyVv";    //wifi账号
const char *password = "pba5ayzk"; //wifi密

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println("");
  Serial.println("");
  Serial.println("TFT_eSPI library test!");
  tft.init();                  //初始化显示寄存器
  tft.fillScreen(TFT_WHITE);   //屏幕颜色
  tft.setTextColor(TFT_BLACK); //设置字体颜色黑色
  tft.setCursor(15, 30, 1);    //设置文字开始坐标(15,30)及1号字体
  tft.setTextSize(2);
  //tft.setRotation(4);    //设置显示镜像 + 旋转90°
  //连接wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  timeClient.begin();
  timeClient.setTimeOffset(28800);
}

void loop()
{
  timeClient.update();
  //打印时间
  Serial.println(timeClient.getFormattedTime());
  tft.println(timeClient.getFormattedTime());
  delay(1000);
}
