#include <SoftwareSerial.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

SoftwareSerial btSerial(D6,D5); // Rx, Tx

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  delay(1000);

  Serial.begin(9600);
  btSerial.begin(9600); // Bluetooth module baud rate

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Started...");
  display.display();
}

void loop() {
  if (btSerial.available() > 0) { // Check if Bluetooth module sends data to ESP8266
    String receivedData = "";

    while (btSerial.available() > 0) {
      char dataChar = btSerial.read();
      receivedData += dataChar; // Append the received characters to the String
    }
    Serial.println(receivedData);
    // Display the received data on the OLED screen
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
//    display.print("Received: ");
    display.println(receivedData); // Print the entire received String
    display.display();
  }
}
