#include <Arduino.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <ESP8266HTTPClient.h>
#include <Ticker.h>

// time
#include <time.h>                  //time() ctime()
//#include <sys/time.h>                          // struct timeval
//#include <coredecls.h>                         // settime ofday_cb()
#define TZ 8            // 中国时区�?8
#define DST_MN 0        // 默认�?0
#define TZ_MN ((TZ)*60) //时间换算
#define TZ_SEC ((TZ)*3600)
#define DST_SEC ((DST_MN)*60)
time_t now;

Ticker ticker;
#ifndef LED_BUILTIN
#define LED_BUILTIN 2 // ESP32 DOES NOT DEFINE LED_BUILTIN
#endif
int LED = LED_BUILTIN;
const String WDAY_NAMES[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"}; //星期
void NowTime();
void tick()
{
  digitalWrite(LED, !digitalRead(LED)); // set pin to the opposite state
  NowTime();
}
void configModeCallback(WiFiManager *myWiFiManager)
{
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  Serial.println(myWiFiManager->getConfigPortalSSID());
}

void setup()
{
  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  WiFiManager wm;
  wm.setAPCallback(configModeCallback);
  if (!wm.autoConnect())
  {
    Serial.println("failed to connect and hit timeout");
    ESP.restart();
    delay(1000);
  }
  Serial.println("connected...yeey :)");
  ticker.detach();
  digitalWrite(LED, LOW);
  configTime(TZ_SEC, DST_SEC, "ntp.ntsc.ac.cn", "ntp1.aliyun.com", "1.cn.pool.ntp.org"); //ntp获取时间，你也可用其�?"pool.ntp.org","0.cn.pool.ntp.org","1.cn.pool.ntp.org","ntp1.aliyun.com"
  ticker.attach(6, tick);
}

void loop()
{
}
//https请求
void NowTime()
{
  now = time(nullptr); //实例化时�?
                       // struct tm *timeInfo;
  tm *timeInfo;

  timeInfo = localtime(&now);
  char buff[16];
  // String date = WDAY_NAMES[timeInfo->tm_wday];
  // Serial.println(date);
  sprintf_P(buff, PSTR("%04d-%02d-%02d  %s"), timeInfo->tm_year + 1900, timeInfo->tm_mon + 1, timeInfo->tm_mday, WDAY_NAMES[timeInfo->tm_wday].c_str());
  Serial.println(buff);
  Serial.println("");
  Serial.printf("%d : %d : %d\n", timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
}