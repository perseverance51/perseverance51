#include <Arduino.h>
union data
{
  int n;
  char ch;
  short m;
};

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  union data a;
  printf("%d, %d\n", sizeof(a), sizeof(union data));
  delay(1500);
  a.n = 0x40;
  printf("%X, %c, %hX\n", a.n, a.ch, a.m);
  delay(1500);
  a.ch = '9';
  printf("%X, %c, %hX\n", a.n, a.ch, a.m);
  delay(1500);
  a.m = 0x2059;
  printf("%X, %c, %hX\n", a.n, a.ch, a.m);
  delay(1500);
  a.n = 0x3E25AD54;
  printf("%X, %c, %hX\n", a.n, a.ch, a.m);
  delay(1500);
}