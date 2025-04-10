// Implementation of the EnergyMeter class
// Created on: 2023-10-01

#ifndef ENERGYSENSOR_H
#define ENERGYSENSOR_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <PZEM004Tv30.h>

// === User Configuration ===
#define PZEM_RX_PIN 8   // Connect to TX of PZEM
#define PZEM_TX_PIN 7   // Connect to RX of PZEM

// === Create Software Serial Port for PZEM ===
SoftwareSerial pzemSerial(PZEM_RX_PIN, PZEM_TX_PIN); // RX, TX
PZEM004Tv30 pzem(pzemSerial);

// === Sensor Values ===
float voltage {};
float current {};
float power {};
float energy {};
float frequency{};
float powerFactor {};


// === Update Readings ===
void updateEnergyReadings() {
    voltage     = pzem.voltage();
    current     = pzem.current();
    power       = pzem.power();
    energy      = pzem.energy();
    frequency   = pzem.frequency();
    powerFactor = pzem.pf();
}


#endif