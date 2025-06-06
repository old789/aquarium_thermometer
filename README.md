#An aquarium thermometer with remote monitoring

Built using an ESP8266 and two DS18B20 probes. There is a 1602 LCD display for local monitoring. Data is transmitted using MQTT.

### Command line
To enter Commadline mode, you need to press the "space" button in a terminal after booting, while on the LCD screen the message 'Wait for press...' appears.  Communication parameters for terminal: 115200,8N1.

| Command *arg* | Explanation |
| --- | --- |
| network *digit* | Set standalone(0) or network (1) mode. In standalone mode network disabled, data don't send out |
| name *word* | Set device name, used as a device identifier on the remote server |
| ssid *word* | Set WiFi SSID |
| passw *word* | Set WiFi password |
| dns *digit* | Resolving mode: 0 - mDNS, 1 - DNS |
| host *word* | Set destination host ( hostname (mDNS or DNS) or IPv4 (DNS only) ) |
| port *number* | Set destination port |
| muser *word* | Set MQTT username |
| mpassw *word* | Set MQTT password |
| t1corr *0/1* | Correction of 1st sensor enable(1)/disable(0) |
| t1high *float* | Set high value for correction of 1st sensor |
| t1low *float* | Set low value for correction of 1st sensor |
| t2corr *0/1* | Correction of 2nd sensor enable(1)/disable(0) |
| t2high *float* | Set high value for correction of 2nd sensor |
| t2low *float* | Set low value for correction of 2nd sensor |
| show | Show current configuration |
| save | Save configuration to EEPROM |
| reboot | Reboot |
| help | Get help |

