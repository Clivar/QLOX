#include <Arduino.h>
#include <Preferences.h>
#include "WiFi.h"
#include <algorithm>
#include "BroadcastDiscover.h"
#include "Api.h"
#include "Wireless.h"

#define DEFAULT_SSID "QLOX"
#define DEFAULT_PASSWORD "QLOXqlox"
#define HOSTNAME "QLOX"

Preferences Wireless::preferences;
String Wireless::ssid;
String Wireless::pass;

void Wireless::setup()
{
    WiFi.onEvent(wiFiStationConnected, SYSTEM_EVENT_STA_CONNECTED);
    WiFi.onEvent(wiFiGotIP, SYSTEM_EVENT_STA_GOT_IP);
    WiFi.onEvent(wiFiStationDisconnected, SYSTEM_EVENT_STA_DISCONNECTED);

    WiFi.onEvent(wiFiAPClientConnected, SYSTEM_EVENT_AP_STACONNECTED);
    WiFi.onEvent(WiFiAPClientDisconnected, SYSTEM_EVENT_AP_STACONNECTED);
    WiFi.onEvent(wiFiAPClientAssignedIP, SYSTEM_EVENT_AP_STAIPASSIGNED);
    WiFi.onEvent(wiFiAPStart, SYSTEM_EVENT_AP_START);
    WiFi.onEvent(wiFiAPStop, SYSTEM_EVENT_AP_STOP);

    String hostName = HOSTNAME;
    if (Wireless::tryGetSettings())
    {
        Serial.println("Initialised as STA");
        WiFi.setHostname(hostName.c_str());
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid.c_str(), pass.c_str());
    }
    else
    {
        Serial.println("Initialised as AP");
        WiFi.config(IPAddress(192, 168, 1, 1), INADDR_NONE, IPAddress(255, 255, 255, 0), INADDR_NONE);
        WiFi.setHostname(hostName.c_str());
        WiFi.mode(WIFI_AP);
        ssid = DEFAULT_SSID;
        pass = DEFAULT_PASSWORD;

        WiFi.softAP(ssid.c_str(), pass.c_str());
    }
}

bool Wireless::tryGetSettings()
{
    preferences.begin("wireless", true);
    ssid = preferences.getString("ssid", "");
    pass = preferences.getString("password", "");
    preferences.end();

    return strcmp(ssid.c_str(), "") && strcmp(pass.c_str(), "");
}

void Wireless::wiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
    Serial.println("Connected to AP successfully!");
}

void Wireless::wiFiAPClientConnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
    Serial.println("Client connected");
}

void Wireless::WiFiAPClientDisconnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
    Serial.println("Client disconnected");
}

void Wireless::wiFiAPClientAssignedIP(WiFiEvent_t event, WiFiEventInfo_t info)
{
    Serial.println("Client has been assigned an IP");
}

void Wireless::wiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info)
{
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    listenForBroadcast(WiFi.localIP().toString());
}

void Wireless::wiFiAPStart(WiFiEvent_t event, WiFiEventInfo_t info)
{
    Serial.println("WiFi Soft AP started");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    listenForBroadcast(WiFi.localIP().toString());
}

void Wireless::wiFiAPStop(WiFiEvent_t event, WiFiEventInfo_t info)
{
    Serial.println("WiFi Soft AP stopped");
    Serial.println("Trying to Restart");
    WiFi.softAP(ssid.c_str(), pass.c_str());
}

void Wireless::wiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
    Serial.println("Disconnected from WiFi access point");
    Serial.print("WiFi lost connection.");
    Serial.println("Trying to Reconnect");
    WiFi.begin(ssid.c_str(), pass.c_str());
}