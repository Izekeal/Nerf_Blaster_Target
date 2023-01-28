#ifndef Target_h
#define Target_h
class Target {
  public:
    Target(int targetNum, int ltPin, int ledPin, bool active, bool ledActive, bool shotDetect);
    bool shot();
    void LED(bool ledActive);
  private:
    int _ltPin;
    int _ledPin;
	bool _ledActive;
    bool _shotDetect;
};
#endif