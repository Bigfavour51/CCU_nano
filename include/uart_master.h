// Description: UART Master Header File
// File Name: uart_master.h
// File Author: AINA FAVOUR OLALEKAN (BIGFAVOUR)



#ifndef __UART_MASTER
#define __UART_MASTER

#include <Arduino.h>
#include <DallasTemp.h>
#include "Ultrasensor.h"
#include <EnergyMeter.h>
#include <DHTsensor.h>
#include <LoadControl.h>

// === Constants ===
#define NUM_THRESHOLDS 3
#define INPUT_BUFFER_SIZE 64
#define MSG_HEADER '#' // optional: use for identifying start of command
#define MSG_TERMINATOR '\n'


// === Global Variables ===
float slaveValues[NUM_THRESHOLDS];

float PowerThreshold, DistanceThreshold, TempThreshold;
bool DistanceLevelValue; // true = water level high, false = low

// === UART Read Function ===
void uart_master_send()
{
    String response = String('#') + "," + String(power) + "," + String(energy) + "," + String(current) + "," +
                      String(voltage) + "," + String(getTemp()) + "," +
                      String(getHumd()) + "," + String(UnitFillChecker()) + String("\n");

    Serial.print(response); // send to slave

    static String input = "";

    while (Serial.available()) {
        char c = Serial.read();

        // Optional: skip garbage before the start character
        if (input.length() == 0 && c != MSG_HEADER) {
            continue;
        }

        input += c;

        // Only process when newline is received
        if (c == MSG_TERMINATOR) {
            input.trim();

            // Strip off header if used
            if (input.charAt(0) == MSG_HEADER) {
                input.remove(0, 1);  // remove header
            }

            char inputBuffer[INPUT_BUFFER_SIZE];
            input.toCharArray(inputBuffer, sizeof(inputBuffer));

            char *token = strtok(inputBuffer, ",");
            int idx = 0;
            while (token != NULL && idx < NUM_THRESHOLDS) {
                slaveValues[idx++] = atof(token);
                token = strtok(NULL, ",");
            }

            if (idx == NUM_THRESHOLDS) {
                // Assign thresholds from master
                PowerThreshold = slaveValues[0];
                DistanceThreshold = slaveValues[1];
                TempThreshold = slaveValues[2];

            

            // input = ""; // reset input buffer
            }
        }
    }
}


#endif // __UART_MASTER
