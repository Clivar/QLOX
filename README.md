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
Use the PlatformIO vscode extension and install the ESP32 usb drivers
