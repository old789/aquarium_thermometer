void loop_cli_mode(){
  terminal.loop();
  delay(10);
}

void setup_cli_mode(){
  lcd.print(F(" Configuration"));
  terminal.setup();
  terminal.setPrompt(">");
  addStandardTerminalCommands();
  TERM_CMD->addCmd("name", "[word]", "Set device name", set_name);
  TERM_CMD->addCmd("network", "[0/1]", "Network enable(1)/disable(0)", set_network_mode);
  TERM_CMD->addCmd("ssid", "[word]", "Set WiFi SSID", set_ssid);
  TERM_CMD->addCmd("passw", "[word]", "Set WiFi password", set_passw);
  TERM_CMD->addCmd("dns", "[0/1]", "Resolving mode: 0 - mDNS, 1 - DNS", set_res);
  TERM_CMD->addCmd("host", "[hostname or IP]", "Set address of a MQTT broker", set_host);
  TERM_CMD->addCmd("port", "[number]", "Set port of a MQTT broker", set_port);
  TERM_CMD->addCmd("muser", "[word]", "Set MQTT user", set_mqtt_user);
  TERM_CMD->addCmd("mpassw", "[word]", "Set MQTT pasword", set_mqtt_passw);
  TERM_CMD->addCmd("show", "", "Show configuration", show_conf);
  TERM_CMD->addCmd("save", "", "Save configuration to EEPROM", save_conf);
  TERM_CMD->addCmd("reboot", "", "Reboot", reboot);
  // TERM_CMD->addCmd("", "[]", "", );
  terminal.prompt();
}

void set_name(Terminal* terminal) {
  bool passed = false;
  String value = terminal->readParameter();
  if (value == NULL) {
    terminal->invalidParameter();
  } else {
    memset(ups_name, 0, sizeof(ups_name));
    value.toCharArray(dev_name, sizeof(dev_name)-1 );
    passed = true;
  }
  //terminal->println();
  if ( passed ) {
    terminal->println(PASSED, "Device name set to \"" + String(dev_name) + "\"");
  } else {
    terminal->println(FAILED, "Device name not set");
  }
  terminal->prompt();
}

void set_network_mode (Terminal* terminal) {
  bool passed = false;
  String value = terminal->readParameter();
  if (value == NULL) {
    terminal->invalidParameter();
  } else {
    memset(ups_name, 0, sizeof(ups_name));
    value.toCharArray(dev_name, sizeof(dev_name)-1 );
    passed = true;
  }
  terminal->println();
  if ( passed ) {
    terminal->println(PASSED, "Device name set to \"" + String(dev_name) + "\"");
  } else {
    terminal->println(FAILED, "Device name not set");
  }
  terminal->prompt();
}

void set_ (Terminal* terminal) {
  bool passed = false;
  String value = terminal->readParameter();
  if (value == NULL) {
    terminal->invalidParameter();
  } else {
    memset(ups_name, 0, sizeof(ups_name));
    value.toCharArray(dev_name, sizeof(dev_name)-1 );
    passed = true;
  }
  terminal->println();
  if ( passed ) {
    terminal->println(PASSED, "Device name set to \"" + String(dev_name) + "\"");
  } else {
    terminal->println(FAILED, "Device name not set");
  }
  terminal->prompt();
}

void set_ (Terminal* terminal) {
  bool passed = false;
  String value = terminal->readParameter();
  if (value == NULL) {
    terminal->invalidParameter();
  } else {
    memset(ups_name, 0, sizeof(ups_name));
    value.toCharArray(dev_name, sizeof(dev_name)-1 );
    passed = true;
  }
  terminal->println();
  if ( passed ) {
    terminal->println(PASSED, "Device name set to \"" + String(dev_name) + "\"");
  } else {
    terminal->println(FAILED, "Device name not set");
  }
  terminal->prompt();
}

void set_ (Terminal* terminal) {
  bool passed = false;
  String value = terminal->readParameter();
  if (value == NULL) {
    terminal->invalidParameter();
  } else {
    memset(ups_name, 0, sizeof(ups_name));
    value.toCharArray(dev_name, sizeof(dev_name)-1 );
    passed = true;
  }
  terminal->println();
  if ( passed ) {
    terminal->println(PASSED, "Device name set to \"" + String(dev_name) + "\"");
  } else {
    terminal->println(FAILED, "Device name not set");
  }
  terminal->prompt();
}

void set_ (Terminal* terminal) {
  bool passed = false;
  String value = terminal->readParameter();
  if (value == NULL) {
    terminal->invalidParameter();
  } else {
    memset(ups_name, 0, sizeof(ups_name));
    value.toCharArray(dev_name, sizeof(dev_name)-1 );
    passed = true;
  }
  terminal->println();
  if ( passed ) {
    terminal->println(PASSED, "Device name set to \"" + String(dev_name) + "\"");
  } else {
    terminal->println(FAILED, "Device name not set");
  }
  terminal->prompt();
}

void set_ (Terminal* terminal) {
  bool passed = false;
  String value = terminal->readParameter();
  if (value == NULL) {
    terminal->invalidParameter();
  } else {
    memset(ups_name, 0, sizeof(ups_name));
    value.toCharArray(dev_name, sizeof(dev_name)-1 );
    passed = true;
  }
  terminal->println();
  if ( passed ) {
    terminal->println(PASSED, "Device name set to \"" + String(dev_name) + "\"");
  } else {
    terminal->println(FAILED, "Device name not set");
  }
  terminal->prompt();
}

void set_ (Terminal* terminal) {
  bool passed = false;
  String value = terminal->readParameter();
  if (value == NULL) {
    terminal->invalidParameter();
  } else {
    memset(ups_name, 0, sizeof(ups_name));
    value.toCharArray(dev_name, sizeof(dev_name)-1 );
    passed = true;
  }
  terminal->println();
  if ( passed ) {
    terminal->println(PASSED, "Device name set to \"" + String(dev_name) + "\"");
  } else {
    terminal->println(FAILED, "Device name not set");
  }
  terminal->prompt();
}

void set_ (Terminal* terminal) {
  bool passed = false;
  String value = terminal->readParameter();
  if (value == NULL) {
    terminal->invalidParameter();
  } else {
    memset(ups_name, 0, sizeof(ups_name));
    value.toCharArray(dev_name, sizeof(dev_name)-1 );
    passed = true;
  }
  terminal->println();
  if ( passed ) {
    terminal->println(PASSED, "Device name set to \"" + String(dev_name) + "\"");
  } else {
    terminal->println(FAILED, "Device name not set");
  }
  terminal->prompt();
}

void set_ (Terminal* terminal) {
  bool passed = false;
  String value = terminal->readParameter();
  if (value == NULL) {
    terminal->invalidParameter();
  } else {
    memset(ups_name, 0, sizeof(ups_name));
    value.toCharArray(dev_name, sizeof(dev_name)-1 );
    passed = true;
  }
  terminal->println();
  if ( passed ) {
    terminal->println(PASSED, "Device name set to \"" + String(dev_name) + "\"");
  } else {
    terminal->println(FAILED, "Device name not set");
  }
  terminal->prompt();
}

void set_ (Terminal* terminal) {
  bool passed = false;
  String value = terminal->readParameter();
  if (value == NULL) {
    terminal->invalidParameter();
  } else {
    memset(ups_name, 0, sizeof(ups_name));
    value.toCharArray(dev_name, sizeof(dev_name)-1 );
    passed = true;
  }
  terminal->println();
  if ( passed ) {
    terminal->println(PASSED, "Device name set to \"" + String(dev_name) + "\"");
  } else {
    terminal->println(FAILED, "Device name not set");
  }
  terminal->prompt();
}

void set_ (Terminal* terminal) {
  bool passed = false;
  String value = terminal->readParameter();
  if (value == NULL) {
    terminal->invalidParameter();
  } else {
    memset(ups_name, 0, sizeof(ups_name));
    value.toCharArray(dev_name, sizeof(dev_name)-1 );
    passed = true;
  }
  terminal->println();
  if ( passed ) {
    terminal->println(PASSED, "Device name set to \"" + String(dev_name) + "\"");
  } else {
    terminal->println(FAILED, "Device name not set");
  }
  terminal->prompt();
}

