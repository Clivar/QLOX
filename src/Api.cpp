#include "Api.h"

SemaphoreHandle_t Api::configMutex = nullptr;
SemaphoreHandle_t Api::rtcMutex = nullptr;
SemaphoreHandle_t Api::wirelessMutex = nullptr;

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
    wirelessMutex = xSemaphoreCreateMutex();

    // Initialize webserver URLs
    server.on("/api/config", HTTP_GET, getConfig);
    server.on("/api/time", HTTP_GET, [](AsyncWebServerRequest *request)
              { return getTime(request); });
    server.on("/api/wireless", HTTP_GET, [](AsyncWebServerRequest *request)
              { return getWireless(request); });
    AsyncCallbackJsonWebHandler *configHandler = new AsyncCallbackJsonWebHandler("/api/config", putConfig);
    server.addHandler(configHandler);
    AsyncCallbackJsonWebHandler *timeHandler = new AsyncCallbackJsonWebHandler("/api/time", putTime);
    server.addHandler(timeHandler);
    AsyncCallbackJsonWebHandler *wirelessHandler = new AsyncCallbackJsonWebHandler("/api/wireless", putWireless);
    server.addHandler(wirelessHandler);
    server.begin();
}

DateTime Api::getTime()
{
    xSemaphoreTake(rtcMutex, portMAX_DELAY);
    const DateTime now = rtc->now();
    xSemaphoreGive(rtcMutex);
    return now;
}

void Api::getWireless(AsyncWebServerRequest *request)
{
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    DynamicJsonDocument json(100);
    json["ssid"] = WiFi.SSID().c_str();
    json["password"] = "**REDACTED**";

    serializeJson(json, *response);
    request->send(response);
}

void Api::putWireless(AsyncWebServerRequest *request, JsonVariant &jsonBody)
{
    String ssid = jsonBody["ssid"].as<String>();
    String password = jsonBody["password"].as<String>();

    xSemaphoreTake(wirelessMutex, portMAX_DELAY);
    preferences.begin("wireless");
    preferences.putString("ssid", ssid);
    preferences.putString("password", password);
    preferences.end();

    getWireless(request);
    xTaskCreate(
        [](void *parameters)
        {
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            xSemaphoreTake(configMutex, portMAX_DELAY);
            xSemaphoreTake(rtcMutex, portMAX_DELAY);
            ESP.restart();
        },
        "RestartESP", 1024, NULL, 1, NULL);
}

void Api::getTime(AsyncWebServerRequest *request)
{
    const DateTime now = getTime();
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    DynamicJsonDocument json(100);
    json["time"] = now.timestamp();

    serializeJson(json, *response);
    request->send(response);
}

void Api::putTime(AsyncWebServerRequest *request, JsonVariant &jsonBody)
{
    const DateTime time = DateTime(jsonBody["time"].as<const char *>());
    xSemaphoreTake(rtcMutex, portMAX_DELAY);
    rtc->adjust(time);
    xSemaphoreGive(rtcMutex);

    getTime(request);
}

void Api::getConfig(AsyncWebServerRequest *request)
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

void Api::putConfig(AsyncWebServerRequest *request, JsonVariant &jsonBody)
{
    JsonObject jsonObj = jsonBody.as<JsonObject>();
    ApiConfigurableOptions options;
    options.Red = jsonObj["red"];
    options.Blue = jsonObj["blue"];
    options.Green = jsonObj["green"];
    options.Brightness = jsonObj["brightness"];
    persistConfigurableOptions(options);
    getConfig(request);
}

void Api::persistConfigurableOptions(ApiConfigurableOptions options)
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
        persistConfigurableOptions(options);
    }

    return options;
}