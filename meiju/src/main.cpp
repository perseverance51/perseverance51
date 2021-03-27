#include <Arduino.h>
enum DAY
{
      MON=1, TUE, WED, THU, FRI, SAT, SUN
};

void setup() {

}

void loop() {
  enum DAY day;
    day = SAT;
    char buffer[5];
sprintf(buffer, "Today = %d\n", day);
Serial.print(buffer);
}