#include "Node/NodeFactory.cpp"
#include <Homie.h>

#define FW_NAME "home-iot"
#define FW_VERSION "2.0.0"

#define GPIO_LED 16

/* Magic sequence for Autodetectable Binary Upload */
const char* __FLAGGED_FW_NAME = "\xbf\x84\xe4\x13\x54" FW_NAME "\x93\x44\x6b\xa7\x75";
const char* __FLAGGED_FW_VERSION = "\x6a\x3f\x3e\x0e\xe1" FW_VERSION "\xb0\x30\x48\xd4\x1a";
/* End of magic sequence for Autodetectable Binary Upload */

std::vector<HomieNode*> nodes;
NodeFactory* factory = new NodeFactory;

HomieNode* ntpNode = new NTPNode();
HomieNode* rebootNode = new RebootNode();

HomieSetting<long> controllerCount("controllerCount", "Number of Nodes");
HomieSetting<const char*> controller0("controller0", "rollershutter,4,5,12,13,24");
HomieSetting<const char*> controller1("controller1", "light,4,12");
HomieSetting<const char*> controller2("controller2", "node");
HomieSetting<const char*> controller3("controller3", "node");

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

    for (int i = 0; i < atoi(parsedJson["settings"]["controllerCount"]); i++) {
      String conf = String("controller" + String(i));
      nodes.push_back(factory->createNode(i + 1, parsedJson["settings"][conf.c_str()]));
    }
  }
}

void setup() {
  Serial.begin(115200);

  controller0.setDefaultValue("none");
  controller1.setDefaultValue("none");
  controller2.setDefaultValue("none");
  controller3.setDefaultValue("none");

  Homie.setLedPin(GPIO_LED, HIGH);
  Homie_setFirmware(FW_NAME, FW_VERSION);
  Homie.setResetTrigger(0, LOW, 1000);
  setupHandler();
  // Homie.loadSettings(setupHandler);
  // Homie.disableLogging();
  Homie.setup();
}

void loop() { Homie.loop(); }
