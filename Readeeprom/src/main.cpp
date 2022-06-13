#include <Arduino.h>
#include <EEPROM.h>
  //数据顺序注意，将大的数据放在前面，不然读数据时，大的数据取出来将不完整或有乱码
struct MyObject {
  float field1;
  int field2;
  byte Height; 
    char name[15];
};

void setup() {
  Serial.begin(9600);
delay(5000);
  // for (int i = 0 ; i < EEPROM.length() ; i++) {//清空eeprom
  //   EEPROM.write(i, 0);
  // }
  float f = 123.456f;  //将浮点数存储到EEPROM中.
  int eeAddress = 0;   //存储的起始地址
  //地址和对象
  EEPROM.put(eeAddress, f);
  Serial.println("Written float data type!");
  //数据顺序注意，将大的数据放在前面，不然读数据时，大的数据取出来将不完整或有乱码
  MyObject customVar = {
   
    3.14f,
    9527,
    64 ,
    "perseverance52"
  };
  eeAddress += sizeof(float); //移动地址到浮动'f'之后的下一个字节。
  EEPROM.put(eeAddress, customVar);//继续存储结构体数据
  Serial.print("Written custom data type! \n\nView the example sketch eeprom_get to see how you can retrieve the values!");
  Serial.println(customVar.name);
}

void loop() { }