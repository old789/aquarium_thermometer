
#include <Wire.h>
#include <hd44780.h>  // https://github.com/duinoWitchery/hd44780
#include <hd44780ioClass/hd44780_I2Cexp.h>

hd44780_I2Cexp lcd;

const int LCD_COLS = 16;
const int LCD_ROWS = 2;

void setup() {
  int status;
  
  status=lcd.begin(LCD_COLS, LCD_ROWS);
  if(status){
    hd44780::fatalError(status);
  }
  lcd.clear();
  lcd.print("Hello, World!");
  lcd.setCursor(0,1);
  lcd.print("Hello, World!");
}

void loop() {}
