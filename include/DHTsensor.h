#ifndef __DHTsensor
#define __DHTsensor

#include <Arduino.h>
#include <DHT.h>

#define DHTTYPE DHT22 
#define DHTPIN 5


DHT dht(DHTPIN, DHTTYPE);


// DHT sensor values
float HumdValue;
float TempV;



void dht_humd_setup();
float getHumd();
float getTemp();



void dht_humd_setup()
{
     pinMode(DHTPIN, INPUT);
     dht.begin();
}

float getHumd()
{
   if (!isnan(HumdValue = dht.readHumidity())) {
    return HumdValue = dht.readHumidity();
   }
   else {
    return 0.00;
   }
}

float getTemp()
{
   if (!isnan(TempV = dht.readTemperature())) {
    return TempV = dht.readTemperature();
   }
   else {
    return 0.00;
   }
}



#endif