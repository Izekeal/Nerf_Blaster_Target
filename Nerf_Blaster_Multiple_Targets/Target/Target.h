#ifndef Target_h
#define Target_h
#include "Arduino.h" // you most likely don't need this here
class Target {
  public:
    Target(int targetNum, int ltPin, int ledPin, bool active, bool ledActive, bool shotDetect);
    bool shot();
    bool ready();
    void ledON(); // these on/off methods could be changed to something like: setLedState(bool state);
    void ledOFF();
  private:
    int _ltPin;
    int _ledPin;
    bool _active;
    bool _shotDetect;
};
#endif
