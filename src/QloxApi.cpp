#include "QloxApi.h"
#include <ArduinoJson.h>

SemaphoreHandle_t QloxApi::mutex = NULL;
Preferences QloxApi::preferences;

// Create AsyncWebServer object on port 80
AsyncWebServer QloxApi::server(80);

void QloxApi::setup(RTC_DS3231 *rtc)
{
    // Create mutex before starting tasks
    mutex = xSemaphoreCreateMutex();
    // Initialize webserver URLs
    server.on("/api/config", HTTP_GET,
              [](AsyncWebServerRequest *request)
              {
                  const ApiConfigurableOptions options = getConfigurableOptions();
                  AsyncResponseStream *response = request->beginResponseStream("application/json");
                  DynamicJsonDocument json(100);
                  json["brightness"] = options.Brightness;
                  json["red"] = options.Red;
                  json["green"] = options.Green;
                  json["blue"] = options.Blue;

                  serializeJson(json, *response);
                  request->send(response);
              });
    server.on("/api/time", HTTP_GET,
              [rtc](AsyncWebServerRequest *request)
              {
                  const DateTime now = rtc->now();
                  AsyncResponseStream *response = request->beginResponseStream("application/json");
                  DynamicJsonDocument json(100);
                  json["time"] = now.timestamp();

                  serializeJson(json, *response);
                  request->send(response);
              });
    server.begin();
}

void QloxApi::persistConfigurableOptions(ApiConfigurableOptions options)
{
    xSemaphoreTake(mutex, portMAX_DELAY);
    preferences.begin("api");
    preferences.putUInt("red", options.Red);
    preferences.putUInt("green", options.Green);
    preferences.putUInt("blue", options.Blue);
    preferences.putUInt("brightness", options.Brightness);
    preferences.end();
    xSemaphoreGive(mutex);
}

ApiConfigurableOptions QloxApi::getConfigurableOptions()
{
    ApiConfigurableOptions options;
    xSemaphoreTake(mutex, portMAX_DELAY);
    bool found = preferences.begin("api", true);
    options.Red = preferences.getUInt("red", 255);
    options.Green = preferences.getUInt("green", 255);
    options.Blue = preferences.getUInt("blue", 255);
    options.Brightness = preferences.getUInt("brightness", 20);
    preferences.end();
    xSemaphoreGive(mutex);
    if (!found)
    {
        QloxApi::persistConfigurableOptions(options);
    }

    return options;
}