#ifndef __THINGSPEAKDATAPUSH_H__
#define __THINGSPEAKDATAPUSH_H__

#include <WiFi.h>
#include "ThingSpeak.h"
#include "i2c_variables.h"


// Wi-Fi credentials
const char* ssid = "Norahlinks";
const char* password = "12345678";

// ThingSpeak credentials
const char* thingSpeakApiKey = "EUFOSPHFCAKRGQL5";
const long channelNumber = 2920023;
WiFiClient client;

// Timing control
unsigned long lastSendTime = 0;
const unsigned long sendInterval = 20000; // 20 seconds

// Wi-Fi retry timing
unsigned long lastWiFiAttemptTime = 0;
const unsigned long wifiRetryInterval = 60000; // 1 minute
const unsigned long wifiTimeout = 10000;       // 10 seconds to attempt connect

bool wifiConnected = false;

void connectToWiFi() {
    Serial.print("Connecting to Wi-Fi...");
    WiFi.begin(ssid, password);
  
    unsigned long startAttemptTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < wifiTimeout) {
      delay(100);  // brief pause during attempts
      Serial.print(".");
    }
  
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nWi-Fi connected!");
      wifiConnected = true;
      ThingSpeak.begin(client);
    } else {
      Serial.println("\nWi-Fi connection failed.");
      wifiConnected = false;
    }
  
    lastWiFiAttemptTime = millis(); // update last attempt time
}

void Thingspeak_setup() {
  connectToWiFi();
  ThingSpeak.begin(client);
}

void Thingspeak_push_data() {
    unsigned long currentMillis = millis();

    // Retry Wi-Fi connection every 60 seconds if not connected
    if (!wifiConnected && (currentMillis - lastWiFiAttemptTime >= wifiRetryInterval)) {
      connectToWiFi();
    }
  
    // If Wi-Fi drops after being connected, mark as disconnected
    if (wifiConnected && WiFi.status() != WL_CONNECTED) {
      Serial.println("Wi-Fi lost. Will retry in 60 seconds.");
      wifiConnected = false;
      lastWiFiAttemptTime = millis(); // Start retry countdown now
    }
  
    // Send data every 20 seconds if connected
    if (wifiConnected && (currentMillis - lastSendTime >= sendInterval)) {
      lastSendTime = currentMillis;
  
      ThingSpeak.setField(1, Power);
      ThingSpeak.setField(2, Energy);
      ThingSpeak.setField(3, Current); 
      ThingSpeak.setField(4, Voltage);
      ThingSpeak.setField(5, temperature);
      ThingSpeak.setField(6, humidity);
      ThingSpeak.setField(7, waterLevel ? 1 : 0); // Convert boolean to int (1 or 0)

  
      int statusCode = ThingSpeak.writeFields(channelNumber, thingSpeakApiKey);
  
      if (statusCode == 200) {
        Serial.println("Data sent successfully.");
      } else {
        Serial.print("Failed to send data. HTTP error code: ");
        Serial.println(statusCode);
      }
    }
}



#endif // __THINGSPEAKDATAPUSH_H__