void wifi_init(){
  Serial.print("Connecting to "); Serial.print(ssid); Serial.println(" ...");

  if ( WiFi.getMode() != WIFI_STA )
    WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, passw);             // Connect to the network
}

void on_wifi_connect(const WiFiEventStationModeConnected& event) {
  Serial.print("Connected to Wi-Fi sucessfully.");
  Serial.print("RSSI: ");
  Serial.println(WiFi.RSSI());
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
}

void on_wifi_got_IP(const WiFiEventStationModeGotIP& event) {
  Serial.print("Obtained IP address: ");
  Serial.println(WiFi.localIP());
  wifi_is_ok = true;
   // Start mDNS
  if (!MDNS.begin(dev_name)) {             
    Serial.println("Error starting mDNS");
  }else{
    Serial.println("mDNS started");
    MDNS.addService("esp", "tcp", 8080);  // Announce esp tcp service on port 8080
  }
}

void on_wifi_disconnect(const WiFiEventStationModeDisconnected& event) {
  Serial.print("Disconnected from Wi-Fi, reason: ");
  Serial.println(event.reason);
  wifi_is_ok = false;
  WiFi.disconnect();
  wifi_init();
}
