#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include <Preferences.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "RTClib.h"
#include "AsyncJson.h"
#include <ArduinoJson.h>

typedef struct
{
    int Red;
    int Green;
    int Blue;
    int Brightness;
} ApiConfigurableOptions;

class Api
{
public:
    static void setup(RTC_DS3231 *rtc);
    static ApiConfigurableOptions GetConfigurableOptions();
    static DateTime GetTime();
    static AsyncWebServer server;

private:
    static RTC_DS3231 *rtc;
    static SemaphoreHandle_t configMutex;
    static SemaphoreHandle_t rtcMutex;
    static SemaphoreHandle_t wirelessMutex;
    static Preferences preferences;
    static void PersistConfigurableOptions(ApiConfigurableOptions options);
    static void GetTime(AsyncWebServerRequest *request);
    static void PutTime(AsyncWebServerRequest *request, JsonVariant &json);
    static void GetConfig(AsyncWebServerRequest *request);
    static void PutConfig(AsyncWebServerRequest *request, JsonVariant &json);
    static void GetWireless(AsyncWebServerRequest *request);
    static void PutWireless(AsyncWebServerRequest *request, JsonVariant &json);
};