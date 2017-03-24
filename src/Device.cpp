#include <Homie.h>
#include "Controller.h"

#define FW_NAME "home-iot"
#define FW_VERSION "1.2.3"

#define GPIO_LED 16

/* Magic sequence for Autodetectable Binary Upload */
const char *__FLAGGED_FW_NAME = "\xbf\x84\xe4\x13\x54" FW_NAME "\x93\x44\x6b\xa7\x75";
const char *__FLAGGED_FW_VERSION = "\x6a\x3f\x3e\x0e\xe1" FW_VERSION "\xb0\x30\x48\xd4\x1a";
/* End of magic sequence for Autodetectable Binary Upload */

Controller* controllers[4];
ControllerFactory* factory = new ControllerFactory;

HomieSetting<long> controllerCount("controllerCount", "Number of Controllers");
HomieSetting<const char*> controller0("controller0", "Controller");
HomieSetting<const char*> controller1("controller1", "Controller");
HomieSetting<const char*> controller2("controller2", "Controller");
HomieSetting<const char*> controller3("controller3", "Controller");

void setupHandler() {
  SPIFFS.begin();
  if (SPIFFS.exists("/homie/config.json")) {
      File configFile = SPIFFS.open("/homie/config.json", "r");
      char buf[1000];
      size_t configSize = configFile.size();
      configFile.readBytes(buf, configSize);
      configFile.close();
      buf[configSize] = '\0';

      StaticJsonBuffer<1000> jsonBuffer;
      JsonObject& parsedJson = jsonBuffer.parseObject(buf);

      for(int i=0;i<atoi(parsedJson["settings"]["controllerCount"]);i++){
        String id = String("controller"+ String(i));
        Serial.println("config");
        Serial.println(id);
        controllers[i] = factory->createController(id, parsedJson["settings"][id.c_str()]);
      }
  }
}

void loopHandler() {
  for(int i=0;i<controllerCount.get();i++){
    controllers[i]->loop();
  }
}

void setup() {
  Serial.begin(115200);

  // default values
  controllerCount.setDefaultValue(0);
  controller0.setDefaultValue("none");
  controller1.setDefaultValue("none");
  controller2.setDefaultValue("none");
  controller3.setDefaultValue("none");

  setupHandler();

  // homie setup
  Homie.setLedPin(GPIO_LED, HIGH);
  Homie_setFirmware(FW_NAME, FW_VERSION);
  Homie.setResetTrigger(0, LOW, 500);
  //Homie.disableLogging();
  //Homie.setSetupFunction(setupHandler);
  Homie.setLoopFunction(loopHandler);
  Homie.setup();
}

void loop() {
  Homie.loop();
}
