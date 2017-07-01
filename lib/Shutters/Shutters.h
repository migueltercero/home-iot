#ifndef Shutters_h
#define Shutters_h

#include <Arduino.h>
#include <EEPROM.h>
#include "functional"

extern "C" {
  typedef std::function<void(String)> StatusCallback;
}

namespace ShuttersInternal {
  const int CALIBRATION_LEVELS = 5;
  const int LEVELS = 100;

  const int STATUS_STEPS = 10;

  enum Stop : unsigned char { STOP_NONE, STOP_NEW_LEVEL, STOP_HALT };
  enum Direction : bool { DIRECTION_DOWN, DIRECTION_UP };
  const unsigned char REQUEST_NONE = 255; // Request must be between 0 and 100, so np
  const unsigned char CALIBRATION_NONE = 255; // CALIBRATION_LEVELS should not be 255, np

  const unsigned char FLAG_KNOWN = 0x80;
  const unsigned char MASK_CURRENT_LEVEL = 0x7F;
}

class Shutters {
private:
  unsigned char _currentLevel;
  bool _moving;
  bool _reached;
  unsigned char _targetLevel;
  unsigned char _requestLevel;
  ShuttersInternal::Stop _stopNeeded;
  unsigned long _timeLastLevel;
  ShuttersInternal::Direction _direction;
  unsigned char _calibration;

  unsigned char _eepromPosition;

  unsigned char _maxDownLevel;

  float _delayTotal;
  float _delayOneLevel;
  int _upRollerShutterPin;
  int _downRollerShutterPin;
  void output(uint8_t up, uint8_t down);
  StatusCallback _statusCallback;

  void log(const char* text);
  void log(String text);
  bool savedIsLastLevelKnown();
  void saveLastLevelUnknown();
  unsigned char savedCurrentLevel();
  void saveCurrentLevelAndKnown(unsigned char level);
  void halt();

public:
  Shutters(int upShutterPin, int downShutterPin, float delay_total, unsigned char maxDownLevel, unsigned char eeprom_offset = 0);
  void attachStatus(StatusCallback function);
  bool begin();
  void loop();
  void requestLevel(unsigned char level);
  void stop();
  bool moving();
  bool reached();
  unsigned char currentLevel();
  void eraseSavedState();
  void up();
  void down();
  void reset();
};

#endif
