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
    memset(dev_name, 0, sizeof(dev_name));
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
  bool passed = false;
  String value = terminal->readParameter();
  if (value == NULL) {
    terminal->invalidParameter();
  } else {
    memset(ssid, 0, sizeof(ssid));
    value.toCharArray(ssid, sizeof(ssid)-1);
    passed = true;
  }
  terminal->println();
  if ( passed ) {
    terminal->println(PASSED, "SSID set to \"" + String(ssid) + "\"");
  } else {
    terminal->println(FAILED, "SSID not set");
  }
  terminal->prompt();
}

void set_passw (Terminal* terminal) {
  bool passed = false;
  String value = terminal->readParameter();
  if (value == NULL) {
    terminal->invalidParameter();
  } else {
    memset(passw, 0, sizeof(passw));
    value.toCharArray(passw, sizeof(passw)-1 );
    passed = true;
  }
  terminal->println();
  if ( passed ) {
    terminal->println(PASSED, "WiFi password set to \"" + String(passw) + "\"");
  } else {
    terminal->println(FAILED, "WiFi password not set");
  }
  terminal->prompt();
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

void set_host (Terminal* terminal) {
  bool passed = false;
  String value = terminal->readParameter();
  if (value == NULL) {
    terminal->invalidParameter();
  } else {
    memset(host, 0, sizeof(host));
    value.toCharArray(host, sizeof(host)-1 );
    passed = true;
  }
  terminal->println();
  if ( passed ) {
    terminal->println(PASSED, "Hostname set to \"" + String(host) + "\"");
  } else {
    terminal->println(FAILED, "Hostname not set");
  }
  terminal->prompt();
}

void set_ (Terminal* terminal) {
  bool passed = false;
  String value = terminal->readParameter();
  if (value == NULL) {
    terminal->invalidParameter();
  } else {
    memset(dev_name, 0, sizeof(dev_name));
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
    memset(dev_name, 0, sizeof(dev_name));
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
    memset(dev_name, 0, sizeof(dev_name));
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
    memset(dev_name, 0, sizeof(dev_name));
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
    memset(dev_name, 0, sizeof(dev_name));
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
    memset(dev_name, 0, sizeof(dev_name));
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
    memset(dev_name, 0, sizeof(dev_name));
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

