void setup() {
  Serial.begin(9600);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(9, OUTPUT);
}


void loop() {
  int x = analogRead(A0);
  int y = analogRead(A1);


  // Initialize all brightness variables to 0
  int xBright1 = 0; // X High (Right)
  int xBright2 = 0; // X Low  (Left)
  int yBright1 = 0; // Y Low  (Down)
  int yBright2 = 0; // Y High (Up)


  // --- 1. Process X-Axis Movement ---
  if (x > 540) {
    xBright1 = map(x, 540, 1023, 0, 127);
    xBright1 = constrain(xBright1, 0, 127);
  }
  else if (x < 484) {
    xBright2 = map(x, 484, 0, 0, 127);
    xBright2 = constrain(xBright2, 0, 127);
  }


  // --- 2. Process Y-Axis Movement ---
  if (y < 484) {
    yBright1 = map(y, 484, 0, 0, 127);
    yBright1 = constrain(yBright1, 0, 127);
  }
  else if (y > 540) {
    yBright2 = map(y, 540, 1023, 0, 127);
    yBright2 = constrain(yBright2, 0, 127);
  }


  // --- 3. Mix and Constrain Values for All 4 Pins ---
  // Pins 5 & 3 (Your Original Pins)
  int outputPin5 = constrain(xBright1 + yBright1, 0, 255);
  int outputPin3 = constrain(xBright2 + yBright1, 0, 255);


  // Pins 9 & 6 (The Bottom / Remaining Pins)
  int outputPin9 = constrain(xBright1 + yBright2, 0, 255); // X High + Y High
  int outputPin6 = constrain(xBright2 + yBright2, 0, 255); // X Low + Y Low


  // --- 4. Write PWM Signals to LEDs ---
  analogWrite(5, outputPin5);
  analogWrite(3, outputPin3);
  analogWrite(9, outputPin9);
  analogWrite(6, outputPin6);
 
  delay(10); // Small stability delay
}
