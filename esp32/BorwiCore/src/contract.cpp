#include "contract.h"
#include "utils.h"
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "configuration.h"

#define MAX_ITEMS 8

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

    // Llamada para getItemCount
    if (!ethCall("0x7749cf23", result))
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

        // getItem
        String itemResult;
        if (ethCall("0x3129e773" + indexPadded, itemResult))
        {
            if (itemResult != "0x")
            {
                String hex = itemResult.substring(130);
                hex.trim();
                items[i] = hexToAscii(hex);
            }
            else
            {
                items[i] = "Item" + String(i + 1);
            }
        }
        else
        {
            items[i] = "Item" + String(i + 1);
        }
        Serial.println("📦 Nombre: " + items[i]);

        // getItemPrice
        String priceResult;
        if (ethCall("0x4c4df870" + indexPadded, priceResult))
        {
            prices[i] = priceResult;
            Serial.println("💰 Precio: " + weiToEth(priceResult));
        }
        else
        {
            prices[i] = "0x0";
        }
    }

    return true;
}
