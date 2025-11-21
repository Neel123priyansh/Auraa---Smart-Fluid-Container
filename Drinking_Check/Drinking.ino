#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#define TRIG_PIN D6
#define ECHO_PIN D5 

Adafruit_MPU6050 mpu;

float previousWaterLevel = 0.0;
float currentWaterLevel = 0.0;

void setup() {
  Serial.begin(115200);
  if (!mpu.begin()) {
    Serial.println("MPU6050 initialization failed!");
    while (1);
  }
  Serial.println("MPU6050 initialized!");
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  float tiltAngle = atan2(a.acceleration.x, a.acceleration.z) * 180 / 3.14159;

  if (tiltAngle > 40 && tiltAngle < 90) {
    Serial.println("Bottle is tilted. Checking water level...");

    currentWaterLevel = measureWaterLevel();
    
    if (currentWaterLevel < previousWaterLevel - 0.5) { 
      Serial.println("User is drinking!");
    } else {
      Serial.println("No drinking detected.");
    }
    
    previousWaterLevel = currentWaterLevel;
  }
  
  delay(500); 
}

float measureWaterLevel() {
  long duration;
  float distance;
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration / 2.0) * 0.0343; 

  Serial.print("Water Level: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  return distance;
}
