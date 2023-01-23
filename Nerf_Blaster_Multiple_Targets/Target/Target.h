#ifndef Target_h
#define Target_h
#include "Arduino.h"
class Target {
  public:
    Target(int targetNum, int ltPin, int ledPin, bool active, bool ledActive, bool shotDetect);
    bool shot();
    bool ready();
    void ledON();
    void ledOFF();
  private:
    int _ltPin;
    int _ledPin;
    bool _active;
    bool _shotDetect;
};
#endif