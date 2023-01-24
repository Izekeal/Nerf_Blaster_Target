#include "Arduino.h" // importing this library here makes more sense as you're making use of it directly in this file
#include "Target.h"
Target::Target(int targetNum, int ltPin, int ledPin, bool active, bool ledActive, bool shotDetect){
  pinMode(ltPin, INPUT);
  pinMode(ledPin, OUTPUT);
  
  // heads up in case you see this elsewhere
  // it is not required (and often left out) but 
  // these can also be referenced using the 'this' keyword 
  // (see: https://stackoverflow.com/questions/6779645/use-of-this-keyword-in-c)
  _ltPin = ltPin;   
  _ledPin = ledPin;
  _active = active;
  _shotDetect = shotDetect;
}
bool Target::shot(){
if ((digitalRead(_ltPin) == LOW) && (_active == true)){
  _shotDetect = true;
  _active = false;
  }
else{
  _shotDetect = false;
  }
return _shotDetect;
}
bool Target::ready(){
// what does this method return if this condition is false?
if (digitalRead(_ltPin) == HIGH){
  _active = true;
  return _active;
  }
}

// these two methods can be combined into one and accept a boolean parameter
void Target::ledON(){
digitalWrite(_ledPin, HIGH);
}
void Target::ledOFF(){
digitalWrite(_ledPin, LOW);
}
