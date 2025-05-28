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

#include "serial_config.h"
#include "logger.h"
#include <EEPROM.h>
#include "buzzer_config.h"
#include "neopixel_config.h"
#define EEPROM_SIZE 128
#define SSID_ADDR 0
#define PASS_ADDR 64
#define MAX_LEN 32

#if defined(ESP32)
Preferences preferences;
#endif

void loadWiFiConfig(String &ssid, String &password)
{
#if defined(ESP32)
    preferences.begin("wifi", true);
    ssid = preferences.getString("ssid", "");
    password = preferences.getString("password", "");
    Logger::log("Loaded WiFi SSID: " + ssid);
    Logger::log("Loaded WiFi Password: " + password);
    preferences.end();
#elif defined(ESP8266)
    EEPROM.begin(EEPROM_SIZE);
    char ssidBuf[MAX_LEN + 1] = {0};
    char passBuf[MAX_LEN + 1] = {0};
    for (size_t i = 0; i < MAX_LEN; ++i)
    {
        ssidBuf[i] = EEPROM.read(SSID_ADDR + i);
        passBuf[i] = EEPROM.read(PASS_ADDR + i);
    }
    ssid = String(ssidBuf);
    password = String(passBuf);
    Logger::log("Loaded WiFi SSID: " + ssid);
    Logger::log("Loaded WiFi Password: " + password);
    EEPROM.end();
#endif
}

void showHardwareAndNetworkStatus()
{
    // Show network status
    Logger::log("Network Status:");
    if (WiFi.status() == WL_CONNECTED)
    {
        Logger::log("# -----------------------------------------");
        Logger::log("# Connected to WiFi");
        Logger::log("# SSID: " + WiFi.SSID());
        Logger::log("# IP Address: " + WiFi.localIP().toString());
    }
    else
    {
        Logger::log("Not connected to WiFi");
    }

    // Show hardware resource information (simply show pin configuration here)
    Logger::log("# Hardware Resources:");
    // Since LED_PIN is undefined, we'll use a default value of 0 as a placeholder.
    // You should replace 0 with the actual pin number if known.
    Logger::log("# RGB LED Pin: " + String(LED_PIN));
    Logger::log("# Buzzer Pin: " + String(BUZZER_PIN));
    Logger::log("# -----------------------------------------");
}
void showHelp()
{
    Logger::log("\n# Help:");
    Logger::log("# 1. Configure WiFi: Enter the SSID and password to connect to your WiFi network.");
    Logger::log("# 2. View hardware resources and network status: Displays current hardware pin configurations and network status.");
    Logger::log("# 3. Exit: Exit the configuration mode.");
    Logger::log("# 'c' or 'C' enter config mode.");
    Logger::log("# Please select an option:");
}
void configureWiFiThroughSerial()
{
    while (true)
    {
        showHelp();
        String input;
        char choice = 0;
        while (true)
        {
            while (Serial.available() == 0)
            {
                delay(10);
            }
            input = Serial.readStringUntil('\n');
            input.trim();
            if (input.length() > 0 &&
                (input[0] == '1' || input[0] == '2' || input[0] == '3' ||
                 input[0] == 'c' || input[0] == 'C'))
            {
                choice = input[0];
                break;
            }
        }

        switch (choice)
        {
        case 'c':
        case 'C':
            showHelp();
            return;
        case '1':
        {
            Logger::log("Please enter new WiFi SSID:");
            while (Serial.available() == 0)
            {
                delay(10);
            }
            String newSsid = Serial.readStringUntil('\n');
            newSsid.trim();

            Logger::log("Please enter new WiFi password:");
            while (Serial.available() == 0)
            {
                delay(10);
            }
            String newPassword = Serial.readStringUntil('\n');
            newPassword.trim();

#if defined(ESP32)
            preferences.begin("wifi", false);
            preferences.putString("ssid", newSsid);
            preferences.putString("password", newPassword);
            preferences.end();
            Logger::log("WiFi configuration saved to Preferences.");
#elif defined(ESP8266)
            EEPROM.begin(EEPROM_SIZE);
            for (size_t i = 0; i < MAX_LEN; ++i)
            {
                EEPROM.write(SSID_ADDR + i, i < newSsid.length() ? newSsid[i] : 0);
                EEPROM.write(PASS_ADDR + i, i < newPassword.length() ? newPassword[i] : 0);
            }
            EEPROM.commit();
            EEPROM.end();
            Logger::log("WiFi configuration saved to EEPROM.");
#endif
            Logger::log("WiFi configuration saved to Flash.");
            break;
        }
        case '2':
            showHardwareAndNetworkStatus();
            break;
        case '3':
            return;
        default:
            Logger::log("Invalid choice, please try again.");
        }
    }
}