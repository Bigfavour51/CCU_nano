// UART implementation for slave mode
// Provides a simple interface to communicate with a master CYD ESP32 screen device using UART.

#ifndef __UART_SLAVE
#define __UART_SLAVE
#include <Arduino.h>

String incoming = ""; // Variable to store incoming data
void uart_master_setup(); // Function to set up UART communication 
bool parseMasterData(String data, float*values); // Function to parse incoming data
void respondWithSix(float* inputs); // Function to respond with six values
void uart_perform_function(); // Function to perform UART operations


void uart_master_setup() {
    Serial.begin(9600); // Initialize UART communication at 115200 baud rate
    delay(500); // Wait for the serial port to initialize
}
bool parseMasterData(String data, float*values)
{
    const char* keys[] = {"temp", "humd", "power_cutoff"};
    const int keycount = 3;
    bool found[keycount] = {false, false, false}; // Array to track if keys are found

    int start = 0;
    while (start < data.length())
    {
        int sep = data.indexOf(':', start);
        if (sep == -1)  sep = data.length();

        String pair = data.substring(start, sep);
        int equal = pair.indexOf('=');

        if (equal != -1)
        {
            String key = pair.substring(0, equal);
            String value = pair.substring(equal + 1);

            for (int i = 0; i < keycount; i++)
            {
                if (key == keys[i])
                {
                    values[i] = value.toFloat();
                    found[i] = true;
                    break;
                }
            }
        }
        start = sep + 1;
    }
        // Check if all keys were found
    for (int i = 0; i < keycount; i++)
    {
        if (!found[i])
        {
            return false; // Return false if any key is not found
        }
    }
}
void respondWithSix(float* inputs) {
    float out[6] = {
        inputs[0] * 0.5,
        inputs[1] * 0.8,
        inputs[2] * 1.2,
        inputs[0] + inputs[1],
        inputs[1] + inputs[2],
        inputs[2] - inputs[0]
    };

    const char* keys[6] = {
        "scaled1", "scaled2", "scaled3", "sum1", "sum2", "diff"
    };

    String response = "";
    for (int i = 0; i < 6; i++) {
        response += keys[i];
        response += "=";
        response += String(out[i], 2);
        if (i < 5) response += ";";
    }

    Serial.println(response);
    Serial.flush(); // Ensure data is pushed out completely
}
void uart_perform_function() {
   while(Serial.available())
   {
    char c = Serial.read();

    if (c == '\n'){
        incoming.trim(); // Remove any leading or trailing whitespace
        Serial.println("Received: " + incoming); // Print the received data

        float values[3] = {0.0, 0.0, 0.0}; // Array to store parsed values
        if (parseMasterData(incoming, values)){
            respondWithSix(values); // Call the function to respond with six values
        }

        incoming = ""; // Clear the incoming string for the next message
    }
    else{
        incoming += c; // Append the character to the incoming string
    }
   }
}


#endif // __UART_SLAVE