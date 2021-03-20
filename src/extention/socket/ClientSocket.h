//
// Created by louis on 13/03/2021.
//

#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include "ClientSocket.h"
#include "../catnip/CatNip.h"
#include "../sensor/Sensors.h"

class ClientSocket {
    public:
        ClientSocket();
        void setMacAddress(uint8_t[6]);
        void sendPacket(CatNip catNip);
        void setConnexion(const char * address);
        void restoreConnexion();
        void checkForRequest(Sensors sensors);
        void disconect();
    private:
        static const uint16_t handCheckPort = 7788;
        unsigned char mac[6];
        bool awaitResponse = false;
        bool forceReconnexion = false;
        CatNip sendFrame;
        WiFiClient client;
        const char * address;
        uint16_t port = 0;
        void evaluateRequest(CatNip cat,Sensors sensors);
        void setupAwaitFrame(CatNip cat);
        void unsetAwaitFrame();
        void sendAwaitFrame();
};