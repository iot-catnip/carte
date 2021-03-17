//
// Created by louis on 06/03/2021.
//

#pragma once

class CatNip {
public:

    static const unsigned char START_FRAME = 0x02;

    /*
     * All Packet Type Of CAT NIP
     */
    static const unsigned char STATUS_HELLO = 0x2A;
    static const unsigned char STATUS_ALIVE = 0x2B;
    static const unsigned char STATUS_RECEIVED = 0x2C;
    static const unsigned char ASK_TEMPERATURE = 0x4A;
    static const unsigned char ASK_HUMIDITY = 0x4B;
    static const unsigned char ASK_WATT = 0x4C;
    static const unsigned char ASK_IF_ON = 0x4D;
    static const unsigned char DATA_TEMPERATURE = 0x5A;
    static const unsigned char DATA_HUMIDITY = 0x5B;
    static const unsigned char DATA_CONSUMATION = 0x5C;
    static const unsigned char DATA_ON = 0x5D;
    static const unsigned char DATA_PORT = 0x5F;
    static const unsigned char ACTION_ON = 0x61;
    static const unsigned char ACTION_OFF = 0x60;

    /**
     * Packet Compositor
     */
    static const unsigned char PACKET_HELLO = 0xFF;
    static const unsigned char PACKET_STATUS = 0xBB;
    static const unsigned char PACKET_DATA = 0xDD;

    /**
     * Packet Length
     */
    static const unsigned char PACKET_HELLO_LENGTH = 0x50;
    static const unsigned char PACKET_STATUS_LENGTH = 0x20;
    static const unsigned char PACKET_DATA_LENGTH = 0x30;

    CatNip();

    void encodeFrame();

    boolean decodeFrame(unsigned char[4]);

    static unsigned char calculateChecksum(unsigned char[10],char offset);

    void setPacketType(unsigned char packetType);

    void setMacAddress(uint8_t *macAddress);

    void setData(short int data);

    uint8_t *getFrame();

    unsigned char getPacketType();

    unsigned char getFrameType();

    uint16_t getData();

    int getFrameSize();


    /**
     * CatNip Builder Attribute
     */

private :

    unsigned char frameType;
    unsigned char packetLength;
    unsigned char packetType;
    unsigned short int data;
    unsigned char checksum;
    //Array with Size of 6
    uint8_t macAddress[6];
    //Array with max Size of 10
    uint8_t frame[10];

    boolean detectFrameType();
    static bool checkMacAddress(unsigned char macAddress[6]);

};