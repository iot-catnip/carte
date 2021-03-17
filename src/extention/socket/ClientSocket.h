//
// Created by louis on 13/03/2021.
//

#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include "ClientSocket.h"
#include "../catnip/CatNip.h"

class ClientSocket {
    public:
        ClientSocket();
        void setMacAddress(uint8_t[6]);
        void sendPacket(CatNip catNip);
        void setConnexion(const char * address);
        void restoreConnexion();
        void checkForRequest();
        void disconect();
    private:
        static const uint16_t handCheckPort = 7788;
        unsigned char mac[6];
        bool awaitResponse = false;
        CatNip sendFrame;
        WiFiClient client;
        const char * address;
        uint16_t port = 0;
        void evaluateRequest(CatNip cat);
        void setupAwaitFrame(CatNip cat);
        void unsetAwaitFrame();
        void sendAwaitFrame();
};