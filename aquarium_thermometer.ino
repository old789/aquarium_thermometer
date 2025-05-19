
#include <Wire.h>
#include <hd44780.h>  // https://github.com/duinoWitchery/hd44780
#include <hd44780ioClass/hd44780_I2Cexp.h>
#include <DS18B20.h>  // https://github.com/RobTillaart/DS18B20_RT
#include "TickTwo.h"  // https://github.com/sstaub/TickTwo
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <mDNSResolver.h>

hd44780_I2Cexp lcd;

OneWire oneWire1(0);
OneWire oneWire2(14);

DS18B20 ds1(&oneWire1);
DS18B20 ds2(&oneWire2);

const int LCD_COLS = 16;
const int LCD_ROWS = 2;

#define HALT while(true){ delay(100); }

double t1, t2;
bool enable_cli = false;
unsigned int roll_cnt=0;
// char roller[] = { '|', '/', '-', '\\' };
char roller[] = { 238, 239 };  // cheap LCD d'not have some characters, so using that
bool wifi_is_ok = false;

#include "config.h"

WiFiEventHandler on_wifi_connect_handler;
WiFiEventHandler on_wifi_got_IP_handler;
WiFiEventHandler on_wifi_disconnect_handler;

void read_themperatures();
void roll_roller();
void report();

TickTwo timer1( read_themperatures, 10000);
TickTwo timer2( roll_roller, 700);
TickTwo timer3( report, 60000);

void setup() {
  int status;
  bool sensor1_fail=true, sensor2_fail=true;
  PGM_P msg_booting=PSTR("\r\nBooting...");
  PGM_P msg_lcd_fail=PSTR("LCD initialise failed, status = ");
  PGM_P msg_sensor1 = PSTR("sensor 1 ");
  PGM_P msg_sensor2 = PSTR("sensor 2 ");
  PGM_P msg_fail = PSTR("fail");
  PGM_P msg_ok = PSTR("Ok!");
  // PGM_P msg_ = PSTR();
  // PGM_P msg_ = PSTR();
  // PGM_P msg_ = PSTR();
 
  Serial.begin(115200);
  delay(100);
  Serial.println(FPSTR(msg_booting));

  status=lcd.begin(LCD_COLS, LCD_ROWS);
  if(status){
    Serial.print(FPSTR(msg_lcd_fail));
    Serial.println(status);
    HALT
  }

  lcd.clear();
  lcd.print(FPSTR(msg_sensor1));
  Serial.print(FPSTR(msg_sensor1));
  if (ds1.begin() == false){
    lcd.print(FPSTR(msg_fail)); 
    Serial.println(FPSTR(msg_fail)); 
  }else{
    sensor1_fail=false;
    lcd.print(FPSTR(msg_ok)); 
    Serial.println(FPSTR(msg_ok)); 
  }
  
  lcd.setCursor(0,1);
  lcd.print(FPSTR(msg_sensor2));
  Serial.print(FPSTR(msg_sensor2));
  if (ds2.begin() == false){
    lcd.print(FPSTR(msg_fail)); 
    Serial.println(FPSTR(msg_fail)); 
  }else{
    sensor2_fail=false;
    lcd.print(FPSTR(msg_ok)); 
    Serial.println(FPSTR(msg_ok)); 
  }
  
  if ( sensor1_fail or sensor2_fail ) {
    HALT
  }

  ds1.setResolution(12);
  ds2.setResolution(12);

  ds1.requestTemperatures();
  ds2.requestTemperatures();
  on_wifi_connect_handler = WiFi.onStationModeConnected(on_wifi_connect);
  on_wifi_got_IP_handler = WiFi.onStationModeGotIP(on_wifi_got_IP);
  on_wifi_disconnect_handler = WiFi.onStationModeDisconnected(on_wifi_disconnect);
  wifi_init();
  delay(2000);
  timer1.start();
  timer2.start();
  timer3.start();
  //timer4.start();
  lcd.clear();
  //IPAddress ip = resolver.search("cub.local");
  
}

void loop() {
  if (enable_cli) {
    // loop_cli_mode();
  }else{
    loop_usual_mode();
  }
}

void loop_usual_mode() {
  timer1.update();
  timer2.update();
  timer3.update();
//  timer4.update();
//  if ( standalone == 0 ) {
//    timer5.update();
//  }
}

void read_themperatures(){
  PGM_P msg_t1 = PSTR("Aqua t = ");
  PGM_P msg_t2 = PSTR("Room t = ");
  PGM_P msg_spacer = PSTR("  ");
  // PGM_P msg_ = PSTR();
  if (ds1.isConversionComplete()){
    t1=ds1.getTempC();
    lcd.setCursor(0,0);
    lcd.print(FPSTR(msg_t1));
    lcd.print(t1,1);
    lcd.print(msg_spacer);
    ds1.requestTemperatures();
  }
  if (ds2.isConversionComplete()){
    t2=ds2.getTempC();
    lcd.setCursor(0,1);
    lcd.print(msg_t2);
    lcd.print(t2,1);
    lcd.print(msg_spacer);
    ds2.requestTemperatures();
  }
}

void roll_roller(){
  lcd.setCursor(15,0);
  lcd.print(roller[roll_cnt++]);
  if ( roll_cnt >= sizeof(roller) ) roll_cnt=0;
}

void report(){
  MDNS.update();
}
