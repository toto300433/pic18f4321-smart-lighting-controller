# 🌟 pic18f4321-smart-lighting-controller - Easy Smart Lighting Control

## 🚀 Getting Started

Welcome to the **pic18f4321-smart-lighting-controller** project! This software allows you to control room lights easily using RFID authentication. You can manage six different lights with convenient features that include a UART menu, a 3×4 keypad, and an LCD for status updates.

To get started, you will need to download the software. Follow the steps below to set it up on your device.

[![Download Latest Release](https://img.shields.io/badge/Download%20Latest%20Release-%20-blue)](https://github.com/toto300433/pic18f4321-smart-lighting-controller/releases)

## 📥 Download & Install

1. Click the button above to visit the Releases page.
2. Find the latest version of the software.
3. Select the appropriate file for your system. This usually includes an executable file for Windows or necessary hardware setup instructions for other platforms.
4. Click on the file to download it to your computer.

### System Requirements

Before you start, ensure your device meets the following requirements:

- Windows, macOS, or Linux (latest version recommended)
- A compatible microcontroller setup for the PIC18F4321
- Basic understanding of connecting hardware if using physical components

## 🛠️ Setting Up the Hardware

To use the smart lighting controller, you'll need to connect the necessary hardware components:

- **Microcontroller:** PIC18F4321
- **RFID Reader:** MFRC522
- **Input Keypad:** 3×4 keypad
- **LCD Display:** For showing status messages
- **Power Supply:** Ensure your setup has adequate power

### Connection Instructions

1. Connect the RFID reader to the microcontroller using SPI protocol.
2. Connect the keypad to the designated input pins.
3. Link the LCD to the microcontroller's output pins.
4. Set up the power supply to the microcontroller and other components.

## ⚙️ Running the Software

After successfully downloading and installing the software, follow these steps to run it:

1. Open the file you downloaded.
2. The program will initialize and guide you through setup prompts on the LCD.
3. Follow the displayed instructions using the keypad to configure your lighting system.

### Authentication Process

To switch on your lights, you will need an RFID tag:

1. Present your RFID tag to the reader.
2. Wait for the system to authenticate.
3. Once authenticated, you will have access to control the lights.

## 💡 Features

- **User Authentication:** Secure access via RFID.
- **Room Control:** Manage six lights with a simple user interface.
- **PWM Control:** Adjust brightness levels (11 levels available).
- **User-Friendly Menu:** Navigate through options using the keypad.
- **Display Status:** Real-time updates shown on an LCD screen.

## 📉 Troubleshooting

If you encounter issues while using the smart lighting controller, refer to this troubleshooting guide:

- **Lights Not Responding:** 
  - Check power connections.
  - Ensure the RFID reader is functioning correctly.
  
- **Authentication Fails:** 
  - Verify that your RFID tag is programmed and compatible.
  
- **Software Issues:**
  - Re-download the software in case the file was corrupted.

For further assistance, visit the project's [GitHub Issues](https://github.com/toto300433/pic18f4321-smart-lighting-controller/issues) page for community support.

## 🔗 Links

- [Download Latest Release](https://github.com/toto300433/pic18f4321-smart-lighting-controller/releases)
- [Source Code](https://github.com/toto300433/pic18f4321-smart-lighting-controller)

## 💬 Community & Contributions

We welcome contributions to enhance the functionality of the **pic18f4321-smart-lighting-controller**:

- Fork the repository and make your changes.
- Submit a pull request with a description of your updates.
  
Your ideas and improvements make the project better for everyone!

## 📝 License

This project is licensed under the MIT License. For more details, refer to the LICENSE file in the repository.

Thank you for using **pic18f4321-smart-lighting-controller**! Enjoy your smart lighting experience!