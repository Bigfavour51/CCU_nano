#include <Arduino.h>
#include <DallasTemp.h>
#include <Ultrasensor.h>
#include <EnergyMeter.h>
#include <DHTsensor.h>
#include <LoadControl.h>

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
    // Read and print temperature and humidity values
    float temperature = getTemperature();
    float humidity = getHumd();
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
    
     // Read and print distance from ultrasonic sensor
     float distance = getDistance();
     Serial.print("Distance: ");
     Serial.print(distance);
     Serial.println(" cm");
    
    // Read and print energy meter values
    updateEnergyReadings();
    Serial.print("Voltage: ");
    Serial.print(voltage);
    Serial.println(" V");
    
    delay(2000); // Delay for 2 seconds before the next reading
    Serial.print("Current: ");
    Serial.print(current);
    Serial.println(" A");
    Serial.print("Power: ");
    Serial.print(power);
    Serial.println(" W");
    Serial.print("Energy: ");
    Serial.print(energy);
    Serial.println(" Wh");
    Serial.print("Frequency: ");
    Serial.print(frequency);
    Serial.println(" Hz");
    Serial.print("Power Factor: ");
    Serial.print(powerFactor);
    Serial.println(" PF");
    Serial.println("------------------------");
    delay(2000); // Delay for 2 seconds before the next reading

  
}

