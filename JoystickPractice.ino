void setup() {
  //Begins the serial monitor at a rate of 9600 bauds.
  Serial.begin(9600);
  //Sets all four pins as outputs so that it can power the LEDs.
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(9, OUTPUT);
}


void loop() {
  //Gets the X and Y position of the joystick
  int x = analogRead(A0);
  int y = analogRead(A1);


  // Initialize all brightness variables to 0
  int xBright1 = 0; // X High (Right)
  int xBright2 = 0; // X Low  (Left)
  int yBright1 = 0; // Y Low  (Down)
  int yBright2 = 0; // Y High (Up)


  // --- Process X-Axis Movement ---
  //Right x value for the LEDs on the right side.
  if (x > 540) {
    //Uses map to get the ratio between the x position of the joystick with a deadzone to half the brightness. The other
    //half of the brightness will come from the y position.
    xBright1 = map(x, 540, 1023, 0, 127);
    //Ensures the brightness stays within zero to half.
    xBright1 = constrain(xBright1, 0, 127);
  }
  //Left x value for the LEDs on the left side.
  else if (x < 484) {
    xBright2 = map(x, 484, 0, 0, 127);
    xBright2 = constrain(xBright2, 0, 127);
  }


  // --- Process Y-Axis Movement ---
  //Up y values for LEDs on the top
  if (y < 484) {
    //Uses map to get the ratio between the y position of the joystick with a deadzone to half the brightness. The other
    //half of the brightness will come from the x position.
    yBright1 = map(y, 484, 0, 0, 127);
    //Ensures the brightness stays within zero to half.
    yBright1 = constrain(yBright1, 0, 127);
  }
  //Down y values for LEDs on the bottom
  else if (y > 540) {
    yBright2 = map(y, 540, 1023, 0, 127);
    yBright2 = constrain(yBright2, 0, 127);
  }


  // --- Mix and Constrain Values for All 4 Pins ---
  // Pins 5 & 3 (The top two LEDs, left and right)
  //Ensures the power stays within zero to 255.
  int outputPin5 = constrain(xBright1 + yBright1, 0, 255);
  int outputPin3 = constrain(xBright2 + yBright1, 0, 255);


  // Pins 9 & 6 (The Bottom two LEDs, left and right)
  int outputPin9 = constrain(xBright1 + yBright2, 0, 255); // X High + Y High
  int outputPin6 = constrain(xBright2 + yBright2, 0, 255); // X Low + Y Low


  // --- Write PWM Signals to LEDs corresponding to the position of the joystick ---
  analogWrite(5, outputPin5);
  analogWrite(3, outputPin3);
  analogWrite(9, outputPin9);
  analogWrite(6, outputPin6);
 
  delay(10); // Small stability delay
}
