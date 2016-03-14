
int GSR_sensor;


const int Blue_Led = 2;
bool Blue_Led_State ;

const int Red_Led = 0;
bool Red_Led_State ;

int loop_counter;

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Ticker.h>
#include <EEPROM.h>
#include <WiFiUdp.h>
#include "helpers.h"
#include "global.h"

//OTA includes
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

WiFiUDP UKI_UDP;
/*
  Include the HTML, STYLE and Script "Pages"
*/
#include "web_Root.h"
#include "web_Admin.h"
#include "web_Script.js.h"
#include "web_Style.css.h"
#include "web_NTPSettings.h"
#include "web_Information.h"
#include "web_General.h"
#include "web_NetworkConfiguration.h"

#include "functions.h"






