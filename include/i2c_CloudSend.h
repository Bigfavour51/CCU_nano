#ifndef __I2C_CLOUDSEND_H__
#define __I2C_CLOUDSEND_H__ 
#include <Wire.h> // Include the Wire library for I2C communication
#include <Arduino.h> // Include the Arduino library for basic functions

// Function to initialised i2c communication
void i2c_init() {
    Wire.begin(); // Start I2C communication as a master
    Wire.setClock(100000); // Set I2C clock speed to 100kHz
}
// Function to send data over I2C
void i2c_send_data(int slaveAddress, float* data, int dataSize) {
    Wire.beginTransmission(slaveAddress); // Begin I2C transmission to the specified slave address
    for (int i = 0; i < dataSize; i++) {
        Wire.write((byte*)&data[i], sizeof(float)); // Send each float value as bytes
    }
    Wire.endTransmission(); // End I2C transmission
}

#endif // __I2C_CLOUDSEND_H__