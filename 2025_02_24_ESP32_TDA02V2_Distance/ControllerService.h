#ifndef ControllerService_h
#define ControllerService_h

#include "UltrasonicSensorService.h"

class ControllerService {
public:
  ControllerService(int triggerPin, int echoPin);
  void begin();
  void tick();

private:
  UltrasonicSensorService *_sensor;
};

#endif
