# QLOX

A C++ implementation of a letter matrix clock for an ESP32 microcontroller.

Currently supports the following features:
- Finds a sentence for a given matrix
- Shows the time and date (alternating) in sentences in a particular color
- Broadcasts a WiFi network by default (with the option to link it to an existing WiFi network)
- Responds with its local ip to a specific UDP broadcast
- Provides an API over http to configure the time/date, color, brightness and wifi network

All of this is build with concurrency and stability in mind

## Development
- Use the PlatformIO vscode extension and install the ESP32 usb drivers
- For tests:
    - Download and install the MinGW setup.
    - Make sure you select mingw32-base, mingw32-gcc=g++ and msys-base.
    - Add C:\MinGW\bin to PATH

## Flow
- Connect QLOX to power and wait for the `QLOX` WiFi network to broadcast.
- Use password `QLOXqlox`
- Fire a udp broadcast to port 49999 of the local subnet, the response is the local ip of the device
- Use the localip to talk to the API
- After wifi re-configuration use the udp broadcast to find the new local ip of the device.

## API
**GET or PUT** /api/config
```json
{
    "brightness": 20,
    "red": 255,
    "green": 255,
    "blue": 0
}
```
**GET or PUT** /api/time
```json
{
    "time": "2022-02-09T00:37:21"
}
```
**GET or PUT** /api/wifi
```json
{
    "ssid": "yourwifinetwork",
    "password": "yourpassword"
}
```
NOTE: The password property will always show `**REDACTED**` on GET.  
A successful PUT call will restart the device after 1 second and load the new wifi settings.  
The default `QLOX` network will then no longer be broadcasted. 