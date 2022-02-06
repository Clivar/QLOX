#include <Arduino.h>
#include <Preferences.h>
#include "WiFi.h"
#include "AsyncUDP.h"
#include <algorithm>

#include "esp_system.h"
#include "esp_log.h"
#include "esp_event.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

#define DEFAULT_SSID "QLOX"
#define DEFAULT_PASSWORD "QLOXqlox"
#define HOSTNAME "QLOX"

#define UDP_MAGICSTRING "HELLOQLOX"

Preferences preferences;
AsyncUDP udp;

String ssid;
String pass;

void udpListen()
{
  while (1)
  {

    sockaddr_in _send_addr;
    _send_addr.sin_family = AF_INET,
    _send_addr.sin_port = htons(5000);
    // also tried local broadcast 192.168.4.255
    _send_addr.sin_addr.s_addr = inet_addr("255.255.255.255");
    _send_addr.sin_len = sizeof(_send_addr);

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    int broadcast = 1;
    //   This option is needed on the socket in order to be able to receive broadcast messages
    //   If not set the receiver will not receive broadcast messages in the local network.
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0)
    {
      Serial.print("Failed setting broadcast option: ");
      Serial.println(errno);
      closesocket(sock);
      continue;
    }

    Serial.println("Setopt");

    struct sockaddr_in Recv_addr;
    struct sockaddr_in Sender_addr;
    int len = sizeof(struct sockaddr_in);
    char recvbuff[50];
    int recvbufflen = 50;
    char sendMSG[] = "Broadcast message from READER";
    Recv_addr.sin_family = AF_INET;
    Recv_addr.sin_port = htons(49999);
    Recv_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (sockaddr *)&Recv_addr, sizeof(Recv_addr)) < 0)
    {
      Serial.println("Failed to bind");
      closesocket(sock);
      continue;
    }
    Serial.println("Binded");

    recvfrom(sock, recvbuff, recvbufflen, 0, (sockaddr *)&Sender_addr, (socklen_t *)&len);
    Serial.print("Received message: ");
    Serial.print(recvbuff);
    
    if (sendto(sock, sendMSG, strlen(sendMSG) + 1, 0, (sockaddr *)&Sender_addr, sizeof(Sender_addr)) < 0)
    {
      Serial.println("Failed to receive message");
      closesocket(sock);
      continue;
    }
    closesocket(sock);
  }
}

bool tryGetSettings()
{
  preferences.begin("api", true);
  ssid = preferences.getString("ssid", "Van Dooren Netwerk");
  pass = preferences.getString("password", "t58183706172068");
  preferences.end();

  return strcmp(ssid.c_str(), "") && strcmp(pass.c_str(), "");
}

/*
* WiFi Events
0  SYSTEM_EVENT_WIFI_READY               < ESP32 WiFi ready
1  SYSTEM_EVENT_SCAN_DONE                < ESP32 finish scanning AP
2  SYSTEM_EVENT_STA_START                < ESP32 station start
3  SYSTEM_EVENT_STA_STOP                 < ESP32 station stop
4  SYSTEM_EVENT_STA_CONNECTED            < ESP32 station connected to AP
5  SYSTEM_EVENT_STA_DISCONNECTED         < ESP32 station disconnected from AP
6  SYSTEM_EVENT_STA_AUTHMODE_CHANGE      < the auth mode of AP connected by ESP32 station changed
7  SYSTEM_EVENT_STA_GOT_IP               < ESP32 station got IP from connected AP
8  SYSTEM_EVENT_STA_LOST_IP              < ESP32 station lost IP and the IP is reset to 0
9  SYSTEM_EVENT_STA_WPS_ER_SUCCESS       < ESP32 station wps succeeds in enrollee mode
10 SYSTEM_EVENT_STA_WPS_ER_FAILED        < ESP32 station wps fails in enrollee mode
11 SYSTEM_EVENT_STA_WPS_ER_TIMEOUT       < ESP32 station wps timeout in enrollee mode
12 SYSTEM_EVENT_STA_WPS_ER_PIN           < ESP32 station wps pin code in enrollee mode
13 SYSTEM_EVENT_AP_START                 < ESP32 soft-AP start
14 SYSTEM_EVENT_AP_STOP                  < ESP32 soft-AP stop
15 SYSTEM_EVENT_AP_STACONNECTED          < a station connected to ESP32 soft-AP
16 SYSTEM_EVENT_AP_STADISCONNECTED       < a station disconnected from ESP32 soft-AP
17 SYSTEM_EVENT_AP_STAIPASSIGNED         < ESP32 soft-AP assign an IP to a connected station
18 SYSTEM_EVENT_AP_PROBEREQRECVED        < Receive probe request packet in soft-AP interface
19 SYSTEM_EVENT_GOT_IP6                  < ESP32 station or ap or ethernet interface v6IP addr is preferred
20 SYSTEM_EVENT_ETH_START                < ESP32 ethernet start
21 SYSTEM_EVENT_ETH_STOP                 < ESP32 ethernet stop
22 SYSTEM_EVENT_ETH_CONNECTED            < ESP32 ethernet phy link up
23 SYSTEM_EVENT_ETH_DISCONNECTED         < ESP32 ethernet phy link down
24 SYSTEM_EVENT_ETH_GOT_IP               < ESP32 ethernet got IP from connected AP
25 SYSTEM_EVENT_MAX
*/

void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
  Serial.println("Connected to AP successfully!");
}

void WiFiAPConnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
  Serial.println("Client connected");
}

void WiFiAPDisconnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
  Serial.println("Client disconnected");
}

void WiFiAssignedIP(WiFiEvent_t event, WiFiEventInfo_t info)
{
  Serial.println("Client has been assigned an IP");
}

void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info)
{
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  udpListen();
}

void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
  Serial.println("Disconnected from WiFi access point");
  Serial.print("WiFi lost connection. Reason: ");
  Serial.println(info.disconnected.reason);
  Serial.println("Trying to Reconnect");
  WiFi.begin(ssid.c_str(), pass.c_str());
}

void setup()
{
  Serial.begin(9600);
  randomSeed(analogRead(0));

  WiFi.onEvent(WiFiStationConnected, SYSTEM_EVENT_STA_CONNECTED);
  WiFi.onEvent(WiFiGotIP, SYSTEM_EVENT_STA_GOT_IP);
  WiFi.onEvent(WiFiStationDisconnected, SYSTEM_EVENT_STA_DISCONNECTED);
  WiFi.onEvent(WiFiAPConnected, SYSTEM_EVENT_AP_STACONNECTED);
  WiFi.onEvent(WiFiAPDisconnected, SYSTEM_EVENT_AP_STACONNECTED);
  WiFi.onEvent(WiFiAssignedIP, SYSTEM_EVENT_AP_STAIPASSIGNED);

  String hostName = HOSTNAME;
  if (tryGetSettings())
  {
    Serial.println("Initialise as STA");
    WiFi.setHostname(hostName.c_str());
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), pass.c_str());
  }
  else
  {
    Serial.println("Initialise as AP");
    WiFi.config(IPAddress(192, 168, 1, 1), INADDR_NONE, IPAddress(255, 255, 255, 0), INADDR_NONE);
    WiFi.setHostname(hostName.c_str());
    WiFi.mode(WIFI_AP);
    ssid = DEFAULT_SSID;
    pass = DEFAULT_PASSWORD;

    WiFi.softAP(ssid.c_str(), pass.c_str());
  }
}

void loop()
{
  // put your main code here, to run repeatedly:
}
