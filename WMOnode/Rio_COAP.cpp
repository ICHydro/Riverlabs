#include "Arduino.h"
#include "Rio_COAP.h"

#define LOGGING

void CoapPacket::addOption(uint8_t number, uint8_t length, uint8_t *opt_payload) {
    options[optionnum].number = number;
    options[optionnum].length = length;
    options[optionnum].buffer = opt_payload;

    ++optionnum;
}

void CoapPacket::parseMessage(uint8_t *message, uint16_t length) {
    type = (message[0] & 0x30) >> 4;    // use bitmask 00110000 = 0x30 = 48
    tokenlen = (uint8_t) (message[0] & 0x0F);
    code = (uint8_t) message[1];
    
    messageid = 0xFF00 & (message[2] << 8);   // from coap-simple
    messageid |= 0x00FF & message[3];

    // messageid = message[3] << 8 | message[2];
    if(tokenlen > 0) {
        for(uint8_t i = 0; i < tokenlen; i++) {
            token[i] = message[4+i];
        }
    }
    // TODO: parse options
}

// following code adapted from Coap::sendPacket in coap-light.cpp

uint8_t CoapPacket::createMessage(uint8_t buffer[]) {

    uint16_t running_delta = 0;
    uint16_t packetSize = 0;
    uint8_t *p = buffer;

    // make coap packet base header
    *p = 0x01 << 6;
    *p |= (type & 0x03) << 4;
    *p++ |= (tokenlen & 0x0F);
    *p++ = code;
    *p++ = (messageid >> 8);
    *p++ = (messageid & 0xFF);
    p = buffer + COAP_HEADER_SIZE;
    packetSize += 4;

    // make token
    if (token != NULL && tokenlen <= 0x0F) {
        memcpy(p, token, tokenlen);
        p += tokenlen;
        packetSize += tokenlen;
    }

    // make option header
    for (int i = 0; i < optionnum; i++)  {
        uint32_t optdelta;
        uint8_t len, delta;

        if (packetSize + 5 + options[i].length >= COAP_BUF_MAX_SIZE) {
            return 0;
        }
        optdelta = options[i].number - running_delta;
        COAP_OPTION_DELTA(optdelta, &delta);
        COAP_OPTION_DELTA((uint32_t)options[i].length, &len);

        *p++ = (0xFF & (delta << 4 | len));
        if (delta == 13) {
            *p++ = (optdelta - 13);
            packetSize++;
        } else if (delta == 14) {
            *p++ = ((optdelta - 269) >> 8);
            *p++ = (0xFF & (optdelta - 269));
            packetSize+=2;
        } if (len == 13) {
            *p++ = (options[i].length - 13);
            packetSize++;
        } else if (len == 14) {
            *p++ = (options[i].length >> 8);
            *p++ = (0xFF & (options[i].length - 269));
            packetSize+=2;
        }

        memcpy(p, options[i].buffer, options[i].length);
        p += options[i].length;
        packetSize += options[i].length + 1;
        running_delta = options[i].number;
    }

    // make payload
    if (payloadlen > 0) {
        if ((packetSize + 1 + payloadlen) >= COAP_BUF_MAX_SIZE) {
            return 0;
        }
        *p++ = 0xFF;
        memcpy(p, payload, payloadlen);
        packetSize += 1 + payloadlen;
    }

    return packetSize;
}

uint8_t CoapPacket::createMessage(EEPROMClass EEPROM) {

    uint16_t running_delta = 0;
    uint16_t packetSize = 0;
    uint16_t s = 0;


    // make coap packet base header
    EEPROM.write(s++, (0x01 << 6) | ((type & 0x03) << 4) | (tokenlen & 0x0F));

    EEPROM.write(s++, code);
    EEPROM.write(s++, (messageid >> 8));
    EEPROM.write(s++, (messageid & 0xFF));
    s = COAP_HEADER_SIZE;   // should be 4 anyway?

    // make token
    if (token != NULL && tokenlen <= 0x0F) {
        for(uint8_t i = 0; i < tokenlen; i++) {
            EEPROM.write(s++, token[i]);
        }
    }

    // make option header
    for (int i = 0; i < optionnum; i++)  {
        uint32_t optdelta;
        uint8_t len, delta;

        if (s + 5 + options[i].length >= COAP_BUF_MAX_SIZE) {
            return 0;
        }
        optdelta = options[i].number - running_delta;
        COAP_OPTION_DELTA(optdelta, &delta);
        COAP_OPTION_DELTA((uint32_t)options[i].length, &len);

        EEPROM.write(s++, 0xFF & (delta << 4 | len));
        if (delta == 13) {
            EEPROM.write(s++, optdelta - 13);
        } else if (delta == 14) {
            EEPROM.write(s++, (optdelta - 269) >> 8);
            EEPROM.write(s++, 0xFF & (optdelta - 269));
        } if (len == 13) {
            EEPROM.write(s++, options[i].length - 13);
        } else if (len == 14) {
            EEPROM.write(s++, options[i].length >> 8);
            EEPROM.write(s++, 0xFF & (options[i].length - 269));
        }

        for(uint8_t j = 0; j < options[i].length; j++) {
            EEPROM.write(s++, options[i].buffer[j]);
        }
        running_delta = options[i].number;
    }

    // make payload
    if (payloadlen > 0) {
        if ((s + 1 + payloadlen) >= EEPROM.length()) {
            return 0;
        }
        EEPROM.write(s++, 0xFF);
        for(uint8_t j = 0; j < payloadlen; j++) {
            EEPROM.write(s++, payload[j]);
        }
    }
    return s;
}


uint8_t CoapPacket::createMessageHeader(EEPROMClass EEPROM) {

    uint16_t running_delta = 0;
    uint16_t packetSize = 0;
    uint16_t s = 0;

    // make coap packet base header
    EEPROM.write(s++, (0x01 << 6) | ((type & 0x03) << 4) | (tokenlen & 0x0F));

    EEPROM.write(s++, code);
    EEPROM.write(s++, (messageid >> 8));
    EEPROM.write(s++, (messageid & 0xFF));
    s = COAP_HEADER_SIZE;   // should be 4 anyway?

    // make token
    if (token != NULL && tokenlen <= 0x0F) {
        for(uint8_t i = 0; i < tokenlen; i++) {
            EEPROM.write(s++, token[i]);
        }
    }

    // make option header
    for (int i = 0; i < optionnum; i++)  {
        uint32_t optdelta;
        uint8_t len, delta;

        if (s + 5 + options[i].length >= COAP_BUF_MAX_SIZE) {
            return 0;
        }
        optdelta = options[i].number - running_delta;
        COAP_OPTION_DELTA(optdelta, &delta);
        COAP_OPTION_DELTA((uint32_t)options[i].length, &len);

        EEPROM.write(s++, 0xFF & (delta << 4 | len));
        if (delta == 13) {
            EEPROM.write(s++, optdelta - 13);
        } else if (delta == 14) {
            EEPROM.write(s++, (optdelta - 269) >> 8);
            EEPROM.write(s++, 0xFF & (optdelta - 269));
        } if (len == 13) {
            EEPROM.write(s++, options[i].length - 13);
        } else if (len == 14) {
            EEPROM.write(s++, options[i].length >> 8);
            EEPROM.write(s++, 0xFF & (options[i].length - 269));
        }

        for(uint8_t j = 0; j < options[i].length; j++) {
            EEPROM.write(s++, options[i].buffer[j]);
        }
        running_delta = options[i].number;
    }
    return s;
}

void CoapPacket::print(Stream &stream) {
    stream.print(F("Type = "));
    stream.println(type);
    stream.print(F("TKL = "));
    stream.println(tokenlen);
    stream.print(F("code = "));
    stream.println(code, HEX);
    stream.print(F("messageID = "));
    stream.println(messageid);
    if(tokenlen > 0) {
        stream.print(F("Token = "));
        for (uint8_t i = 0; i < tokenlen; i++) {
            stream.print(token[i]);
        }
        stream.println();
    }
}

