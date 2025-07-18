bool mqtt_setserver(){
  char mqtt_serv[] = "mqtt\0";
  char mqtt_proto[] = "tcp\0";
  unsigned int rc = 0;

  if ( mqtt_host_resolving == 1 ) {  
    // using a hostname as a MQTT server address
    client.setServer(mqtt_host, mqtt_port);
    return(true);
  } 
  // looking for a MQTT server IP in mDNS 
  rc = mdns_resolving(mqtt_serv,mqtt_proto,mqtt_host,&mqtt_host_ip,&mqtt_port);
  if ( rc != 1 ) {
    Serial.println(F("MQTT host not found"));
  }
  if (mqtt_host_ip == IPADDR_NONE){
    return(false);
  }
  if (mqtt_port == 0){
    return(false);
  }
  if ( mqtt_host_ip != prev_mqtt_host_ip or mqtt_port != prev_mqtt_port ) {
    client.setServer(mqtt_host_ip, mqtt_port);
    prev_mqtt_host_ip = mqtt_host_ip;
    prev_mqtt_port = mqtt_port;
    Serial.print(F("MQTT host "));
    Serial.print(mqtt_host_ip);
    Serial.print(":");
    Serial.println(mqtt_port);
  }
  return(true);
}

bool mqtt_publish(){
  char t1_str[6], t2_str[6];
  if (!client.connect(dev_name, mqtt_user, mqtt_passw, NULL, 0, false, NULL, true)) {
    Serial.println(F("MQTT server not connected"));
    return(false);
  }
  if ( first_message_after_boot ) {
    if (!client.publish(topic0, "aquarium thermometer booted")) {
      Serial.print(F("Error publishing to "));
      Serial.println(topic0);
        return(false);
    }
    first_message_after_boot = false;
  }
  dtostrf(t1,1,1,t1_str);
  if (!client.publish(topic1, t1_str)) {
    Serial.print(F("Error publishing to "));
    Serial.println(topic1);
    return(false);
  }
  dtostrf(t2,1,1,t2_str);
  if (!client.publish(topic2, t2_str)) {
    Serial.print(F("Error publishing to "));
    Serial.println(topic2);
    return(false);
  }
  client.disconnect();
  return(true);
}

