#include <Adafruit_NeoPixel.h>

const int ledPin = 6;  // Pin where LED data is connected
const int numLEDs = 12;  // Number of LEDs in your strip
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numLEDs, ledPin, NEO_GRB + NEO_KHZ800);

const int potPin = A0;  // Potentiometer connected to analog pin A0

void setup() {
  strip.begin();
  strip.show();  // Initialize all LEDs to 'off'

  pinMode(potPin, INPUT);
}

void loop() {
  int potValue = analogRead(potPin);  // Read potentiometer value

  if (potValue < 341) {
    pattern1();  // Call pattern 1 for low potentiometer values
  } else if (potValue >= 341 && potValue < 682) {
    pattern2();  // Call pattern 2 for medium potentiometer values
  } else {
    pattern3();  // Call pattern 3 for high potentiometer values
  }
}

void pattern1() {
  for (int i = 0; i < numLEDs; i++) {
    strip.setPixelColor(i, strip.Color(255, 0, 0));  // Set LED color to red
  }
  strip.show();
  delay(500);
  
  strip.clear();
  strip.show();
  delay(500);
}

void pattern2() {
    for (int brightness = 0; brightness <= 255; brightness++) {
    for (int i = 0; i < numLEDs; i++) {
      strip.setPixelColor(i, strip.Color(brightness, brightness, brightness));  // Set LED color
    }
    strip.show();
    delay(10);
  }
  
  for (int brightness = 255; brightness >= 0; brightness--) {
    for (int i = 0; i < numLEDs; i++) {
      strip.setPixelColor(i, strip.Color(brightness, brightness, brightness));  // Set LED color
    }
    strip.show();
    delay(10);
  }
}

void pattern3() {
  int hue = 0;

  for (int i = 0; i < numLEDs; i++) {
    strip.setPixelColor(i, strip.ColorHSV(hue, 255, 255));  // Set LED color using HSV color space
    hue += 30;  // Change hue for the next LED
  }
  strip.show();
  delay(100);
}
