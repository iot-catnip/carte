//
// Created by louis on 13/03/2021.
//

#include <Arduino.h>
#include "ClientSocket.h"

ClientSocket::ClientSocket()
{
    this->port = ClientSocket::handCheckPort;
}

void ClientSocket::setMacAddress(uint8_t *macAddress)
{
    for (int i = 6 - 1; i >= 0; i--)
    {
        this->mac[i] = macAddress[i];
    }
}

void ClientSocket::setConnexion(const char *address)
{
    WiFiClient client;
    this->address = address;
    while (!client.connect(address, ClientSocket::handCheckPort))
    {
        Serial.println("Connection to host failed");
        delay(1000);
        return;
    }
    Serial.println("Connected start Handshake");
    this->client = client;

    //Handshake Setup
    CatNip cat;
    cat.setPacketType(CatNip::STATUS_HELLO);
    cat.setMacAddress(this->mac);
    cat.encodeFrame();

    //Await Handshake response
    this->sendFrame = cat;

    this->awaitResponse = true;
}

void ClientSocket::restoreConnexion()
{
    if (!client.connected()||this->forceReconnexion)
    {
        Serial.println("Try to reconnect");
        this->setConnexion(this->address);
    }
}

void ClientSocket::sendPacket(CatNip cat)
{
    Serial.println("Send Frame");
    if (cat.getFrameSize() != 0)
    {
        this->client.write(cat.getFrame(), cat.getFrameSize() / 8);
    }
    else
    {
        try
        {
            cat.encodeFrame();
            this->client.write(cat.getFrame(), cat.getFrameSize() / 8);
        }
        catch (const std::exception &e)
        {
            Serial.println(*e.what() << '\n');
        }
    }
}

void ClientSocket::disconect()
{
    if (!client.connected())
    {
        Serial.println("Not Connected Close Socket");
        this->client.stop();
    }
}

void ClientSocket::checkForRequest(Sensors sensors)
{
    if (client.available())
    {
        unsigned char buffer[6];
        int i = 0;
        String log;
        log = "Incoming transmision : <Buffer";
        while (client.available() || i < 3)
        {
            int read;
            read = this->client.read();
            buffer[i] = read;
            log += " "+String(read,HEX);
            i++;
        }
        log+=">";
        Serial.println(log);

        CatNip cat;
        if (cat.decodeFrame(buffer))
        {
            Serial.println("Receive Frame");
            try
            {
                this->evaluateRequest(cat,sensors);
            }
            catch (const std::exception &e)
            {
                Serial.println(*e.what() << '\n');
            }
        }
    }
    this->sendAwaitFrame();
}

void ClientSocket::evaluateRequest(CatNip cat,Sensors sensors)
{
    CatNip catResponse;
    switch (cat.getPacketType())
    {
    case CatNip::ASK_HUMIDITY:
        catResponse.setPacketType(CatNip::DATA_HUMIDITY);
        catResponse.setData((short)sensors.getHumidity()*10);
        setupAwaitFrame(catResponse);
        break;
    case CatNip::ASK_TEMPERATURE:
        catResponse.setPacketType(CatNip::DATA_TEMPERATURE);
        catResponse.setData((short)sensors.getAvgTemperature()*10); //equiv 21.5°c
        setupAwaitFrame(catResponse);
        break;
    case CatNip::ASK_WATT:
        catResponse.setPacketType(CatNip::DATA_CONSUMATION);
        catResponse.setData((short)sensors.getPower());
        setupAwaitFrame(catResponse);
        break;
    case CatNip::ASK_IF_ON:
        catResponse.setPacketType(CatNip::DATA_ON);
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

void ClientSocket::setupAwaitFrame(CatNip cat)
{
    Serial.println("Setup await");
    this->sendFrame = cat;
    this->awaitResponse = true;
}

void ClientSocket::unsetAwaitFrame()
{
    Serial.println("End communication");
    this->awaitResponse = false;
    CatNip cat;
    this->sendFrame = cat;
}

void ClientSocket::sendAwaitFrame()
{
    if (this->awaitResponse)
    {
        try
        {
            this->sendPacket(this->sendFrame);
        }
        catch (const std::exception &e)
        {
            Serial.println(*e.what() << '\n');
        }
    }
}