#define ledPin 9
boolean start = false;
unsigned long startMillis;
unsigned long currentMillis;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
}


void loop() {
  // put your main code here, to run repeatedly:
  //blink(5000, 2000);
  //gradual(10000);
  digitalWrite(ledPin, HIGH);
 
}




void blink(int durationLight, int durationDark){


  if(!start){
    startMillis = millis();
    currentMillis = millis();
    start = true;
  }


  currentMillis = millis();


  if(currentMillis - startMillis >= durationDark){
    digitalWrite(ledPin,HIGH);
    if(currentMillis - startMillis >= durationLight){
      digitalWrite(ledPin,LOW);
      startMillis = currentMillis;
    }
  }
}
  boolean restart;
void gradual(int durationLight){


  if(!start){
    startMillis = millis();
    currentMillis = millis();
    start = true;
    restart = false;
  }


  currentMillis = millis();


  if(restart == false){
    analogWrite(ledPin, (int)((currentMillis-startMillis)/(double)durationLight * 255));
  }else{
    analogWrite(ledPin, 255-(int)((currentMillis-startMillis)/(double)durationLight * 255));
  }
  if((int)((currentMillis-startMillis)/(double)durationLight * 255)>= 255 && !restart){
     restart = true;
     startMillis = currentMillis;
  }else if(255-(int)((currentMillis-startMillis)/(double)durationLight * 255) <= 0 && restart){
     restart = false;
     startMillis = currentMillis;
  }
 


}

