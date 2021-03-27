
#pragma once
#include <ArduinoJson.h>

typedef struct HeFengCurrentData {
  String Today;
  String cond_txt;
  String fl;
  byte tmp;
  byte hum;
  byte wind_sc;
  String iconMeteoCon;

}
CurrentData;

typedef struct HeFengForeData {
  String datestr;
  String tmp_min;
  String tmp_max;
  String iconMeteoCon;
}
ForeData;

typedef struct NongLingDate0 {

  //  String ganzhiDate;//"二零二零年冬月十六"
  //  String shengxiaoDate;//"鼠"
  //  String nongliDate;
  //  String chineseDate;//"庚子"
  byte data_lunarDay;
  byte data_lunarMonth;
  unsigned int data_lunarYear;
}
NongLi_Date;//声明别名

typedef struct Work_Data {

  String workdate;
  //  String Date_Time；
}
WorkData;

typedef struct WriteString {
  int a;
  int b;
  String str;
}
Write_String;

class HeFeng {
  private:
    String getMeteoconIcon(String cond_code);
    String Read_String(int a, int b);
  public:
    HeFeng();
    void doUpdateCurr(CurrentData *data, String HEFENG_KEY, String HEFENG_LOCATION);//获取当前气象数据
    void doUpdateFore(ForeData *data, String HEFENG_KEY, String HEFENG_LOCATION);//获取三天气象数据
    void NongLingDate(NongLi_Date *data, String DateAddress);//获取农历
    void WorkDate(WorkData *data, String WorkDate);//获取当前日期
    void Write_String(int a, int b, String str);
};
