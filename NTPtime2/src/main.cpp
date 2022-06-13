#include <Arduino.h>
#include <WiFi.h>

#define NTP1 "ntp1.aliyun.com"
#define NTP2 "ntp2.aliyun.com"
#define NTP3 "ntp3.aliyun.com"
//填写WIFI入网信息
const char *ssid = "MERCURY_D268G";                                                                                // WIFI账户
const char *password = "pba5ayzk";                                                                                 // WIFI密码
const String WDAY_NAMES[] = {"星期天", "星期一", "星期二", "星期三", "星期四", "星期五", "星期六"};                //星期
const String MONTH_NAMES[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"}; //月份

//time_t now; //实例化时间
void setClock()
{

  struct tm timeInfo; //声明一个结构体
  if (!getLocalTime(&timeInfo))
  { //一定要加这个条件判断，否则内存溢出
    Serial.println("Failed to obtain time");
    return;
  }
  //Serial.print(asctime(&timeInfo)); //默认打印格式：Mon Oct 25 11:13:29 2021

  //strcpy(buff1, asctime(&timeInfo));
  String date = WDAY_NAMES[timeInfo.tm_wday];
  Serial.println(date.c_str());
  // sprintf_P(buff1, PSTR("%04d-%02d-%02d %s"), timeInfo.tm_year + 1900, timeInfo.tm_mon + 1, timeInfo.tm_mday, WDAY_NAMES[timeInfo.tm_wday].c_str());
  String shuju = String(timeInfo.tm_year + 1900); //年
  shuju += "-";
  shuju += timeInfo.tm_mon + 1; //月
  shuju += "-";
  shuju += timeInfo.tm_mday; //日
  shuju += " ";
  shuju += timeInfo.tm_hour; //时
  shuju += ":";
  shuju += timeInfo.tm_min;
  shuju += ":";
  shuju += timeInfo.tm_sec;
  shuju += " ";
  shuju += WDAY_NAMES[timeInfo.tm_wday].c_str(); //星期
  Serial.println(shuju);
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
}

void loop()
{
  Serial.println("Waiting 10s before the next round...");
  delay(10000);
  setClock();
  // printLocalTime();
}