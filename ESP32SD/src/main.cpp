/*
 如果使用hard SPI（HSPI）接口需要注意以下几点：
 1.烧写程序前先断开SD卡供电，否则无法进行程序烧录。
 2.并且SD卡需要在复位或重启后再给SD卡供电。否则ESP32会无限重启。
 如果使用soft SPI（VSPI）接口不存在以上问题。
 不定义SPI引脚直接使用SD.begin()；默认是VSPI接口。
*/

#include <Arduino.h>
//引用SD相关库
#include <SD.h>
#include <FS.h>
#include <SPI.h>
//HSPI接口和VSPI接口二选一
//#define  hspi
#define  vspi
#ifdef vspi
SPIClass sdSPI(VSPI);
#define SD_MISO     19
#define SD_MOSI     23
#define SD_SCLK     18
#define SD_CS       5

#else
SPIClass sdSPI(HSPI);
#define SD_MISO     12
#define SD_MOSI     13
#define SD_SCLK     14
#define SD_CS       15

#endif
void SD_init();
 
void setup()
{
  Serial.begin(115200);
  delay(500);
#ifdef  hspi 
  Serial.println("请插入内存卡");
  delay(12000);
#endif 
  SD_init();

}
 
void loop() {
  //打印存储卡信息
  Serial.printf("存储卡总大小是： %lluMB \n", SD.cardSize() / (1024 * 1024)); // "/ (1024 * 1024)"可以换成">> 20"
  Serial.printf("文件系统总大小是： %lluB \n", SD.totalBytes());
  Serial.printf("文件系统已用大小是： %lluB \n", SD.usedBytes());
  delay(5000);
}
 
 
void SD_init() {
  //挂载文件系统
   sdSPI.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
  if (!SD.begin(SD_CS, sdSPI))
  {
    // if(!SD.begin()){
    Serial.println("存储卡挂载失败");
    return;
  }
  uint8_t cardType = SD.cardType();
 
  if (cardType == CARD_NONE)
  {
    Serial.println("未连接存储卡");
    return;
  }
  else if (cardType == CARD_MMC)
  {
    Serial.println("挂载了MMC卡");
  }
  else if (cardType == CARD_SD)
  {
    Serial.println("挂载了SDSC卡");
  }
  else if (cardType == CARD_SDHC)
  {
    Serial.println("挂载了SDHC卡");
  }
  else
  {
    Serial.println("挂载了未知存储卡");
  }
}