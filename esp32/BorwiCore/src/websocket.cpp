#include "websocket.h"
#include "configuration.h"
#include "utils.h"
#include "display.h"
#include "dispenser.h"


#include <WebSocketsClient.h>
#include <ArduinoJson.h>

static WebSocketsClient webSocket;
static bool awaitingPayment = false;

bool isAwaitingPayment()
{
    return awaitingPayment;
}

void setAwaitingPayment(bool value)
{
    awaitingPayment = value;
}

void handleIncomingMessage(uint8_t *payload, size_t length)
{
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
    if (!topic.equalsIgnoreCase(topicPurchase))
        return;

    Serial.println("✅ Evento de compra recibido!");

    String data = doc["params"]["result"]["data"];
    data.replace("0x", "");

    String indexHex = data.substring(0, 64);
    int itemIndex = strtol(indexHex.c_str(), nullptr, 16);

    String amountHex = data.substring(64 * 2, 64 * 3);
    String nameLenHex = data.substring(64 * 3, 64 * 4);
    int nameLen = strtol(nameLenHex.c_str(), nullptr, 16);
    String nameHex = data.substring(64 * 4, 64 * 4 + nameLen * 2);
    String itemName = hexToAscii(nameHex);

    Serial.println("📦 Producto: " + itemName);
    Serial.println("💰 Monto: " + weiToEth(amountHex));
    Serial.printf("🔑 Index: %d\n", itemIndex);

    dispenseProduct(itemIndex);
    awaitingPayment = false;
}

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{
    Serial.printf("📡 WebSocket evento: %d\n", type);

    if (type == WStype_CONNECTED)
    {
        Serial.println("✅ WebSocket conectado, suscribiéndose a eventos...");

        String sub = String("{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"eth_subscribe\",\"params\":[\"logs\", {\"address\":\"") +
                     contractAddress + "\", \"topics\": [\"" + topicPurchase + "\"]}]}";

        webSocket.sendTXT(sub);
    }
    else if (type == WStype_TEXT)
    {
        handleIncomingMessage(payload, length);
    }
    else if (type == WStype_DISCONNECTED)
    {
        Serial.println("❌ WebSocket desconectado, intentando reconectar...");
    }
    else if (type == WStype_ERROR)
    {
        Serial.println("❌ Error en WebSocket");
    }
}

void initWebSocket()
{
    webSocket.beginSSL(ws_server, ws_port);
    webSocket.onEvent(webSocketEvent);
    webSocket.setReconnectInterval(5000);
}

void updateWebSocket()
{
    webSocket.loop();
}
