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
    this->client = client;
}

void ClientSocket::restoreConnexion(){
    if (!client.connected())
    {
        Serial.println("Restore Connexion");
        this->setConnexion(this->address,this->port);
    }
    
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
    if (!client.connected()) {
        Serial.println("Not Connected Close Socket");
        this->client.stop();
    }
}

void ClientSocket::checkForRequest(){
    if (client.available())
    {
        unsigned char buffer[10];
        int i = 0;
        while (client.available()) {
            int read;
            read = this->client.read();
            Serial.printf("%x ",read);
            buffer[i]=read;
            i++;
        }
        Serial.printf("\n");
    }
}