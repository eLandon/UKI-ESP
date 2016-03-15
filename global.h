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



void ConfigureWifi()
{
  Serial.println("Configuring Wifi");
  WiFi.begin (config.ssid.c_str(), config.password.c_str());
  if (!config.dhcp)
  {
    WiFi.config(IPAddress(config.IP[0],config.IP[1],config.IP[2],config.IP[3] ),  IPAddress(config.Gateway[0],config.Gateway[1],config.Gateway[2],config.Gateway[3] ) , IPAddress(config.Netmask[0],config.Netmask[1],config.Netmask[2],config.Netmask[3] ));
  }
}

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
    ConfigureWifi();
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


void setupWifi_old(){
  if (!ReadConfig())  {
    // DEFAULT CONFIG 
    config.ssid = "Freebox-6F7B3C";
    config.password = "accessorem6-gignendi7-insultare!";
    config.dhcp = true;
    config.IP[0] = 192; config.IP[1] = 168; config.IP[2] = 0; config.IP[3] = 100;
    config.Netmask[0] = 255; config.Netmask[1] = 255; config.Netmask[2] = 255; config.Netmask[3] = 0;
    config.Gateway[0] = 192; config.Gateway[1] = 168; config.Gateway[2] = 0; config.Gateway[3] = 254;
    config.ntpServerName = "0.de.pool.ntp.org";
    config.Update_Time_Via_NTP_Every =  0;
    config.timezone = -10;
    config.daylight = true;
    config.DeviceName = "UKI_ESP_default";
    config.AutoTurnOff = false;
    config.AutoTurnOn = false;
    config.TurnOffHour = 0;
    config.TurnOffMinute = 0;
    config.TurnOnHour = 0;
    config.TurnOnMinute = 0;
    WriteConfig();
    Serial.println("General config applied");
  }


  if (AdminEnabled)  {
    WiFi.mode(WIFI_AP_STA);
    //WiFi.softAP( ACCESS_POINT_NAME , ACCESS_POINT_PASSWORD);
    WiFi.softAP( config.DeviceName.c_str() , ACCESS_POINT_PASSWORD);
  }
  else  { WiFi.mode(WIFI_STA); }
  ConfigureWifi();
  while (WiFi.status() != 3) {
    Serial.println(WiFi.status());
  }
//    Serial.print(".");
//    digitalWrite(Blue_Led, Blue_Led_State);
//    Blue_Led_State = !Blue_Led_State;
//    yield();
//  }
  Serial.println(WiFi.status());

  /* A MODIFIER : pour le moment si AdminEnabled, lance pendant AdminTimeOut un AP en parallèle de la connection configurée
   *  devrait être remplacé par  - tente de se connecter au wifi configuré, si ok on continue normalement
   *                             - si n'arrive pas à se connecter au wifi configuré, passe en mode AP avec webserver pour reconfigurer le wifi / ou demarrer en mode AP si bouton utilisateur appuyé pendant démarragae
   *                             - normalement redémarre après reconfiguration, donc ok
   *                             - eventuellement ajouter dans loop un redemarrage en cas de perte de connection
   */

}




#endif
