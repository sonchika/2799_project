#include <Wire.h>    
#define LEDPin 8   
long flag;  
byte PanicStatus;   
int i;

//ECE2799 Team 6
//Simple Communication (button and LED)
//Main unit 


void setup() {
 pinMode(LEDPin, OUTPUT); // Declare the LED as an output
 Serial.begin(9600);          // initialize serial
 Wire.begin();                // initalize i2c communication 
}

void loop() {
  flag = random(2); //generate a random 0 or 1 to simulate motion detection

 if(flag){ //if flag is 1 then motion detected
 
  Serial.println("Motion detected!"); 
  Wire.beginTransmission(1);   //to Slave1                       
  Wire.write(1);               //send true             
  Wire.endTransmission();  
 }

 else{
  Serial.println("Motion stopped!");
 }
 
 //Read in controller
 Wire.requestFrom(1, 1);
 PanicStatus = Wire.read();
 Serial.println(PanicStatus);
 
 if(PanicStatus == 1){
    digitalWrite(LEDPin, HIGH);           // turn on LED    
    delay(1000);
    digitalWrite(LEDPin, LOW);
  }
  
}
