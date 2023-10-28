#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <MFRC522.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

const char *fortunes[] = {
  "You will have a great day!",
  "Expect a surprise today!",
  "A new opportunity is coming.",
  "Stay positive and keep moving.",
  "Your hard work will pay off soon.",
  "Good things are on the horizon.",
  "Dream big and aim high."
};

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();  // Initialize RFID reader
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Place RFID tag...");
  display.display();
}

void loop() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    display.setCursor(0, 0);
    display.println("Reading tag...");
    display.display();

    // Read the tag's UID
    String uid = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      uid += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
      uid += String(mfrc522.uid.uidByte[i], HEX);
    }

    // Generate a random fortune index
    int randomIndex = random(sizeof(fortunes) / sizeof(fortunes[0]));

    // Display the fortune on OLED screen
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Tag UID:");
    display.println(uid);
    display.println();
    display.println("Your Fortune:");
    display.println(fortunes[randomIndex]);
    display.display();

    Serial.println("Tag UID: " + uid);
    Serial.println("Fortune: " + fortunes[randomIndex]);

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }

  delay(1000);
}
