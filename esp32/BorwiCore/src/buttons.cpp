#include "buttons.h"

#define BUTTON_UP 32
#define BUTTON_DOWN 33
#define BUTTON_OK 25

void initButtons()
{
    pinMode(BUTTON_UP, INPUT_PULLUP);
    pinMode(BUTTON_DOWN, INPUT_PULLUP);
    pinMode(BUTTON_OK, INPUT_PULLUP);
}

ButtonAction readButton()
{
    if (digitalRead(BUTTON_UP) == LOW)
    {
        delay(200); // simple debounce
        return MOVE_UP;
    }

    if (digitalRead(BUTTON_DOWN) == LOW)
    {
        delay(200);
        return MOVE_DOWN;
    }

    if (digitalRead(BUTTON_OK) == LOW)
    {
        delay(200);
        return SELECT_OK;
    }

    return NONE;
}
