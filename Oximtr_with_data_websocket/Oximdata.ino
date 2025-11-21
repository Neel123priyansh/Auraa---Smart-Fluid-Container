#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <FirebaseESP8266.h> // Include the Firebase library
#include "Adafruit_GFX.h"
#include "OakOLED.h"

// Replace with your Firebase project credentials
// const char* FIREBASE_HOST "https://oxibse-default-rtdb.firebaseio.com/" // Your Firebase URL
// const char* FIREBASE_AUTH "UssSBRRKoi1R0jTDIP0kYmKxFqUEiuSTV8tMEY4e" // Firebase Secret Key
const char* ssid = "Priyanshu";  // Enter SSID here
const char* password = "1122334455";  // Enter Password here

// FirebaseData firebaseData;

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");

  // Firebase.begin(&config, &auth);
}

// void loop() {
//   float BPM = 75.0;  // Replace with the actual BPM reading
//   float SpO2 = 98.0; // Replace with the actual SpO2 reading
  
//   if (Firebase.pushFloat(firebaseData, "/health/BPM", BPM)) {
//     Serial.println("BPM sent to Firebase");
//   } else {
//     Serial.println("Error sending BPM: " + firebaseData.errorReason());
//   }

//   if (Firebase.pushFloat(firebaseData, "/health/SpO2", SpO2)) {
//     Serial.println("SpO2 sent to Firebase");
//   } else {
//     Serial.println("Error sending SpO2: " + firebaseData.errorReason());
//   }

//   delay(1000); // Send data every second
// }
