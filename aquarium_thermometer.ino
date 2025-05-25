
#include <Wire.h>
#include <hd44780.h>  // https://github.com/duinoWitchery/hd44780
#include <hd44780ioClass/hd44780_I2Cexp.h>
#include <DS18B20.h>  // https://github.com/RobTillaart/DS18B20_RT
#include "TickTwo.h"  // https://github.com/sstaub/TickTwo
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <PubSubClient.h>  // https://github.com/hmueller01/pubsubclient3

hd44780_I2Cexp lcd;

OneWire oneWire1(0);
OneWire oneWire2(14);

DS18B20 ds1(&oneWire1);
DS18B20 ds2(&oneWire2);

WiFiClient espClient;
PubSubClient client(espClient);

const int LCD_COLS = 16;
const int LCD_ROWS = 2;

#define UNSUCCESSFUL_ATTEMPTS_COUNT 30
#define HALT while(true){ delay(100); }

double t1, t2;
char t1_str[6], t2_str[6];
bool enable_cli = false;
unsigned int roll_cnt=0;
// char roller[] = { '|', '/', '-', '\\' };
char roller[] = { 238, 239 };  // cheap LCD d'not have some characters, so using that
bool wifi_is_ok = false;
IPAddress mqtt_host_ip(IPADDR_NONE);
IPAddress prev_mqtt_host_ip(IPADDR_NONE);
IPAddress syslog_host_ip;
unsigned int unsucessfull_attempt=0;
bool first_message_after_boot = true;

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
  PGM_P msg_sensor1 = PSTR("sensor 1 ");
  PGM_P msg_sensor2 = PSTR("sensor 2 ");
  PGM_P msg_fail = PSTR("fail");
  PGM_P msg_ok = PSTR("Ok!");
 
  Serial.begin(115200);
  delay(100);
  Serial.println(F("\r\nBooting..."));

  status=lcd.begin(LCD_COLS, LCD_ROWS);
  if(status){
    Serial.print(F("LCD initialise failed, status = "));
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
  if ( roll_cnt >= sizeof(roller) ) roll_cnt=0;
  if ( network_enable and wifi_is_ok ) {
    MDNS.update();
  }
}

void report(){
  char mqtt_serv[] = "mqtt";
  char mqtt_proto[] = "tcp";
  unsucessfull_attempt++;
  if (wifi_is_ok) {
    if ( mqtt_host_resolving == 1 ) {  
      // using a hostname as a MQTT server address
      client.setServer(mqtt_host, mqtt_port);
    } else {
      // looking for a MQTT server IP in mDNS 
      unsigned int rc = mdns_resolving(mqtt_serv,mqtt_proto,mqtt_host,&mqtt_host_ip,&mqtt_port);
      if ( rc != 1 ) {
        Serial.println(F("MQTT host not found"));
      }
      if (mqtt_host_ip == IPADDR_NONE){
        return;
      }
      if ( mqtt_host_ip != prev_mqtt_host_ip ) {
        client.setServer(mqtt_host_ip, mqtt_port);
        prev_mqtt_host_ip = mqtt_host_ip;
        Serial.print(F("MQTT host IP: "));
        Serial.println(mqtt_host_ip);
      }
    }
    if (!client.connect(dev_name, mqtt_user, mqtt_passw, NULL, 0, false, NULL, true)) {
      Serial.println(F("MQTT server not connected"));
      return;
    }
    if ( first_message_after_boot ) {
      if (!client.publish("system.events", "boot")) {
        Serial.println(F("Error publishing to system.events"));
        return;
      }
      first_message_after_boot = false;
    }
    dtostrf(t1,1,1,t1_str);
    if (!client.publish("t.aquarium.water", t1_str)) {
      Serial.println(F("Error publishing to t.aquarium.water"));
      return;
    }
    dtostrf(t2,1,1,t2_str);
    if (!client.publish("t.aquarium.room", t2_str)) {
      Serial.println(F("Error published to t.aquarium.room"));
      return;
    }
    client.disconnect();
    unsucessfull_attempt=0;
  }
  if ( unsucessfull_attempt > UNSUCCESSFUL_ATTEMPTS_COUNT ) {
    Serial.println(F("Too many unsucessfull attempts to publish, restart"));
    ESP.restart();
  }
}
