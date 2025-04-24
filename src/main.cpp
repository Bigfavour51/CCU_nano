#include <Arduino.h>
#include "DallasTemp.h"
#include "Ultrasensor.h"
#include "EnergyMeter.h"
#include "DHTsensor.h"
#include "LoadControl.h"
#include "UART_master.h"
#include "i2c_CloudSend.h"


void setup(){
    Serial.begin(9600);
    dht_humd_setup(); // DHT sensor setup
    dallas_temp_sensor_setup(); // Dallas Temperature sensor setup
    loadControlSetup(); // Load control setup
    i2c_init(); // I2C setup
}

void loop() 
{
    // Read data from the master device
    updateEnergyReadings();
    uart_master_send(); // send master data to slave
    float dallasTemp = getTemperature();
    loadControl(power, dallasTemp);
    i2c_send_data(); // Send data over I2C
    delay(500); // Delay for 1 second before next read
}

