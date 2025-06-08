/* some notes:
 * 
 */

#include "Rio.h"

#ifndef Rio_MQTT_h
#define Rio_MQTT_h

extern uint32_t IP;
extern uint16_t Port;
extern uint8_t protocol;

void MQTT_connect(char *clientid, uint8_t clientidsize, char *username, uint8_t usernamesize);
//uint8_t CreateMqttHeader(uint8_t *buffer, uint16_t messageid);                                // only used internally
void MQTT_send(uint16_t);

#endif