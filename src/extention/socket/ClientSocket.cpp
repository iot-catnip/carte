//
// Created by louis on 13/03/2021.
//

#include <Arduino.h>
#include "ClientSocket.h"

void ClientSocket::setConnexion(const char *address, uint16_t port){
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
    if (cat.getFrameSize()!=0)
    {
        this->client.write(cat.getFrame(),cat.getFrameSize()/8);
    }else{
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
    this->client.stop();
}

void ClientSocket::checkForRequest(){
    char buffer;
    if (client.available()) {
        buffer = this->client.read();
        Serial.print(buffer);
        Serial.printf("\n");
    }
}