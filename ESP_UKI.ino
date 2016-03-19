/*
    ESP_UKI

  TODO :  better function organizing
          add firmware number in webserver
          rework led system with tickers
          send ADC to default IP via udp, allow configuration

         build onDemand config mode
         load/save parameters (fixed ip, uki_name, udp_port udp_ip
*/



#include <FS.h>                   //this needs to be first, or it all crashes and burns...


char mqtt_server[40];
char mqtt_port[6] = "8080";
char blynk_token[34] = "YOUR_BLYNK_TOKEN";

bool shouldSaveConfig = false;

#include "includes.h"  //headers and variables declaration






/* UDP CONFIGURATION */
int UKI_UDP_In_Port = 9000;  //udp port input for ESP
IPAddress UKI_UDP_Master_IP(192, 168, 0, 41);  //default udp address to send to. Will automatically change to the ip sending something to udp in
Ticker tkUKI;  // periodic send ADC to UDP
int GSR_sensor;


void setup ( void ) {
  
  //EEPROM.begin(512);
  Serial.begin(115200);
  Serial.println("Starting ESP8266");

  //clean FS, for testing
  //SPIFFS.format();

  //read configuration from FS json
  Serial.println("mounting FS...");

  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
          Serial.println("\nparsed json");

          strcpy(mqtt_server, json["mqtt_server"]);
          strcpy(mqtt_port, json["mqtt_port"]);
          strcpy(blynk_token, json["blynk_token"]);

        } else {
          Serial.println("failed to load json config");
        }
      }
    }
  } else {
    Serial.println("failed to mount FS");
  }
  //end read
  
  setupWifi();

  
  
  
  setupLeds();
  
  setupOTA();
  
  delay(200);
  Serial.println("Ready");
  Serial.print("IP address: ");
  //Serial.println(WiFi.localIP());

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
  StartConfigAP();
   //save the custom parameters to FS
  if (shouldSaveConfig) {
    Serial.println("saving config");
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json["mqtt_server"] = mqtt_server;
    json["mqtt_port"] = mqtt_port;
    json["blynk_token"] = blynk_token;

    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      Serial.println("failed to open config file for writing");
    }

    json.printTo(Serial);
    json.printTo(configFile);
    configFile.close();
    //end save
  }
  

  /*  UKI part	*/
  GSR_sensor = analogRead(A0);
  //UKI_UDP.beginPacketMulticast((224, 1, 2, 3), 8000, WiFi.localIP());//
  UKI_UDP.beginPacket(UKI_UDP_Master_IP, 8000);
  UKI_UDP.print(config.DeviceName);
  UKI_UDP.print(" ");
  UKI_UDP.print(GSR_sensor);
  UKI_UDP.endPacket();
  yield();
  
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

