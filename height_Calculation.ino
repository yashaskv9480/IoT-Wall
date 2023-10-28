
#include <Wire.h>
#include <ESP8266WiFi.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <PubSubClient.h> // Include the PubSubClient library

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Define your WiFi and MQTT broker details

const char* ssid = "OnePlus 8 Pro";
const char* password = "1234567890";
const char* mqtt_server = "173.212.249.30";
const char* mqtt_username = "smps";
const char* mqtt_password = "smps1234";


WiFiClient espClient;
PubSubClient client(espClient);

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int triggerPin = D4;
const int echoPin = D3;
const float referenceHeight = 4.92;

void setup() {
  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Initial setup...");

  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Configure MQTT
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  // Connect to MQTT broker
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP8266Client", mqtt_username, mqtt_password)) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("MQTT connection failed, rc=");
      Serial.print(client.state());
      Serial.println(" Trying again in 5 seconds...");
      delay(5000);
    }
  }
}

void loop() {
    if (!client.connected()) {
    reconnect();
  }
  // Trigger the USS to send a pulse
  Serial.print("hello");
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

  // Print the calculated values on OLED
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  display.print("Person's Height: ");
  display.print(personHeight, 2);
  display.println(" ft");

  display.display();

  // Publish the person's height to MQTT
  char message[10];
    snprintf(message, sizeof(message), "%.2f", personHeight);
  client.publish("height", message);

  // Print the calculated values on Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distanceFeet, 2);
  Serial.print(" feet\t");
  Serial.print("Person's Height: ");
  Serial.print(personHeight, 2);
  Serial.println(" feet");

  delay(1000); // Delay before the next measurement
}

void callback(char* topic, byte* payload, unsigned int length) {
  // Handle incoming MQTT messages if needed
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    if (client.connect("ESP8266Client", mqtt_username, mqtt_password)) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" Trying again in 5 seconds");
      delay(5000);
    }
  }
}
