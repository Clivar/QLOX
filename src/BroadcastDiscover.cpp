
#include <Arduino.h>
#include <Preferences.h>
#include "AsyncUDP.h"

#define UDP_MAGICSTRING "HELLOQLOX"

AsyncUDP udp;

void listenForBroadcast(String localIp)
{
    if (udp.listen(49999))
    {
        Serial.println("UDP connected");
        udp.onPacket([localIp](AsyncUDPPacket packet)
                     {
                         Serial.print("Got packet");

                         String magicString = UDP_MAGICSTRING;
                         int length = magicString.length() + 1;
                         char buf[length];
                         for (int i = 0; i < length; i++)
                         {
                             buf[i] = (char)*(packet.data() + i);
                         }
                         const char *request = (const char *)&buf;
                         Serial.println(request);
                         if (strcmp(request, magicString.c_str()) == 0)
                         {
                             Serial.print("Replying to: ");
                             Serial.println(packet.remoteIP());
                             packet.printf(localIp.c_str());
                         } });
    }
}