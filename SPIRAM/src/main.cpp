#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <HTTPClient.h>
// #elif defined(ESP8266)
// #include <ESP8266WiFi.h>
// #include <ESP8266HTTPClient.h>
#endif
#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson

// 测试HTTP请求用的URL
#define URL "http://www.weather.com.cn/data/sk/101250301.html"
//http://www.weather.com.cn/data/sk/101250301.html
//https://devapi.qweather.com/v7/weather/3d?lang=en&location=CN101250303&key=bfaee7681d3649d88e03082f1a1e48e7&gzip=n
//和风实时气象V7版本
// 设置wifi接入信息(请根据您的WiFi信息进行修改)
const char *ssid = "MERCURY_D268G";
const char *password = "pba5ayzk";

struct SpiRamAllocator
{
  void *allocate(size_t size)
  {                                                   //申请空间
    return heap_caps_malloc(size, MALLOC_CAP_SPIRAM); //Heap分配
  }
  void deallocate(void *pointer)
  { //Heap释放
    heap_caps_free(pointer);
  }
};

using SpiRamJsonDocument = BasicJsonDocument<SpiRamAllocator>;

void esp8266Http();
void setup()
{
  //初始化串口设置
  Serial.begin(115200);
  Serial.println();
  Serial.println(ESP.getFreeHeap());
  //Serial.println(getFreePsram());
  //设置ESP8266工作模式为无线终端模式
  WiFi.mode(WIFI_STA);

  //开始连接wifi
  WiFi.begin(ssid, password);

  //等待WiFi连接,连接成功打印IP
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi Connected!");
}

void loop()
{
  // 如果ESP8266连接WiFi则发送HTTP请求
  if ((WiFi.status() == WL_CONNECTED))
  {
    esp8266Http();
  }

  delay(60000); // 短暂等待
}

// 发送HTTP请求并且将服务器响应通过串口输出
void esp8266Http()
{

  //创建 HTTPClient 对象
  HTTPClient httpClient;

  //配置请求地址。此处也可以不使用端口号和PATH而单纯的
  httpClient.begin(URL);
  Serial.print("URL: ");
  Serial.println(URL);

  //启动连接并发送HTTP请求
  int httpCode = httpClient.GET();
  Serial.print("Send GET request to URL: ");
  Serial.println(URL);

  //如果服务器响应OK则从服务器获取响应体信息并通过串口输出
  //如果服务器不响应OK则将服务器响应状态码通过串口输出
  if (httpCode == HTTP_CODE_OK)
  {
    String responsePayload = httpClient.getString();

    SpiRamJsonDocument doc(384);
    DeserializationError error = deserializeJson(doc, responsePayload);
    if (error)
    {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }
    JsonObject root = doc.as<JsonObject>();
    delay(50);
    for (int i = 0; i < 3; i++)
    {
      String datestr = root["daily"][i]["fxDate"]; //获取天3天天气日期，成功。
      delay(50);
      Serial.println(datestr);
      String tmp_min = root["daily"][i]["tempMin"];
      Serial.println(tmp_min);
      String tmp_max = root["daily"][i]["tempMax"];
      Serial.println(tmp_max);
      String cond_code = root["daily"][i]["iconDay"]; //获取天气图标
      Serial.println(cond_code);
    }
    return;
  }
  else
  {
    Serial.println("GET... failed, error: ");
  }

  //关闭ESP8266与服务器连接
  httpClient.end();
}