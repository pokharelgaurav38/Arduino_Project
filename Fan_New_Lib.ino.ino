#include <IRremote.hpp>

int RECV_PIN = 11; // IR receiver pin
int speed1 = 2;    // Speed 1 pin
int speed2 = 4;    // Speed 2 pin
int speed3 = 6;    // Speed 3 pin
int led = 8;      // LED pin
unsigned long Delay = 0;
int i = 0;
int j = 0;
int k = 0;

#define code1 0x43430E7F   // code received from power button
#define code2 0x54511082   // code received from Speed button
#define code3 0x1E11AAFE   // code received from Timer button

unsigned long lastCodeTime = 0;
const unsigned long debounceDelay = 500; // debounce delay in milliseconds

void setup() {
  Serial.begin(9600);   
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);
  pinMode(speed1, OUTPUT);
  pinMode(speed2, OUTPUT);
  pinMode(speed3, OUTPUT);
  pinMode(led, OUTPUT);

  // Set default state: fan on speed 1
  j = 1;
  k = 1;
  digitalWrite(speed3, HIGH); 
  digitalWrite(speed2, LOW); 
  digitalWrite(speed1, LOW);
  Serial.println("Default state: Fan on speed 1");
}

void loop() {
  unsigned long currentMillis = millis();
  if (IrReceiver.decode()) {
    if (currentMillis - lastCodeTime > debounceDelay) {
      lastCodeTime = currentMillis;
      unsigned long value = IrReceiver.decodedIRData.decodedRawData;
      Serial.print("IR Code Received: ");
      Serial.println(value, HEX);

      switch (value) {
        case code1:
          Serial.println("Power Button Pressed");
          if (j == 0) {
            j = 1;
            k++;
            digitalWrite(speed3, HIGH); 
            digitalWrite(speed2, LOW); 
            digitalWrite(speed1, LOW);
            Serial.println("Fan set to speed 1");
          } else {
            i = 0;
            j = 0;
            k = 0;
            Delay = 0;
            digitalWrite(led, LOW); 
            digitalWrite(speed3, LOW); 
            digitalWrite(speed2, LOW); 
            digitalWrite(speed1, LOW);
            Serial.println("Fan turned off from power button");
          }
          break;

        case code2:
          Serial.println("Speed Button Pressed");
          if (k == 1) {
            k++;
            digitalWrite(speed3, LOW); 
            digitalWrite(speed2, HIGH); 
            digitalWrite(speed1, LOW); 
            Serial.println("Fan set to speed 2");
          } else if (k == 2) {
            k++;
            digitalWrite(speed3, LOW); 
            digitalWrite(speed2, LOW); 
            digitalWrite(speed1, HIGH); 
            Serial.println("Fan set to speed 3");
          } else if (k == 3) {
            k = 1;
            j = 1;
            digitalWrite(speed3, HIGH); 
            digitalWrite(speed2, LOW); 
            digitalWrite(speed1, LOW);
            Serial.println("Fan set to speed 1");
          }
          break;
        
        case code3:
          Serial.println("Timer Button Pressed");
          if (k >= 1) {
            if (i == 0) {
              i++;
              Delay = currentMillis + (60000 * 15); // for 15 min
              digitalWrite(led, HIGH);
              delay(100);
              digitalWrite(led, LOW);
              delay(100);
              digitalWrite(led, HIGH);
              Serial.println("Timer set for 15 minutes");
            } else if (i == 1) {
              i++;
              Delay = currentMillis + (60000 * 30); // for 30 min
              digitalWrite(led, HIGH);
              delay(100);
              digitalWrite(led, LOW);
              delay(100);
              digitalWrite(led, HIGH);
              Serial.println("Timer set for 30 minutes");             
            } else if (i == 2) {
              i++;
              Delay = currentMillis + (60000 * 60); // for 1 hr
              digitalWrite(led, HIGH);
              delay(100);
              digitalWrite(led, LOW);
              delay(100);
              digitalWrite(led, HIGH);
              Serial.println("Timer set for 1 hour");             
            } else if (i == 3) {
              i++;
              Delay = currentMillis + (60000 * 120); // for 2 hr
              digitalWrite(led, HIGH);
              delay(100);
              digitalWrite(led, LOW);
              delay(100);
              digitalWrite(led, HIGH);
              Serial.println("Timer set for 2 hours");             
            } else {
              i = 0;
              Delay = 0;
              digitalWrite(led, LOW);
              Serial.println("No Timer");    
            }
          }
          break;
      }
    }
    IrReceiver.resume();   
  }
  
  if (Delay > currentMillis) {
    Serial.println("Timer Running"); 
  }
  if (Delay <= currentMillis && Delay != 0) {
    i = 0;
    j = 0;
    k = 0;
    Delay = 0;
    digitalWrite(speed1, LOW); 
    digitalWrite(speed2, LOW); 
    digitalWrite(speed3, LOW);
    digitalWrite(led, LOW); 
    Serial.println("Timer off");
  }
}
