
#include <Arduino.h>
#include "CatNip.h"


CatNip::CatNip() {
    this->frameType = 0;
    this->packetType = 0;
    this->packetLength = 0;
    this->data = 0;
    this->checksum = 0;
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
            throw "ENCODE_FRAME_ERROR : packetLength must have packet hello length";
        }
        if (this->packetType != CatNip::STATUS_HELLO) {
            throw "ENCODE_FRAME_ERROR : packetType must be status Hello to encode Hello Frame";
        }
        if (!CatNip::checkMacAddress(this->macAddress)) {
            throw "ENCODE_FRAME_ERROR : mac Address is not defined";
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
        this->frame[3] = this->data & 0xFF;
        this->frame[4] = (this->data >> 8) & 0xFF;
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

void CatNip::decodeFrame(unsigned char frame[4]) {
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
                    this->detectFrameType();
                    this->packetType = frame[2];
                }
                throw "DECODE_FRAME_ERROR : Wrong Checksum -> not a CATNIP frame";
            }
            throw "DECODE_FRAME_ERROR : not supported packet type -> not a CATNIP frame";
        }
        throw "DECODE_FRAME_ERROR : Wrong Length -> not a CATNIP frame";
    }
    throw "DECODE_FRAME_ERROR : Not a CATNIP frame";
}

void CatNip::setPacketType(unsigned char packetType) {
    if (packetType == CatNip::STATUS_ALIVE ||
        packetType == CatNip::STATUS_RECEIVED ||
        packetType == CatNip::STATUS_HELLO ||
        packetType == CatNip::DATA_CONSUMATION ||
        packetType == CatNip::DATA_HUMIDITY ||
        packetType == CatNip::DATA_CONSUMATION ||
        packetType == CatNip::DATA_ON) {
        this->packetType = packetType;
    }
    throw "UNKNOWN_TYPE_ERROR : unknown packet type client";
}

void CatNip::setMacAddress(unsigned char macAddress[6]) {
    for (int i = 0; i < 7; ++i) {
        if (macAddress[i]==0)
            throw "UNKNOWN_TYPE_ERROR : Not mac address";
    }
    *this->macAddress = *macAddress;
}

void CatNip::setData(int data) {
    this->data = data;
}

uint8_t *CatNip::getFrame() {
    return this->frame;
}

int CatNip::getFrameSize(){
    return this->packetLength;
}

void CatNip::detectFrameType() {
    if (this->packetType == 0) {
        throw "CANT_BE_NULL : packetType can't be null";
    }

    if (this->packetType == CatNip::STATUS_HELLO) {
        this->frameType = CatNip::PACKET_HELLO;
        this->packetLength = CatNip::PACKET_HELLO_LENGTH;
        return;
    }

    if (this->packetType == CatNip::STATUS_ALIVE ||
        this->packetType == CatNip::STATUS_RECEIVED ||
        this->packetType == CatNip::ASK_HUMIDITY ||
        this->packetType == CatNip::ASK_TEMPERATURE ||
        this->packetType == CatNip::ASK_WATT ||
        this->packetType == CatNip::ASK_IF_ON ||
        this->packetType == CatNip::ACTION_ON ||
        this->packetType == CatNip::ACTION_OFF) {

        this->packetType = CatNip::PACKET_STATUS;
        this->packetLength = CatNip::PACKET_STATUS_LENGTH;
        return;
    }

    if (this->packetType == CatNip::DATA_CONSUMATION ||
        this->packetType == CatNip::DATA_HUMIDITY ||
        this->packetType == CatNip::DATA_ON ||
        this->packetType == CatNip::DATA_TEMPERATURE) {

        this->frameType = CatNip::PACKET_DATA;
        this->packetLength = CatNip::PACKET_DATA_LENGTH;
        return;
    }

    throw "UNKNOWN_TYPE_ERROR frameType";
}

bool CatNip::checkMacAddress(unsigned char macAddress[]) {
    if (macAddress[6]) {
        return false;
    }
    for (unsigned char i = 0; i < 6; i++) {
        if (macAddress[i] == 0) {
            return false;
        }
    }
    return true;
}

unsigned char CatNip::calculateChecksum(unsigned char frame[], char offset) {
    char sum = 0x0;
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

int main(int argc, char *argv[]) {
    unsigned char test[4] = {
            0x02,
            0x1A,
            0x16,
            0xF7,
    };
    printf("%d", CatNip::calculateChecksum(test, 4));
    return 0;
}