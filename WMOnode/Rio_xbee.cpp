
/************************************************************
 * Xbee functions developed by Imperial College London and Riverlabs
 *
 ***********************************************************/


#include "Rio_xbee.h"

// Wait for cellular registration, signalled by modem status message containing status 2
// Set isRegistered to true when registered on the network

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
        Serial.println(F("Registered with the cellular network"));
        seqStatus.isRegistered = true;
      }
    } else {
      // print details of other received messages
      printUnexpectedMessage();
    }
  } else if (xbc.getResponse().isError()) {
    Serial.print(F("XBee error. error code is "));
    Serial.println(xbc.getResponse().getErrorCode(), DEC);
    seqStatus.xbcErrorOccurred = true;
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
//            seqStatus.isConnected = true;
//          } else {
//            // Force another AI command
//            seqStatus.aiRequested = false;
//          }
//        } else {
//          // This should never happen
//          Serial.println(F("Failed to get AI value from AI command"));
//          seqStatus.xbcErrorOccurred = true;
//        }
//      } 
//      else {
//        Serial.print(F("Command returned error code: "));
//        Serial.println(atResponse.getStatus(), HEX);
//       seqStatus.xbcErrorOccurred = true;
//      }
//    } else {
//      printUnexpectedMessage();
//    }   
//  } else if (xbc.getResponse().isError()) {
//      Serial.print(F("Error reading packet.  Error code: "));  
//      Serial.println(xbc.getResponse().getErrorCode());
//      seqStatus.xbcErrorOccurred = true;
//  } else {
//    Serial.println(F("No response."));
//  }
//}


void sendDNSLookupCommand(char address[], int len) {
  // LA (lookup address) command
  uint8_t laCmd[] = {'L','A'};
  AtCommandRequest atRequest(laCmd, (uint8_t*)address, len);

  xbc.send(atRequest);
  seqStatus.dnsLookupRequested = true;
}

void getLAResponse() {

  xbc.readPacket(2000);

  if (xbc.getResponse().isAvailable()) {
    // got a response!
 
    // should be an AT command response
    if (xbc.getResponse().getApiId() == AT_COMMAND_RESPONSE) {
      printUnexpectedMessage();
      xbc.getResponse().getAtCommandResponse(atResponse);

      if (atResponse.isOk()) {

        if (atResponse.getValueLength() == 4) {
          // The IP address should be 4 bytes
          Serial.print(F("IP Address: "));
          
          for (int i = 0; i < atResponse.getValueLength(); i++) {
            Serial.print(atResponse.getValue()[i], HEX);
            Serial.print(" ");
          }

          Serial.println("");

          IPaddr = (((uint32_t)atResponse.getValue()[0]) << 24) + 
                   (((uint32_t)atResponse.getValue()[1]) << 16) +   //WB changed. Bug in Isabella's original version?
                   (atResponse.getValue()[2] << 8) + 
                   atResponse.getValue()[3];

          seqStatus.hostIPResolved = true;
        }
      } 
      else {
        Serial.print(F("LA command returned error code: "));
        Serial.println(atResponse.getStatus(), HEX);
        seqStatus.xbcErrorOccurred = true;
      }
    } else {
      printUnexpectedMessage();
    }   
  } else {
    // AT command failed
    if (xbc.getResponse().isError()) {
      Serial.print(F("Error reading packet.  Error code: "));  
      Serial.println(xbc.getResponse().getErrorCode());
      seqStatus.xbcErrorOccurred = true;
    } else {
      Serial.println(F("No response from XBee"));
    }
  }
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
  seqStatus.ipRequestSent = true;
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
  seqStatus.ipRequestSent = true;
}

// Read and print an incoming packet

void readIPResponse() {
  xbc.readPacket(5000);

  if (xbc.getResponse().isAvailable()) {
    // got something.. print packet to USB
    if (xbc.getResponse().getApiId() == RX_IP4_RESPONSE) {
      // a message was received from the internet, so copy to ipResponse object for convenience
      ipResponse.init(xbc.getResponse());
      printIPRX(ipResponse, Serial);
      seqStatus.ipResponseReceived = true;
    } else {
      printUnexpectedMessage();
      seqStatus.gotStatusResponse = true;
    }
  } else if (xbc.getResponse().isError()) {
    Serial.print(F("XBee error. error code is "));
    Serial.println(xbc.getResponse().getErrorCode(), DEC);
    seqStatus.xbcErrorOccurred = true;
  }
}

// print the details of an unexpected message from the modem

void printUnexpectedMessage() {
  if (xbc.getResponse().getApiId() == MODEM_STATUS_RESPONSE) {
    // a modem status message was received
    xbc.getResponse().getModemStatusResponse(modemStatusResponse);
    Serial.print(F("Modem Status Response = "));
    Serial.println(modemStatusResponse.getStatus(), HEX);
    seqStatus.gotStatusResponse = true;
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

// Print out the contents of the ipResponse object

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
    for (int i = 0; i < ipResponse.getDataLength(); i++) {
      stream.print((char) ipResponse.getData()[i]);
    }
    stream.println("");
  }
}

bool getAIStatus(Stream &stream, uint8_t *returnvalue) {
  // Get the AI status from the modem.
  // A callback could be used, so as not to block, but in this case a quick response is expected
  // as no internet messages need be sent/received, so sendAndWait() is used instead.

  // association status command
  uint8_t assocCmd[] = {'A','I'};
  AtCommandRequest atRequest(assocCmd);
  // XBee AT Command response object
  // This object is used to receive an AT command response frame
  AtCommandResponse atResponse;
  
  uint8_t status = xbc.sendAndWait(atRequest, 150);
  
  if (status == 0) {
    xbc.getResponse().getAtCommandResponse(atResponse);
    if (atResponse.isOk()) {
      if (atResponse.getValueLength() == 1) {
        if (atResponse.getValue()[0] == 0) {
          *returnvalue = atResponse.getValue()[0];
          //stream.println(F("Internet connection established"));
          seqStatus.isConnected = true;
          return(1);
        } else {
          // stream.print(F("AI status = "));
          *returnvalue = atResponse.getValue()[0];
          // stream.println(*returnvalue);
          return(1);
        }
      } else {
        // This should never happen
        stream.println(F("Failed to get AI value from AI command"));
        seqStatus.xbcErrorOccurred = true;
        return(0);
      }
    } else {
        stream.print(F("Command returned error code: "));
        *returnvalue = atResponse.getStatus();
        stream.println(*returnvalue, HEX);
        seqStatus.xbcErrorOccurred = true;
    }
  } else {
    stream.print(F("sendAndWait() returned error code when attempting to get AI indicator: "));
    stream.println(status);
    return(0);
    // TOD); move this out of this function
    seqStatus.xbcErrorOccurred = true;        
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
        stream.print(F("DB Command returned error code: "));
        *returnvalue = atResponse.getStatus();
        stream.println(*returnvalue, HEX);
        return(0);
    }
  } else {
    stream.print(F("sendAndWait() returned error code when attempting to get DB indicator: "));
    stream.println(status);
    return(0);        
  }
}

// Callback function for incoming TCP/IP message

void zbIPResponseCb(IPRxResponse& ipResponse, uintptr_t) {
  // Note that any incoming IP message is considered a success, even if it does not 
  // contain the complete transmission from the web server.
  // This is not reaslistic as a TCP message can be fragmented across multiple packets.
  Serial.println(F("Callback - IP4 message received"));
  printIPRX(ipResponse, Serial);
  seqStatus.ipResponseReceived = true;
}


// Callback function for incoming TCP/IP message
// This one specifically for COAP

void zbIPResponseCb_COAP(IPRxResponse& ipResponse, uintptr_t) {
  // Note that any incoming IP message is considered a success, even if it does not 
  // contain the complete transmission from the web server.
  // This is not realistic as a TCP message can be fragmented across multiple packets.
  Serial.println(F("Callback - IP4 message received"));
  printIPRX(ipResponse, Serial);
  CoapPacket cp;
  cp.parseMessage(ipResponse.getData(), ipResponse.getDataLength());
  cp.print(Serial);

 // TODO: process coap response fully
  if(cp.type == 2) {
    Serial.println(F("COAP acknowledgement received (2)"));
    seqStatus.CoapSentAcknowledged = true;
    seqStatus.ipResponseReceived = 1;
  }
  if(cp.type == 0) {
    Serial.println(F("COAP acknowledgeable message received (type = 0)"));
    seqStatus.CoapSent203Received = true;
    // send acknowledgement:
    uint8_t buffer[5];    // can be small becuase it is only an acknowledgement.
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

    seqStatus.ipRequestSentOk = false;     // reset this

    tcpSend(IP, Port, protocol, (uint8_t*)buffer, packetSize);

    uint32_t starttime = millis();
    while(!seqStatus.ipRequestSentOk && (millis() - starttime < 5000)) {  // wait up to 5 seconds 
      xbc.loop();
    }

    if(seqStatus.ipRequestSentOk) {
      Serial.println(F("COAP 2.03 acknowledgement successfully sent. Transaction finished."));
    } else {
      Serial.println(F("Xbee did not (yet) confirm. Acknowledgement assumed to be sent."));
    }
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
  Serial.print(F("Callback - Modem Status Response= "));
  Serial.println(modemStatus, HEX);
  if (modemStatus == 2) {
    Serial.println(F("Registered with the cellular network"));
    seqStatus.isRegistered = true;
  } else if (modemStatus == 3) {
    Serial.println(F("Unregistered with the cellular network"));
    seqStatus.isRegistered = false;
  } else if (modemStatus == 0) {
    Serial.println(F("Hardware reset or power up"));
    seqStatus.isRegistered = false;
  } else if (modemStatus == 0x0E) {
    Serial.println(F("Remote Manager connected"));
  } else if (modemStatus == 0x0F) {
    Serial.println(F("Remote Manager disconnected"));
  }
}


void zbLAResponseCb(AtCommandResponse& atr, uintptr_t) {
  // We expect the callback to be from the DNS Lookup Command LA
  if (atr.getCommand()[0] != 'L' || atr.getCommand()[1] != 'A') {
    Serial.print(F("Callback - expected a response to LA command, but received response to "));
    Serial.print(atr.getCommand()[0]);
    Serial.println(atr.getCommand()[1]);
    // For now, just ignore. This is probably an AI or DB response coming in. The LA should still come.
    //seqStatus.xbcErrorOccurred = true;
    return;
  }
  
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
           atr.getValue()[3], 
      //Serial.println(IP, HEX);
      seqStatus.hostIPResolved = true;
    }
  } else {
    Serial.print(F("LA Command return error code: "));
    Serial.println(atr.getStatus(), HEX);
    seqStatus.xbcErrorOccurred = true;
  }
}

void zbTcpSendResponseCb(TxStatusResponse& txr, uintptr_t) {
  if (txr.isSuccess()) {
    Serial.println(F("Callback - Transmission was successful!"));
    seqStatus.ipRequestSentOk = true;
  } else {
    Serial.print(F("Transmission returned error code: "));
    Serial.println(txr.getStatus(), HEX);
    seqStatus.xbcErrorOccurred = true;
  }
}

void sendXbeeMessage(uint8_t* buffer, uint16_t bufferSize, char *host, uint8_t hostlength) {

    // Skip DNS by uncommenting this line:
    //seqStatus.hostIPResolved = true;

    // Do not wait for ipResponseReceived, which is handled by the callback.
    // Just wait for the XBee confirmation.

    if (!seqStatus.xbcErrorOccurred && !seqStatus.ipResponseReceived) {
        xbc.loop();
    
        if (!seqStatus.hostIPResolved) {
            // This is the DNS lookup section
            // A request is made to lookup an IP address and response handled in callback function zbLAResponseCb
            if (!seqStatus.dnsLookupRequested) {
              // Send a lookup request command
              Serial.println(F("Sending DNS Lookup")); 
              sendDNSLookupCommand(host, hostlength);
            }
        } else if (!seqStatus.ipRequestSent) {
            // Send the request
            // The response is handled in callback function zbTcpSendResponseCb
            Serial.print(F("Sending TCP request to "));
            Serial.print(IP, HEX);
            Serial.print(F(", port "));
            Serial.println(Port, HEX);
            //tcpSend(IP, Port, protocol, buffer, bufferSize);
            tcpSend(IP, Port, protocol, EEPROM, bufferSize);
        } else if (!seqStatus.ipResponseReceived) {
          // process incoming IP messages until complete, or host closes the connection.
          // There is nothing to do here as the processing is handled in callback function zbIPResponseCb
        }
    
        // Reset status if successful
        if (!seqStatus.xbcErrorOccurred && seqStatus.ipResponseReceived) {
            seqStatus.ipRequestSent = false;
            seqStatus.ipRequestSentOk = false;
            seqStatus.ipResponseReceived = false;
        }
    }
}

/* Quick hack to enable EEPROM version: */

void sendXbeeMessage(EEPROMClass, uint16_t bufferSize, char *host, uint8_t hostlength) {
  uint8_t *buffer = 0;
  sendXbeeMessage(buffer, bufferSize, host, hostlength);
}
