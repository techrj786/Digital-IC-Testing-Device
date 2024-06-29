# IC_TESTER

![GitHub Contributors](https://img.shields.io/github/contributors/techrj786/IC_TESTER.svg)

Based on the provided codes, here's a detailed overview of your project including pin connections:

### Pin Connections

#### Arduino Uno:
- **Digital Pins**:
  - D2 to D7: Push button pins for selecting ICs (connected to buttons).
  - D8, D9, D11, D12, A1, A2, A4, A5: Input pins for ICs (7400, 7408, 7432, 7486).
  - D10, D13, A0, A3: Output pins for ICs (7400, 7408, 7432, 7486).

- **Software Serial**:
  - D1 (TX), D0 (RX): Used for communication with ESP8266.

#### ESP8266 (ESP-01):
- **Software Serial**:
  - D3 (RX), D1 (TX): Used for communication with Arduino.

#### OLED Display (SSD1306):
- **I2C Interface**:
  - SDA (A4), SCL (A5): Connected to Arduino's I2C pins for communication with OLED display.

### Project Overview
1. **Arduino Code**:
   - Controls selection and testing of various ICs (7400, 7402, 7404, 7408, 7432, 7486) using push buttons and displays results via UART to the ESP8266.
   - Manages debouncing of push buttons for reliable input.
   - Communicates with ESP8266 using Software Serial (UART) for sending IC testing results.
   
2. **ESP8266 Code**:
   - Acts as a receiver for IC testing data from Arduino via UART.
   - Displays received data on an SSD1306 OLED display.
   - Initializes the OLED display and continuously updates it with received data.
   - Interfaces with Arduino using Software Serial for UART communication.

3. **Overall Functionality**:
   - Arduino selects and tests ICs based on button presses, checking their functionality and sending results to the ESP8266.
   - ESP8266 receives data from Arduino, displays it on the OLED display in real-time, providing visual feedback on the status of tested ICs.
   - Both devices work together to create a monitoring and diagnostic system for integrated circuit functionality.

This setup allows for a compact and interactive system where IC testing results are efficiently displayed on a small OLED screen, suitable for debugging and monitoring purposes in electronics projects.

![WhatsApp Image 2024-06-28 at 00 37 32_44d06e1c](https://github.com/a5r12/IC_TESTER/assets/159612830/b3e22952-f6ba-4dbe-9094-f479c47a8d41)


First ever prototype based on idea.

in this live project we change our second arduino with esp8266 in order to take advantage of wifi module and that 16x2 LCD is replaced by the SSD1306 OLED 128X64 (PIXEL). so you can change according to your project .

![image](https://github.com/a5r12/IC_TESTER/assets/159612830/b07d1e09-1889-4a2c-a58b-30af9a3bee92)


thats how output look like ....
you can change the font_style , font_size and string format etc as per your requirements.


basically the need of 2 device because of, All digital and analog pins are occupied , and we want to display the output on oled or lcd then required more pins that's why we have use second device either it could be esp8266 or arduino or something else..
