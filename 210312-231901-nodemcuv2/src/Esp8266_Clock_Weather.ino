#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <ESP8266HTTPClient.h>
//#include <DS18B20.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include <DNSServer.h> //密码直连将其三个库注释
#include <ESP8266WebServer.h>
#include <CustomWiFiManager.h>
#include <EEPROM.h>
#include <NTPClient.h>

#include "SH1106Wire.h" //1.3寸用这个
//#include "SSD1306Wire.h"    //0.96寸用这个
#include "OLEDDisplayUi.h"
#include "HeFeng.h"
#include "WeatherStationImages.h"
#include "font.h"
#include <Wire.h>

ESP8266WebServer server(80);
//定义端口号16,2，14，12，15，3,
unsigned char cout1[6] = {D0, D4, D5, D6, D8, D9};

//DS18B20 ds(D7); //温度传感器io口
#define ONE_WIRE_BUS D7        // 定义DS18B20数据口连接arduino的 13 脚
OneWire oneWire(ONE_WIRE_BUS); // 初始连接在单总线上的单总线设备
DallasTemperature sensors(&oneWire);

#if defined(ESP8266)    //OLED引脚定义
const int SDA_PIN = D2; //对应nodemcu接D5或者D1，，wemosD1mini的D2
const int SDC_PIN = D1; //对应nodemcu接D6或者D2，，wemosD1mini的D5
#else
const int SDA_PIN = D1; //对应nodemcu接D5或者D1
const int SCL = D2;     //对应nodemcu接D6或者D2
#endif

const int I2C_DISPLAY_ADDRESS = 0x3c;                      //I2c地址默认
SH1106Wire display(I2C_DISPLAY_ADDRESS, SDA_PIN, SDC_PIN); // 1.3寸用这个
//SSD1306Wire     display(I2C_DISPLAY_ADDRESS, SDA_PIN, SDC_PIN);   // 0.96寸用这个

//const char* WIFI_SSID = "MERCURY_D268G";  //填写你的WIFI名称及密码MERCURY_D268G CMCC-DyVv
//const char* WIFI_PWD = "pba5ayzk";
const char *DateAddress = "https://api.xlongwei.com/service/datetime/convert.json"; //农历;
//https://api.xlongwei.com/service/datetime/info.json
//https://api.xlongwei.com/service/datetime/workday.json
const char *WorkDate = "http://quan.suning.com/getSysTime.do"; //日期
unsigned int *a;                                               //EEPROM存放当前日期参数
unsigned int *b;
const char *str;

//由于太多人使用我的秘钥，导致获取次数超额，所以不提供秘钥了，大家可以到https://dev.heweather.com/获取免费的
const char *HEFENG_KEY = "95696b51ec11438899034a810a08fe7d"; //填写你的和风天气秘钥
//f490e741439a4b4ba3e9c51037821170
//bfaee7681d3649d88e03082f1a1e48e7 nodemcu
//95696b51ec11438899034a810a08fe7d
const char *HEFENG_LOCATION = "CN101250303"; //填写你的城市ID,可到https://where.heweather.com/index.html查询
//const char* HEFENG_LOCATION = "auto_ip";//自动IP定位

//#define TZ              8      // 中国时区为8
//#define DST_MN          0      // 默认为0

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "ntp1.aliyun.com", 60 * 60 * 8, 30 * 60 * 1000);

const unsigned int UPDATE_NTP_SECS = 1 * 60;         // 更新NTP时间更新频率
const unsigned int UPDATE_Weather_SECS = 6 * 60;     // 更新实时天气的频率
const unsigned int UPDATE_For_SECS = 15 * 60;        // 更新3天天气的频率
const unsigned int UPDATE_Temperature_SECS = 1 * 60; // DS18B20更新频率
const unsigned int UPDATE_Lunar_SECS = 15 * 60;      //农历更新频率
const unsigned int UPDATE_Today_SECS = 30 * 60;      //日期更新频率

const String WDAY_NAMES[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};                                     //星期
const String MONTH_NAMES[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"}; //月份

OLEDDisplayUi ui(&display); //初始化OLED

NongLi_Date NongLi;
CurrentData currentWeather; //结构体实例化对象
ForeData foreWeather[3];
WorkData NowToday;
HeFeng HeFengClient;

boolean flag = false;                      //oled屏幕关闭/开启
bool readyForUpdate = false;               // 3天更新标志
bool readyForWeatherUpdate = false;        // 实时天气更新标志
bool readyNLiUpdate = false;               // 农历更新标志
bool readyTodayUpdate = false;             //当天日期更新标志
bool first = true;                         //首次更新标志
unsigned long timeSinceLastNTPUpdate = 0;  //上次NTP更新后的时间
unsigned long timeSinceLastToUpdate = 0;   //上次日期更新后的时间
unsigned long timeSinceLastWUpdate = 0;    //上次气象更新后的时间
unsigned long timeSinceLastFUpdate = 0;    //上次3天更新后的时间
unsigned long timeSinceLastNLUpdate = 0;   //上次农历更新后的时间
unsigned long timeSinceLastCurrUpdate = 0; //上次温度更新后的时间

String currTemp = "8.88";                                              //温度初始值
void drawProgress(OLEDDisplay *display, int percentage, String label); //提前声明函数
void updateData(OLEDDisplay *display);                                 //根据millis和设定的时间进行判断更新天气间隔时间
void Day3updateData(OLEDDisplay *display);                             //根据millis,3天气象更新
void NLiupdateData(OLEDDisplay *display);                              //更新农历设定时间
void TodayupdateData(OLEDDisplay *display);                            //更新当天日期设定时间
void updateDatas(OLEDDisplay *display);                                //首次更新，重启时更新
void drawDateTime(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawCurrentWeather(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawForecast(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void drawForecastDetails(OLEDDisplay *display, int x, int y, int dayIndex);
void drawHeaderOverlay(OLEDDisplay *display, OLEDDisplayUiState *state);
void setReadyForWeatherUpdate(); //实时天气
void setReadyForUpdate();        //3天
void setReadyNLiUpdate();        //农历
void setReadyTodayUpdate();      //当天日期
void drawDateTime2(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void five(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void six(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void htmlcode(); //页面访问
void m();        //页面回调函数
//添加框架
//此数组保留指向所有帧的函数指针
//框架是从右向左滑动的单个视图
FrameCallback frames[] = {drawCurrentWeather, drawDateTime2, drawForecast, drawDateTime, five, six}; //
//页面数量,图形绘制回调函数数组,
int numberOfFrames = 6; // 显示6屏内容信息

OverlayCallback overlays[] = {drawHeaderOverlay}; //覆盖回调函数
unsigned int numberOfOverlays = 1;                //覆盖数

void setup()
{
  //定义端口号16,2，14，12，15，3,
  for (int i = 0; i < 6; i++)
  { //定义输入引脚  （此程序并没有用到，放到这里方便大家使用）
    pinMode(cout1[i], OUTPUT);
    //pinMode(cout[i],LOW);
  }
  Serial.begin(115200);
  sensors.begin(); // 初始温度传感器库DallasTemperature
  display.init();
  display.clear();
  display.display();
  //    display.flipScreenVertically(); //屏幕翻转
  // display.mirrorScreen();//使用分光棱镜显示需要用到此函数
  display.setContrast(240); //屏幕亮度

  //Web配网
  webconnect();
  ui.setTargetFPS(80);                   //刷新频率
  ui.setActiveSymbol(activeSymbole);     //设置活动符号
  ui.setInactiveSymbol(inactiveSymbole); //设置非活动符号
  ui.disableAllIndicators();             //不显示页码小点。
  // 符号位置
  // 你可以把这个改成TOP, LEFT, BOTTOM, RIGHT
  //ui.setIndicatorPosition(BOTTOM);// 显示页码小点位置，定义第一帧在栏中的位置
  //ui.setIndicatorDirection(LEFT_RIGHT);

  // 屏幕切换方向
  // 您可以更改使用的屏幕切换方向 SLIDE_LEFT, SLIDE_RIGHT, SLIDE_TOP, SLIDE_DOWN
  ui.setFrameAnimation(SLIDE_LEFT);           //切屏方向
  ui.setFrames(frames, numberOfFrames);       // 设置框架和显示屏幕内容数
  ui.setTimePerFrame(5000);                   //设置切换时间
  ui.setOverlays(overlays, numberOfOverlays); //设置覆盖的画面数

  // UI负责初始化显示
  ui.init();
  //    display.flipScreenVertically(); //屏幕反转
  //  display.mirrorScreen();//使用分光棱镜显示需要用到
  timeClient.begin();

  Serial.printf("WiFi name:%s\n IP Address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());

  server.on("/", htmlcode);
  server.on("/m", HTTP_GET, m);
  server.begin();
}

void loop()
{

  server.handleClient();
  ArduinoOTA.handle();
  if (first)
  { //首次加载
    updateDatas(&display);
    first = false;
  }
  if ((millis() - timeSinceLastWUpdate) > (1000L * UPDATE_Weather_SECS))
  { //天气更新频率
    setReadyForWeatherUpdate();
    timeSinceLastWUpdate = millis();
  }
  else if ((millis() - timeSinceLastToUpdate) > (1000L * UPDATE_Today_SECS))
  { //当天日期更新频率
    setReadyTodayUpdate();
    timeSinceLastToUpdate = millis();
  }
  else if ((millis() - timeSinceLastFUpdate) > (1000L * UPDATE_For_SECS))
  { //3天更新频率
    setReadyForUpdate();
    timeSinceLastFUpdate = millis();
  }
  else if (millis() - timeSinceLastCurrUpdate > (1000L * UPDATE_Temperature_SECS))
  { //温度更新频率
    if (ui.getUiState()->frameState == FIXED)
    {
      float temp = sensors.getTempCByIndex(0) - 61.13;
      //float temp = ds.getTempC() - 6.13;
      currTemp = String(temp, 2); //浮点数转字符串，2表示小数点精度？

      //      currTemp = ds.getTempC();
      timeSinceLastCurrUpdate = millis();
    }
  }
  else if ((millis() - timeSinceLastNTPUpdate) > (1000L * UPDATE_NTP_SECS))
  { //NTP更新频率
    timeClient.update();
    unsigned int Nowhours = timeClient.getHours(); //小时
    unsigned int minu = timeClient.getMinutes();   //分钟
    unsigned int sece = timeClient.getSeconds();   //秒
    unsigned int TodayDay = timeClient.getDay();   //星期
    unsigned int T1 = Nowhours * 3600 + minu * 60 + sece;
    unsigned int T2 = 86400 - T1;
    printf("现在时间：%s\n", timeClient.getFormattedTime().c_str());
    Serial.println("现在时间秒数:" + String("") + T1);
    Serial.println("距第二天剩余秒数:" + String("") + T2);
    Serial.println(String("") + "星期:" + TodayDay);
    //    if (Now_hours == "00" && minu0 == "00" == true) {
    if (128 > T1)
    {
      setReadyNLiUpdate();   //农历更新
      setReadyTodayUpdate(); //当天日期更新
      Serial.println("现在时间秒数:" + String("") + T1);
      Serial.println(F("达到更新日期时间."));
    }
    timeSinceLastNTPUpdate = millis();
  }
  else if ((millis() - timeSinceLastNLUpdate) > (1000L * UPDATE_Lunar_SECS))
  { //农历更新频率
    setReadyNLiUpdate();
    timeSinceLastNLUpdate = millis();
  }
  else if (readyNLiUpdate && (ui.getUiState()->frameState == FIXED))
  { //农历更新已准备就绪，且框架==固定的
    NLiupdateData(&display);
  }
  else if (readyTodayUpdate && (ui.getUiState()->frameState == FIXED))
  { //当前日期更新已准备就绪，且框架==固定的
    TodayupdateData(&display);
  }
  else if (readyForWeatherUpdate && (ui.getUiState()->frameState == FIXED))
  { //实时天气数据更新已准备就绪，且框架==固定的
    updateData(&display);
  }
  else if (readyForUpdate && (ui.getUiState()->frameState == FIXED))
  { //3天气象数据更新已准备就绪，且框架==固定的
    Day3updateData(&display);
  }

  int remainingTimeBudget = ui.update(); //剩余时间预算
  if (remainingTimeBudget > 0)
  {
    //你可以在这里工作如果你低于你的时间预算。
    delay(remainingTimeBudget);
  }
}
/********************************配网函数*********************************************/
void webconnect()
{ ////Web配网，密码直连将其注释
  display.clear();
  display.drawXbm(0, 0, 128, 64, bilibili); //显示哔哩哔哩
  display.display();

  WiFiManager wifiManager;           //实例化WiFiManager
  wifiManager.setDebugOutput(false); //关闭Debug
  //wifiManager.setConnectTimeout(10); //设置超时
  //wifiManager.setHeadImgBase64(FPSTR(Icon)); //设置配网页面中显示的图标
  wifiManager.setPageTitle("欢迎进入WiFi配置页"); //设置页标题

  if (!wifiManager.autoConnect("ESP8266-IOT-Display"))
  { //AP模式下的wifi名称
    Serial.println(F("连接失败并超时"));
    //重新设置并再试一次，或者让它进入深度睡眠状态
    ESP.restart();
    delay(1000);
  }
  Serial.println("connected...^_^");
  yield();
}
/********************************重启时的进度绘制函数*********************************************/
void drawProgress(OLEDDisplay *display, int percentage, String label)
{ //绘制进度
  (*display).clear();
  (*display).setTextAlignment(TEXT_ALIGN_CENTER);
  (*display).setFont(ArialMT_Plain_10);
  (*display).drawString(64, 10, label);
  (*display).drawProgressBar(2, 28, 124, 10, percentage);
  (*display).display();
}
/********************************数据更新函数*********************************************/

void updateData(OLEDDisplay *display)
{ //实时天气更新
  HeFengClient.doUpdateCurr(&currentWeather, HEFENG_KEY, HEFENG_LOCATION);
  delay(1200);
  readyForWeatherUpdate = false;
}

void Day3updateData(OLEDDisplay *display)
{ //3天预报天气更新
  HeFengClient.doUpdateFore(foreWeather, HEFENG_KEY, HEFENG_LOCATION);
  delay(1200);
  readyForUpdate = false;
}

void NLiupdateData(OLEDDisplay *display)
{ //农历更新
  HeFengClient.NongLingDate(&NongLi, DateAddress);
  delay(1200);
  readyNLiUpdate = false;
}

void TodayupdateData(OLEDDisplay *display)
{ //当天日期
  HeFengClient.WorkDate(&NowToday, WorkDate);
  delay(1200);
  readyTodayUpdate = false;
}

void updateDatas(OLEDDisplay *display)
{ //首次天气、预报天气，农历的整体更新，重启时才调用
  drawProgress(display, 25, "Updating Today Date...");
  HeFengClient.WorkDate(&NowToday, WorkDate);
  delay(1500);
  drawProgress(display, 50, "Updating foreWeather...");
  HeFengClient.doUpdateFore(foreWeather, HEFENG_KEY, HEFENG_LOCATION);
  delay(1200);
  drawProgress(display, 75, "NongLi...");
  HeFengClient.NongLingDate(&NongLi, DateAddress);
  delay(1000);
  drawProgress(display, 85, "Updating weather...");
  HeFengClient.doUpdateCurr(&currentWeather, HEFENG_KEY, HEFENG_LOCATION);
  delay(800);
  drawProgress(display, 100, "Wellcome...");
  delay(500);
}
/********************************更新前的逻辑判断函数*********************************************/
void setReadyForWeatherUpdate()
{ //为实时天气更新做好准备
  Serial.println(F("Setting readyForUpdate to true"));
  readyForWeatherUpdate = true;
}
void setReadyForUpdate()
{ //为3天更新做好准备
  Serial.println(F("Setting ready 3Day Update to true"));
  readyForUpdate = true;
}
void setReadyNLiUpdate()
{ //为农历更新做好准备
  Serial.println(F("Setting readyNLiUpdate to true"));
  readyNLiUpdate = true;
}
void setReadyTodayUpdate()
{ //为当前日期更新做好准备
  Serial.println(F("Setting readyTodayUpdate to true"));
  readyTodayUpdate = true;
}

/********************************OLED显示函数*********************************************/

void drawDateTime(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{ //显示当前时间和日期
  timeClient.update();
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_16);
  display->drawString(64 + x, 3 + y, NowToday.workdate);
  display->setFont(ArialMT_Plain_24);
  display->drawString(64 + x, 22 + y, timeClient.getFormattedTime());
  display->setTextAlignment(TEXT_ALIGN_LEFT);
}

void drawCurrentWeather(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{                                     //显示当天实时气象天气
  display->setFont(ArialMT_Plain_14); //字体总共5种规格10,14，16,24,自制12，14，显示过大可改12
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(32 + x, 35 + y, currentWeather.cond_txt);
  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawString(73 + x, 38 + y, "Wind: " + String(currentWeather.wind_sc) + "  ");
  display->setFont(ArialMT_Plain_24);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  String temp = String(currentWeather.tmp) + "°C";
  display->drawString(68 + x, 0 + y, temp); //温度显示位置
  display->setFont(ArialMT_Plain_10);
  display->drawString(95 + x, 26 + y, ": " + String(currentWeather.hum) + "%"); //湿度显示
  display->drawXbm(71, 26, 12, 12, shi);
  display->drawXbm(83, 26, 12, 12, du);
  display->setFont(Meteocons_Plain_40); //显示气象图标大小设置40，36和21三种,0.96寸屏幕就不要设置成40的，否则过大显示不全，40仅使用1.3寸。
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(30 + x, y - 2, currentWeather.iconMeteoCon);
  display->drawHorizontalLine(0, 51, 34);  //画水平线
  display->drawHorizontalLine(68, 51, 60); //画水平线
  display->drawVerticalLine(68, 27, 24);   //画垂直线
}

void drawForecast(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{ //天气三天预报位置布局
  drawForecastDetails(display, x - 8, y, 0);
  drawForecastDetails(display, x + 33, y, 1);
  drawForecastDetails(display, x + 77, y, 2);
}

void drawForecastDetails(OLEDDisplay *display, int x, int y, int dayIndex)
{ //3天预报天气
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_10);
  display->drawString(x + 29, y, foreWeather[dayIndex].datestr);                       //预报日期
  display->drawRect(x + 13, y, 32, 12);                                                // 画空心矩形w,h,宽度,高度
  display->setFont(Meteocons_Plain_21);                                                //显示气象图标大小设置40，36和21三种。
  display->drawString(x + 23, y + 12, foreWeather[dayIndex].iconMeteoCon);             //预报图标位置
  String temp = foreWeather[dayIndex].tmp_min + " | " + foreWeather[dayIndex].tmp_max; //预报气温范围
  display->setFont(ArialMT_Plain_12);
  display->drawString(x + 27, y + 34, temp); //预报温度显示位置
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_12); //显示今天 明天 后天
  display->drawXbm(29, 16, 12, 12, Jin);
  display->drawXbm(70, 16, 12, 12, Ming);
  display->drawXbm(113, 16, 12, 12, Hou);
  display->drawHorizontalLine(0, 51, 128); //画水平线
}

/********************************页眉页脚显示函数*********************************************/
void drawHeaderOverlay(OLEDDisplay *display, OLEDDisplayUiState *state)
{ //绘图页眉覆盖
  timeClient.update();
  unsigned int Nowhours = timeClient.getHours(); //小时
  unsigned int minu = timeClient.getMinutes();   //分钟
  char buff[14];
  sprintf_P(buff, PSTR("%02d:%02d"), Nowhours, minu);

  display->setColor(INVERSE);         //BLACK,WHITE,INVERSE
  display->setFont(ArialMT_Plain_12); //添加的自定义字体
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawString(0, 53, String(buff));
  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  display->drawString(128, 53, String(currTemp + "°C"));
}

void drawDateTime2(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{ //显示中文星期和时间
  display->clear();
  timeClient.update();
  display->setFont(ArialMT_Plain_24); //显示字体大小
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawString(16 + x, 22 + y, timeClient.getFormattedTime()); //显示位置
  display->drawXbm(42, 5, 16, 16, xing);
  display->drawXbm(59, 5, 16, 16, qi);
  display->drawHorizontalLine(0, 51, 128); //画水平线
  (*display).drawXbm(76, 5, 16, 16, Week[timeClient.getDay()]);
}

void five(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{ //农历显示
  display->clear();
  display->setFont(ArialMT_Plain_24); //字体总共3种规格10,16,24,自制12

  display->drawXbm(25, 10, 16, 16, hu);
  display->drawXbm(45, 10, 16, 16, nan1);
  display->drawXbm(65, 10, 16, 16, li);

  display->drawRect(21, 7, 85, 22); // 画空心矩形w,h,宽度,高度
  display->drawXbm(85, 10, 16, 16, ling);

  display->setFont(ArialMT_Plain_12); //自定义添加的Arimo#字体,
  display->drawXbm(3, 35, 12, 12, nong);
  display->drawXbm(15, 35, 12, 12, li2);

  display->setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
  display->drawString(44, 42, String(NongLi.data_lunarYear));
  display->drawXbm(61, 35, 12, 12, Year1);
  display->setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
  display->drawString(79, 42, String(NongLi.data_lunarMonth));

  display->drawXbm(87, 35, 12, 12, yue);
  display->setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
  display->drawString(106, 42, String(NongLi.data_lunarDay));

  display->drawXbm(114, 35, 12, 12, ri);
  display->drawHorizontalLine(0, 51, 128); //画水平线
}

void six(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  display->clear();

  display->drawXbm(0, 0, 128, 64, hunan1);
  display->display();
  delay(800);
  display->clear();
  delay(50);
  display->drawXbm(0, 0, 128, 64, hunan2);
  display->display();
  delay(800);
  display->clear();
  delay(50);
  display->drawXbm(0, 0, 128, 64, changsha);
  display->display();
  delay(800);
  display->clear();
  delay(50);
  display->drawXbm(0, 0, 128, 64, changsha2);
  display->display();
  delay(800);
}

void m()
{ //-- ("/m", HTTP_GET, m);----"x1")=="1" 房间号==设备号 http:ip/m?x1=1
  String ip = WiFi.localIP().toString().c_str();
  if (server.arg("x1") == "1")
  {
    if (digitalRead(2) == 1)
    { //板载led灯，低电平亮
      digitalWrite(2, 0);
      htmlcode();
    }
    else
    {
      digitalWrite(2, 1);
      htmlcode();
    }
  }

  if (server.arg("x1") == "2")
  {
    if (digitalRead(16) == 0)
    {
      digitalWrite(16, 1);
      htmlcode();
    }
    else
    {
      digitalWrite(16, 0);
      htmlcode();
    }
  }

  if (server.arg("x1") == "3")
  {
    if (digitalRead(14) == 0)
    {
      digitalWrite(14, 1);
      htmlcode();
    }
    else
    {
      digitalWrite(14, 0);
      htmlcode();
    }
  }
  if (server.arg("x1") == "4")
  {
    if (digitalRead(12) == 0)
    {
      digitalWrite(12, 1);
      htmlcode();
    }
    else
    {
      digitalWrite(12, 0);
      htmlcode();
    }
  }

  if (server.arg("x1") == "5")
  {
    if (digitalRead(15) == 0)
    {
      digitalWrite(15, 1);
      htmlcode();
    }
    else
    {
      digitalWrite(15, 0);
      htmlcode();
    }
  }
  if (server.arg("x1") == "6")
  { //控制oled屏幕开关
    if (flag == false)
    {
      display.displayOff();
      htmlcode();
      flag = true;
    }
    else
    {
      display.displayOn();
      flag = false;
      htmlcode();
    }
  }

  /**********总开*******************************/
  if (server.arg("x1") == "7")
  {
    if (digitalRead(2) == 0)
    {
      digitalWrite(2, 1);
      htmlcode();
    }
    else
    {
      digitalWrite(2, 0);
      htmlcode();
    }

    if (digitalRead(16) == 0)
    {
      digitalWrite(16, 1);
      htmlcode();
    }
    else
    {
      digitalWrite(16, 0);
      htmlcode();
    }

    if (digitalRead(14) == 0)
    {
      digitalWrite(14, 1);
      htmlcode();
    }
    else
    {
      digitalWrite(14, 0);
      htmlcode();
    }
    if (digitalRead(12) == 0)
    {
      digitalWrite(12, 1);
      htmlcode();
    }
    else
    {
      digitalWrite(12, 0);
      htmlcode();
    }

    if (digitalRead(15) == 0)
    {
      digitalWrite(15, 1);
      htmlcode();
    }
    else
    {
      digitalWrite(15, 0);
      htmlcode();
    }
  }
  if (server.arg("x1") == "8")
  {

    htmlcode();
    delay(1000);
    ESP.restart();
  }
  else
  {
    htmlcode();
  } // not found htmllink
}

void htmlcode()
{
  unsigned long timecnt = millis();
  unsigned long fen1 = timecnt / 60000L; //计算取商
  unsigned long fen2 = fen1 % 60L;       //计算分钟取余
  unsigned int hour1 = fen1 / 60L;       //计算小时取商
  unsigned int hour2 = hour1 % 24L;      //计算小时取余
  unsigned int day1 = hour1 / 24;        //计算天

  String str = String(day1) + ":" + String(hour2) + ":" + String(fen2);

  String a = (digitalRead(2)) ? "black" : "red";
  String b = (digitalRead(16)) ? "black" : "red";
  String c = (digitalRead(14)) ? "black" : "red";
  String d = (digitalRead(12)) ? "black" : "red";
  String e = (digitalRead(15)) ? "black" : "red";
  String f = (flag) ? "black" : "red";
  String html = "<!DOCTYPE html><html lang='zh-CN'><head><meta charset='UTF-8'><meta name='viewport'    content='width=device-width, initial-scale=1.0, minimum-scale=0.5, maximum-scale=2.0, user-scalable=yes' /><meta http-equiv='X-UA-Compatible' content='ie=edge'><title>esp8266气象时钟控制主页</title><link rel='stylesheet' href='http://at.alicdn.com/t/font_1640699_nk29qyrqhco.css'><style>    h2,    h1 {      line-height: 1%;    }    body {      width: 360px;      margin: 0;      padding: 0;      background: LightCyan;    }    .button {      width: 130px;      height: 70px;      text-align: center;      color: #FFF;      border-radius: 1px;      margin: 40px -4.5px 0px 0;      position: relative;      overflow: hidden;      border: 2px solid rgb(2, 69, 253);      background: darkcyan;      font-size: 14px;      outline: none;      border-radius: 5px 5px 5px 5px;    }    .top1 {      width: 360px;      height: 45px;      color: rgb(53, 136, 5);      border: 2px;      background: rgb(233, 235, 236);      font-size: 20px;      left: -15px;      position: relative;      top: 2px;    }    .device1 {      left: 61px;      position: relative;      font-size: 20px;    }</style></head><body><a href='/'><button class='top1' type='button'><strong><font style='font-family:宋体'>esp8266&#x6C14;&#x8C61;&#x65F6;&#x949F;&#x63A7;&#x5236;&#x4E3B;&#x9875;</font></strong></button></a><dl><center><span style='left: -16px; position: relative;' id='time1'> Time Loading ......</span><script>        setInterval('time1.innerHTML=new Date().toLocaleString()', 1000);</script></center><div class='device1'><font style='font-family:黑体'>&#x8BBE;&#x5907;&#x5728;&#x7EBF;&#x65F6;&#x95F4;:<a>";
  html += str;

  html += "</a></div><center style=' left: -15px; position: relative;'><a href='./m?x1=1'><button type='button' class='button'><div class='m x-keting1' style='font-size:40px; color: " + a + ";'></div>&#x5BA2; &#x5385;</button></a>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href='./m?x1=2'><button type='button' class='button'><div class='m x-woshi' style='font-size:40px; color:" + b + ";'></div>&#x5367;&#x5BA4;</button></a></br><a href='./m?x1=3'><button type='button' class='button'><div class='m x-chuanglian' style='font-size:40px; color: " + c + ";'></div>&#x7A97;&#x5E18;</button></a>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href='./m?x1=4'><button type='button' class='button'><div class='m x-chufang' style='font-size:40px; color: " + d + ";'></div>&#x53A8;&#x623F;</button></a></br><a href='./m?x1=5'><button type='button' class='button'><div class='m x-reshuiqi' style='font-size:40px; color: " + e + ";'></div>&#x70ED;&#x6C34;&#x5668;</button></a>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href='./m?x1=6'><button type='button' class='button'><div class='m x-deng1' style='font-size:40px; color: " + f + ";'></div>OLED&#x5C4F;&#x5E55;</button></a></br><a href='./m?x1=7'><button type='button' class='button'><div class='m x-shuimian' style='font-size:40px;'></div>&#x603B;&#x5F00;&#x5173;</button></a>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href='./m?x1=8'><button type='button' class='button'><div class='m x-kongtiao' style='font-size:40px;'></div>&#x91CD;&#x542F;ESP8266</button></a></center></ul></body></html>";

  server.send(1000, "text/html", html);
}