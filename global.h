#ifndef GLOBAL_H
#define GLOBAL_H



/* DEFAULT ACCESS POINT CONFIGURATION */
#define ACCESS_POINT_NAME  "ESP"
#define ACCESS_POINT_PASSWORD  "12345678"
boolean AdminEnabled = false; // Enable Admin Mode for a given Time
int AdminTimeOutCounter = 0;  // Counter for Disabling the AdminMode
#define AdminTimeOut 60  // Defines the Time in Seconds, when the Admin-Mode will be disabled



/* UDP CONFIGURATION */
int UKI_UDP_In_Port = 9000;  //udp port input for ESP
IPAddress UKI_UDP_Master_IP(192, 168, 0, 41);  //default udp address to send to. Will automatically change to the ip sending something to udp in
 


int GSR_sensor;
int loop_counter;
int counter;


void setupWifi() {
  
  //if no config in eeprom, start as default access point
  if (!ReadConfig())  {
    WiFi.mode(WIFI_AP);
    WiFi.softAP( ACCESS_POINT_NAME , ACCESS_POINT_PASSWORD);
    Serial.println("Starting default access point (SSID:ESP, pwd:12345678)");
    Serial.println(WiFi.softAPIP());
    }
  //if config in eeprom, try and connect to wifi, if fail after timeout start access point with config.name SSID
  else {  
    WiFi.mode(WIFI_STA);
   // ConfigureWifi();
    Serial.println("Connecting to configured wifi network");
    while (WiFi.status() != 3) {
      counter++;
      Serial.print(".");
      Serial.println(WiFi.status());

      if (counter > 50) {
        Serial.println("failed");
        Serial.println("Starting as access point, SSID:"+ (String)config.DeviceName.c_str()+" pwd:" + (String)ACCESS_POINT_PASSWORD);
        WiFi.mode(WIFI_AP);
        //WiFi.softAP( ACCESS_POINT_NAME , ACCESS_POINT_PASSWORD);
        WiFi.softAP( config.DeviceName.c_str() , ACCESS_POINT_PASSWORD);
        Serial.println(WiFi.softAPIP());
        break;
        }
      }
    } 
 
}



#endif
