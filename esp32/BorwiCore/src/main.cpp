#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <QRcode_ST7735.h>
#include "configuration.h"
#include "state.h"
#include "utils.h"
#include "display.h"
#include "contract.h"
#include "dispenser.h"
#include "buttons.h"
#include "websocket.h"
#include "screensaver.h"

Adafruit_ST7735 tft = Adafruit_ST7735(5, 2, 4); // CS, DC, RST
QRcode_ST7735 qrcode(&tft);

String items[8];
String prices[8];
int itemCount = 0;
int selectedIndex = 0;

void setup()
{
  Serial.begin(115200);

  initButtons();
  initDispensers();
  initDisplay();
  resetLog();

  logLine("Connecting WiFi...", ST77XX_CYAN);
  WiFi.begin(ssid, password);
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 10000)
    delay(500);

  if (WiFi.status() != WL_CONNECTED)
  {
    logLine("Error: WiFi", ST77XX_RED);
    return;
  }

  logLine("WiFi connected", ST77XX_GREEN);
  delay(300);

  logLine("Searching products...", ST77XX_CYAN);
  if (!loadItems(items, prices, itemCount) || itemCount == 0)
  {
    logLine("Error al cargar productos", ST77XX_RED);
    return;
  }
  delay(500);
  logLine("Products loaded: " + String(itemCount), ST77XX_GREEN);
  delay(1000);
  logLine("Starting WebSocket...", ST77XX_CYAN);
  delay(1000);
  initWebSocket();
  waitNonBlocking(1000);
  logLine("WebSocket ready", ST77XX_GREEN);
  waitNonBlocking(1000);
  logLine("Initializing screen...", ST77XX_CYAN);
  waitNonBlocking(1000);
  initScreensaver();
  drawMenu(items, prices, selectedIndex, itemCount);
}

void loop()
{
  updateWebSocket();
  updateScreensaver();

  if (itemCount == 0)
    return;

  ButtonAction action = readButton();

  if (isScreensaverActive())
  {
    if (readButton() != NONE)
    {
      resetScreensaverTimer();
      drawMenu(items, prices, selectedIndex, itemCount);
    }
    return;
  }

  if (isAwaitingPayment())
  {
    // Cancelar y volver al menú
    if (action == MOVE_DOWN || action == SELECT_OK || action == MOVE_UP)
    {
      showMessage("Operation canceled...", ST77XX_RED, 1);
      waitNonBlocking(2000);
      setAwaitingPayment(false);
      drawMenu(items, prices, selectedIndex, itemCount);
    }
    return;
  }

  switch (action)
  {
  case MOVE_UP:
    selectedIndex = (selectedIndex - 1 + itemCount) % itemCount;
    drawMenu(items, prices, selectedIndex, itemCount);
    break;
  case MOVE_DOWN:
    selectedIndex = (selectedIndex + 1) % itemCount;
    drawMenu(items, prices, selectedIndex, itemCount);
    break;
  case SELECT_OK:
  {
    String url = "ethereum:" + String(contractAddress) + "?value=" + prices[selectedIndex];
    showQR(url);
    setAwaitingPayment(true);
    break;
  }
  default:
    break;
  }

  if (action != NONE)
  {
    resetScreensaverTimer();
  }
}
