#include "TimedAction.h"

/*
|| <<constructor>>
*/
TimedAction::TimedAction(unsigned long intervl, CallbackTimer function){
    active = true;
	previous = 0;
	interval = intervl;
	execute = function;
}

/*
|| <<constructor>>
*/
TimedAction::TimedAction(unsigned long prev,unsigned long intervl, CallbackTimer function){
    active = true;
	previous = prev;
	interval = intervl;
	execute = function;
}

void TimedAction::reset(){
    previous = millis();
}

void TimedAction::disable(){
    active = false;
}

void TimedAction::enable(){
  reset();
	active = true;
}

bool TimedAction::isEnabled(){
	return active;
}

void TimedAction::check(){
  if ( active && (millis()-previous >= interval) ) {
    previous = millis();
    execute();
  }
}

void TimedAction::setInterval( unsigned long intervl){
	interval = intervl;
}
