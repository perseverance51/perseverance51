#include <ESP8266WiFi.h>
#include <PubSubClient.h>
/**********************网络设置项 **********************/
const char *ssid = "";                   //路由器名字
const char *password = "";               //路由器密码
const char *mqtt_server = "192.168.4.1"; //MQTT服务器IP
const int mqttPort = 8080;               //MQTT服务器端口
/*****************用户信息项 (全部为自定义项)*****************/
const char *ESP8266_ID = "esp8266ID";     //自定义ID
const char *ESP8266_user = "";            //用户名
const char *ESP8266_password = "";        //密码
const char *ESP8266_pub = "esp2/sub";     //发送主题（对方的订阅主题）
const char *ESP8266_sub = "set/switch/3"; //订阅主题（对方的发送主题）
WiFiClient espClient;
PubSubClient client(espClient); //定义客户端对象
const int led = 2;              //板载LED，右上角的蓝色小灯
const int btn = 4;              //按钮，实现按下按钮，主动给手机发送一个信息

void setup()
{
  Serial.begin(115200);
  Serial.println("");
  pinMode(btn, INPUT_PULLUP); //设置按钮引脚为上拉模式，低电平为按下
  pinMode(led, OUTPUT);       //设置LED引脚为输出模式
  digitalWrite(led, 1);       //初始LED为高电平（熄灭），板载LED为低电平点亮
                              //  gotowifi();                         //先连接到指定路由器
  smartConfig();              //智能配网
                              //   if (!autoConfig())
                              //  {
                              //    Serial.println("Start module");
                              //    smartConfig();
                              //  }
  initMQTT();                 //初始化MQTT客户端
  gotoMQTT();                 //连接到指定MQTT服务器，并订阅指定主题
}

void loop()
{
  //在服务器连接正常的前提下，如果按下按钮，主动给手机发送一个问候语
  if (client.connected())
  {
    if (digitalRead(btn) == 0)
    {
      client.publish(ESP8266_pub, "hello,Android");
      delay(200);
    }
  }
  client.loop(); //持续运行MQTT运行函数，完成接收数据和定时发送心跳包
  //这个只是一个演示，如果要做到稳定可靠
  //自己记得添加对WiFi连接状态的判断（路由器断线重连）
  //自己记得添加对MQTT服务器连接状态的判断（MQTT断线重连）
}

void gotowifi() //连接WiFi
{
  WiFi.begin(ssid, password);
  Serial.print("连接WiFi中");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi连接成功");
}

//配网
void smartConfig()
{
    WiFi.mode(WIFI_STA);
    Serial.println("\r\nWait for Smartconfig");
    WiFi.beginSmartConfig();
    while (1)
    {
        /*    就是这段   */
        delay(500);


        if (WiFi.smartConfigDone())
        {
            Serial.println("SmartConfig Success");
            Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
            Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
            break;
        }
    }
}


//bool autoConfig()
//{
//  WiFi.begin();
//  for (int i = 0; i < 20; i++)
//  {
//    int wstatus = WiFi.status();
//    if (wstatus == WL_CONNECTED)
//    {
//      Serial.println("AutoConfig Success");
//      Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
//      Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
//      WiFi.printDiag(Serial);
//      return true;
//      //break;
//    }
//    else
//    {
//      Serial.print("AutoConfig Waiting......");
//      Serial.println(wstatus);
//      delay(1000);
//    }
//  }
//  Serial.println("AutoConfig Faild!" );
//  return false;
//  //WiFi.printDiag(Serial);
//}
//void smartConfig()
//{
//  WiFi.mode(WIFI_STA);
//  Serial.println("\r\nWait for Smartconfig");
//  WiFi.beginSmartConfig();
//  while (1)
//  {
//    Serial.print(".");
//    if (WiFi.smartConfigDone())
//    {
//      Serial.println("SmartConfig Success");
//      Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
//      Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
//      WiFi.setAutoConnect(true);  // 设置自动连接
//      break;
//    }
//    delay(1000); // 这个地方一定要加延时，否则极易崩溃重启
//  }
//}

void initMQTT() //初始化MQTT设置
{
  //指定客户端要连接的MQTT服务器IP和端口
  client.setServer(mqtt_server, mqttPort);
  //绑定数据回调函数
  client.setCallback(callback);
}

void gotoMQTT() //连接MQTT服务器
{
  //用while循环执行到连接MQTT成功
  while (!client.connected())
  {
    Serial.println("连接MQTT服务器中");
    //连接MQTT服务器，提交ID，用户名，密码
    bool is = client.connect(ESP8266_ID, ESP8266_user, ESP8266_password);
    if (is)
    {
      Serial.println("连接MQTT服务器成功");
    }
    else
    {
      Serial.print("失败原因 ");
      Serial.print(client.state());
      delay(2000);
    }
  }
  client.subscribe(ESP8266_sub, 1); //添加订阅
}

void callback(char *topic, byte *payload, unsigned int length) //数据回调函数，监听数据接收
{
  Serial.print("消息来自订阅主题: ");
  Serial.println(topic);
  Serial.print("消息:");
  String data = "";
  for (int i = 0; i < length; i++)
  {
    data += (char)payload[i];
  }
  Serial.println(data);
  Serial.println();
  MQTT_Handler(data); //把接收的数据，传入处理函数执行分析处理
}

void MQTT_Handler(String data) //数据处理函数，执行对接收数据的分析处理
{
  if (data == "")
  {
    return;
  }
  if (data == "10D432594C7B234966B0CC4D24E57")
  {
    digitalWrite(led, 0);
    client.publish(ESP8266_pub, "ESP8266 is open");
  }
  else if (data == "00D432594C7B234966B0CC4D24E57")
  {
    digitalWrite(led, 1);
    client.publish(ESP8266_pub, "ESP8266 is close");
  }
}
