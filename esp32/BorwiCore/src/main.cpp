#include <WiFi.h>
#include "configuration.h"
#include "state.h"

#include "display.h"
#include "contract.h"
#include "dispenser.h"
#include "buttons.h"
#include "websocket.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <QRcode_ST7735.h>

#define LED_PIN 26

Adafruit_ST7735 tft = Adafruit_ST7735(5, 2, 4); // CS, DC, RST
QRcode_ST7735 qrcode(&tft);

String items[8];
String prices[8];
int itemCount = 0;
int selectedIndex = 0;
bool wifiOK = false;
bool itemsOK = false;

void setup()
{
  Serial.begin(115200);

  initButtons();
  initDispensers();
  initDisplay();
  resetLog();
  pinMode(LED_PIN, OUTPUT);

  logLine("Conectando WiFi...", ST77XX_CYAN);
  WiFi.begin(ssid, password);
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 10000)
    delay(500);

  if (WiFi.status() != WL_CONNECTED)
  {
    logLine("Error: WiFi", ST77XX_RED);
    return;
  }

  logLine("WiFi conectado", ST77XX_GREEN);
  delay(300);

  logLine("Cargando productos...", ST77XX_CYAN);
  if (!loadItems(items, prices, itemCount) || itemCount == 0)
  {
    logLine("Error al cargar productos", ST77XX_RED);
    return;
  }

  logLine("Productos cargados: " + String(itemCount), ST77XX_GREEN);
  delay(1000);
  logLine("Iniciando WebSocket...", ST77XX_CYAN);
  delay(1000);
  initWebSocket();
  delay(1000);
  logLine("WebSocket listo", ST77XX_GREEN);
  delay(1000);
  logLine("Iniciando...", ST77XX_ORANGE);
  delay(1000);
  digitalWrite(LED_PIN, HIGH);
  drawMenu(items, prices, selectedIndex, itemCount);
}

void loop()
{
  updateWebSocket();

  if (itemCount == 0)
    return;

  ButtonAction action = readButton();

  if (isAwaitingPayment())
  {
    // Cancelar y volver al menú
    if (action == MOVE_DOWN)
    {
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
}
