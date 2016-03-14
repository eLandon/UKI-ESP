/*
    ESP_UKI

  TODO :  clean webserver
          add uki configuration/information page (player number, ADC value, IP of main computer)
          send ADC to default IP via udp, allow configuration
          ajout numéro de firmware sur webserver
*/
	

#include "includes.h"  //headers and variables declaration

/* UKI udp configuration */

int UKI_UDP_In_Port = 9000;  //udp port input for ESP
IPAddress UKI_UDP_Master_IP(192, 168, 0, 41);  //default udp address to send to. Will automatically change to the ip sending something to udp in
 

void setup ( void ) {
  startESP();
  setupWifi();
  setupWebserver();
  setupOTA();
  
  delay(200);
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //UKI sensor setup
  UKI_UDP.begin(UKI_UDP_In_Port); 
  delay(1000);
  digitalWrite(Red_Led, HIGH); //red led off
  digitalWrite(Blue_Led, HIGH);
  delay(1000);
  ledBlink(Red_Led, 3, 100); //3 quick blink on red led as we start 
  delay (1000);
}


void loop ( void ) {
  
  loopWebserver();
  
  loopHandles();

  /*  UKI part	*/
  GSR_sensor = analogRead(A0);
  //UKI_UDP.beginPacketMulticast((224, 1, 2, 3), 8000, WiFi.localIP());//
  UKI_UDP.beginPacket(UKI_UDP_Master_IP, 8000);
  UKI_UDP.print(config.DeviceName);
  UKI_UDP.print(" ");
  UKI_UDP.print(GSR_sensor);
  UKI_UDP.endPacket();
  //yield();
//Red_Led_State = !Red_Led_State;
  
  //analogWrite(Red_Led, GSR_sensor);

  
  delay(20);
  

  //Check udp in
  int packetSize = UKI_UDP.parsePacket();
  
  if(packetSize) {
    UKI_UDP_Master_IP = UKI_UDP.remoteIP();
    UKI_UDP.beginPacket(UKI_UDP_Master_IP, 8000);
    UKI_UDP.print("new master ip");
    UKI_UDP.endPacket();
  }









}

