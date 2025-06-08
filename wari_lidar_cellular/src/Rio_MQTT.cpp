#include "Rio_MQTT.h"

void MQTT_connect(char *clientid, uint8_t clientidsize, char *username, uint8_t usernamesize) {

    clientidsize--;                                 // remove array end
    usernamesize--;                                 // remove array end

    uint8_t buffer[50];                             //18 + clientidsize + usernamesize
    uint8_t i = 0;
    uint8_t s = 0;                                  // buffersize

    // create a connection message
    
    buffer[s++] = 0b00010000;                       // control field: 0001 = connect. Note that QoS flag should not be set here.
    buffer[s++] = clientidsize + usernamesize + 16; // Remaining length.
    buffer[s++] = 0x00;                             // protocol name length (2 bytes)
    buffer[s++] = 0x04;
    buffer[s++] = 0x4D;                             // M
    buffer[s++] = 0x51;                             // Q
    buffer[s++] = 0x54;                             // T
    buffer[s++] = 0x54;                             // T
    buffer[s++] = 0x04;                             // 4
    buffer[s++] = 0b11000010;                       // connect flags (11000010 = C2)
    buffer[s++] = 0x00;                             // keep alive timer (2 bytes)
    buffer[s++] = 0x0F;                             // 15 s
    buffer[s++] = 0x00;                             // clientID length (byte 1)
    buffer[s++] = clientidsize;                     // clientID length (byte 2)
    for(uint8_t i = 0; i < clientidsize; i++) { 
        buffer[s++] = clientid[i];
    }
    buffer[s++] = 0x00;                             // username length (first byte)
    buffer[s++] = usernamesize;                     // assuming that username fits in second byte
    for(uint8_t i = 0; i < usernamesize; i++) {
        buffer[s++] = username[i];
    }    
    buffer[s++] = 0x00;                             // password length(2 bytes)
    buffer[s++] = 0x00;

    // send the message

    tcpSend(IP, Port, protocol, buffer, s);

    // Don't return anything. Any follow up is handled by the main loop.
}

uint8_t CreateMqttHeader(uint8_t *buffer, uint16_t messageid) {

    char topic[] = "v1/devices/me/telemetry";
    uint8_t topiclength = sizeof(topic) - 1;

    uint8_t s = 0;

    buffer[s++] = 0x32;                             // 0b00110010
    buffer[s++] = 2 + topiclength + 2 + 61;         // remaining length. Payload is 61 bytes
    buffer[s++] = 0x00;                             // topic length (1st byte)
    buffer[s++] = topiclength;                      // topid length (2nd byte)

    memcpy(buffer + s, topic, topiclength);
    s = s + topiclength;
    
    buffer[s++] = (messageid >> 8);    // message ID MSB
    buffer[s++] = (messageid & 0xFF);    // message ID LSB

    return(s);

}

void MQTT_send(uint16_t messageid) {
    uint8_t buffer[150];
    uint16_t bufferSize = CreateMqttHeader(buffer, messageid);
    bufferSize = CreateSendBuffer(startposition, Eeprom3Gmask, buffer, bufferSize);
    tcpSend(IP, Port, protocol, buffer, bufferSize);
}