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
float aD, aSum, aMax;

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

void setMotors(float a, float b) {
  Serial.print(a);
  Serial.print(", ");
  Serial.println(b);
  if (fabs(a) < 0.2) {
    analogWrite(PWMA, 0);
  } else {
    analogWrite(
      PWMA,
      map(
        min(100, fabs(a) * 100),
        0, 100,
        80, 255
      )
    );
  }
  if (fabs(b) < 0.2) {
    analogWrite(PWMB, 0);
  } else {
    analogWrite(
      PWMB,
      map(
        min(100, fabs(b) * 100),
        0, 100,
        80, 255
      )
    );
  }

  digitalWrite(AIN1, a >= 0 ? LOW : HIGH);
  digitalWrite(AIN2, a > 0 ? HIGH : LOW);
  digitalWrite(BIN1, b >= 0 ? LOW : HIGH);
  digitalWrite(BIN2, b > 0 ? HIGH : LOW);
}

void loop() {
  ctrl.read(ble);

  if (ctrl.isDirty(BluefruitState::NONE)) {
    float aX = ctrl.getAccelX();
    float aY = ctrl.getAccelY();
    if (aY >= 0) {
      if (aX <= 0) {
        setMotors(aY - aX, max(aX, aY));
      }
    }
  }
}
