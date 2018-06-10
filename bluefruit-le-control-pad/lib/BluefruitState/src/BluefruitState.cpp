#include "Arduino.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "BluefruitState.h"

bool BluefruitState::isButtonPressed(uint8_t num) {
  return bitRead(m_isPressed, num - 1);
}

uint8_t BluefruitState::getButtons() {
  return m_isPressed;
}

bool BluefruitState::isDirty(Data data) {
  if (data == NONE) {
    // This is a cheat for "Is any of the data dirty?"
    return m_isDirty > 0;
  }
  return bitRead(m_isDirty, data);
}

void BluefruitState::resetParsing() {
  m_checksum = m_replyIndex = 0;
  m_data = NONE;
}

void BluefruitState::finalizeParsing() {
  m_checksum = ~m_checksum;
  if (m_checksum != m_replyByte) {
    Serial.print("Checksum mismatch. Computed: ");
    Serial.print(m_checksum);
    Serial.print(" Expected: ");
    Serial.println(m_replyByte);
    resetParsing();
    return;
  }

  switch (m_data) {
    case BUTTONS:
      if (m_buf[1] - '0') {
        bitSet(m_isPressed, m_buf[0] - '0' - 1);
      } else {
        bitClear(m_isPressed, m_buf[0] - '0' - 1);
      }
      bitSet(m_isDirty, BUTTONS);
      break;
    case ACCELEROMETER:
      memcpy(&m_accelX, &m_buf[0], 4);
      memcpy(&m_accelY, &m_buf[4], 4);
      memcpy(&m_accelZ, &m_buf[8], 4);
      bitSet(m_isDirty, ACCELEROMETER);
      break;
    case GYRO:
      memcpy(&m_gyroX, &m_buf[0], 4);
      memcpy(&m_gyroY, &m_buf[4], 4);
      memcpy(&m_gyroZ, &m_buf[8], 4);
      bitSet(m_isDirty, GYRO);
      break;
    case QUATERNIONS:
      memcpy(&m_quatX, &m_buf[0], 4);
      memcpy(&m_quatY, &m_buf[4], 4);
      memcpy(&m_quatZ, &m_buf[8], 4);
      memcpy(&m_quatW, &m_buf[12], 4);
      bitSet(m_isDirty, QUATERNIONS);
      break;
    case JOY:
      memcpy(&m_aJoyX, &m_buf[0], 4);
      memcpy(&m_aJoyY, &m_buf[4], 4);
      memcpy(&m_bJoyX, &m_buf[8], 4);
      memcpy(&m_bJoyY, &m_buf[12], 4);
      bitSet(m_isDirty, JOY);
      break;
    default:
      resetParsing();
      return;
  }

  resetParsing();
  return;
}

uint8_t BluefruitState::read(Adafruit_BluefruitLE_SPI &ble) {
  m_isDirty = 0;

  if (!ble.available()) {
    return 0;
  }

  m_replyByte = ble.read();

  // Wait for a bang ! character that begins the packet.
  if (m_replyIndex == 0) {
    if (m_replyByte == '!') {
      m_replyIndex++;
      m_checksum += m_replyByte;
    }
    return m_replyByte;
  }

  // See what the data type is.
  if (m_replyIndex == 1) {
    switch (m_replyByte) {
      case 'B':
        m_data = BUTTONS;
        break;
      case 'A':
        m_data = ACCELEROMETER;
        break;
      case 'G':
        m_data = GYRO;
        break;
      case 'Q':
        m_data = QUATERNIONS;
        break;
      case 'J':
        m_data = JOY;
        break;
      default:
        resetParsing();
        return m_replyByte;
    }

    memset(m_buf, 0, 12);
    m_replyIndex++;
    m_checksum += m_replyByte;
    return m_replyByte;
  }

  // See if we've reached the checksum and can finalize the parsing.
  if (
    (m_replyIndex == 4 && m_data == BUTTONS) ||
    (m_replyIndex == 14 && m_data == ACCELEROMETER) ||
    (m_replyIndex == 14 && m_data == GYRO) ||
    (m_replyIndex == 18 && m_data == QUATERNIONS) ||
    (m_replyIndex == 18 && m_data == JOY)
  ) {
    finalizeParsing();
    return m_replyByte;
  }

  // Continue assigning to the buffer.
  m_buf[m_replyIndex - 2] = m_replyByte;
  m_replyIndex++;
  m_checksum += m_replyByte;

  // Pass on the reply byte because if another application wants it then we only
  // had the one read() call.
  return m_replyByte;
}

void BluefruitState::printButtons() {
  for (uint8_t i = 1; i <= 8; i++) {
    Serial.print(isButtonPressed(i));
    if (i < 8) {
      Serial.print(" ");
    }
  }
  Serial.println();
}

void BluefruitState::printAccel() {
  Serial.print("X: ");
  Serial.print(m_accelX);
  Serial.print(" Y: ");
  Serial.print(m_accelY);
  Serial.print(" Z: ");
  Serial.println(m_accelZ);
}

void BluefruitState::printGyro() {
  Serial.print("X: ");
  Serial.print(m_gyroX);
  Serial.print(" Y: ");
  Serial.print(m_gyroY);
  Serial.print(" Z: ");
  Serial.println(m_gyroZ);
}

void BluefruitState::printQuat() {
  Serial.print("X: ");
  Serial.print(m_quatX);
  Serial.print(" Y: ");
  Serial.print(m_quatY);
  Serial.print(" Z: ");
  Serial.print(m_quatZ);
  Serial.print(" W: ");
  Serial.println(m_quatW);
}

void BluefruitState::printJoy() {
  Serial.print(m_aJoyY);
  Serial.print(" ");
  Serial.println(m_bJoyY);
}

#define GETTER(member, func) float BluefruitState::func() { return member; }

GETTER(m_accelX, getAccelX)
GETTER(m_accelY, getAccelY)
GETTER(m_accelZ, getAccelZ)
GETTER(m_gyroX, getGyroX)
GETTER(m_gyroY, getGyroY)
GETTER(m_gyroZ, getGyroZ)
GETTER(m_quatX, getQuatX)
GETTER(m_quatY, getQuatY)
GETTER(m_quatZ, getQuatZ)
GETTER(m_quatW, getQuatW)
GETTER(m_aJoyX, getAJoyX)
GETTER(m_aJoyY, getAJoyY)
GETTER(m_bJoyX, getBJoyX)
GETTER(m_bJoyY, getBJoyY)
