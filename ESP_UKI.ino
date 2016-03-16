/*
    ESP_UKI

    largely base on http://www.john-lassen.de/index.php/projects/esp-8266-arduino-ide-webconfig

  TODO :  better function organizing
          add firmware number in webserver
          rework led system with tickers
          send ADC to default IP via udp, allow configuration

         build onDemand config mode
         load/save parameters (fixed ip, uki_name, udp_port udp_ip
*/


  /* LEDS  */



#include "includes.h"  //headers and variables declaration



Ticker tkOTA;  // periodic check if OTA available
Ticker tkUKI;  // periodic send ADC to UDP



void setup ( void ) {
  
  EEPROM.begin(512);
  Serial.begin(115200);
  Serial.println("Starting ESP8266");
  setupLeds();
  //setupWifi();
  //setupOTA();
  
  delay(200);
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //UKI sensor setup
  UKI_UDP.begin(UKI_UDP_In_Port); 
//  delay(1000);
//  digitalWrite(Red_Led, HIGH); //red led off
//  digitalWrite(Blue_Led, HIGH);
//  delay(1000);
//  ledBlink(Red_Led, 3, 100); //3 quick blink on red led as we start 
//  delay (1000);
  blueLedState(-1, 500);
  
  
}


void loop ( void ) {
  loop_counter += 1;
  //loopWebserver();
  
  //loopOTA();

  /*  UKI part	*/
//  GSR_sensor = analogRead(A0);
//  //UKI_UDP.beginPacketMulticast((224, 1, 2, 3), 8000, WiFi.localIP());//
//  UKI_UDP.beginPacket(UKI_UDP_Master_IP, 8000);
//  UKI_UDP.print(config.DeviceName);
//  UKI_UDP.print(" ");
//  UKI_UDP.print(GSR_sensor);
//  UKI_UDP.endPacket();
//  //yield();
//  
//  delay(20);
//  
//  //Check udp in
//  int packetSize = UKI_UDP.parsePacket();
//  
//  if(packetSize) {
//    UKI_UDP_Master_IP = UKI_UDP.remoteIP();
//    UKI_UDP.beginPacket(UKI_UDP_Master_IP, 8000);
//    UKI_UDP.print("new master ip");
//    UKI_UDP.endPacket();
//  }



}

