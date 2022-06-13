#include <Arduino.h>
unsigned char ch[] = "智者；学习；“知识”，上善若水。见善而怠，时至而疑，知非而处，此三者，道之所止也。";
String st = "智者；学习；“知识”，上善若水。见善而怠，时至而疑，知非而处，此三者，道之所止也。";
//不能判断连续的中文标点符号，例如“，，，，。，”
int ZH(unsigned char *ch4);
int libian(String str0);
void setup()
{
  int num = 0;
  int num2 = 0;
  Serial.begin(115200);
  Serial.println();
  Serial.println("print:");
  Serial.println(st);
  Serial.println(st.length());
  Serial.println();
  num = ZH(ch);
  Serial.printf("\n中文标点符号有%d个", num);
  num2 = libian(st);
  Serial.printf("\n中文标点符号有%d个", num2);
}
void loop()
{
}
int ZH(unsigned char *ch4)
{
  int i = 0, j = 0, tem = 0;

  while (ch4[i] != '\0')
  {
    j = i + 1;
    if (j == '\0')
    {
      break;
    }
    else if ((ch4[i] <= 0xa9) && (ch4[i] >= 0xa1))
    {
      if ((ch4[j] >= 0x40) && (ch4[j] <= 0xff))
      {
        tem++;
      }
      i = i + 2;
    }
    else
      i++;
  }
  return tem;
}
int libian(String str0)
{
  int temp = 0;
  String str = str0;
  if (str.indexOf('，') != -1)
  {
    temp++;
    str.replace("，", "");
  }
  if (str.indexOf('；') != -1)
  {
    temp++;
    str.replace("；", "");
  }
  if (str.indexOf('。') != -1)
  {
    temp++;
    str.replace("。", "");
  }
  if (str.indexOf('“') != -1)
  {
    temp++;
    str.replace("“", "");
  }
  if (str.indexOf('”') != -1)
  {
    temp++;
    str.replace("”", "");
  }
  Serial.println();
  Serial.println(str);
  Serial.println(str.length());
  int jishu = (str0.length() - str.length()) / 3;
  return jishu;
}