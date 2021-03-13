//
// Created by louis on 06/03/2021.
//

#ifndef CARTE_CATNIP_H
#define CARTE_CATNIP_H

#endif //CARTE_CATNIP_H

class CatNip {
public:

    static unsigned char START_FRAME;

    /*
     * All Packet Type Of CAT NIP
     */
    static unsigned char STATUS_HELLO;
    static unsigned char STATUS_ALIVE;
    static unsigned char STATUS_RECEIVED;
    static unsigned char ASK_TEMPERATURE;
    static unsigned char ASK_HUMIDITY;
    static unsigned char ASK_WATT;
    static unsigned char ASK_IF_ON;
    static unsigned char DATA_TEMPERATURE;
    static unsigned char DATA_HUMIDITY;
    static unsigned char DATA_CONSUMATION;
    static unsigned char DATA_ON;
    static unsigned char ACTION_ON;
    static unsigned char ACTION_OFF;

    /**
     * Packet Compositor
     */
    static unsigned char PACKET_HELLO;
    static unsigned char PACKET_STATUS;
    static unsigned char PACKET_DATA;

    /**
     * Packet Length
     */
    static unsigned char PACKET_HELLO_LENGTH;
    static unsigned char PACKET_STATUS_LENGTH;
    static unsigned char PACKET_DATA_LENGTH;

    CatNip();

    void encodeFrame();

    void decodeFrame(unsigned char[4]);

    static unsigned char calculateChecksum(unsigned char[10],char offset);

    void setPacketType(unsigned char packetType);

    void setMacAddress(unsigned char[6]);

    void setData(int data);

    unsigned char * getFrame();

    unsigned char getPacketType();

    unsigned char getFrameType();


    /**
     * CatNip Builder Attribute
     */

private :

    unsigned char frameType;
    unsigned char packetLength;
    unsigned char packetType;
    short int data;
    unsigned char checksum;
    //Array with Size of 6
    unsigned char macAddress[6];
    //Array with max Size of 10
    unsigned char frame[10];

    void detectFrameType();
    static bool checkMacAddress(unsigned char macAddress[6]);

};

/**
 * Start frame value
 */
unsigned char CatNip::START_FRAME = 0x02;

/**
 * All Packet Type Var
 */
unsigned char CatNip::STATUS_HELLO = 0x2A;
unsigned char CatNip::STATUS_ALIVE = 0x2B;
unsigned char CatNip::STATUS_RECEIVED = 0x2C;
unsigned char CatNip::ASK_TEMPERATURE = 0x4A;
unsigned char CatNip::ASK_HUMIDITY = 0x4B;
unsigned char CatNip::ASK_WATT = 0x4C;
unsigned char CatNip::ASK_IF_ON = 0x4D;
unsigned char CatNip::DATA_TEMPERATURE = 0x5A;
unsigned char CatNip::DATA_HUMIDITY = 0x5B;
unsigned char CatNip::DATA_CONSUMATION = 0x5C;
unsigned char CatNip::DATA_ON = 0x5D;
unsigned char CatNip::ACTION_ON = 0x61;
unsigned char CatNip::ACTION_OFF = 0x60;

/**
 * Packet Type
 */
unsigned char CatNip::PACKET_HELLO = 0xFF;
unsigned char CatNip::PACKET_STATUS = 0xBB;
unsigned char CatNip::PACKET_DATA = 0xDD;

/**
 * Packet Length
 */
unsigned char CatNip::PACKET_HELLO_LENGTH = 0x50;
unsigned char CatNip::PACKET_STATUS_LENGTH = 0x20;
unsigned char CatNip::PACKET_DATA_LENGTH = 0x30;