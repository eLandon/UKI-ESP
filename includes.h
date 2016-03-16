


#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include <Ticker.h>
#include <EEPROM.h>
#include <WiFiUdp.h>

//OTA includes
#include <ESP8266mDNS.h>
//#include <WiFiUdp.h>
#include <ArduinoOTA.h>


ESP8266WebServer server(80); 

WiFiUDP UKI_UDP;



#include "helpers.h"  //some helpers functions
#include "leds.h"     //config and functions relative to leds
#include "eeprom.h"   //config and functions relative to config permanent storage
#include "global.h"   //config and functions relative to wifi and access point configuration
//#include "web.h"      //config and functions relative to configuration web server
#include "ota.h"      //config and functions relative to ota firmware updates












