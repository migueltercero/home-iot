#ifndef Logger_H
#define Logger_H

#include <Homie.h>

void log(String logger, String text) { Homie.getLogger() << "[" << logger << "] " << (const char*)text.c_str() << endl; }

#endif;