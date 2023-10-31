#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <stdbool.h>

void joystick_init();

bool joystick_getTriggered();

float joystick_getX();
float joystick_getY();

#endif
