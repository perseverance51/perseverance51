#include <Arduino.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Ticker.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2 // ESP32 DOES NOT DEFINE LED_BUILTIN
#endif
int LED = LED_BUILTIN;
const char *NTPAddress = "ntp1.aliyun.com"; //ntp.ntsc.ac.cn, ntp1.aliyun.com,1.cn.pool.ntp.org

Ticker ticker;
void NowTime();
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTPAddress, 8 * 60 * 60); //ntpUDP初始化，NTPAddress，8 * 60 * 60时区
void configModeCallback(WiFiManager *myWiFiManager)
{
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  Serial.println(myWiFiManager->getConfigPortalSSID());
}
void tick()
{
  digitalWrite(LED, !digitalRead(LED)); // set pin to the opposite state
  if (timeClient.update())
    Serial.println(timeClient.getFormattedTime());
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
  Serial.println(F("connected...yeey :)"));
  timeClient.begin();
  ticker.detach();
  ticker.attach(1, tick);
}

void loop()
{
}
