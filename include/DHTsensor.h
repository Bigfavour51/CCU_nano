#ifndef __DHTsensor
#define __DHTsensor

#include <Arduino.h>
#include <DHT.h>

#define DHTTYPE DHT22 
#define DHTPIN 2


DHT dht(DHTPIN, DHTTYPE);


float tempValue;
float HumdValue;


void temp_sensor_setup();
float getTemp();
float getHumd();



void temp_sensor_setup()
{
     pinMode(DHTPIN, INPUT);
     dht.begin();
}

float getTemp()
{

   if (!isnan(tempValue = dht.readTemperature())) {
    return tempValue;
   }
   else{
    return 0.00;
   }
}

float getHumd()
{
   if (!isnan(HumdValue = dht.readHumidity())) {
    return tempValue = dht.readTemperature();
   }
   else {
    return 0.00;
   }
}

#endif