/* UDP CONFIGURATION */
int UKI_UDP_In_Port = 9000;  //udp port input for ESP

//default address and port to send to (IP read from config)
IPAddress UKI_UDP_Out_IP ;
int UKI_UDP_Out_Port = 8000 ;

Ticker tkUKI;  // periodic send ADC to UDP
int GSR_sensor;
bool flag_UDP;
int  UDP_tick = 50 ; //delay between to udp send


void UDP_flag(){
  flag_UDP = true;
}


void UDP_send_receive() {

  if (flag_UDP) {
    flag_UDP = false;
      
       /*  UKI part  */
    GSR_sensor = analogRead(A0);
    //UKI_UDP.beginPacketMulticast((224, 1, 2, 3), 8000, WiFi.localIP());//
    UKI_UDP.beginPacket(UKI_UDP_Out_IP, UKI_UDP_Out_Port);
    UKI_UDP.print("UKI");
    UKI_UDP.print(" ");
    UKI_UDP.print(GSR_sensor);
    UKI_UDP.endPacket();
      
    //Check udp in
    int packetSize = UKI_UDP.parsePacket();
    
    if(packetSize) {
      UKI_UDP_Out_IP = UKI_UDP.remoteIP();
      UKI_UDP.beginPacket(UKI_UDP_Out_IP, 8000);
      UKI_UDP.print("new master ip");
      UKI_UDP.endPacket();
    }
  }
  

 
}


void setupUDP(){
  UKI_UDP_Out_IP.fromString(UKI_UDP_IP);
  Serial.print ("sending UDP to ");
  Serial.println (UKI_UDP_Out_IP);
  UKI_UDP.begin(UKI_UDP_In_Port); 
  tkUKI.attach_ms(UDP_tick, UDP_flag); //raises flag 
}
