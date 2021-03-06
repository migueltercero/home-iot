#ifndef NodeFactory_H
#define NodeFactory_H

#include "Node.cpp"
#include "LightNode.cpp"
//#include "LEDNode.cpp"
#include "RollershutterNode.cpp"
#include "TimeNode.cpp"
#include "RebootNode.cpp"

using namespace std;

class NodeFactory {
public:
  NodeFactory(){}
  HomieNode *createNode(const int id, const char *conf) {
    vector<string> cfg;
    split(conf, ",", cfg);
    if (cfg.size() > 1) {
      string node = cfg[0];
      if (node.compare("light") == 0) {
        return new LightNode(id, atoi(cfg[1].c_str()),
                                   atoi(cfg[2].c_str()));
      }
      //if (node.compare("led") == 0) {
      //  return new LEDNode(id, atoi(cfg[1].c_str()),
      //                             atoi(cfg[2].c_str()),atoi(cfg[3].c_str()),atoi(cfg[4].c_str()));
      //}
      // if(controller.compare("light-timer") == 0){
      // return new LightTimerController(id, atoi(cfg[1].c_str()),
      // atoi(cfg[2].c_str()), atoi(cfg[3].c_str())); return;
      //}
      if (node.compare("rollershutter") == 0) {
        return new RollershutterNode(
            id, atoi(cfg[1].c_str()), atoi(cfg[2].c_str()),
            atoi(cfg[3].c_str()), atoi(cfg[4].c_str()), atoi(cfg[5].c_str()));
      }
    }
  }

private:
  void split(const string &s, const char *delim, vector<string> &v) {
    // to avoid modifying original string
    // first duplicate the original string and return a char pointer then free
    // the memory
    char *dup = strdup(s.c_str());
    char *token = strtok(dup, delim);
    while (token != NULL) {
      v.push_back(string(token));
      // the call is treated as a subsequent calls to strtok:
      // the function continues from where it left in previous invocation
      token = strtok(NULL, delim);
    }
    free(dup);
  }
};

#endif
