#include <Arduino.h>

const byte GPIOPins[] = {2, 6, 7, 8, 9, 12};
const char GPIOPins2[] = {2, 6, 7, 8, 9, 12};
void setup()
{
  Serial.begin(115200);
  for (uint8_t i = 0; i < 6; i++)
  {
    pinMode(GPIOPins[6], OUTPUT);
    digitalWrite(GPIOPins[6], LOW);
  }
}

void loop()
{
  uint8_t charsize = sizeof(GPIOPins2) / sizeof(GPIOPins2[0]);
  uint8_t bytesize = sizeof(GPIOPins) / sizeof(GPIOPins[0]);
  unsigned char char_strlen = strlen(GPIOPins2);
  uint8_t byte_strlen = strlen(GPIOPins);
  Serial.print("charsize =");
  Serial.println(charsize);
  Serial.print("char_strlen = ");
  Serial.println(char_strlen);

  Serial.print("bytesize = ");
  Serial.println(bytesize);
  Serial.print("byte strlen =");
  Serial.println(byte_strlen);
  delay(5000);
}