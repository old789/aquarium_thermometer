
#include <Wire.h>
#include <hd44780.h>  // https://github.com/duinoWitchery/hd44780
#include <hd44780ioClass/hd44780_I2Cexp.h>
#include <DS18B20.h>  // https://github.com/RobTillaart/DS18B20_RT

hd44780_I2Cexp lcd;

OneWire oneWire1(0);
OneWire oneWire2(14);

DS18B20 ds1(&oneWire1);
DS18B20 ds2(&oneWire2);

const int LCD_COLS = 16;
const int LCD_ROWS = 2;

#define HALT while(true){ delay(100); }

double t1, t2;

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
  delay(10000);
  lcd.clear();
}

void loop() {
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
  delay(60000);
}
