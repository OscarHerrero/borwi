# Borwi (Based On Real World Interaction)

**Borwi** is an experimental project that connects smart contracts on the blockchain with real-world physical interactions.

As a proof of concept, Borwi features a **vending machine controlled by smart contracts** deployed on the **Base Sepolia** blockchain. Users can scan a QR code and pay with cryptocurrency to receive a physical product in real time.

---

## 🎯 Project Goals

- Bridge blockchain and IoT in a direct, tangible experience.
- Build a fully autonomous vending machine with no intermediaries or traditional interfaces.
- Listen to **on-chain events** from physical devices connected via WiFi.
- Generate QR codes on-screen with payment instructions encoded as `ethereum:` URIs.
- Use smart contracts to manage product logic, pricing, and payments.

---

## ⚙️ Technologies Used

| Technology | Purpose |
|------------|---------|
| **Solidity** | Smart contract logic (`VendingMachineV2.sol`). |
| **ESP32 + ST7735** | Microcontroller + TFT screen to display the menu and QR code. |
| **WebSockets + JSON-RPC** | Real-time communication with the blockchain (Base Sepolia). |
| **PlatformIO (Arduino)** | Development environment for the ESP32. |
| **QRcode_ST7735** | Library for generating QR codes directly on the TFT screen. |

---

## 📦 Project Structure

| Folder | Description |
|--------|-------------|
| `contracts/VendingMachineV2.sol` | Smart contract with product data, pricing, and `receive()`-based purchase logic. |
| `esp32/BorwiCore/` | Complete C++ code for the ESP32. Handles network, display, QR, and on-chain events. |
| `assets/` | Visual assets and resources. |

---

## 🛠️ How It Works

1. The smart contract `VendingMachineV2.sol` is deployed on **Base Sepolia**.
2. A user scans the **QR code** shown on the TFT display.
3. The QR contains an `ethereum:` URI with the exact payment value for a product.
4. The user pays directly from their wallet (e.g., MetaMask mobile).
5. The contract emits a `ProductPurchased(...)` event.
6. The ESP32, connected to WiFi, **listens to this event** via WebSocket.
7. When detected, it updates the display and triggers a **servo** to dispense the product.

---

## 🧩 Physical Components

- 🧠 ESP32 DevKit
- 🎨 TFT Display ST7735 160x128
- 🔘 Physical buttons (up/down/select)
- ⚙️ 360º servo for product dispensing
- 🔌 Power supply

---

## 🚀 Roadmap

- [x] `VendingMachineV2.sol` contract with products and payments.
- [x] QR code generation scannable via MetaMask mobile.
- [x] WebSocket event listening on ESP32.
- [x] Integrated TFT menu navigation.
- [x] Visual confirmation after payment.
- [x] Servo-based physical product dispensing.
- [x] Animations and visual improvements.

---

## 📝 Notes

- The project is in an **experimental phase** but **fully functional**.
- Great for demos and educational exploration of Web3 + hardware.
- Products are identified by their exact price in `wei`, which determines which item is purchased.

---

## 📄 License

This project is licensed under the MIT License.
