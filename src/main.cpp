#include <Arduino.h>
#include <DallasTemp.h>
#include <Ultrasensor.h>
#include <EnergyMeter.h>
#include <DHTsensor.h>
#include <LoadControl.h>
#include <UART_slave.h>

float distance {};

void setup(){
    Serial.begin(115200);
    Serial.println("Setup started");   
    // Initialize sensors
    dht_humd_setup(); // DHT sensor setup
    dallas_temp_sensor_setup(); // Dallas Temperature sensor setup
    loadControlSetup(); // Load control setup
   
   
}

void loop() 
{
    // Read data from the master device
    uart_slave_read(); // Read data from the master device
    delay(1000); // Delay for 1 second before next read
}

