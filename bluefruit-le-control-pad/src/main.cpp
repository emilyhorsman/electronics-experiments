#include "Adafruit_ATParser.h"
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"
#include "Arduino.h"
#include "../../common/BluefruitLEConfig.h"
#include <BluefruitState.h>

Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);
BluefruitState ctrl;
unsigned long int t;

void setup() {
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
  if (ctrl.isDirty(BluefruitState::ACCELEROMETER)) {
    ctrl.printAccel();
  }

  if (ctrl.isDirty(BluefruitState::GYRO)) {
    ctrl.printGyro();
  }

  if (ctrl.isDirty(BluefruitState::BUTTONS)) {
    ctrl.printButtons();
  }
}
