#include "dispenser.h"
#include "display.h"
#include "state.h"

#define SERVO_PIN_1 12
#define SERVO_PIN_2 13
#define SERVO_PIN_3 14
#define SERVO_PIN_4 27

Servo dispenserServo1;
Servo dispenserServo2;
Servo dispenserServo3;
Servo dispenserServo4;

void initDispensers()
{
    dispenserServo1.setPeriodHertz(50);
    dispenserServo2.setPeriodHertz(50);
    dispenserServo3.setPeriodHertz(50);
    dispenserServo4.setPeriodHertz(50);

    dispenserServo1.attach(SERVO_PIN_1);
    dispenserServo2.attach(SERVO_PIN_2);
    dispenserServo3.attach(SERVO_PIN_3);
    dispenserServo4.attach(SERVO_PIN_4);
}

void dispenseProduct(int itemIndex)
{
    showMessage("Payment received...", ST77XX_GREEN, 1);
    delay(1000);

    Servo *servoToUse = nullptr;
    switch (itemIndex)
    {
    case 0:
        servoToUse = &dispenserServo1;
        break;
    case 1:
        servoToUse = &dispenserServo2;
        break;
    case 2:
        servoToUse = &dispenserServo3;
        break;
    case 3:
        servoToUse = &dispenserServo4;
        break;
    default:
        showMessage("Error Servo...", ST77XX_RED);
        Serial.println("⚠️ Índice fuera de rango. No se acciona servo.");
        return;
    }

    if (servoToUse != nullptr)
    {
        showMessage("Dispensing product...", ST77XX_GREEN, 1);
        servoToUse->write(100);
        delay(5000); // Duración del giro
        servoToUse->write(95);
        Serial.println("✅ Producto dispensado.");
    }

    showMessage("Thank you!", ST77XX_GREEN);
    delay(3000);
    drawMenu(items, prices, selectedIndex, itemCount);
}
