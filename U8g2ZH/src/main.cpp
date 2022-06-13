#include <Arduino.h>
/**********图像显示库*************/

#include <U8g2lib.h>
#include <Wire.h>
#define SDA (4) //nodemcu引脚连接,使用的是硬件IIC
#define SCL (5) //

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R2, /* reset=*/U8X8_PIN_NONE, /* clock=*/SCL, /* data=*/SDA); //配置构造函数，使用full frame buffer

void setup()
{
  Serial.begin(115200);
  Serial.println("");
  u8g2.begin();
  u8g2.enableUTF8Print(); //开启UTF-8字符显示，即开启中文的显示
}

void loop()
{
  String str0 = "开启UTF-8字符显示，蜀道之难难于上青天";
  u8g2.setFont(u8g2_font_wqy14_t_gb2312); //u8g2_font_wqy14_t_gb2312a
  if (str0.indexOf('，') < 128)
  {
    u8g2.setCursor(0, 22); //22居中显示
    u8g2.println(str0.substring(0, str0.indexOf('，') - 2));
    u8g2.setCursor(0, 38); //一行可以正常显示9个汉字
    u8g2.println(str0.substring(str0.indexOf('，')));
  }

  u8g2.sendBuffer();
}