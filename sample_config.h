//   Config begin

// Name of system for logging
char dev_name[33] = {0};

// Model of device
char dev_model[33] = {0};

// WiFi SSID
char ssid[33] = {0};

// WiFi password
char passw[65] = {0};

// Send data to MQTT or standalone mode ( false )
bool network_enable = true;

// resolving mode
unsigned int mqtt_host_resolving = 0; // 0 - mDNS, 1 - DNS

// hostname ( DNS or mDNS mode )or IP ( DNS only mode )of a MQTT server
char mqtt_host[33] = {0};
uint16_t mqtt_port = 1883;

// MQTT authentification parameters
char mqtt_user[33] = {0};
char mqtt_passw[33] = {0};

//   Config end
