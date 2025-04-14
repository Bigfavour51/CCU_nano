// Header implementation for Ultrasonic Sensor
// Provides a simple interface to measure distance using ultrasonic waves.

#ifndef __ULTRASENSOR
#define __ULTRASENSOR

#include <Arduino.h>
#include <Ultrasonic.h>  
#include <uart_master.h>

#define TRIG_PIN 9
#define ECHO_PIN 6

int AVsample = 20; // Number of samples for averaging

// Declare sensor globally (only once)
Ultrasonic ultrasonic(TRIG_PIN, ECHO_PIN); // Create an instance of the Ultrasonic class
int distanceValue; // variable to store distance values




// Function to get distance in centimeters
int getDistance() {
   if(isnan(distanceValue = ultrasonic.read())) { // Read distance from the ultrasonic sensor
      return 0.00; // Return 0 if distance is not a number
   } else {
         for (int i = 0; i < AVsample; i++) { // Loop to average the distance readings
            distanceValue += ultrasonic.read(); // Add the distance reading to the variable
         }
         distanceValue /= AVsample; // Calculate the average distance
      return distanceValue; // Return the measured distance
   }
 
}

bool UnitFillChecker = (getDistance() == DistanceThreshold) ? true : false;

#endif // __ULTRASENSOR
