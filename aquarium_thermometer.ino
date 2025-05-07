
#include <Wire.h>
#include <hd44780.h>  // https://github.com/duinoWitchery/hd44780
#include <hd44780ioClass/hd44780_I2Cexp.h>

hd44780_I2Cexp lcd;

const int LCD_COLS = 16;
const int LCD_ROWS = 2;

void setup() {
  int status;

  Serial.begin(115200);
  delay(100);  
  status=lcd.begin(LCD_COLS, LCD_ROWS);
  if(status){
    Serial.printf("LCD initialise failed, status = %d\r\n", status);
    while(true){ delay(100); }
  }
  lcd.clear();
  lcd.print("Line 1");
  lcd.setCursor(0,1);
  lcd.print("Line 2");
}

void loop() {}
