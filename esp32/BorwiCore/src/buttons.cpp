#include "buttons.h"
#include "utils.h"

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
        waitNonBlocking(200);
        return MOVE_UP;
    }

    if (digitalRead(BUTTON_DOWN) == LOW)
    {
        waitNonBlocking(200);
        return MOVE_DOWN;
    }

    if (digitalRead(BUTTON_OK) == LOW)
    {
        waitNonBlocking(200);
        return SELECT_OK;
    }

    return NONE;
}
