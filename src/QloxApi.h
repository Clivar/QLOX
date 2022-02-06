#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include <Preferences.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "RTClib.h"

typedef struct
{
    int Red;
    int Green;
    int Blue;
    int Brightness;
} ApiConfigurableOptions;

class QloxApi
{
public:
    static void setup(RTC_DS3231 *rtc);
    static ApiConfigurableOptions getConfigurableOptions();
    static AsyncWebServer server;

private:
    static SemaphoreHandle_t mutex;
    static Preferences preferences;
    static void runWebServer(void *parameters);
    static void persistConfigurableOptions(ApiConfigurableOptions options);
    static void getBrightness();
    static void getRGB();
};