               // The Webserver

/* NTP things */
boolean firstStart = true;                    // On firststart = true, NTP will try to get a valid time   
strDateTime DateTime;                     // Global DateTime structure, will be refreshed every Second
WiFiUDP UDPNTPClient;                     // NTP Client
unsigned long UnixTimestamp = 0;                // GLOBALTIME  ( Will be set by NTP)
boolean Refresh = false; // For Main Loop, to refresh things like GPIO / WS2812
int cNTP_Update = 0;                      // Counter for Updating the time via NTP
Ticker tkSecond;                        // Second - Timer for Updating Datetime Structure
byte Minute_Old = 100;        // Helpvariable for checking, when a new Minute comes up (for Auto Turn On / Off)



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



/*
**
**  NTP 
**
*/

const int NTP_PACKET_SIZE = 48; 
byte packetBuffer[ NTP_PACKET_SIZE]; 
void NTPRefresh()
{

  


  if (WiFi.status() == WL_CONNECTED)
  {
    IPAddress timeServerIP; 
    WiFi.hostByName(config.ntpServerName.c_str(), timeServerIP); 
    //sendNTPpacket(timeServerIP); // send an NTP packet to a time server


    Serial.println("sending NTP packet...");
    memset(packetBuffer, 0, NTP_PACKET_SIZE);
    packetBuffer[0] = 0b11100011;   // LI, Version, Mode
    packetBuffer[1] = 0;     // Stratum, or type of clock
    packetBuffer[2] = 6;     // Polling Interval
    packetBuffer[3] = 0xEC;  // Peer Clock Precision
    packetBuffer[12]  = 49;
    packetBuffer[13]  = 0x4E;
    packetBuffer[14]  = 49;
    packetBuffer[15]  = 52;
    UDPNTPClient.beginPacket(timeServerIP, 123); 
    UDPNTPClient.write(packetBuffer, NTP_PACKET_SIZE);
    UDPNTPClient.endPacket();


    delay(1000);
  
    int cb = UDPNTPClient.parsePacket();
    if (!cb) {
      Serial.println("NTP no packet yet");
    }
    else 
    {
      Serial.print("NTP packet received, length=");
      Serial.println(cb);
      UDPNTPClient.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
      unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
      unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
      unsigned long secsSince1900 = highWord << 16 | lowWord;
      const unsigned long seventyYears = 2208988800UL;
      unsigned long epoch = secsSince1900 - seventyYears;
      UnixTimestamp = epoch;
    }
  }
}

void Second_Tick()
{
  strDateTime tempDateTime;
  AdminTimeOutCounter++;
  cNTP_Update++;
  UnixTimestamp++;
  ConvertUnixTimeStamp(UnixTimestamp +  (config.timezone *  360) , &tempDateTime);
  if (config.daylight) // Sommerzeit beachten
    if (summertime(tempDateTime.year,tempDateTime.month,tempDateTime.day,tempDateTime.hour,0))
    {
      ConvertUnixTimeStamp(UnixTimestamp +  (config.timezone *  360) + 3600, &DateTime);
    }
    else
    {
      DateTime = tempDateTime;
    }
  else
  {
      DateTime = tempDateTime;
  }
  Refresh = true;
}





void setupWebserver(){
  server.on ( "/", []() {Serial.println("admin.html"); server.send ( 200, "text/html", web_AdminMainPage ); }  );
  server.on ( "/admin.html", []() {Serial.println("admin.html"); server.send ( 200, "text/html", web_AdminMainPage ); }  );
  server.on ( "/config.html", send_network_configuration_html );
  server.on ( "/info.html", []() { Serial.println("info.html"); server.send ( 200, "text/html", web_Information ); }  );
  server.on ( "/ntp.html", send_NTP_configuration_html  );
  server.on ( "/general.html",  send_general_html );
  server.on ( "/style.css", []() { Serial.println("style.css"); server.send ( 200, "text/plain", web_Style_css );} );
  server.on ( "/microajax.js", []() { Serial.println("microajax.js"); server.send ( 200, "text/plain", web_microajax_js ); } );
  server.on ( "/admin/values", send_network_configuration_values_html );
  server.on ( "/admin/connectionstate", send_connection_state_values_html );
  server.on ( "/admin/infovalues", send_information_values_html );
  server.on ( "/admin/ntpvalues", send_NTP_configuration_values_html );
  server.on ( "/admin/generalvalues", send_general_configuration_values_html);
  server.on ( "/admin/devicename",     send_devicename_value_html);
  /* old led management
  server.on ( "/", []() {digitalWrite(Blue_Led, LOW); Serial.println("admin.html"); server.send ( 200, "text/html", web_AdminMainPage ); digitalWrite(Blue_Led, HIGH);  }  );
  server.on ( "/admin.html", []() {digitalWrite(Blue_Led, LOW); Serial.println("admin.html"); server.send ( 200, "text/html", web_AdminMainPage ); digitalWrite(Blue_Led, HIGH);  }  );
  server.on ( "/config.html", send_network_configuration_html );
  server.on ( "/info.html", []() { digitalWrite(Blue_Led, LOW) ; Serial.println("info.html"); server.send ( 200, "text/html", web_Information ); digitalWrite(Blue_Led, HIGH) ; }  );
  server.on ( "/ntp.html", send_NTP_configuration_html  );
  server.on ( "/general.html",  send_general_html );
  server.on ( "/style.css", []() { digitalWrite(Blue_Led, LOW) ; Serial.println("style.css"); server.send ( 200, "text/plain", web_Style_css ); digitalWrite(Blue_Led, HIGH) ; } );
  server.on ( "/microajax.js", []() { digitalWrite(Blue_Led, LOW) ; Serial.println("microajax.js"); server.send ( 200, "text/plain", web_microajax_js ); digitalWrite(Blue_Led, HIGH); } );
  server.on ( "/admin/values", send_network_configuration_values_html );
  server.on ( "/admin/connectionstate", send_connection_state_values_html );
  server.on ( "/admin/infovalues", send_information_values_html );
  server.on ( "/admin/ntpvalues", send_NTP_configuration_values_html );
  server.on ( "/admin/generalvalues", send_general_configuration_values_html);
  server.on ( "/admin/devicename",     send_devicename_value_html);
  */

  server.onNotFound ( []() {
    Serial.println("Page Not Found");
    server.send ( 400, "text/html", "Page not Found" );
//    ledBlink(Blue_Led, 10, 100);
  }  );
  server.begin();
  Serial.println( "HTTP server started" );
  tkSecond.attach(1, Second_Tick);
  UDPNTPClient.begin(2390);  // Port for NTP receive
}



void loopWebserver(){
  if (AdminEnabled)  {
    if (AdminTimeOutCounter > AdminTimeOut)   {
      AdminEnabled = false;
      Serial.println("Admin Mode disabled!");
      WiFi.mode(WIFI_STA);
    }
  }
  if (config.Update_Time_Via_NTP_Every  > 0 )  {
    if (cNTP_Update > 5 && firstStart)    {
      NTPRefresh();
      cNTP_Update = 0;
      firstStart = false;
    }
    else if ( cNTP_Update > (config.Update_Time_Via_NTP_Every * 60) )    {
      NTPRefresh();
      cNTP_Update = 0;
    }
  }

  if (DateTime.minute != Minute_Old)  {
    Minute_Old = DateTime.minute;
    if (config.AutoTurnOn)
      if (DateTime.hour == config.TurnOnHour && DateTime.minute == config.TurnOnMinute)      {
        Serial.println("SwitchON");
      }
    }

    Minute_Old = DateTime.minute;
    if (config.AutoTurnOff)    {
      if (DateTime.hour == config.TurnOffHour && DateTime.minute == config.TurnOffMinute)      {
        Serial.println("SwitchOff");
      }
    }
  }


 

