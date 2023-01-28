#include "Arduino.h"
#include "Target.h"
Target::Target(int targetNum, int ltPin, int ledPin, bool active, bool ledActive, bool shotDetect){
  pinMode(ltPin, INPUT);
  pinMode(ledPin, OUTPUT);
  _ltPin = ltPin;
  _ledPin = ledPin;
  _active = active;
  _shotDetect = shotDetect;
}
bool Target::shot(){
if (digitalRead(_ltPin) == LOW){
  _shotDetect = true;
  }
else{
  _shotDetect = false;
  }
return _shotDetect;
}
bool Target::ready(){
if (digitalRead(_ltPin) == HIGH){
  _active = true;
  return _active;
  }
}
void Target::reset(){
	_shotDetect = false;
}
void Target::ledON(){
digitalWrite(_ledPin, HIGH);
}
void Target::ledOFF(){
digitalWrite(_ledPin, LOW);
}