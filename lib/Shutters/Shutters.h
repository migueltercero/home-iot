#ifndef Shutters_h
#define Shutters_h

#include <Arduino.h>
#include <functional>

extern "C" {
  typedef std::function<void(void)> callback;
  typedef std::function<uint8_t(void)> getValueCallback;
  typedef std::function<void(uint8_t)> setValueCallback;
}

namespace ShuttersInternal {
  const uint16_t SAFETY_DELAY = 1 * 1000;
  const uint8_t LEVELS = 100;

  const uint8_t UP_LEVEL = 0;
  const uint8_t DOWN_LEVEL = 100;

  enum State : uint8_t {
    STATE_IDLE, // not moving
    STATE_RESETTING, // when state not known, goes to 0
    STATE_TARGETING, // when going to target
    STATE_NORMALIZING, // when target changed, goes to next known level
    STATE_CALIBRATING // when 0 or 100, to ensure actually at the end
  };
  enum Direction : bool { DIRECTION_DOWN, DIRECTION_UP };
  const uint8_t LEVEL_NONE = 255; // level must be between 0 and 100, so np
}

class Shutters {
private:
  uint32_t _courseTime;
  float _calibrationRatio = 0.1;
  uint32_t _stepTime;
  uint32_t _calibrationTime;

  ShuttersInternal::State _state;
  uint32_t _stateTime;
  ShuttersInternal::Direction _direction;

  uint8_t _level;

  uint8_t _targetLevel;

  bool _safetyDelay;
  uint32_t _safetyDelayTime;

  bool _reset;

  callback _upCallback;
  callback _downCallback;
  callback _haltCallback;

  getValueCallback _getStateCallback;
  setValueCallback _setStateCallback;

  setValueCallback _onLevelReachedCallback;

  void _up();
  void _down();
  void _halt();
  void _setSafetyDelay();
  void _notifyLevel();
public:
  Shutters(uint32_t courseTime, callback upCallback, callback downCallback, callback haltCallback, getValueCallback getStateCallback, setValueCallback setStateCallback, setValueCallback onLevelReachedCallback, float calibrationRatio);
  void begin();
  void setLevel(uint8_t level);
  void stop();
  void loop();
  bool isIdle();
  uint8_t getCurrentLevel();
  void reset();
};

#endif
