#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <DS18B20.h>

#include <DNSServer.h>//密码直连将其三个库注释
#include <ESP8266WebServer.h>
#include <CustomWiFiManager.h>

#include <EEPROM.h>

#include <NTPClient.h>
//#include <WiFiUdp.h>

//#include <time.h>
//#include <sys/time.h>
//#include <coredecls.h>

#include "SH1106Wire.h"   //1.3寸用这个
//#include "SSD1306Wire.h"    //0.96寸用这个
#include "OLEDDisplayUi.h"

#include "HeFeng.h"
//#include "WeatherStationFonts.h"
#include "WeatherStationImages.h"

#include "font.h"

DS18B20 ds(D7); //温度传感器io口

//#include <U8g2lib.h>
//#define U8X8_HAVE_HW_I2C
#include <Wire.h>

//U8G2_SH1106_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0,  /*SCL*/  D1,  /*SDA*/  D2,   /*reset*/  U8X8_PIN_NONE);

#if defined(ESP8266)//OLED引脚定义
const int SDA_PIN = D2;//对应nodemcu接D5或者D1，，wemosD1mini的D2
const int SDC_PIN = D1;//对应nodemcu接D6或者D2，，wemosD1mini的D5
#else
const int SDA_PIN = D1;//对应nodemcu接D5或者D1
const int SCL = D2;//对应nodemcu接D6或者D2
#endif
const int I2C_DISPLAY_ADDRESS = 0x3c;  //I2c地址默认
SH1106Wire     display(I2C_DISPLAY_ADDRESS, SDA_PIN, SDC_PIN);   // 1.3寸用这个
//SSD1306Wire     display(I2C_DISPLAY_ADDRESS, SDA_PIN, SDC_PIN);   // 0.96寸用这个

String NongLingYear;
String NongLingMon;
String NongLingDay;

//const char* WIFI_SSID = "MERCURY_D268G";  //填写你的WIFI名称及密码MERCURY_D268G CMCC-DyVv
//const char* WIFI_PWD = "pba5ayzk";
const char* DateAddress = "https://api.xlongwei.com/service/datetime/convert.json";//农历;
//https://api.xlongwei.com/service/datetime/info.json
//https://api.xlongwei.com/service/datetime/workday.json
const char* WorkDate = "http://quan.suning.com/getSysTime.do";//日期
unsigned int*a;//EEPROM存放当前日期参数
unsigned int*b;
const char* str;

//由于太多人使用我的秘钥，导致获取次数超额，所以不提供秘钥了，大家可以到https://dev.heweather.com/获取免费的
const char* HEFENG_KEY = "f490e741439a4b4ba3e9c51037821170";//填写你的和风天气秘钥
//f490e741439a4b4ba3e9c51037821170
//bfaee7681d3649d88e03082f1a1e48e7 nodemcu
//95696b51ec11438899034a810a08fe7d
const char* HEFENG_LOCATION = "CN101250303";//填写你的城市ID,可到https://where.heweather.com/index.html查询
//const char* HEFENG_LOCATION = "auto_ip";//自动IP定位

//#define TZ              8      // 中国时区为8
//#define DST_MN          0      // 默认为0

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "ntp1.aliyun.com", 60 * 60 * 8, 30 * 60 * 1000);

const unsigned int UPDATE_NTP_SECS = 0.5 * 60; // 更新NTP时间更新频率
const unsigned int UPDATE_Weather_SECS = 6 * 60; // 更新实时天气的频率
const unsigned int UPDATE_For_SECS = 15 * 60; // 更新3天天气的频率
const unsigned int UPDATE_Temperature_SECS = 1 * 60; // DS18B20更新频率
const unsigned int UPDATE_Lunar_SECS = 60 * 60; //农历更新频率
const unsigned int UPDATE_Today_SECS = 30 * 60; //日期更新频率

const String WDAY_NAMES[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};  //星期
const String MONTH_NAMES[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};  //月份

// web配网页面自定义我的图标请随便使用一个图片转base64工具转换https://tool.css-js.com/base64.html, 64*64
//const char Icon[] PROGMEM = "data:image/png;base64,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";

OLEDDisplayUi   ui( &display );//初始化OLED

NongLi_Date NongLi;
CurrentData currentWeather; //结构体实例化对象
ForeData foreWeather[3];
WorkData NowToday;
HeFeng HeFengClient;

//#define TZ_MN           ((TZ)*60)   //时间换算
//#define TZ_SEC          ((TZ)*3600)
//#define DST_SEC         ((DST_MN)*60)

//time_t now; //实例化时间

bool readyForUpdate = false; // 3天更新标志
bool readyForWeatherUpdate = false; // 实时天气更新标志
bool readyNLiUpdate = false;  // 农历更新标志
bool readyTodayUpdate = false; //当天日期更新标志
bool first = true;  //首次更新标志
unsigned long timeSinceLastNTPUpdate = 0;    //上次NTP更新后的时间
unsigned long timeSinceLastToUpdate = 0;    //上次日期更新后的时间
unsigned long timeSinceLastWUpdate = 0;    //上次气象更新后的时间
unsigned long timeSinceLastFUpdate = 0;    //上次3天更新后的时间
unsigned long timeSinceLastNLUpdate = 0;    //上次农历更新后的时间
unsigned long timeSinceLastCurrUpdate = 0;   //上次温度更新后的时间

String currTemp = "8.88"; //温度初始值
void drawProgress(OLEDDisplay *display, int percentage, String label);   //提前声明函数
void updateData(OLEDDisplay *display);//根据millis和设定的时间进行判断更新天气间隔时间
void Day3updateData(OLEDDisplay *display) ;  //根据millis,3天气象更新
void NLiupdateData(OLEDDisplay *display);//更新农历设定时间
void TodayupdateData(OLEDDisplay *display);//更新当天日期设定时间
void updateDatas(OLEDDisplay *display);//首次更新，重启时更新
void drawDateTime(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);
void drawCurrentWeather(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);
void drawForecast(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);
void drawForecastDetails(OLEDDisplay *display, int x, int y, int dayIndex);
void drawHeaderOverlay(OLEDDisplay *display, OLEDDisplayUiState* state);
void setReadyForWeatherUpdate();  //实时天气
void setReadyForUpdate(); //3天
void setReadyNLiUpdate(); //农历
void setReadyTodayUpdate(); //当天日期
void drawDateTime2(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);
void five(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);
void six(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);

//添加框架
//此数组保留指向所有帧的函数指针
//框架是从右向左滑动的单个视图
FrameCallback frames[] = { drawCurrentWeather, drawDateTime2, drawForecast, drawDateTime, five, six}; //
//页面数量,图形绘制回调函数数组,
int numberOfFrames = 6;// 显示6屏内容信息

OverlayCallback overlays[] = { drawHeaderOverlay }; //覆盖回调函数
unsigned int numberOfOverlays = 1;  //覆盖数


void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  //  u8g2.begin();
  //  u8g2.clearDisplay();    // 清除显示数据及屏幕
  //  u8g2.clearBuffer();    // 清Buffer缓冲区的数据
  //  u8g2.sendBuffer();    // 将Buffer帧缓冲区的内容发送到显示器,发送刷新消息
  //  u8g2.enableUTF8Print();
  // 屏幕初始化
  display.init();
  display.clear();
  display.display();
  //    display.flipScreenVertically(); //屏幕翻转
 // display.mirrorScreen();//使用分光棱镜显示需要用到此函数
  display.setContrast(240); //屏幕亮度

  //Web配网
  webconnect();
  ui.setTargetFPS(80);  //刷新频率
  ui.setActiveSymbol(activeSymbole); //设置活动符号
  ui.setInactiveSymbol(inactiveSymbole); //设置非活动符号
  ui.disableAllIndicators();//不显示页码小点。
  // 符号位置
  // 你可以把这个改成TOP, LEFT, BOTTOM, RIGHT
  //ui.setIndicatorPosition(BOTTOM);// 显示页码小点位置，定义第一帧在栏中的位置
  //ui.setIndicatorDirection(LEFT_RIGHT);

  // 屏幕切换方向
  // 您可以更改使用的屏幕切换方向 SLIDE_LEFT, SLIDE_RIGHT, SLIDE_TOP, SLIDE_DOWN
  ui.setFrameAnimation(SLIDE_LEFT);//切屏方向

  ui.setFrames(frames, numberOfFrames); // 设置框架和显示屏幕内容数
  ui.setTimePerFrame(5000); //设置切换时间

  ui.setOverlays(overlays, numberOfOverlays); //设置覆盖的画面数

  // UI负责初始化显示
  ui.init();
  //    display.flipScreenVertically(); //屏幕反转
  //  display.mirrorScreen();//使用分光棱镜显示需要用到

//  configTime(TZ_SEC, DST_SEC, "ntp.ntsc.ac.cn", "ntp1.aliyun.com"); //ntp获取时间，你也可用其他"1.cn.pool.ntp.org","ntp1.aliyun.com"
//  delay(200);
  timeClient.begin();

}

void loop() {
  display.clear();
  if (first) {  //首次加载
    updateDatas(&display);
    first = false;
  }
  if (millis() - timeSinceLastWUpdate > (1000L * UPDATE_Weather_SECS)) { //天气更新频率
    setReadyForWeatherUpdate();
    timeSinceLastWUpdate = millis();
  }
  if (millis() - timeSinceLastToUpdate > (1000L * UPDATE_Weather_SECS)) { //当天日期更新频率
    setReadyTodayUpdate(); 
    timeSinceLastToUpdate = millis();
  }
  else if (millis() - timeSinceLastFUpdate > (1000L * UPDATE_For_SECS)) { //3天更新频率
    setReadyForUpdate();
    timeSinceLastFUpdate = millis();
  }
  else if (millis() - timeSinceLastCurrUpdate > (1000L * UPDATE_Temperature_SECS)) { //温度更新频率
    if ( ui.getUiState()->frameState == FIXED)
    {
      currTemp = String(ds.getTempC(), 2);//2表示小数点精度？
      //      currTemp = ds.getTempC();
      timeSinceLastCurrUpdate = millis();
    }
  }
  else if (millis() - timeSinceLastNTPUpdate > (1000L * UPDATE_NTP_SECS)) { //NTP更新频率
    timeClient.update();
    unsigned int Nowhours = timeClient.getHours();//小时
    unsigned int minu =  timeClient.getMinutes();//分钟
    unsigned int sece =  timeClient.getSeconds();//秒
    unsigned int TodayDay = timeClient.getDay();//星期
//    char buff[16];
//    sprintf_P(buff, PSTR("%02d:%02d:%02d"), timeClient.getHours(), timeClient.getMinutes(), timeClient.getSeconds());
//    Serial.println(String(buff));
//    Serial.println(timeClient.getFormattedTime());//20:36:13
//    String Date1 = "星期:";
    unsigned int T1 = Nowhours * 3600 + minu * 60 + sece;
    unsigned int T2 = 86400 - T1;

    Serial.println("现在时间秒数:" + String("") + T1 );
    Serial.println("距第二天剩余秒数:" + String("") + T2);
    Serial.println(String("") + "星期:" + TodayDay);

    //    Date1.concat(timeClient.getDay());//字符串和整数拼接
    //     Date1 += timeClient.getDay();
    //    Serial.print(Date1);
    //    String Now_hours ;
    //    String minu0;
    //    Now_hours += Nowhours;
    //    minu0 += minu;
    //    if (Now_hours == "00" && minu0 == "00" == true) {
    if (90 > T1) {
      setReadyNLiUpdate();//农历更新
      setReadyForWeatherUpdate();//实时天气更新，包含日期在内。
      Serial.println("现在时间秒数:" + String("") + T1 );
      Serial.println(F("达到更新农历时间."));
    }
    timeSinceLastNTPUpdate = millis();
  }
  else if (millis() - timeSinceLastNLUpdate > (1000L * UPDATE_Lunar_SECS)) { //农历更新频率
    setReadyNLiUpdate();
    timeSinceLastNLUpdate = millis();
  }
  else if (readyNLiUpdate && ui.getUiState()->frameState == FIXED) { //农历更新已准备就绪，且框架==固定的
    NLiupdateData(&display);
  }
  else if (readyTodayUpdate && ui.getUiState()->frameState == FIXED) { //当前日期更新已准备就绪，且框架==固定的
    TodayupdateData(&display);
  }
  else if (readyForWeatherUpdate && ui.getUiState()->frameState == FIXED) { //实时天气数据更新已准备就绪，且框架==固定的
    updateData(&display);
  }
  else if (readyForUpdate && ui.getUiState()->frameState == FIXED ) { //3天气象数据更新已准备就绪，且框架==固定的
    Day3updateData(&display);
  }

  int remainingTimeBudget = ui.update(); //剩余时间预算
  if (remainingTimeBudget > 0) {
    //你可以在这里工作如果你低于你的时间预算。
    delay(remainingTimeBudget);
  }

}

void webconnect() {  ////Web配网，密码直连将其注释
  display.clear();
  display.drawXbm(0, 0, 128, 64, bilibili); //显示哔哩哔哩
  display.display();

  WiFiManager wifiManager;  //实例化WiFiManager
  wifiManager.setDebugOutput(false); //关闭Debug
  //wifiManager.setConnectTimeout(10); //设置超时
  //wifiManager.setHeadImgBase64(FPSTR(Icon)); //设置配网页面中显示的图标
  wifiManager.setPageTitle("欢迎进入WiFi配置页");  //设置页标题

  if (!wifiManager.autoConnect("ESP8266-IOT-Display")) {  //AP模式下的wifi名称
    Serial.println(F("连接失败并超时"));
    //重新设置并再试一次，或者让它进入深度睡眠状态
    ESP.restart();
    delay(1000);
  }
  Serial.println("connected...^_^");
  yield();
}

void drawProgress(OLEDDisplay *display, int percentage, String label) {    //绘制进度
  display->clear();
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_10);
  display->drawString(64, 10, label);
  display->drawProgressBar(2, 28, 124, 10, percentage);
  display->display();
}

void updateData(OLEDDisplay *display) {  //实时天气更新
  HeFengClient.doUpdateCurr(&currentWeather, HEFENG_KEY, HEFENG_LOCATION);
  delay(1200);
  //  HeFengClient.doUpdateFore(foreWeather, HEFENG_KEY, HEFENG_LOCATION);
  //  delay(1200);
  readyForWeatherUpdate = false;
}

void Day3updateData(OLEDDisplay *display) {  //3天预报天气更新
  HeFengClient.doUpdateFore(foreWeather, HEFENG_KEY, HEFENG_LOCATION);
  delay(1200);
  readyForUpdate = false;
}

void NLiupdateData(OLEDDisplay *display) {  //农历更新
  HeFengClient.NongLingDate(&NongLi, DateAddress);
  delay(1200);
  readyNLiUpdate = false;
}

void TodayupdateData(OLEDDisplay *display) { //当天日期
  HeFengClient.WorkDate(&NowToday, WorkDate);
  delay(1200);
  readyTodayUpdate = false;
}

void updateDatas(OLEDDisplay *display) {  //首次天气、预报天气，农历的整体更新，重启时才调用
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

void drawDateTime(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {  //显示当前时间和日期
   timeClient.update();
//  now = time(nullptr);
//  struct tm* timeInfo;
//  timeInfo = localtime(&now);
//  char buff[16];

  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_16);
//  String date = WDAY_NAMES[timeInfo->tm_wday];

//  sprintf_P(buff, PSTR("%04d-%02d-%02d  %s"), timeInfo->tm_year + 1900, timeInfo->tm_mon + 1, timeInfo->tm_mday, WDAY_NAMES[timeInfo->tm_wday].c_str());//格式化输出时间，星期
//display->drawString(64 + x, 3 + y, currentWeather.Today);
 display->drawString(64 + x, 3 + y, NowToday.workdate);
//  display->drawString(64 + x, 5 + y, String(buff));
  display->setFont(ArialMT_Plain_24);

 // sprintf_P(buff, PSTR("%02d:%02d:%02d"), timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
  display->drawString(64 + x, 22 + y, timeClient.getFormattedTime());
 // display->drawString(64 + x, 22 + y, String(buff));
  display->setTextAlignment(TEXT_ALIGN_LEFT);
}

void drawCurrentWeather(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {  //显示当天实时气象天气
  // u8g2.clearBuffer();
  display->setFont(ArialMT_Plain_14);//字体总共5种规格10,14，16,24,自制12，14，显示过大可改12
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(32 + x, 35 + y, currentWeather.cond_txt);
  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawString(73 + x, 38 + y, "Wind: " + String(currentWeather.wind_sc) + "  ");
  display->setFont(ArialMT_Plain_24);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  String temp = String(currentWeather.tmp) + "°C" ;
  display->drawString(68 + x, 0 + y, temp);//温度显示位置
  display->setFont(ArialMT_Plain_10);
  display->drawString(95 + x, 26 + y, ": " + String(currentWeather.hum) + "%"); //湿度显示
  display->drawXbm(71, 26, 12, 12, shi);
  display->drawXbm(83, 26, 12, 12, du);
  display->setFont(Meteocons_Plain_40);//显示气象图标大小设置40，36和21三种,0.96寸屏幕就不要设置成40的，否则过大显示不全，40仅使用1.3寸。
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(30 + x, y - 2, currentWeather.iconMeteoCon);
  display->drawHorizontalLine(0, 51, 34);//画水平线
  display->drawHorizontalLine(68, 51, 60);//画水平线
  display->drawVerticalLine(68, 27, 24);//画垂直线
}

void drawForecast(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {  //天气三天预报位置布局
  drawForecastDetails(display, x - 8, y, 0);
  drawForecastDetails(display, x + 33, y, 1);
  drawForecastDetails(display, x + 77, y, 2);
}

void drawForecastDetails(OLEDDisplay *display, int x, int y, int dayIndex) {  //3天预报天气
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_10);
  display->drawString(x + 29, y, foreWeather[dayIndex].datestr);//预报日期
  display->drawRect(x + 13, y , 32, 12);// 画空心矩形w,h,宽度,高度
  display->setFont(Meteocons_Plain_21);//显示气象图标大小设置40，36和21三种。
  display->drawString(x + 23, y + 12, foreWeather[dayIndex].iconMeteoCon);//预报图标位置
  String temp = foreWeather[dayIndex].tmp_min + " | " + foreWeather[dayIndex].tmp_max;//预报气温范围
  display->setFont(ArialMT_Plain_12);
  display->drawString(x + 27, y + 34, temp);//预报温度显示位置
  display->setTextAlignment(TEXT_ALIGN_LEFT);

  display->setFont(ArialMT_Plain_12);//显示今天 明天 后天

  display->drawXbm(29 , 16, 12, 12, Jin);
  display->drawXbm(70 , 16, 12, 12, Ming);
  display->drawXbm(113, 16, 12, 12, Hou);
  //display->drawXbm(x + 42 ,y + 28, 12, 12, Tian);
  display->drawHorizontalLine(0, 51, 128);//画水平线
}


void drawHeaderOverlay(OLEDDisplay *display, OLEDDisplayUiState* state) {   //绘图页眉覆盖
  timeClient.update();
  unsigned int Nowhours = timeClient.getHours();//小时
  unsigned int minu =  timeClient.getMinutes();//分钟
  //  now = time(nullptr);
  //  struct tm* timeInfo;
  //  timeInfo = localtime(&now);
    char buff[14];
    sprintf_P(buff, PSTR("%02d:%02d"), Nowhours, minu);

  display->setColor(INVERSE);//BLACK,WHITE,INVERSE
  display->setFont(ArialMT_Plain_12);//添加的自定义字体
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawString(0, 53, String(buff));
  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  // float currTemp0 = currTemp - 2.6;
  // String temp = String("") + currTemp0 + "°C";
  String temp = currTemp + "°C";
  display->drawString(128, 53, temp);
  //  display->drawHorizontalLine(0, 51, 128);//画横线
}
/********************************更新前的逻辑判断函数*********************************************/
void setReadyForWeatherUpdate() {  //为实时天气更新做好准备
  Serial.println(F("Setting readyForUpdate to true"));
  readyForWeatherUpdate = true;
}
void setReadyForUpdate() {  //为3天更新做好准备
  Serial.println(F("Setting ready 3Day Update to true"));
  readyForUpdate = true;
}
void setReadyNLiUpdate() {  //为农历更新做好准备
  Serial.println(F("Setting readyNLiUpdate to true"));
  readyNLiUpdate = true;
}
void setReadyTodayUpdate() {  //为当前日期更新做好准备
  Serial.println(F("Setting readyTodayUpdate to true"));
  readyTodayUpdate = true;
}

/********************************更新前的逻辑判断函数*********************************************/

void drawDateTime2(OLEDDisplay * display, OLEDDisplayUiState * state, int16_t x, int16_t y) { //显示中文星期和时间
   display->clear();
    timeClient.update();
//  now = time(nullptr);
//  struct tm* timeInfo;
//  timeInfo = localtime(&now);
//  char buff[16];

//  display->setTextAlignment(TEXT_ALIGN_LEFT);
//  display->setFont(ArialMT_Plain_16);
//  String date = WDAY_NAMES[timeInfo->tm_wday];

//  sprintf_P(buff, PSTR("%02d/%02d"), timeInfo->tm_mon + 1, timeInfo->tm_mday);
//  display->drawString(9 + x, 5 + y, String(buff));

  display->setFont(ArialMT_Plain_24);//显示字体大小
  // sprintf_P(buff, PSTR("%02d:%02d:%02d"), timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
  // display->drawString(16 + x, 22 + y, String(buff));//显示位置
  display->setTextAlignment(TEXT_ALIGN_LEFT);  
  display->drawString(16 + x, 22 + y, timeClient.getFormattedTime());//显示位置

  display->drawXbm(42, 5, 16, 16, xing);
  display->drawXbm(59, 5, 16, 16, qi);
  display->drawHorizontalLine(0, 51, 128);//画水平线
  if (timeClient.getDay() == 1)
    display->drawXbm(76, 5, 16, 16, yi);
  else if (timeClient.getDay() == 2)
    display->drawXbm(76, 5, 16, 16, er);
  else if (timeClient.getDay() == 3)
    display->drawXbm(76, 5, 16, 16, san);
  else if (timeClient.getDay() == 4)
    display->drawXbm(76, 5, 16, 16, si);
  else if (timeClient.getDay() == 5)
    display->drawXbm(76, 5, 16, 16, wu);
  else if (timeClient.getDay() == 6)
    display->drawXbm(76, 5, 16, 16, liu);
  else if (timeClient.getDay() == 0)
    display->drawXbm(76, 7, 12, 12, ri);
}

void five(OLEDDisplay * display, OLEDDisplayUiState * state, int16_t x, int16_t y) {//农历显示
  display->clear();
//  now = time(nullptr);//取当前时间的函数

  //display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_24);//字体总共3种规格10,16,24,自制12
  // display->drawRect(21, 7, 23, 22);// 画空心矩形w,h,宽度,高度
  display->drawXbm(25, 10, 16, 16, hu);

  // display->drawRect(21, 7, 43, 22);// 画空心矩形w,h,宽度,高度
  display->drawXbm(45, 10, 16, 16, nan1);

  // display->drawRect(21, 7, 63, 22);
  display->drawXbm(65, 10, 16, 16, li);

  display->drawRect(21, 7, 85, 22);// 画空心矩形w,h,宽度,高度
  //display->drawRoundRect(21, 7, 85, 22,5,WHITE);//画空心圆角矩形
  display->drawXbm(85, 10, 16, 16, ling);

  display->setFont(ArialMT_Plain_12);   //自定义添加的Arimo#字体,
  display->drawXbm(3, 35, 12, 12, nong);
  display->drawXbm(15, 35, 12, 12, li2);

  display->setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
  display->drawString(44, 42, String(NongLi.data_lunarYear));
  //display->drawString(28, 35, NongLingYear);

  display->drawXbm(61, 35, 12, 12, Year1);
  display->setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
  //display->drawString(74, 35, NongLingMon);
  display->drawString(79, 42, String(NongLi.data_lunarMonth));

  display->drawXbm(87, 35, 12, 12, yue);
  display->setTextAlignment(TEXT_ALIGN_CENTER_BOTH);
  display->drawString(106, 42, String(NongLi.data_lunarDay));
  //  display->drawString(100, 35, NongLingDay);

  display->drawXbm(114, 35, 12, 12, ri);
  display->drawHorizontalLine(0, 51, 128);//画水平线
}

void six(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
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
