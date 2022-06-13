#include <Arduino.h>

/**
  ESP32 最简单直接获取网络时间方法
*/

#include <WiFi.h>

#define NTP1 "ntp1.aliyun.com"
#define NTP2 "ntp2.aliyun.com"
#define NTP3 "ntp3.aliyun.com"

//填写WIFI入网信息
const char *ssid = "MERCURY_D268G";                                                                                // WIFI账户
const char *password = "pba5ayzk";                                                                                 // WIFI密码
const String WDAY_NAMES[] = {"星期天", "星期一", "星期二", "星期三", "星期四", "星期五", "星期六"};                //星期
const String MONTH_NAMES[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"}; //月份

time_t now; //实例化时间
String setClock()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  { //如果获取失败，就开启联网模式，获取时间
    Serial.println("Failed to obtain time");
    WiFi.disconnect(false);
    WiFi.mode(WIFI_STA); //开启网络
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
    }
    configTime(8 * 3600, 0, NTP1, NTP2, NTP3);
    return asctime(&timeinfo);
  }
  // Serial.println(&timeinfo, "%F %T %A"); // 格式化输出:2021-10-24 23:00:44 Sunday
  //sprintf_P(str, PSTR("%s,%s,%s"), &timeinfo, "%F %T %A");
  // strcpy(str, asctime(&timeinfo));
  // Serial.println(asctime(&timeinfo)); //打印格式：Mon Oct 25 11:13:29 2021
  String str = asctime(&timeinfo);
  // Serial.printf("asctime= %s", str);
  // WiFi.disconnect(true);
  return str;
}

void setTime1()
{
  now = time(nullptr);
  struct tm *timeInfo; //声明一个结构体
  timeInfo = localtime(&now);
  char buff[16];
  String date = WDAY_NAMES[timeInfo->tm_wday];
  sprintf_P(buff, PSTR("%04d-%02d-%02d %s"), timeInfo->tm_year + 1900, timeInfo->tm_mon + 1, timeInfo->tm_mday, WDAY_NAMES[timeInfo->tm_wday].c_str());
  Serial.println(buff);
  //String str = asctime(&timeinfo);
  // Serial.println(asctime(&timeinfo));
  //strcpy(str, asctime(&timeinfo));
  // return str;
}
void setup()
{
  Serial.begin(115200);
  Serial.println();
  //设置ESP32工作模式为无线终端模式
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected!");
  configTime(8 * 3600, 0, NTP1, NTP2, NTP3);
  // setClock();
  // 从网络时间服务器上获取并设置时间
  // 获取成功后芯片会使用RTC时钟保持时间的更新
  //Serial.println(setTime().c_str());
  // WiFi.disconnect(true);
  // WiFi.mode(WIFI_OFF); //关闭网络
  // Serial.println("WiFi disconnected!");
}

void loop()
{

  Serial.println("Waiting 10s before the next round...");
  delay(10000);
  // Serial.printf("%s", setClock().c_str());
  // Serial.printf("%s", setClock().substring(0, 10));
  setTime1();
}
