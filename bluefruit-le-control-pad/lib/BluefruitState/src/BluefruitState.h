#ifndef _BLUEFRUIT_STATE_H_
#define _BLUEFRUIT_STATE_H_

class BluefruitState {
private:
  bool m_isDirty;
  bool m_isPressed[8];
  bool m_isValidReply;
  uint8_t m_replyByte;
  uint8_t m_replyIndex;
  uint8_t m_checksum;
  uint8_t m_buttonNum;
  bool m_buttonVal;

  void resetParsing();

public:
  bool isDirty();
  bool isButtonPressed(uint8_t num);
  uint8_t read(Adafruit_BluefruitLE_SPI &ble);
  void printControlPad();
};

#endif
