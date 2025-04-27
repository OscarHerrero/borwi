# Borwi (Based On Real World Interaction)

**Borwi** es un proyecto experimental que busca unir el mundo de la blockchain con interacciones físicas en el mundo real.

Esta primera prueba de concepto conecta un contrato inteligente desplegado en **Base Testnet** con un **ESP32** que escucha eventos de la blockchain y enciende o apaga un LED.

---

## 📋 Objetivos del proyecto

- Demostrar la interacción directa entre un contrato en blockchain y un dispositivo físico.
- Crear una arquitectura simple y robusta para futuros experimentos basados en eventos on-chain.
- Aprender y documentar buenas prácticas para proyectos híbridos blockchain + IoT.

---

## ⚙️ Tecnologías utilizadas

- **Solidity** — para el desarrollo del contrato inteligente (`TurnLed.sol`).
- **PlatformIO** — para programar el **ESP32** como listener y actuador.
- **Base Testnet** — para desplegar el contrato inteligente.

---

## 📦 Componentes principales

| Componente | Descripción |
|------------|-------------|
| `contracts/TurnLed.sol` | Contrato inteligente que emite eventos para encender/apagar el LED. |
| `esp32/` | Código en PlatformIO que consulta eventos y controla el LED. |

---

## 🛠️ ¿Cómo funciona?

1. **Despliegue** del contrato `TurnLed` en **Base Testnet**.
2. El contrato permite a cualquier usuario enviar transacciones para **encender** o **apagar** un LED.
3. El **ESP32**, conectado a internet, hace **polling** a un servicio RPC de la blockchain para **escuchar eventos** emitidos por `TurnLed`.
4. Al detectar un evento, el ESP32 **enciende** o **apaga** un LED físico conectado.

---

## 🚀 Roadmap inicial

- [ ] Crear contrato `TurnLed.sol`.
- [ ] Programar ESP32 como listener de eventos vía HTTP polling.
- [ ] Crear infraestructura mínima de conexión segura.
- [ ] Documentar el flujo completo de despliegue y operación.
- [ ] Probar con múltiples actuadores físicos.

---

## 📚 Notas importantes

- El proyecto está en **fase experimental**.
- Se recomienda utilizar siempre **cuentas de prueba** y trabajar en **Base Testnet**.
- Inicialmente, el ESP32 hará **polling** consultando eventos nuevos cada pocos segundos.
- El sistema es fácilmente escalable para múltiples tipos de actuadores en el futuro.

---

## 📄 Licencia

Este proyecto está licenciado bajo MIT License.
