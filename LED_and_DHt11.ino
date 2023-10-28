#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 2         // DHT11 data pin
#define DHTTYPE DHT11    // DHT11 sensor type
DHT_Unified dht(DHTPIN, DHTTYPE);

const int numLEDs = 10;  // Number of LEDs
const int ledPins[numLEDs] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 12};  // LED pins

void setup() {
  Serial.begin(9600);

  dht.begin();

  for (int i = 0; i < numLEDs; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
}

void loop() {
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  float humidity = event.relative_humidity;

  int ledCount = map(humidity, 0, 100, 0, numLEDs);

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%  Led Count: ");
  Serial.println(ledCount);

  for (int i = 0; i < numLEDs; i++) {
    if (i < ledCount) {
      digitalWrite(ledPins[i], HIGH);  // Turn on LEDs up to ledCount
    } else {
      digitalWrite(ledPins[i], LOW);   // Turn off remaining LEDs
    }
  }

  delay(1000);  // Update every 1 second
}
