
//flag for saving data
bool shouldSaveConfig = false;
#define TRIGGER_PIN 12
Ticker tkConfig ;
bool StartConfig = false;



//callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

void ConfigAPMode () {
  Serial.println("Config check");
  if ( digitalRead(TRIGGER_PIN) == LOW) {
    StartConfig = true;
  }
}

void StartConfigAP(){

  if (StartConfig) {
    // detach all tickers (redLed, blueLed, OTA, wifimgr, UKI_UDP)
//    redLedState (1, 500);
//    blueLedState (1,500);
//    detachOTA();
    tkConfig.detach();
    delay (500);
    
    //WiFiManager
  
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;

    //reset settings - for testing
    //wifiManager.resetSettings();

    //sets timeout until configuration portal gets turned off
    //useful to make it all retry or go to sleep
    //in seconds
    //wifiManager.setTimeout(120);

    //set config save notify callback
    wifiManager.setSaveConfigCallback(saveConfigCallback);

    //it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration

    //WITHOUT THIS THE AP DOES NOT SEEM TO WORK PROPERLY WITH SDK 1.5 , update to at least 1.5.1
    //WiFi.mode(WIFI_STA);
    //redLedState (0, 500);
    //blueLedState (-1, 100);
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
   // blueLedState(1,500);

    //save the custom parameters to FS
    if (shouldSaveConfig) {
      Serial.print("should save config");
      }
    }
  }


void setupWifi() {
  pinMode(TRIGGER_PIN, INPUT);
  tkConfig.attach(5, ConfigAPMode); // check TRIGGER_PIN state periodically
  
}


