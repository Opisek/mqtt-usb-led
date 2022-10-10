#include <Arduino.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <string>
#include <config.h>

void mqttCallback(char* topic, byte* payload, unsigned int length);
void connectWifi();

WiFiClient wifiClient;
PubSubClient mqttClient(MQTT_ADDRESS, MQTT_PORT, mqttCallback, wifiClient);

String mqttName;
int brightness;
int lastBrightness;
float currentBrightness;
long lastMillis;

void setup() {
  //Serial.begin(115200);
  pinMode (LAMP_PIN, OUTPUT);

  IPAddress lanAddress, lanGateway, lanSubnet;
  lanAddress.fromString(LAN_ADDRESS);
  lanGateway.fromString(LAN_GATEWAY);
  lanSubnet.fromString(LAN_SUBNET);
  WiFi.config(lanAddress, lanGateway, lanSubnet);
  WiFi.mode(WIFI_STA);

  mqttName = BOARD "-" MQTT_USERNAME "-";
  uint8_t mac[6];
  WiFi.macAddress(mac);
  for (int i = 0; i < 6; ++i) mqttName += String(mac[i], 16);

  brightness = 0;
  lastBrightness = 0;
  currentBrightness = 0;
  lastMillis = millis();
  analogWrite(LAMP_PIN, 0);
}

void loop() {
  long currentMillis = millis();
  long deltaTime = currentMillis - lastMillis;
  if (deltaTime < 0) deltaTime = 0; // rollover protection
  float change = deltaTime * TRANSITION_SPEED;
  lastMillis = currentMillis;
  if (brightness != currentBrightness) {
    if (abs(brightness - currentBrightness) <= change) currentBrightness = brightness;
    else currentBrightness += change * ((brightness > currentBrightness) - .5f) * 2;
    analogWrite(LAMP_PIN, currentBrightness);
  }

  if (WiFi.status() != WL_CONNECTED) { // connect wifi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) delay(100);
  } else if (!mqttClient.connected()) { // connect mqtt
    if (mqttClient.connect((char*)mqttName.c_str(), MQTT_USERNAME, MQTT_PASSWORD, MQTT_BASE "available", 0, true, "offline")) {
      mqttClient.subscribe(MQTT_BASE "power");
      mqttClient.subscribe(MQTT_BASE "brightness");
      mqttClient.publish(MQTT_BASE "available", "online", true);
      std::string payload = "{\"name\": \"" DEVICE_NAME "\",\"unique_id\":\"" DEVICE_ID "\",\"command_topic\":\"" MQTT_BASE "power\",\"state_topic\":\"" MQTT_BASE "power\",\"payload_on\":\"1\",\"payload_off\":\"0\",\"availability_topic\":\"" MQTT_BASE "available\",\"brightness_state_topic\":\"" MQTT_BASE "brightness\",\"brightness_command_topic\":\"" MQTT_BASE "brightness\",\"payload_on\":255,\"payload_off\":0,\"brightness_scale\":255,\"retain\":true}";
      mqttClient.publish_P(MQTT_DISCOVERY, (uint8_t*)payload.c_str(), payload.length(), true);
    }
  } else mqttClient.loop(); // loop mqtt
  delay(30);
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  int level = 0;
  for (unsigned int i = 0; i < length; ++i) {
    level *= 10;
    level += payload[i] - '0';
  }
  Serial.write(topic);
  if (std::strcmp(topic, MQTT_BASE "power") == 0) {
    brightness = lastBrightness *  (level != 0);
  } else {
    brightness = level;
    lastBrightness = level;
  } 
}