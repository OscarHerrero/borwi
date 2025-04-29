#include <WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>
#include "configuration.h" // Tu archivo con ssid, password, ws_server, ws_port, etc.

// Prototipos de funciones
void webSocketEvent(WStype_t type, uint8_t *payload, size_t length);
void subscribeToLogs();
void handleIncomingMessage(uint8_t *payload, size_t length);

// Variables globales
WebSocketsClient webSocket;

// Topics de los eventos
const char *topicTurnedOn = "0x3091ac1e7403e7adf3fe73b5d60eacfb0cf83921b8587c0b53588a2fca15b816";
const char *topicTurnedOff = "0x29eb1a64e6920f34d7e1070f9ed6a6e372570c9ab6ff1571318dd43259a34707";

// Pin del LED
const int ledPin = 2;

bool subscribed = false;

void setup()
{
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Conectar a WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" ¡Conectado!");

  // Conectar WebSocket
  webSocket.beginSSL(ws_server, ws_port);
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000); // Reconectar cada 5s si se desconecta
}

void loop()
{
  webSocket.loop();
}

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{
  switch (type)
  {
  case WStype_CONNECTED:
    Serial.println("WebSocket conectado!");
    subscribeToLogs();
    break;

  case WStype_DISCONNECTED:
    Serial.println("WebSocket desconectado!");
    subscribed = false;
    break;

  case WStype_TEXT:
    Serial.printf("Mensaje WebSocket: %s\n", payload);
    handleIncomingMessage(payload, length);
    break;

  case WStype_ERROR:
    Serial.println("WebSocket Error!");
    break;
  }
}

void subscribeToLogs()
{
  if (subscribed)
    return;

  String payload = String(R"rawliteral({
    "jsonrpc": "2.0",
    "id": 1,
    "method": "eth_subscribe",
    "params": [
      "logs",
      {
        "address": ")rawliteral") +
                   contractAddress + R"rawliteral("
      }
    ]
  })rawliteral";

  webSocket.sendTXT(payload);
  subscribed = true;
}

void handleIncomingMessage(uint8_t *payload, size_t length)
{
  StaticJsonDocument<8192> doc; // Usamos StaticJsonDocument para evitar warning de ArduinoJson
  DeserializationError error = deserializeJson(doc, payload, length);

  if (error)
  {
    Serial.println("Error de parseo JSON");
    return;
  }

  if (doc["method"].is<const char *>() && String(doc["method"].as<const char *>()) == "eth_subscription")
  {
    String topic = doc["params"]["result"]["topics"][0];

    if (topic.equalsIgnoreCase(topicTurnedOn))
    {
      digitalWrite(ledPin, HIGH);
      Serial.println("LED encendido (evento TurnedOn)");
    }
    else if (topic.equalsIgnoreCase(topicTurnedOff))
    {
      digitalWrite(ledPin, LOW);
      Serial.println("LED apagado (evento TurnedOff)");
    }
  }
}
