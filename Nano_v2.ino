//ECE 2799 Team 6
//Controller

#include <Wire.h>
#define buttonPin 2
#define motorPin 4
int buttonState;
int lastButtonState = LOW;
int alertState = 0;
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
int motorON;
enum State_enum {SENSE, ALERT};
int state = 0;

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(motorPin, OUTPUT);
  Serial.begin(9600);
  Wire.begin(1);                          //Begins I2C communication with Slave Address as 1 at pin (A4,A5)
  Wire.onReceive(receiveEvent);           //Function call when Slave receives value from master
  Wire.onRequest(requestEvent);           //Function call when Master request value from Slave
}

void loop() {

  int reading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {

    if (reading != buttonState) {
      buttonState = reading;
    }

    if (buttonState == 1) {
      if (alertState == 0) {
        digitalWrite(motorPin, LOW);
        alertState = 1;
      }

      else {
        alertState = 0;
      }


    }
  }
  lastButtonState = reading;

}



void receiveEvent(int howMany) {
  byte alarm = Wire.read();
  if (alarm) {
    digitalWrite(motorPin, HIGH);
    motorON = 1;
  }
  else {
    digitalWrite(motorPin, LOW);
    motorON = 0;
  }
  Serial.print("Motor On = ");
  Serial.println(motorON);
}

void requestEvent() {                               //This Function is called when Master wants value from slave
  Serial.print("request alert state = ");
  Serial.println(alertState);
  Wire.write(alertState);
  // alertState = 0;
}
