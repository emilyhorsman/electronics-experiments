#include "Arduino.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "BluefruitState.h"

bool BluefruitState::isButtonPressed(uint8_t num) {
  return bitRead(m_isPressed, num - 1);
}

uint8_t BluefruitState::getButtons() {
  return m_isPressed;
}

bool BluefruitState::isDirty() {
  return m_isDirty;
}

void BluefruitState::resetParsing() {
  m_checksum = m_replyIndex = 0;
  m_data = NONE;
}

void BluefruitState::finalizeParsing() {
  m_checksum = ~m_checksum;
  if (m_checksum != m_replyByte) {
    Serial.print("Checksum mismatch. Received: ");
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
      Serial.println(m_isPressed);
      break;
    case ACCELEROMETER:
      memcpy(&m_accelX, &m_buf[0], 4);
      memcpy(&m_accelY, &m_buf[4], 4);
      memcpy(&m_accelZ, &m_buf[8], 4);
      break;
    case GYRO:
      memcpy(&m_gyroX, &m_buf[0], 4);
      memcpy(&m_gyroY, &m_buf[4], 4);
      memcpy(&m_gyroZ, &m_buf[8], 4);
      break;
    default:
      resetParsing();
      return;
  }

  m_isDirty = true;
  resetParsing();
  return;
}

uint8_t BluefruitState::read(Adafruit_BluefruitLE_SPI &ble) {
  m_isDirty = false;

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
    (m_replyIndex == 14 && m_data == GYRO)
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

void BluefruitState::printControlPad() {
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

float BluefruitState::getAccelX() {
  return m_accelX;
}

float BluefruitState::getAccelY() {
  return m_accelY;
}

float BluefruitState::getAccelZ() {
  return m_accelZ;
}

float BluefruitState::getGyroX() {
  return m_gyroX;
}

float BluefruitState::getGyroY() {
  return m_gyroY;
}

float BluefruitState::getGyroZ() {
  return m_gyroZ;
}
