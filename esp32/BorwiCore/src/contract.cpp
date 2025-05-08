#include "contract.h"
#include "utils.h"
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "configuration.h"
#include "display.h"

#define MAX_ITEMS 8

const String getItemSelector = "0x3129e773";      // getItem(uint256)
const String getItemCountSelector = "0xa9c92fc0"; // getitemCounter()

bool ethCall(String data, String &result)
{
    WiFiClientSecure secureClient;
    secureClient.setInsecure();

    HTTPClient http;
    http.begin(secureClient, String("https://") + String(ws_server));
    http.addHeader("Content-Type", "application/json");

    String payload = "{\"jsonrpc\":\"2.0\",\"method\":\"eth_call\",\"params\":[{\"to\":\"" + String(contractAddress) +
                     "\",\"data\":\"" + data + "\"},\"latest\"],\"id\":1}";

    int code = http.POST(payload);
    if (code > 0)
    {
        JsonDocument doc;
        DeserializationError err = deserializeJson(doc, http.getString());
        if (!err && doc["result"])
        {
            result = doc["result"].as<String>();
            http.end();
            return true;
        }
    }

    http.end();
    return false;
}

bool loadItems(String items[], String prices[], int &itemCount)
{
    Serial.println("🔄 Iniciando carga de productos...");
    String result;

    if (!ethCall(getItemCountSelector, result))
    {
        Serial.println("❌ Error al obtener getItemCount");
        return false;
    }

    result.replace("0x", "");
    itemCount = strtol(result.c_str(), nullptr, 16);
    if (itemCount > MAX_ITEMS)
        itemCount = MAX_ITEMS;

    Serial.printf("✅ Productos detectados: %d\n", itemCount);

    for (int i = 0; i < itemCount; i++)
    {
        Serial.printf("🔹 Cargando producto #%d\n", i);
        String indexPadded = pad64(String(i, HEX));
        String callData = getItemSelector + indexPadded;

        String itemResult;
        if (!ethCall(callData, itemResult))
        {
            items[i] = "Item" + String(i + 1);
            prices[i] = "0x0";
            continue;
        }

        itemResult.replace("0x", "");
        Serial.println("🔸 Resultado crudo:");
        Serial.println(itemResult);

        if (itemResult.length() < 320)
        {
            items[i] = "Desconocido";
            prices[i] = "0x0";
            Serial.println("⚠️ Respuesta demasiado corta");
            continue;
        }

        // Layout de struct Item
        String priceHex = itemResult.substring(128, 192);
        String lenHex = itemResult.substring(256, 320);
        int nameLen = strtol(lenHex.c_str(), nullptr, 16);
        String nameHex = itemResult.substring(320, 320 + nameLen * 2);
        String amountHex = itemResult.substring(192, 256);

        Serial.println("🧩 priceHex: " + priceHex);
        Serial.println("🧩 amountHex: " + amountHex);
        Serial.println("🧩 lenHex: " + lenHex);
        Serial.println("🧩 nameHex: " + nameHex);

        prices[i] = "0x" + priceHex;
        items[i] = hexToAscii(nameHex);

        Serial.println("📦 " + items[i]);
        Serial.println("💰 Precio: " + weiToEth(prices[i]));
        Serial.println("🔑 Cantidad: " + String(strtol(amountHex.c_str(), nullptr, 16)));
        logLine(items[i] + " " + weiToEth(prices[i]) + "  *" + String(strtol(amountHex.c_str(), nullptr, 16)), ST77XX_WHITE);
    }

    return true;
}
