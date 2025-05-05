#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <QRcode_ST7735.h> // ✅ Falta esto si usas qrcode

extern Adafruit_ST7735 tft;
extern QRcode_ST7735 qrcode; // ✅ Esto estaba faltando

void initDisplay();
void drawMenu(const String items[], const String prices[], int selectedIndex, int itemCount);
void showQR(const String &url);
void showMessage(const String &msg, uint16_t color = 0xFFFF, uint8_t size = 2);
void logLine(const String &msg, uint16_t color = ST77XX_WHITE);
void resetLog();
void drawHeaderStatus(bool wifi, bool ws, bool items);

#endif
