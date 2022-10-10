#define BOARD "ESP8266"

#define WIFI_SSID "ssid"
#define WIFI_PASSWORD "password"
#define LAN_ADDRESS "10.0.0.3"
#define LAN_GATEWAY "10.0.0.1"
#define LAN_SUBNET "255.255.0.0"

#define MQTT_ADDRESS "10.0.0.2"
#define MQTT_PORT 1883
#define MQTT_USERNAME "username"
#define MQTT_PASSWORD "password"

#define MQTT_DISCOVERY "homeassistant/light/bedroom/bedlamp/config"
#define MQTT_BASE "bedroom/bedlamp/"

#define DEVICE_NAME "Bed Lamp"
#define DEVICE_ID "bedlamp"

#define TRANSITION_SPEED 255.f/1000.f
#define LAMP_PIN 4