#include <Wire.h>
#include <LiquidCrystal_I2C.h>


// FIX 1: You must instantiate the lcd object with its I2C address, columns, and rows.
// 0x27 is the most common address for 16x2 displays. Change to 0x3F if 0x27 doesn't work.
LiquidCrystal_I2C lcd(0x27, 16, 2);




// Create a custom pixel pattern (e.g., a smiley face) so byte(0) isn't blank/corrupted
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


int cactusPos[5]{
  random(17, 19),
  random(23, 25),
  random(29, 32),
  random(36, 39),
  random(42, 46)
};


int dinoCol = 3;
int dinoRow = 1;
boolean lose = false;
boolean jump = false;


void setup() {
  Serial.begin(9600);
  //Initializes the display
  lcd.init();
 
  //Turns on the display's background light
  lcd.backlight();
 
  //Uploads your smiley face pixel grid into memory slot 0. The screen can hold
  //8 custom characters total (slots 0 to 7).
  lcd.createChar(0, dino);
  lcd.createChar(1, cactus);
 
  lcd.setCursor(3, 1);
 
  //Prints the custom graphic stored in memory slot 0 (the smiley face).write() is used for
  //raw bytes/graphics instead of text strings.
  lcd.write(byte(0));
  lcd.blink();
}


void loop() {
  static long gameBegin = millis();
  static double multiplier = 2;
  if(!lose){
    static long start = millis();
    static long dinoJump;
    long current = millis();
    static double speed = random(500, 1000) * multiplier;




    if(current - start >= speed){
      for(int i = 0; i < 5; i++){
        lcd.setCursor(cactusPos[i], 1);
        lcd.print(" ");
        cactusPos[i] = cactusPos[i] - 1;
        if(cactusPos[i] < 0){
          cactusPos[i] = random(20, 23);
        }
        lcd.setCursor(cactusPos[i], 1);
        lcd.write(byte(1));
      }
      start = current;
    }


    for(int i = 0; i < 5; i++){
      if(dinoCol == cactusPos[i] && dinoRow == 1){
        lose = true;
      }
    }


    if(analogRead(0) > 530 && !jump){
      lcd.setCursor(dinoCol, 1);
      lcd.print(" ");
      dinoRow = 0;
      lcd.setCursor(dinoCol, dinoRow);
      lcd.write(byte(0));
      jump = true;
      dinoJump = millis();
    }else if(current - dinoJump >= 1500 && jump){
      lcd.setCursor(dinoCol, dinoRow);
      lcd.print(" ");
      dinoRow = 1;
      lcd.setCursor(dinoCol, dinoRow);
      lcd.write(byte(0));
      jump = false;
      dinoJump = millis();
    }
    //multiplier += ((millis() - gameBegin) % 5000)/10;
  }
  //Serial.println(multiplier);
}
