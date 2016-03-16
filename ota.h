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
    //ledBlink(Blue_Led, 3, 100);

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
  Serial.println("OTA started");
}

void loopOTA(){
  
  if (loop_counter == 5)  {
    ArduinoOTA.handle();
    delay(10);
    }


}

