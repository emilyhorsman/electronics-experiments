#ifndef _BLUEFRUIT_STATE_H_
#define _BLUEFRUIT_STATE_H_

class BluefruitState {
private:
  enum Data { NONE, BUTTONS, ACCELEROMETER };
  bool m_isPressed[8];
  bool m_isDirty;
  float m_accelX;
  float m_accelY;
  float m_accelZ;

  bool m_isValidReply;
  Data m_data;
  uint8_t m_replyByte;
  uint8_t m_replyIndex;
  uint8_t m_checksum;
  uint8_t m_buttonNum;
  bool m_buttonVal;
  uint8_t m_accelBuf[12];

  void resetParsing();

public:
  bool isDirty();
  bool isButtonPressed(uint8_t num);
  uint8_t read(Adafruit_BluefruitLE_SPI &ble);
  void printControlPad();
  void printAccel();
  float getAccelX();
  float getAccelY();
  float getAccelZ();
};

#endif
