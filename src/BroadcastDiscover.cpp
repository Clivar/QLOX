
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
                         int length = min(magicString.length(), packet.length());
                         char buf[length+1];
                         buf[length] = '\0';
                         for (int i = 0; i < length; i++)
                         {
                             buf[i] = (char)*(packet.data() + i);
                         }

                         if (strcmp((const char *)&buf, magicString.c_str()) == 0)
                         {
                             Serial.print("Replying to: ");
                             Serial.println(packet.remoteIP());
                             packet.printf(localIp.c_str());
                         } });
    }
}