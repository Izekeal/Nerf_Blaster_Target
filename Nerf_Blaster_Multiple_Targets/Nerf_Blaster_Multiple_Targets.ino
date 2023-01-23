//NERF target practice game

#include <LiquidCrystal_I2C.h>
#include <Target.h>


//to-do: find the piezo speaker and hook it up to a PWM pin
//all piezo code is commented out for now
//#define PIN_PIEZO 6

#define PIN_RESET 8

//#define PIN_LED_BLUE 5
//#define PIN_LED_WHITE 6
//#define PIN_LED_GREEN 7
//#define PIN_LINE_TRACKER1 4
//#define PIN_LINE_TRACKER2 10
//#define PIN_LINE_TRACKER3 11


//Set up three targets
//TargetID, Line Tracker Pin, LED Pin, Is the target active, is the target's LED active, has the target detected a shot
Target a(0,4,5,false,false,false);
Target b(1,10,6,false,false,false);
Target c(2,11,7,false,false,false);

//16 segment, 2 row LCD display, address = 0x27
LiquidCrystal_I2C lcd(0x27,16,2);

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

  //Initialize the LED and the Line Trackers
  //pinMode(PIN_PIEZO, OUTPUT);
  //pinMode(PIN_LED_BLUE, OUTPUT);
  //pinMode(PIN_LED_WHITE, OUTPUT);
  //pinMode(PIN_LED_GREEN, OUTPUT);
  //pinMode(PIN_LINE_TRACKER1, INPUT);
  //pinMode(PIN_LINE_TRACKER2, INPUT);
  //pinMode(PIN_LINE_TRACKER3, INPUT);

  pinMode(PIN_RESET, INPUT_PULLUP);

  gameBegin();

}

void loop() {
  if(gameRunning){
    updateLCD();
    newTarget();
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

//updateScore now needs to read something like this:
//a.shot();
//b.shot();
//c.shot();
void updateScore(){
//  if ((digitalRead(PIN_LINE_TRACKER1) == LOW) && (targetReady == true)){
    if (a.shot() == true){
      gameScore += 1;
      targetReady = false;
      targetCheck = millis();
      a.ledON();
  }
  
  if ((millis() - targetCheck > 500) && (a.ready())){
    targetReady = true;
    a.ledOFF();
  }
}

//Wait a random amount of time and then make a new target active, light up its corresponding LED to show the player which target is active
//Tell the game which targets are active so it can monitor those targets in updateScore
void newTarget(){
  if (activeTargets < 3){
    
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
/*
void aLEDon(){
  digitalWrite(PIN_LED_BLUE,HIGH);
  digitalWrite(PIN_LED_WHITE,HIGH);
  digitalWrite(PIN_LED_GREEN,HIGH);
}

void aLEDoff(){
  digitalWrite(PIN_LED_BLUE,LOW);
  digitalWrite(PIN_LED_WHITE,LOW);
  digitalWrite(PIN_LED_GREEN,LOW);
}

void bLEDon(){
  digitalWrite(PIN_LED_BLUE,HIGH);
}

void bLEDoff(){
  digitalWrite(PIN_LED_BLUE,LOW);
}

void wLEDon(){
  digitalWrite(PIN_LED_WHITE,HIGH);
}

void wLEDoff(){
  digitalWrite(PIN_LED_WHITE,LOW);
}

void gLEDon(){
  digitalWrite(PIN_LED_GREEN,HIGH);
}

void gLEDoff(){
  digitalWrite(PIN_LED_GREEN,LOW);
}
*/