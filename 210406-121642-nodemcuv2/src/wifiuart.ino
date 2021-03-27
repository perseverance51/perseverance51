#include <ESP8266WiFi.h>
#include <aJSON.h>
String comdata = "";
int numdata[4] = {0};
int i = 0, j = 0;
float xinlv = 0;
float tiwen = 0;
float num = 0;
//=============  此处必须修改============
String DEVICEID = "21372";   // 你的设备ID=======
String APIKEY = "2cdc4c07b"; // 设备密码==
String INPUTID = "19282";    //接口ID1==============
String INPUTID1 = "19346";   //接口ID2，同时上传两个接口时要用==============

unsigned long lastCheckStatusTime = 0;       //记录上次报到时间
unsigned long lastUpdateTime = 0;            //记录上次上传数据时间
const unsigned long postingInterval = 40000; // 每隔40秒向服务器报到一次
const unsigned long updateInterval = 5000;   // 数据上传间隔时间5秒
unsigned long checkoutTime = 0;              //登出时间

//=============wifi配置初始化，此处也需要修改===========
const char *ssid = "MERCURY_D268G";  //无线名称
const char *password = "pba5ayzk";   //无线密码
const char *host = "www.bigiot.net"; //通讯地址
const int httpPort = 8181;           //TCP协议接口

void setup()
{
  Serial.begin(115200);
  delay(1000);
  WiFi.begin(ssid, password); //这里就可以连接上网络
}
WiFiClient client;

void loop()
{
  Serial.flush();
  Serial_receivestr();
  //============wifi连接测试================
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    //Serial.print(".");
  }
  //==============使用TCP协议==============
  if (!client.connected())
  {
    if (!client.connect(host, httpPort))
    {
      //Serial.println("connection failed");
      delay(5000);
      return;
    }
  }

  //================ 每一定时间查询一次设备在线状态，同时替代心跳=======================
  if (millis() - lastCheckStatusTime > postingInterval)
  {
    checkStatus();
  }
  //checkout 50ms 后 checkin
  if (checkoutTime != 0 && millis() - checkoutTime > 50)
  {
    checkIn();
    checkoutTime = 0;
  }
  if (millis() - lastUpdateTime > updateInterval)
  {
    //    float val=110;//定义变量
    //     update1(DEVICEID, INPUTID, val);
    Serial_receivestr();
    //    Serial_hander();
    lastUpdateTime = millis();
  }
  //=================读取每个服务器的回应并打印在窗口======================
  if (client.available())
  {
    String inputString = client.readStringUntil('\n'); //检测json数据是否完整
    inputString.trim();
    //Serial.println(inputString);
    int len = inputString.length() + 1;

    if (inputString.startsWith("{") && inputString.endsWith("}"))
    { //净化json数据
      char jsonString[len];
      inputString.toCharArray(jsonString, len);
      aJsonObject *msg = aJson.parse(jsonString);
      processMessage(msg);
      aJson.deleteItem(msg);
    }
  }
}

void Serial_receivestr()
{
  while (Serial.available() > 0) //当发现缓存中有数据时，将数据送至字符数组a中
  {
    //   comdata +=char(Serial.read());

    comdata = Serial.readStringUntil('\n');
    delay(3);
    //    comdata.trim();
    // Serial.flush();
    //    delay(3);
  }

  comdata += char(Serial.read());
  delay(2);

  Serial.println(comdata);
  unsigned int len = comdata.length();

  for (int i = 0; i < len; i++)
  {
    if (comdata[i] == '.')
      continue;
    num = num * 10 + comdata[i].toInt() - '0';
  }
  //        comdata = String("");
  //        for(int i = 0; i < 4; i++)
  //               numdata[i] = 0;
  //        for(int i = 0; i < 3; i++)
  //          num=numdata[i+1]*10+numdata[i];
  // comdata = "";
  if (num > 1000)
    tiwen = num / 100;
  else if (num > 0)
    xinlv = num;
  num = 0;
  update2(DEVICEID, INPUTID1, tiwen, INPUTID, xinlv);
  //        tiwen=0,xinlv=0;
  delay(100);
}

//=========================处理来自贝壳物联来的数据=========================
void processMessage(aJsonObject *msg)
{
  aJsonObject *method = aJson.getObjectItem(msg, "M");
  aJsonObject *content = aJson.getObjectItem(msg, "C");
  aJsonObject *client_id = aJson.getObjectItem(msg, "ID");
  if (!method)
  {
    return;
  }
  String M = method->valuestring;
  if (M == "WELCOME TO BIGIOT")
  {
    checkOut();
    checkoutTime = millis();
    return;
  }
}

//=============查询设备在线状态===========
void checkStatus()
{
  String msg = "{\"M\":\"status\"}\n";
  Serial.println(msg);
  lastCheckStatusTime = millis();
}
//=============设备登录===========
void checkIn()
{
  String msg = "{\"M\":\"checkin\",\"ID\":\"" + DEVICEID + "\",\"K\":\"" + APIKEY + "\"}\n";
  Serial.println(msg);
  lastCheckStatusTime = millis();
}
//=============强制设备下线，用消除设备掉线延时===========
void checkOut()
{
  String msg = "{\"M\":\"checkout\",\"ID\":\"" + DEVICEID + "\",\"K\":\"" + APIKEY + "\"}\n";
  Serial.println(msg);
}

//上传一个接口数据
//{"M":"update","ID":"2","V":{"2":"120"}}\n

void update1(String did, String inputid, float value)
{
  String msg1 = "{\"M\":\"update\",\"ID\":\"" + did + "\",\"V\":{\"" + inputid + "\":\"" + value + "\"}}\n";
  Serial.println(msg1);
}

//同时上传两个接口数据调用此函数
//{"M":"update","ID":"112","V":{"6":"1","36":"116"}}\n
void update2(String did, String inputid1, float value1, String inputid2, float value2)
{
  String msg1 = "{\"M\":\"update\",\"ID\":\"" + did + "\",\"V\":{\"" + inputid1 + "\":\"" + value1 + "\",\"" + inputid2 + "\":\"" + value2 + "\"}}\n";
  //Serial.print(msg1);
  Serial.println(msg1);
}
