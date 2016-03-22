/*
    ESP_UKI

  TODO :  better function organizing
          add firmware number in webserver
          send ADC to default IP via udp, allow configuration

         build onDemand config mode
         load/save parameters (fixed ip, uki_name, udp_port udp_ip
*/


#include "includes.h"  //headers and variables declaration





void setup ( void ) {
  
  
  Serial.begin(115200);
  Serial.println("Starting ESP8266");

  setupLeds();
  
  setupWifi(); 

  setupUDP() ;
  
  //setupOTA();
 
  blueLedState(-1, 500);
  
  
}


void loop ( void ) {
  //Serial.println(WiFi.status());
  StartConfigAP();
  UDP_send_receive();

  
}




