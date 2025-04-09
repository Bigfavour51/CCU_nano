/*
Bigfav: This header file is suppose to handle everything requirednto get the values from two seperate potentiometer using Object oriented approach


*/



#ifndef __POTENTIOMETER
#define __POTENTIOMETER

#include <Arduino.h>





class Potentiometer
{
private:
   float POT1Value;
   float POT2Value;
   const int POT1Pin;
   const int POT2Pin;
public:
    Potentiometer(const int pot1, const int pot2);
    ~Potentiometer();
    float getTemp();
};


Potentiometer::Potentiometer(const int pot1, const int pot2)
    : POT1Pin {pot1},POT2Pin {pot2}
{
    
    pinMode(POT1Pin, INPUT);
    pinMode(POT2Pin, INPUT);
    
}

float Potentiometer :: getTemp()
{
   return POT1Value = digitalRead(POT1Pin);

}

#endif