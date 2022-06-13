#include <Arduino.h>
#include <Streaming.h>
#include <string.h>
#include <ESP8266WiFi.h>

int numb = 1234;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  //Serial.printf("Connected, IP address: %s/%s\n", WiFi.localIP().toString().c_str(), WiFi.subnetMask().toString().c_str()); //Get ip and subnet mask
  //Serial.printf("Connected, MAC address: %s\n", WiFi.macAddress().c_str());                                                 //Get the local mac address
}

void loop()
{
  char *numb_str = NULL;
  sprintf(numb_str, "%d", numb); //把整数打印到字符串中，spritnf 在大多数场合可以替代itoa
  char s[10];
  itoa(numb, s, 10);

  // put your main code here, to run repeatedly:
  String str = "perseverance";
  String str0 = str.substring(0, 5); //（从起始点截取某个长度的字符串）
  Serial.println(str0.lastIndexOf("ce"));
  Serial.println(str0.indexOf());
  String str1 = str.substring(3);
  String str2 = str.substring(5, 10);
  const char *str3 = "no substr class";
  String sdate = "hell world";
  char cdate[8] = "goodbye";
  char str4[10], str5[10];
  const char *str6 = str.c_str();
  Serial.println(str6);
  strcpy(str4, sdate.c_str()); //将String类型转char类型,会包含空字符
  unsigned int str4_length = sizeof(str4) / sizeof(char);
  Serial.printf("str4_length = %d \n", str4_length);
  strncpy(str5, sdate.c_str(), 4); //截取指定String类型数据的长度
  const char *p = sdate.c_str();
  //strcpy(str6, "good");
  //memcpy(str6, cdate, 4);
  //strcpy_P(str6, cdate);
  unsigned int str5_length = sizeof(str5) / sizeof(char);
  Serial.println(str);
  Serial.printf("str5_length = %d \n", str5_length);
  Serial.printf("str0= %s , str1 = %s  , str2= %s \n str3 = %s ,str4 = %s  \n", str0.c_str(), str1.c_str(), str2.c_str(), str3, str4);
  Serial.printf("str5 = %s \n", str5);
  Serial.printf("sdate = %s \n", sdate.c_str());
  Serial.printf("cdate = %s  \n", cdate);
  int len = sdate.length();
  for (int i = 0; i < len; i++)
  {
    Serial.printf("p = %c \n", *p++);
  }
  Serial << "hello world ";
  Serial.println("");
  delay(5000);
}