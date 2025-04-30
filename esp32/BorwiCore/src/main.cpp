// VendingMachine ESP32 + TFT + WebSocket
// Pantalla TFT ST7735, JSON-RPC por WebSocket, botones físicos

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <WebSocketsClient.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <QRcode_ST7735.h>
#include "configuration.h" // ssid, password, ws_server, ws_port, contractAddress
#include "helpers.h"       // hexToAscii, pad64, weiToEth

#define TFT_CS 5
#define TFT_RST 4
#define TFT_DC 2
#define TFTMODEL INITR_BLACKTAB

#define BUTTON_UP 32
#define BUTTON_DOWN 33
#define BUTTON_OK 25

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
QRcode_ST7735 qrcode(&tft);

WebSocketsClient webSocket;
HTTPClient http;

String items[8];
String prices[8];
int itemCount = 0;
int selectedIndex = 0;
bool awaitingPayment = false;

void drawMenu()
{
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(1);

  // Flecha arriba (si no estamos en el primero)
  if (selectedIndex > 0)
  {
    tft.setCursor(60, 5);
    tft.print("^");
  }

  // Mostrar nombre del ítem
  tft.setTextSize(2);
  tft.setCursor(10, 30);
  tft.print(items[selectedIndex]);

  // Mostrar precio
  tft.setTextSize(1);
  tft.setCursor(10, 60);
  tft.print("Precio:");

  tft.setCursor(10, 72);
  tft.print(weiToEth(prices[selectedIndex]));

  // Flecha abajo (si hay más)
  if (selectedIndex < itemCount - 1)
  {
    tft.setCursor(60, 110);
    tft.print("v");
  }

  // Texto inferior
  tft.setCursor(10, 120);
  tft.setTextColor(ST77XX_BLUE);
  tft.print("OK = QR de pago");
}

void showQRForSelected()
{
  tft.fillScreen(ST77XX_BLACK);
  String ethUrl = String("ethereum:") + contractAddress + "?value=" + prices[selectedIndex];
  Serial.println("Mostrando QR para item seleccionado:");
  Serial.println(String("ethereum:") + contractAddress + "?value=" + prices[selectedIndex]);

  qrcode.create(ethUrl);
  awaitingPayment = true;
}

void drawConfirmationScreen(const char *message)
{
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(10, 40);
  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(2);
  tft.println(message);
  delay(3000);
  drawMenu();
  awaitingPayment = false;
}

void loadItemsFromContract()
{
  Serial.println("🔄 Iniciando carga de productos...");

  WiFiClientSecure secureClient;
  secureClient.setInsecure();
  http.begin(secureClient, String("https://") + ws_server);
  http.addHeader("Content-Type", "application/json");

  String payload = String("{\"jsonrpc\":\"2.0\",\"method\":\"eth_call\",\"params\":[{\"to\":\"") +
                   contractAddress + "\",\"data\":\"0x7749cf23\"},\"latest\"],\"id\":1}";

  Serial.println("➡️ Enviando eth_call para getItemCount...");
  Serial.println("Payload: " + payload);

  int code = http.POST(payload);
  Serial.printf("Código HTTP: %d\n", code);

  if (code > 0)
  {
    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, http.getString());

    if (!err && doc["result"])
    {
      String result = doc["result"];
      Serial.println("Resultado crudo (hex): " + result);
      result.replace("0x", "");
      itemCount = strtol(result.c_str(), NULL, 16);
      Serial.printf("✅ Productos detectados: %d\n", itemCount);
      if (itemCount > 8)
        itemCount = 8; // límite
    }
    else
    {
      Serial.println("❌ Error al parsear respuesta de getItemCount");
    }
  }
  else
  {
    Serial.println("❌ Error HTTP en getItemCount");
  }
  http.end();

  for (int i = 0; i < itemCount; i++)
  {
    Serial.printf("🔹 Cargando producto #%d\n", i);

    String indexPadded = pad64(String(i, HEX));

    // getItem
    String payloadItem = String("{\"jsonrpc\":\"2.0\",\"method\":\"eth_call\",\"params\":[{\"to\":\"") +
                         contractAddress + "\",\"data\":\"0x3129e773" + indexPadded +
                         "\"},\"latest\"],\"id\":1}";

    http.begin(secureClient, String("https://") + ws_server);
    http.addHeader("Content-Type", "application/json");
    http.POST(payloadItem);
    JsonDocument doc1;
    deserializeJson(doc1, http.getString());
    items[i] = "Item" + String(i + 1);
    if (doc1["result"].as<String>() != "0x")
    {
      String hex = doc1["result"].as<String>().substring(130);
      hex.trim();
      items[i] = hexToAscii(hex);
    }
    Serial.println("📦 Nombre: " + items[i]);
    http.end();

    // getItemPrice
    String payloadPrice = String("{\"jsonrpc\":\"2.0\",\"method\":\"eth_call\",\"params\":[{\"to\":\"") +
                          contractAddress + "\",\"data\":\"0x4c4df870" + indexPadded +
                          "\"},\"latest\"],\"id\":1}";

    http.begin(secureClient, String("https://") + ws_server);
    http.addHeader("Content-Type", "application/json");
    http.POST(payloadPrice);
    JsonDocument doc2;
    deserializeJson(doc2, http.getString());
    prices[i] = doc2["result"].as<String>();
    Serial.println("💰 Precio: " + weiToEth(prices[i]));

    http.end();
  }

  Serial.println("✅ Carga finalizada.\nRedibujando menú...");
  drawMenu();
}

void handleIncomingMessage(uint8_t *payload, size_t length)
{
  Serial.println("📨 WebSocket mensaje recibido:");
  Serial.println((char *)payload);

  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, payload, length);

  if (error)
  {
    Serial.println("❌ Error parseando JSON del evento");
    return;
  }

  if (doc["method"] != "eth_subscription")
    return;

  String topic = doc["params"]["result"]["topics"][0];

  if (topic.equalsIgnoreCase(topicPurchase))
  {
    Serial.println("✅ Evento de compra recibido!");

    // Extraer address (comprador)
    String buyer = doc["params"]["result"]["topics"][1];
    buyer.replace("0x000000000000000000000000", "0x");

    // Extraer y parsear datos
    String data = doc["params"]["result"]["data"];
    data.replace("0x", "");

    // Amount (wei): posición 64–95
    String amountHex = data.substring(64 * 2, 64 * 3);
    uint64_t amountWei = strtoull(amountHex.c_str(), nullptr, 16);

    // Longitud del string name: posición 96–127
    String nameLenHex = data.substring(64 * 3, 64 * 4);
    int nameLen = strtol(nameLenHex.c_str(), nullptr, 16);

    // Hex del string name (empezando en byte 128)
    String nameHex = data.substring(64 * 4, 64 * 4 + nameLen * 2);
    String itemName = hexToAscii(nameHex);

    // Mostrar datos en consola
    Serial.println("👤 Comprador: " + buyer);
    Serial.println("📦 Producto: " + itemName);
    Serial.println("💰 Pago recibido: " + weiToEth(String(amountHex)));


    if (awaitingPayment)
    {
      drawConfirmationScreen("Gracias!");
    }
  }
}

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{
  if (type == WStype_CONNECTED)
  {
    Serial.println("✅ WebSocket conectado, suscribiéndose a eventos...");
    String sub = String("{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"eth_subscribe\",\"params\":[\"logs\", {\"address\":\"") +
                 contractAddress + "\", \"topics\": [\"" + topicPurchase + "\"]}]}";
    Serial.println("🛰️ Enviando payload de suscripción:");
    Serial.println(sub);
    webSocket.sendTXT(sub);
  }
  else if (type == WStype_TEXT)
  {
    Serial.println("📨 WebSocket mensaje recibido:");
    Serial.println((char *)payload);
    handleIncomingMessage(payload, length);
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_OK, INPUT_PULLUP);

  tft.initR(TFTMODEL);
  tft.setRotation(1);
  qrcode.init();

  Serial.println("Iniciando pantalla y conexión WiFi...");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
    delay(500);

  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(10, 30);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(1);

  Serial.println("WiFi conectado correctamente");
  Serial.println("Cargando productos desde el contrato...");

  tft.println("Cargando productos...");

  loadItemsFromContract();

  if (itemCount == 0)
  {
    tft.fillScreen(ST77XX_BLACK);
    tft.setCursor(10, 40);
    tft.setTextColor(ST77XX_RED);
    tft.setTextSize(1);
    tft.println("Error al cargar items");
  }

  webSocket.beginSSL(ws_server, ws_port);
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000);
}

void loop()
{
  webSocket.loop();

  // No permitir interacción si no hay productos o se espera pago
  if (itemCount == 0 || awaitingPayment)
    return;

  if (digitalRead(BUTTON_UP) == LOW)
  {
    selectedIndex = (selectedIndex - 1 + itemCount) % itemCount;
    drawMenu();
    delay(200);
  }

  if (digitalRead(BUTTON_DOWN) == LOW)
  {
    selectedIndex = (selectedIndex + 1) % itemCount;
    drawMenu();
    delay(200);
  }

  if (digitalRead(BUTTON_OK) == LOW)
  {
    showQRForSelected();
    delay(500);
  }
}
