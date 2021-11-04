//
// Created by louis on 20/03/2021.
//

#pragma once
#include <SPI.h>
#include <DHT.h>
#include <ACS712.h>
#include <max6675.h>

class Sensors
{
private:
    DHT* dht;
    MAX6675* thermocouple;
    ACS712* acsCurrent;
    int zeroAcs;
public:
    Sensors();
    void init();
    float getHumidity();
    float getTemperature();
    float getTermoTemperature();
    float getAvgTemperature();
    float getCourant();
    float getPower();
};