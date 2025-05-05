#ifndef DISPENSER_H
#define DISPENSER_H

#include <Arduino.h>
#include <ESP32Servo.h>

void initDispensers();
void dispenseProduct(int itemIndex);

#endif
