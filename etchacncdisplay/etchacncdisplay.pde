/**
 * Continuous Lines. 
 * 
 * Click and drag the mouse to draw a line. 
 */
import processing.serial.*;
Serial port;  // The serial port object
int x,y,estop,px,py;
String zstr,pendown,estopstr;

void setup() {
  size(405,405);
  background(102);
  // Using the first available port (might be different on your computer)
  println(Serial.list());
  println(Serial.list()[7]);
  port = new Serial(this, Serial.list()[7], 9600);
  // Request values right off the bat
  port.write(65);
  x = 0;
  y = 400;
  pendown = "DOWN";
}

void draw() {
//  stroke(255);
//  if (z < 10) {
//    line(x, y, px, py);
//  }
}

// Called whenever there is something available to read
void serialEvent(Serial port) {
  // Data from the Serial port is read in serialEvent() using the readStringUntil() function with * as the end character.
  String input = port.readStringUntil(10); 
  
  if (input != null) {
    // Print message received
//    println( "Receiving:" + input);
    
    // The data is split into an array of Strings with a comma or asterisk as a delimiter and converted into an array of integers.
   String[] vals = splitTokens(input, ",#"); 
    // Fill variables
    px = x;
    py = y;
    x = 2* int (vals[0]);
    y = 2* (200 - int (vals[1]));
    zstr = vals[2];
//    println (zstr);
//    println (pendown);
    estopstr = vals[3];
    if (zstr.equals("UP")) {
      pendown = "UP";
//      println ("penup");
    } else if (zstr.equals("DOWN")) {
      pendown = "DOWN";
//      println ("pendown");
    }
    if (estopstr.equals("RESET")) {
      background(102);
    }
    stroke(255);
    if (pendown.equals("DOWN")) {
      println( "drawing " + input);
      line(x, y, px, py);
    }
  }
  // When finished ask for values again
  //port.write(65);
}
