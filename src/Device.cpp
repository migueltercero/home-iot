#include <Homie.h>
#include "ControllerFactory.cpp"
#include "TimedAction.h"

#define FW_NAME "home-iot"
#define FW_VERSION "2.0.0"

#define GPIO_LED 16

/* Magic sequence for Autodetectable Binary Upload */
const char *__FLAGGED_FW_NAME =
    "\xbf\x84\xe4\x13\x54" FW_NAME "\x93\x44\x6b\xa7\x75";
const char *__FLAGGED_FW_VERSION =
    "\x6a\x3f\x3e\x0e\xe1" FW_VERSION "\xb0\x30\x48\xd4\x1a";
/* End of magic sequence for Autodetectable Binary Upload */

TimedAction *timer;

Controller *controllers[4];
ControllerFactory *factory = new ControllerFactory;

HomieSetting<bool> reboot("reboot", "¿reinicio periodico?");
HomieSetting<long> rebootTime("rebootTime",
                              "Frecuencia en horas de cada reinicio");
HomieSetting<long> controllerCount("controllerCount", "Number of Controllers");
HomieSetting<const char *> controller0("controller0",
                                       "rollershutter,4,5,12,13,24");
HomieSetting<const char *> controller1("controller1", "light,4,12");
HomieSetting<const char *> controller2("controller2", "Controller");
HomieSetting<const char *> controller3("controller3", "Controller");

void log(String text) { Homie.getLogger() << text << endl; }

void preSetupHandler() {
  SPIFFS.begin();
  if (SPIFFS.exists("/homie/config.json")) {
    File configFile = SPIFFS.open("/homie/config.json", "r");
    char buf[1000];
    size_t configSize = configFile.size();
    configFile.readBytes(buf, configSize);
    configFile.close();
    buf[configSize] = '\0';

    StaticJsonBuffer<1000> jsonBuffer;
    JsonObject &parsedJson = jsonBuffer.parseObject(buf);

    for (int i = 0; i < atoi(parsedJson["settings"]["controllerCount"]); i++) {
      String conf = String("controller" + String(i));
      controllers[i] = factory->createController(
          i + 1, parsedJson["settings"][conf.c_str()]);
    }
  }
}

void loopHandler() {
  for (int i = 0; i < controllerCount.get(); i++) {
    controllers[i]->loop();
  }

  if (reboot.get()) {
    timer->check();
  }
}

void timerHandler() {
  log("reboot");
  Homie.reboot();
}

void onHomieEvent(const HomieEvent &event) {
  switch (event.type) {
  case HomieEventType::NORMAL_MODE:
    if (reboot.get()) {
      // timer
      timer = new TimedAction(rebootTime.get() * 60 * 60 * 1000, timerHandler);
    }
    break;
 case HomieEventType::MQTT_READY:
    for (int i = 0; i < controllerCount.get(); i++) {
      controllers[i]->updateStatus();
    }
    break;
  default:
    break;
  }
}

void setup() {
  Serial.begin(115200);

  //Homie.reset();
  
  Homie.setLedPin(GPIO_LED, HIGH);
  Homie_setFirmware(FW_NAME, FW_VERSION);
  Homie.setResetTrigger(0, LOW, 500);

  // default values
  reboot.setDefaultValue(true);
  rebootTime.setDefaultValue(24);
  controllerCount.setDefaultValue(0);
  controller0.setDefaultValue("none");
  controller1.setDefaultValue("none");
  controller2.setDefaultValue("none");
  controller3.setDefaultValue("none");

  preSetupHandler();
  //Homie.loadSettings();
  Homie.setLoopFunction(loopHandler);

  Homie.onEvent(onHomieEvent);
  // Homie.disableLogging();
  // Homie.setLoggingPrinter(logger.);
  
  
  Homie.setup();
}

void loop() { 
  Homie.loop(); 
}
