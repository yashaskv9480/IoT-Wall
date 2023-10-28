#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>
#include <Servo.h>

Adafruit_BMP085 bmp;
Servo servo;

void setup() {
  Serial.begin(9600);

  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1);
  }

  servo.attach(9);  // Connect the servo to pin 9
}

void loop() {
  float pressure = bmp.readPressure() / 100.0;  // Convert pressure to hPa

  // Map the pressure value to servo angle range (0 to 180 degrees)
  int servoAngle = map(pressure, 950, 1050, 0, 180);
  servo.write(servoAngle);

  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.print(" hPa  ");
  Serial.print("Servo Angle: ");
  Serial.println(servoAngle);

  delay(1000);  // Update every 1 second
}
