// Copyright (C) 2025 wwhai
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
#include <Arduino.h>
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#include <ArduinoJson.h>
#include "mqtt_config.h"
#include "logger.h"
#include "neopixel_handler.h"
#include "buzzer_handler.h"
#include "serial_config.h"
#include "buzzer_config.h"
#include "mqtt_handler.h"

const String USER_ID = "wwhai";
const String GROUP_ID = "g1";
const String DEVICE_ID = "dev1";

void callback(char *topic, byte *payload, unsigned int length);

void setup()
{
    Logger::log("Starting Device...");
    Logger::init();
    Logger::log("Device ID: " + DEVICE_ID);
    Logger::log("User ID: " + USER_ID);
    Logger::log("Group ID: " + GROUP_ID);
    Logger::log("Initializing RGB LED...");
    initNeopixel();
    Logger::log("Initializing Buzzer...");
    pinMode(BUZZER_PIN, OUTPUT);
    Logger::log("Buzzer Pin: " + String(BUZZER_PIN));
    String ssid, password;
    int wifiConfigFailCount = 0;

    while (wifiConfigFailCount < 5)
    {
        Logger::log("Loading WiFi configuration...");
        loadWiFiConfig(ssid, password);
        if (!ssid.isEmpty() && !password.isEmpty())
        {
            Logger::log("WiFi configuration loaded successfully.");
            break;
        }
        else
        {
            Logger::log("Failed to load WiFi configuration. Attempt " + String(wifiConfigFailCount + 1) + "/5");
        }
        wifiConfigFailCount++;
        delay(1000);
    }

    while (ssid.isEmpty() || password.isEmpty())
    {
        Logger::log("WiFi configuration not found. Please configure WiFi.");
        configureWiFiThroughSerial();
        loadWiFiConfig(ssid, password);
    }

    // WiFi连接流程
    int connectTry = 0;
    const int maxConnectTry = 10;
    WiFi.begin(ssid.c_str(), password.c_str());
    while (WiFi.status() != WL_CONNECTED && connectTry < maxConnectTry)
    {
        delay(1000);
        Logger::log("Connecting to WiFi...");
        connectTry++;
    }

    while (WiFi.status() != WL_CONNECTED)
    {
        Logger::log("WiFi connect failed. Please configure WiFi.");
        configureWiFiThroughSerial();
        loadWiFiConfig(ssid, password);
        WiFi.begin(ssid.c_str(), password.c_str());
        connectTry = 0;
        while (WiFi.status() != WL_CONNECTED && connectTry < maxConnectTry)
        {
            delay(1000);
            Logger::log("Connecting to WiFi...");
            connectTry++;
        }
    }

    // 只有WiFi连接成功才继续
    if (WiFi.status() == WL_CONNECTED)
    {
        Logger::log("Connected to WiFi");

        // 优化后的MQTT初始化与连接
        setupMQTT(DEVICE_ID, USER_ID, GROUP_ID, callback);
        reconnectMQTT(DEVICE_ID, USER_ID, GROUP_ID);
    }
    else
    {
        Logger::log("WiFi not connected, MQTT will not start.");
    }
}

void loop()
{
    handleMQTT(DEVICE_ID, USER_ID, GROUP_ID);

    // 检查串口输入，随时进入配网模式
    if (Serial.available())
    {
        String input = Serial.readStringUntil('\n');
        input.trim();
        if (input.length() > 0 && (input[0] == 'c' || input[0] == 'C'))
        {
            Logger::log("# Entering configuration mode...");
            configureWiFiThroughSerial();
        }
    }
}

void callback(char *topic, byte *payload, unsigned int length)
{
    String message = "";
    for (unsigned int i = 0; i < length; i++)
    {
        message += (char)payload[i];
    }
    Logger::log("Received message: " + message);

    // JsonDocument doc;
    // DeserializationError error = deserializeJson(doc, message);

    // if (error)
    // {
    //     Logger::log("JSON parsing failed");
    //     return;
    // }

    // const char *command = doc["command"];
    // JsonObject parameters = doc["parameters"];

    // if (strcmp(command, "LED") == 0)
    // {
    //     int ledIndex = parameters["led_index"];
    //     JsonArray color = parameters["color"];
    //     int duration = parameters["duration"];

    //     setPixelColor(ledIndex, color[0], color[1], color[2]);
    //     delay(duration);
    //     setPixelColor(ledIndex, 0, 0, 0);
    // }
    // else if (strcmp(command, "BEEP") == 0)
    // {
    //     int beepCount = parameters["beep_count"];
    //     int duration = parameters["duration"];
    //     beep(beepCount, duration);
    // }
}
