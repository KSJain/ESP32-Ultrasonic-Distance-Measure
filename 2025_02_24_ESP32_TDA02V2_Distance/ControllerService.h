#ifndef ControllerService_h
#define ControllerService_h

#include "UltrasonicSensor.h"

class ControllerService {
public:
  ControllerService(int triggerPin, int echoPin);
  void begin();
  void tick();

private:
  UltrasonicSensor *_ultrasonicSensor;
};

#endif
