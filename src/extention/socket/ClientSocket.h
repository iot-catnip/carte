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
        void sendPacket(CatNip catNip);
        void setConnexion(const char * address, uint16_t port);
        void restoreConnexion();
        void checkForRequest();
        void disconect();
    private:
        bool awaitResponse = false;
        CatNip sendFrame;
        WiFiClient client;
        const char * address;
        uint16_t port;
        void evaluateRequest(CatNip cat);
        void setupAwaitFrame(CatNip cat);
        void unsetAwaitFrame();
        void sendAwaitFrame();
};