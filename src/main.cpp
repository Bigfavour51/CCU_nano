#include <Arduino.h>
#include <DallasTemp.h>
#include <Ultrasensor.h>
#include <EnergyMeter.h>
#include <DHTsensor.h>
#include <LoadControl.h>
#include <UART_master.h>

float distance {};

void setup(){
    Serial.begin(9600);
    Serial.println("Setup started");   
    // Initialize sensors
    dht_humd_setup(); // DHT sensor setup
    dallas_temp_sensor_setup(); // Dallas Temperature sensor setup
    loadControlSetup(); // Load control setup
}

void loop() 
{
    // Read data from the master device
    updateEnergyReadings();
    uart_master_send(); // send master data to slave
    delay(1000); // Delay for 1 second before next read
}

