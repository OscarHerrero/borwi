#include <Arduino.h>
#include "utils.h"
#include "websocket.h"
#include "screensaver.h"

String pad64(String hex)
{
    while (hex.length() < 64)
        hex = "0" + hex;
    return hex;
}

String hexToAscii(const String &hex)
{
    String result = "";
    for (unsigned int i = 0; i < hex.length(); i += 2)
    {
        String byteStr = hex.substring(i, i + 2);
        char c = (char)strtol(byteStr.c_str(), nullptr, 16);
        if (c != 0)
            result += c;
    }
    return result;
}

String weiToEth(const String &hexWei)
{
    String cleanHex = hexWei;
    cleanHex.replace("0x", "");
    double wei = strtoull(cleanHex.c_str(), nullptr, 16);
    double eth = wei / 1e18;
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "%.4f ETH", eth);
    return String(buffer);
}

void waitNonBlocking(unsigned long durationMs)
{
    unsigned long start = millis();
    while (millis() - start < durationMs)
    {
        updateWebSocket();   // sigue recibiendo eventos
        resetScreensaverTimer(); // resetea el temporizador del salvapantallas
        yield();             // cede tiempo a otras tareas
    }
}
