#include <DHT.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define DHTPIN D4        // DHT11 data pin
#define DHTTYPE DHT11    // DHT11 sensor type
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const char* ssid = "OnePlus 8 Pro";
const char* password = "1234567890";
const char* mqtt_server = "173.212.249.30";
const char* mqtt_topic = "weather"; // MQTT topic to publish data
const char* mqtt_username = "smps"; // MQTT broker username
const char* mqtt_password = "smps1234"; // MQTT broker password

DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(9600);
  dht.begin();
  setupWiFi();
  client.setServer(mqtt_server, 1883);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Initial setup...");
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  // Wait a few seconds between measurements
  delay(2000);

  // Reading temperature and humidity
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Check if any read failed and exit early (to try again)
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

// Clear the display
  display.clearDisplay();

  // Print the calculated values on OLED
//  display.setTextSize(2);
//  display.setTextColor(SSD1306_WHITE);
//  display.setCursor(0, 0);
//  display.print("Temp: ");
//  display.println(temperature);
//  
//  display.print("Humidity: ");
//  display.print(humidity);
//
//  display.display();
  // Publish data to MQTT
  char message[100];
  snprintf(message, sizeof(message), "%.2f,%.2f", temperature, humidity);
  client.publish(mqtt_topic, message);

  // Print temperature and humidity to the serial monitor
  Serial.print("Temp: ");
  Serial.println(temperature);
  Serial.println(" °C");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
}

void setupWiFi() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
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
