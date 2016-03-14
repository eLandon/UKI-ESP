

void ledBlink (int Led, int blink_qty, int blink_time) {
  for (int i = 0 ; i < blink_qty ; i++) {
    digitalWrite(Led, LOW) ;
    delay(blink_time);
    digitalWrite(Led, HIGH);
    delay(blink_time);
  }
}
  
  void startESP() {
  EEPROM.begin(512);
  Serial.begin(115200);
  pinMode(Blue_Led, OUTPUT);
  digitalWrite(Blue_Led, HIGH);
  pinMode(Red_Led, OUTPUT);
  digitalWrite(Red_Led, LOW);//red led on
  delay(500);
  Serial.println("Starting ES8266");
}


void setupWifi(){
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

void setupWebserver(){
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

  server.onNotFound ( []() {
    Serial.println("Page Not Found");
    server.send ( 400, "text/html", "Page not Found" );
    ledBlink(Blue_Led, 10, 100);
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


void setupOTA(){
  ArduinoOTA.onStart([]() {
    Serial.println("Start");
    digitalWrite(Red_Led, HIGH);
    digitalWrite(Blue_Led, LOW);
    delay(1000);
    digitalWrite(Blue_Led, HIGH);
    delay(500);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("End");
    digitalWrite(Blue_Led, HIGH);
    delay(1000);
    ledBlink(Blue_Led, 3, 100);

  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\n", (progress / (total / 100)));
    digitalWrite(Blue_Led, Blue_Led_State);
    Blue_Led_State = !Blue_Led_State;

  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
    digitalWrite(Blue_Led, HIGH);
  });
  ArduinoOTA.begin();
}

void loopHandles(){
  if (loop_counter == 10)  {
    loop_counter = 0;
    server.handleClient();
    Red_Led_State = !Red_Led_State;
    digitalWrite(Red_Led, Red_Led_State);
    delay(10);
    }
  if (loop_counter == 5)  {
    ArduinoOTA.handle();
    delay(10);
    }
  loop_counter += 1;
  if (Refresh)  {
    Refresh = false;
    ///Serial.println("Refreshing...");
    //Serial.printf("FreeMem:%d %d:%d:%d %d.%d.%d \n",ESP.getFreeHeap() , DateTime.hour,DateTime.minute, DateTime.second, DateTime.year, DateTime.month, DateTime.day);
  }

}



