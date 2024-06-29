#include <Arduino.h>
#include <SoftwareSerial.h>

// Define the push button pins
const int buttonPins[] = {2, 3, 4, 5, 6, 7}; // Push buttons for selecting ICs
const int numButtons = 6;

// Define the input and output pins on the Arduino for different ICs
const int ic7400InputPins[] = {8, 9, 11, 12, A1, A2, A4, A5}; // Arduino pins connected to IC inputs (7400, 7408, 7432, 7486)
const int ic7400OutputPins[] = {10, 13, A0, A3};              // Arduino pins connected to IC outputs (7400, 7408, 7432, 7486)

const int ic7402InputPins[] = {9, 10, 12, 13, A0, A1, A3, A4}; // Arduino pins connected to IC inputs (7402)
const int ic7402OutputPins[] = {8, 11, A2, A5};                // Arduino pins connected to IC outputs (7402)

const int ic7404InputPins[] = {8, 10, 12, A1, A3, A5};         // Arduino pins connected to IC inputs (7404)
const int ic7404OutputPins[] = {9, 11, 13, A0, A2, A4};        // Arduino pins connected to IC outputs (7404)

const int ic7408InputPins[] = {8, 9, 11, 12, A0, A2, A4, A5};  // Arduino pins connected to IC inputs (7408)
const int ic7408OutputPins[] = {10, 13, A1, A3};               // Arduino pins connected to IC outputs (7408)

const int ic7432InputPins[] = {8, 9, 11, 12, A0, A2, A4, A5};  // Arduino pins connected to IC inputs (7432)
const int ic7432OutputPins[] = {10, 13, A1, A3};               // Arduino pins connected to IC outputs (7432)

const int ic7486InputPins[] = {8, 9, 11, 12, A0, A2, A4, A5};  // Arduino pins connected to IC inputs (7486)
const int ic7486OutputPins[] = {10, 13, A1, A3};               // Arduino pins connected to IC outputs (7486)

// Variables for debouncing
const unsigned long debounceDelay = 50; // Debounce time in milliseconds
unsigned long lastDebounceTime[numButtons];
bool lastButtonState[numButtons];
bool buttonState[numButtons];

// Define software serial for ESP8266 communication on D0 (RX) and D1 (TX)
SoftwareSerial espSerial(0, 1); // RX, TX

void setup() {
  // Initialize the push buttons
  for (int i = 0; i < numButtons; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
    lastButtonState[i] = digitalRead(buttonPins[i]);
    buttonState[i] = lastButtonState[i];
    lastDebounceTime[i] = 0;
  }

  // Initialize serial communication for debugging
  Serial.begin(9600);

  // Initialize software serial communication with ESP8266
  espSerial.begin(9600); // Make sure this baud rate matches ESP8266's baud rate

  // Set initial pin states
  for (int i = 0; i < 8; i++) {
    pinMode(ic7400InputPins[i], OUTPUT);
    digitalWrite(ic7400InputPins[i], LOW); // Initialize inputs to LOW
  }

  for (int i = 0; i < 4; i++) {
    pinMode(ic7400OutputPins[i], INPUT);
  }
}

void loop() {
  // Check button states and debounce
  for (int i = 0; i < numButtons; i++) {
    int reading = digitalRead(buttonPins[i]);

    if (reading != lastButtonState[i]) {
      lastDebounceTime[i] = millis();
    }

    if ((millis() - lastDebounceTime[i]) > debounceDelay) {
      if (reading != buttonState[i]) {
        buttonState[i] = reading;
        if (buttonState[i] == LOW) {
          // Handle button press
          switch (i) {
            case 0:
              testIC7400();
              break;
            case 1:
              testIC7402();
              break;
            case 2:
              testIC7404();
              break;
            case 3:
              testIC7408();
              break;
            case 4:
              testIC7432();
              break;
            case 5:
              testIC7486();
              break;
          }
        }
      }
    }

    lastButtonState[i] = reading;
  }
}

// Function to test IC 7400 (Quad 2-input NAND gates)
void testIC7400() {
  Serial.println("Testing IC 7400 (Quad 2-input NAND gates)");

  // Set all IC 7400 input pins to LOW initially for safety
  for (int i = 0; i < 8; i++) {
    digitalWrite(ic7400InputPins[i], LOW);
  }

  // Set all IC 7400 output pins to INPUT for reading output
  for (int i = 0; i < 4; i++) {
    pinMode(ic7400OutputPins[i], INPUT);
  }

  bool allGatesWorking = true; // Assume all gates are working initially

  int inputs[][2] = {
    {ic7400InputPins[0], ic7400InputPins[1]},
    {ic7400InputPins[2], ic7400InputPins[3]},
    {ic7400InputPins[4], ic7400InputPins[5]},
    {ic7400InputPins[6], ic7400InputPins[7]}
  };

  for (int i = 0; i < 4; i++) {
    for (int A = 0; A <= 1; A++) {
      for (int B = 0; B <= 1; B++) {
        digitalWrite(inputs[i][0], A);
        digitalWrite(inputs[i][1], B);
        bool expectedOutput = !(A && B);
        int actualOutput = digitalRead(ic7400OutputPins[i]);
        delay(100);

        Serial.print("[ GATE_");
        Serial.print(i + 1); // Gate number
        Serial.print(", ");
        if (actualOutput == expectedOutput) {
          Serial.println("working ]");
        } else {
          Serial.println("not working ]");
          allGatesWorking = false; // If any gate fails, set flag to false
        }
      }
    }
  }

  // Print IC status
  if (allGatesWorking) {
    Serial.println("IC_7400 is OK");
  } else {
    Serial.println("IC_7400 is damaged");
  }

  // Send data to ESP8266 over UART
  espSerial.println("IC7400:");
  espSerial.print("Status: ");
  espSerial.println(allGatesWorking ? "OK" : "Damaged");
  for (int i = 0; i < 4; i++) {
    bool gateStatus = (digitalRead(ic7400OutputPins[i]) == LOW); // Check if gate is working (LOW = working)
    espSerial.print("gate_");
    espSerial.print(i + 1);
    espSerial.print(": ");
    espSerial.println(gateStatus ? "OK" : "Fault");
  }
}

// Function to test IC 7402 (Quad 2-input NOR gates)
void testIC7402() {
  Serial.println("Testing IC 7402 (Quad 2-input NOR gates)");

  // Set all IC 7402 input pins to LOW initially for safety
  for (int i = 0; i < 8; i++) {
    digitalWrite(ic7402InputPins[i], LOW);
  }

  // Set all IC 7402 output pins to INPUT for reading output
  for (int i = 0; i < 4; i++) {
    pinMode(ic7402OutputPins[i], INPUT);
  }

  bool allGatesWorking = true; // Assume all gates are working initially

  int inputs[][2] = {
    {ic7402InputPins[0], ic7402InputPins[1]},
    {ic7402InputPins[2], ic7402InputPins[3]},
    {ic7402InputPins[4], ic7402InputPins[5]},
    {ic7402InputPins[6], ic7402InputPins[7]}
  };

  for (int i = 0; i < 4; i++) {
    for (int A = 0; A <= 1; A++) {
      for (int B = 0; B <= 1; B++) {
        digitalWrite(inputs[i][0], A);
        digitalWrite(inputs[i][1], B);
        bool expectedOutput = !(A || B);
        int actualOutput = digitalRead(ic7402OutputPins[i]);
        delay(100);

        Serial.print("[ GATE_");
        Serial.print(i + 1); // Gate number
        Serial.print(", ");
        if (actualOutput == expectedOutput) {
          Serial.println("working ]");
        } else {
          Serial.println("not working ]");
          allGatesWorking = false; // If any gate fails, set flag to false
        }
      }
    }
  }

  // Print IC status
  if (allGatesWorking) {
    Serial.println("IC_7402 is OK");
  } else {
    Serial.println("IC_7402 is damaged");
  }

  // Send data to ESP8266 over UART
  espSerial.println("IC7402:");
  espSerial.print("Status: ");
  espSerial.println(allGatesWorking ? "OK" : "Damaged");
  for (int i = 0; i < 4; i++) {
    bool gateStatus = (digitalRead(ic7402OutputPins[i]) == LOW); // Check if gate is working (LOW = working)
    espSerial.print("gate_");
    espSerial.print(i + 1);
    espSerial.print(": ");
    espSerial.println(gateStatus ? "OK" : "Fault");
  }
}

// Function to test IC 7404 (Hex inverters)
void testIC7404() {
  Serial.println("Testing IC 7404 (Hex inverters)");

  // Set all IC 7404 input pins to LOW initially for safety
  for (int i = 0; i < 6; i++) {
    digitalWrite(ic7404InputPins[i], LOW);
  }

  // Set all IC 7404 output pins to INPUT for reading output
  for (int i = 0; i < 6; i++) {
    pinMode(ic7404OutputPins[i], INPUT);
  }

  bool allGatesWorking = true; // Assume all gates are working initially

  for (int i = 0; i < 6; i++) {
    for (int A = 0; A <= 1; A++) {
      digitalWrite(ic7404InputPins[i], A);
      bool expectedOutput = !A;
      int actualOutput = digitalRead(ic7404OutputPins[i]);
      delay(100);

      Serial.print("[ GATE_");
      Serial.print(i + 1); // Gate number
      Serial.print(", ");
      if (actualOutput == expectedOutput) {
        Serial.println("working ]");
      } else {
        Serial.println("not working ]");
        allGatesWorking = false; // If any gate fails, set flag to false
      }
    }
  }

  // Print IC status
  if (allGatesWorking) {
    Serial.println("IC_7404 is OK");
  } else {
    Serial.println("IC_7404 is damaged");
  }

  // Send data to ESP8266 over UART
  espSerial.println("IC7404:");
  espSerial.print("Status: ");
  espSerial.println(allGatesWorking ? "OK" : "Damaged");
  for (int i = 0; i < 6; i++) {
    bool gateStatus = (digitalRead(ic7404OutputPins[i]) == LOW); // Check if gate is working (LOW = working)
    espSerial.print("gate_");
    espSerial.print(i + 1);
    espSerial.print(": ");
    espSerial.println(gateStatus ? "OK" : "Fault");
  }
}

// Function to test IC 7408 (Quad 2-input AND gates)
void testIC7408() {
  Serial.println("Testing IC 7408 (Quad 2-input AND gates)");

  // Set all IC 7408 input pins to LOW initially for safety
  for (int i = 0; i < 8; i++) {
    digitalWrite(ic7408InputPins[i], LOW);
  }

  // Set all IC 7408 output pins to INPUT for reading output
  for (int i = 0; i < 4; i++) {
    pinMode(ic7408OutputPins[i], INPUT);
  }

  bool allGatesWorking = true; // Assume all gates are working initially

  int inputs[][2] = {
    {ic7408InputPins[0], ic7408InputPins[1]},
    {ic7408InputPins[2], ic7408InputPins[3]},
    {ic7408InputPins[4], ic7408InputPins[5]},
    {ic7408InputPins[6], ic7408InputPins[7]}
  };

  for (int i = 0; i < 4; i++) {
    for (int A = 0; A <= 1; A++) {
      for (int B = 0; B <= 1; B++) {
        digitalWrite(inputs[i][0], A);
        digitalWrite(inputs[i][1], B);
        bool expectedOutput = (A && B);
        int actualOutput = digitalRead(ic7408OutputPins[i]);
        delay(100);

        Serial.print("[ GATE_");
        Serial.print(i + 1); // Gate number
        Serial.print(", ");
        if (actualOutput == expectedOutput) {
          Serial.println("working ]");
        } else {
          Serial.println("not working ]");
          allGatesWorking = false; // If any gate fails, set flag to false
        }
      }
    }
  }

  // Print IC status
  if (allGatesWorking) {
    Serial.println("IC_7408 is OK");
  } else {
    Serial.println("IC_7408 is damaged");
  }

  // Send data to ESP8266 over UART
  espSerial.println("IC7408:");
  espSerial.print("Status: ");
  espSerial.println(allGatesWorking ? "OK" : "Damaged");
  for (int i = 0; i < 4; i++) {
    bool gateStatus = (digitalRead(ic7408OutputPins[i]) == LOW); // Check if gate is working (LOW = working)
    espSerial.print("gate_");
    espSerial.print(i + 1);
    espSerial.print(": ");
    espSerial.println(gateStatus ? "OK" : "Fault");
  }
}

// Function to test IC 7432 (Quad 2-input OR gates)
void testIC7432() {
  Serial.println("Testing IC 7432 (Quad 2-input OR gates)");

  // Set all IC 7432 input pins to LOW initially for safety
  for (int i = 0; i < 8; i++) {
    digitalWrite(ic7432InputPins[i], LOW);
  }

  // Set all IC 7432 output pins to INPUT for reading output
  for (int i = 0; i < 4; i++) {
    pinMode(ic7432OutputPins[i], INPUT);
  }

  bool allGatesWorking = true; // Assume all gates are working initially

  int inputs[][2] = {
    {ic7432InputPins[0], ic7432InputPins[1]},
    {ic7432InputPins[2], ic7432InputPins[3]},
    {ic7432InputPins[4], ic7432InputPins[5]},
    {ic7432InputPins[6], ic7432InputPins[7]}
  };

  for (int i = 0; i < 4; i++) {
    for (int A = 0; A <= 1; A++) {
      for (int B = 0; B <= 1; B++) {
        digitalWrite(inputs[i][0], A);
        digitalWrite(inputs[i][1], B);
        bool expectedOutput = (A || B);
        int actualOutput = digitalRead(ic7432OutputPins[i]);
        delay(100);

        Serial.print("[ GATE_");
        Serial.print(i + 1); // Gate number
        Serial.print(", ");
        if (actualOutput == expectedOutput) {
          Serial.println("working ]");
        } else {
          Serial.println("not working ]");
          allGatesWorking = false; // If any gate fails, set flag to false
        }
      }
    }
  }

  // Print IC status
  if (allGatesWorking) {
    Serial.println("IC_7432 is OK");
  } else {
    Serial.println("IC_7432 is damaged");
  }

  // Send data to ESP8266 over UART
  espSerial.println("IC7432:");
  espSerial.print("Status: ");
  espSerial.println(allGatesWorking ? "OK" : "Damaged");
  for (int i = 0; i < 4; i++) {
    bool gateStatus = (digitalRead(ic7432OutputPins[i]) == LOW); // Check if gate is working (LOW = working)
    espSerial.print("gate_");
    espSerial.print(i + 1);
    espSerial.print(": ");
    espSerial.println(gateStatus ? "OK" : "Fault");
  }
}

// Function to test IC 7486 (Quad 2-input XOR gates)
void testIC7486() {
  Serial.println("Testing IC 7486 (Quad 2-input XOR gates)");

  // Set all IC 7486 input pins to LOW initially for safety
  for (int i = 0; i < 8; i++) {
    digitalWrite(ic7486InputPins[i], LOW);
  }

  // Set all IC 7486 output pins to INPUT for reading output
  for (int i = 0; i < 4; i++) {
    pinMode(ic7486OutputPins[i], INPUT);
  }

  bool allGatesWorking = true; // Assume all gates are working initially

  int inputs[][2] = {
    {ic7486InputPins[0], ic7486InputPins[1]},
    {ic7486InputPins[2], ic7486InputPins[3]},
    {ic7486InputPins[4], ic7486InputPins[5]},
    {ic7486InputPins[6], ic7486InputPins[7]}
  };

  for (int i = 0; i < 4; i++) {
    for (int A = 0; A <= 1; A++) {
      for (int B = 0; B <= 1; B++) {
        digitalWrite(inputs[i][0], A);
        digitalWrite(inputs[i][1], B);
        bool expectedOutput = (A ^ B);
        int actualOutput = digitalRead(ic7486OutputPins[i]);
        delay(100);

        Serial.print("[ GATE_");
        Serial.print(i + 1); // Gate number
        Serial.print(", ");
        if (actualOutput == expectedOutput) {
          Serial.println("working ]");
        } else {
          Serial.println("not working ]");
          allGatesWorking = false; // If any gate fails, set flag to false
        }
      }
    }
  }

  // Print IC status
  if (allGatesWorking) {
    Serial.println("IC_7486 is OK");
  } else {
    Serial.println("IC_7486 is damaged");
  }

  // Send data to ESP8266 over UART
  espSerial.println("IC7486:");
  espSerial.print("Status: ");
  espSerial.println(allGatesWorking ? "OK" : "Damaged");
  for (int i = 0; i < 4; i++) {
    bool gateStatus = (digitalRead(ic7486OutputPins[i]) == LOW); // Check if gate is working (LOW = working)
    espSerial.print("gate_");
    espSerial.print(i + 1);
    espSerial.print(": ");
    espSerial.println(gateStatus ? "OK" : "Fault");
  }
}
