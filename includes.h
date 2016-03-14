


#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Ticker.h>
#include <EEPROM.h>
#include <WiFiUdp.h>
//OTA includes
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>


ESP8266WebServer server(80); 

WiFiUDP UKI_UDP;



#include "helpers.h"

#include "leds.h"
#include "eeprom.h"
#include "global.h"
#include "web.h"




#include "ota.h"












