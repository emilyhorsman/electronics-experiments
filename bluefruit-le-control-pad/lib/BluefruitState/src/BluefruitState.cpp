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
  } else if (m_replyIndex == 2) {
    m_isValidReply = true;
    m_buttonNum = m_replyByte - '0';
  } else if (m_replyIndex == 3) {
    m_isValidReply = true;
    m_buttonVal = m_replyByte - '0';
  }

  if (m_replyIndex <= 3 && m_isValidReply) {
    m_replyIndex++;
    m_checksum += m_replyByte;
    return m_replyByte;
  } else if (m_replyIndex == 4) {
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
