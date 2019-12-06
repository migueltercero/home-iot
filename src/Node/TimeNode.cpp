#ifndef NTPNode_H
#define NTPNode_H

#include "Homie.h"
#include "Node.cpp"
#include "TimeLib.h"
#include "TimedAction.h"
#include <NTPClient.h>
#include <WiFiUdp.h>

class TimeNode : public Node<TimeNode> {
private:
  WiFiUDP ntpUDP;
  NTPClient* ntpClient;

  const char* time = "time";
  HomieSetting<bool> enable;

public:
  TimeNode() : Node("time", "Time Node", "String"), enable("ntp-enabled", "Sincronize time with NTP?") {
    ntpClient = new NTPClient(ntpUDP);
  }

protected:
  void setup() {
    Node::setup();

    this->advertise(time).setName("Time").setDatatype("string");

    ntpClient->setTimeOffset(3600);
    ntpClient->setUpdateInterval(60 * 60 * 1000);
    ntpClient->begin();

    enable.setDefaultValue(false);
  }

  void loop() {
    if (enable.get() && Homie.isConnected()) {
      Node::loop();
      ntpClient->update();
    }
  }

  void onReadyToOperate() {
    if (enable.get()) {
      Node::onReadyToOperate();
      setTime(ntpClient->getEpochTime());
      send(time, log.getFormattedTime());
    }
  }
};

#endif
