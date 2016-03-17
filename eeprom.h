//
//struct strConfig {
//  String ssid;
//  String password;
//  byte  IP[4];
//  byte  Netmask[4];
//  byte  Gateway[4];
//  boolean dhcp;
//  String ntpServerName;
//  long Update_Time_Via_NTP_Every;
//  long timezone;
//  boolean daylight;
//  String DeviceName;
//  boolean AutoTurnOff;
//  boolean AutoTurnOn;
//  byte TurnOffHour;
//  byte TurnOffMinute;
//  byte TurnOnHour;
//  byte TurnOnMinute;
//  byte LED_R;
//  byte LED_G;
//  byte LED_B;
//}   config;
//
//
///*
//**
//** CONFIGURATION HANDLING
//**
//*/
//
//
//
//
//
//
//void WriteConfig()
//{
//
//  Serial.println("Writing Config");
//  EEPROM.write(0,'C');
//  EEPROM.write(1,'F');
//  EEPROM.write(2,'G');
//
//  EEPROM.write(16,config.dhcp);
//  EEPROM.write(17,config.daylight);
//  
//  EEPROMWritelong(18,config.Update_Time_Via_NTP_Every); // 4 Byte
//
//  EEPROMWritelong(22,config.timezone);  // 4 Byte
//
//
//  EEPROM.write(26,config.LED_R);
//  EEPROM.write(27,config.LED_G);
//  EEPROM.write(28,config.LED_B);
//
//  EEPROM.write(32,config.IP[0]);
//  EEPROM.write(33,config.IP[1]);
//  EEPROM.write(34,config.IP[2]);
//  EEPROM.write(35,config.IP[3]);
//
//  EEPROM.write(36,config.Netmask[0]);
//  EEPROM.write(37,config.Netmask[1]);
//  EEPROM.write(38,config.Netmask[2]);
//  EEPROM.write(39,config.Netmask[3]);
//
//  EEPROM.write(40,config.Gateway[0]);
//  EEPROM.write(41,config.Gateway[1]);
//  EEPROM.write(42,config.Gateway[2]);
//  EEPROM.write(43,config.Gateway[3]);
//
//
//  WriteStringToEEPROM(64,config.ssid);
//  WriteStringToEEPROM(96,config.password);
//  WriteStringToEEPROM(128,config.ntpServerName);
//
//  EEPROM.write(300,config.AutoTurnOn);
//  EEPROM.write(301,config.AutoTurnOff);
//  EEPROM.write(302,config.TurnOnHour);
//  EEPROM.write(303,config.TurnOnMinute);
//  EEPROM.write(304,config.TurnOffHour);
//  EEPROM.write(305,config.TurnOffMinute);
//  WriteStringToEEPROM(306,config.DeviceName);
//  
//
//
//  EEPROM.commit();
//}
//boolean ReadConfig()
//{
//
//  Serial.println("Reading Configuration");
//  if (EEPROM.read(0) == 'C' && EEPROM.read(1) == 'F'  && EEPROM.read(2) == 'G' )
//  {
//    Serial.println("Configurarion Found!");
//    config.dhcp =   EEPROM.read(16);
//
//    config.daylight = EEPROM.read(17);
//
//    config.Update_Time_Via_NTP_Every = EEPROMReadlong(18); // 4 Byte
//
//    config.timezone = EEPROMReadlong(22); // 4 Byte
//
//    config.LED_R = EEPROM.read(26);
//    config.LED_G = EEPROM.read(27);
//    config.LED_B = EEPROM.read(28);
//
//    config.IP[0] = EEPROM.read(32);
//    config.IP[1] = EEPROM.read(33);
//    config.IP[2] = EEPROM.read(34);
//    config.IP[3] = EEPROM.read(35);
//    config.Netmask[0] = EEPROM.read(36);
//    config.Netmask[1] = EEPROM.read(37);
//    config.Netmask[2] = EEPROM.read(38);
//    config.Netmask[3] = EEPROM.read(39);
//    config.Gateway[0] = EEPROM.read(40);
//    config.Gateway[1] = EEPROM.read(41);
//    config.Gateway[2] = EEPROM.read(42);
//    config.Gateway[3] = EEPROM.read(43);
//    config.ssid = ReadStringFromEEPROM(64);
//    config.password = ReadStringFromEEPROM(96);
//    config.ntpServerName = ReadStringFromEEPROM(128);
//    
//    
//    config.AutoTurnOn = EEPROM.read(300);
//    config.AutoTurnOff = EEPROM.read(301);
//    config.TurnOnHour = EEPROM.read(302);
//    config.TurnOnMinute = EEPROM.read(303);
//    config.TurnOffHour = EEPROM.read(304);
//    config.TurnOffMinute = EEPROM.read(305);
//    config.DeviceName= ReadStringFromEEPROM(306);
//    return true;
//    
//  }
//  else
//  {
//    Serial.println("Configurarion NOT FOUND!!!!");
//    return false;
//  }
//}

