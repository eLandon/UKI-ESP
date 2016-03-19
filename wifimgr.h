
//flag for saving data

#define TRIGGER_PIN 12
Ticker tkConfig ;
bool StartConfig = false;



//callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

// Ticker flag to go to config mode
void ConfigAPMode () {
  Serial.println("Config check");
  if ( digitalRead(TRIGGER_PIN) == LOW) {
    StartConfig = true;
  }
}

void StartConfigAP(){

  if (StartConfig) {
    StartConfig = false;
    // detach all tickers (redLed, blueLed, OTA, wifimgr, UKI_UDP)
    redLedState (1, 500);
    blueLedState (1,500);
    detachOTA();
    tkConfig.detach();
    delay (500);
    
    //WiFiManager
    
    // The extra parameters to be configured (can be either global or just in the setup)
    // After connecting, parameter.getValue() will get you the configured value
    // id/name placeholder/prompt default length
    WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server, 40);
    WiFiManagerParameter custom_mqtt_port("port", "mqtt port", mqtt_port, 5);
    WiFiManagerParameter custom_blynk_token("blynk", "blynk token", blynk_token, 32);
  
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;

    //reset settings - for testing
    //wifiManager.resetSettings();

    //set config save notify callback
    wifiManager.setSaveConfigCallback(saveConfigCallback);

    //add all your parameters here
    wifiManager.addParameter(&custom_mqtt_server);
    wifiManager.addParameter(&custom_mqtt_port);
    wifiManager.addParameter(&custom_blynk_token);

    //it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration

    redLedState (0, 500);
    blueLedState (-1, 100);
    delay(1000);
    if (!wifiManager.startConfigPortal("UKI_AP")) {
      Serial.println("failed to connect and hit timeout");
      delay(3000);
      //reset and try again, or maybe put it to deep sleep
      ESP.reset();
      delay(5000);
      }
  

    //if you get here you have connected to the WiFi
    Serial.println("connected to UKI wifi");
    blueLedState(1,500);

    //read updated parameters
    strcpy(mqtt_server, custom_mqtt_server.getValue());
    strcpy(mqtt_port, custom_mqtt_port.getValue());
    strcpy(blynk_token, custom_blynk_token.getValue());

    // attach() tickers again ?

    //save the custom parameters to FS
//    if (shouldSaveConfig) {
//      Serial.print("should save config");
//      }
    }
  }


void setupWifi() {
  pinMode(TRIGGER_PIN, INPUT);
  tkConfig.attach(5, ConfigAPMode); // check TRIGGER_PIN state periodically
  
}


