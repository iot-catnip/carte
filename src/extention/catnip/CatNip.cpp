
#include <Arduino.h>
#include "CatNip.h"


CatNip::CatNip() {
    this->frameType = 0;
    this->packetType = 0;
    this->packetLength = 0;
    this->data = 0;
    this->checksum = 0;
    this->macAddress[0]=0;
}

void CatNip::encodeFrame() {
    if (this->frameType == 0 || this->packetLength == 0) {
        this->detectFrameType();
        if (this->frameType == 0 || this->packetLength == 0) {
            throw "UNEXPECTED_ERROR : frameType and/or packetLength not defined but should be defined";
        }
    }

    if (this->packetType == 0) {
        throw "CANT_BE_NULL : packetType can't be null";
    }

    if (this->frameType == CatNip::PACKET_HELLO) {
        if (this->packetLength != CatNip::PACKET_HELLO_LENGTH) {
            Serial.print("ENCODE_FRAME_ERROR : packetLength must have packet hello length");
            return;
        }
        if (this->packetType != CatNip::STATUS_HELLO) {
            Serial.print("ENCODE_FRAME_ERROR : packetType must be status Hello to encode Hello Frame");
            return;
        }
        if (!CatNip::checkMacAddress(this->macAddress)) {
            Serial.print("ENCODE_FRAME_ERROR : mac Address is not defined");
            return;
        }
        this->frame[0] = CatNip::START_FRAME;
        this->frame[1] = this->packetLength;
        this->frame[2] = this->packetType;
        this->frame[3] = this->macAddress[0];
        this->frame[4] = this->macAddress[1];
        this->frame[5] = this->macAddress[2];
        this->frame[6] = this->macAddress[3];
        this->frame[7] = this->macAddress[4];
        this->frame[8] = this->macAddress[5];
        this->frame[9] = CatNip::calculateChecksum(this->frame, 9);
        return;
    }

    if (this->frameType == CatNip::PACKET_DATA) {
        if (this->packetLength != CatNip::PACKET_DATA_LENGTH) {
            throw "ENCODE_FRAME_ERROR : packetLength must have packet data length";
        }
        this->frame[0] = CatNip::START_FRAME;
        this->frame[1] = this->packetLength;
        this->frame[2] = this->packetType;
        this->frame[3] = (this->data >> 8) & 0xFF;
        this->frame[4] = this->data & 0xFF;
        this->frame[5] = CatNip::calculateChecksum(this->frame, 5);
        return;
    }

    if (this->frameType == CatNip::PACKET_STATUS) {
        if (this->packetLength != CatNip::PACKET_STATUS_LENGTH) {
            throw "ENCODE_FRAME_ERROR : packetLength must have packet hello length";
        }
        this->frame[0] = CatNip::START_FRAME;
        this->frame[1] = this->packetLength;
        this->frame[2] = this->packetType;
        this->frame[3] = CatNip::calculateChecksum(this->frame, 3);
        return;
    }
}

boolean CatNip::decodeFrame(unsigned char frame[4]) {
    if (frame[0] == CatNip::START_FRAME) {
        if (frame[1] == CatNip::PACKET_STATUS_LENGTH) {
            if (frame[2] == CatNip::STATUS_ALIVE ||
                frame[2] == CatNip::STATUS_RECEIVED ||
                frame[2] == CatNip::ASK_HUMIDITY ||
                frame[2] == CatNip::ASK_TEMPERATURE ||
                frame[2] == CatNip::ASK_WATT ||
                frame[2] == CatNip::ASK_IF_ON ||
                frame[2] == CatNip::ACTION_ON ||
                frame[2] == CatNip::ACTION_OFF) {
                if (calculateChecksum(frame, 3) == frame[3]) {
                    this->packetType = frame[2];
                    this->detectFrameType();
                    return true;
                }
                Serial.println("DECODE_FRAME_ERROR : Wrong Checksum -> not a CATNIP frame");
                return false;
                //throw "DECODE_FRAME_ERROR : Wrong Checksum -> not a CATNIP frame";
            }
            Serial.println("DECODE_FRAME_ERROR : not supported packet type -> not a CATNIP frame");
            //throw "DECODE_FRAME_ERROR : not supported packet type -> not a CATNIP frame";
            return false;
        }
        Serial.println("DECODE_FRAME_ERROR : Wrong Length -> not a CATNIP frame");
        return false;
        //throw "DECODE_FRAME_ERROR : Wrong Length -> not a CATNIP frame";
    }
    Serial.println("DECODE_FRAME_ERROR : Not a CATNIP frame");
    return false;
    //throw "DECODE_FRAME_ERROR : Not a CATNIP frame";
}

void CatNip::setPacketType(unsigned char packetType) {
    if (packetType == CatNip::STATUS_ALIVE ||
        packetType == CatNip::STATUS_RECEIVED ||
        packetType == CatNip::STATUS_HELLO ||
        packetType == CatNip::DATA_CONSUMATION ||
        packetType == CatNip::DATA_TEMPERATURE ||
        packetType == CatNip::DATA_HUMIDITY ||
        packetType == CatNip::DATA_CONSUMATION ||
        packetType == CatNip::DATA_ON) {
        this->packetType = packetType;
        return;
    }
    throw "UNKNOWN_TYPE_ERROR : unknown packet type client";
}

void CatNip::setMacAddress(uint8_t macAddress[6]) {
    if (CatNip::checkMacAddress(macAddress))
    {
        for (int i = 6 - 1; i >= 0; i--)
        {
            this->macAddress[i]=macAddress[i];
        }
        return;
    }else{
        Serial.println("UNKNOW_TYPE_ERROR : Not mac address");
    }
}

void CatNip::setData(short int data) {
    this->data = data;
}

uint8_t *CatNip::getFrame() {
    return this->frame;
}

int CatNip::getFrameSize(){
    return this->packetLength;
}

uint16_t CatNip::getData(){
    return this->data;
}

boolean CatNip::detectFrameType() {
    if (this->packetType == 0) {
        Serial.println("CANT_BE_NULL : packetType can't be null");
        return false;
        //throw "CANT_BE_NULL : packetType can't be null";
    }

    if (this->packetType == CatNip::STATUS_HELLO) {
        this->frameType = CatNip::PACKET_HELLO;
        this->packetLength = CatNip::PACKET_HELLO_LENGTH;
        return true;
    }

    if (this->packetType == CatNip::STATUS_ALIVE ||
        this->packetType == CatNip::STATUS_RECEIVED ||
        this->packetType == CatNip::ASK_HUMIDITY ||
        this->packetType == CatNip::ASK_TEMPERATURE ||
        this->packetType == CatNip::ASK_WATT ||
        this->packetType == CatNip::ASK_IF_ON ||
        this->packetType == CatNip::ACTION_ON ||
        this->packetType == CatNip::ACTION_OFF) {

        this->frameType = CatNip::PACKET_STATUS;
        this->packetLength = CatNip::PACKET_STATUS_LENGTH;
        return true;
    }

    if (this->packetType == CatNip::DATA_CONSUMATION ||
        this->packetType == CatNip::DATA_HUMIDITY ||
        this->packetType == CatNip::DATA_ON ||
        this->packetType == CatNip::DATA_TEMPERATURE) {

        this->frameType = CatNip::PACKET_DATA;
        this->packetLength = CatNip::PACKET_DATA_LENGTH;
        return true;
    }
    Serial.println("UNKNOWN_TYPE_ERROR frameType");
    return false;
    //throw "UNKNOWN_TYPE_ERROR frameType";
}

bool CatNip::checkMacAddress(uint8_t macAddress[6]) {
    for (unsigned char i = 0; i < 6; i++) {
        if (macAddress[i] == 0) {
            return false;
        }
    }
    return true;
}

unsigned char CatNip::calculateChecksum(unsigned char frame[], char offset) {
    unsigned char sum = 0x0;
    for (unsigned char i = 0; i < offset; ++i) {
        sum += frame[i];
    }
    return sum % 16;
}

unsigned char CatNip::getPacketType() {
    return this->packetType;
}

unsigned char CatNip::getFrameType() {
    return this->frameType;
}