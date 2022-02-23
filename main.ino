#define servoPin 9
#define motorPin 10
#define irPin 11
//#define DECODE_NEC          // Includes Apple and Onkyo
#define DECODE_SONY

#include <Arduino.h>
#include <Servo.h>
#include <IRremote.hpp>

Servo myServo;  // create a servo object
int myServoValue = 0;
const int servoTol = 10;

void setup() {
  Serial.begin(9600);
  Serial.println("Serial initialized");
  
//  // initialize digital pin LED_BUILTIN as an output.
//  pinMode(LED_BUILTIN, OUTPUT);

  // Start the receiver and if not 3. parameter specified, take LED_BUILTIN pin from the internal boards definition as default feedback LED
  Serial.print(F("Ready to receive IR signals of protocols: "));
  printActiveIRProtocols(&Serial);
  Serial.print(F("at pin "));
  Serial.println(irPin); 
  IrReceiver.begin(irPin, ENABLE_LED_FEEDBACK);
  
  myServo.attach(servoPin);
  
  pinMode(motorPin, OUTPUT);
  digitalWrite(motorPin, LOW); 

  while (!Serial);
  Serial.println("Provide speed from 0 to 255");
}

// the loop function runs over and over again forever
void loop() {
  handleIR();
  handleMotor();
  handleServo();
}

void handleMotor() {
    if (Serial.available()) {
    int speed = Serial.parseInt();
    if (speed >= 0 && speed <= 255) {
//      Serial.print("About to set the speed to %d", speed);
      digitalWrite(motorPin, speed);
 //     Serial.print("Speed was set to %d", speed);
    }
  }
}

void handleServo() {
//  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
//  myServo.write(0);
//  delay(1000);                       // wait for a second
//  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
//  myServo.write(180);
//  delay(1000);                       // wait for a second


  myServo.write(myServoValue);
}

void handleIR() {
  if (IrReceiver.decode()) {
//    // Print a short summary of received data
//    IrReceiver.printIRResultShort(&Serial);
//    if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
//        // We have an unknown protocol here, print more info
//        IrReceiver.printIRResultRawFormatted(&Serial, true);
//    }
//    Serial.println();

    /*
     * !!!Important!!! Enable receiving of the next value,
     * since receiving has stopped after the end of the current received data packet.
     */
    IrReceiver.resume(); // Enable receiving of the next value

    if (IrReceiver.decodedIRData.address == 0x1) {
      if (IrReceiver.decodedIRData.command == 0x74) { //arrow up
          decreaseServo();
      } else if (IrReceiver.decodedIRData.command == 0x75) { //arrow down
          increaseServo();
      }
    }
  }
}

void increaseServo() {
  myServoValue += servoTol;
  if (myServoValue >= 255) {
    myServoValue = 255;
  }
}

void decreaseServo() {
  myServoValue -= servoTol;
  if (myServoValue <= 0) {
    myServoValue = 0;
  }
}
