/*二号机8  */

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN

#define LedPin 5    //LED灯
#define buttonPin 4 //发射器触发数据5引脚-按钮
int led_State = 0;
int buttonstate = 0;
int lastButtonState = 0;

int buttonPushCounter = 0;

char text[32] = {0}; //用于存放接收到的数据

const byte addresses[][6] = {"66666", "88888"}; //为双向通信创建两个通道地址，6为写通道，8为读取通道
//boolean buttonState = 0;
void setup()
{
  pinMode(LedPin, OUTPUT);
  pinMode(LedPin, led_State);

  pinMode(buttonPin, INPUT); //设置5号引脚为输入引脚

  Serial.begin(9600);
  radio.begin();
  radio.setChannel(114);                  // 设置信道(0-127)，114号通道
  radio.openWritingPipe(addresses[0]);    // 88888
  radio.openReadingPipe(1, addresses[1]); // 66666
  radio.setPALevel(RF24_PA_HIGH);
}
void loop()
{
  buttonstate = digitalRead(buttonPin);
  //  radio.startListening();
  if (buttonstate != lastButtonState)
  {
    radio.stopListening();
    Serial.print("按键已经触发，");
    if (buttonstate == HIGH)
    {
      radio.stopListening();
      // buttonPushCounter = 0 ;
      const char text2[13] = {"evergreen"};
      radio.write(&text2, sizeof(text2));
      delay(200);
      Serial.println("数据已发送。");

      //while (Serial.read() >= 0) {} //清空串口缓存
    }
    else
    {
      radio.startListening();
    }
  }

  else if (radio.available() > 0) // 是否有有效数据可以读取
  {

    radio.read(&text, sizeof(text));

    delay(500);
    String comdata = "";
    comdata += text;
    // Serial.println(comdata);

    if (comdata == "perseverance")
    {
      //        led_State = ~led_State;
      digitalWrite(LedPin, !digitalRead(LedPin)); //LedPin状态切换
      Serial.print("接收到：");
      Serial.println(comdata);

    }

  }
  else
  {
    radio.startListening();
  }
}
