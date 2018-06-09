#include "Adafruit_ATParser.h"
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"
#include "Arduino.h"
#include "../../common/BluefruitLEConfig.h"
#include <BluefruitState.h>

#define PWMA  5
#define AIN1  10
#define AIN2  9
#define PWMB  6
#define BIN1  11
#define BIN2  12

Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);
BluefruitState ctrl;
unsigned long int t;
bool isTurning;

void setup() {
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  analogWrite(PWMA, 255);
  analogWrite(PWMB, 255);
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);

  Serial.begin(115200);

  if (!ble.begin(true)) {
    Serial.println("Could not find on-board Bluefruit module.");
    while(1);
  }
  ble.echo(false);
  ble.info();
  ble.verbose(false);

  while (!ble.isConnected()) {
    Serial.println("Waiting for BT connection.");
    delay(500);
  }

  ble.setMode(BLUEFRUIT_MODE_DATA);
}

void loop() {
  ctrl.read(ble);
  if (ctrl.isDirty(BluefruitState::BUTTONS)) {
    if (ctrl.isButtonPressed(5)) {
      analogWrite(PWMA, 255);
      analogWrite(PWMB, 255);
      digitalWrite(AIN1, LOW);
      digitalWrite(AIN2, HIGH);
      digitalWrite(BIN1, LOW);
      digitalWrite(BIN2, HIGH);
    } else if (ctrl.isButtonPressed(6)) {
      isTurning = true;
      t = millis();
      analogWrite(PWMA, 80);
      analogWrite(PWMB, 80);
      digitalWrite(AIN1, HIGH);
      digitalWrite(AIN2, LOW);
      digitalWrite(BIN1, HIGH);
      digitalWrite(BIN2, LOW);
    } else if (ctrl.isButtonPressed(7)) {
      analogWrite(PWMA, 100);
      analogWrite(PWMB, 100);
      digitalWrite(AIN1, LOW);
      digitalWrite(AIN2, HIGH);
      digitalWrite(BIN1, HIGH);
      digitalWrite(BIN2, LOW);
    } else if (ctrl.isButtonPressed(8)) {
      analogWrite(PWMA, 100);
      analogWrite(PWMB, 100);
      digitalWrite(AIN1, HIGH);
      digitalWrite(AIN2, LOW);
      digitalWrite(BIN1, LOW);
      digitalWrite(BIN2, HIGH);
    } else {
      isTurning = false;
      digitalWrite(AIN1, LOW);
      digitalWrite(AIN2, LOW);
      digitalWrite(BIN1, LOW);
      digitalWrite(BIN2, LOW);
    }
  }

  if (isTurning) {
    unsigned long int delta = min(1000, millis() - t);
    analogWrite(PWMA, map(delta, 0, 1000, 80, 255));
    analogWrite(PWMB, map(delta, 0, 1000, 80, 255));
  }
}
