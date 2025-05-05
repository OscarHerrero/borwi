#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>

enum ButtonAction
{
    NONE,
    MOVE_UP,
    MOVE_DOWN,
    SELECT_OK
};

void initButtons();
ButtonAction readButton();

#endif
