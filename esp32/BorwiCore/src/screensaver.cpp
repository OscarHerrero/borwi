#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include "screensaver.h"
#include "display.h"
#include "websocket.h"
#include "led.h"

extern Adafruit_ST7735 tft;

// Estado del salvapantallas
static unsigned long lastActivity = 0;
static const unsigned long timeout = 120000;
static bool active = false;

// Tiempo para animación
static unsigned long lastFrameTime = 0;
static const unsigned long frameInterval = 30; // ms

// Texto principal que rebota
static float x = 20;
static float y = 30;
static float dx = 1.8;
static float dy = 0.9;
static const int textW = 86;
static const int textH = 34;
static uint16_t currentColor = ST77XX_CYAN;

// Footer rotativo
const char *footerMessages[] = {
    "BASE NETWORK",
    "POWERED BY ETH",
    "ZosiDev Inc.",
    "BORWI CONNECTED",
    "SECURE ONCHAIN"};
const uint8_t footerCount = sizeof(footerMessages) / sizeof(footerMessages[0]);
static uint8_t currentFooterIndex = 0;
static int footerX = 160;
static const int footerY = 120;

void initScreensaver()
{
    initLED();
    turnOnLED();
    resetScreensaverTimer();
}

void resetScreensaverTimer()
{
    lastActivity = millis();
    if (active)
    {
        active = false;
        tft.fillScreen(ST77XX_BLACK);
        if(!isLEDOn())
        {
            turnOnLED();
        }
    }
}

bool isScreensaverActive()
{
    return active;
}

void updateScreensaver()
{
    if (isAwaitingPayment())
        return;
    unsigned long now = millis();

    if (!active && now - lastActivity > timeout)
    {
        active = true;
        tft.fillScreen(ST77XX_BLACK);
        footerX = tft.width();
        lastFrameTime = now;
    }

    if (!active || (now - lastFrameTime < frameInterval))
    {
        return;
    }

    lastFrameTime = now; // actualiza el tiempo del último frame

    // Borrar texto principal
    tft.fillRect((int)x, (int)y, textW, textH, ST77XX_BLACK);

    // Actualizar posición
    x += dx;
    y += dy;

    if (x <= 0 || x + textW >= tft.width())
    {
        dx = -dx;
        currentColor = random(0xFFFF);
    }

    if (y <= 10 || y + textH >= tft.height() - 14)
    {
        dy = -dy;
        currentColor = random(0xFFFF);
    }

    // Dibujar texto principal
    tft.setTextSize(2);
    tft.setTextColor(currentColor);
    tft.setCursor((int)x, (int)y);
    tft.print(" BORWI");
    tft.setCursor((int)x, (int)y + 16);
    tft.print("MACHINE");

    // Footer dinámico
    tft.fillRect(0, 118, tft.width(), 10, ST77XX_BLACK); // limpiar zona

    const char *footerText = footerMessages[currentFooterIndex];
    int textLength = strlen(footerText);
    int footerTextWidth = textLength * 6;

    // Actualizar posición
    footerX -= 1;

    // Si se fue completamente, cambia al siguiente
    if (footerX + footerTextWidth < 0)
    {
        currentFooterIndex = (currentFooterIndex + 1) % footerCount;
        footerText = footerMessages[currentFooterIndex];
        textLength = strlen(footerText);
        footerTextWidth = textLength * 6;
        footerX = tft.width();
    }

    tft.setTextSize(1);
    tft.setTextColor(ST77XX_GREEN);
    tft.setCursor(footerX, footerY);
    tft.print(footerText);

    if (isLEDOn())
    {
        turnOffLED();
    }
}
