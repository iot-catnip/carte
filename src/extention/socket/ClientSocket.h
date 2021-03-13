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
        void setConnexion(char * address, int port);
    private:
        WiFiClient client;
};