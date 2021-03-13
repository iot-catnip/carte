//
// Created by louis on 13/03/2021.
//

#include <Arduino.h>
#include "ClientSocket.h"

void ClientSocket::setConnexion(char *address, int port){
    WiFiClient client;
    while (!client.connect(address,port))
    {
        Serial.println("Connection to host failed");
        delay(1000);
        return;
    }
    this->client = client;
}

void ClientSocket::sendPacket(CatNip cat){
    if (cat.getFrame())
    {
        this->client.write(cat.getFrame(),cat.getFrameSize()/8);
    }    
}