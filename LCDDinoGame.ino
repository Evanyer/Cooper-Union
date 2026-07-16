#include <Wire.h>
#include <LiquidCrystal_I2C.h>


// You must instantiate the lcd object with its I2C address, columns, and rows.
// 0x27 is the most common address for 16x2 displays. Change to 0x3F if 0x27 doesn't work.
LiquidCrystal_I2C lcd(0x27, 16, 2);




// Create a custom dino pixel pattern
byte dino[8] = {//This is an array with size of 8 elements. 0 means the pixel is turned off, and 1 means on
  B00111, //   ███
  B00101, //   █ █
  B00111, //   ███
  B10110, // █ ██
  B11111, // █████
  B01110, //  ███
  B01010, //  █ █
  B00000  // (empty bottom row)
};

//Cactus pixel pattern
byte cactus[8] = {
  B00100,  //   *  
  B00101,  //   * *
  B10101,  // * * *
  B10111,  // * ***
  B11100,  // ***  
  B00100,  //   *  
  B00100,  //   *  
  B00100   //   *  
};

//The column position of the cactus. We don't need a row position as it will always be at 1.
int cactusPos[5]{
  random(17, 19),
  random(23, 25),
  random(29, 32),
  random(36, 39),
  random(42, 46)
};

//Tracks the position of dino to help check if it collides with a cactus.
int dinoCol = 3;
int dinoRow = 1;
//Boolean to continue the game if the player hasn't lost.
boolean lose = false;
//Boolean to prevent jump holdings from flying.
boolean jump = false;


void setup() {
  //Begins the serial monitor at a rate of 9600 bauds.
  Serial.begin(9600);
  //Initializes the display
  lcd.init();
 
  //Turns on the display's background light
  lcd.backlight();
 
  //Uploads your dino pixel grids into memory slot 0. The screen can hold
  //8 custom characters total (slots 0 to 7).
  lcd.createChar(0, dino);
  //Uploads the cactus pixel grid to memory slot 1
  lcd.createChar(1, cactus);
 //Positions the cursor at col 3 and row 1 to draw the dino at that spot.
  lcd.setCursor(3, 1);
 
  //Prints the custom graphic stored in memory slot 0 (dino).write() is used for
  //raw bytes/graphics instead of text strings.
  lcd.write(byte(0));
  //Makes the cursor blink
  lcd.blink();
}


void loop() {
  //static long gameBegin = millis();
  //A multiplier in case the speed is too slow or fast.
  static double multiplier = 1;
  //Stops the game if the player loses.
  if(!lose){
    //'start' helps track the start of an update game interval
    static long start = millis();
    //Checks how long a dino has been in the air. After a period of time, the dino must fall back to ground.
    static long dinoJump;
    //Checks if the interval is finished.
    long current = millis();
    //How long a interval should last
    static double speed = random(500, 1000) * multiplier;



    //If a interval finishes...
    if(current - start >= speed){
      //Find where the furthest cactus currently is BEFORE moving them
      int furthestCactus = 16;
      for(int j = 0; j < 5; j++) {
        if(cactusPos[j] > furthestCactus) {
          furthestCactus = cactusPos[j];
        }
      }

      //Loops through 'cactusPos' array and delete the current cactus.
      for(int i = 0; i < 5; i++){
        if(cactusPos[i] >= 0 && cactusPos[i] < 16) {
          lcd.setCursor(cactusPos[i], 1);
          lcd.print(" ");
        }

        //Moves all cactus a column forward.
        cactusPos[i] = cactusPos[i] - 1;

        //If the cactus goes out of bounds, it sends the current cactus back to a new position.
        if(cactusPos[i] < 0){
          // Spawn exactly behind the furthest active cactus plus a safe gap
          cactusPos[i] = furthestCactus + random(6, 10);
          furthestCactus = cactusPos[i]; // Update tracker for the next iteration
        }

        //Redraws all cactuses in their new position after being deleted from their old one.
        if(cactusPos[i] >= 0 && cactusPos[i] < 16) {
          lcd.setCursor(cactusPos[i], 1);
          lcd.write(byte(1));
        }
      }
      // Roll a fresh random speed for the next frame movement sequence
      speed = random(200, 400) * multiplier;
      //Ends a interval and starts a new one.
      start = current;
    }
    //Checks if the position of the dino and one of the cactus is the same. If so, the player lost and end the game.
    for(int i = 0; i < 5; i++){
      if(dinoCol == cactusPos[i] && dinoRow == 1){
        lose = true;
      }
    }

    //Checks if the joystick is pushed up and is not already jumping.
    if(analogRead(0) > 530 && !jump){
      //Puts the cursor at the position of the dino and deletes it.
      lcd.setCursor(dinoCol, 1);
      lcd.print(" ");
      //Changes the dino row to the one above to simulate jumping.
      dinoRow = 0;
      //Puts the cursor at the new position and redraws the dino.
      lcd.setCursor(dinoCol, dinoRow);
      lcd.write(byte(0));
      //Sets jump to true to prevent jumping when already jumping.
      jump = true;
      //Starts a timer for how long the dino is in the air.
      dinoJump = millis();
      //If the dino has been in the air for long enough and was jumping
    }else if(current - dinoJump >= 1200 * multiplier && jump){
      //Deletes the current dino
      lcd.setCursor(dinoCol, dinoRow);
      lcd.print(" ");
      //Updates it row position back to the ground
      dinoRow = 1;
      lcd.setCursor(dinoCol, dinoRow);
      lcd.write(byte(0));
      //Sets jump to false to allow the player to jump again.
      jump = false;
      dinoJump = millis();
    }
    //multiplier += ((millis() - gameBegin) % 5000)/10;
  }
  //Serial.println(multiplier);
}
