#include <Arduino.h>
#include "led.h"

#define LED_PIN 26
static bool ledOn = false;

void initLED()
{
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
}

void turnOnLED()
{
    digitalWrite(LED_PIN, HIGH);
    ledOn = true;
}

void turnOffLED()
{
    digitalWrite(LED_PIN, LOW);
    ledOn = false;
}

bool isLEDOn()
{
    return ledOn;
}