
void eeprom_save(){
  EEPROM.put(0, mark);
  EEPROM.put(PT_NETWORK_ENA, network_enable);
  EEPROM.put(PT_DEV_NAME, dev_name);
  EEPROM.put(PT_SSID, ssid);
  EEPROM.put(PT_PASSW, passw);
  EEPROM.put(PT_DNS, mqtt_host_resolving);
  EEPROM.put(PT_HOST, mqtt_host);
  EEPROM.put(PT_PORT, mqtt_port);
  EEPROM.put(PT_MUSER, mqtt_user);
  EEPROM.put(PT_MPASSW, mqtt_passw);
  EEPROM.put(PT_T1_RAW_HIGH, t1_raw_high);
  EEPROM.put(PT_T1_RAW_LOW, t1_raw_low);
  EEPROM.put(PT_T2_RAW_HIGH, t2_raw_high);
  EEPROM.put(PT_T2_RAW_LOW, t2_raw_low);
  EEPROM.put(PT_CRC, ram_crc());
  EEPROM.commit();
}

unsigned long ram_crc() {
  unsigned long crc = ~0L;
  PGM_P msg = PSTR("Can't allocate memory");
  const unsigned long crc_table[16] = {
    0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
    0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
    0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
    0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
  };
  byte *buf = (byte*)malloc( SIZE_EEPROM + 8 );

  if ( ! buf ){
    Serial.println(FPSTR(msg));
    return(0);
  }

  memset(buf, 0, SIZE_EEPROM + 8);
  memcpy(buf, &mark, sizeof(mark));
  memcpy(buf+PT_NETWORK_ENA, &network_enable, sizeof(network_enable));
  memcpy(buf+PT_DEV_NAME, &dev_name, strlen(dev_name));
  memcpy(buf+PT_SSID, &ssid, strlen(ssid));
  memcpy(buf+PT_PASSW, &passw, strlen(passw));
  memcpy(buf+PT_HOST, &mqtt_host, strlen(mqtt_host));
  memcpy(buf+PT_PORT, &mqtt_port, sizeof(mqtt_port));
  memcpy(buf+PT_MUSER, &mqtt_user, strlen(mqtt_user));
  memcpy(buf+PT_MPASSW, &mqtt_passw, strlen(mqtt_passw));
  memcpy(buf+PT_T1_RAW_HIGH, &t1_raw_high, sizeof(t1_raw_high));
  memcpy(buf+PT_T1_RAW_LOW, &t1_raw_low, sizeof(t1_raw_low));
  memcpy(buf+PT_T2_RAW_HIGH, &t2_raw_high, sizeof(t2_raw_high));
  memcpy(buf+PT_T2_RAW_LOW, &t2_raw_low, sizeof(t2_raw_low));

  for (uint16_t index = 0 ; index <= SIZE_EEPROM  ; ++index) {
    crc = crc_table[(crc ^ buf[index]) & 0x0f] ^ (crc >> 4);
    crc = crc_table[(crc ^ (buf[index] >> 4)) & 0x0f] ^ (crc >> 4);
    crc = ~crc;
  }
  free(buf);
  return crc;
}

bool eeprom_read(){
uint16_t m = 0;
unsigned long crc = 0;
PGM_P msg1 = PSTR("EEPROM read ERROR: incorrect marker");
PGM_P msg2 = PSTR("EEPROM read ERROR: incorrect crc");

  EEPROM.get(0, m);
  if ( m != mark ) {
    Serial.println(FPSTR(msg1));
    return(false);
  }

  EEPROM.get(PT_NETWORK_ENA, network_enable);
  EEPROM.get(PT_DEV_NAME, dev_name);
  EEPROM.get(PT_SSID, ssid);
  EEPROM.get(PT_PASSW, passw);
  EEPROM.get(PT_DNS, mqtt_host_resolving);
  EEPROM.get(PT_HOST, mqtt_host);
  EEPROM.get(PT_PORT, mqtt_port);
  EEPROM.get(PT_MUSER, mqtt_user);
  EEPROM.get(PT_MPASSW, mqtt_passw);
  EEPROM.get(PT_T1_RAW_HIGH, t1_raw_high);
  EEPROM.get(PT_T1_RAW_LOW, t1_raw_low);
  EEPROM.get(PT_T2_RAW_HIGH, t2_raw_high);
  EEPROM.get(PT_T2_RAW_LOW, t2_raw_low);
  EEPROM.get(PT_CRC, crc);
 
  if ( crc != ram_crc() ){
    Serial.println(FPSTR(msg2));
    return(false);
  }
  return(true);
}

