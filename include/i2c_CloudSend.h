#ifndef __I2C_CLOUDSEND_H__
#define __I2C_CLOUDSEND_H__ 

#include <Wire.h> // Include the Wire library for I2C communication
#include <Arduino.h> // Include the Arduino library for basic functions
#include "uart_master.h"

#define slaveAddress 0x08 // Define the I2C slave address
// Function to initialised i2c communication
void i2c_init() {
    Wire.begin(); // Start I2C communication as a master
    Wire.setClock(100000); // Set I2C clock speed to 100kHz
}
// Function to send data over I2C
void i2c_send_data() {
    delay(500);
    Wire.beginTransmission(slaveAddress); // Begin I2C transmission to the specified slave address

    // Send the float as 4 bytes
    Wire.write((byte*)&power, sizeof(power));
    Wire.write((byte*)&energy, sizeof(energy));
    Wire.write((byte*)&current, sizeof(current));
    Wire.write((byte*)&voltage, sizeof(voltage));
    Wire.write((byte*)&i2c_tempValue, sizeof(i2c_tempValue));
    Wire.write((byte*)&i2c_HumdValue, sizeof(i2c_HumdValue));
    Wire.write((byte*)&UnitFillChecker, sizeof(UnitFillChecker)); // Send the boolean value    

    Wire.endTransmission(); // End I2C transmission

   
}


#endif // __I2C_CLOUDSEND_H__