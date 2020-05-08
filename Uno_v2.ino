//ECE2799 Team 6
//Main unit

#include <Wire.h>
#define speakerPin 4
#define PIR 3
#define pingPin 2
bool firstRun = true;
float prevDist;
float avgDist;
float takeSamples();
byte panicStatus;
long flag;
enum State_enum {SENSE, ALERT};
int state = 0;
int counter = 0;

void setup() {
  pinMode(PIR, INPUT);
  pinMode(speakerPin, OUTPUT);
  Serial.begin(9600);          // initialize serial
  Wire.begin();                // initalize i2c communication
}

void loop() {

  switch (state) {
    case SENSE:
      Serial.println("SENSE");

      noTone(speakerPin);         //turn off speaker when ALERT case is over
   //  panicStatus = 0;
      avgDist = takeSamples();
      if (firstRun) {
        prevDist = avgDist;
        firstRun = false;
      }

      //  panicStatus = 0;            //reset panic flag
      //  flag = random(2);              //generate a random 0 or 1 to simulate motion detection
      if (avgDist < (prevDist - 1)) {
        //Process PIR Input
        // if (digitalRead(PIR) == HIGH) { // check if the sensor is HIGH
        Serial.println("Motion detected!");
        flag = 1;
        //}
      }
      if (flag) {
        Serial.println("Turn on LED");
        Wire.beginTransmission(1);   //connect to Slave 1 (controller)
        Wire.write(1);               //send 1, signifies motion detection
        Wire.endTransmission();      //stop transmitting to Slave 1 (controller)
        flag = 0;
      }

      prevDist = avgDist;

      //Read in controller
      Wire.requestFrom(1, 1);       //read from Slave 1 (controller)
      panicStatus = Wire.read();    //store reading (button press)
      Serial.print("PRESS STATUS = ");
      Serial.println(panicStatus);
      

      if (panicStatus == 1) {       //if button is pressed
        Serial.println("PRESS");
        state = ALERT;              //switch to alert state
      }
      break;

    case ALERT:
      Serial.println("ALERT");

     // panicStatus = 0;
      tone(speakerPin, 3000);
      for (counter = 0; counter < 30000000; counter++) { //30 rounds
        Wire.requestFrom(1, 1);     //request button sate form Slave 1 (controller)
        panicStatus = Wire.read();
        Serial.println(panicStatus);

        if (panicStatus == 0) {      //if button is pressed
          noTone(speakerPin);       //turn off alarm
          state = SENSE;            //switch back to sensing
          break;
        }
      } //if 3000 rounds reached
      panicStatus = 0;
      state = SENSE;               //switch back to sensing
      break;
  }
}

float takeSamples() {
  //Process Ultrasonic Sensor
  float duration, inches;
  float distSum = 0;

  //Take 5 samples
  int samples = 0;
  while (samples < 5) {

    //trigger PING pulse
    pinMode(pingPin, OUTPUT);
    digitalWrite(pingPin, LOW);
    delayMicroseconds(2);
    digitalWrite(pingPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(pingPin, LOW);

    // Read in signal from PING pin
    pinMode(pingPin, INPUT);
    duration = pulseIn(pingPin, HIGH);

    // convert the time into a distance using conversions found on datasheet
    inches = (duration / 1000000) * 890 * 12;

    //Check if measurements are in bounds
    if ((inches > 2) && (inches < 100)) {
      samples ++;
      distSum += inches;
    }
  }
  return (distSum / 5);
}
