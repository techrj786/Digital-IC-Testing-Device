
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>

// Define OLED parameters
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Define software serial for UART communication on ESP8266 (D3 as RX, D1 as TX)
SoftwareSerial SerialESP8266(3, 1); // RX, TX
void setup() {
    // Initialize OLED display with address 0x3C
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
    }

    // Clear the display buffer
    display.clearDisplay();

    // Display initial message
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("ESP8266 Receiver");
    display.display();

    // Initialize serial communication with baud rate 9600 for UART
    SerialESP8266.begin(9600); // Make sure this baud rate matches Arduino's UART baud rate
}
void loop() {
    // Check if data is available to read from UART
    if (SerialESP8266.available() > 0) {
        // Read data from UART
        String receivedData = SerialESP8266.readStringUntil('\n');

        // Print received data to Serial monitor (for debugging)
        Serial.print("Received Data: ");
        Serial.println(receivedData);

        // Clear the display and print received data
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 0);
        display.println("Received Data:");
        display.setTextSize(1);
        display.setCursor(0, 16);
        display.println(receivedData);
        display.display();
    }

    delay(100); // Delay for stability
}
