//ticker flag for saving data
bool flag_SaveConfig = false;

#define TRIGGER_PIN 12 //start onDemand config portal when set to LOW
Ticker tkConfig ;
bool flag_ConfigPortal = false;

char UKI_NAME[40];
char UKI_UDP_PORT[6] = "9000";
char UKI_UDP_IP[34] = "192.168.10.100";



//callback notifying the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  flag_SaveConfig = true;
}

// Ticker flag to go to config mode
void CheckTriggerPin () {
  Serial.println("Config check");
  if ( digitalRead(TRIGGER_PIN) == LOW) {
    flag_ConfigPortal = true;
  }
}

void ReadConfig() {
  
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

          strcpy(UKI_NAME, json["UKI_NAME"]);
          strcpy(UKI_UDP_PORT, json["UKI_UDP_PORT"]);
          strcpy(UKI_UDP_IP, json["UKI_UDP_IP"]);

        } else {
          Serial.println("failed to load json config");
        }
      }
    }
  } else {
    Serial.println("failed to mount FS");
  }
  //end read
}

void WriteConfig() {

  //save the custom parameters to FS
    if (flag_SaveConfig) {
      
      flag_SaveConfig = false ;
      Serial.println("saving config");
      DynamicJsonBuffer jsonBuffer;
      JsonObject& json = jsonBuffer.createObject();
      json["UKI_NAME"] = UKI_NAME;
      json["UKI_UDP_PORT"] = UKI_UDP_PORT;
      json["UKI_UDP_IP"] = UKI_UDP_IP;

      File configFile = SPIFFS.open("/config.json", "w");
      if (!configFile) {
        Serial.println("failed to open config file for writing");
      }
  
      json.printTo(Serial);
      json.printTo(configFile);
      configFile.close();
      //end save
    }
}

void StartConfigAP(){  
  /* stops all tickers, start config portal and waits for new configuration
   *  if new connection, saves the new configuration values and goes back to loop()
   add a check if disconnected ?
   */

  if (flag_ConfigPortal) { //or disconnected from wifi
    
    flag_ConfigPortal = false; //reset flag
    
    // detach all tickers (redLed, blueLed, OTA, wifimgr, UKI_UDP)
    redLedState (1, 500);
    blueLedState (1,500);
    detachOTA();
    tkConfig.detach();
    delay (500); 

    ReadConfig() ; //read config.json from FS
    
    //WiFiManager
    
    // The extra parameters to be configured (can be either global or just in the setup)
    // After connecting, parameter.getValue() will get you the configured value
    // id/name placeholder/prompt default length
    WiFiManagerParameter custom_UKI_NAME("name", "UKI NAME", UKI_NAME, 40);
    WiFiManagerParameter custom_UKI_UDP_PORT("port", "UKI udp port", UKI_UDP_PORT, 5);
    WiFiManagerParameter custom_UKI_UDP_IP("ip", "UKI udp IP", UKI_UDP_IP, 32);
  
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;
    
    //wifiManager.resetSettings();//reset settings - for testing    
    wifiManager.setSaveConfigCallback(saveConfigCallback);//set config save notify callback

    //add all your parameters here
    wifiManager.addParameter(&custom_UKI_NAME);
    wifiManager.addParameter(&custom_UKI_UDP_PORT);
    wifiManager.addParameter(&custom_UKI_UDP_IP);

    //it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration

    redLedState (0, 500);
    blueLedState (-1, 100);
    delay(1000);
    if (!wifiManager.startConfigPortal("ESP_UKI_AP")) {
      Serial.println("failed to connect, restarting");
      //reset and try again
      redLedState (-1, 100);
      blueLedState (-1, 100);
      ESP.reset();
      delay(3000);
      }

    //if you get here you have connected to the WiFi
    Serial.println("connected to UKI wifi");
    blueLedState(1,500);

    //read updated parameters
    strcpy(UKI_NAME, custom_UKI_NAME.getValue());
    strcpy(UKI_UDP_PORT, custom_UKI_UDP_PORT.getValue());
    strcpy(UKI_UDP_IP, custom_UKI_UDP_IP.getValue());

    WriteConfig();
    
    // attach() tickers back now that it's working
    
  }
}


void setupWifi() {
  pinMode(TRIGGER_PIN, INPUT);
  tkConfig.attach(5, CheckTriggerPin); // check TRIGGER_PIN state periodically
  WiFi.begin();
//  while(WiFi.status()<3) {
//    Serial.print
//clean FS, for testing
  //SPIFFS.format();
  //}
  
}


