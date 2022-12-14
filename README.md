# mqtt-usb-led
A simple ESP8266 firmware for controlling USB-powered LED lamps via MQTT (e.g. through Home Assistant). 

## Preamble
This project provides you with a simple **IoT interface** for LED lamps powered by 5 volt direct current (like via USB). It is meant to bridge the gap between your Smart Home intfrastructure and your legacy devices in a **non-invasive** way by building an in-line power limiter.

## Requirements
### Hardware
A dedicated device must be put together to be used with the piece of software. The following are key requirements for your circut:
- A development board supported by the **Arduino SDK** (recommendation: **WEMOS LOLIN D1 Mini** or other board incorporating an **ESP8266** microcontroller)
- A **MOSFET** for **3.3V logic-level PWM** control of a **5V** current (recommendation: **IRFZ44N**)
- 5V power input (recommendation: **Female USB C port**) **NOTE:** Do **not** use the USB port built into your board!
- 5V power output (recommendation: **Female USB A port**) **NOTE:** This is where you will plug in your LED Lamp.

<img src="https://user-images.githubusercontent.com/40141286/195949428-fd9daea4-3042-412d-99e7-db9a311f3867.png" width=400px height=400px>

### Compilation
The following Arduino SDK libraries must be included for a successful compilation:
- knolleary/PubSubClient

### Infrastructure
This project is meant to be incorporated into an IoT network. The following components are required for this element to function:
- An MQTT broker (recommendation: [Eclipse Mosquitto](https://mosquitto.org/))
- An MQTT client and a front-end user interface (recommendation: [Home Assistant](https://www.home-assistant.io/))

## Configuration
### config.h
The **config.h** file must be edited to configure the following:
- Board model
- WiFi credentals
- Static IP address
- MQTT credentials
- MQTT base topic
- MOSFET gate pin
- Brightness transition speed

Additionally the following can be set to enable automatic integration with Home Assistant:
- MQTT discovery topic
- Device name and ID

### Home Assistant
The easiest way to enable Home Assistant integration is to configure automatic MQTT discovery. This way, you can simply add your device to your dashboard.
