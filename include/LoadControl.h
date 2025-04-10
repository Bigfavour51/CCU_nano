#ifndef __LOADCONTROL_H__
#define __LOADCONTROL_H__
#include <Arduino.h>

#define LOAD_PIN 12 // Pin to control the load (relay or switch)
#define LOAD_ON LOW // Define the state to turn on the load
#define LOAD_OFF HIGH // Define the state to turn off the load
#define LOAD_ON_DELAY 4000 // Delay in milliseconds before turning off the load
#define LOAD_OFF_DELAY 1000 // Delay in milliseconds before turning on the load



// Function to initialize the load control pin
void loadControlSetup() {
    pinMode(LOAD_PIN, OUTPUT); // Set the load pin as output
    digitalWrite(LOAD_PIN, LOAD_OFF); // Initialize the load to OFF state
    delay(1000);
    digitalWrite(LOAD_PIN, LOAD_ON); // Turn on the load for a short duration
    delay(LOAD_ON_DELAY); // Wait for the defined ON delay
    digitalWrite(LOAD_PIN, LOAD_OFF); // Initialize the load to OFF state
   
}

// Function to turn on the load
void turnOnLoad() {
    digitalWrite(LOAD_PIN, LOAD_ON); // Set the load pin to HIGH
}
void turnOffLoad() {
    digitalWrite(LOAD_PIN, LOAD_OFF); // Set the load pin to LOW
}   


#endif
