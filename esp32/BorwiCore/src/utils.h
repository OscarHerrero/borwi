#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>

String pad64(String hex);
String hexToAscii(const String &hex);
String weiToEth(const String &hexWei);

#endif
