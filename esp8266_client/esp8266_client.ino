#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>
#include "secrets.h" // Подключаем наш скрытый файл

hd44780_I2Cexp lcd;
WiFiClient espClient;
PubSubClient mqttClient(espClient);

const char* mqtt_topic = "home/orangepi/temp";

void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Orange Pi Temp:");
  lcd.setCursor(0, 1);
  lcd.print(message);
  lcd.print(" C");
}

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}

void reconnect() {
  while (!mqttClient.connected()) {
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (mqttClient.connect(clientId.c_str())) {
      mqttClient.subscribe(mqtt_topic);
    } else {
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  lcd.begin(16, 2);
  lcd.backlight();
  setup_wifi();
  mqttClient.setServer(mqtt_server, 1883);
  mqttClient.setCallback(callback);
}

void loop() {
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();
}
