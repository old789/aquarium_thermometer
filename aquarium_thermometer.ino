
#include <Wire.h>
#include <hd44780.h>  // https://github.com/duinoWitchery/hd44780
#include <hd44780ioClass/hd44780_I2Cexp.h>
#include <DS18B20.h>  // https://github.com/RobTillaart/DS18B20_RT
#include "TickTwo.h"  // https://github.com/sstaub/TickTwo
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <PubSubClient.h>  // https://github.com/hmueller01/pubsubclient3
#include <Terminal.h>  // https://github.com/johngavel/Terminal

hd44780_I2Cexp lcd;

OneWire oneWire1(0);
OneWire oneWire2(14);

DS18B20 ds1(&oneWire1);
DS18B20 ds2(&oneWire2);

WiFiClient espClient;
PubSubClient client(espClient);

Terminal terminal(&Serial);

#define LCD_COLS 16
#define LCD_ROWS 2

#define UNSUCCESSFUL_ATTEMPTS_COUNT 30
#define HALT while(true){ delay(100); }

double t1, t2;
bool enable_cli = false;
unsigned int roll_cnt=0;
// char roller[] = { '|', '/', '-', '\\' };
char roller[] = { 238, 239 };  // cheap LCD d'not have some characters, so using that
bool wifi_is_ok = false;
IPAddress mqtt_host_ip(IPADDR_NONE);
IPAddress prev_mqtt_host_ip(IPADDR_NONE);
uint16_t prev_mqtt_port = 1883;
unsigned int unsucessfull_attempt = 0;
bool first_message_after_boot = true;

//   Config begin
char dev_name[33] = {0};  // Name of system for logging
char dev_model[33] = "2ch thermometre for an aquarium"; // Model of device
char ssid[33] = {0}; // WiFi SSID
char passw[65] = {0}; // WiFi password
bool network_enable = true; // Send data to MQTT or standalone mode ( false )
unsigned int mqtt_host_resolving = 0; // Resolving mode: 0 - mDNS, 1 - DNS
char mqtt_host[33] = {0}; // hostname ( DNS or mDNS mode )or IP ( DNS only mode )of a MQTT server
uint16_t mqtt_port = 1883;
char mqtt_user[33] = {0};  // MQTT authentification parameters
char mqtt_passw[33] = {0}; // MQTT authentification parameters
//   Config end

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
  int lcd_status;
 
  Serial.begin(115200);
  delay(100);
  Serial.println(F("\r\nBooting..."));

  lcd_status=lcd.begin(LCD_COLS, LCD_ROWS);
  if( lcd_status ){
    Serial.print(F("LCD initialise failed, status = "));
    Serial.println(lcd_status);
    HALT
  }

  lcd.clear();
  enable_cli=wait_for_key_pressed(10);
  lcd.clear();
  if ( enable_cli ) {
    setup_cli_mode();
  }else{
    setup_usual_mode();
  }
}

void loop() {
  if (enable_cli) {
    loop_cli_mode();
  }else{
    loop_usual_mode();
  }
}

void setup_usual_mode(){
  bool sensor1_fail=true, sensor2_fail=true;
  PGM_P msg_sensor1 = PSTR("sensor 1 ");
  PGM_P msg_sensor2 = PSTR("sensor 2 ");
  PGM_P msg_fail = PSTR("fail");
  PGM_P msg_ok = PSTR("Ok!");

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
  if ( network_enable ) {
    on_wifi_connect_handler = WiFi.onStationModeConnected(on_wifi_connect);
    on_wifi_got_IP_handler = WiFi.onStationModeGotIP(on_wifi_got_IP);
    on_wifi_disconnect_handler = WiFi.onStationModeDisconnected(on_wifi_disconnect);
    wifi_init();
  }
  delay(2000);
  lcd.clear();
  read_themperatures();
  timer1.start();
  timer2.start();
  if ( network_enable ) {
    timer3.start();
  }
}

void loop_usual_mode() {
  timer1.update();
  timer2.update();
  if ( network_enable ) {
    timer3.update();
  }
}

void read_themperatures(){
  if (ds1.isConversionComplete()){
    t1=ds1.getTempC();
    lcd.setCursor(0,0);
    lcd.print(F("Aqua t = "));
    lcd.print(t1,1);
    lcd.print(F("  "));
    ds1.requestTemperatures();
  }
  if (ds2.isConversionComplete()){
    t2=ds2.getTempC();
    lcd.setCursor(0,1);
    lcd.print(F("Room t = "));
    lcd.print(t2,1);
    lcd.print(F("  "));
    ds2.requestTemperatures();
  }
}

void roll_roller(){
  lcd.setCursor(15,0);
  lcd.print(roller[roll_cnt++]);
  if ( roll_cnt >= sizeof(roller) ) {
    roll_cnt=0;
  }
  if ( network_enable and wifi_is_ok ) {
    MDNS.update();
  }
}

void report(){
  unsucessfull_attempt++;
  if ( wifi_is_ok and mqtt_setserver() and mqtt_publish() ) {
    unsucessfull_attempt=0;
    print_warning_sign(false);
    return;
  }
  print_warning_sign(true);
  if ( unsucessfull_attempt > UNSUCCESSFUL_ATTEMPTS_COUNT ) {
    Serial.println(F("Too many unsucessfull attempts to publish, restart"));
    ESP.restart();
  }
}

void print_warning_sign(bool oops){
  lcd.setCursor(15,1);
  if ( oops ) {
    lcd.print("!");
  }else{
    lcd.print(" ");
  }
}

bool wait_for_key_pressed( uint8_t tries ) {
  Serial.println(F("\r\nPress space key for enter in configuration mode"));
  lcd.clear();
  for ( uint8_t i=0; i < tries; i++ ) {
      lcd.setCursor(0,0); lcd.print(F("Wait for input"));
      lcd.setCursor(0,1); lcd.printf("%02d",tries-i); 
      delay(1000);
      if ( Serial.available() ) {
        if ( Serial.read() == ' ' ){
          return(true);
        }
      }
  }
  return(false);
}

