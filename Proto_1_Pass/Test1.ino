#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// Pin definitions
#define UV_LED_PIN D4       // UV-C LED pin
#define TRIG_PIN D6         // Ultrasonic sensor TRIG pin
#define ECHO_PIN D5         // Ultrasonic sensor ECHO pin

// MPU6050 object
Adafruit_MPU6050 mpu;

// Constants
const unsigned long UV_DURATION_MS = 60000; // 1 minute for UV-C LED

// Variables
float previousWaterLevel = 0.0;
float currentWaterLevel = 0.0;
bool uvOn = false;

void setup() {
  Serial.begin(115200);

  // Initialize MPU6050
  if (!mpu.begin()) {
    Serial.println("MPU6050 initialization failed!");
    while (1);
  }
  Serial.println("MPU6050 initialized!");

  // Initialize ultrasonic sensor pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Initialize UV LED pin
  pinMode(UV_LED_PIN, OUTPUT);
  digitalWrite(UV_LED_PIN, LOW);

  Serial.println("System initialized. Starting countdown...");
}

void loop() {
  // Countdown Timer (3 seconds)
  for (int i = 3; i > 0; i--) {
    Serial.print("Starting in: ");
    Serial.println(i);
    delay(1000);
  }
  Serial.println("Process started!");

  // Measure initial water level
  previousWaterLevel = measureWaterLevel();
  Serial.print("Initial Water Level: ");
  Serial.print(previousWaterLevel);
  Serial.println(" cm");

  // Check tilt and monitor water level
  monitorDrinking();

  // Turn on UV-C LED for sanitization
  uvOn = true;
  unsigned long uvStartTime = millis();
  digitalWrite(UV_LED_PIN, HIGH);
  Serial.println("UV-C LED turned on. Sanitizing...");

  // Wait for 1 minute while UV-C LED is on
  while (millis() - uvStartTime < UV_DURATION_MS) {
    delay(100); // Non-blocking delay
  }

  // Turn off UV-C LED
  digitalWrite(UV_LED_PIN, LOW);
  uvOn = false;
  Serial.println("UV-C sanitization complete. Turning off UV-C LEDs.");

  // System is ready for the next operation
  Serial.println("System ready for the next operation.");
  delay(3000); // Delay before restarting the loop
}

float measureWaterLevel() {
  long duration;
  float distance;

  // Send a 10us pulse to TRIG pin
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read the ECHO pin, and calculate the distance
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration / 2.0) * 0.0343; // Convert to cm

  // Print and return the measured distance
  Serial.print("Water Level: ");
  Serial.print(distance);
  Serial.println(" cm");

  return distance;
}

void monitorDrinking() {
  sensors_event_t a, g, temp;

  // Monitor tilt and detect drinking
  for (int i = 0; i < 5; i++) { // Monitor for 5 readings
    // Get MPU6050 data
    mpu.getEvent(&a, &g, &temp);

    // Calculate tilt angle
    float tiltAngle = atan2(a.acceleration.x, a.acceleration.z) * 180 / 3.14159;

    // Check if the bottle is tilted
    if (tiltAngle > 40 && tiltAngle < 90) {
      Serial.println("Bottle is tilted. Checking water level...");

      // Measure current water level
      currentWaterLevel = measureWaterLevel();

      // Check for significant water level change
      if (currentWaterLevel < previousWaterLevel - 0.5) { // Adjust threshold as needed
        float waterDrank = previousWaterLevel - currentWaterLevel;
        Serial.print("User is drinking! Water Drank: ");
        Serial.print(waterDrank);
        Serial.println(" cm");
      } else {
        Serial.println("No drinking detected.");
      }

      // Update previous water level
      previousWaterLevel = currentWaterLevel;
    } else {
      Serial.println("Bottle is not tilted.");
    }

    delay(1000); // Delay between checks
  }
}
