#ifndef LIBMQTT_H
#define LIBMQTT_H

#include <Arduino.h>
#include <PubSubClient.h>
#include "CMotor.h"

void callback(char* topic, byte* message, unsigned int length);
void initMQTT(PubSubClient &client, const char * mqtt_server);
void reconnectMQTT(PubSubClient &client, const String *list_of_topics);

#endif