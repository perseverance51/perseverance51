#include <EEPROM.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include "HeFeng.h"
#include <ArduinoJson.h>
HeFeng::HeFeng()
{
}

void HeFeng::NongLingDate(NongLi_Date *data, String DateAddress)
{ //获取农历数据HeFengCurrentData *data指向结构体中的数据
  std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
  client->setInsecure();
  HTTPClient https;
  // String url = DateAddress;
  Serial.print("[HTTPS] begin...nongli\n");

  if (https.begin(*client, DateAddress))
  { // HTTPS
    int httpCode = https.GET();
    if (httpCode > 0)
    {
      Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
      {
        String json = https.getString();
        Serial.println(F("Non Li")); //打印json数据
        StaticJsonDocument<256> doc;
        //  StaticJsonDocument<256> doc;
        deserializeJson(doc, json);
        // if (error)
        // { //获取失败
        //   Serial.print(F("deserializeJson() failed: "));
        //   Serial.println(error.f_str());
        //   return;
        // }
        EEPROM.begin(156);
        String nongli8 = doc["nongli"]; // "农历-年-月-日"
        //          data->nongliDate = nongli8;
        Serial.println(nongli8);
        //          String chinese = doc["chinese"]; // "二零二零年冬月二十二"
        //          data->chineseDate = chinese;
        //          Serial.println(chinese);
        //const char* ganzhi = doc["ganzhi"]; // "庚子"
        //const char* shengxiao = doc["shengxiao"]; // "鼠"
        unsigned int lunarYear = doc["lunarYear"]; // 2020
        byte lunarMonth = doc["lunarMonth"];
        byte lunarDay = doc["lunarDay"]; // 日
        Serial.println("Serivce Get date:" + String("") + lunarDay);
        unsigned int ReadLunarYear = EEPROM.read(60) + 2000;
        delay(10);
        EEPROM.commit();
        Serial.println("eeprom in year:" + String("") + ReadLunarYear);

        byte ReadLunarDay = EEPROM.read(66);
        delay(10);
        EEPROM.commit();
        Serial.println("eeprom in Day:" + String("") + ReadLunarDay);

        byte ReadLunarMonth = EEPROM.read(64);
        delay(10);
        EEPROM.commit();
        Serial.println("eeprom in month:" + String("") + ReadLunarMonth);

        if (ReadLunarYear != lunarYear)
        {
          Serial.println(F("存储的年份不相等"));
          EEPROM.write(60, lunarYear - 2000); //给EEPROM 第0位，写入lunarYear的值
          delay(10);
          EEPROM.commit(); //将数据提交，保存到EEPROM,
          data->data_lunarYear = lunarYear;
        }
        else
        {
          Serial.println("Serivce Get date:" + String("") + lunarYear);
          data->data_lunarYear = ReadLunarYear;
        }

        if ((int(ReadLunarMonth) != int(lunarMonth)))
        {
          Serial.println(F("存储的月份数据不相等"));
          if ((int(lunarMonth)) > 0)
          {
            EEPROM.write(64, lunarMonth); //给EEPROM 第0位，写入lunarYear的值
            delay(10);
            EEPROM.commit(); //将数据提交，保存到EEPROM
            data->data_lunarMonth = lunarMonth;
            Serial.println("Serivce Get date:" + String("") + lunarMonth);
          }
        }

        if (((int(ReadLunarDay)) != (int(lunarDay))) && ((int(lunarDay)) > 0))
        {
          // Serial.println(F("与存储的日期不相等"));
          // if ((int(lunarDay)) > 0)
          // {
          Serial.println(F("Serivce 大于 eeprom"));
          EEPROM.write(66, lunarDay); //给EEPROM 第72位，写入lunarYear的值
          delay(10);
          EEPROM.commit(); //将数据提交，保存到EEPROM,
          delay(10);
          data->data_lunarDay = lunarDay;
          Serial.println("Serivce Get date:" + String("") + lunarDay);
          // }
        }

        data->data_lunarMonth = lunarMonth;
        data->data_lunarDay = lunarDay;
        json.clear(); //清除JsonDocument并释放内存空间
      }
    }
    else
    {
      EEPROM.begin(156);
      Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
      byte lunarYear0 = EEPROM.read(60);
      delay(10);
      EEPROM.commit();

      data->data_lunarYear = lunarYear0 + 2000;
      Serial.println(lunarYear0);
      byte lunarMonth0 = EEPROM.read(64);
      delay(10);
      EEPROM.commit();

      data->data_lunarMonth = lunarMonth0;
      Serial.println("lunarMonth0的值:" + String("") + lunarMonth0);
      byte lunarDay0 = EEPROM.read(66);
      delay(10);
      EEPROM.commit();
      EEPROM.end(); //写入flash，并且释放内存空间
      data->data_lunarDay = lunarDay0;
      Serial.println("lunarDay0的值:" + String("") + lunarDay0);
    }
    https.end();
  }
  else
  {
    EEPROM.begin(156);
    Serial.printf("[HTTPS] Unable to connect\n");
    byte lunarYear0 = EEPROM.read(60);
    delay(10);
    EEPROM.commit();

    data->data_lunarYear = lunarYear0 + 2000;
    Serial.println("lunarYear0的值" + String("") + lunarYear0);
    byte lunarMonth0 = EEPROM.read(64);
    delay(10);
    EEPROM.commit();

    data->data_lunarMonth = lunarMonth0;
    Serial.print("lunarMonth0的值:" + String("") + lunarMonth0);

    byte lunarDay0 = EEPROM.read(66);
    delay(10);
    EEPROM.commit();
    EEPROM.end(); //写入flash，并且释放内存空间
    data->data_lunarDay = lunarDay0;
    Serial.print("lunarDay0的值:" + String("") + lunarDay0);
  }
}

void HeFeng::doUpdateCurr(CurrentData *data, String HEFENG_KEY, String HEFENG_LOCATION)
{ //获取实时天气状况
  //指纹认证：f5d533d8da35ebe8b0fbf5d5b9530b82e75c0cad
  std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);

  //client->setFingerprint("f5d533d8da35ebe8b0fbf5d5b9530b82e75c0cad");
  client->setInsecure();
  HTTPClient https;
  String url = "https://devapi.qweather.com/v7/weather/now?lang=en&location=" + HEFENG_LOCATION + "&key=" + HEFENG_KEY + "&gzip=n"; //V7版本
  Serial.print("[HTTPS] begin...now Weather\n");

  if (https.begin(*client, url))
  { // HTTPS
    int httpCode = https.GET();
    if (httpCode > 0)
    {
      // HTTP header has been send and Server response header has been handled
      //      Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

      if ((httpCode == HTTP_CODE_OK) || (httpCode == HTTP_CODE_MOVED_PERMANENTLY))
      {
        String json = https.getString();
        Serial.println(F("Now Weather")); //打印json数据
        EEPROM.begin(156);
        // StaticJsonDocument<768> doc1; //静态json缓冲区
        DynamicJsonDocument doc1(768);
        DeserializationError error = deserializeJson(doc1, json);
        if (error)
        {
          Serial.print(F("deserializeJson() failed: "));
          Serial.println(error.f_str());
          return;
        }
        JsonObject root = doc1["now"];
        //        String TodayNow = root["obsTime"];
        //        data->Today = TodayNow.substring(0, 10); //2021-03-01T13:17+08:00返回 0和 10 之间的
        //        Serial.println(TodayNow.substring(0, 10));
        byte now_temp = root["temp"]; //获取当天温度
        byte Read_temp = EEPROM.read(30);
        if (int(Read_temp) != int(now_temp))
        {
          if ((int(now_temp)) != 0)
          {
            Serial.println(now_temp);
            data->tmp = now_temp;
            EEPROM.write(30, now_temp); //给EEPROM 第0位，写入lunarYear的值
            EEPROM.commit();
          }
        }
        Serial.println(now_temp);
        data->tmp = now_temp;

        String now_feelsLike = root["feelsLike"];
        //Serial.println(now_feelsLike);
        data->fl = now_feelsLike; //获取体感温度

        byte now_humidity = root["humidity"]; //获取湿度
        byte Read_humidity = EEPROM.read(40);
        if ((int(Read_humidity)) != (int(now_humidity)))
        {
          Serial.println(now_humidity);
          data->hum = now_humidity;
          EEPROM.write(40, now_humidity); //给EEPROM 第0位，写入lunarYear的值
          EEPROM.commit();
        }
        Serial.println(now_humidity);
        data->hum = now_humidity;

        byte now_windScale = root["windScale"]; //获取风力大小
        byte Read_windScale = EEPROM.read(40);
        if ((int(Read_windScale)) != (int(now_windScale)))
        {
          EEPROM.write(50, now_windScale); //给EEPROM 第0位，写入lunarYear的值
          EEPROM.commit();
        }
        Serial.println(now_windScale);
        data->wind_sc = now_windScale;

        String now_icon = root["icon"]; //获取天气图标代码
        char now_icon0[4];
        strcpy(now_icon0, now_icon.c_str()); //把String now_icon赋值给char now_icon0;
        int address = 80;
        int len = strlen(now_icon0);
        for (int i = 0; i < len; i++)
        { //now_icon0长度传递给eeprom寄存address;
          address++;
          EEPROM.write(address, now_icon0[i]);
        }
        EEPROM.commit();
        //EEPROM.end();                          //写入flash，并且释放内存空间
        String meteoconIcon = getMeteoconIcon(now_icon);
        String now_text = root["text"];
        Serial.println(now_text);
        data->cond_txt = now_text;
        data->iconMeteoCon = meteoconIcon;
        json.clear(); //清除JsonDocument并释放内存空间
      }
    }
    else
    {
      EEPROM.begin(156);
      Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
      data->tmp = -1;
      // data->fl = "-1";
      data->hum = 0;
      byte wind_sc0 = EEPROM.read(50);
      data->wind_sc = wind_sc0;
      EEPROM.commit();
      Serial.println("eerpom in date:" + String("") + wind_sc0);
      int address = 80;
      char now_icon1[4];
      for (int i = 0; i < 3; i++)
      { //now_icon0长度传递给eeprom寄存address;
        address++;
        now_icon1[i] = EEPROM.read(address);
      }
      //EEPROM.commit();
      EEPROM.end(); //写入flash，并且释放内存空间
      String meteoconIcon = getMeteoconIcon(now_icon1);
      data->iconMeteoCon = meteoconIcon;
      data->cond_txt = "eeprom";
    }
    https.end();
  }
  else
  {
    EEPROM.begin(156);
    Serial.printf("[HTTPS] Unable to connect\n");
    byte now_temp0 = EEPROM.read(30);
    data->tmp = now_temp0;
    EEPROM.commit();

    Serial.println("Temp:" + now_temp0);
    //data->tmp = 0;
    // data->fl = "-1";

    data->hum = 0;
    byte wind_sc0 = EEPROM.read(50);
    EEPROM.commit();
    data->wind_sc = wind_sc0;
    Serial.println("wind_sc0:" + wind_sc0);
    // data->wind_sc = 0;
    data->cond_txt = "eeprom"; //无网络就将气象名称显示eeprom字样以区别有网络状态
    char now_icon1[4];
    int address = 80;
    for (int i = 0; i < 3; i++)
    { //now_icon0长度传递给eeprom寄存address;
      address++;
      now_icon1[i] = EEPROM.read(address);
    }

    EEPROM.end(); //写入flash，并且释放内存空间
    String meteoconIcon = getMeteoconIcon(now_icon1);
    data->iconMeteoCon = meteoconIcon;
    // data->iconMeteoCon = ")";
  }
}

void HeFeng::doUpdateFore(ForeData *data, String HEFENG_KEY, String HEFENG_LOCATION)
{ //获取3天预报天气信息
  std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
  client->setInsecure();
  HTTPClient https;
  String url = "https://devapi.qweather.com/v7/weather/3d?lang=en&location=" + HEFENG_LOCATION + "&key=" + HEFENG_KEY + "&gzip=n";
  Serial.print("[HTTPS] begin...forecast\n");
  if (https.begin(*client, url))
  { // HTTPS
    int httpCode = https.GET();
    if (httpCode > 0)
    {
      //      Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
      {
        String json = https.getString();
        Serial.println(F("3Day Weather"));
        DynamicJsonDocument doc(3072);                           //动态json缓冲区
        DeserializationError error = deserializeJson(doc, json); //反序列化JSON数据
        if (!error)
        { //检查反序列化是否成功
          JsonObject root = doc.as<JsonObject>();
          delay(50);
          for (int i = 0; i < 3; i++)
          {
            String datestr = root["daily"][i]["fxDate"]; //获取天3天天气日期，成功。
            delay(50);
            Serial.println(datestr);
            if (!(datestr.equals("null")))
            {
              data[i].datestr = datestr.substring(5, datestr.length());
            }

            String tmp_min = root["daily"][i]["tempMin"];
            Serial.println(tmp_min);
            if (!(tmp_min.equals("null")))
            {
              data[i].tmp_min = tmp_min;
            }

            String tmp_max = root["daily"][i]["tempMax"];
            Serial.println(tmp_max);
            if (!(tmp_max.equals("null")))
            {
              data[i].tmp_max = tmp_max;
            }

            String cond_code = root["daily"][i]["iconDay"]; //获取天气图标
            Serial.println(cond_code);
            if (!(cond_code.equals("null")))
            {
              String meteoconIcon = getMeteoconIcon(cond_code);
              data[i].iconMeteoCon = meteoconIcon;
            }
          }
          return;
        }
        json.clear(); //清除JsonDocument并释放内存空间
      }
    }
    else
    {
      Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
      int i;
      for (i = 0; i < 3; i++)
      {
        data[i].tmp_min = "-1";
        data[i].tmp_max = "-1";
        data[i].datestr = "N/A";
        data[i].iconMeteoCon = ")";
      }
    }
    https.end();
  }
  else
  {
    Serial.printf("[HTTPS] Unable to connect\n");
    int i;
    for (i = 0; i < 3; i++)
    {
      data[i].tmp_min = "-1";
      data[i].tmp_max = "-1";
      data[i].datestr = "N/A";
      data[i].iconMeteoCon = ")";
    }
  }
}
void HeFeng::WorkDate(WorkData *data, String WorkDate)
{ //获取当天日期信息
  WiFiClient client;
  HTTPClient http;
  Serial.print("[HTTPS] begin...Today\n");
  if (http.begin(client, WorkDate))
  {
    int httpCode = http.GET();
    // http.addHeader("Content-Type", "text/plain");     //设置消息类型
    // int httpCode = http.POST("Message from ESP8266"); //发送请求

    if (httpCode > 0)
    {
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
      {
        String json = http.getString();
        EEPROM.begin(112);
        Serial.println(json);
        StaticJsonDocument<96> doc; //静态json缓冲区
        deserializeJson(doc, json); //反序列化JSON数据
        String TodayRead = Read_String(EEPROM.read(100), 101);
        EEPROM.commit();
        Serial.println("EEPROM中的数据:" + String("") + TodayRead);
        String Today = doc["sysTime2"];          //  "2021-03-03 01:28:24"
        Serial.println(Today.substring(0, 10));  //年月日
        Serial.println(Today.substring(11, 19)); //时分秒
        //       data->Date_Time = Today.substring(11, 19);
        if (!(TodayRead.equals(Today.substring(0, 10))))
        {
          Write_String(100, 101, Today.substring(0, 10)); //将数据存入eeprom
          delay(30);
        }
        data->workdate = Today.substring(0, 10);
      }
    }
    else
    {
      Serial.println(F("以下为EEPROM读取的信息："));
      EEPROM.begin(112);
      String Today = Read_String(EEPROM.read(100), 101);
      EEPROM.commit();
      delay(30);
      Serial.print(Today);
      data->workdate = Today;
    }
    http.end();
  }
}

//a写入字符串长度，b是起始位，str为要保存的字符串
void HeFeng::Write_String(int a, int b, String str)
{
  EEPROM.write(a, str.length()); //EEPROM第a位，写入str字符串的长度
  //把str所有数据逐个保存在EEPROM
  int c = str.length();
  for (int i = 0; i < c; i++)
  {
    EEPROM.write(b + i, str[i]);
    delay(10);
  }
  EEPROM.commit();
}
//a位是字符串长度，b是起始位
String HeFeng::Read_String(int a, int b)
{
  String data = "";
  //从EEPROM中逐个取出每一位的值，并链接
  for (int i = 0; i < a; i++)
  {
    data += char(EEPROM.read(b + i));
    delay(10);
  }
  return data;
}

String HeFeng::getMeteoconIcon(String cond_code)
{ //获取天气图标
  if (cond_code == "100" || cond_code == "0" || cond_code == "2")
  { //白天晴
    return "B";
  }
  if (cond_code == "150" || cond_code == "1")
  { //夜晚 Clear
    return "C";
  }
  if (cond_code == "999")
  { //N/A
    return ")";
  }
  if (cond_code == "9" || cond_code == "4")
  { //多云或者用D
    return "A";
  }
  if (cond_code == "500" || cond_code == "30")
  { //薄雾
    return "E";
  }
  if (cond_code == "503" || cond_code == "504" || cond_code == "507" || cond_code == "508" || cond_code == "26" || cond_code == "27" || cond_code == "28" || cond_code == "29")
  { //扬沙//浮尘//沙尘暴//强沙尘暴
    return "F";
  }
  if (cond_code == "499" || cond_code == "901")
  { //雪
    return "G";
  }
  if (cond_code == "102" || cond_code == "103" || cond_code == "5")
  { //晴间多云
    return "H";
  }
  if (cond_code == "153" || cond_code == "8")
  { //夜晚气象，阴
    return "I";
  }
  //     if (cond_code == "153") {//夜晚气象，阴
  //    return "4";
  //  }
  //  if (cond_code == "154") {//夜晚气象，阴
  //    return "5";
  //  }
  if (cond_code == "502" || cond_code == "511" || cond_code == "512" || cond_code == "513" || cond_code == "31")
  { //霾//中度霾//重度霾//严重霾
    return "L";
  }
  if (cond_code == "501" || cond_code == "509" || cond_code == "510" || cond_code == "514" || cond_code == "515" || cond_code == "30" || cond_code == "315" || cond_code == "316" || cond_code == "317" || cond_code == "318" || cond_code == "18")
  { //雾//浓雾//强浓雾//大雾//特强浓雾
    return "M";
  }
  if (cond_code == "104" || cond_code == "154" || cond_code == "7")
  { //白天阴
    return "N";
  }
  if (cond_code == "302" || cond_code == "12")
  { //雷阵雨
    return "O";
  }
  if (cond_code == "303")
  { //雷阵雨//强雷阵雨
    return "P";
  }
  if (cond_code == "305" || cond_code == "308" || cond_code == "309" || cond_code == "314" || cond_code == "399" || cond_code == "13")
  { //小雨//极端降雨//毛毛雨/细雨//小到中雨//雨
    return "Q";
  }
  if (cond_code == "306" || cond_code == "307" || cond_code == "310" || cond_code == "311" || cond_code == "312" || cond_code == "14")
  { //中雨//大雨//暴雨//大暴雨//特大暴雨//中到大雨
    return "R";
  }
  //  if (cond_code == "315" || cond_code == "316" || cond_code == "317" || cond_code == "318" || cond_code == "18" ) { //大到暴雨//暴雨到大暴雨//大暴雨到特大暴雨
  //    return "8";//显示黑色图标不可见，
  //  }
  if (cond_code == "200" || cond_code == "201" || cond_code == "202" || cond_code == "203" || cond_code == "204" || cond_code == "205" || cond_code == "206" || cond_code == "207" || cond_code == "208" || cond_code == "209" || cond_code == "210" || cond_code == "211" || cond_code == "212")
  {
    return "S";
  }
  if (cond_code == "300" || cond_code == "301")
  { //阵雨
    return "T";
  }
  if (cond_code == "400" || cond_code == "404" || cond_code == "408" || cond_code == "22")
  { //小雪
    return "U";
  }
  if (cond_code == "407" || cond_code == "21")
  { //阵雪
    return "V";
  }
  if (cond_code == "401" || cond_code == "402" || cond_code == "403" || cond_code == "409" || cond_code == "410" || cond_code == "23" || cond_code == "24" || cond_code == "25")
  { //中雪//大雪//暴雪//中到大雪//大到暴雪
    return "W";
  }
  if (cond_code == "304" || cond_code == "405" || cond_code == "406" || cond_code == "20")
  { //雷阵雨伴有//冻雨//雨雪天气//阵雨夹雪
    return "X";
  }
  if (cond_code == "101" || cond_code == "4")
  { //多云
    return "Y";
  }
  if (cond_code == "457" || cond_code == "21")
  { //阵雪
    return "#";
  }
  if (cond_code == "313" || cond_code == "19")
  { //冻雨
    return "$";
  }
  if (cond_code == "900")
  { //显示黑色图标不可见，雨夹雪
    return "“";
  }
  return ")"; //未知
}
