# Borwi (Based On Real World Interaction)

**Borwi** is an experimental project aiming to connect the blockchain world with physical real-world interactions.

This initial proof of concept links a smart contract deployed on **Base Testnet** with an **ESP32** device that listens to blockchain events to turn a LED on or off.

---

## 📋 Project Goals

- Demonstrate direct interaction between a blockchain contract and a physical device.
- Create a simple and robust architecture for future experiments based on on-chain events.
- Learn and document best practices for hybrid blockchain + IoT projects.

---

## ⚙️ Technologies Used

- **Solidity** — for developing the smart contract (`TurnLed.sol`).
- **PlatformIO** — for programming the **ESP32** as a listener and actuator.
- **Base Testnet** — for deploying the smart contract.

---

## 📦 Main Components

| Component | Description |
|-----------|-------------|
| `contracts/TurnLed.sol` | Smart contract that emits events to turn the LED on/off. |
| `esp32/` | PlatformIO code that polls events and controls the LED. |

---

## 🛠️ How It Works

1. **Deploy** the `TurnLed` smart contract on **Base Testnet**.
2. The contract allows any user to send transactions to **turn on** or **turn off** a LED.
3. The **ESP32**, connected to the internet, **polls** a blockchain RPC service to **listen to events** emitted by `TurnLed`.
4. When an event is detected, the ESP32 **turns the connected LED on or off** accordingly.

---

## 🚀 Initial Roadmap

- [ ] Create the `TurnLed.sol` smart contract.
- [ ] Program the ESP32 as an event listener via HTTP polling.
- [ ] Set up minimal secure connection infrastructure.
- [ ] Document the complete deployment and operation workflow.
- [ ] Test with multiple types of physical actuators.

---

## 📚 Important Notes

- The project is currently in an **experimental phase**.
- Always use **test accounts** and operate on **Base Testnet**.
- Initially, the ESP32 will **poll** for new events every few seconds.
- The system is easily scalable for multiple types of actuators in future versions.

---

## 📄 License

This project is licensed under the MIT License.
