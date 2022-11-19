//NERF target practice game

#include <LiquidCrystal_I2C.h>

//to-do: find the piezo speaker and hook it up to pin 6
//all piezo code is commented out for now
//#define PIN_PIEZO 6
#define PIN_RESET 8

#define PIN_LED 5
#define PIN_LINE_TRACKER 4

//16 segment, 2 row LCD display, address = 0x27
LiquidCrystal_I2C lcd(0x27,16,2);

bool gameRunning;
bool targetReady;
bool lcdClear;
int gameTimer;
int gameScore;
unsigned int timerCheck;
unsigned long timerStart;

void setup() {
  Serial.begin(9600);

  //Initialize the LCD, turn on its backlight
  lcd.init();
  lcd.backlight();

  //Initialize the LED and the Line Tracker
  //pinMode(PIN_PIEZO, OUTPUT);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_LINE_TRACKER, INPUT);
  digitalWrite(PIN_LED, LOW);

  pinMode(PIN_RESET, INPUT_PULLUP);

  gameScore = 0;
  gameTimer = 45;
  
  gameBegin();

}

void loop() {
  if(gameRunning){
    updateLCD();
    updateScore();
    updateTimer();
  }
  if (digitalRead(PIN_RESET) == LOW){
    gameBegin();
  }

}

void updateTimer(){
  if (millis() - timerStart > 1000){
    gameTimer -= 1;
    //tone(PIN_PIEZO, 400, 50);
    timerStart = millis();
  }
  //This clears an extra 0 from the screen that is leftover from a two-digit number
  if ((gameTimer == 9) && (lcdClear == false)){
    lcd.clear();
    lcdClear = true;
  }
  if (gameTimer < 1){
    gameEnd();
  }
}


//Update the score only if the target has been hit from a neutral position
//Using targetReady prevents the score from being increased indefinitely while a LOW state is detected

//to-do: consider adding a "cooldown" period to prevent the score from going up if the sensor picks
//up any erratic LOW/HIGH behaviour.
void updateScore(){
  if ((digitalRead(PIN_LINE_TRACKER) == LOW) && (targetReady == true)){
    gameScore += 1;
    targetReady = false;
    digitalWrite(PIN_LED,HIGH);
  }
  
  if (digitalRead(PIN_LINE_TRACKER) == HIGH){
    targetReady = true;
    digitalWrite(PIN_LED,LOW);
  }
}

void updateLCD(){
  lcd.setCursor(1,0);
  lcd.print("Timer: ");
  lcd.print(gameTimer);
  lcd.setCursor(1,1);
  lcd.print("Score: ");
  lcd.print(gameScore);
}

void gameEnd(){
  lcd.clear();
  gameRunning = false;
  lcd.setCursor(4,0);
  lcd.print("GAME OVER");
  lcd.setCursor(0,1);
  lcd.print("Final Score: ");
  lcd.print(gameScore);
  digitalWrite(PIN_LED,HIGH);
  gameScore = 0;
  gameTimer = 45;
}

//Display the game start screen
//to-do: Add a countdown timer from 5 seconds to the for loop, maybe an animation for fun
void gameBegin(){
  digitalWrite(PIN_LED,HIGH);
  lcd.setCursor(1,0);
  lcd.print("   NERF TEST");
  lcd.setCursor(0,1);
  lcd.print(" * * * * * * * ");

  for (byte i = 0; i < 9; i++) {
    delay(250);
    //tone(PIN_PIEZO, 1220 - (i%2 * 50));
    delay(300);
    //noTone(PIN_PIEZO);
  }
  
  lcd.clear();
  //The game begins when the LED turns off
  digitalWrite(PIN_LED,LOW);
  timerStart = millis();
  gameRunning = true;
  targetReady = true;
  lcdClear = false;

}