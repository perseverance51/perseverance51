/*
 * Wire库I2C基本读写程序
 * AT24C02读写操作
 *
*/

#include <Arduino.h>
#include <Wire.h> // 使用Wire库，需要包含头文件

#define SDA_PIN 4 // SDA引脚，默认gpio4(D2)
#define SCL_PIN 5 // SCL引脚，默认gpio5(D1)

const int16_t I2C_ADDR = 0x50; // AT24C02的7为IIC器件地址0x50

/* 描述：AT24C02写入一个字节函数
 * 参数：date_wr:    要写入的值
 *      WriteAddr:  要写入的地址
 * 返回值：通信成功：0 
 *        通信失败：1->数据溢出   2->发送addtess时从机接受到NACK 
 *                3->发送数据时接受到NACK  4->其他错误
 */
uint8_t at24c02_write(char data_wr, uint8_t WriteAddr)
{
  /* 1. 开始一次传输，设置I2C器件地址 */
  Wire.beginTransmission(I2C_ADDR);

  /* 2. 需要写入的位置 */
  Wire.write(WriteAddr);

  /* 3. 需要写入的值 */
  Wire.write(data_wr);

  /* 4. 完成一次I2C通信，默认发送一个停止位 */
  return Wire.endTransmission();
}

/* 描述：AT24C02读取一个字节函数
 * 参数：date_wr:    要读出值的存放指针
 *      WriteAddr:  要读出的地址
 * 返回值：通信成功：0  
 *        通信失败：1->数据溢出   2->发送address时从机接受到NACK 
 *                3->发送数据时接受到NACK  4->未接受到数据  5->其他
 */
uint8_t at24c02_read(char *data_wr, uint8_t ReadAddr)
{
  uint8_t t = 200;
  uint8_t ret = 0;
  /* 1. 开始一次传输，设置I2C器件地址 */
  Wire.beginTransmission(I2C_ADDR);

  /* 2. 需要读出的位置 */
  Wire.write(ReadAddr);

  /* 3. 完成一次I2C通信，发送一个开始位(即重发码) */
  ret = Wire.endTransmission(false);

  /* 4. 开始一次读取，设置I2C器件地址，读取AT24C02一个字节 */
  Wire.requestFrom(I2C_ADDR, 1);

  /* 5. 读出AT24C02返回的值，成功读取后写入缓存变量处，读取失败返回失败码 */
  while (!Wire.available())
  {
    t--;
    delay(1);
    if (t == 0)
    {
      return 1;
    }
  }
  *data_wr = Wire.read(); // receive a byte as character
  return ret;
}
void setup()
{
  /* 初始化串口波特率为115200 */
  Serial.begin(115200);

  /* 初始化IIC接口，不写入地址则默认为主设备 */
  Wire.begin(SDA_PIN, SCL_PIN);
}

char data_buf = 0x00;

void loop()
{

  /* 向AT24C02写入数据 */
  if (at24c02_write(0x55, 0x03) == 0)
    Serial.println("Write 0x55 to at24c02 in 0x03 \n");
  else
    Serial.println("Write at24c02 err \n");

  delay(500);

  /* 读取AT24C02里的数据 */
  if (at24c02_read(&data_buf, 0x03) == 0)
  {
    Serial.print("Read at24c02 in 0x03, data = 0x");
    Serial.println(data_buf, HEX);
  }
  else
    Serial.println("Read at24c02 err \n");

  while (1)
  {
    Serial.println("Hello");
    delay(500);
  }
}