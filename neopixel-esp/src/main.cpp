#include "Adafruit_NeoPixel.h"
#include "Arduino.h"

Adafruit_NeoPixel ring = Adafruit_NeoPixel(16, 0, NEO_GRB + NEO_KHZ800);
uint8_t led = 0;
uint8_t color = 0;
uint32_t colors[3] = {
  Adafruit_NeoPixel::Color(255, 0, 0),
  Adafruit_NeoPixel::Color(0, 255, 0),
  Adafruit_NeoPixel::Color(0, 0, 255),
};

void setup() {
  ring.begin();
  ring.setBrightness(200);
  ring.show();
}

void loop() {
  for (uint8_t i = led; i != (led + 10) % 16; i = (i + 1) % 16) {
    ring.setPixelColor(i, 0, 0, 0);
  }
  led = (led + 1) % 16;
  uint8_t j = 0;
  for (uint8_t i = led; i != (led + 10) % 16; i = (i + 1) % 16) {
    ring.setPixelColor(i, 0, 0, 255 - ((10 - j) * 25));
    j++;
  }
  ring.show();
  delay(30);
}
