//
// Created by louis on 13/03/2021.
//

#include <Arduino.h>
#include "ClientSocket.h"

void ClientSocket::setConnexion(const char *address, uint16_t port){
    WiFiClient client;
    this->address = address;
    this->port = port;
    while (!client.connect(address,port))
    {
        Serial.println("Connection to host failed");
        delay(1000);
        return;
    }
    Serial.println("Connected");
    this->client = client;
    this->awaitResponse = false;
}

void ClientSocket::restoreConnexion(){
    if (!client.connected())
    {
        Serial.println("Restore Connexion");
        this->setConnexion(this->address,this->port);
    }
    
}

void ClientSocket::sendPacket(CatNip cat){
    Serial.println("Send Frame");
    if (cat.getFrameSize()!=0)
    {
        this->client.write(cat.getFrame(),cat.getFrameSize()/8);
    }
    else{
        try
        {
            cat.encodeFrame();
            this->client.write(cat.getFrame(),cat.getFrameSize()/8);
        }
        catch(const std::exception& e)
        {
            Serial.println(*e.what() << '\n');
        }
    }
}

void ClientSocket::disconect(){
    if (!client.connected()) {
        Serial.println("Not Connected Close Socket");
        this->client.stop();
    }
}

void ClientSocket::checkForRequest(){
    if (client.available())
    {
        unsigned char buffer[4];
        int i = 0;
        while (client.available()||i<4) {
            int read;
            read = this->client.read();
            buffer[i]=read;
            i++;
        }
        CatNip cat;
        if(cat.decodeFrame(buffer)){
            Serial.println("Receive Fram");
            try{
                this->evaluateRequest(cat);
            }catch(const std::exception& e)
            {
                Serial.println(*e.what() << '\n');
            }
        }
    }
    this->sendAwaitFrame();
}

void ClientSocket::evaluateRequest(CatNip cat){
    //@todo : Faire des classe de récupération des data
    CatNip catResponse;
    switch (cat.getPacketType())
    {
    case CatNip::ASK_HUMIDITY:
        catResponse.setPacketType(CatNip::DATA_HUMIDITY);
        catResponse.setData(802); //equiv 80.5%
        setupAwaitFrame(catResponse);
        break;
    case CatNip::ASK_TEMPERATURE:
        catResponse.setPacketType(CatNip::DATA_TEMPERATURE);
        catResponse.setData(215); //equiv 21.5°c 
        setupAwaitFrame(catResponse);
        break;
    case CatNip::ASK_WATT:
        catResponse.setPacketType(CatNip::DATA_CONSUMATION);
        catResponse.setData(1140); 
        setupAwaitFrame(catResponse);
        break;
    case CatNip::ASK_IF_ON:
        catResponse.setPacketType(CatNip::DATA_CONSUMATION);
        catResponse.setData(1); 
        setupAwaitFrame(catResponse);
        break;
    case CatNip::ACTION_ON:
        //démarer la prise
        catResponse.setPacketType(CatNip::STATUS_ALIVE);
        setupAwaitFrame(catResponse);
        break;
    case CatNip::ACTION_OFF:
        //arreter la prise
        catResponse.setPacketType(CatNip::STATUS_ALIVE);
        setupAwaitFrame(catResponse);
        break;
    case CatNip::STATUS_RECEIVED:
        this->unsetAwaitFrame();
        break;
    }    
}

void ClientSocket::setupAwaitFrame(CatNip cat){
    Serial.println("Setup await");
    this->sendFrame = cat;
    this->awaitResponse = true;
}

void ClientSocket::unsetAwaitFrame(){
    Serial.println("End communication");
    this->awaitResponse=false;
    CatNip cat;
    this->sendFrame = cat;
}

void ClientSocket::sendAwaitFrame(){
    if(this->awaitResponse){
        try{
            this->sendPacket(this->sendFrame);
        }catch(const std::exception& e)
        {
            Serial.println(*e.what() << '\n');
        }
    }
}