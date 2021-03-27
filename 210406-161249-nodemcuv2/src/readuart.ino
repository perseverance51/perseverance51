const char hz[][32] = {
    "当前电压:", //0-

    "Arduino 电压表演示程序", //1

    "接收到的数据:", //2

};

String str1 = "接收到的数据:";
void setup()
{
  Serial.begin(115200);
}

void loop()
{
  String comdata;
  while (Serial.available() > 0)
  {
    comdata += char(Serial.read());
    delay(2);
  }
  if (comdata.length() > 0)
  {

    Serial.println(hz[2]);

    Serial.println(comdata);
    comdata = "";
  }
}
