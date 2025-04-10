#ifndef CONTROLLIB
#define CONTROLLIB


int voltage {};
int current {};
int power {};
int temperature {};
int humdity {};
int level {};

// This header file is design to handkle the sending of 6 data to an MCU that pushes the data to the cloud via i2c

#include <Arduino.h>
#include <Wire.h>


void sendData(int data1, int data2, int data3, int data4, int data5, int data6);
void sendData(int data1, int data2, int data3, int data4, int data5, int data6)
{
    if(data1 > 255 || data2 > 255 || data3 > 255 || data4 > 255 || data5 > 255 || data6 > 255)
    {
        Serial.println("Data out of range");
        return;
    }

    Wire.beginTransmission(8); // transmit to device #8
    Wire.write(data1);         // sends five bytes
    Wire.write(data2);
    Wire.write(data3);
    Wire.write(data4);
    Wire.write(data5);
    Wire.write(data6);
    Wire.endTransmission();    // stop transmitting
}
// This function is used to send the data to the MCU via i2c
// The data is sent in the order of data1, data2, data3, data4, data5, data6    



#endif