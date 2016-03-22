/*
    ESP_UKI

  TODO :  better function organizing
          add firmware number in webserver
          send ADC to default IP via udp, allow configuration

         build onDemand config mode
         load/save parameters (fixed ip, uki_name, udp_port udp_ip
*/


#include "includes.h"  //headers and variables declaration


/* UDP CONFIGURATION */
int UKI_UDP_In_Port = 9000;  //udp port input for ESP
String UKI_UDP_IP_test = "192.168.1.1";
IPAddress UKI_UDP_Master_IP(192, 168, 10, 100);  //default udp address to send to. Will automatically change to the ip sending something to udp in
Ticker tkUKI;  // periodic send ADC to UDP
int GSR_sensor;


void setup ( void ) {
  
  Serial.begin(115200);
  Serial.println("Starting ESP8266");

  setupLeds();
   
  setupWifi();  
  
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
  //Serial.println(WiFi.status());
  StartConfigAP();
  

  /*  UKI part	*/
  GSR_sensor = analogRead(A0);
  //UKI_UDP.beginPacketMulticast((224, 1, 2, 3), 8000, WiFi.localIP());//
  UKI_UDP.beginPacket(UKI_UDP_IP_test, 8000);
  UKI_UDP.print("UKI");
  UKI_UDP.print(" ");
  UKI_UDP.print(GSR_sensor);
  UKI_UDP.endPacket();
  yield();
  
  delay(50);
  
  //Check udp in
  int packetSize = UKI_UDP.parsePacket();
  
  if(packetSize) {
    UKI_UDP_Master_IP = UKI_UDP.remoteIP();
    UKI_UDP.beginPacket(UKI_UDP_Master_IP, 8000);
    UKI_UDP.print("new master ip");
    UKI_UDP.endPacket();
  }



}

