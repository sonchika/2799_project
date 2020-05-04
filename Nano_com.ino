#include <Wire.h>

//ECE 2799 Team 6
//Simple Communication (button and led)
//Controller
#define buttonPin 2
#define motorPin 4        //actually an LED for test purposes
int buttonState;
int lastButtonState = LOW;
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
int motorON;

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

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;
    }

    // only toggle the LED if the new button state is HIGH
    //Serial.write("Button state = " + buttonState);
    //Serial.println();

    if (buttonState == 1) {
     // Serial.write("Motor on = " + motorON);
     // Serial.println();
      if (motorON) {
        digitalWrite(motorPin, LOW);
      }
    }
  }
  lastButtonState = reading;
}

void receiveEvent(int howMany) {
  //Serial.println("------------- receive ------------");
  byte alarm = Wire.read();
  if (alarm) {
    digitalWrite(motorPin, HIGH);
    motorON = 1;
   // Serial.println("Motor ON");
  }
}

void requestEvent() {                               //This Function is called when Master wants value from slave
  Wire.write(lastButtonState);                          // sends one byte converted POT value to master
}
