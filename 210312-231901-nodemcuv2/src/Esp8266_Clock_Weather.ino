#include <Arduino.h>
#include <ESP8266WiFi.h>
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
const unsigned char cout1[6] = {D0, D4, D5, D6, D8, D9};
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
unsigned char *str;

//由于太多人使用我的秘钥，导致获取次数超额，所以不提供秘钥了，大家可以到https://dev.heweather.com/获取免费的
const char *HEFENG_KEY = "f490e741439a4b4ba3e9c51037821170"; //填写你的和风天气秘钥
//f490e741439a4b4ba3e9c51037821170
//bfaee7681d3649d88e03082f1a1e48e7 nodemcu
//95696b51ec11438899034a810a08fe7d
const char *HEFENG_LOCATION = "CN101250303"; //填写你的城市ID,可到https://where.heweather.com/index.html查询
//const char* HEFENG_LOCATION = "auto_ip";//自动IP定位

//#define TZ  8// 中国时区为8
//#define DST_MN    0// 默认为0

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
// 声明一个ui对象,并将这个ui对象和屏幕驱动库sh1102wire关联。
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
//void drawHeaderOverlay(OLEDDisplay *display, OLEDDisplayUiState *state);
void setReadyForWeatherUpdate(); //实时天气
void setReadyForUpdate();        //3天
void setReadyNLiUpdate();        //农历
void setReadyTodayUpdate();      //当天日期
void drawDateTime2(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void five(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void six(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void PANDA(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void ShowSnow(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void ShowStars(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void ShowPolygon(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void DrawLineRect(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void DrawPolygon(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void Math(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y);
void math_xy(void);
void math_jcb(void);
void math_sin(void);
void math_xy2();
void math_jcb2();
void math_sin2();
void math_tyuan();
void math_quxian();
void math_paowuxian();
void htmlcode(); //html页面访问
void m();        //html页面回调函数
void MovMind();
u16 GetMindDistanceSquare(u16 x0, u16 y0, u16 x1, u16 y1);

void GPIO_State();
//添加框架
//此数组保留指向所有帧的函数指针
//框架是从右向左滑动的单个视图
FrameCallback frames[] = {Math, DrawLineRect, drawCurrentWeather, drawDateTime2, drawForecast, drawDateTime, five, six, PANDA, ShowSnow, ShowStars, ShowPolygon, DrawPolygon}; //
//页面数量,图形绘制回调函数数组,
int numberOfFrames = 13; // 显示6屏内容信息

//OverlayCallback overlays[] = {drawHeaderOverlay}; //覆盖回调函数
unsigned int numberOfOverlays = 1; //覆盖数

void setup()
{
  //定义端口号16,2，14，12，15，3,
  for (int i = 0; i < 6; i++)
  { //定义输入引脚  （此程序并没有用到，放到这里方便大家使用）
    pinMode(cout1[i], OUTPUT);
    //pinMode(cout[i],LOW);
  }
  // Serial.begin(115200);
  Serial.begin(115200, SERIAL_8N1); //同上

  sensors.begin(); // 初始温度传感器库DallasTemperature
  display.init();
  display.clear();
  display.display();
  //    display.flipScreenVertically(); //屏幕翻转
  // display.mirrorScreen();//使用分光棱镜显示需要用到此函数
  display.setContrast(240); //屏幕亮度
  delayMicroseconds(100);

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
  ui.enableAutoTransition();
  ui.setFrameAnimation(SLIDE_LEFT);     //切屏方向
  ui.setFrames(frames, numberOfFrames); // 设置框架和显示屏幕内容数
  ui.setTimePerFrame(3500);             //设置切换时间
  ui.setTimePerTransition(500);         //设置转场大约所需要时间
  //ui.setOverlays(overlays, numberOfOverlays); //设置页眉，覆盖的画面函数

  // UI负责初始化显示
  ui.init();
  //    display.flipScreenVertically(); //屏幕反转
  //  display.mirrorScreen();//使用分光棱镜显示需要用到
  timeClient.begin();

  Serial.printf("WiFi name:%s\n IP Address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
  server.on("/", htmlcode);
  server.on("/m", HTTP_GET, m);
  server.begin(); //启动网络服务功能
}

void loop()
{

  server.handleClient();
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
  {                                //温度更新频率
                                   // if (ui.getUiState()->frameState == FIXED)
                                   // {
    sensors.requestTemperatures(); // 发送命令获取温度
    delay(50);
    //float temp = ds.getTempC() - 6.13;
    currTemp = String((sensors.getTempCByIndex(0) - 5.6), 1); //获取当前温度值

    //currTemp = ds.getTempC();
    timeSinceLastCurrUpdate = millis();
    //}
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
      Serial.println(F("达到更新日期时间"));
    }
    timeSinceLastNTPUpdate = millis();
    while (Serial.read() >= 0)
    {
    }
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
  display.drawXbm(0, 0, 128, 64, bilibili); //显示哔哩哔哩logo
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
  drawProgress(display, 70, "NongLi...");
  HeFengClient.NongLingDate(&NongLi, DateAddress);
  delay(1000);
  drawProgress(display, 80, "Updating weather...");
  HeFengClient.doUpdateCurr(&currentWeather, HEFENG_KEY, HEFENG_LOCATION);
  delay(800);
  drawProgress(display, 90, "Get the current temperature");
  sensors.requestTemperatures();
  delay(50);
  currTemp = String((sensors.getTempCByIndex(0) - 5.6), 1); //获取当前温度值
  delay(800);
  drawProgress(display, 100, "Wellcome...");
  delay(600);
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
{
  timeClient.update();
  unsigned int Nowhours = timeClient.getHours(); //小时
  unsigned int minu = timeClient.getMinutes();   //分钟
  char buff[14];
  sprintf_P(buff, PSTR("%02d:%02d"), Nowhours, minu); //显示当天实时气象天气
  display->setFont(ArialMT_Plain_14);                 //字体总共5种规格10,14，16,24,自制12，14，显示过大可改12
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

  display->setColor(INVERSE);         //BLACK,WHITE,INVERSE
  display->setFont(ArialMT_Plain_12); //添加的自定义字体
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawString(0, 53, String(buff));
  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  display->drawString(128, 53, String(currTemp + "°C"));
  display->display();
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
// void drawHeaderOverlay(OLEDDisplay *display, OLEDDisplayUiState *state)
// { //绘图页眉覆盖
//   timeClient.update();
//   unsigned int Nowhours = timeClient.getHours(); //小时
//   unsigned int minu = timeClient.getMinutes();   //分钟
//   char buff[14];
//   sprintf_P(buff, PSTR("%02d:%02d"), Nowhours, minu);

//   display->setColor(INVERSE);         //BLACK,WHITE,INVERSE
//   display->setFont(ArialMT_Plain_12); //添加的自定义字体
//   display->setTextAlignment(TEXT_ALIGN_LEFT);
//   display->drawString(0, 53, String(buff));
//   display->setTextAlignment(TEXT_ALIGN_RIGHT);
//   display->drawString(128, 53, String(currTemp + "°C"));
// }

void drawDateTime2(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{ //显示中文星期和时间
  display->clear();
  timeClient.update();
  display->setFont(ArialMT_Plain_24); //显示字体大小
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawString(16 + x, 22 + y, timeClient.getFormattedTime()); //显示位置
  display->drawXbm(42, 5, 16, 16, xing);
  display->drawXbm(59, 5, 16, 16, qi);
  // display->drawHorizontalLine(0, 51, 128); //画水平线
  (*display).drawXbm(76, 5, 16, 16, Week[timeClient.getDay()]);
}

void five(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{ //农历显示
  timeClient.update();
  unsigned int Nowhours = timeClient.getHours(); //小时
  unsigned int minu = timeClient.getMinutes();   //分钟
  char buff[14];
  sprintf_P(buff, PSTR("%02d:%02d"), Nowhours, minu);

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

  display->setColor(INVERSE);         //BLACK,WHITE,INVERSE
  display->setFont(ArialMT_Plain_12); //添加的自定义字体
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawString(0, 53, String(buff));
  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  display->drawString(128, 53, String(currTemp + "°C"));
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
void PANDA(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{ //太空人动漫
  unsigned int i;
  for (i = 0; i < 45; i++)
  {
    display->clear();
    delay(5);
    display->drawXbm(17, 0, 94, 64, spaceman[i]);
    display->display();
    delay(80);
  }
}
//下雪的函数
void ShowSnow(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  int a[66], i, CNT1 = 0, num = 0;
  struct Snow
  {
    short x;
    short y;
    short speed;
  } snow[100];

  //randomSeed(1);
  randomSeed(analogRead(A0));
  for (i = 0; i < 66; i++)
    a[i] = (i - 2) * 10;
  display->clear();
  //while (1)
  do
  {
    if (num != 100)
    {
      snow[num].speed = 1 + random(5);
      i = random(67);
      snow[num].x = a[i];
      snow[num].y = 0;
      num++;
    }
    for (i = 0; i < num; i++)
    {
      snow[i].y += snow[i].speed;
      display->setPixel(snow[i].x, snow[i].y + 1); //雪花像素大小
      display->setPixel(snow[i].x + 1, snow[i].y);
      display->setPixel(snow[i].x, snow[i].y);
      display->setPixel(snow[i].x - 1, snow[i].y);
      display->setPixel(snow[i].x, snow[i].y - 1);
      if (snow[i].y > 63)
      {
        snow[i].y = 0;
      }
    }
    delay(30);
    display->display();
    display->clear();
    CNT1++;
  } while (CNT1 < 96);
}
/**********************星空动画***********************************/
void ShowStars(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  //  unsigned long timeSince = 0; //上次温度更新后的时间
  //  int interval_time = 10000;
  display->clear();
  int i;
  int StarCount = 0;
  //  int countSnow = 0;
  //  int fps = 60;
  typedef struct START
  {
    short x;
    short y;
    short speed;
    unsigned char speedcount;
    unsigned char isexist;
  } Star;

  Star star[132] = {0};
  //srand(2);
  randomSeed(analogRead(A0));
  for (i = 0; i < 132; i++)
  {
    if (star[i].isexist == 0)
    {
      //设置128个()星星的初始信息
      star[i].x = random(0, 132); //随机生成初始x坐标
      star[i].y = random(0, 65);  //随机生成y的坐标
      star[i].speedcount = 0;
      star[i].speed = random(8) + 1; //1-8的数
      star[i].isexist = 1;
    }
  }
  do
  {
    display->clear();
    //此段函数一直在运行
    //依次画出128个星星
    for (i = 1; i < 132; i++)
    {
      //如果这一个星星已经移动到退出屏幕界面
      //则在最左侧重新生成一颗新星星
      if (star[i].isexist == 0)
      {
        star[i].x = 1;
        star[i].y = random(0, 64);
        star[i].speed = random(0, 8) + 1;
        star[i].speedcount = 0;
        star[i].isexist = 1;
      }
      else
      {
        star[i].speedcount++;
        if (star[i].x >= 132) //标记已经退出屏幕
          star[i].isexist = 0;

        //清除上一个时刻画的星星(的尾巴) 不管有没有操作 都进行清除操作
        display->setColor(INVERSE); //BLACK,WHITE,INVERSE
        display->drawLine(star[i].x, star[i].y, star[i].x, star[i].y);
        // display->display();
        if (star[i].speedcount == star[i].speed) //运行时间到了一定的长度
        {
          star[i].speedcount = 0; //复位运行时间并向右移一格
          star[i].x += 1;         //总之星星的结束需要在这经历124次
        }                         //只不过有的更快 就能移动更快
        //从头到尾画出整条星星 不管星星是否已经变化
        display->drawLine(star[i].x, star[i].y, star[i].x + (6 / star[i].speed) - 1, star[i].y);
        // display->display();
      }
    }
    delay(40);
    display->display();
    display->clear();
    StarCount++;
  } while (StarCount < 64);
}
/**********************多边形动画***********************************/
void ShowPolygon(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  display->clear();
  int JiShu = 0;
  int x0 = 63, y0 = 31; //正多边形的外接圆的圆心
  unsigned char i = 0, j;
  int n = 1, r = 31;        //画正n边形 大小半径31
  int v = 1, countShow = 0; //每个相邻的多边形隔1 画count次后退出
  int a[30], b[30];
  do
  {
    display->clear();
    for (i = 0; i < n; i++)
    {
      a[i] = r * cos(2 * 3.1415926 * i / n) + x0;
      b[i] = r * sin(2 * 3.1415926 * i / n) + y0;
    }
    for (i = 0; i <= n - 2; i++)
    {
      for (j = i + 1; j <= n - 1; j++)
        display->drawLine(a[i], b[i], a[j], b[j]);
      delay(5);
      // display->display();
    }
    n += v;
    if (n == 12 || n == 0)
    {
      v = -v;
    }

    delay(300); //延时停顿100ms
    display->display();
    if (++countShow == 90)
    {
      countShow = 0;
      return;
    }
    JiShu++;
  } while (JiShu < 96);
}
/**********************划线和矩形动画***********************************/
void DrawLineRect(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  int i = 0, j = 0;
  display->clear();
  //for(i=0;i<16;i++)
  do
  {
    i++;
    display->drawLine(0, 0, i * 12, 63);
    display->display();
    delay(100);
  } while (i < 10);
  //for(i=0;i<16;i++)
  i = 0;
  do
  {
    i++;
    display->drawLine(128, 0, 128 - i * 12, 63);
    display->display();
    delay(120);
  } while (i < 10);
  i = 0;
  delay(500);

  //矩形
  for (j = 0; j < 2; j++)
  {
    // if (j == 0)
    display->clear();
    for (i = 0; i < 31; i += 2)
    {
      display->drawRect(i * 2, i, 128 - i * 4, 64 - 2 * i); //画矩形外框
      display->display();
      delay(100);
    }
    if (j == 0)
    {
      display->clear();
      delay(500);
      for (i = 32; i > 0; i -= 2)
      {
        display->fillRect(i * 2, i, 128 - i * 4, 64 - 2 * i); //画实心矩形
        display->display();
        delay(100);
      }
      display->clear();
    }
    // display->setColor(INVERSE); //BLACK,WHITE,INVERSE
  }
  //display->setColor(WHITE); //BLACK,WHITE,INVERSE
  //矩形
  for (j = 0; j < 2; j++)
  {
    // if (j == 0)
    display->clear();
    for (i = 0; i < 21; i += 2)
    {
      display->drawRect(i * 2, i, 128 - i * 4, 64 - 2 * i);
      display->display();
      delay(100);
    }
    //if (j == 0)
    display->clear();
    delay(500);
    for (i = 22; i > 2; i -= 2)
    {
      display->fillRect(i * 2, i, 128 - i * 4, 64 - 2 * i);
      display->display();
      delay(100);
    }
    display->clear();
    // display->setColor(INVERSE); //BLACK,WHITE,INVERSE
  }
  //display->setColor(WHITE); //BLACK,WHITE,INVERSE
  delay(50);
}
void DrawPolygon(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  //绘制三角形
  display->clear();
  display->drawTriangle(5, 10, 100, 30, 60, 50);
  display->display();
  delay(400);
  display->clear();
  display->fillTriangle(5, 5, 100, 30, 60, 50);
  display->display();
  delay(400);
  display->clear();
  display->drawTriangle(28, 30, 128, 10, 60, 50);
  display->display();
  delay(400);
  display->clear();
  display->fillTriangle(28, 30, 128, 10, 60, 50);
  display->display();
  delay(400);

  //五角星绘制
  display->clear();
  display->drawLine(63, 0, 86, 63); //五角星绘制
  display->display();
  delay(400);
  display->drawLine(86, 63, 25, 23); //画线
  display->display();
  delay(400);
  display->drawLine(25, 23, 101, 23); //画线
  display->display();
  delay(400);
  display->drawLine(101, 23, 40, 64); //画线
  display->display();
  delay(400);
  display->drawLine(40, 64, 63, 0); //画线
  display->display();
  delay(400);
  display->fillTriangle(63, 0, 54, 23, 72, 23); //顶部三角形
  display->display();
  delay(400);
  display->fillTriangle(101, 23, 77, 39, 72, 23); //右边三角形
  display->display();
  delay(400);
  display->fillTriangle(63, 48, 77, 39, 86, 63); //右下边三角形
  display->display();
  delay(400);
  display->fillTriangle(63, 48, 40, 64, 48, 40); //左下边三角形
  display->display();
  delay(400);
  display->fillTriangle(25, 23, 54, 23, 49, 39); //左边三角形
  display->display();
  delay(1000);
  display->clear();
}

/**********************数学函数**********************************/
void Math(OLEDDisplay *display, OLEDDisplayUiState *state, int16_t x, int16_t y)
{
  display->clear();
  math_xy();
  math_sin();
  math_xy();
  math_jcb();
  math_xy2();
  math_jcb2();
  math_xy2();
  math_sin2();
  math_xy();
  math_tyuan();
  math_xy();
  math_quxian();
  math_xy();
  math_paowuxian();
  //   math_xy();
  //  math_shuangquxian();
  delay(800);
  display->clear();
  display->drawXbm(0, 0, 128, 64, shoushi);
  display->display();
  //delay(500);
  //ui.nextFrame();//下一屏内容显示
  ui.switchToFrame(0); //切换到第一个画面
  //  ui.transitionToFrame(0);
}
void math_sin()
{
  int y1, y2, x1 = 0;
  for (int i = 0; i < 320; i++)
  {
    float mid = 0.4 * i;
    y1 = 31 + 31 * sin(mid / 6 - 2.8);
    y2 = 31 + 31 * cos(mid / 6 - 1.5);
    delay(10);
    x1 = int(mid);
    display.setPixel(x1, y1);
    display.setPixel(x1, y2);
    display.display();
    delay(50);
  }
  display.clear();
}
void math_sin2()
{
  int y1, y2, x1 = 0;
  for (int i = 0; i < 320; i++)
  {
    float mid = 0.4 * i;
    y1 = 32 - 31 * sin(mid * 3.14 / 28);
    y2 = 32 - 31 * cos(mid * 3.14 / 28);
    delay(10);
    x1 = int(mid);
    display.setPixel(x1, y1);
    display.setPixel(x1, y2);
    display.display();
    delay(50);
  }
  display.clear();
}
void math_xy2(void)
{
  display.clear();
  display.setColor(WHITE);          //BLACK,WHITE,INVERSE
  display.drawLine(0, 32, 128, 32); //描绘坐标系X轴
  display.drawLine(10, 0, 10, 64);  //描绘坐标系Y轴
  display.drawLine(10, 0, 7, 4);    //描绘小箭头
  display.display();
  delay(80);
  display.drawLine(10, 0, 13, 4);
  display.drawLine(128, 32, 124, 28); //小箭头
  display.drawLine(128, 32, 124, 36); //x画个箭头
  display.display();
  delay(80);
}
void math_xy(void)
{
  display.clear();
  display.setColor(WHITE);            //BLACK,WHITE,INVERSE
  display.drawLine(0, 31, 127, 31);   //x轴
  display.drawLine(128, 31, 122, 34); //x画个箭头
  display.drawLine(128, 31, 122, 28); //x画个箭头
  display.display();
  delay(80);
  display.drawLine(65, 63, 65, 0); //y轴
  display.drawLine(65, 0, 62, 5);  //y画个箭头
  display.drawLine(65, 0, 68, 5);  //y画个箭头
  display.display();
  delay(80);
}
void math_tyuan()
{
  int y1, y2;
  for (int i = 20; i < 110; i++)
  { //float mid = 0.4 * i;
    y1 = 32 + 2 * sqrt(32 * 60 - pow((i - 64), 2)) / 3;
    y2 = 32 - 2 * sqrt(32 * 60 - pow((i - 64), 2)) / 3;
    delay(10);
    // x1 = int(mid);
    display.setPixel(i, y1);
    display.setPixel(i, y2);
    display.display();
    delay(100);
  }
  display.clear();
}
void math_paowuxian()
{
  int y1, y2;
  for (int i = 0; i < 128; i++)
  {
    y1 = 0.01 * pow((i - 64), 2) + 22;
    y2 = (-0.01) * pow((i - 64), 2) + 42;
    delay(10);
    display.setPixel(i, y1);
    display.setPixel(i, y2);
    display.display();
    delay(100);
  }
  delay(1000);
}
void math_quxian()
{
  int y1, y2;
  for (int i = 0; i < 64; i++)
  {

    y2 = 32 - 3 * sqrt(63 - pow((64 - i), 2) / 64);
    y1 = 32 + 2 * sqrt(pow((64 + i), 2) / 62 - 65);

    display.setPixel((64 + i), y1);
    display.setPixel((64 - i), y2);
    display.display();
    delay(100);
  }
  display.clear();
}
void math_jcb(void)
{ //锯齿波1显示
  for (int x = 0; x < 320; x++)
  {
    int new_x, y;
    volatile float mid = 0.1 * x;
    y = 60 * fabs(asin(fabs(sin((mid + 1) / 2))) - 0.5); //0.785
    //y2 = 80*fabs(asin(fabs(sin((mid/8+1.57)/2)))-0.785);
    new_x = int(x / 2.5);
    delay(5);
    display.setPixel(new_x, y);
    display.display();
    delay(50);
  }
}

void math_jcb2(void)
{ //锯齿波2显示
  for (int x = 0; x < 320; x++)
  {
    int y, new_x;
    float mid = 0.1 * x;
    y = 80 * fabs(asin(fabs(sin((mid + 1.57) / 2))) - 0.785);
    new_x = int(x / 2.5);
    delay(5);
    display.setPixel(new_x, y);
    display.display();
    delay(50);
  }
  display.clear();
}
/**********************思维动画**********************************/
void MovMind()
{
  display.clear();
  unsigned int MINDMAX = 14;
  // unsigned int SSD1351_WIDTH = 64;
  // unsigned int SSD1351_HEIGHT = 128;
  // char Index;
  typedef struct
  {
    float x;    //运动的思维点位横坐标
    float y;    //运动的思维点位纵坐标
    float dirx; //运动的思维点位横坐标运动量
    float diry; //运动的思维点位纵坐标运动量
    float r;    //
    //	u16 color;//运动的思维点位颜色
  } MTMOVMIND;
  MTMOVMIND mtmovmind[MINDMAX];

  unsigned int i, j;
  for (i = 0; i < MINDMAX; i++)
  {
    mtmovmind[i].x = rand() % 128;
    mtmovmind[i].y = rand() % 64;
    mtmovmind[i].dirx = (rand() % 30 - 15) * 0.1f;
    mtmovmind[i].diry = (rand() % 30 - 15) * 0.1f;
    if (mtmovmind[i].dirx < 0.2 && mtmovmind[i].dirx > -0.2)
      mtmovmind[i].dirx = 0.5;
    if (mtmovmind[i].diry < 0.2 && mtmovmind[i].diry > -0.2)
      mtmovmind[i].diry = 0.5;
  }
  for (i = 0; i < MINDMAX; i++)
  {
    for (j = 0; j < MINDMAX; j++)
    {
      if ((mtmovmind[i].x - mtmovmind[j].x) * (mtmovmind[i].x - mtmovmind[j].x) + (mtmovmind[i].y - mtmovmind[j].y) * (mtmovmind[i].y - mtmovmind[j].y) < 900)
      {
        display.drawLine(mtmovmind[j].x, mtmovmind[j].y, mtmovmind[i].x, mtmovmind[i].y);
      }
    }
  }

  for (i = 0; i < MINDMAX; i++)
    display.fillCircle(mtmovmind[i].x, mtmovmind[i].y, 2);
}

u16 GetMindDistanceSquare(u16 x0, u16 y0, u16 x1, u16 y1)
{
  return ((x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1));
}
void GPIO_State()
{

  for (int i = 0; i < 6; i++)
  {
    digitalWrite(cout1[i], !digitalRead(cout1[i]));
  }
}

void m()
{ //-- ("/m", HTTP_GET, m);----"x1")=="1" 房间号==设备号 http:ip/m?x1=1
  String ip = WiFi.localIP().toString().c_str();
  int Num = server.arg("x1").toInt();
  switch (Num)
  {

  case 1:
    switch (digitalRead(2))
    {
    case 0:
      digitalWrite(2, 1);
      htmlcode();
      break;
    case 1:
      digitalWrite(2, 0);
      htmlcode();
      break;
    }
    break;
  case 2:
    switch (digitalRead(16))
    {
    case 0:
      digitalWrite(16, 1);
      htmlcode();
      break;
    case 1:
      digitalWrite(16, 0);
      htmlcode();
      break;
    }
    break;
  case 3:
    switch (digitalRead(14))
    {
    case 0:
      digitalWrite(14, 1);
      htmlcode();
      break;
    case 1:
      digitalWrite(14, 0);
      htmlcode();
      break;
    }
    break;
  case 4:
    switch (digitalRead(12))
    {
    case 0:
      digitalWrite(12, 1);
      htmlcode();
      break;
    case 1:
      digitalWrite(12, 0);
      htmlcode();
      break;
    }
    break;
  case 5:
    switch (digitalRead(15))
    {
    case 0:
      digitalWrite(15, 1);
      htmlcode();
      break;
    case 1:
      digitalWrite(15, 0);
      htmlcode();
      break;
    }
    break;
  case 6:
    switch (flag)
    {
    case 0:
      display.displayOff();
      htmlcode();
      flag = true;
      break;
    case 1:
      display.displayOn();
      flag = false;
      htmlcode();
      break;
    }
    break;
  case 7:
    GPIO_State();
    htmlcode();
    break;
  case 8:
    htmlcode();
    delay(1000);
    ESP.restart();
    break;
  case 10:
    setReadyForUpdate(); //3天
    htmlcode();
    break;
  case 11:
    setReadyForWeatherUpdate(); //实时天气
    htmlcode();
    break;
  case 12:
    setReadyNLiUpdate(); //农历更新
    htmlcode();
    break;
  case 13:
    setReadyTodayUpdate(); //当天日期更新
    htmlcode();
    break;
  default:
    htmlcode();
    break;
  }
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
  String g = (readyForUpdate) ? "black" : "red";        // 3天更新标志
  String h = (readyForWeatherUpdate) ? "black" : "red"; // 实时天气更新标志
  String x = (readyNLiUpdate) ? "black" : "red";        // 农历更新标志
  String y = (readyTodayUpdate) ? "black" : "red";      //当天日期更新标志
  String f = (flag) ? "black" : "red";
  String html = "<!DOCTYPE html><html lang='zh-CN'><head><meta charset='UTF-8'><meta name='viewport'content='width=device-width, initial-scale=1.0, minimum-scale=0.5, maximum-scale=2.0, user-scalable=yes' /><meta http-equiv='X-UA-Compatible' content='ie=edge'><title>esp8266气象时钟控制主页</title><link rel='stylesheet' href='http://at.alicdn.com/t/font_1640699_nk29qyrqhco.css'><style>ul {display: inline;list-style-type: none;margin: 80px 0px 35px 0px;text-align: center;font-size: 16px;text-decoration: none;}li {display: inline;display: inline-block;width: 100px;padding: 10px;background-color: #ff9137; } li a {font-family: Arial, 'Microsoft YaHei';text-decoration: none;display: block; } body {width: 360px;margin: 0;padding: 0;background: LightCyan;} .button {width: 130px;height: 70px;text-align: center;color: #FFF;border-radius: 1px;margin: 40px -4.5px 0px 0;position: relative;overflow: hidden;border: 2px solid rgb(2, 69, 253);background: darkcyan;font-size: 14px;outline: none;border-radius: 5px 5px 5px 5px;} .top1 {width: 360px;height: 45px;color: rgb(53, 136, 5);border: 2px;background: rgb(233, 235, 236);font-family: Arial, '宋体';font-style: 700;font-size: 20px;left: -15px;position: relative;top: 2px; }.device1 {left: 61px;position: relative;font-size: 20px;font-family: 'Microsoft yahei';font-style: 700;} .nav {left: -15px;} nav a {display: inline;margin-left: 30px;border-right: 0px solid #fff;width: 128px;border: 2px solid rgb(3, 59, 245);text-align: center;font-size: 24px;font: 700;font-family: 'Franklin Gothic Medium', 'Arial Narrow', Arial, sans-serif;color: #fc0758;text-decoration: none;}nav a.active {background-color: DarkTurquoise;    }</style></head><body><a href='/'><button class='top1'type='button'><strong> esp8266气象时钟控制主页</strong></button></a><dl><center><span style='left: -16px; position: relative;' id='time1'> Time Loading ......</span><script>  setInterval('time1.innerHTML=new Date().toLocaleString()', 1000);</script></center><div class='device1'><font style='font-family:SimHei'>&#x8BBE;&#x5907;&#x5728;&#x7EBF;&#x65F6;&#x95F4;:<a>";
  html += str;
  html += "</a></font></div><center style='left: -15px; position: relative;'><a href='./m?x1=1'><button type='button' class='button'><div class='m x-keting1' style='font-size:40px; color: " + a + ";'></div>&#x5BA2; &#x5385;</button></a>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href='./m?x1=2'><button type='button' class='button'><div class='m x-woshi' style='font-size:40px; color:" + b + ";'></div>&#x5367;&#x5BA4;</button></a></br><a href='./m?x1=3'><button type='button' class='button'><div class='m x-chuanglian' style='font-size:40px; color: " + c + ";'></div>&#x7A97;&#x5E18;</button></a>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href='./m?x1=4'><button type='button' class='button'><div class='m x-chufang' style='font-size:40px; color: " + d + ";'></div>&#x53A8;&#x623F;</button></a></br><a href='./m?x1=5'><button type='button' class='button'><div class='m x-reshuiqi' style='font-size:40px; color: " + e + ";'></div>&#x70ED;&#x6C34;&#x5668;</button></a>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href='./m?x1=6'><button type='button' class='button'><div class='m x-deng1' style='font-size:40px; color: " + f + ";'></div>OLED&#x5C4F;&#x5E55;</button></a></br><a href='./m?x1=7'><button type='button' class='button'><div class='m x-huandengpao' style='font-size:40px;'></div>&#x603B;&#x5F00;&#x5173;</button></a>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href='./m?x1=8'><button type='button' class='button'><div class='m x-kongtiao' style='font-size:40px;'></div>&#x91CD;&#x542F;ESP8266</button></a><a href='./m?x1=10'><button type='button' class='button'><div class='m x-M' style='font-size:40px; color: " + e + ";'></div>3天气象</button></a>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href='./m?x1=11'><button type='button' class='button'><div class='m x-fengshan' style='font-size:40px; color: " + f + ";'></div>实时天气</button></a></br><a href='./m?x1=12'><button type='button' class='button'><div class='m x-shuimian' style='font-size:40px;'></div>农历</button></a>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href='./m?x1=13'><button type='button' class='button'><div class='m x-icon-test' style='font-size:40px;'></div>NTP时间</button></a></center></ul><br></body></html>";

  server.send(1000, "text/html", html); //字符串数量4660
}
