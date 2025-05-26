#include "mqtt_handler.h"
#include "mqtt_config.h"
#include "logger.h"

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif

WiFiClient espClient;
PubSubClient client(espClient);

static String _deviceId, _userId, _groupId;

void setupMQTT(const String& deviceId, const String& userId, const String& groupId, void (*callback)(char*, byte*, unsigned int))
{
    _deviceId = deviceId;
    _userId = userId;
    _groupId = groupId;
    client.setServer(MQTT_SERVER, MQTT_PORT);
    client.setCallback(callback);
}

void reconnectMQTT(const String& deviceId, const String& userId, const String& groupId)
{
    while (!client.connected())
    {
        if (Serial.available())
        {
            char c = Serial.read();
            if (c == 'c' || c == 'C')
            {
                Logger::log("Entering WiFi configuration mode...");
                extern void configureWiFiThroughSerial();
                extern void loadWiFiConfig(String&, String&);
                String ssid, password;
                configureWiFiThroughSerial();
                loadWiFiConfig(ssid, password);
                WiFi.disconnect();
                WiFi.begin(ssid.c_str(), password.c_str());
                continue;
            }
        }

        if (client.connect(deviceId.c_str()))
        {
            Logger::log("Connected to MQTT");
            String commandsTopic = "users/" + userId + "/groups/" + groupId + "/" + deviceId + "/commands";
            Logger::log("Subscribing to topic: " + commandsTopic);
            client.subscribe(commandsTopic.c_str());
        }
        else
        {
            Logger::log("MQTT connection failed");
            delay(5000);
        }
    }
}

void handleMQTT(const String& deviceId, const String& userId, const String& groupId)
{
    if (!client.connected())
    {
        reconnectMQTT(deviceId, userId, groupId);
    }
    client.loop();
}