#include <Arduino.h>
#include <EEPROM.h>

struct MyObject{
 
  float field1;
  int field2;
  byte Height;
   char name[15];//
};

void setup(){
   String  value="";
  float f = 0.00f;   //用于存放将要读取的eeprom浮点数据
  int eeAddress = 0; //从EEPROM地址0开始读取数据
  Serial.begin( 9600 );
  Serial.print( "Read float from EEPROM: " );
  delay(5000);
  //Get the float data from the EEPROM at position 'eeAddress'
  EEPROM.get( eeAddress, f );
  Serial.println( f, 3 );  //如果EEPROM中的数据不是一个有效的浮点数，则可能打印'ovf, nan'。

  eeAddress = sizeof(float); //获取浮点数的大小，并赋值给将要访问的eeprom地址
  MyObject customVar; //对结构体进行初始化，用于接收存放将要读取的eeprom中的数据
  EEPROM.get( eeAddress, customVar );//从eeprom中获取数据，并使用结构体来存放接收
  Serial.println( "Read custom object from EEPROM: " );
   Serial.println(sizeof(float));//4
    Serial.println(sizeof(int));//2
    Serial.println(sizeof(byte));//1
  Serial.println( customVar.name);//打印获取的数据
 Serial.println( customVar.field1 );
  Serial.println( customVar.field2 );
 Serial.println( customVar.Height );

  //读取数据方式二
 for (int j=11;j<26;j++){//通过地址直接访问eeprom，读取数据
   value += (char)EEPROM.read(j);}
  Serial.println(value);
}

void loop(){ }