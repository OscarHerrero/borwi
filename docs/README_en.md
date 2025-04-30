# Borwi (Based On Real World Interaction)

**Borwi** is an experimental project that connects smart contracts on the blockchain with physical real-world interactions.

Currently, Borwi is a **vending machine controlled by smart contracts** deployed on the **Base** blockchain. Users can scan a QR code and pay with cryptocurrency to receive a physical product in real-time.

---

## 🎯 Project Goals

- Bridge blockchain and IoT into a direct and tangible experience.
- Build a fully autonomous vending machine with no intermediaries or traditional interfaces.
- Listen to **on-chain** events from WiFi-connected physical devices.
- Generate QR codes on screen with payment instructions encoded as `ethereum:` URIs.
- Use smart contracts to govern pricing, product logic, and payments.

---

## ⚙️ Technologies Used

| Technology         | Purpose                                                  |
|--------------------|----------------------------------------------------------|
| **Solidity**        | Smart contract logic (`VendingMachine.sol`).            |
| **ESP32 + ST7735**  | Microcontroller + TFT screen for displaying menus and QR codes. |
| **WebSockets + JSON-RPC** | Real-time communication with the blockchain (Base).  |
| **PlatformIO (Arduino)** | Development environment for the ESP32.               |
| **QRcode_ST7735**   | Library to render QR codes directly on the TFT screen.  |

---

## 📦 Project Structure

| Folder                 | Description                                                     |
|------------------------|-----------------------------------------------------------------|
| `contracts/VendingMachine.sol` | Smart contract with products, prices, and purchase logic via `receive()` |
| `esp32/BorwiCore/`     | Full C++ code for the ESP32. Manages WiFi, screen, QR, and on-chain events. |
| `assets/`              | Icons, mockups, and visual resources.                           |

---

## 🛠️ How It Works

1. The smart contract `VendingMachine.sol` is deployed on **Base Sepolia**.
2. A user scans the **QR code** shown on the device’s TFT screen.
3. That QR contains an `ethereum:` URI with the exact value to pay.
4. The user pays directly from their wallet (e.g. MetaMask mobile).
5. The contract emits a `ProductPurchased(...)` event.
6. The ESP32, connected via WiFi, **listens to that event** via WebSocket.
7. Upon detection, it updates the screen and activates a **servo** to dispense the product.

---

## 🧩 Physical Components

- 🧠 ESP32 DevKit
- 🎨 TFT ST7735 160x128 screen
- 🔘 Physical buttons (up/down/confirm)
- 🔊 Optional buzzer
- ⚙️ 360º servo for dispensing
- 🔌 Stable power supply

---

## 🚀 Roadmap

- [x] `VendingMachine.sol` contract with products and payments.
- [x] QR code generation scannable by MetaMask mobile.
- [x] WebSocket event listening from ESP32.
- [x] Integrated TFT screen with navigable menu.
- [x] Visual confirmation after payment.
- [ ] Physical product dispensing via servo.
- [ ] Animations, sound, and UI enhancements (LVGL phase).
- [ ] Multilingual version.
- [ ] Migration to mainnet or local production setup.

---

## 📝 Notes

- This project is in an **experimental stage** but **fully functional**.
- Perfect for demos and educational exploration of Web3 + hardware.
- Products are identified by their exact `wei` price, which determines which item is purchased.

---

## 📄 License

This project is licensed under the MIT License.
