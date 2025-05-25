bool mqtt_setserver(){
  if ( mqtt_host_resolving == 1 ) {  
    // using a hostname as a MQTT server address
    client.setServer(mqtt_host, mqtt_port);
    return(true);
  } 
  // looking for a MQTT server IP in mDNS 
  unsigned int rc = mdns_resolving(mqtt_serv,mqtt_proto,mqtt_host,&mqtt_host_ip,&mqtt_port);
  if ( rc != 1 ) {
    Serial.println(F("MQTT host not found"));
  }
  if (mqtt_host_ip == IPADDR_NONE){
    return(false);
  }
  if ( mqtt_host_ip != prev_mqtt_host_ip ) {
    client.setServer(mqtt_host_ip, mqtt_port);
    prev_mqtt_host_ip = mqtt_host_ip;
    Serial.print(F("MQTT host IP: "));
    Serial.println(mqtt_host_ip);
  }
  return(true);
}

bool mqtt_publish(){
  if (!client.connect(dev_name, mqtt_user, mqtt_passw, NULL, 0, false, NULL, true)) {
    Serial.println(F("MQTT server not connected"));
    return(false);
  }
  if ( first_message_after_boot ) {
    if (!client.publish("system.events", "boot")) {
      Serial.println(F("Error publishing to system.events"));
        return(false);
    }
    first_message_after_boot = false;
  }
  dtostrf(t1,1,1,t1_str);
  if (!client.publish("t.aquarium.water", t1_str)) {
    Serial.println(F("Error publishing to t.aquarium.water"));
    return(false);
  }
  dtostrf(t2,1,1,t2_str);
  if (!client.publish("t.aquarium.room", t2_str)) {
    Serial.println(F("Error published to t.aquarium.room"));
    return(false);
  }
  client.disconnect();
  return(true);
}

