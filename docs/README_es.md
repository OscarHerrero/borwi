# Borwi (Based On Real World Interaction)

**Borwi** es un proyecto experimental que conecta contratos inteligentes en blockchain con interacciones físicas del mundo real.

Actualmente, Borwi es una **máquina expendedora controlada por contratos inteligentes** desplegados en la blockchain **Base**. El usuario puede escanear un código QR y pagar con criptomonedas para obtener un producto físico en tiempo real.

---

## 🎯 Objetivos del proyecto

- Unir blockchain e IoT en una experiencia directa y tangible.
- Crear una vending machine completamente autónoma, sin intermediarios ni interfaces tradicionales.
- Escuchar eventos **on-chain** desde dispositivos físicos conectados vía WiFi.
- Generar código QR en pantalla con instrucciones de pago codificadas como `ethereum:` URIs.
- Utilizar contratos inteligentes para gobernar la lógica de precios, productos y pagos.

---

## ⚙️ Tecnologías utilizadas

| Tecnología | Uso |
|------------|-----|
| **Solidity** | Lógica del contrato inteligente `VendingMachine.sol`. |
| **ESP32 + ST7735** | Microcontrolador + pantalla TFT para mostrar menú y QR. |
| **WebSockets + JSON-RPC** | Comunicación en tiempo real con la blockchain (Base). |
| **PlatformIO (Arduino)** | Entorno de desarrollo para el ESP32. |
| **QRcode_ST7735** | Librería de generación de códigos QR directamente en la pantalla TFT. |

---

## 📦 Estructura del proyecto

| Carpeta | Descripción |
|--------|-------------|
| `contracts/VendingMachine.sol` | Contrato inteligente con productos, precios y lógica de compra vía `receive()` |
| `esp32/BorwiCore/` | Código completo en C++ para el ESP32. Gestiona red, pantalla, QR y eventos on-chain. |
| `assets/` | Iconos, mockups y recursos visuales. |

---

## 🛠️ ¿Cómo funciona?

1. El contrato inteligente `VendingMachine.sol` está desplegado en **Base Sepolia**.
2. Un usuario escanea el **código QR** mostrado en la pantalla TFT del dispositivo.
3. Ese QR contiene una URI `ethereum:` con el valor exacto del producto a pagar.
4. El usuario paga directamente desde su wallet (ej. MetaMask mobile).
5. El contrato emite un evento `ProductPurchased(...)`.
6. El ESP32, conectado a internet por WiFi, **escucha ese evento** vía WebSocket.
7. Al detectarlo, actualiza la pantalla y acciona un **servo** para dispensar el producto.

---

## 🧩 Componentes físicos

- 🧠 ESP32 DevKit
- 🎨 Pantalla TFT ST7735 160x128
- 🔘 Botones físicos (subir/bajar/aceptar)
- 🔊 Buzzer opcional
- ⚙️ Servo 360º para dispensado
- 🔌 Fuente de alimentación estable

---

## 🚀 Roadmap

- [x] Contrato `VendingMachine.sol` con productos y pagos.
- [x] Generación de QR legible desde MetaMask mobile.
- [x] Escucha de eventos via WebSocket en ESP32.
- [x] Integración de pantalla TFT con menú navegable.
- [x] Confirmación visual tras el pago.
- [ ] Control de dispensado físico con servo.
- [ ] Animaciones, sonidos y mejoras gráficas (fase LVGL).
- [ ] Versión multilingüe.
- [ ] Migración a mainnet o producción en red local.

---

## 📝 Notas

- El proyecto está en **fase experimental** pero **totalmente funcional**.
- Ideal para demostraciones y exploración educativa de Web3 + hardware.
- Los productos se identifican por su precio exacto en `wei`, el cual determina qué item se compra.

---

## 📄 Licencia

Este proyecto está licenciado bajo MIT License.
