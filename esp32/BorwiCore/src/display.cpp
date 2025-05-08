#include "display.h"
#include "utils.h"
#include <QRcode_ST7735.h>
#include "state.h"

#define TFTMODEL INITR_BLACKTAB

extern QRcode_ST7735 qrcode;

static int logY = 0;

void initDisplay()
{
  tft.initR(TFTMODEL);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);
  qrcode.init();
}

void drawMenu(const String items[], const String prices[], int selectedIndex, int itemCount)
{
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextSize(1);

  int y = 10;
  for (int i = 0; i < itemCount; i++)
  {
    tft.setCursor(0, y);

    if (i == selectedIndex)
    {
      tft.setTextColor(ST77XX_GREEN);
      tft.print("> ");
    }
    else
    {
      tft.setTextColor(ST77XX_BLACK);
      tft.print("");
    }

    tft.setTextColor(i == selectedIndex ? ST77XX_BLUE : ST77XX_WHITE);
    tft.print(items[i]);

    tft.setCursor(90, y);
    tft.print(weiToEth(prices[i]));

    y += 18;
  }

  // Instrucciones al pie
  tft.setTextColor(ST77XX_YELLOW);
  tft.setCursor(0, 105);
  tft.print("__________________________");
  tft.setCursor(0, 115);
  tft.print("OK = QR  UP/DOWN = Navegar");
  tft.setCursor(0, 120);
  tft.print("__________________________");
}

void showQR(const String &url)
{
  tft.fillScreen(ST77XX_BLACK);
  qrcode.create(url);

  tft.setTextColor(ST77XX_BLACK);
  tft.setTextSize(1);
  tft.setCursor(2, 115);
  tft.println("PRESS ANY BUTTON TO CANCEL");
}

void showMessage(const String &msg, uint16_t color, uint8_t size)
{
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(color);
  tft.setCursor(10, 40);
  tft.setTextSize(size);
  tft.println(msg);
}

void resetLog()
{
  tft.fillScreen(ST77XX_BLACK);
  logY = 0;
}

void logLine(const String &msg, uint16_t color)
{
  tft.setTextColor(color);
  tft.setCursor(0, logY);
  tft.setTextSize(1);
  tft.print("> ");
  tft.println(msg);
  logY += 10;

  if (logY >= 128)
  {
    resetLog();
    logLine("...continuando log...");
  }
}

