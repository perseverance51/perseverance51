#include <Arduino.h>

char char1[10] = {'h', 'e', 'l', 'l', 'o', '!'};
char char2[10];
char char3[10];

char *xch1 = (char *)"welcome!"; //char*是一个变量，而右边是常量，常量赋值给变量需要强制类型转换或者使用const修饰变量
//const char* p="hello";
char *xch2;
char *xch3;

String str1 = "world!";
String str2;
String str3;
String *xstr;
void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("test!");
  //------------------char[]的赋值
  strcpy(char2, str1.c_str()); //String转char数组方法一
  Serial.print("char2:");
  Serial.println(char2);
  for (unsigned int i = 0; i < str1.length(); i++)
  {
    char3[i] = str1[i];
  }
  Serial.print("char3:");
  Serial.println(char3);

  //-----------------char *xch的赋值
  xch2 = char1;                //将char数组名赋值给char*，数组名为首元素地址。
  xch3 = (char *)str1.c_str(); //对char*进行赋值的时候，必须进行强制类型转换
  Serial.print("xch3:");
  Serial.println(xch3);
  const char *xch4 = str1.c_str(); //在定义char*变量的同时进行初始化赋值
  Serial.print("xch4:");
  Serial.println(xch4);
  for (unsigned int i = 0; i < strlen(xch4); i++)
  {
    Serial.print(xch4[i]);
  }
  Serial.println();
  //-----------------String str2的赋值
  str2 = String(char1); //char*转String
  Serial.print("str2:");
  Serial.println(str2);
  str3 = char2; //隐式转换
  Serial.print("str3:");
  Serial.println(str3);
  Serial.println(str3.c_str());            //println打印String可加可不加
  Serial.printf("str3= %s", str3.c_str()); //格式化打印String一定要加.c_str()转换
}

void loop()
{
}