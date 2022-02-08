#include "Api.h"
#include "WiFi.h"

SemaphoreHandle_t Api::configMutex = nullptr;
SemaphoreHandle_t Api::rtcMutex = nullptr;
Preferences Api::preferences;
RTC_DS3231 *Api::rtc;

// Create AsyncWebServer object on port 80
AsyncWebServer Api::server(80);

void Api::setup(RTC_DS3231 *rtcPtr)
{
    rtc = rtcPtr;
    // Create mutex before starting tasks
    configMutex = xSemaphoreCreateMutex();
    rtcMutex = xSemaphoreCreateMutex();

    server.on("/api/config", HTTP_GET, GetConfig);
    server.on("/api/time", HTTP_GET, [](AsyncWebServerRequest *request)
              { return GetTime(request); });
    server.on("/api/wireless", HTTP_GET, [](AsyncWebServerRequest *request)
              { return GetWireless(request); });
    // Initialize webserver URLs
    AsyncCallbackJsonWebHandler *configHandler = new AsyncCallbackJsonWebHandler("/api/config", PutConfig);
    server.addHandler(configHandler);
    AsyncCallbackJsonWebHandler *timeHandler = new AsyncCallbackJsonWebHandler("/api/time", PutTime);
    server.addHandler(timeHandler);
    AsyncCallbackJsonWebHandler *timeHandler = new AsyncCallbackJsonWebHandler("/api/wireless", PutWireless);
    server.addHandler(timeHandler);
    server.begin();
}

DateTime Api::GetTime()
{
    xSemaphoreTake(rtcMutex, portMAX_DELAY);
    const DateTime now = rtc->now();
    xSemaphoreGive(rtcMutex);
    return now;
}

void Api::GetWireless(AsyncWebServerRequest *request)
{
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    DynamicJsonDocument json(100);
    json["ssid"] = WiFi.SSID().c_str();
    json["password"] = "**REDACTED**";

    serializeJson(json, *response);
    request->send(response);
}

void Api::PutWireless(AsyncWebServerRequest *request, JsonVariant &jsonBody)
{
    String ssid = jsonBody["ssid"].as<String>();
    String password = jsonBody["password"].as<String>();

    preferences.begin("wireless");
    preferences.putString("ssid", ssid);
    preferences.putString("password", password);
    preferences.end();

    GetWireless(request);
    ESP.restart();
}

void Api::PutTime(AsyncWebServerRequest *request, JsonVariant &jsonBody)
{
    const DateTime time = DateTime(jsonBody["time"].as<const char *>());
    xSemaphoreTake(rtcMutex, portMAX_DELAY);
    rtc->adjust(time);
    xSemaphoreGive(rtcMutex);

    GetTime(request);
}

void Api::GetTime(AsyncWebServerRequest *request)
{
    const DateTime now = GetTime();
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    DynamicJsonDocument json(100);
    json["time"] = now.timestamp();

    serializeJson(json, *response);
    request->send(response);
}

void Api::PutTime(AsyncWebServerRequest *request, JsonVariant &jsonBody)
{
    const DateTime time = DateTime(jsonBody["time"].as<const char *>());
    xSemaphoreTake(rtcMutex, portMAX_DELAY);
    rtc->adjust(time);
    xSemaphoreGive(rtcMutex);

    GetTime(request);
}

void Api::GetConfig(AsyncWebServerRequest *request)
{
    const ApiConfigurableOptions options = GetConfigurableOptions();
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    DynamicJsonDocument json(100);
    json["brightness"] = options.Brightness;
    json["red"] = options.Red;
    json["green"] = options.Green;
    json["blue"] = options.Blue;

    serializeJson(json, *response);
    request->send(response);
}

void Api::PutConfig(AsyncWebServerRequest *request, JsonVariant &jsonBody)
{
    JsonObject jsonObj = jsonBody.as<JsonObject>();
    ApiConfigurableOptions options;
    options.Red = jsonObj["red"];
    options.Blue = jsonObj["blue"];
    options.Green = jsonObj["green"];
    options.Brightness = jsonObj["brightness"];
    PersistConfigurableOptions(options);
    GetConfig(request);
}

void Api::PersistConfigurableOptions(ApiConfigurableOptions options)
{
    xSemaphoreTake(configMutex, portMAX_DELAY);
    preferences.begin("api");
    preferences.putUInt("red", options.Red);
    preferences.putUInt("green", options.Green);
    preferences.putUInt("blue", options.Blue);
    preferences.putUInt("brightness", options.Brightness);
    preferences.end();
    xSemaphoreGive(configMutex);
}

ApiConfigurableOptions Api::GetConfigurableOptions()
{
    ApiConfigurableOptions options;
    xSemaphoreTake(configMutex, portMAX_DELAY);
    bool found = preferences.begin("api", true);
    options.Red = preferences.getUInt("red", 255);
    options.Green = preferences.getUInt("green", 255);
    options.Blue = preferences.getUInt("blue", 255);
    options.Brightness = preferences.getUInt("brightness", 20);
    preferences.end();
    xSemaphoreGive(configMutex);
    if (!found)
    {
        PersistConfigurableOptions(options);
    }

    return options;
}