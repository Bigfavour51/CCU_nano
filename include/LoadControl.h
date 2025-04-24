#ifndef __LOADCONTROL_H__
#define __LOADCONTROL_H__
#include <Arduino.h>
#include <uart_master.h>

#define LOAD_PIN A0 // Pin to control the load (relay or switch)
#define LOAD_ON 250 // Define the state to turn on the load
#define LOAD_OFF 0 // Define the state to turn off the load
#define LOAD_ON_DELAY 4000 // Delay in milliseconds before turning off the load
#define LOAD_OFF_DELAY 1000 // Delay in milliseconds before turning on the load



// Function to initialize the load control pin
void loadControlSetup() {
    pinMode(LOAD_PIN, OUTPUT); // Set the load pin as output
    analogWrite(LOAD_PIN, LOAD_ON); // Initialize the load to OFF state
    delay(3000);
    analogWrite(LOAD_PIN, LOAD_OFF);
}

// Function to turn on the load
void turnOnLoad() {
    digitalWrite(LOAD_PIN, LOAD_ON); // Set the load pin to HIGH
}
void turnOffLoad() {
    digitalWrite(LOAD_PIN, LOAD_OFF); // Set the load pin to LOW
}   


void loadControl(float &newPower, float &newTep) {
    // Implementation to check the distance  or temperature level and turn on/off the load accordingly
    // if the temperature is above the threshold or the distance is below the threshold, turn on the load
    // float DallasTemp = getTemperature();
    if((newPower >= PowerThreshold) || (newTep >= TempThreshold))
    {
       turnOnLoad(); // Turn on the load if power or temperature is below the threshold
    }
    else{
        turnOffLoad(); // Turn off the load if power or temperature exceeds the threshold
        
    }
   
}

#endif
