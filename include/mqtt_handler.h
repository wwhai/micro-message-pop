#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include <Arduino.h>
#include <PubSubClient.h>

extern PubSubClient client;

void setupMQTT(const String& deviceId, const String& userId, const String& groupId, void (*callback)(char*, byte*, unsigned int));
void handleMQTT(const String& deviceId, const String& userId, const String& groupId);
void reconnectMQTT(const String& deviceId, const String& userId, const String& groupId);

#endif