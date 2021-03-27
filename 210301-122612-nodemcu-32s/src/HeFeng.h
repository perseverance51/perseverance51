
#pragma once
#include <ArduinoJson.h>

typedef struct HeFengCurrentData {

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

class HeFeng {
  private:
    String getMeteoconIcon(String cond_code);
  public:
    HeFeng();
    void doUpdateCurr(CurrentData *data, String HEFENG_KEY, String HEFENG_LOCATION);
    void doUpdateFore(ForeData *data, String HEFENG_KEY, String HEFENG_LOCATION);
    void NongLingDate(NongLi_Date *data, String DateAddress);
    
};
