
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

void setup() {
  int status;

  Serial.begin(115200);
  delay(100);
  Serial.println("\r\nBooting...");

  status=lcd.begin(LCD_COLS, LCD_ROWS);
  if(status){
    Serial.printf("LCD initialise failed, status = %d\r\n", status);
    while(true){ delay(100); }
  }

  lcd.clear();
  if (ds1.begin() == false){
    lcd.print("sensor 1 fail"); 
  }else{
    lcd.print("sensor 1 Ok!"); 
  }
  
  lcd.setCursor(0,1);

  if (ds2.begin() == false){
    lcd.print("sensor 2 fail"); 
  }else{
    lcd.print("sensor 2 Ok!"); 
  }
  
  ds1.setResolution(10);
  ds2.setResolution(10);

  ds1.requestTemperatures();
  ds2.requestTemperatures();
  delay(10000);
  lcd.clear();
}

void loop() {
  if (ds1.isConversionComplete()){
    lcd.setCursor(0,0);
    lcd.print("t1 = ");
    lcd.print(ds1.getTempC(),1);
    lcd.print("  ");
    ds1.requestTemperatures();
  }
  if (ds2.isConversionComplete()){
    lcd.setCursor(0,1);
    lcd.print("t2 = ");
    lcd.print(ds2.getTempC(),1);
    lcd.print("  ");
    ds2.requestTemperatures();
  }
  //lcd.clear();
  delay(60000);
}
