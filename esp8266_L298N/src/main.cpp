#include <Arduino.h>

/* Create a WiFi access point and provide a web server on it to move robot. 
 *  
 *  Important: Use without ATMega328 chip, or wire RST in GND
 *  
 *  Author: Lucio A. Rocha
 *  Last Update: 25 Jan 2017
 *
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

/* Set these to your desired credentials. */
const char *ssid = "utfprbot";
const char *password = "654321";

ESP8266WebServer server(80);

const int led = 2; //GPIO2

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
 * connected to this access point to see it.
 */
void moveUp();
void moveLeftUp();
void moveRightUp();
void moveLeftDown();
void moveRightDown();
void moveDown();
void moveStop();
void turnOn();
void turnOff();

void handleRoot()
{

  int size = 1000;
  char temp[size];

  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  snprintf(temp, size,

           "<html>\
  <head>\
    <title>UTFPRBOT-1.0</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h3>You are connected on UTFPRBOT #1!</h3>\
    <p>Uptime: %02d:%02d:%02d</p>\
    <p>Status: Light ON</h1></p>\
    <p><a href=\"http://192.168.4.1/on\"><h1>Turn ON</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/of\"><h1>Turn OFF</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/st\"><h1 align=\"center\">STOP</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/up\"><h1 align=\"center\">/\\/\\</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/lu\"><h1 align=\"center\">/\\_____</h1></a><a href=\"http://192.168.4.1/ru\"><h1 align=\"center\">_____/\\</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/ld\"><h1 align=\"center\">\\/_____</h1></a><a href=\"http://192.168.4.1/rd\"><h1 align=\"center\">_____\\/</h1></a></p>\
    <p><a href=\"http://192.168.4.1/do\"><h1 align=\"center\">\\/\\/</h1></a></p>\
    <p></p>\
  </body>\
</html>",

           hr, min % 60, sec % 60);
  server.send(200, "text/html", temp);
}

void setup()
{
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");

  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);

  //URLs available to query
  server.on("/", handleRoot);
  server.on("/up", moveUp);        //1
  server.on("/lu", moveLeftUp);    //2
  server.on("/ru", moveRightUp);   //3
  server.on("/ld", moveLeftDown);  //4
  server.on("/rd", moveRightDown); //5
  server.on("/do", moveDown);      //6
  server.on("/st", moveStop);      //7
  server.on("/on", turnOn);        //8
  server.on("/of", turnOff);       //9

  server.begin();
  Serial.println("HTTP server started");
}

void moveUp()
{

  digitalWrite(led, HIGH);

  int size = 1000;
  char temp[size];

  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  snprintf(temp, size,

           "<html>\
  <head>\
    <title>UTFPRBOT - 1.0</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h3>Request: Move UP</h3>\
    <p>Uptime: %02d:%02d:%02d</p>\
    <p><a href=\"http://192.168.4.1/on\"><h1>Turn ON</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/of\"><h1>Turn OFF</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/st\"><h1 align=\"center\">STOP</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/up\"><h1 align=\"center\">/\\/\\</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/lu\"><h1 align=\"center\">/\\_____</h1></a><a href=\"http://192.168.4.1/ru\"><h1 align=\"center\">_____/\\</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/ld\"><h1 align=\"center\">\\/_____</h1></a><a href=\"http://192.168.4.1/rd\"><h1 align=\"center\">_____\\/</h1></a></p>\
    <p><a href=\"http://192.168.4.1/do\"><h1 align=\"center\">\\/\\/</h1></a></p>\
    <p></p>\
  </body>\
</html>",

           hr, min % 60, sec % 60);

  server.send(200, "text/html", temp);
  Serial.println("SETM1");
}

void moveLeftUp()
{

  digitalWrite(led, HIGH);

  int size = 1000;
  char temp[size];

  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  snprintf(temp, size,

           "<html>\
  <head>\
    <title>UTFPRBOT - 1.0</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h3>Request: Move LEFT-UP</h3>\
    <p>Uptime: %02d:%02d:%02d</p>\
    <p><a href=\"http://192.168.4.1/on\"><h1>Turn ON</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/of\"><h1>Turn OFF</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/st\"><h1 align=\"center\">STOP</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/up\"><h1 align=\"center\">/\\/\\</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/lu\"><h1 align=\"center\">/\\_____</h1></a><a href=\"http://192.168.4.1/ru\"><h1 align=\"center\">_____/\\</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/ld\"><h1 align=\"center\">\\/_____</h1></a><a href=\"http://192.168.4.1/rd\"><h1 align=\"center\">_____\\/</h1></a></p>\
    <p><a href=\"http://192.168.4.1/do\"><h1 align=\"center\">\\/\\/</h1></a></p>\
    <p></p>\
  </body>\
</html>",

           hr, min % 60, sec % 60);

  server.send(200, "text/html", temp);
  Serial.println("SETM2");
}

void moveLeftDown()
{

  digitalWrite(led, HIGH);

  int size = 1000;
  char temp[size];

  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  snprintf(temp, size,

           "<html>\
  <head>\
    <title>UTFPRBOT - 1.0</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h3>Request: Move LEFT-DOWN</h3>\
    <p>Uptime: %02d:%02d:%02d</p>\
    <p><a href=\"http://192.168.4.1/on\"><h1>Turn ON</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/of\"><h1>Turn OFF</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/st\"><h1 align=\"center\">STOP</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/up\"><h1 align=\"center\">/\\/\\</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/lu\"><h1 align=\"center\">/\\_____</h1></a><a href=\"http://192.168.4.1/ru\"><h1 align=\"center\">_____/\\</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/ld\"><h1 align=\"center\">\\/_____</h1></a><a href=\"http://192.168.4.1/rd\"><h1 align=\"center\">_____\\/</h1></a></p>\
    <p><a href=\"http://192.168.4.1/do\"><h1 align=\"center\">\\/\\/</h1></a></p>\
    <p></p>\
  </body>\
</html>",

           hr, min % 60, sec % 60);

  server.send(200, "text/html", temp);
  Serial.println("SETM4");
}

void moveRightUp()
{

  digitalWrite(led, HIGH);

  int size = 1000;
  char temp[size];

  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  snprintf(temp, size,

           "<html>\
  <head>\
    <title>UTFPRBOT - 1.0</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h3>Request: Move RIGHT-UP</h3>\
    <p>Uptime: %02d:%02d:%02d</p>\
    <p><a href=\"http://192.168.4.1/on\"><h1>Turn ON</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/of\"><h1>Turn OFF</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/st\"><h1 align=\"center\">STOP</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/up\"><h1 align=\"center\">/\\/\\</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/lu\"><h1 align=\"center\">/\\_____</h1></a><a href=\"http://192.168.4.1/ru\"><h1 align=\"center\">_____/\\</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/ld\"><h1 align=\"center\">\\/_____</h1></a><a href=\"http://192.168.4.1/rd\"><h1 align=\"center\">_____\\/</h1></a></p>\
    <p><a href=\"http://192.168.4.1/do\"><h1 align=\"center\">\\/\\/</h1></a></p>\
    <p></p>\
  </body>\
</html>",

           hr, min % 60, sec % 60);

  server.send(200, "text/html", temp);
  Serial.println("SETM3");
}

void moveRightDown()
{

  digitalWrite(led, HIGH);

  int size = 1000;
  char temp[size];

  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  snprintf(temp, size,

           "<html>\
  <head>\
    <title>UTFPRBOT - 1.0</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h3>Request: Move RIGHT-DOWN</h3>\
    <p>Uptime: %02d:%02d:%02d</p>\
    <p><a href=\"http://192.168.4.1/on\"><h1>Turn ON</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/of\"><h1>Turn OFF</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/st\"><h1 align=\"center\">STOP</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/up\"><h1 align=\"center\">/\\/\\</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/lu\"><h1 align=\"center\">/\\_____</h1></a><a href=\"http://192.168.4.1/ru\"><h1 align=\"center\">_____/\\</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/ld\"><h1 align=\"center\">\\/_____</h1></a><a href=\"http://192.168.4.1/rd\"><h1 align=\"center\">_____\\/</h1></a></p>\
    <p><a href=\"http://192.168.4.1/do\"><h1 align=\"center\">\\/\\/</h1></a></p>\
    <p></p>\
  </body>\
</html>",

           hr, min % 60, sec % 60);

  server.send(200, "text/html", temp);
  Serial.println("SETM5");
}

void moveDown()
{

  digitalWrite(led, HIGH);

  int size = 1000;
  char temp[size];

  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  snprintf(temp, size,

           "<html>\
  <head>\
    <title>UTFPRBOT - 1.0</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h3>Request: Move DOWN</h3>\
    <p>Uptime: %02d:%02d:%02d</p>\
    <p><a href=\"http://192.168.4.1/on\"><h1>Turn ON</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/of\"><h1>Turn OFF</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/st\"><h1 align=\"center\">STOP</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/up\"><h1 align=\"center\">/\\/\\</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/lu\"><h1 align=\"center\">/\\_____</h1></a><a href=\"http://192.168.4.1/ru\"><h1 align=\"center\">_____/\\</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/ld\"><h1 align=\"center\">\\/_____</h1></a><a href=\"http://192.168.4.1/rd\"><h1 align=\"center\">_____\\/</h1></a></p>\
    <p><a href=\"http://192.168.4.1/do\"><h1 align=\"center\">\\/\\/</h1></a></p>\
    <p></p>\
  </body>\
</html>",

           hr, min % 60, sec % 60);

  server.send(200, "text/html", temp);
  Serial.println("SETM6");
}

void moveStop()
{

  digitalWrite(led, HIGH);

  int size = 1000;
  char temp[size];

  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  snprintf(temp, size,

           "<html>\
  <head>\
    <title>UTFPRBOT - 1.0</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h3>Request: STOP</h3>\     
    <p>Uptime: %02d:%02d:%02d</p>\
    <p><a href=\"http://192.168.4.1/on\"><h1>Turn ON</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/of\"><h1>Turn OFF</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/st\"><h1 align=\"center\">STOP</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/up\"><h1 align=\"center\">/\\/\\</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/lu\"><h1 align=\"center\">/\\_____</h1></a><a href=\"http://192.168.4.1/ru\"><h1 align=\"center\">_____/\\</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/ld\"><h1 align=\"center\">\\/_____</h1></a><a href=\"http://192.168.4.1/rd\"><h1 align=\"center\">_____\\/</h1></a></p>\
    <p><a href=\"http://192.168.4.1/do\"><h1 align=\"center\">\\/\\/</h1></a></p>\
    <p></p>\
  </body>\
</html>",

           hr, min % 60, sec % 60);

  server.send(200, "text/html", temp);
  Serial.println("SETM7");
}

void turnOn()
{

  digitalWrite(led, HIGH);

  int size = 1000;
  char temp[size];

  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  snprintf(temp, size,

           "<html>\
  <head>\
    <title>UTFPRBOT - 1.0</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h3>Request: Light ON</h3>\
    <p>Uptime: %02d:%02d:%02d</p>\
    <p>Status: Light ON</p>\
    <p><a href=\"http://192.168.4.1/on\"><h1>Turn ON</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/of\"><h1>Turn OFF</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/st\"><h1 align=\"center\">STOP</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/up\"><h1 align=\"center\">/\\/\\</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/lu\"><h1 align=\"center\">/\\_____</h1></a><a href=\"http://192.168.4.1/ru\"><h1 align=\"center\">_____/\\</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/ld\"><h1 align=\"center\">\\/_____</h1></a><a href=\"http://192.168.4.1/rd\"><h1 align=\"center\">_____\\/</h1></a></p>\
    <p><a href=\"http://192.168.4.1/do\"><h1 align=\"center\">\\/\\/</h1></a></p>\
    <p></p>\
  </body>\
</html>",

           hr, min % 60, sec % 60);

  server.send(200, "text/html", temp);
  Serial.println("SETM8");
}

void turnOff()
{

  digitalWrite(led, LOW);

  int size = 1000;
  char temp[size];

  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  snprintf(temp, size,

           "<html>\
  <head>\
    <title>UTFPRBOT - 1.0</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h3>Request: Light OFF</h3>\
    <p>Uptime: %02d:%02d:%02d</p>\
    <p>Status: Light OFF</p>\
    <p><a href=\"http://192.168.4.1/on\"><h1>Turn ON</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/of\"><h1>Turn OFF</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/st\"><h1 align=\"center\">STOP</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/up\"><h1 align=\"center\">/\\/\\</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/lu\"><h1 align=\"center\">/\\_____</h1></a><a href=\"http://192.168.4.1/ru\"><h1 align=\"center\">_____/\\</h1></a></p>\
    <p></p>\
    <p><a href=\"http://192.168.4.1/ld\"><h1 align=\"center\">\\/_____</h1></a><a href=\"http://192.168.4.1/rd\"><h1 align=\"center\">_____\\/</h1></a></p>\
    <p><a href=\"http://192.168.4.1/do\"><h1 align=\"center\">\\/\\/</h1></a></p>\
    <p></p>\
  </body>\
</html>",

           hr, min % 60, sec % 60);

  server.send(200, "text/html", temp);
  Serial.println("SETM9");
}

void loop()
{
  //IPAddress myIP = WiFi.softAPIP();
  //Serial.print("AP IP address: ");
  //Serial.println(myIP);
  server.handleClient();
}