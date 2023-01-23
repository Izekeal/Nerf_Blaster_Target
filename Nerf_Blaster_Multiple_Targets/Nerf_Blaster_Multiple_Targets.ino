//NERF target practice game

#include <LiquidCrystal_I2C.h>
#include <Target.h>


//to-do: find the piezo speaker and hook it up to a PWM pin
//all piezo code is commented out for now
//#define PIN_PIEZO 6

#define PIN_RESET 8

//Set up three targets
//TargetID, Line Tracker Pin, LED Pin, Is the target active, is the target's LED active, has the target detected a shot
//To:do - find out if the boolean values are needed
Target a(0,4,5,false,false,false);
Target b(1,10,6,false,false,false);
Target c(2,11,7,false,false,false);

//16 segment, 2 row LCD display, address = 0x27
LiquidCrystal_I2C lcd(0x27,16,2);

//To:do - check each variable here, determine which are no longer needed
bool gameRunning;
bool targetReady;
bool lcdClear;
int gameTimer;
int gameScore;
int activeTargets;
unsigned int targetCheck;
unsigned long timerStart;

void setup() {
  Serial.begin(9600);

  //Initialize the LCD, turn on its backlight
  lcd.init();
  lcd.backlight();

  //Initialize the Piezo Speaker
  //pinMode(PIN_PIEZO, OUTPUT);

  pinMode(PIN_RESET, INPUT_PULLUP);

  gameBegin();
}

void loop() {
  if(gameRunning){
    updateLCD();
    //newTarget();
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


//To:do - Update this function and add comments as I go
void updateScore(){
  if ((a.shot() == true) && (targetReady == true)){
    gameScore += 1;
    targetReady = false;
    targetCheck = millis();
    a.ledON();
  }
  
  if ((millis() - targetCheck > 500) && (a.ready() == true)){
    targetReady = true;
    a.ledOFF();
  }
}

//Wait a random amount of time and then make a new target active, light up its corresponding LED to show the player which target is active
//Tell the game which targets are active so it can monitor those targets in updateScore
//void newTarget(){
//  if (activeTargets < 3){
//    
//  }
//}
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
  a.ledON();
}

//Display the game start screen
//to-do: Add a countdown timer from 5 seconds to the for loop, maybe an animation for fun
//with some beeps from the Piezo speaker for each second of the countdown, LEDs could blink as well
void gameBegin(){
  a.ledON();
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
  //The game begins when the LEDs turn off
  a.ledOFF();
  timerStart = millis();
  gameRunning = true;
  targetReady = true;
  lcdClear = false;
  gameScore = 0;
  gameTimer = 45;
  activeTargets = 0;
}