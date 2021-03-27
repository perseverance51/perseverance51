#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Arduino.h>
#include <TimeLib.h>

//#define TZ              8      // 中国时区为8
//#define DST_MN          0      // 默认为0
//#define TZ_MN           ((TZ)*60)   //时间换算
//#define TZ_SEC          ((TZ)*3600)
//#define DST_SEC         ((DST_MN)*60)



#ifndef STASSID
#define STASSID "qwer"
#define STAPSK  "asdfghjkl"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);

// NTP Servers:国家授时中心新的NTP服务器地址：ntp.ntsc.ac.cn
static const char ntpServerName[] = "cn.pool.ntp.org";
const int timeZone = 8;     // 时区

WiFiUDP Udp;
unsigned int localPort = 8888;  // local port to listen for UDP packets

time_t getNtpTime();
void digitalClockDisplay();
void printDigits(int digits);
void sendNTPpacket(IPAddress &address);

bool LED_Flag = false;
String str =
  "<!DOCTYPE html><html><head><meta charset=\"UTF-8\"><meta name=\"viewport\"content=\"width=device-width, initial-scale=1.0\"><meta http-equiv=\"X-UA-Compatible\"content=\"ie=edge\"><title>不知名up的ESP8266网页配网</title></head><body><form name=\"my\">WiFi名称：<input type=\"text\"name=\"s\"placeholder=\"请输入您WiFi的名称\"id=\"aa\"><br>WiFi密码：<input type=\"text\"name=\"p\"placeholder=\"请输入您WiFi的密码\"id=\"bb\"><br><input type=\"button\"value=\"连接\"onclick=\"wifi()\"></form><script language=\"javascript\">function wifi(){var ssid=my.s.value;var password=bb.value;var xmlhttp=new XMLHttpRequest();xmlhttp.open(\"GET\",\"/HandleVal?ssid=\"+ssid+\"&password=\"+password,true);xmlhttp.send()}</script></body></html>";
/*****************************************************
   函数名称：handleRoot()
   函数说明：客户端请求回调函数
   参数说明：无
******************************************************/
void handleRoot() {
  server.send(200, "text/html", str);
}
/*****************************************************
   函数名称：HandleVal()
   函数说明：对客户端请求返回值处理
   参数说明：无
******************************************************/
void HandleVal()
{
  String wifis = server.arg("ssid"); //从JavaScript发送的数据中找ssid的值
  String wifip = server.arg("password"); //从JavaScript发送的数据中找password的值
  Serial.println(wifis); Serial.println(wifip);
  WiFi.begin(wifis, wifip);
}
/*****************************************************
   函数名称：handleNotFound()
   函数说明：响应失败函数
   参数说明：无
******************************************************/
void handleNotFound() {
  digitalWrite(LED_BUILTIN, 0);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(LED_BUILTIN, 1);
}
/*****************************************************
   函数名称：autoConfig()
   函数说明：自动连接WiFi函数
   参数说明：无
   返回值说明:true：连接成功 false：连接失败
******************************************************/
bool autoConfig()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin();
  Serial.print("AutoConfig Waiting......");
  for (int i = 0; i < 20; i++)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println("AutoConfig Success");
      Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
      Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
      WiFi.printDiag(Serial);
      return true;
      //break;
    }
    else
    {
      Serial.print(".");
      LED_Flag = !LED_Flag;
      if (LED_Flag)
        digitalWrite(LED_BUILTIN, HIGH);
      else
        digitalWrite(LED_BUILTIN, LOW);
      delay(500);
    }
  }
  Serial.println("AutoConfig Faild!" );
  return false;
  //WiFi.printDiag(Serial);
}
/*****************************************************
   函数名称：htmlConfig()
   函数说明：web配置WiFi函数
   参数说明：无
******************************************************/
void htmlConfig()
{
  WiFi.mode(WIFI_AP_STA);//设置模式为AP+STA
  digitalWrite(LED_BUILTIN, LOW);
  WiFi.softAP(ssid, password);
  Serial.println("AP设置完成");

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/HandleVal", HTTP_GET, HandleVal);
  server.onNotFound(handleNotFound);//请求失败回调函数

  server.begin();//开启服务器
  Serial.println("HTTP server started");
  while (1)
  {
    server.handleClient();
    MDNS.update();
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println("HtmlConfig Success");
      Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
      Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
      Serial.println("HTML连接成功");
      break;
    }
  }
}

void setup(void) {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.begin(115200);
  bool wifiConfig = autoConfig();
  Udp.begin(localPort);
  setSyncProvider(getNtpTime);
  setSyncInterval(300);
  if (wifiConfig == false) {
    htmlConfig();//HTML配网
  }
  //configTime(TZ_SEC, DST_SEC, "ntp.ntsc.ac.cn", "ntp7.aliyun.com"); //ntp获取时间，你也可用其他"1.cn.pool.ntp.org","ntp1.aliyun.com"
  delay(200);

}

time_t prevDisplay = 0; // when the digital clock was displayed

void loop(void) {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
if (timeStatus() != timeNotSet) {
    if (now() != prevDisplay) { //update the display only if time has changed
      prevDisplay = now();
      digitalClockDisplay();
    }
  }

}

void digitalClockDisplay()
{
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(".");
  Serial.print(month());
  Serial.print(".");
  Serial.print(year());
  Serial.println();
}

void printDigits(int digits)
{
  // utility for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

/*-------- NTP code ----------*/

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address

  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("Transmit NTP Request");
  // get a random server from the pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  Serial.print(ntpServerName);
  Serial.print(": ");
  Serial.println(ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}
