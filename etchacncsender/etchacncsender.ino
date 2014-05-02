/* Encoder Library - TwoKnobs Example
 * http://www.pjrc.com/teensy/td_libs_Encoder.html
 *
 * This example code is in the public domain.
 */

#include <Encoder.h>
#include <Bounce2.h>

// Change these pin numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder knobLeft(2, 4);
Encoder knobRight(3, 5);
//   avoid using pins with LEDs attached
#define PENUP 8
#define PENDOWN 9
#define MOTOROFF 10

// Instantiate a Bounce object
Bounce penupcheck = Bounce(); 
Bounce pendowncheck = Bounce(); 
Bounce systemreset = Bounce(); 

void setup() {
  Serial1.begin(9600);
  Serial1.println("; GCODE init statements");
  Serial.begin(9600);
  Serial.println("; GCODE init statements");
    // Setup the button
  pinMode(PENUP,INPUT);
  pinMode(PENDOWN,INPUT);
  pinMode(MOTOROFF,INPUT);
  // Activate internal pull-up
  digitalWrite(PENUP,HIGH);
  digitalWrite(PENDOWN,HIGH);
  digitalWrite(MOTOROFF,HIGH);
  
  // After setting up the button, setup debouncer
  penupcheck.attach(PENUP);
  penupcheck.interval(100);
  pendowncheck.attach(PENDOWN);
  pendowncheck.interval(100);
  systemreset.attach(MOTOROFF);
  systemreset.interval(200);
  
}

long positionLeft  = -999;
long positionRight = -999;
long xmax = 200;
long ymax = 200;

void loop() {
   // Update the debouncer
 penupcheck.update();
 pendowncheck.update();
 systemreset.update();
 
 // Get the update value
 int penupvalue = penupcheck.read();
 int pendownvalue = pendowncheck.read();
 int systemresetvalue = systemreset.read();

 if (penupvalue == LOW ) {
   Serial1.println("G1 Z50 F1000");
   Serial.print(positionLeft);
   Serial.print(",");
   Serial.print(positionRight);
   Serial.println(",UP,RUN#");
 }
 if (pendownvalue == LOW) {
   Serial1.println("G1 Z0 F1000");
   Serial.print(positionLeft);
   Serial.print(",");
   Serial.print(positionRight);
   Serial.println(",DOWN,RUN#");
 }
 if (systemresetvalue == LOW) {
   Serial1.println("G1 X0 Y0 Z50 F1000"); //may use: M84=disable until next move, M18=disable all steppers
   delay (1000);
   Serial.print(positionLeft);
   Serial.print(",");
   Serial.print(positionRight);
   Serial.println(",CURRENT,RESET#"); //may use: M84=disable until next move, M18=disable all steppers
   positionLeft = 0;
   positionRight = 0;
   knobLeft.write(0);
   knobRight.write(0);
 }
 
  long newLeft, newRight;
  newLeft = knobLeft.read();
  newRight = knobRight.read();
  if (newLeft != positionLeft || newRight != positionRight) {
//    Serial.print("Left = ");
//    Serial.print(newLeft);
//    Serial.print(", Right = ");
//    Serial.print(newRight);
//    Serial.println();
    if (newLeft < 0) {
      knobLeft.write(0);
      newLeft = 0;
    }
    if (newRight < 0) {
      knobRight.write(0);
      newRight = 0;
    }
    if (newLeft > xmax) {
      knobLeft.write(200);
      newLeft = 200;
    }
    if (newRight > ymax) {
      knobRight.write(200);
      newRight = 200;
    }
    Serial1.print ("G1 X");
    Serial1.print (newLeft);
    Serial1.print (" Y");
    Serial1.print (newRight);
    Serial1.print (" F1000");
    Serial1.println();
    Serial.print (newLeft);
    Serial.print (",");
    Serial.print (newRight);
    Serial.print (",CURRENT,RUN#");
    Serial.println();
    positionLeft = newLeft;
    positionRight = newRight;
  }
  // if a character is sent from the serial monitor,
  // reset both back to zero.
//  if (Serial.available()) {
//    Serial.read();
//    Serial.println("Reset both knobs to zero");
//    knobLeft.write(0);
//    knobRight.write(0);
//  }
delay (100);
}
