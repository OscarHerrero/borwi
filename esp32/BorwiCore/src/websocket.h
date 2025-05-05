#ifndef WEBSOCKET_HANDLER_H
#define WEBSOCKET_HANDLER_H

#include <Arduino.h>

void initWebSocket();
void updateWebSocket();
bool isAwaitingPayment();
void setAwaitingPayment(bool value);

#endif
