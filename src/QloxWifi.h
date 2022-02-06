#include <Arduino.h>
#include <Preferences.h>
#include "WiFi.h"

class QloxWifi
{
public:
    static void setup();

private:
    static Preferences preferences;
    static String ssid;
    static String pass;
    static bool tryGetSettings();
    static void wiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info);
    static void wiFiAPClientConnected(WiFiEvent_t event, WiFiEventInfo_t info);
    static void WiFiAPClientDisconnected(WiFiEvent_t event, WiFiEventInfo_t info);
    static void wiFiAPClientAssignedIP(WiFiEvent_t event, WiFiEventInfo_t info);
    static void wiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info);
    static void wiFiAPStart(WiFiEvent_t event, WiFiEventInfo_t info);
    static void wiFiAPStop(WiFiEvent_t event, WiFiEventInfo_t info);
    static void wiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info);
};