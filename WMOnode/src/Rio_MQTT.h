/* some notes:
 * 
 */

#include "Rio.h"

#ifndef Rio_MQTT_h
#define Rio_MQTT_h

void MQTT_connect(uint8_t *buffer, char *clientid, uint8_t clientidsize, char *username, uint8_t usernamesize);
uint8_t CreateMqttHeader(uint8_t *buffer, uint16_t messageid);

#endif