// Header file for Dallas Temperature Sensor 
#ifndef __DALLASTEMP_H__
#define __DALLASTEMP_H__


#include <OneWire.h>
#include <DallasTemperature.h>
#include <Arduino.h>
#define ONE_WIRE_BUS 2 // Pin where the DS18B20 is connected with a 10k pull-up resistor

float tempValue; // Variable to store temperature value

OneWire oneWire(ONE_WIRE_BUS); // Create a OneWire instance
DallasTemperature sensors(&oneWire); // Create a DallasTemperature instance

void dallas_temp_sensor_setup() {
    sensors.begin(); // Start the Dallas Temperature library
}

float getTemperature() {
    sensors.requestTemperatures(); // Request temperature readings
    tempValue = sensors.getTempCByIndex(0); // Get temperature in Celsius
    return tempValue; // Return the temperature value
}



#endif // __DALLASTEMP_H__