#include "Arduino.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "BluefruitState.h"

bool BluefruitState::isButtonPressed(uint8_t num) {
  return m_isPressed[num - 1];
}

bool BluefruitState::isDirty() {
  return m_isDirty;
}

void BluefruitState::resetParsing() {
  m_checksum = m_replyIndex = 0;
  m_data = NONE;
}

uint8_t BluefruitState::read(Adafruit_BluefruitLE_SPI &ble) {
  m_isDirty = false;

  if (!ble.available()) {
    return 0;
  }

  m_isValidReply = false;
  m_replyByte = ble.read();
  if (m_replyIndex == 0 && m_replyByte == '!') {
    m_isValidReply = true;
  } else if (m_replyIndex == 1 && m_replyByte == 'B') {
    m_isValidReply = true;
    m_data = BUTTONS;
  } else if (m_replyIndex == 1 && m_replyByte == 'A') {
    m_isValidReply = true;
    m_data = ACCELEROMETER;
    memset(m_accelBuf, 0, 12);
  } else if (m_replyIndex == 2 && m_data == BUTTONS) {
    m_isValidReply = true;
    m_buttonNum = m_replyByte - '0';
  } else if (m_replyIndex == 3 && m_data == BUTTONS) {
    m_isValidReply = true;
    m_buttonVal = m_replyByte - '0';
  } else if (m_replyIndex >= 2 && m_replyIndex <= 13 && m_data == ACCELEROMETER) {
    m_isValidReply = true;
    m_accelBuf[m_replyIndex - 2] = m_replyByte;
  }

  if (m_replyIndex <= 1 && m_isValidReply) {
    m_replyIndex++;
    m_checksum += m_replyByte;
    return m_replyByte;
  } else if (m_replyIndex <= 3 && m_data == BUTTONS && m_isValidReply) {
    m_replyIndex++;
    m_checksum += m_replyByte;
    return m_replyByte;
  } else if (m_replyIndex == 4 && m_data == BUTTONS) {
    m_checksum = ~m_checksum;
    if (m_checksum != m_replyByte) {
      Serial.print("Checksum mismatch. Received: ");
      Serial.print(m_checksum);
      Serial.print(" Expected: ");
      Serial.println(m_replyByte);
      return m_replyByte;
    }

    resetParsing();
    m_isPressed[m_buttonNum - 1] = m_buttonVal;
    m_isDirty = true;
  } else if (m_replyIndex <= 13 && m_data == ACCELEROMETER && m_isValidReply) {
    m_replyIndex++;
    m_checksum += m_replyByte;

    return m_replyByte;
  } else if (m_replyIndex == 14 && m_data == ACCELEROMETER) {
    m_checksum = ~m_checksum;
    if (m_checksum != m_replyByte) {
      Serial.print("Checksum mismatch. Received: ");
      Serial.print(m_checksum);
      Serial.print(" Expected: ");
      Serial.println(m_replyByte);
      return m_replyByte;
    }

    m_isDirty = true;
    memcpy(&m_accelX, &m_accelBuf[0], 16);
    memcpy(&m_accelY, &m_accelBuf[4], 16);
    memcpy(&m_accelZ, &m_accelBuf[8], 16);
    resetParsing();
  } else {
    resetParsing();
  }

  // Pass on the reply byte because if another application wants it then we only
  // had the one read() call.
  return m_replyByte;
}

void BluefruitState::printControlPad() {
  for (uint8_t i = 0; i < 8; i++) {
    Serial.print(m_isPressed[i]);
    if (i < 7) {
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

float BluefruitState::getAccelX() {
  return m_accelX;
}

float BluefruitState::getAccelY() {
  return m_accelY;
}

float BluefruitState::getAccelZ() {
  return m_accelZ;
}
