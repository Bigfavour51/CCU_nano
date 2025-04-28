#ifndef I2C_VARIABLES_H
#define I2C_VARIABLES_H

#include "Arduino.h"

//data from pzem004t
float Power = 0.00; //W 
float Energy = 0.00; //Wh --> by 1000 to get guage reading in wh
float Current = 0.0; //A -->multiply by 10 to get guage reading
float Voltage = 0.00; //V
float temperature = 0.00; //C
float humidity = 0.00; //%
bool waterLevel = false; //true if water level is high, false if low


//Preset data tp be sent from screen to meter
int PowerThreshold = 0; //w
int DistanceThreshold = 0; // off after x secs of no motion
int TempThreshold = 0; //off if below x degrees


void setDefaultThresholds()
{
    PowerThreshold = 100; //w
    DistanceThreshold = 30; // off after 30 secs of no motion
    TempThreshold = 16; //off if below 16 degrees
    // alarmTime = 0; //0-255 minutes
}

void setDefaultMeterData(){
   Power = 78.94; //W 
    Energy = 0.00; //Wh --> by 1000 to get guage reading in wh
    Current = 0.65; //A -->multiply by 10 to get guage reading
    Voltage = 220.46; //V
    temperature = 15.26; //C
    humidity = 55.45; //%
    waterLevel = true; //true if water level is high, false if low
}

#endif // I2C_VARIABLES_H