// Description: UART Slave Header File
// File Name: uart_slave.h
// File Author: AINA FAVOUR OLALEKAN (BIGFAVOUR).

// #ifndef __UART_SLAVE
// #define __UART_SLAVE

#include "i2c_variables.h"


// === Constants ===
#define NUM_THRESHOLDS 7
#define INPUT_BUFFER_SIZE 64
#define MSG_HEADER '#' // optional: use for identifying start of command
#define MSG_TERMINATOR '\n'
#define SLAVE_DEBUG_BAUDRATE 115200 //bigfav: cahnged from 115200 for debugging
#define SLAVE_BAUDRATE 9600

// === Global Variables ===
float masterValues[NUM_THRESHOLDS];



void uart_slave_setup() {
//   Serial.begin(SLAVE_DEBUG_BAUDRATE);     // Debug monitor
  Serial2.begin(SLAVE_BAUDRATE, SERIAL_8N2, 22, 27);  // RX=22, TX=27 (adjust pins if needed)

  delay(2000);
  Serial.println("slave Ready");
}


// === UART Read Function ===
void uart_slave_read()
{
    Serial.println("Reading from master...");
    // delay(1000); // Optional delay for stability
    //Read from Serial2 (UART) and process the incoming data

    static String input = "";

    while (Serial2.available()) {
        char c = Serial2.read();

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
                masterValues[idx++] = atof(token);
                token = strtok(NULL, ",");
            }

            if (idx == NUM_THRESHOLDS) {
                // Assign thresholds from master
                Power = masterValues[0];
                Energy= masterValues[1];
                Current = masterValues[2];
                Voltage= masterValues[3];
                temperature = masterValues[4];
                humidity = masterValues[5];
                waterLevel= (bool)masterValues[6];

                /*

                Serial.print("Received from master: ");
                Serial.print(Power); Serial.print(", ");   
                Serial.print(Energy); Serial.print(", ");
                Serial.print(Current); Serial.print(", ");
                Serial.print(Voltage); Serial.print(", ");
                Serial.print(temperature); Serial.print(", ");
                Serial.print(humidity); Serial.print(", ");
                Serial.print(waterLevel); Serial.println();*/



                String message = String( "#" ) + "," + String(PowerThreshold) + "," + String(DistanceThreshold) + "," + String(TempThreshold) + "\n";
                Serial2.print(message);
                Serial.print("Sent to master: ");
                Serial.println(message);
           
            input = ""; // reset input buffer
            } 
                

              
        }
       
    }
}

