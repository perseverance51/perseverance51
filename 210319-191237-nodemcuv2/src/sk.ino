#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h> //闪存文件系统
#include <String.h>

ESP8266WebServer  server(80);

const char *ssid = "MERCURY_D268G";
const char *password = "pba5ayzk";

String getContentType(String filename)
{
  if (server.hasArg("download"))
     return "application/octet-stream";
  else
     if (filename.endsWith(".htm")) return "text/html";
  else  if (filename.endsWith(".html")) return "text/html";
  else  if (filename.endsWith(".css")) return "text/css";
  else  if (filename.endsWith(".js")) return "application/javascript";
  else  if (filename.endsWith(".png")) return "image/png";
  else  if (filename.endsWith(".gif")) return "image/gif";
  else  if (filename.endsWith(".jpg")) return "image/jpeg";
  else  if (filename.endsWith(".ico")) return "image/x-icon";
  else  if (filename.endsWith(".xml")) return "text/xml";
  else  if (filename.endsWith(".pdf")) return "application/x-pdf";
  else  if (filename.endsWith(".zip")) return "application/x-zip";
  else  if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}
  void handleNotFound() 
{
  String path = server.uri();
  Serial.print("load url:");
  Serial.println(path);
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path))
  {
    if (SPIFFS.exists(pathWithGz))
      path += ".gz";
    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, contentType);
    file.close();
    return;
  }
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++)
     
    {
      message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
  server.send(404, "text/plain", message);
}
void handleMain() 
{
   Serial.print("handleMain");
  File file = SPIFFS.open("/index.html", "r");
  size_t sent = server.streamFile(file, "text/html");
  file.close();
  return;
}

void handlePin() 
{
  if (server.hasArg("a"))
     
    {
        // 请求中是否包含有a的参数
          String action = server.arg("a");
        // 获得a参数的值
          if (action == "on") 
      {
          // a=on
            digitalWrite(2, LOW);
          // 点亮8266上的蓝色led，led是低电平驱动，需要拉低才能亮
            server.send(200, "text/html", "Pin 2 has turn on");
         return;
          // 返回数据
      }
       else  if (action == "off") 
      {
          // a=off
            digitalWrite(2, HIGH);
          // 熄灭板载led
            server.send(200, "text/html", "Pin 2 has turn off");
         return;
      }
      server.send(200, "text/html", "unknown action");
       return;
    }
  server.send(200, "text/html", "action no found");
}
void setup() 
{
   
      // 日常初始化网络
      pinMode(2, OUTPUT);
  Serial.begin(115200);
  SPIFFS.begin();
  int connectCount = 0;
  WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.print(".");
    if (connectCount > 30)
       
      {
        Serial.println( "Connect fail!");
        break;
      }
    connectCount += 1;
  }
  if (WiFi.status() == WL_CONNECTED)
     
    {
      Serial.println("");
      Serial.print("Connected to ");
      Serial.println(ssid);
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
      connectCount = 0;
    }
  server.on ("/", handleMain);
    // 绑定地址到handleMain方法处理
      server.on ("/pin", HTTP_GET, handlePin);
    // 绑定/pin地址到handlePin方法处理
      server.onNotFound(handleNotFound);
    // NotFound处理
      server.begin();
  Serial.println("HTTP server started");
}
  void loop() 
{
  server.handleClient();
}
 
