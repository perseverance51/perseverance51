#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>     //用于esp_wifi_restore() 删除保存的wifi信息

   //板载led灯
#define NTP1  "ntp1.aliyun.com"
#define NTP2  "ntp2.aliyun.com"
#define NTP3  "ntp3.aliyun.com"

const unsigned long interval = 6000UL;//设置更新频率
unsigned long previousMillis = 0;
bool ledState = false;
void SmartConfig()
{
   WiFi.mode(WIFI_STA);
   Serial.println("\r\n wait for smartconfig....");
   WiFi.beginSmartConfig();
   while(1)
   {
    Serial.print(".");
    delay(500);
    if ( WiFi.smartConfigDone())
    {
      Serial.println("SmartConfig Success");
      Serial.printf("SSID:%s\r\n",WiFi.SSID().c_str());
      Serial.printf("PSW:%s\r\n",WiFi.psk().c_str());
      esp_wifi_set_storage(WIFI_STORAGE_RAM);
      WiFi.setAutoConnect(true);  // 设置自动连接
      break;      
    }
   }  
}
bool AutoConfig();
void setup() {
  Serial.begin(115200);
 pinMode(LED_BUILTIN, OUTPUT);//板载led灯作为指示
delay(100);
  if (!AutoConfig())
  {
  	SmartConfig();
  }
 
  Serial.println("WiFi Connected.");
  Serial.println(WiFi.SSID().c_str());
   Serial.println(WiFi.psk().c_str());
 configTime(8 * 3600, 0, NTP1, NTP2,NTP3);
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
 // esp_wifi_restore();  //删除保存的wifi信息
   //Wait for SmartConfig packet from mobile
  Serial.println("Waiting for SmartConfig.");

  
}
 
void loop() {
  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
     ledState = !ledState;//状态翻转
     digitalWrite(LED_BUILTIN, ledState);
     struct tm timeInfo; //声明一个结构体
  if (!getLocalTime(&timeInfo))
  { //一定要加这个条件判断，否则内存溢出
    Serial.println("Failed to obtain time");
  }
   Serial.println(&timeInfo, "%F %T %A"); 
        previousMillis = currentMillis;
}
}

bool AutoConfig()
{
  WiFi.begin();
  for (int i=0; i<20; i++)
  {
    int wstatus = WiFi.status();
    if (wstatus == WL_CONNECTED )  
  
       {
          Serial.println("wifi smartConfig success");
          Serial.printf("SSID:%s",WiFi.SSID().c_str());
          Serial.printf(",PWS:%s\r\n",WiFi.psk().c_str());
          Serial.print("localIP:");
          Serial.println(WiFi.localIP());
          Serial.print(",GateIP:");
          Serial.println(WiFi.gatewayIP());
          
          return true;
          
       }
       else
       {
          Serial.print("WIFI AutoConfig Waiting ....");
          Serial.println(wstatus);
          delay(1000);
        
       }
    
  }
  Serial.println("Wifi autoconfig faild!");
  return false;
  }

