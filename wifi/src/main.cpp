#include "Arduino.h"
#include "Config.h"
#include <ESP8266WiFi.h>

void setup() {
  pinMode(0, OUTPUT);
  digitalWrite(0, HIGH);

  Serial.begin(115200);
  delay(100);

  Serial.println("Connecting");
  WiFi.disconnect(false);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (true) {
    if (WiFi.status() == WL_CONNECTED) {
      break;
    } else if (WiFi.status() == WL_NO_SSID_AVAIL) {
      Serial.println("Cannot reach configured SSID.");
      break;
    } else if (WiFi.status() == WL_CONNECT_FAILED) {
      Serial.println("Connection failed, password is likely incorrect.");
      break;
    } else if (WiFi.status() == WL_DISCONNECTED) {
      Serial.println("Not in station mode.");
      break;
    } else {
      Serial.print(".");
      delay(500);
    }
  }
  Serial.println();
}

void loop() {
  Serial.println(WiFi.localIP());
  delay(1000);
}
