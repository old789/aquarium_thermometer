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

unsigned int mdns_resolving(char *service, char *proto, char *hostname, IPAddress *ip, uint16_t *port) {
  Serial.println("Sending mDNS query");
  int n = MDNS.queryService(service, proto);
  Serial.println("mDNS query done");
  if (n == 0) {
    Serial.println("no services found");
    return(0);
  } else {
    Serial.print(n);
    Serial.println(" service(s) found");
    for (int i = 0; i < n; ++i) {
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(MDNS.hostname(i));
      Serial.print(" (");
      Serial.print(MDNS.IP(i));
      Serial.print(":");
      Serial.print(MDNS.port(i));
      Serial.println(")");
      if ( strcmp(MDNS.hostname(i).c_str(), hostname) == 0 ) {
        *ip=MDNS.IP(i);
        *proto=MDNS.port(i);
        return(1);
      }
    }
  }
  return(0);
}
