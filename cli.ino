void loop_cli_mode(){
  terminal.loop();
  delay(10);
}

void setup_cli_mode(){
  lcd.print(F(" Configuration"));
  terminal.setup();
  terminal.setPrompt(">");
  terminal.useBS(true);
  addStandardTerminalCommands();
  TERM_CMD->addCmd("name", "[word]", "Set device name", set_name);
  TERM_CMD->addCmd("network", "[0/1]", "Network enable(1)/disable(0)", set_network_mode);
  TERM_CMD->addCmd("ssid", "[word]", "Set WiFi SSID", set_ssid);
  TERM_CMD->addCmd("passw", "[word]", "Set WiFi password", set_passw);
  TERM_CMD->addCmd("dns", "[0/1]", "Resolving mode: 0 - mDNS, 1 - DNS", set_dns);
  TERM_CMD->addCmd("host", "[hostname or IP]", "Set address of a MQTT broker", set_mqtt_host);
  TERM_CMD->addCmd("port", "[number]", "Set port of a MQTT broker", set_mqtt_port);
  TERM_CMD->addCmd("muser", "[word]", "Set MQTT user", set_mqtt_user);
  TERM_CMD->addCmd("mpassw", "[word]", "Set MQTT pasword", set_mqtt_passw);
  TERM_CMD->addCmd("show", "", "Show configuration", show_conf);
  TERM_CMD->addCmd("save", "", "Save configuration to EEPROM", save_conf);
  TERM_CMD->addCmd("reboot", "", "Reboot", reboot);
  // TERM_CMD->addCmd("", "[]", "", );
  terminal.prompt();
}

void set_string_parameter(Terminal* terminal, char* param, unsigned int param_size, String param_name){
  bool passed = false;
  String value = terminal->readParameter();
  if (value == NULL) {
    terminal->invalidParameter();
  } else {
    memset(param, 0, param_size);
    value.toCharArray(param, param_size-1 );
    passed = true;
  }
  terminal->println();
  if ( passed ) {
    terminal->println(PASSED, param_name + " set to \"" + String(param) + "\"");
  } else {
    terminal->println(FAILED, param_name + " not set");
  }
  terminal->prompt();
}
  

void set_name(Terminal* terminal) {
  set_string_parameter( terminal, dev_name, sizeof(dev_name), "Device name");
}

void set_network_mode (Terminal* terminal) {
  bool passed = false;
  int i;
  String value = terminal->readParameter();
  if (value == NULL) {
    terminal->invalidParameter();
  } else {
    i = value.toInt();
    if ( i == 0 or i == 1) {
      network_enable = (bool)i; 
      passed = true;
    } else {
      terminal->println(ERROR, "Parameter " + value + " is not between 0 or 1!");
    }
  }
  terminal->println();
  if ( passed ) {
    if (network_enable) {
      terminal->println(PASSED, "Network mode enabled");
    } else {
      terminal->println(PASSED, "Network mode disabled");
    }
  } else {
    terminal->println(FAILED, "Network mode unchanged");
  }
  terminal->prompt();
}

void set_ssid (Terminal* terminal) {
  set_string_parameter( terminal, ssid, sizeof(ssid), "SSID");
}

void set_passw (Terminal* terminal) {
  set_string_parameter( terminal, passw, sizeof(passw), "WiFi password");
}

void set_dns (Terminal* terminal) {
  bool passed = false;
  int i;
  String value = terminal->readParameter();
  if (value == NULL) {
    terminal->invalidParameter();
  } else {
    i = value.toInt();
    if ( i == 0 or i == 1) {
      mqtt_host_resolving = i;
      passed = true;
    } else {
      terminal->println(ERROR, "Parameter " + value + " is not between 0 or 1!");
    }
  }
  terminal->println();
  if ( passed ) {
    if (mqtt_host_resolving == 0) {
      terminal->println(PASSED, "Resolving mode set to mDNS");
    } else {
      terminal->println(PASSED, "Resolving mode set to DNS");
    }
  } else {
    terminal->println(FAILED, "Resolving mode unchanged");
  }
  terminal->prompt();
}

void set_mqtt_host (Terminal* terminal) {
  set_string_parameter( terminal, mqtt_host, sizeof(mqtt_host), "Hostname");
}

void set_mqtt_port (Terminal* terminal) {
  bool passed = false;
  String value = terminal->readParameter();
  if (value == NULL) {
    terminal->invalidParameter();
  } else {
    mqtt_port = value.toInt();
    if ( mqtt_port > 0 and mqtt_port < 0xffff) {
      passed = true;
    } else {
      terminal->println(ERROR, "Parameter " + value + " is not between 0 and 0xffff!");
    }
  }
  terminal->println();
  if ( passed ) {
    terminal->println(PASSED, "MQTT broker port set to \"" + String(mqtt_port) + "\"");
  } else {
    terminal->println(FAILED, "MQTT broker port not set");
  }
  terminal->prompt();
}

void set_mqtt_user (Terminal* terminal) {
  set_string_parameter( terminal, mqtt_user, sizeof(mqtt_user), "MQTT username");
}

void set_mqtt_passw (Terminal* terminal) {
  set_string_parameter( terminal, mqtt_passw, sizeof(mqtt_passw), "MQTT passwoord");
}

void show_conf (Terminal* terminal) {
  if ( network_enable )
    terminal->println(INFO,"Network enabled");
  else
    terminal->println(INFO,"Network disabled");
  terminal->println(INFO,"Device name = \"" + String(dev_name) + "\"");
  terminal->println(INFO,"WiFi SSID = \"" + String(ssid) + "\"");
  terminal->println(INFO,"WiFi password = \"" + String(passw) + "\"");
  if ( mqtt_host_resolving == 0 ) {
    terminal->println(INFO,"Resolving mode: mDNS");
  } else {
    terminal->println(INFO,"Resolving mode: DNS");
  }
  terminal->println(INFO,"MQTT broker host = \"" + String(mqtt_host) + "\"");
  terminal->println(INFO,"MQTT broker port = \"" + String(mqtt_port) + "\"");
  terminal->println(INFO,"MQTT username = \"" + String(mqtt_user) + "\"");
  terminal->println(INFO,"MQTT password = \"" + String(mqtt_passw) + "\"");
  terminal->prompt();
}

void save_conf (Terminal* terminal) {
  eeprom_save();
  terminal->println(PASSED,"Configuration saved to EEPROM");
  terminal->prompt();
}

void reboot (Terminal* terminal) {
  Serial.println("Reboot...");
  delay(3000);
  ESP.restart();
}
