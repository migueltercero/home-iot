#ifndef Controller_Factory_H
#define Controller_Factory_H

#include "Node.cpp"
#include "LightNode.cpp"
#include "RollershutterNode.cpp"


using namespace std;

class NodeFactory {
public:
  NodeFactory(){}
  Node *createController(const int id, const char *conf) {
    vector<string> cfg;
    split(conf, ",", cfg);
    if (cfg.size() > 1) {
      string controller = cfg[0];
      if (controller.compare("light") == 0) {
        return new LightNode(id, atoi(cfg[1].c_str()),
                                   atoi(cfg[2].c_str()));
      }
      // if(controller.compare("light-timer") == 0){
      // return new LightTimerController(id, atoi(cfg[1].c_str()),
      // atoi(cfg[2].c_str()), atoi(cfg[3].c_str())); return;
      //}
      if (controller.compare("rollershutter") == 0) {
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