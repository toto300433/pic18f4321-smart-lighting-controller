# PIC18F4321 Smart Lighting Controller

<p align="left">
  <a href="LICENSE"><img alt="License" src="https://img.shields.io/badge/License-MIT-green.svg"></a>
  <img alt="Conventional Commits" src="https://img.shields.io/badge/Conventional%20Commits-1.0.0-%23FA6673">
  <img alt="Platform" src="https://img.shields.io/badge/Platform-Windows%20%7C%20PowerShell-blue">
  <img alt="PIC18F4321" src="https://img.shields.io/badge/MCU-PIC18F4321-lightgrey">
  <img alt="Toolchain" src="https://img.shields.io/badge/Toolchain-MPLAB%20X%20%7C%20XC8-orange">
</p>

---

## Overview

Cooperative firmware for the **PIC18F4321** microcontroller that authenticates users via **MFRC522 RFID** and drives six lights using **50 Hz PWM** with 11 discrete intensity levels. The system integrates:

* **RFID authentication** (entry/exit, UID recognition)
* **3×4 keypad input** (per-light adjustments, reset with `#`)
* **UART menu** at 9600 bps (ESC or short `#` to open)
* **16×2 LCD** displaying current time and light configuration
* **Cooperative FSMs** for predictable, real‑time performance

This project was originally developed as a university assignment, but has been fully restructured into a **portfolio‑ready embedded systems project**.

---

## Repository Structure

```
<repo>/
├─ src/firmware/        # MPLAB X project (PIC18F4321, XC8)
├─ tools/               # PowerShell scripts (build, flash, smoke tests)
├─ docs/specs/          # Original specification and project report
├─ docs/assets/         # Diagrams, pinout, photos
├─ test/                # Smoke tests (UART, LED PWM)
├─ data/, models/       # Placeholders (not tracked in Git)
└─ README.md            # You are here
```

---

## Bill of Materials (BOM)

* **MCU**: PIC18F4321 @ 40 MHz (10 MHz xtal + PLL)
* **RFID**: MFRC522 module (SPI bit‑banged in cooperative FSM)
* **Display**: 16×2 LCD (parallel interface)
* **Input**: 3×4 matrix keypad
* **Lighting outputs**: 6× PWM channels (50 Hz, 11 levels)
* **Connectivity**: UART 9600 bps (C6/C7)

---

## Pinout Summary

* **Keypad rows**: RA1–RA4
* **Keypad columns**: RB0, RC5, RB1
* **LEDs**: RB2–RB3, RE0, RB5–RB7
* **RFID (SPI soft)**: RC0–RC4
* **LCD**: RD1–RD7
* **UART TX/RX**: C6 / C7 @ 9600 bps

---

## Getting Started (Windows / PowerShell)

```powershell
# Clone repository
git clone https://github.com/AlbertoMarquillas/pic18f4321-smart-lighting-controller.git
cd pic18f4321-smart-lighting-controller

# Build (open MPLAB X, select XC8 toolchain)
# HEX will be generated under:
# src/firmware/P2_compus.X/dist/<config>/production/*.hex

# Flash (use MPLAB IPE with PICkit/ICD)
# or edit tools/flash.ps1 with your programmer CLI

# UART Smoke Test
tools/serial-smoke.ps1 -ComPort COM3 -Baud 9600
```

---

## Usage Cheatsheet

* **RFID**: present card → toggle user entry/exit and restore light config
* **Keypad**: adjust individual light levels (0x0–0xA)
* **Short `#`**: open UART menu (ESC from PC)
* **UART Menu**: 1) Who is in the room?  2) Show configs  3) Set time
* **Hold `#` ≥ 3s**: reset all light configs

---

## Features

* Cooperative multitasking via software FSMs
* Real‑time PWM lighting control (50 Hz, 11 levels)
* Multi‑user session management (UID‑based)
* Text‑based UART menu with real‑time updates
* Modular source layout for maintainability

---

## What I Learned

* Designing cooperative schedulers for resource‑limited MCUs
* Implementing a software SPI driver for MFRC522 RFID
* Integrating multiple peripherals (RFID, keypad, LCD, UART) in real time
* Managing EEPROM/flash persistence for user light profiles
* Structuring embedded projects for professional portfolios

---

## Roadmap

* [ ] Add persistence across resets via EEPROM
* [ ] Provide schematic and wiring diagrams in docs/assets/
* [ ] Record demo video and GIF for README
* [ ] Add static linting workflow (XC8 or CMake) to CI

---

## License

This project is licensed under the terms of the **MIT License**. See [LICENSE](LICENSE).

---

## Privacy & Ethics

* Example UIDs are used in demos; real RFID card data should not be published.
* Includes Microchip headers and MFRC522 driver references (license‑compatible).
