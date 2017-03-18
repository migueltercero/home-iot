#ifndef Light_H
#define Light_H

class Light {
  int _pin;

  public:
    Light(int pin);

    void on();
    void off();

    bool isActive();
};

#endif
