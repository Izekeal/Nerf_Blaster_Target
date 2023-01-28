#include "Arduino.h"
#include "Target.h"
Target::Target(int targetNum, int ltPin, int ledPin, bool active, bool ledActive, bool shotDetect){
  pinMode(ltPin, INPUT);
  pinMode(ledPin, OUTPUT);
  _ltPin = ltPin;
  _ledPin = ledPin;
  _ledActive = ledActive;
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
void Target::reset(){
	_shotDetect = false;
}
void Target::LED(bool ledActive){
  _ledActive = ledActive;
  if (_ledActive == true){
	  digitalWrite(_ledPin, HIGH);
  }
  if (_ledActive == false){
	  digitalWrite(_ledPin, LOW);
  }
}