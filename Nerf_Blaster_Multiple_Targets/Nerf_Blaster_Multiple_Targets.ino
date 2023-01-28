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
//NEW: Temporary fix for Target C misbehaving is to increase the array size by 1 and have a "dummy" target at position 3 in the array
//To-do: Find out why this fixes the bug
bool activeTarget[3];
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
void updateScore(){
  if (activeTarget[0] == true){
    a.LED(true);
    if (a.shot() == true){
      gameScore ++;
      activeTarget[0] = false;
      a.LED(false);
    }
  }
  if (activeTarget[1] == true){
    b.LED(true);
    if (b.shot() == true){
      gameScore ++;
      activeTarget[1] = false;
      b.LED(false);
    }
  }
  if (activeTarget[2] == true){
    c.LED(true);
    if (c.shot() == true){
      //to-do: Find out why this resets gameScore to zero on a successful shot instead of incrementing it,
      //and also sets the score to a static value of one when this target becomes active
      //Theories: Pin 11 on Arduino is bad, connector on breadboard is bad, using two jumper cables and the breadboard as an inbetween is bad
      gameScore ++;
      activeTarget[2] = false;
      c.LED(false);
    }
  }
}

//Wait a random amount of time and then make a new target active, light up its corresponding LED to show the player which target is active
//Tell the game which targets are active using activeTarget array so it can monitor those targets in updateScore
void newTarget(){
  //Activate a new target after a delay of 1.25 seconds.  Eventually this will be a random amount of time
  if (millis() - targetDelay > 1250){
    targetDelay = millis();
    //Pick a random target, choices are 0, 1, 2
    getTarget = random(0,3);
    //Would be nice to pass this value to the Target class instead of using a separate boolean array
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
  a.LED(true);
  b.LED(true);
  c.LED(true);
}

//Display the game start screen
//to-do: Add a countdown timer from 5 seconds to the for loop, maybe an animation for fun
//with some beeps from the Piezo speaker for each second of the countdown, LEDs could blink as well
void gameBegin(){
  a.LED(true);
  b.LED(true);
  c.LED(true);
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
  
  for (byte i = 0; i < 3; i++){
    activeTarget[i] = false;
  }
  lcd.clear();
  //The game begins when the LEDs turn off
  a.LED(false);
  b.LED(false);
  c.LED(false);
  timerStart = millis();
  targetDelay = millis();
  gameRunning = true;
  lcdClear = false;
  gameScore = 0;
  gameTimer = 45;
}