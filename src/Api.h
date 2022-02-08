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
    static DateTime getTime();
    static AsyncWebServer server;

private:
    static RTC_DS3231 *rtc;
    static SemaphoreHandle_t configMutex;
    static SemaphoreHandle_t rtcMutex;
    static SemaphoreHandle_t wirelessMutex;
    static Preferences preferences;
    static void persistConfigurableOptions(ApiConfigurableOptions options);
    static void getTime(AsyncWebServerRequest *request);
    static void putTime(AsyncWebServerRequest *request, JsonVariant &json);
    static void getConfig(AsyncWebServerRequest *request);
    static void putConfig(AsyncWebServerRequest *request, JsonVariant &json);
    static void getWireless(AsyncWebServerRequest *request);
    static void putWireless(AsyncWebServerRequest *request, JsonVariant &json);
};