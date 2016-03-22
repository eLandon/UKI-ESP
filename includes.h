

#include <FS.h>                   //this needs to be first, or it all crashes and burns...

#include <ESP8266WiFi.h>
//#include <WiFiClient.h>
//#include <ESP8266WebServer.h>
//web config portal
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>

#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson

#include <Ticker.h>
#include <WiFiUdp.h>

//OTA includes
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>


//ESP8266WebServer server(80); 

WiFiUDP UKI_UDP;


#include "leds.h"     //config and functions relative to leds

#include "ota.h"      //config and functions relative to ota firmware updates
#include "wifimgr.h"   //config and functions relative to wifi and access point configuration and configuration permanent saving


#include "udp.h"  //some helpers functions







