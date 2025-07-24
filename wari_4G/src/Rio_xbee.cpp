/************************************************************
 * Xbee functions developed by Imperial College London and Riverlabs
 *
 ***********************************************************/

#include "Rio_xbee.h"
#include "Rio.h"

// Wait for cellular registration, signalled by modem status message containing status 2
// Set isRegistered to true when registered on the network

// Print out the contents of the ipResponse object

void printSendAndWaitError(Stream &stream) {
    stream.print(F("sendAndWait() error: "));
}

void printIPRX(IPRxResponse& ipResponse, Stream &stream) {
  stream.println(F("IP Transmission Received"));
  stream.print(F("IP "));
  stream.print(ipResponse.getRemoteIPAddress(), HEX);
  stream.print("(");
  stream.print(ipResponse.getDestinationPort());
  stream.println(")");
  stream.print(F("Source port "));
  stream.println(ipResponse.getSourcePort());
  stream.print(F("Protocol "));
  stream.println(ipResponse.getProtocol());
  stream.print(F("Data length "));
  stream.println(ipResponse.getDataLength());

  if (ipResponse.getDataLength() > 0) {
    stream.print(F("Data: "));

    // Uncomment this loop to print every received byte in HEX
    for (int i = 0; i < ipResponse.getDataLength(); i++) {
      stream.print(ipResponse.getData()[i], HEX);
      stream.print(" ");
    }
    stream.println();

    // Uncomment this loop to print the received message
    //for (int i = 0; i < ipResponse.getDataLength(); i++) {
    //  stream.print((char) ipResponse.getData()[i]);
    //}
    //stream.println("");
  }
}

void checkForRegistrationMessage() {
  // check for modem status messages
  xbc.readPacket();
  if (xbc.getResponse().isAvailable()) {
    // got something
    if (xbc.getResponse().getApiId() == MODEM_STATUS_RESPONSE) {
      // a modem status message was received, check to see whether it contains value 2, 
      xbc.getResponse().getModemStatusResponse(modemStatusResponse);
      int modemStatus = modemStatusResponse.getStatus();
      Serial.print(F("Modem Status Response = "));
      Serial.println(modemStatus, HEX);
      if (modemStatus == 2) {
        //Serial.println(F("Registered with the cellular network"));
        MyXBeeStatus.isRegistered = true;
      }
    } else {
      // print details of other received messages
      printUnexpectedMessage();
    }
  } else if (xbc.getResponse().isError()) {
    Serial.print(F("XBee error: "));
    Serial.println(xbc.getResponse().getErrorCode(), DEC);
    MyXBeeStatus.xbcErrorOccurred = true;
  }
}

//void getAIResponse() {
//  xbc.readPacket(2000);
//  if (xbc.getResponse().isAvailable()) {
//    // got a response!
//
//    // should be an AT command respons
//    if (xbc.getResponse().getApiId() == AT_COMMAND_RESPONSE) {
//      xbc.getResponse().getAtCommandResponse(atResponse);
//      
//      if (atResponse.isOk()) {
//        Serial.println(atResponse.getValue()[0]);
//        if (atResponse.getValueLength() == 1) {
//          if (atResponse.getValue()[0] == 0) {
//            Serial.println(F("Internet connection established"));
//            MyXBeeStatus.isConnected = true;
//          } else {
//            // Force another AI command
//            MyXBeeStatus.aiRequested = false;
//          }
//        } else {
//          // This should never happen
//          Serial.println(F("Failed to get AI value from AI command"));
//          MyXBeeStatus.xbcErrorOccurred = true;
//        }
//      } 
//      else {
//        Serial.print(F("Command returned error code: "));
//        Serial.println(atResponse.getStatus(), HEX);
//       MyXBeeStatus.xbcErrorOccurred = true;
//      }
//    } else {
//      printUnexpectedMessage();
//    }   
//  } else if (xbc.getResponse().isError()) {
//      Serial.print(F("Error reading packet.  Error code: "));  
//      Serial.println(xbc.getResponse().getErrorCode());
//      MyXBeeStatus.xbcErrorOccurred = true;
//  } else {
//    Serial.println(F("No response."));
//  }
//}


void sendDNSLookupCommand(char address[], int len) {
  // LA (lookup address) command
  uint8_t laCmd[] = {'L','A'};
  AtCommandRequest atRequest(laCmd, (uint8_t*)address, len);

  xbc.send(atRequest);
  MyXBeeStatus.dnsLookupRequested = true;
}

// send a TCP message to an address/port on the internet
// Do not wait for the TX response packet

void tcpSend(uint32_t ip, uint16_t port, uint8_t protocol, uint8_t* payload, uint16_t payloadLength) {
  // XBEE IP Transmission object
  // This object is used to send internet transmissions
  IPTxRequest ipRequest(ip, port, payload, payloadLength);
  ipRequest.setProtocol(protocol);

  // send the frame
  xbc.send(ipRequest);
  MyXBeeStatus.ipRequestSent = true;
}

// send a TCP message to an address/port on the internet - read from EEPROM
// Do not wait for the TX response packet

void tcpSend(uint32_t ip, uint16_t port, uint8_t protocol, EEPROMClass, uint16_t payloadLength) {
  // XBEE IP Transmission object
  // This object is used to send internet transmissions
  IPTxRequest ipRequest(ip, port, EEPROM, payloadLength);
  ipRequest.setProtocol(protocol);

  // send the frame
  xbc.send(ipRequest);
  MyXBeeStatus.ipRequestSent = true;
}

// Read and print an incoming packet

void readIPResponse() {
  xbc.readPacket(5000);

  if (xbc.getResponse().isAvailable()) {
    // got something.. print packet to USB
    if (xbc.getResponse().getApiId() == RX_IP4_RESPONSE) {
      // a message was received from the internet, so copy to ipResponse object for convenience
      ipResponse.init(xbc.getResponse());
      #if DEBUG > 1
          printIPRX(ipResponse, Serial);
      #endif
      MyXBeeStatus.ipResponseReceived = true;
    } else {
      printUnexpectedMessage();
      MyXBeeStatus.gotStatusResponse = true;
    }
  } else if (xbc.getResponse().isError()) {
    Serial.print(F("XBee error: "));
    Serial.println(xbc.getResponse().getErrorCode(), DEC);
    MyXBeeStatus.xbcErrorOccurred = true;
  }
}

// print the details of an unexpected message from the modem

void printUnexpectedMessage() {
  if (xbc.getResponse().getApiId() == MODEM_STATUS_RESPONSE) {
    // a modem status message was received
    xbc.getResponse().getModemStatusResponse(modemStatusResponse);
    Serial.print(F("Modem Status Response = "));
    Serial.println(modemStatusResponse.getStatus(), HEX);
    MyXBeeStatus.gotStatusResponse = true;
  } else {
    // Generic print of other packets
    Serial.print(F("API="));
    Serial.print(xbc.getResponse().getApiId(), HEX);
    Serial.print(F(",frame="));

    // print frame data
    for (int i = 0; i < xbc.getResponse().getFrameDataLength(); i++) {
      Serial.print(xbc.getResponse().getFrameData()[i], HEX);
      Serial.print(" ");
    }

    Serial.println("");
  }
}

bool getAIStatus(Stream &stream, uint8_t *returnvalue) {
  // Get the AI status from the modem.
  // A callback could be used, so as not to block, but in this case a quick response is expected
  // as no internet messages need be sent/received, so sendAndWait() is used instead.

  // association status command
  uint8_t assocCmd[] = {'A','I'};
  AtCommandRequest atRequest(assocCmd);
  AtCommandResponse atResponse;
  
  uint8_t status = xbc.sendAndWait(atRequest, 150);
  
  if (status == 0) {
    xbc.getResponse().getAtCommandResponse(atResponse);
    if (atResponse.isOk()) {
      if (atResponse.getValueLength() == 1) {
        if (atResponse.getValue()[0] == 0) {
          *returnvalue = atResponse.getValue()[0];
          MyXBeeStatus.isConnected = true;
          return(1);
        } else {
          // stream.print(F("AI status = "));
          *returnvalue = atResponse.getValue()[0];
          // stream.println(*returnvalue);
          return(1);
        }
      } else {
        // This should never happen
        stream.println(F("Failed to get AI"));
        MyXBeeStatus.xbcErrorOccurred = true;
        return(0);
      }
    } else {
        stream.print(F("Command returned error: "));
        *returnvalue = atResponse.getStatus();
        stream.println(*returnvalue, HEX);
        MyXBeeStatus.xbcErrorOccurred = true;
    }
  } else {
    printSendAndWaitError(stream);
    stream.println(status);
    return(0);
    // TOD); move this out of this function
    MyXBeeStatus.xbcErrorOccurred = true;        
  }
}

bool getDBStatus(Stream &stream, uint8_t *returnvalue) {
  // Get the DB status from the modem.
  // A callback could be used, so as not to block, but in this case a quick response is expected
  // as no internet messages need be sent/received, so sendAndWait() is used instead.

  uint8_t assocCmd[] = {'D','B'};
  AtCommandRequest atRequest(assocCmd, 0, 1);
  AtCommandResponse atResponse;
  
  uint8_t status = xbc.sendAndWait(atRequest, 150);
  
  if (status == 0) {
    xbc.getResponse().getAtCommandResponse(atResponse);
    if (atResponse.isOk()) {
        stream.print(F("Cellular signal strength = "));
        *returnvalue = atResponse.getValue()[0];
        stream.println(*returnvalue, HEX);
        return(1);
    } else {
        stream.print(F("DB Command returned error: "));
        *returnvalue = atResponse.getStatus();
        stream.println(*returnvalue, HEX);
        return(0);
    }
  } else {
    printSendAndWaitError(stream);
    stream.println(status);
    return(0);        
  }
}

bool getAPN(Stream &stream, uint8_t *returnvalue) {
  // Get the APN value from the modem.
  // A callback could be used, so as not to block, but in this case a quick response is expected
  // as no internet messages need be sent/received, so sendAndWait() is used instead.

  uint8_t assocCmd[] = {'A','N'};
  AtCommandRequest atRequest(assocCmd);
  AtCommandResponse atResponse;
  
  uint8_t status = xbc.sendAndWait(atRequest, 150);
  
  if (status == 0) {
    xbc.getResponse().getAtCommandResponse(atResponse);
    if (atResponse.isOk()) {
        stream.print(F("APN = "));
        stream.println(atResponse.getValueLength());
        *returnvalue = atResponse.getValue()[0];
        stream.println((char) *returnvalue);
        return(1);
    } else {
        //stream.print(F("DB Command returned error code: "));
        //*returnvalue = atResponse.getStatus();
        //stream.println(*returnvalue, HEX);
        return(0);
    }
  } else {
    printSendAndWaitError(stream);
    stream.println(status);
    return(0);        
  }
}


// Generic callback function for incoming TCP/IP message

void zbIPResponseCb(IPRxResponse& ipResponse, uintptr_t) {
    // Note that any incoming IP message is considered a success, even if it does not 
    // contain the complete transmission from the web server.
    // This is not reaslistic as a TCP message can be fragmented across multiple packets.
    //Serial.println(F("Callback - IP4 message received"));
    #if DEBUG > 1
        printIPRX(ipResponse, Serial);
    #endif
    MyXBeeStatus.ipResponseReceived = true;
}

// Callback function for incoming NTP message
// Based on code of the Arduino NTPClient library
 
void zbIPResponseCb_NTP(IPRxResponse& ipResponse, uintptr_t) {

    //#if DEBUG > 1
    //    printIPRX(ipResponse, Serial);
    //#endif

    unsigned long highWord = word(ipResponse.getData()[40], ipResponse.getData()[41]);
    unsigned long lowWord = word(ipResponse.getData()[42], ipResponse.getData()[43]);
    
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;

    // Discard if first two bits of response (leap indicator) euqals 3
    // which indicates an error (clock unsyncronised)
    // In that case secSince1900 tends to be zero.
    // We also check that, although it is probably not necessary

    if(((ipResponse.getData()[0] >> 6) != 3) || (secsSince1900 == 0) ) {
        uint32_t secsSince2000 = secsSince1900 - 2208988800UL - 946684800UL;
        Rtc.SetDateTime((RtcDateTime) secsSince2000);
        MyXBeeStatus.ipResponseReceived = true;
    } else {
        #if DEBUG > 1
            Serial.println(F("Invalid NTP response"));
        #endif
    }
}

// Callback function for incoming TCP/IP message
// This one specifically for COAP

void zbIPResponseCb_COAP(IPRxResponse& ipResponse, uintptr_t) {
    // Note that any incoming IP message is considered a success, even if it does not 
    // contain the complete transmission from the web server.
    // This is not realistic as a TCP message can be fragmented across multiple packets.
    //Serial.println(F("Callback - IP4 message received"));
    MyXBeeStatus.ipResponseReceived = true;
    #if DEBUG > 1
        printIPRX(ipResponse, Serial);
    #endif
    CoapPacket cp;
    cp.parseMessage(ipResponse.getData(), ipResponse.getDataLength());
    #if DEBUG > 1
        cp.print(Serial);
    #endif
  
   // TODO: process coap response fully
    if(cp.type == 2) {
      //Serial.println(F("COAP acknowledgement received (2)"));
      MyXBeeStatus.CoapSentAcknowledged = true;
      MyXBeeStatus.ipResponseReceived = 1; 
    }
    if(cp.type == 0) {
        //Serial.println(F("COAP acknowledgeable message received (type = 0)"));
        // send acknowledgement:
        uint8_t buffer[5];    // can be small because it is only an acknowledgement.
        uint8_t packetSize;
        CoapPacket packet;
        
        packet.type = COAP_ACK;
        packet.code = 0;
        packet.tokenlen = 0;
        packet.payloadlen = 0;
        packet.messageid = cp.messageid;
        packetSize = packet.createMessage(buffer);
        
        Serial.print(F("Sending acknowledgement: "));
        for(int i = 0; i < packetSize; i++) {
            Serial.print(buffer[i], HEX);
            Serial.print(" ");
        }
        Serial.println("");
    
        MyXBeeStatus.ipRequestSentOk = false;     // reset this
    
        tcpSend(IP, Port, protocol, (uint8_t*)buffer, packetSize);
    
        uint32_t starttime = millis();
        while(!MyXBeeStatus.ipRequestSentOk && (millis() - starttime < 5000)) {  // wait up to 5 seconds 
            xbc.loop();
        }
    
        if(MyXBeeStatus.ipRequestSentOk) {
            Serial.println(F("Sent."));
        } else {
            //Serial.println(F("Xbee did not (yet) confirm. Assume sent."));
        }
    }

    if(cp.code != 0) {
        if(cp.code == 0x41) {
            Serial.println(F("2.01 confirmation received. Transaction finished"));
            MyXBeeStatus.MessageConfirmed = true;
        }
        if(cp.code == 0x43) {
            Serial.println(F("2.03 confirmation received. Transaction finished"));
            MyXBeeStatus.MessageConfirmed = true;
        }        
    }
}

// Callback function for incoming MQTT messages

void zbIPResponseCb_MQTT(IPRxResponse& ipResponse, uintptr_t) {
  // Note: this code cannot deal with fragmented TCP messages.
  //       I don't know how relevant this is, but incoming packets are likely
  //       small enough to minimize the issue.
  #if DEBUG > 1
      Serial.println(F("Callback - IP4 message received"));
      printIPRX(ipResponse, Serial);
  #endif

  // Identify and process response types

  uint8_t type = ipResponse.getData()[0] >> 4;
  // Serial.println(type);

  // only CONNACK and PUBACK implemented
  
  if(type == 2) {
    if(ipResponse.getDataLength() != 4){
        Serial.println(F("Unexpected data length. Message corrupted?"));
    } else {
        Serial.print(F("CONNACK received"));
        MyXBeeStatus.MqttConnack = true;
        MyXBeeStatus.ipResponseReceived = 1;
        if(ipResponse.getData()[3] == 0) {
             Serial.println(F(": accepted"));
            MyXBeeStatus.MqttConnected = true;
        }
        Serial.println();
    }
  }

  if(type == 4) {
    Serial.print(F("PUBACK received. Message ID: "));
    MyXBeeStatus.MqttPuback = true;
    MyXBeeStatus.MessageConfirmed = true;
    MyXBeeStatus.ipResponseReceived = 1;
    Serial.println((uint16_t) (ipResponse.getData()[2]) << 8 | ipResponse.getData()[3]);
  }
}

// when XBee powers up, connects and disconnects from the internet, it sends modem status messages,
// assumming it's configured correctly, as follows (p.119 Digi XBee Cellular 3g Manual):
// 0 = Hardware reset or power up
// 1 = Watchdog timer reset
// 2 = Registered with cellular network
// 3 = Unregistered with cellular network
// 0x0E = Remote Manager connected
// 0x0F = Remote Manager disconnected

void zbModemStatusCb(ModemStatusResponse& mx, uintptr_t) {
  int modemStatus = mx.getStatus();
  Serial.print(F("Callback - Modem Status Response: "));
  Serial.println(modemStatus, HEX);
  if (modemStatus == 2) {
    //Serial.println(F("Registered with the cellular network"));
    MyXBeeStatus.isRegistered = true;
  } else if (modemStatus == 3) {
    //Serial.println(F("Unregistered with the cellular network"));
    MyXBeeStatus.isRegistered = false;
  } else if (modemStatus == 0) {
    //Serial.println(F("Hardware reset or power up"));
    MyXBeeStatus.isRegistered = false;
  } else if (modemStatus == 0x0E) {
    //Serial.println(F("Remote Manager connected"));
  } else if (modemStatus == 0x0F) {
    //Serial.println(F("Remote Manager disconnected"));
  }
}


void zbAtResponseCb(AtCommandResponse& atr, uintptr_t) {

    // An At response has to be able to deal with different At commands.
    // Currently only LA, AI, and DB are implemented.
    
    if (atr.getCommand()[0] == 'L' || atr.getCommand()[1] == 'A') {
        if (atr.isOk()) {
            if (atr.getValueLength() == 4) {
                // The IP address should be 4 bytes
                Serial.print(F("Callback - IP Address: "));
              
                for (int i = 0; i < atr.getValueLength(); i++) {
                    Serial.print(atr.getValue()[i], HEX);
                    Serial.print(' ');
                }
                Serial.println();
    
                IP = (((uint32_t)atr.getValue()[0]) << 24) + 
                     (((uint32_t)atr.getValue()[1]) << 16) + 
                     (((uint32_t)atr.getValue()[2]) << 8) + 
                     atr.getValue()[3];
                // reset flags
                // not that a postive LA response is the result of an ip request
                MyXBeeStatus.hostIPResolved = true;
                MyXBeeStatus.ipResponseReceived = true;
            }
        } else {
            Serial.print(F("LA command error: "));
            Serial.println(atr.getStatus(), HEX);
            MyXBeeStatus.xbcErrorOccurred = true;
        }
    } else if(atr.getCommand()[0] == 'A' || atr.getCommand()[1] == 'I') {    
        if (atr.isOk()) {
            if (atr.getValueLength() == 1) {
                Serial.print(F("AI status = "));
                Serial.println(atr.getValue()[0], HEX);
                if (atr.getValue()[0] == 0) {
                    MyXBeeStatus.isConnected = true;
                }
            } else {
                // This should never happen
                Serial.println(F("Failed to get AI value"));
                MyXBeeStatus.xbcErrorOccurred = true;
            }
        } else {
            Serial.print(F("AI command error: "));
            Serial.println(atr.getStatus(), HEX);
        } 
    } else if(atr.getCommand()[0] == 'D' || atr.getCommand()[1] == 'B') {
        if (atr.isOk()) {
            Serial.print(F("Cellular signal strength = "));
            Serial.println(atr.getValue()[0]);
        } else {
            Serial.print(F("DB Command error: "));
            Serial.println(atr.getStatus());
        }
    // capture any unrecognized At commands:
    } else {
        Serial.print(F("Callback - AT command not recognized:"));
        Serial.print((char)atr.getCommand()[0]);
        Serial.println((char)atr.getCommand()[1]);
    }
    return;
}

void zbTcpSendResponseCb(TxStatusResponse& txr, uintptr_t) {
  if (txr.isSuccess()) {
    //Serial.println(F("Callback - Transmission was successful!"));
    MyXBeeStatus.ipRequestSentOk = true;
  } else {
    Serial.print(F("Transmission returned error: "));
    Serial.println(txr.getStatus(), HEX);
    MyXBeeStatus.xbcErrorOccurred = true;
  }
}

void sendXbeeMessage(uint8_t* buffer, uint16_t bufferSize, char *host, uint8_t hostlength) {

    // Skip DNS by uncommenting this line:
    //MyXBeeStatus.hostIPResolved = true;

    // Do not wait for ipResponseReceived, which is handled by the callback.
    // Just wait for the XBee confirmation.

    if (!MyXBeeStatus.xbcErrorOccurred && !MyXBeeStatus.ipResponseReceived) {
        xbc.loop();
    
        if (!MyXBeeStatus.hostIPResolved) {
            // This is the DNS lookup section
            // A request is made to lookup an IP address and response handled in callback function zbAIResponseCb
            if (!MyXBeeStatus.dnsLookupRequested) {
              // Send a lookup request command
              Serial.println(F("Sending DNS Lookup")); 
              sendDNSLookupCommand((char*) host, hostlength);
            }
        } else if (!MyXBeeStatus.ipRequestSent) {
            // Send the request
            // The response is handled in callback function zbTcpSendResponseCb
            Serial.print(F("Sending TCP request to "));
            Serial.print(IP, HEX);
            Serial.print(F(", port "));
            Serial.println(Port, HEX);
            tcpSend(IP, Port, protocol, buffer, bufferSize);
        } else if (!MyXBeeStatus.ipResponseReceived) {
          // process incoming IP messages until complete, or host closes the connection.
          // There is nothing to do here as the processing is handled in callback function zbIPResponseCb
        }
    
        // Reset status if successful
        if (!MyXBeeStatus.xbcErrorOccurred && MyXBeeStatus.ipResponseReceived) {
            MyXBeeStatus.ipRequestSent = false;
            MyXBeeStatus.ipRequestSentOk = false;
            MyXBeeStatus.ipResponseReceived = false;
        }
    }
}

/* if no buffer given, we use eeprom: */

void sendXbeeMessage(uint16_t bufferSize, char *host, uint8_t hostlength) {

    // Skip DNS by uncommenting this line:
    //MyXBeeStatus.hostIPResolved = true;

    // Do not wait for ipResponseReceived, which is handled by the callback.
    // Just wait for the XBee confirmation.

    if (!MyXBeeStatus.xbcErrorOccurred && !MyXBeeStatus.ipResponseReceived) {
        xbc.loop();
    
        if (!MyXBeeStatus.hostIPResolved) {
            // This is the DNS lookup section
            // A request is made to lookup an IP address and response handled in callback function zbLAResponseCb
            if (!MyXBeeStatus.dnsLookupRequested) {
              // Send a lookup request command
              Serial.println(F("Sending DNS Lookup")); 
              sendDNSLookupCommand((char*) host, hostlength);
            }
        } else if (!MyXBeeStatus.ipRequestSent) {
            // Send the request
            // The response is handled in callback function zbTcpSendResponseCb
            Serial.print(F("Sending TCP request to "));
            Serial.print(IP, HEX);
            Serial.print(F(", port "));
            Serial.println(Port, HEX);
            tcpSend(IP, Port, protocol, EEPROM, bufferSize);
        } else if (!MyXBeeStatus.ipResponseReceived) {
          // process incoming IP messages until complete, or host closes the connection.
          // There is nothing to do here as the processing is handled in callback function zbIPResponseCb
        }
    
        // Reset status if successful
        if (!MyXBeeStatus.xbcErrorOccurred && MyXBeeStatus.ipResponseReceived) {
            MyXBeeStatus.ipRequestSent = false;
            MyXBeeStatus.ipRequestSentOk = false;
            MyXBeeStatus.ipResponseReceived = false;
        }
    }
}

bool setclock_ntp() {

    const char host[] = "pool.ntp.org";
    uint16_t Port = 123;                 // 0x50 = 80; 0x1BB = 443, 0x1633 = 5683 (COAP), 0x75B = 1883 (MQTT), 1337 = NTP
    uint8_t protocol = 0;                         // 0 for UDP, 1 for TCP, 4 for SSL over TCP
    
    uint8_t AIstatus = 255;
    uint32_t timeInMillis = 0;
    uint8_t i = 0;

    // wait up to 3 min to connect

    while((i++ < 60) && (AIstatus != 0)) {
        timeInMillis = millis();
        while((millis() - timeInMillis) < 3000) {
            xbc.loop();
        }
        getAIStatus(Serial, &AIstatus);
        wdt_reset();
        #if DEBUG > 0
            Serial.print(F("AI status = "));
            Serial.println(AIstatus, HEX);
        #endif
        digitalWrite(WriteLED, HIGH);
        delay(50);
        digitalWrite(WriteLED, LOW);
    }

    if(MyXBeeStatus.isConnected) {

        // wait up to 10 seconds for reply. Make 3 attempts.
        i = 0;
        while(!MyXBeeStatus.hostIPResolved && (i++ < 3)) {
            sendDNSLookupCommand((char*) host, sizeof(host) - 1);
            timeInMillis = millis();
            while((!MyXBeeStatus.hostIPResolved) && ((millis() - timeInMillis) < 10000)) {
                xbc.loop();
            }
        }

        // Reset flats. (Set to true during IP resolution)  
        MyXBeeStatus.ipResponseReceived = false;

        byte packetBuffer[48];
        memset(packetBuffer, 0, 48);

        packetBuffer[0] = 0b11100011;   // LI, Version, Mode
        packetBuffer[1] = 0;     // Stratum, or type of clock
        packetBuffer[2] = 6;     // Polling Interval
        packetBuffer[3] = 0xEC;  // Peer Clock Precision
        // 8 bytes of zero for Root Delay & Root Dispersion
        packetBuffer[12]  = 49;
        packetBuffer[13]  = 0x4E;
        packetBuffer[14]  = 49;
        packetBuffer[15]  = 52;

        tcpSend(IP, Port, protocol, packetBuffer, 48);
        timeInMillis = millis();
        while((!MyXBeeStatus.ipResponseReceived) && (millis() - timeInMillis) < 15000) {
            wdt_reset();
            xbc.loop();
        }
    }
    if(MyXBeeStatus.ipResponseReceived) {
        MyXBeeStatus.reset();
        return(1);
    } else {
        #if DEBUG > 0
            Serial.println(F("NTP timeout"));
        #endif
        MyXBeeStatus.reset();
        return(0);
    }
}
