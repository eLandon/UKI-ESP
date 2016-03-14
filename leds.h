/* LEDS  */
const int Blue_Led = 2;
bool Blue_Led_State ;
Ticker tkBlue_Led ;
int tkBlue_Led_Counter;

const int Red_Led = 0;
bool Red_Led_State ;
Ticker tkRed_Led ;
int tkRed_Led_Counter;


void setupLeds() {
  pinMode(Blue_Led, OUTPUT);
  digitalWrite(Blue_Led, HIGH);
  pinMode(Red_Led, OUTPUT);
  digitalWrite(Red_Led, LOW);//red led on
  
}


void ledBlink (int Led, int blink_qty, int blink_time) {
  for (int i = 0 ; i < blink_qty ; i++) {
    digitalWrite(Led, LOW) ;
    delay(blink_time);
    digitalWrite(Led, HIGH);
    delay(blink_time);
  }
}
