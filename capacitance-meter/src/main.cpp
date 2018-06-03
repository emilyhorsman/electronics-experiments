/**
 * Capacitance charge plotter.
 *
 * Based loosely on https://www.arduino.cc/en/Tutorial/CapacitanceMeter
 */
#include "Adafruit_ATParser.h"
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"
#include "Arduino.h"
#include "../../common/BluefruitLEConfig.h"

#define DISCHARGE_PIN   5
#define CHARGE_PIN      6
#define ADC_INPUT       18
#define ADC_UPPER       1023

enum State { CHARGE, DISCHARGE };

State state;
unsigned long startTime;
unsigned long elapsedTime;
unsigned long tick;
int analogValue;

Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

void chargeCapacitor();
void dischargeCapacitor();

void setup() {
  pinMode(CHARGE_PIN, OUTPUT);
  digitalWrite(CHARGE_PIN, LOW);

  Serial.begin(115200);
  Serial.println("Setup complete! Charge pin LOW.");
  if (!ble.begin(true)) {
    Serial.println("Could not find on-board Bluefruit module.");
    while(1);
  }
  ble.echo(false);

  Serial.println("Requesting info:");
  ble.info();
  ble.verbose(false);

  while (!ble.isConnected()) {
    Serial.println("Waiting for BT connection.");
    delay(500);
  }

  // This is required for the Plotter to work.
  ble.setMode(BLUEFRUIT_MODE_DATA);

  Serial.println("Off we go!");
  tick = startTime = millis();
  state = CHARGE;
  chargeCapacitor();
}

// Re-writen from Arduino example to be more like a state machine. I'm not sure
// if that's actually a good idea or not.
void loop() {
  analogValue = analogRead(ADC_INPUT);
  elapsedTime = millis() - startTime;

  if (millis() - tick > 25) {
    ble.println(analogValue);
    Serial.print(elapsedTime);
    Serial.print(',');
    Serial.println(analogValue);
    tick = millis();
  }

  if (state == CHARGE && analogValue >= 1023) {
    state = DISCHARGE;
    dischargeCapacitor();
  } else if (state == DISCHARGE && analogValue < 1) {
    state = CHARGE;
    chargeCapacitor();
  }
}

void chargeCapacitor() {
  // High impedance, no demand on circuit (don't discharge the cap while
  // charging).
  pinMode(DISCHARGE_PIN, INPUT);
  digitalWrite(CHARGE_PIN, HIGH);
}

void dischargeCapacitor() {
  digitalWrite(CHARGE_PIN, LOW);
  pinMode(DISCHARGE_PIN, OUTPUT);  // Low impedance, acts as a sink.
  digitalWrite(DISCHARGE_PIN, LOW);
}
