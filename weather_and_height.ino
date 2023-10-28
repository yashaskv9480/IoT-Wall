#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <DHT_U.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define DHTPIN 2         // DHT11 data pin
#define DHTTYPE DHT11    // DHT11 sensor type
DHT_Unified dht(DHTPIN, DHTTYPE);

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int triggerPin = 3;  // Trigger pin of the USS
const int echoPin = 4;     // Echo pin of the USS
const float referenceHeight = 7.0;  // Height of the sensor from the ground in feet

const char* ssid = "Yashas iPhone";
const char* password = "yashu123";
const char* mqtt_server = "172.20.10.8";
const char* mqtt_topic = "height_weather"; // MQTT topic to publish data

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(9600);
  dht.begin();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Configure MQTT
  client.setServer(mqtt_server, 1883);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  // Read temperature from DHT11
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  float temperature = event.temperature;

  // Trigger the USS to send a pulse
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  // Measure the time it takes for the pulse to return
  long duration = pulseIn(echoPin, HIGH);

  // Calculate distance in inches
  float distanceInches = duration * 0.0133 / 2.0;

  // Convert distance to feet
  float distanceFeet = distanceInches / 12.0;

  // Calculate person's height
  float personHeight = referenceHeight - distanceFeet;

  // Clear the display
  display.clearDisplay();

  // Print temperature on OLED
  display.setCursor(0, 0);
  display.println("Temperature: " + String(temperature, 1) + "°C");

  // Print distance and height on OLED
  display.setCursor(0, 20);
  display.print("Distance: ");
  display.print(distanceFeet, 2);
  display.println(" ft");

  display.print("Person's Height: ");
  display.print(personHeight, 2);
  display.println(" ft");

  display.display();

  // Publish data to MQTT
  char message[50];
  sprintf(message, "{\"temperature\":%.2f,\"distance\":%.2f,\"height\":%.2f}", temperature, distanceFeet, personHeight);
  client.publish(mqtt_topic, message);

  // Print the calculated values on Serial Monitor
  Serial.print("Temperature: ");
  Serial.println(temperature);
  Serial.print("Distance: ");
  Serial.print(distanceFeet, 2);
  Serial.print(" feet\t");
  Serial.print("Person's Height: ");
  Serial.print(personHeight, 2);
  Serial.println(" feet");

  delay(1000); // Update every 1 second
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    if (client.connect("ESP8266Client")) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" Trying again in 5 seconds");
      delay(5000);
    }
  }
}
