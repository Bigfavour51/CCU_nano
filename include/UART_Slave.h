// Description: UART Slave Header File
// File Name: uart_slave.h
// File Author: AINA FSVOUR OLALEKAN (BIGFAVOUR).

#ifndef __UART_SLAVE
#define __UART_SLAVE

#include <i2c_variables.h>

// === Constants ===
#define NUM_THRESHOLDS 7
#define INPUT_BUFFER_SIZE 64
#define MSG_HEADER '#' // optional: use for identifying start of command
#define MSG_TERMINATOR '\n'
#define MASTER_BAUDRATE 115200
#define SLAVE_BAUDRATE 9600

// === Global Variables ===
float masterValues[NUM_THRESHOLDS];
float PowerValue, EnergyValue, CurrentValue, VoltageValue;
float temperatureValue, humidityValue;
bool DistanceLevelValue; // true = water level high, false = low





void uart_master_setup() {
  Serial.begin(MASTER_BAUDRATE);     // Debug monitor
  Serial2.begin(SLAVE_BAUDRATE, SERIAL_8N1, 16, 17);  // RX=16, TX=17 (adjust pins if needed)

  delay(2000);
  Serial.println("Master Ready");
}
// === UART Read Function ===
void uart_slave_read()
{
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
                masterValues[idx++] = atof(token);
                token = strtok(NULL, ",");
            }

            if (idx == NUM_THRESHOLDS) {
                // Assign thresholds from master
                PowerValue = masterValues[0];
                EnergyValue= masterValues[1];
                CurrentValue = masterValues[2];
                VoltageValue= masterValues[3];
                temperatureValue = masterValues[4];
                humidityValue = masterValues[5];
                DistanceLevelValue= (bool)masterValues[6];
                

                String message = "#" + String(PowerThreshold) + "," + String(DistanceThreshold) + "," + String(TempThreshold) + "\n";
                Serial2.print(message);
                Serial.print("Sent to master: ");
                Serial.println(message);
           
            input = ""; // reset input buffer
        }
    }
}

}





#endif // __UART_SLAVE

