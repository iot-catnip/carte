//
// Created by louis on 13/03/2021.
//

#include <Arduino.h>
#include "Sensors.h"
// ------------------- PINS ----------------------
// MAX6575 module (T° sensor)
#define thermo_CLK 18
#define thermo_DO 19
#define thermo_CS 23

// DHT11 module (T° + humidity sensor)
#define DHTPIN 17

// ACS712 module (current sensor)
#define ACSPIN 35


Sensors::Sensors(){
    this->dht = new DHT(DHTPIN, DHT11);
    this->thermocouple = new MAX6675(thermo_CLK, thermo_CS, thermo_DO);
    this->acsCurrent = new ACS712(ACS712_20A, ACSPIN);
};

void Sensors::init(){
    this->dht->begin();
    this->zeroAcs = this->acsCurrent->calibrate();
    this->acsCurrent->setZeroPoint(zeroAcs);
}

float Sensors::getHumidity(){
    return this->dht->readHumidity();
}

float Sensors::getTemperature(){
    return this->dht->readTemperature();
};

float Sensors::getTermoTemperature(){
    return this->thermocouple->readCelsius();
};

float Sensors::getAvgTemperature(){
    return (this->dht->readTemperature()+this->thermocouple->readCelsius())/2;
}

float Sensors::getCourant(){
    return this->acsCurrent->getCurrentAC();
};

float Sensors::getPower(){
    return this->acsCurrent->getCurrentAC()*230;
};
