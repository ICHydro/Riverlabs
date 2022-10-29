

#include "Rio_MQTT.h"

void MQTT_connect(uint8_t *buffer, char *clientid, uint8_t clientidsize, char *username, uint8_t usernamesize) {

    uint32_t timeInMillis = 0;
    uint8_t i = 0;

    clientidsize--;                                 // remove array end
    usernamesize--;                                 // remove arran end

    uint8_t s = 0;                                  // buffersize

    // create a connection message
    
    buffer[s++] = 0b00010010;                       // control field: 0001 = connect. Flags = 0010 means QoS = 1
    buffer[s++] = clientidsize + usernamesize + 16; // Remaining length.
    buffer[s++] = 0x00;                             // protocol name length (2 bytes)
    buffer[s++] = 0x04;
    buffer[s++] = 0x4D;               // M
    buffer[s++] = 0x51;               // Q
    buffer[s++] = 0x54;               // T
    buffer[s++] = 0x54;               // T
    buffer[s++] = 0x04;               // 4
    buffer[s++] = 0b11000010;         // connect flags (11000010 = C2)
    buffer[s++] = 0x00;               // keep alive timer (2 bytes)
    buffer[s++] = 0x0F;               // 15 s
    buffer[s++] = 0x00;               // clientID length (byte 1)
    buffer[s++] = clientidsize;       // clientID length (byte 2)
    for(uint8_t i = 0; i < clientidsize; i++) { 
        buffer[s++] = clientid[i];
    }
    buffer[s++] = 0x00;               // username length (first byte)
    buffer[s++] = usernamesize;       // assuming that username fits in second byte
    for(uint8_t i = 0; i < usernamesize; i++) {
        buffer[s++] = username[i];
    }    
    buffer[s++] = 0x00;               // password length(2 bytes)
    buffer[s++] = 0x00;

    // send the message

    tcpSend(IP, Port, protocol, buffer, s);

    // Don't return anything. Any follow us handled by the main loop.
}

uint8_t CreateMqttHeader(uint8_t *buffer, uint16_t messageid) {

    byte header[] = {0x32, 0x58, 0x0, 0x17, 0x76, 0x31, 0x2F, 0x64, 0x65, 0x76, 0x69, 0x63, 0x65, 0x73, 0x2F, 0x6D, 0x65, 0x2F, 0x74, 0x65, 0x6C, 0x65, 0x6D, 0x65, 0x74, 0x72, 0x79};
    
    memcpy(buffer, header, 27);
    bufferSize = 27;
    buffer[bufferSize++] = (messageid >> 8);    // message ID MSB
    buffer[bufferSize++] = (messageid & 0xFF);    // message ID LSB

    return(bufferSize);

}

void MQTT_send(uint16_t messageid) {
    uint8_t[150] buffer;
    uint16_t bufferSize = CreateMqttHeader(buffer, messageid);
    CreateSendBuffer(startposition, Eeprom3Gmask, buffer);
    tcpSend(IP, Port, protocol, buffer, bufferSize);
}
