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
bool lcdClear;
int gameTimer;
int gameScore;
//Three targets at the moment, 0,1,2.  This variable == true when a target can be shot at
bool activeTarget[2];
int getTarget;
unsigned long targetDelay;
unsigned long timerStart;

void setup() {
  Serial.begin(9600);

  //Initialize the LCD, turn on its backlight
  lcd.init();
  lcd.backlight();

  //Initialize the Piezo Speaker
  //pinMode(PIN_PIEZO, OUTPUT);

  //Initialize pseudo-random number generator, on an unconnected pin (Analog 9)
  randomSeed(analogRead(9));

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


//To:do - Update this function and add comments as I go
//Can make a separate conditional statement for each target, at least to begin with
//The statement checks to see if the target is "active" and only then will it check to see
//if the target has been shot
//This condition will likely replace the Target.ready() function call
void updateScore(){
  if (activeTarget[0] == true){
    a.ledON();
    if (a.shot() == true){
      gameScore ++;
      activeTarget[0] = false;
      a.ledOFF();
      a.reset();
    }
  }
  if (activeTarget[1] == true){
    b.ledON();
    if (b.shot() == true){
      gameScore ++;
      activeTarget[1] = false;
      b.ledOFF();
      b.reset();
    }
  }
  if (activeTarget[2] == true){
    c.ledON();
    if (c.shot() == true){
      //to-do: Find out why this resets gameScore to zero instead of incrementing it
      //when the other two targets work properly
      gameScore ++;
      activeTarget[2] = false;
      c.ledOFF();
      c.reset();
    }
  }
}

//Wait a random amount of time and then make a new target active, light up its corresponding LED to show the player which target is active
//Tell the game which targets are active using activeTarget array so it can monitor those targets in updateScore
void newTarget(){
  if (millis() - targetDelay > 1250){
    targetDelay = millis();
    //Pick a random number, choices are 0, 1, 2
    getTarget = random(3);
    if (activeTarget[getTarget] == false){
      activeTarget[getTarget] = true;
    }
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
  
  for (byte i = 0; i < 2; i++){
    activeTarget[i] = false;
  }
  lcd.clear();
  //The game begins when the LEDs turn off
  a.ledOFF();
  timerStart = millis();
  targetDelay = millis();
  gameRunning = true;
  lcdClear = false;
  gameScore = 0;
  gameTimer = 45;
}