#ifndef TimeController_H
#define TimeController_H

#include "Logger.cpp"
#include "Node.cpp"
#include "TimeLib.h"
#include "TimedAction.h"
#include <NTPClient.h>
#include <WiFiUdp.h>

class NTPNode : public Node {
private:
  WiFiUDP ntpUDP;
  NTPClient* ntpClient;

public:
  NTPNode() : Node("ntp", "NTP Node", "time", "time", "Time", "time", "", "") {}

protected:
  void setup() {
    Node::setup();
    
    ntpClient = new NTPClient(ntpUDP, "europe.pool.ntp.org", 0, 60 * 1000);
    ntpClient->begin();
  }
  void loop() {
    Node::loop();
    ntpClient->update();
  }
  void onReadyToOperate() {
    Node::onReadyToOperate();
    setTime(ntpClient->getEpochTime());
    char formatedTime[20];
    sprintf(formatedTime, "%i:%i:%i %i/%i/%i", hour(), minute(), second(), day(), month(), year());
    send(formatedTime);
  }

  String getLoggerName() { return "NTPNode"; }
};

#endif
